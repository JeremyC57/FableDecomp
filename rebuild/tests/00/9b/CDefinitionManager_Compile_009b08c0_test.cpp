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
struct CWideString {
    void* PStringData;
    CWideString();                                       // 0x0099AED0
    ~CWideString();                                      // 0x0099B510
};
struct CCharString {
    void* PStringData;
    CCharString(const char* text, long length = -1);     // 0x0099EBF0
    ~CCharString();                                      // 0x0099EAE0
};
bool __fastcall PathExists(const CWideString& path);     // 0x00999230
enum FileAccessType { FILE_ACCESS_READ = 1, FILE_ACCESS_WRITE = 4 };
struct CASuspendableProcess;
// Layout: engine/CDiskFileWin32.h (vptr 0x0122D06C, Opened +0x14, Pathname +0x18), as in CDefStringTable::LoadTable.
struct CDiskFileWin32 : CBaseClassNonCopyable {
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
struct CFileDataInputStream : CDataInputStream {         // 0x24 bytes: the 0x18-byte CDataInputStream + file, buffer
    unsigned char _body[0xc];
    CFileDataInputStream(CDiskFileWin32* file, unsigned long bufferSize);   // 0x00994700
    ~CFileDataInputStream();                             // 0x00994780
};
struct CMemoryDataOutputStream : CDataOutputStream {      // 0x18 bytes
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
    if (g_bLoadBinaryDefinitions && PathExists(GetCompiledFileName())) {
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
// ---- test harness ----
#include <stdio.h>
#include <string.h>
static char g_log[256];
static void Log(const char* s) { strcat(g_log, s); }
static bool g_exists, g_loadOk;
bool g_bLoadBinaryDefinitions;
bool g_bSaveBinaryDefinitions;
extern "C" double __fastcall GFGetTime() { Log("t"); return 1.0; }
void CBaseClassNonCopyable::v00() {}
CBaseClassNonCopyable::CBaseClassNonCopyable() {}
CBaseClassNonCopyable::~CBaseClassNonCopyable() {}
CWideString::CWideString() : PStringData(0) {}
CWideString::~CWideString() {}
CCharString::CCharString(const char* text, long length) : PStringData((void*)text) { if (length == -1 && text[0] == 0) Log("s"); }
CCharString::~CCharString() { Log("~s"); }
bool __fastcall PathExists(const CWideString&) { Log("E"); return g_exists; }
void CDiskFileWin32::Open(const CWideString&, FileAccessType access, unsigned long flags) { Log(access == FILE_ACCESS_READ && flags == 4 ? "Or" : (access == FILE_ACCESS_WRITE && flags == 4 ? "Ow" : "O?")); Opened = true; }
void CDiskFileWin32::Close() { if (Opened) Log("C"); Opened = false; }
void CDiskFileWin32::Write(const void* data, long size, CASuspendableProcess* process) { Log(data == (const void*)0x1234 && size == 7 && !process ? "W" : "W?"); }
CFileDataInputStream::CFileDataInputStream(CDiskFileWin32* file, unsigned long size) { Log(file->Opened && size == 0x4000 ? "I" : "I?"); }
CFileDataInputStream::~CFileDataInputStream() { Log("~I"); }
CMemoryDataOutputStream::CMemoryDataOutputStream() { Log("M"); }
CMemoryDataOutputStream::~CMemoryDataOutputStream() { Log("~M"); }
unsigned long CMemoryDataOutputStream::GetSize() const { return 7; }
const void* CMemoryDataOutputStream::GetData() const { return (const void*)0x1234; }
namespace NDefinitionManager {
void CDefinitionManager::CheckDependencies(const vector<CWideString>& p) { Log((const void*)&p == (const void*)SymbolPathList ? "D" : "D?"); }
bool CDefinitionManager::LoadBinaryDefinitions(CDataInputStream&) { Log("L"); return g_loadOk; }
void CDefinitionManager::SaveBinaryDefinitions(CDataOutputStream&) { Log("S"); }
void CDefinitionManager::CreateSymbolsFromPathList(const vector<CWideString>& p, bool clear) { Log((const void*)&p == (const void*)SymbolPathList && clear ? "Y" : "Y?"); }
void CDefinitionManager::DoCompilePathList(const vector<CWideString>& p, const CCharString&) { Log((const void*)&p == (const void*)CompilePathList ? "P" : "P?"); }
void CDefinitionManager::RemoveUnusedMemory() { Log("R"); }
}
static int Run(bool load, bool exists, bool ok, bool save, const char* want)
{
    static NDefinitionManager::CDefinitionManager m;
    g_log[0] = 0; g_bLoadBinaryDefinitions = load; g_exists = exists; g_loadOk = ok; g_bSaveBinaryDefinitions = save;
    m.Compile();
    if (strcmp(g_log, want) != 0) { printf("FAIL %s want %s\n", g_log, want); return 1; }
    return 0;
}
int main()
{
    int bad = 0;
    bad += Run(true, true, true, true, "tDtEOrItLt~IC");
    bad += Run(true, true, false, false, "tDtEOrItL~ICtYsP~stR");
    bad += Run(true, false, false, true, "tDtEtYsP~stROwMtSWtC~M");
    bad += Run(false, true, true, false, "tDttYsP~stR");
    if (bad) return 1;
    printf("009b08c0_TEST PASS\n");
    return 0;
}