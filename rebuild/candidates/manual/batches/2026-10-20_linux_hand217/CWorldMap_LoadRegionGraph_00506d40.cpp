// CWorldMap::LoadRegionGraph(const CWideString&) -- load the region connection graph: open "Data\Misc\<name>" if it exists, create the graph object on first use and initialise it from the file's text
// Retail: Fable.exe @ 0x00506D40, 237 bytes, `ret 4`. PDB ?LoadRegionGraph@CWorldMap@@QAEXABVCWideString@@@Z
// Callees: __fastcall NResourceDirectoryNames::GetMiscDir() 0x0041A0A0 (L"Data\\Misc\\" 0x0122F3E8; manifest
// "CUserProfileManager::GetAutoSaveFileName" is a byte twin), __fastcall operator+(const CWideString&, const CWideString&)
// 0x0099BE70, ~CWideString 0x0099B510, __fastcall PathExists(const CWideString&) 0x00999230,
// CDiskFileWin32::CDiskFileWin32(const CWideString&, FileAccessType, unsigned long) 0x0099AD80, operator new 0x00BFEA1A,
// CCountedPointer<CRegionConnectionGraph>::Reset 0x0051CEA0, CCharString::CCharString() 0x0099E4B0,
// CCharString::LoadFromFile(CAFile*, CASuspendableProcess*, long) 0x0099F2E0,
// CRegionConnectionGraph::InitialiseGraph(const CCharString&, const CWorldMap&) 0x00828710, ~CCharString 0x0099EAE0,
// and the inlined virtual ~CDiskFileWin32 (vtable 0x0122D06C, Close 0x0099A920, ~CAFile 0x0099A300).
// Retail offsets: CWorldMap PRegionGraph {Data +0x44, Info +0x48} (PDB +0x54). CRegionConnectionGraph (16 bytes):
// CVectorMap GraphNodes = vector (3 words) + empty comparator + Dirty bool +0x0D.
// Shape: the GetMiscDir temporary and the graph text both live in the dead `name` argument slot; the path is a named
// local; the disk file (28 bytes) is a local whose inline virtual destructor runs after the text's.
#include <stddef.h>   // wchar_t = unsigned short under VC7.1 without /Zc:wchar_t

struct CWideStringData;
struct CWideString {
    CWideStringData* PStringData;
    ~CWideString();                                          // 0x0099B510
};
CWideString __fastcall operator+(const CWideString& left, const CWideString& right);   // 0x0099BE70
bool __fastcall PathExists(const CWideString& path);          // 0x00999230
namespace NResourceDirectoryNames {
    CWideString __fastcall GetMiscDir();                     // 0x0041A0A0
}

enum FileAccessType { FILE_ACCESS_READ = 1, FILE_ACCESS_WRITE = 4 };
struct CAFile {
    virtual ~CAFile();                                       // 0x0099A300
};
struct CDiskFileWin32 : CAFile {                             // vtable 0x0122D06C
    FileAccessType AccessType;                               // +0x04
    long           Position;                                 // +0x08
    long           Length;                                   // +0x0C
    void*          WinFileHandle;                            // +0x10
    bool           Opened;                                   // +0x14
    CWideString    Pathname;                                 // +0x18
    CDiskFileWin32(const CWideString& name, FileAccessType access, unsigned long flags);   // 0x0099AD80
    virtual void Close();                                    // 0x0099A920
    virtual ~CDiskFileWin32() { Close(); }
};

struct CASuspendableProcess;
struct CCharStringData;
struct CCharString {
    CCharStringData* PStringData;
    CCharString();                                           // 0x0099E4B0
    ~CCharString();                                          // 0x0099EAE0
    void LoadFromFile(CAFile* file, CASuspendableProcess* process, long length);   // 0x0099F2E0
};

struct CKeyPairCompareLess { };
struct CVectorMap {
    void*               _M_start;                            // +0x00
    void*               _M_finish;                           // +0x04
    void*               _M_end_of_storage;                   // +0x08
    CKeyPairCompareLess Compare;                             // +0x0C
    bool                Dirty;                               // +0x0D
    CVectorMap() : _M_start(0), _M_finish(0), _M_end_of_storage(0), Dirty(false) { }
};
struct CWorldMap;
struct CRegionConnectionGraph {
    CVectorMap GraphNodes;                                   // +0x00
    void InitialiseGraph(const CCharString& text, const CWorldMap& worldMap);   // 0x00828710
};

struct CCPPointerInfo;
template<class T> struct CCountedPointer {
    T*              Data;
    CCPPointerInfo* Info;
    T* operator->() const { return Data; }
    void Reset(T* data);                                     // 0x0051CEA0 (CCountedPointer<CRegionConnectionGraph>)
};

struct CWorldMap {
    unsigned char                           _pad00[0x44];
    CCountedPointer<CRegionConnectionGraph> PRegionGraph;    // +0x44 (PDB +0x54)
    void LoadRegionGraph(const CWideString& name);
};

void CWorldMap::LoadRegionGraph(const CWideString& name)
{
    CWideString path = NResourceDirectoryNames::GetMiscDir() + name;
    if (PathExists(path))
    {
        CDiskFileWin32 file(path, FILE_ACCESS_READ, 2);
        if (!PRegionGraph.Data)
            PRegionGraph.Reset(new CRegionConnectionGraph);
        CCharString text;
        text.LoadFromFile(&file, 0, -1);
        PRegionGraph->InitialiseGraph(text, *this);
    }
}
