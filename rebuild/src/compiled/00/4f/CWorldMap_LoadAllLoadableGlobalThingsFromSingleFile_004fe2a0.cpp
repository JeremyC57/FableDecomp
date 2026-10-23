// CWorldMap::LoadAllLoadableGlobalThingsFromSingleFile(const CWideString& filename) -- read the compiled global-things script and, for every "NEWMAP <n>" ... "ENDMAP" block, hand the block's text as a memory stream to the thing manager for map n
// Leaf: ?LoadAllLoadableGlobalThingsFromSingleFile@CWorldMap@
// Retail: Fable.exe (TLC, VC7.1) @ 0x004FE2A0, 614 bytes, `ret 4` (FableWin.pdb
//   ?LoadAllLoadableGlobalThingsFromSingleFile@CWorldMap@@IAEXABVCWideString@@@Z).
// Callees: CCharString() 0x0099E4B0, CCharString(const char*, long) 0x0099EBF0, ~CCharString 0x0099EAE0,
//   CDiskFileWin32(path, 1, 2) 0x0099AD80 (inline dtor: Close 0x0099A920, ~CWideString 0x0099B510, ~CBase 0x0099A300),
//   CCharString::LoadFromFile 0x0099F2E0, CStringParser(const CCharString&, callback, const CCharString&, long) 0x00404720
//   (inline dtor: ~list 0x004046B0, ~CBaseClass 0x0099A2E0), CStringParser::FindKeyword 0x009B9BA0 / ReadAsInteger
//   0x009BA540 / ReadAsStringUntilString 0x009BA330, DisplayProgress 0x009E9F40 (__fastcall), operator new 0x00BFEA1A,
//   CBaseClassNonCopyable() 0x0099A2F0, CWideString() 0x0099AED0, the memory input stream's Init(data, length) 0x009E93C0
//   (vtable 0x0122E0FC), counted-pointer deleter 0x0050BE40, CWorld slot 3 (the thing manager) and its
//   LoadGlobalThingBatch(CCountedPointer<CDataInputStream>, long) 0x005223F0, operator delete 0x00BFE9BC.
//   Literals "" 0x0122D70E, "NEWMAP" 0x0124466C, "ENDMAP" 0x01244664, "LoadAllLoadableGlobalThingsFromSingleFile" 0x01244638;
//   the empty-text constant 0x0129AAF4.
// Layout: CStringParser retail 0x28 bytes as the landed DoScriptDataPersist 0x00CB7CB0; CCharString data block
//   {const char* Data; long Length} per this function's inline accessors.
// Shape: the stream's OwnsData flag is a member with its own inline ctor (puts the Name lea after the word stores, as
//   retail); the counted-pointer info comes from `p ? new CCPPointerInfo(deleter, p) : 0`; c_str()/length() are
//   if-return accessors (retail tests the null data block once, non-null arm first).
struct CWideString {
    void* PStringData;
    CWideString();                                     // 0x0099AED0
    ~CWideString();                                    // 0x0099B510
};
struct CAFile;
struct CASuspendableProcess;
struct CCharStringData { const char* Data; long Length; };
extern const char g_EmptyText[];                       // 0x0129AAF4
struct CCharString {
    CCharStringData* PStringData;
    CCharString();                                     // 0x0099E4B0
    CCharString(const char* text, long length = -1);   // 0x0099EBF0
    ~CCharString();                                    // 0x0099EAE0
    void LoadFromFile(CAFile* file, CASuspendableProcess* process, long length);   // 0x0099F2E0
    const char* c_str() const { if (!PStringData) return g_EmptyText; return PStringData->Data; }
    long length() const { if (!PStringData) return 0; return PStringData->Length; }
};
void __fastcall DisplayProgress(const CCharString& text, bool flag, float fraction, bool flag2);   // 0x009E9F40

struct CBaseClass {                                    // the CStringParser root (polymorphic)
    virtual void v00();
    ~CBaseClass();                                     // 0x0099A2E0
};
struct CStringParserCommentList {                      // retail std::list<CStringParserCommentBoundaries>: sentinel only
    void* m_pHead;
    ~CStringParserCommentList();                       // 0x004046B0
};
struct CStringParser : CBaseClass {                    // retail 0x28 bytes
    char*                    String;                   // +0x04
    unsigned long            BaseLineOffset;           // +0x08
    CCharString              Filename;                 // +0x0c
    CCharString              TempCharString;           // +0x10
    CStringParserCommentList Comments;                 // +0x14
    void (*ErrorCallback)(CCharString*);               // +0x18
    long                     CursorPos[3];             // +0x1c CCursorPos
    CStringParser(const CCharString& text, void (*errorCallback)(CCharString*), const CCharString& filename,
                  long baseLineOffset);                // 0x00404720
    bool FindKeyword(const CCharString& keyword);      // 0x009B9BA0
    long ReadAsInteger();                              // 0x009BA540
    CCharString ReadAsStringUntilString(const CCharString& end);   // 0x009BA330
};

struct CBase {
    virtual void v00();
    ~CBase();                                          // 0x0099A300
};
enum FileAccessType { FILE_ACCESS_READ = 1, FILE_ACCESS_WRITE = 4 };
struct CDiskFileWin32 : CBase {                        // vtable 0x0122D06C, 0x1c bytes
    virtual void v00();
    virtual void Open(const CWideString& pathname, FileAccessType access, unsigned long flags);   // slot 1
    virtual void Close();                              // 0x0099A920 (slot 2)
    long        AccessType;
    long        Position;
    long        Length;
    void*       WinFileHandle;
    bool        Opened;
    CWideString Pathname;                              // +0x18
    CDiskFileWin32(const CWideString& pathname, FileAccessType access, unsigned long flags);   // 0x0099AD80
    ~CDiskFileWin32() { Close(); }
};

struct CBaseClassNonCopyable {
    CBaseClassNonCopyable();                           // 0x0099A2F0
    virtual ~CBaseClassNonCopyable();
};
struct CDataInputStream : CBaseClassNonCopyable {
    long f4, f8, fc;
};
struct CMemoryDataInputStream : CDataInputStream {     // vtable 0x0122E0FC, 0x24 bytes
    const char* Data;                                  // +0x10
    long        Size;                                  // +0x14
    long        Position;                              // +0x18
    struct CFlag { bool Value; CFlag() : Value(false) { } } OwnsData;   // +0x1c
    CWideString Name;                                  // +0x20
    CMemoryDataInputStream() : Data(0), Size(0), Position(0) { }
    virtual ~CMemoryDataInputStream();
    void Init(const char* data, long length);          // 0x009E93C0
};
struct CCPPointerInfo {
    long RefCount;
    void (__fastcall* Deleter)(void* object);
    void* Object;
    CCPPointerInfo(void (__fastcall* deleter)(void*), void* object) : RefCount(1), Deleter(deleter), Object(object) { }
};
void __fastcall DeleteDataInputStream(void* object);  // 0x0050BE40
struct CDataInputStreamPointer {                       // CCountedPointer<CDataInputStream>
    CDataInputStream* Data;
    CCPPointerInfo*   Info;
    CDataInputStreamPointer(CDataInputStream* p) : Data(p)
    {
        Info = p ? new CCPPointerInfo(DeleteDataInputStream, p) : 0;
    }
    CDataInputStreamPointer(const CDataInputStreamPointer& other) : Data(other.Data), Info(other.Info)
    {
        if (Info)
            ++Info->RefCount;
    }
    ~CDataInputStreamPointer()
    {
        if (Info && --Info->RefCount == 0) {
            Info->Deleter(Info->Object);
            delete Info;
        }
    }
};
struct CThingManager {
    void LoadGlobalThingBatch(CDataInputStreamPointer stream, long map);   // 0x005223F0
};
struct CWorld {
    virtual void v00(); virtual void v01(); virtual void v02();
    virtual CThingManager* GetThingManager();          // slot 3 (+0xc)
};
struct CWorldMap {
    void* __vftable;
    long f4;
    CWorld* World;                                     // +0x08
    void LoadAllLoadableGlobalThingsFromSingleFile(const CWideString& filename);
};

void CWorldMap::LoadAllLoadableGlobalThingsFromSingleFile(const CWideString& filename)
{
    CCharString text;
    CDiskFileWin32 file(filename, FILE_ACCESS_READ, 2);
    text.LoadFromFile((CAFile*)&file, 0, -1);
    CStringParser parser(text, 0, CCharString(""), 0);
    while (parser.FindKeyword("NEWMAP")) {
        long map = parser.ReadAsInteger();
        CCharString block = parser.ReadAsStringUntilString("ENDMAP");
        DisplayProgress("LoadAllLoadableGlobalThingsFromSingleFile", false, -1.0f, false);
        CMemoryDataInputStream* stream = new CMemoryDataInputStream;
        stream->Init(block.c_str(), block.length());
        CDataInputStreamPointer pointer(stream);
        World->GetThingManager()->LoadGlobalThingBatch(pointer, map);
        parser.FindKeyword("ENDMAP");
    }
}