// 009b08c0 CDefinitionManager::Compile
// Flags: /O2 /Oy /W3
// Leaf: Compile
// Bring the definitions up to date: check the symbol-path dependencies, then try the compiled binary file when
// binary loading is on and the file exists. When that fails, build the symbols from the symbol paths, compile the
// compile paths, drop unused memory, and (when binary saving is on) write the definitions to the compiled file
// through a memory stream.
// Callees: GFGetTime 0x009E1BC0 (__fastcall, double, the result dropped), CheckDependencies 0x009B05F0,
// PathExists 0x00999230 (__fastcall), CBaseClassNonCopyable ctor 0x0099A2F0 / dtor 0x0099A300, CWideString
// ctor 0x0099AED0 / dtor 0x0099B510, CDiskFileWin32::Open 0x0099A6A0 / Close 0x0099A920 / Write 0x0099AA90,
// CFileDataInputStream ctor 0x00994700 / dtor 0x00994780, LoadBinaryDefinitions 0x009AFB90,
// CreateSymbolsFromPathList 0x009AEF60, CCharString(const char*, long) 0x0099EBF0 / dtor 0x0099EAE0,
// DoCompilePathList 0x009AF480, RemoveUnusedMemory 0x009AE5F0, CMemoryDataOutputStream ctor 0x00996AF0 /
// GetSize 0x00994B40 / GetData 0x00995E70 / dtor 0x00996B30, SaveBinaryDefinitions 0x009AF7A0.
// Globals: bytes 0x013CA7D8 (load binary definitions) and 0x0138E188 (save binary definitions), descriptive names.
#include "engine/CDefinitionManager.h"
#include "engine/CDataInputStream.h"
#include "engine/CDataOutputStream.h"
extern "C" double __fastcall GFGetTime();                // 0x009E1BC0
struct CBaseClassNonCopyable {
    virtual void v00();
    CBaseClassNonCopyable();                             // 0x0099A2F0
    ~CBaseClassNonCopyable();                            // 0x0099A300
};
class CWideString { public:
    void* PStringData;
    CWideString();                                       // 0x0099AED0
    ~CWideString();                                      // 0x0099B510
};
class CCharString { public:
    void* PStringData;
    CCharString(const char* text, long length = -1);     // 0x0099EBF0
    ~CCharString();                                      // 0x0099EAE0
};
class CAFile { public: static bool __fastcall PathExists(const CWideString& path); };     // 0x00999230
enum FileAccessType { FILE_ACCESS_READ = 1, FILE_ACCESS_WRITE = 4 };
class CASuspendableProcess;
// Layout: engine/CDiskFileWin32.h (vptr 0x0122D06C, Opened +0x14, Pathname +0x18), as in CDefStringTable::LoadTable.
class CDiskFileWin32 : public CBaseClassNonCopyable { public:
    long        AccessType;                              // +0x04
    long        Position;                                // +0x08
    long        Length;                                  // +0x0c
    void*       WinFileHandle;                           // +0x10
    bool        Opened;                                  // +0x14
    CWideString Pathname;                                // +0x18
    CDiskFileWin32() { Opened = false; }
    virtual ~CDiskFileWin32() { Close(); }
    virtual void Open(const CWideString& pathname, FileAccessType access, unsigned long flags);   // 0x0099A6A0
    virtual void Close();                                // 0x0099A920
    virtual void Write(const void* data, long size, CASuspendableProcess* process);              // 0x0099AA90
};
class CFileDataInputStream : public CDataInputStream { public:         // 0x24 bytes: the 0x18-byte CDataInputStream + file, buffer
    unsigned char _body[0xc];
    CFileDataInputStream(CDiskFileWin32* file, unsigned long bufferSize);   // 0x00994700
    ~CFileDataInputStream();                             // 0x00994780
};
class CMemoryDataOutputStream : public CDataOutputStream { public:      // 0x18 bytes
    unsigned char _body[0x10];
    CMemoryDataOutputStream();                           // 0x00996AF0
    ~CMemoryDataOutputStream();                          // 0x00996B30
    unsigned long GetSize() const;                       // 0x00994B40
    const void* GetData() const;                         // 0x00995E70
};
template <class T> struct vector { T* _M_start; T* _M_finish; T* _M_end_of_storage; };
extern bool g_bLoadBinaryDefinitions;                    // 0x013CA7D8
extern bool g_bSaveBinaryDefinitions;                    // 0x0138E188
namespace NDefinitionManager {
// Layout: engine/CDefinitionManager.h with the retail 12-byte STLport containers (SymbolPathList +0x40,
// CompilePathList +0x4c, CompiledFileName +0xb8; landed destructor 0x009B02E0).
struct CDefinitionManager : ::CDefinitionManager {
    const vector<CWideString>& GetSymbolPathList() const { return *(const vector<CWideString>*)SymbolPathList; }
    const vector<CWideString>& GetCompilePathList() const { return *(const vector<CWideString>*)CompilePathList; }
    const CWideString& GetCompiledFileName() const { return *(const CWideString*)&CompiledFileName_PStringData; }
    void CheckDependencies(const vector<CWideString>& paths);                         // 0x009B05F0
    bool LoadBinaryDefinitions(CDataInputStream& stream);                             // 0x009AFB90
    void SaveBinaryDefinitions(CDataOutputStream& stream);                            // 0x009AF7A0
    void CreateSymbolsFromPathList(const vector<CWideString>& paths, bool clear);     // 0x009AEF60
    void DoCompilePathList(const vector<CWideString>& paths, const CCharString& prefix);   // 0x009AF480
    void RemoveUnusedMemory();                                                        // 0x009AE5F0
    void Compile();
};
void CDefinitionManager::Compile()
{
    GFGetTime();
    CheckDependencies(GetSymbolPathList());
    GFGetTime();
    if (g_bLoadBinaryDefinitions && CAFile::PathExists(GetCompiledFileName())) {
        CDiskFileWin32 file;
        file.Open(GetCompiledFileName(), FILE_ACCESS_READ, 4);
        CFileDataInputStream stream(&file, 0x4000);
        GFGetTime();
        if (LoadBinaryDefinitions(stream)) {
            GFGetTime();
            return;
        }
    }
    GFGetTime();
    CreateSymbolsFromPathList(GetSymbolPathList(), true);
    DoCompilePathList(GetCompilePathList(), "");
    GFGetTime();
    RemoveUnusedMemory();
    if (g_bSaveBinaryDefinitions) {
        CDiskFileWin32 file;
        file.Open(GetCompiledFileName(), FILE_ACCESS_WRITE, 4);
        CMemoryDataOutputStream stream;
        GFGetTime();
        SaveBinaryDefinitions(stream);
        file.Write(stream.GetData(), stream.GetSize(), 0);
        GFGetTime();
        file.Close();
    }
}
}