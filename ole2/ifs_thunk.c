
#include "config.h"

#include <assert.h>
#include <stdlib.h>
#include <stdarg.h>
#include <stdio.h>
#include <string.h>
#include <malloc.h>

#define COBJMACROS
#include "windef.h"
#include "winbase.h"
#include "wingdi.h"
#include "winuser.h"
#include "wownt32.h"
#include "ole2.h"
#include "winerror.h"

#include "wine/winbase16.h"
#include "wine/wingdi16.h"
#include "wine/winuser16.h"
#include "ifs.h"

#include "wine/debug.h"
#include "ifs_thunk.h"

extern interface_entry interfaces[];
extern size_t interfaces_count;
/* FIXME */
interface_32 *interface32_instances[1024];
size_t interface32_instance_size = 1024;
size_t interface32_instance_cur = 0;
interface_16 *interface16_instances[1024];
size_t interface16_instance_size = 1024;
size_t interface16_instance_cur = 0;

#ifdef _DEBUG
#define IFS_GUARD_SIZE 500
#else
#define IFS_GUARD_SIZE 0
#endif
static int iid_cmp(const void *p1, const void *p2)
{
    return memcmp(&((const interface_entry*)p1)->iid, &((const interface_entry*)p2)->iid, sizeof(IID));
}
SEGPTR make_thunk_32(void *funcptr, const char *arguments, const char *name, BOOL ret_32bit, BOOL reg_func, BOOL is_cdecl);
static void register_instance32(interface_32 *i32)
{
    if (interface32_instance_cur >= interface32_instance_size)
        return;
    interface32_instances[interface32_instance_cur++] = i32;
}
static void register_instance16(interface_16 *i16)
{
    if (interface16_instance_cur >= interface16_instance_size)
        return;
    interface16_instances[interface16_instance_cur++] = i16;
}
static void init_interface_entry(interface_entry *e)
{
    size_t i = 0;
    SEGPTR *vtbl16 = e->lpVtbl16;
    while (e->vtbl16[i].func16)
    {
        vtbl16[i] = make_thunk_32(e->vtbl16[i].func16, e->vtbl16[i].args, e->vtbl16[i].name, TRUE, FALSE, TRUE);
        i++;
    }
    e->spVtbl16 = MapLS(e->lpVtbl16);
}
SEGPTR iface32_16(REFIID riid, void *iface32)
{
    interface_entry *result;
    size_t i;
    interface_16 *i16;
    SEGPTR s;
    BOOL is_iunk = IsEqualGUID(&IID_IUnknown, riid); /* FIXME */
    if (!iface32)
    {
        return 0;
    }
    result = (interface_entry*)bsearch(riid, interfaces, interfaces_count, sizeof(interfaces[0]), iid_cmp);
    for (i = 0; i < interface16_instance_size; i++)
    {
        if (interface32_instances[i] && &interface32_instances[i]->lpVtbl == iface32)
        {
            s = interface32_instances[i]->iface16;
            if (is_iunk || !memcmp(interface32_instances[i]->riid, riid, sizeof(IID)))
            {
                TRACE("32-bit interface %p -> %04x:%04x(%.*s)\n", iface32, SELECTOROF(s), OFFSETOF(s), strstr(result->vtbl16[0].name, "::") - result->vtbl16[0].name, result->vtbl16[0].name);
                return s;
            }
            else
            {
                TRACE("32-bit interface %p is not %04x:%04x(%.*s)\n", iface32, SELECTOROF(s), OFFSETOF(s), strstr(result->vtbl16[0].name, "::") - result->vtbl16[0].name, result->vtbl16[0].name);
            }
        }
        if (interface16_instances[i] && interface16_instances[i]->lpVtbl == iface32)
        {
            s = MapLS(&interface16_instances[i]->lpVtbl);
            if (is_iunk || !memcmp(interface16_instances[i]->riid, riid, sizeof(IID)))
            {
                TRACE("32-bit interface %p -> %04x:%04x(%.*s)\n", iface32, SELECTOROF(s), OFFSETOF(s), strstr(result->vtbl16[0].name, "::") - result->vtbl16[0].name, result->vtbl16[0].name);
                return s;
            }
            else
            {
                TRACE("32-bit interface %p is not %04x:%04x(%.*s)\n", iface32, SELECTOROF(s), OFFSETOF(s), strstr(result->vtbl16[0].name, "::") - result->vtbl16[0].name, result->vtbl16[0].name);
            }
        }
    }
    if (!result)
    {
        ERR("unknown interface %s\n", debugstr_guid(riid));
        return 0;
    }
    i16 = (interface_16*)HeapAlloc(GetProcessHeap(), 0, sizeof(interface_16) + IFS_GUARD_SIZE * 2);
    memset(i16, 0xcd, sizeof(interface_16) + IFS_GUARD_SIZE * 2);
    i16 = (interface_16*)((char*)i16 + IFS_GUARD_SIZE);
    if (!result->spVtbl16)
    {
        init_interface_entry(result);
    }
    s = MapLS(&i16->lpVtbl);
    TRACE("32-bit interface %p -> new %04x:%04x(%.*s)\n", iface32, SELECTOROF(s), OFFSETOF(s), strstr(result->vtbl16[0].name, "::") - result->vtbl16[0].name, result->vtbl16[0].name);
    i16->iface32 = iface32;
    i16->lpVtbl = result->spVtbl16;
    i16->riid = &result->iid;
    register_instance16(i16);
    return s;
}
void *iface16_32(REFIID riid, SEGPTR iface16)
{
    interface_entry *result;
    size_t i;
    interface_32 *i32;
    LPVOID piface16 = MapSL(iface16);
    BOOL is_iunk = IsEqualGUID(&IID_IUnknown, riid); /* FIXME */
    if (!iface16)
    {
        return 0;
    }
    result = (interface_entry*)bsearch(riid, interfaces, interfaces_count, sizeof(interfaces[0]), iid_cmp);
    for (i = 0; i < interface32_instance_size; i++)
    {
        if (interface16_instances[i] && (LPVOID)&interface16_instances[i]->lpVtbl == piface16)
        {
            if (is_iunk || !memcmp(interface16_instances[i]->riid, riid, sizeof(IID)))
            {
                TRACE("16-bit interface %04x:%04x -> %p(%.*s)\n", SELECTOROF(iface16), OFFSETOF(iface16), interface16_instances[i]->iface32, strstr(result->vtbl16[0].name, "::") - result->vtbl16[0].name, result->vtbl16[0].name);
                return interface16_instances[i]->iface32;
            }
            else
            {
                TRACE("16-bit interface %04x:%04x is not %p(%.*s)\n", SELECTOROF(iface16), OFFSETOF(iface16), interface16_instances[i]->iface32, strstr(result->vtbl16[0].name, "::") - result->vtbl16[0].name, result->vtbl16[0].name);
            }
        }
        if (interface32_instances[i] && interface32_instances[i]->iface16 == iface16)
        {
            if (is_iunk ||!memcmp(interface32_instances[i]->riid, riid, sizeof(IID)))
            {
                TRACE("16-bit interface %04x:%04x -> %p(%.*s)\n", SELECTOROF(iface16), OFFSETOF(iface16), (void*)&interface32_instances[i]->lpVtbl, strstr(result->vtbl16[0].name, "::") - result->vtbl16[0].name, result->vtbl16[0].name);
                return (void*)&interface32_instances[i]->lpVtbl;
            }
            else
            {
                TRACE("16-bit interface %04x:%04x is not %p(%.*s)\n", SELECTOROF(iface16), OFFSETOF(iface16), (void*)&interface32_instances[i]->lpVtbl, strstr(result->vtbl16[0].name, "::") - result->vtbl16[0].name, result->vtbl16[0].name);
            }
        }
    }
    if (!result)
    {
        ERR("unknown interface %s\n", debugstr_guid(riid));
        return 0;
    }
    i32 = (interface_32*)HeapAlloc(GetProcessHeap(), 0, sizeof(interface_32) + IFS_GUARD_SIZE * 2);
    memset(i32, 0xcd, sizeof(interface_32) + IFS_GUARD_SIZE * 2);
    i32 = (interface_32*)((char*)i32 + IFS_GUARD_SIZE);
    if (!result->spVtbl16)
    {
        init_interface_entry(result);
    }
    TRACE("16-bit interface %04x:%04x -> new %p(%.*s)\n", SELECTOROF(iface16), OFFSETOF(iface16), i32, strstr(result->vtbl16[0].name, "::") - result->vtbl16[0].name, result->vtbl16[0].name);
    i32->iface16 = iface16;
    i32->lpVtbl = result->lpVtbl32;
    i32->riid = &result->iid;
    register_instance32(i32);
    return (void*)&i32->lpVtbl;
}


HRESULT CDECL IOleInPlaceSiteWindowless_16_32_OnDefWindowMessage(SEGPTR This, DWORD args16_msg, DWORD args16_wParam, DWORD args16_lParam, SEGPTR args16_plResult)
{
    FIXME("\n");
    return E_NOTIMPL16;
}
HRESULT STDMETHODCALLTYPE IOleInPlaceSiteWindowless_32_16_OnDefWindowMessage(IOleInPlaceSiteWindowless *This, UINT msg, WPARAM wParam, LPARAM lParam, LRESULT *plResult)
{
    FIXME("\n");
    return E_NOTIMPL;
}

HRESULT STDMETHODCALLTYPE IOleInPlaceObjectWindowless_32_16_OnWindowMessage(IOleInPlaceObjectWindowless *This, UINT msg, WPARAM wParam, LPARAM lParam, LRESULT *plResult)
{
    FIXME("\n");
    return E_NOTIMPL;
}
HRESULT CDECL IOleInPlaceObjectWindowless_16_32_OnWindowMessage(SEGPTR This, DWORD args16_msg, DWORD args16_wParam, DWORD args16_lParam, SEGPTR args16_plResult)
{
    FIXME("\n");
    return E_NOTIMPL16;
}
HRESULT CDECL ISimpleFrameSite_16_32_PreMessageFilter(SEGPTR This, WORD args16_hWnd, DWORD args16_msg, DWORD args16_wp, DWORD args16_lp, SEGPTR args16_plResult, SEGPTR args16_pdwCookie)
{
    FIXME("\n");
    return E_NOTIMPL16;
}
HRESULT STDMETHODCALLTYPE ISimpleFrameSite_32_16_PreMessageFilter(ISimpleFrameSite *This, HWND hWnd, UINT msg, WPARAM wp, LPARAM lp, LRESULT *plResult, DWORD *pdwCookie)
{
    FIXME("\n");
    return E_NOTIMPL;
}
HRESULT CDECL ISimpleFrameSite_16_32_PostMessageFilter(SEGPTR This, WORD args16_hWnd, DWORD args16_msg, DWORD args16_wp, DWORD args16_lp, SEGPTR args16_plResult, DWORD args16_dwCookie)
{
    FIXME("\n");
    return E_NOTIMPL16;
}
HRESULT STDMETHODCALLTYPE ISimpleFrameSite_32_16_PostMessageFilter(ISimpleFrameSite *This, HWND hWnd, UINT msg, WPARAM wp, LPARAM lp, LRESULT *plResult, DWORD dwCookie)
{
    FIXME("\n");
    return E_NOTIMPL;
}

struct hresult_map
{
    HRESULT hresult16;
    HRESULT hresult32;
} hresult_table[] =
{
    { E_UNEXPECTED16, E_UNEXPECTED },
    { E_NOTIMPL16, E_NOTIMPL },
    { E_OUTOFMEMORY16, E_OUTOFMEMORY },
    { E_INVALIDARG16, E_INVALIDARG },
    { E_NOINTERFACE16, E_NOINTERFACE },
    { E_POINTER16, E_POINTER },
    { E_HANDLE16, E_HANDLE },
    { E_ABORT16, E_ABORT },
    { E_FAIL16, E_FAIL },
    { E_ACCESSDENIED16, E_ACCESSDENIED },
};
HRESULT hresult32_16(HRESULT hresult)
{
    int i;
    for (i = 0; i < ARRAYSIZE(hresult_table); i++)
    {
        if (hresult_table[i].hresult32 == hresult)
        {
            TRACE("%08x->%08x\n", hresult, hresult_table[i].hresult16);
            return hresult_table[i].hresult16;
        }
    }
    return hresult;
}

HRESULT hresult16_32(HRESULT hresult)
{
    int i;
    for (i = 0; i < ARRAYSIZE(hresult_table); i++)
    {
        if (hresult_table[i].hresult16 == hresult)
        {
            TRACE("%08x->%08x\n", hresult, hresult_table[i].hresult32);
            return hresult_table[i].hresult32;
        }
    }
    return hresult;
}
/* {F6989118-9D36-4B65-AE0C-0C20886D50F8} */
const IID IID_ISTGMEDIUMRelease = { 0xf6989118, 0x9d36, 0x4b65, { 0xae, 0xc, 0xc, 0x20, 0x88, 0x6d, 0x50, 0xf8 } };

ULONG CDECL ISTGMEDIUMRelease_16_32_Release(SEGPTR This)
{
    ISTGMEDIUMRelease *iface32 = (ISTGMEDIUMRelease*)get_interface32(This);
    STGMEDIUM *ptr = (STGMEDIUM*)get_interface32_ptr(This);
    ULONG result;
    TRACE("(%04x:%04x(%p))\n", SELECTOROF(This), OFFSETOF(This), iface32);
    result = iface32->lpVtbl->Release(iface32);
    if (result == 0)
    {
        TRACE("(%04x:%04x(%p)) free\n", SELECTOROF(This), OFFSETOF(This), iface32);
    }
    return result;
}
ULONG STDMETHODCALLTYPE ISTGMEDIUMRelease_32_16_Release(ISTGMEDIUMRelease *This)
{
    SEGPTR iface16 = get_interface16(This);
    STGMEDIUM16 *ptr = (STGMEDIUM16*)get_interface16_ptr(This);
    ULONG result;
    TRACE("(%p(%04x:%04x))\n", This, SELECTOROF(iface16), OFFSETOF(iface16));
    result = ISTGMEDIUMRelease16_Release(iface16);
    if (result == 0)
    {
        TRACE("(%p(%04x:%04x)) free\n", This, SELECTOROF(iface16), OFFSETOF(iface16));
    }
    return result;
}

ULONG STDMETHODCALLTYPE ISTGMEDIUMRelease_32_16_Release(ISTGMEDIUMRelease *This);
typedef struct
{
    ISTGMEDIUMRelease ISTGMEDIUMRelease_iface;
    LONG ref;
} ISTGMEDIUM_impl;


static inline ISTGMEDIUM_impl *impl_from_ISTGMEDIUMRelease(ISTGMEDIUMRelease *iface)
{
    return CONTAINING_RECORD(iface, ISTGMEDIUM_impl, ISTGMEDIUMRelease_iface);
}

static ULONG WINAPI ISTGMEDIUMRelease_AddRef(ISTGMEDIUMRelease *iface)
{
    ISTGMEDIUM_impl *This = impl_from_ISTGMEDIUMRelease(iface);
    return InterlockedIncrement(&This->ref);
}

static ULONG WINAPI ISTGMEDIUMRelease_Release(ISTGMEDIUMRelease *iface)
{
    ISTGMEDIUM_impl *This = impl_from_ISTGMEDIUMRelease(iface);
    return InterlockedDecrement(&This->ref);
}

static HRESULT WINAPI ISTGMEDIUMRelease_QueryInterface(ISTGMEDIUMRelease *iface,
    REFIID riid,
    void** ppvObject)
{
    *ppvObject = NULL;

    if (IsEqualIID(riid, &IID_ISTGMEDIUMRelease) ||
        IsEqualIID(riid, &IID_IUnknown))
    {
        *ppvObject = iface;
        IUnknown_AddRef(iface);
    }

    return *ppvObject ? S_OK : E_NOINTERFACE;
}

static const ISTGMEDIUMReleaseVtbl ISTGMEDIUMRelease_VTable =
{
    ISTGMEDIUMRelease_QueryInterface, ISTGMEDIUMRelease_AddRef, ISTGMEDIUMRelease_Release
};

void map_formatetc16_32(FORMATETC *a32, const FORMATETC16 *a16)
{
    a32->cfFormat = a16->cfFormat;
    a32->ptd = (DVTARGETDEVICE*)MapSL(a16->ptd);
    a32->dwAspect = a16->dwAspect;
    a32->lindex = a16->lindex;
    a32->tymed = a16->tymed;
}
void map_formatetc32_16(FORMATETC16 *a16, const FORMATETC *a32)
{
    a16->cfFormat = a32->cfFormat;
    a16->ptd = MapLS(a32->ptd);
    a16->dwAspect = a32->dwAspect;
    a16->lindex = a32->lindex;
    a16->tymed = a32->tymed;
}

void map_stgmedium32_16(STGMEDIUM16 *a16, const STGMEDIUM *a32)
{
    IUnknown *punk = a32->pUnkForRelease;
    interface_16 *i16;
    a16->tymed = a32->tymed;
    a16->pUnkForRelease = iface32_16(&IID_ISTGMEDIUMRelease, punk);
    i16 = get_interface32_ptr(a16->pUnkForRelease);
    switch ((TYMED)a32->tymed)
    {
    case TYMED_HGLOBAL:
    {
        GlobalFree(0);
        LPVOID p = GlobalLock(a32->hGlobal);
        SIZE_T size = GlobalSize(a32->hGlobal);
        SEGPTR g16 = GlobalAlloc16(0, size);
        LPVOID p32 = GlobalLock16(g16);
        memcpy(p32, p, GlobalSize(a32->hGlobal));
        WOWGlobalUnlock16(g16);
        a16->hGlobal = g16;
        FIXME("leak %04x(%p)\n", a16->hGlobal, a32->hGlobal);
        break;
    }
    case TYMED_FILE:
        a16->lpszFileName = MapLS(strdupWtoA(a32->lpszFileName));
        break;
    case TYMED_ISTREAM:
        a16->pstm = iface32_16(&IID_IStream, a32->pstm);
        break;
    case TYMED_ISTORAGE:
        a16->pstg = iface32_16(&IID_IStorage, a32->pstg);
        break;
    case TYMED_NULL:
        break;
    case TYMED_GDI:
    case TYMED_MFPICT:
    case TYMED_ENHMF:
    default:
        ERR("unsupported tymed %d\n", a32->tymed);
        break;
    }
}
void map_stgmedium16_32(STGMEDIUM *a32, const STGMEDIUM16 *a16)
{
    a32->tymed = a16->tymed;
    a32->pUnkForRelease = (IUnknown*)iface16_32(&IID_ISTGMEDIUMRelease, a16->pUnkForRelease);
    switch ((TYMED)a32->tymed)
    {
    case TYMED_HGLOBAL:
    {
        SIZE_T size = GlobalSize16(a16->hGlobal);
        LPVOID p16 = GlobalLock16(a16->hGlobal);
        LPVOID p32;
        a32->hGlobal = GlobalAlloc(0, size);
        p32 = GlobalLock(a32->hGlobal);
        memcpy(p32, p16, size);
        GlobalUnlock(a32->hGlobal);
        FIXME("leak %p(%04x)\n", a32->hGlobal, a16->hGlobal);
        break;
    }
    case TYMED_FILE:
        a32->lpszFileName = strdupAtoW(MapSL(a16->lpszFileName));
        break;
    case TYMED_ISTREAM:
        a32->pstm = (IStream*)iface16_32(&IID_IStream, a16->pstm);
        break;
    case TYMED_ISTORAGE:
        a32->pstg = (IStorage*)iface16_32(&IID_IStorage, a16->pstg);
        break;
    case TYMED_NULL:
        break;
    case TYMED_GDI:
    case TYMED_MFPICT:
    case TYMED_ENHMF:
    default:
        ERR("unsupported tymed %d\n", a16->tymed);
        break;
    }
}
