1 pascal OleBuildVersion() OleBuildVersion16
2 pascal OleInitialize(ptr) OleInitialize16
3 pascal OleUninitialize() OleUninitialize16
4 pascal DllGetClassObject(ptr ptr ptr) DllGetClassObject16
#5 WEP
6 stub OLEQUERYLINKFROMDATA
7 stub OLEQUERYCREATEFROMDATA
8 stub OLECREATEFROMDATA
9 stub OLECREATELINKFROMDATA
10 pascal OleCreate(ptr ptr long ptr segptr segptr ptr) OleCreate16
11 stub OLECREATELINK
12 pascal OleLoad(segptr ptr segptr ptr) OleLoad16
13 pascal OleSave(segptr segptr word) OleSave16
14 pascal OleRun(segptr) OleRun16
#15 ___EXPORTEDSTUB
16 pascal -ret16 OleIsRunning(segptr) OleIsRunning16
17 pascal OleLockRunning(segptr word word) OleLockRunning16
18 pascal ReadClassStg(segptr ptr) ReadClassStg16
19 pascal WriteClassStg(segptr ptr) WriteClassStg16
20 pascal ReadClassStm(segptr ptr) ReadClassStm16
21 stub WRITECLASSSTM
22 stub BINDMONIKER
23 stub MKPARSEDISPLAYNAME
24 stub OLESAVETOSTREAM
25 stub OLELOADFROMSTREAM
26 stub CREATEBINDCTX
27 pascal CreateItemMoniker(str str ptr) CreateItemMoniker16
28 pascal CreateFileMoniker(str ptr) CreateFileMoniker16
29 stub CREATEGENERICCOMPOSITE
30 pascal GetRunningObjectTable(long ptr) GetRunningObjectTable16
31 stub OLEGETMALLOC
32 pascal ReleaseStgMedium(ptr) ReleaseStgMedium16
33 stub READSTRINGSTREAM
34 stub WRITESTRINGSTREAM
35 pascal RegisterDragDrop(word segptr) RegisterDragDrop16
36 pascal RevokeDragDrop(word) RevokeDragDrop16
37 pascal DoDragDrop(segptr segptr long ptr) DoDragDrop16
38 pascal CreateOleAdviseHolder(ptr) CreateOleAdviseHolder16
39 pascal CreateDataAdviseHolder(ptr) CreateDataAdviseHolder16
40 pascal -ret16 OleCreateMenuDescriptor(word ptr) OleCreateMenuDescriptor16
41 pascal OleSetMenuDescriptor(word word word ptr ptr) OleSetMenuDescriptor16
42 pascal -ret16 OleDestroyMenuDescriptor(word) OleDestroyMenuDescriptor16
43 stub OPENORCREATESTREAM
44 stub CREATEANTIMONIKER
45 stub CREATEPOINTERMONIKER
46 stub MONIKERRELATIVEPATHTO
47 stub MONIKERCOMMONPREFIXWITH
48 stub ISACCELERATOR
49 pascal OleSetClipboard(segptr) OleSetClipboard16
50 pascal OleGetClipboard(ptr) OleGetClipboard16
51 stub OLEDUPLICATEDATA
52 stub OLEGETICONOFFILE
53 stub OLEGETICONOFCLASS
54 pascal CreateILockBytesOnHGlobal(word word ptr) CreateILockBytesOnHGlobal16
55 stub GETHGLOBALFROMILOCKBYTES
56 pascal -ret16 OleMetafilePictFromIconAndLabel(word str str word) OleMetafilePictFromIconAndLabel16
57 stub GETCLASSFILE
58 stub OLEDRAW
59 pascal OleCreateDefaultHandler(ptr segptr ptr ptr) OleCreateDefaultHandler16
60 stub OLECREATEEMBEDDINGHELPER
61 stub OLECONVERTISTORAGETOOLESTREAMEX
62 stub OLECONVERTOLESTREAMTOISTORAGEEX
63 stub SETDOCUMENTBITSTG
64 stub GETDOCUMENTBITSTG
65 stub WRITEOLESTG
66 stub READOLESTG
67 stub OLECREATEFROMFILE
68 stub OLECREATELINKTOFILE
69 stub CREATEDATACACHE
70 stub OLECONVERTISTORAGETOOLESTREAM
71 stub OLECONVERTOLESTREAMTOISTORAGE
74 pascal ReadFmtUserTypeStg(segptr ptr ptr) ReadFmtUserTypeStg16
75 pascal WriteFmtUserTypeStg(segptr word str) WriteFmtUserTypeStg16
76 pascal -ret16 OleFlushClipboard() OleFlushClipboard16
77 pascal OleIsCurrentClipboard(segptr) OleIsCurrentClipboard16
78 stub OLETRANSLATEACCELERATOR
79 pascal OleDoAutoConvert(ptr ptr) OleDoAutoConvert16
80 pascal OleGetAutoConvert(ptr ptr) OleGetAutoConvert16
81 stub OLESETAUTOCONVERT
82 pascal GetConvertStg(ptr) GetConvertStg16
83 stub SETCONVERTSTG
84 stub CREATESTREAMONHGLOBAL
85 stub GETHGLOBALFROMSTREAM
86 pascal OleSetContainedObject(segptr word) OleSetContainedObject16
87 stub OLENOTEOBJECTVISIBLE
88 stub OLECREATESTATICFROMDATA
89 stub OLEREGGETUSERTYPE
90 stub OLEREGGETMISCSTATUS
91 stub OLEREGENUMFORMATETC
92 stub OLEREGENUMVERBS
93 stub OLEGETENUMFORMATETC
100 stub MAKEDEBUGSTREAM
104 stub DBGLOGOPEN
105 stub DBGLOGCLOSE
106 stub DBGLOGOUTPUTDEBUGSTRING
107 stub DBGLOGWRITE
108 stub DBGLOGTIMESTAMP
109 stub DBGLOGWRITEBANNER
110 stub DBGDUMPOBJECT
111 stub DBGISOBJECTVALID
112 stub DUMPALLOBJECTS
113 stub VALIDATEALLOBJECTS
114 stub DBGDUMPCLASSNAME
115 stub DBGDUMPEXTERNALOBJECT
120 variable _IID_IEnumUnknown(0x00000100 0x00000000 0x000000c0 0x46000000)
121 variable _IID_IEnumString(0x00000101 0x00000000 0x000000c0 0x46000000)
122 variable _IID_IEnumMoniker(0x00000102 0x00000000 0x000000c0 0x46000000)
123 variable _IID_IEnumFORMATETC(0x00000103 0x00000000 0x000000c0 0x46000000)
124 variable _IID_IEnumOLEVERB(0x00000104 0x00000000 0x000000c0 0x46000000)
125 variable _IID_IEnumSTATDATA(0x00000105 0x00000000 0x000000c0 0x46000000)
126 variable _IID_IEnumGeneric(0x00000106 0x00000000 0x000000c0 0x46000000)
127 variable _IID_IEnumHolder(0x00000107 0x00000000 0x000000c0 0x46000000)
128 variable _IID_IEnumCallback(0x00000108 0x00000000 0x000000c0 0x46000000)
129 variable _IID_IPersistStream(0x00000109 0x00000000 0x000000c0 0x46000000)
130 variable _IID_IPersistStorage(0x0000010a 0x00000000 0x000000c0 0x46000000)
131 variable _IID_IPersistFile(0x0000010b 0x00000000 0x000000c0 0x46000000)
132 variable _IID_IPersist(0x0000010c 0x00000000 0x000000c0 0x46000000)
133 variable _IID_IViewObject(0x0000010d 0x00000000 0x000000c0 0x46000000)
134 variable _IID_IDataObject(0x0000010e 0x00000000 0x000000c0 0x46000000)
135 variable _IID_IAdviseSink(0x0000010f 0x00000000 0x000000c0 0x46000000)
136 variable _IID_IDataAdviseHolder(0x00000110 0x00000000 0x000000c0 0x46000000)
137 variable _IID_IOleAdviseHolder(0x00000111 0x00000000 0x000000c0 0x46000000)
138 variable _IID_IOleObject(0x00000112 0x00000000 0x000000c0 0x46000000)
139 variable _IID_IOleInPlaceObject(0x00000113 0x00000000 0x000000c0 0x46000000)
140 variable _IID_IOleWindow(0x00000114 0x00000000 0x000000c0 0x46000000)
141 variable _IID_IOleInPlaceUIWindow(0x00000115 0x00000000 0x000000c0 0x46000000)
142 variable _IID_IOleInPlaceFrame(0x00000116 0x00000000 0x000000c0 0x46000000)
143 variable _IID_IOleInPlaceActiveObject(0x00000117 0x00000000 0x000000c0 0x46000000)
144 variable _IID_IOleClientSite(0x00000118 0x00000000 0x000000c0 0x46000000)
145 variable _IID_IOleInPlaceSite(0x00000119 0x00000000 0x000000c0 0x46000000)
146 variable _IID_IParseDisplayName(0x0000011a 0x00000000 0x000000c0 0x46000000)
147 variable _IID_IOleContainer(0x0000011b 0x00000000 0x000000c0 0x46000000)
148 variable _IID_IOleItemContainer(0x0000011c 0x00000000 0x000000c0 0x46000000)
149 variable _IID_IOleLink(0x0000011d 0x00000000 0x000000c0 0x46000000)
150 variable _IID_IOleCache(0x0000011e 0x00000000 0x000000c0 0x46000000)
151 variable _IID_IOleManager(0x0000011f 0x00000000 0x000000c0 0x46000000)
152 variable _IID_IOlePresObj(0x00000120 0x00000000 0x000000c0 0x46000000)
153 variable _IID_IDropSource(0x00000121 0x00000000 0x000000c0 0x46000000)
154 variable _IID_IDropTarget(0x00000122 0x00000000 0x000000c0 0x46000000)
155 variable _IID_IDebug(0x00000123 0x00000000 0x000000c0 0x46000000)
156 variable _IID_IDebugStream(0x00000124 0x00000000 0x000000c0 0x46000000)
157 variable _IID_IAdviseSink2(0x00000125 0x00000000 0x000000c0 0x46000000)
158 variable _IID_IViewObject2(0x00000127 0x00000000 0x000000c0 0x46000000)
159 variable _IID_IOleCache2(0x00000128 0x00000000 0x000000c0 0x46000000)
160 variable _IID_IOleCacheControl(0x00000129 0x00000000 0x000000c0 0x46000000)
161 variable _IID_IRunnableObject(0x00000126 0x00000000 0x000000c0 0x46000000)

# WINE MemLockBytes implementation.
500 cdecl HGLOBALLockBytesImpl16_QueryInterface(segptr ptr ptr) HGLOBALLockBytesImpl16_QueryInterface
501 cdecl HGLOBALLockBytesImpl16_AddRef(ptr) HGLOBALLockBytesImpl16_AddRef
502 cdecl HGLOBALLockBytesImpl16_Release(ptr) HGLOBALLockBytesImpl16_Release
503 cdecl HGLOBALLockBytesImpl16_ReadAt(ptr int64 ptr long ptr) HGLOBALLockBytesImpl16_ReadAt
504 cdecl HGLOBALLockBytesImpl16_WriteAt(ptr int64 ptr long ptr) HGLOBALLockBytesImpl16_WriteAt
505 cdecl HGLOBALLockBytesImpl16_Flush(ptr) HGLOBALLockBytesImpl16_Flush
506 cdecl HGLOBALLockBytesImpl16_SetSize(ptr int64) HGLOBALLockBytesImpl16_SetSize
507 cdecl HGLOBALLockBytesImpl16_LockRegion(ptr int64 int64 long) HGLOBALLockBytesImpl16_LockRegion
508 cdecl HGLOBALLockBytesImpl16_UnlockRegion(ptr int64 int64 long) HGLOBALLockBytesImpl16_UnlockRegion
509 cdecl HGLOBALLockBytesImpl16_Stat(ptr ptr long) HGLOBALLockBytesImpl16_Stat
