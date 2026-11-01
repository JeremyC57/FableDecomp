# h24_defmgr.py: naming wave 24, family defmgr -- the chunk 691 queue rows around CScriptDefinitionManager::InitAndCompile 0x00CD3F50
# (labels_chunk691): CDefStringTable::SaveTable 0x009D4CE0, CDefinitionManager::Compile 0x009B08C0 / ClearSymbols 0x009ACB10,
# SetGlobalScriptDef 0x00CBF647, NBankFileManager::GetHeaderDir 0x009A76A0 / SymbolHeaderDir 0x013CA7D4, and the callers that named
# them under stand-ins (0x004336BC, 0x00416005, 0x006F9BE0, 0x009A76C0, 0x01224030).
import os, sys
sys.path.insert(0, os.path.dirname(os.path.abspath(__file__)))
import he

# ---- 0x009D4CE0 CDefStringTable::SaveTable(bool) (landed as SaveBinary) ----
he.edit('009d4ce0', 'src', '''// CDefStringTable::SaveBinary(bool lock) -- unless''', '''// CDefStringTable::SaveTable(bool lock) -- unless''')
he.edit('009d4ce0', 'src', '// Leaf: SaveBinary\n', '''// Leaf: ?SaveTable@CDefStringTable@@QAEX_N@Z
// Naming (naming wave 24): Ego_r.pdb / FableWin.pdb ?SaveTable@CDefStringTable@@QAEX_N@Z -- the PDB member order GetString /
//   GetStringAsCharArray / GetStringCRC / ReadTable / LoadTable / SaveTable is retail 0x009D49B0 / 49F0 / 4A20 / 4AB0 / 4C30 / 4CE0
//   (labels_chunk691); its caller CScriptDefinitionManager::InitAndCompile 0x00CD3F50 already binds it. Was landed as SaveBinary.
''')
he.edit('009d4ce0', 'src', '    void SaveBinary(bool lock);\n};\n\nvoid CDefStringTable::SaveBinary(bool lock)', '    void SaveTable(bool lock);\n};\n\nvoid CDefStringTable::SaveTable(bool lock)')
he.edit_all('009d4ce0', 'tst', 'SaveBinary', 'SaveTable')

# ---- 0x009B08C0 CDefinitionManager::Compile (landed on the NDefinitionManager::CDefinitionManager stand-in) ----
A = '009b08c0'
he.edit(A, 'src', '#include "engine/CDefinitionManager.h"\n', '''#define CDefinitionManager CDefinitionManagerLayout
#include "engine/CDefinitionManager.h"
#undef CDefinitionManager
''')
he.edit(A, 'src', '// Globals: bytes 0x013CA7D8', '''// Naming (naming wave 24): Ego_r.pdb / FableWin.pdb ?Compile@CDefinitionManager@@QAEXXZ (labels_chunk691; the caller 0x00CD3F50 binds
//   it). The row defined it on a NDefinitionManager::CDefinitionManager stand-in; now the PDB class itself over the engine header's
//   layout (the rename pattern of the landed CompileString / RecompileAllDefs rows), with its callees at their PDB access (IAE for
//   CheckDependencies / CreateSymbolsFromPathList / DoCompilePathList / RemoveUnusedMemory) on std::vector<CWideString>.
// Globals: bytes 0x013CA7D8''')
he.edit(A, 'src', 'template <class T> struct vector { T* _M_start; T* _M_finish; T* _M_end_of_storage; };\n', '''namespace std {
template <class _Tp> class allocator { };
template <class _Tp, class _Alloc = allocator<_Tp> > class vector { public: _Tp* _M_start; _Tp* _M_finish; _Tp* _M_end_of_storage; };
}
''')
he.edit(A, 'src', '''namespace NDefinitionManager {
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
''', '''// Layout: engine/CDefinitionManager.h with the retail 12-byte STLport containers (SymbolPathList +0x40,
// CompilePathList +0x4c, CompiledFileName +0xb8; landed destructor 0x009B02E0).
class CDefinitionManager : public CDefinitionManagerLayout {
public:
    const std::vector<CWideString>& GetSymbolPathList() const { return *(const std::vector<CWideString>*)SymbolPathList; }
    const std::vector<CWideString>& GetCompilePathList() const { return *(const std::vector<CWideString>*)CompilePathList; }
    const CWideString& GetCompiledFileName() const { return *(const CWideString*)&CompiledFileName_PStringData; }
    bool LoadBinaryDefinitions(CDataInputStream& stream);                             // 0x009AFB90
    void SaveBinaryDefinitions(CDataOutputStream& stream);                            // 0x009AF7A0
    void Compile();
protected:
    void CheckDependencies(const std::vector<CWideString>& paths);                    // 0x009B05F0
    void CreateSymbolsFromPathList(const std::vector<CWideString>& paths, bool clear);     // 0x009AEF60
    void DoCompilePathList(const std::vector<CWideString>& paths, const CCharString& prefix);   // 0x009AF480
    void RemoveUnusedMemory();                                                        // 0x009AE5F0
};
''')
he.edit(A, 'src', '        file.Close();\n    }\n}\n}', '        file.Close();\n    }\n}')

# ---- 0x009ACB10 CDefinitionManager::ClearSymbols (landed as a junk T::Run forwarder) ----
he.put('009acb10', 'src', '''// CDefinitionManager::ClearSymbols() -- empty the symbol map the manager holds through its counted pointer PSymbolMap (+0x58)
// Leaf: ?ClearSymbols@CDefinitionManager@@QAEXXZ
// Retail: Fable.exe @ 0x009ACB10, 8 bytes: `mov ecx,[ecx+58h] ; jmp CSymbolMap::Clear` (VC7.1 tail-calls the void forward).
// Naming (naming wave 24): Ego_r.pdb / FableWin.pdb ?ClearSymbols@CDefinitionManager@@QAEXXZ (labels_chunk691); called by
//   CScriptDefinitionManager::InitAndCompile 0x00CD3F50 and CMainGameComponent::InitDefinitions 0x00416005 right after the compile.
//   The row had landed as a descriptive `T::Run` forwarder under a junk catalog name.
// Callees: CSymbolMap::Clear 0x009E5250 (Ego_r ?Clear@CSymbolMap@@QAEXXZ).
// Layout: engine/CDefinitionManager.h, PSymbolMap_Data +0x58 (the Data word of CCountedPointer<CSymbolMap> PSymbolMap).
class CSymbolMap { public: void Clear(); };                // 0x009E5250
#define CDefinitionManager CDefinitionManagerLayout
#include "engine/CDefinitionManager.h"
#undef CDefinitionManager

class CDefinitionManager : public CDefinitionManagerLayout {
public:
    void ClearSymbols();
};

void CDefinitionManager::ClearSymbols()
{
    PSymbolMap_Data->Clear();
}''')
he.put('009acb10', 'tst', '''// Behaviour test for 0x009ACB10 CDefinitionManager::ClearSymbols(): forwards to Clear() on the symbol map at +0x58.
#include <stdio.h>
class CSymbolMap { public: int cleared; void Clear() { ++cleared; } };
class CDefinitionManager { public: char pad[0x58]; CSymbolMap* PSymbolMap_Data; void ClearSymbols(); };
void CDefinitionManager::ClearSymbols() { PSymbolMap_Data->Clear(); }
int main(void)
{
    CSymbolMap m; m.cleared = 0;
    CDefinitionManager d; d.PSymbolMap_Data = &m;
    d.ClearSymbols(); d.ClearSymbols();
    if (m.cleared != 2) { printf("FAIL_009acb10\\n"); return 1; }
    printf("PASS_009acb10\\n"); return 0;
}''')

# ---- 0x00416005 CMainGameComponent::InitDefinitions: the follow-up is CDefinitionManager::ClearSymbols ----
A = '00416005'
he.edit(A, 'src', '''// if (flag) Get() -> 0x009ACB10 (forwards to its +0x58 member); the global call again; return true.
// The global's class and both non-virtual callees are not identified; their names here are placeholders.''',
'''// if (flag) Get() -> ClearSymbols 0x009ACB10 (CDefinitionManager, Ego_r ?ClearSymbols@CDefinitionManager@@QAEXXZ; naming wave 24,
// labels_chunk691); the global call again; return true. CGameDefinitionManager derives from CDefinitionManager (Ego_r).
// The global's class and its callee are not identified; their names here are placeholders.''')
he.edit(A, 'src', '''class CGameDefinitionManager {
public:
    static CGameDefinitionManager& __fastcall Get();     // 0x0044C6B0
    virtual ~CGameDefinitionManager();
    virtual void _slot01();
    virtual void InitAndCompile();                       // slot 2 (+8)
    void FollowUp_9ACB10();                              // 0x009ACB10
};''', '''class CDefinitionManager {
public:
    virtual ~CDefinitionManager();
    virtual void _slot01();
    virtual void InitAndCompile();                       // slot 2 (+8)
    void ClearSymbols();                                 // 0x009ACB10
};
class CGameDefinitionManager : public CDefinitionManager {
public:
    static CGameDefinitionManager& __fastcall Get();     // 0x0044C6B0
    virtual void InitAndCompile();                       // slot 2 (+8)
};''')
he.edit(A, 'src', 'CGameDefinitionManager::Get().FollowUp_9ACB10();', 'CGameDefinitionManager::Get().ClearSymbols();')
he.edit_all(A, 'tst', 'FollowUp_9ACB10', 'ClearSymbols')

# ---- 0x00CBF647 SetGlobalScriptDef (landed as GFLoadScriptDef_00cbf647 on g_scriptDef) ----
A = '00cbf647'
he.edit(A, 'src', '// GFLoadScriptDef_00cbf647() -- resolve', '// SetGlobalScriptDef() -- resolve')
he.edit(A, 'src', '// Retail: Fable.exe (TLC, VC7.1) function @ 0x00CBF647, length 50 bytes.\n', '''// Leaf: ?SetGlobalScriptDef@@YIXXZ
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00CBF647, length 50 bytes.
// Naming (naming wave 24): Ego_r.pdb / FableWin.pdb ?SetGlobalScriptDef@@YIXXZ (a __fastcall free function: ecx is unused, so the
//   bytes equal the cdecl form; labels_chunk691; the caller 0x00CD3F50 binds it); the def pointer is the ESTABLISHED
//   ?pGlobalScriptDef@@3V?$CDefPointer@$$CBVCScriptDef@@@@A and the lookup FableWin's
//   ??$GetDef@VCScriptDef@@@CDefinitionManager@@QBE_NABVCCharString@@PAV?$CDefPointer@$$CBVCScriptDef@@@@@Z. Was GFLoadScriptDef_00cbf647.
''')
he.edit(A, 'src', '''struct CDefPointer;
extern CDefPointer g_scriptDef;                 // 0x0143E90C

struct CDefinitionManager_Calls {              // engine/CDefinitionManager.h exists but is not used: only its GetDef call is used
    bool GetDef(const CCharString& name, CDefPointer& def);   // 0x00CD2994
};

CDefinitionManager_Calls* GFGetPDefinitionManager();  // 0x00CD3EF0

void GFLoadScriptDef_00cbf647()
{
    CCharString name("SCRIPT_DEF", -1);
    GFGetPDefinitionManager()->GetDef(name, g_scriptDef);
}''', '''class CScriptDef;
template <class T> class CDefPointer;
extern CDefPointer<const CScriptDef> pGlobalScriptDef;  // 0x0143E90C

#define CDefinitionManager CDefinitionManagerLayout
#include "engine/CDefinitionManager.h"
#undef CDefinitionManager
class CDefinitionManager : public CDefinitionManagerLayout {
public:
    template <class T> bool GetDef(const CCharString& name, CDefPointer<const T>* def) const;   // 0x00CD2994 (<CScriptDef>)
};

CDefinitionManager* GFGetPDefinitionManager();          // 0x00CD3EF0

void __fastcall SetGlobalScriptDef()
{
    CCharString name("SCRIPT_DEF", -1);
    GFGetPDefinitionManager()->GetDef<CScriptDef>(name, &pGlobalScriptDef);
}''')

# ---- 0x009A76A0 NBankFileManager::GetHeaderDir (landed as a junk C::Get on g_str_9a76a0) ----
he.put('009a76a0', 'src', '''// NBankFileManager::GetHeaderDir() -- return a copy of the bank-file header directory SymbolHeaderDir
// Leaf: ?GetHeaderDir@NBankFileManager@@YI?AVCWideString@@XZ
// Retail: Fable.exe @ 0x009A76A0, 17 bytes: `push esi ; push 13CA7D4h ; mov esi,ecx ; call CWideString(const CWideString&) ; mov eax,esi ;
//   pop esi ; ret` -- a __fastcall function returning a CWideString by value (the hidden result in ecx).
// Naming (naming wave 24): Ego_r.pdb / FableWin.pdb ?GetHeaderDir@NBankFileManager@@YI?AVCWideString@@XZ (labels_chunk691; the PDB order
//   GetHeaderDir / SetHeaderDir is retail 0x009A76A0 / 0x009A76C0); the global is the PDB's NBankFileManager::SymbolHeaderDir (its
//   dynamic initializer 0x01224030 and the setter 0x009A76C0 name it). Was landed as a junk `C::Get` on a char stand-in g_str_9a76a0.
// Callees: CWideString(const CWideString&) 0x0099B720.
class CWideString {
public:
    void* PStringData;
    CWideString(const CWideString& other);              // 0x0099B720
};
namespace NBankFileManager {
    extern CWideString SymbolHeaderDir;                 // 0x013CA7D4
    CWideString __fastcall GetHeaderDir();
}

CWideString __fastcall NBankFileManager::GetHeaderDir()
{
    return SymbolHeaderDir;
}
''')
he.put('009a76a0', 'tst', '''// Behaviour test for 0x009A76A0 NBankFileManager::GetHeaderDir(): returns a copy of SymbolHeaderDir.
#include <cstdio>
#include <cstring>
class CWideString {
public:
    char Text[16];
    CWideString() { Text[0] = 0; }
    CWideString(const CWideString& o) { std::memcpy(Text, o.Text, sizeof(Text)); }
};
namespace NBankFileManager {
    CWideString SymbolHeaderDir;
    CWideString GetHeaderDir() { return SymbolHeaderDir; }
}
int main()
{
    std::strcpy(NBankFileManager::SymbolHeaderDir.Text, "Data\\\\Bank\\\\");
    CWideString d = NBankFileManager::GetHeaderDir();
    NBankFileManager::SymbolHeaderDir.Text[0] = 'X';
    if (std::strcmp(d.Text, "Data\\\\Bank\\\\") != 0) { std::printf("B1"); return 1; }
    std::printf("RB_9a76a0_OK\\n"); return 0;
}
''')

# ---- 0x009A76C0 NBankFileManager::SetHeaderDir: the global is NBankFileManager::SymbolHeaderDir ----
A = '009a76c0'
he.edit(A, 'src', '''// (its dynamic initializer 0x01224030 default-constructs a CWideString); its real name is not in the PDB name sets, so
// the existing landed spelling g_CCharString_013ca7d4 (staticinit 0x01224030) is reused to keep one name per address.''',
'''// (its dynamic initializer 0x01224030 default-constructs a CWideString): Ego_r.pdb / FableWin.pdb name its initializer
// NBankFileManager::`dynamic initializer for 'SymbolHeaderDir'' -- NBankFileManager::SymbolHeaderDir (naming wave 24, labels_chunk691;
// was the descriptive g_CCharString_013ca7d4).''')
he.edit(A, 'src', '''extern CWideString g_CCharString_013ca7d4;              // 0x013CA7D4 bank header directory

namespace NBankFileManager {
    void __fastcall SetHeaderDir(const CWideString& dir);
}''', '''namespace NBankFileManager {
    extern CWideString SymbolHeaderDir;                 // 0x013CA7D4 bank header directory
    void __fastcall SetHeaderDir(const CWideString& dir);
}''')
he.edit(A, 'src', '    g_CCharString_013ca7d4 = dir;', '    SymbolHeaderDir = dir;')

# ---- 0x01224030 the dynamic initializer of NBankFileManager::SymbolHeaderDir ----
A = '01224030'
he.edit(A, 'src', '// dynamic initializer for CCharString g_CCharString_013ca7d4 @ 0x013CA7D4', "// dynamic initializer for CWideString NBankFileManager::SymbolHeaderDir @ 0x013CA7D4")
he.edit(A, 'src', '''// Identity: the default constructor called here is 0x0099AED0, CWideString::CWideString() (labels, chunks 237/247/265/284), so the object is a CWideString.

CWideString g_CCharString_013ca7d4;''', '''// Identity: the default constructor called here is 0x0099AED0, CWideString::CWideString() (labels, chunks 237/247/265/284), so the object is a CWideString.
// Naming (naming wave 24): Ego_r.pdb / FableWin.pdb NBankFileManager::`dynamic initializer for 'SymbolHeaderDir'' (labels_chunk691; read
//   by GetHeaderDir 0x009A76A0, written by SetHeaderDir 0x009A76C0); was the descriptive g_CCharString_013ca7d4.

namespace NBankFileManager { CWideString SymbolHeaderDir; }''')

# ---- 0x006F9BE0 NAnimationEvents::CManager::Save: the data directory is NBankFileManager::GetHeaderDir ----
A = '006f9be0'
he.edit(A, 'src', 'CWideString __fastcall GetDataDirectory();          // 0x009A76A0 (name descriptive)',
        'namespace NBankFileManager { CWideString __fastcall GetHeaderDir(); }   // 0x009A76A0 (Ego_r; naming wave 24, was GetDataDirectory)')
he.edit_all(A, 'src', 'GetDataDirectory()', 'NBankFileManager::GetHeaderDir()')

# ---- 0x004336BC CStartupDefinitionManager::InitAndCompilePlatformSpecific ----
A = '004336bc'
he.edit(A, 'src', '''//   CDefinitionManager::InitAndCompile 0x009B08C0; creators''', '''//   CDefinitionManager::Compile 0x009B08C0; creators''')
he.edit(A, 'src', '#pragma optimize("s", on)\n#pragma optimize("y", off)\n', '''// Naming (naming wave 24, labels_chunk691): 0x009B08C0 is Ego_r ?Compile@CDefinitionManager@@QAEXXZ (was InitAndCompile), 0x009A76A0
//   ?GetHeaderDir@NBankFileManager@@YI?AVCWideString@@XZ (was GetBankDirectory), the lists are std::vector<CWideString> (FableWin
//   push_back / ~vector / operator=; was a CWideStringVector stand-in) under the PDB member names SymbolPathList / CompilePathList.
#pragma optimize("s", on)
#pragma optimize("y", off)
''')
he.edit(A, 'src', 'CWideString __fastcall GetBankDirectory();             // 0x009A76A0\n',
        'namespace NBankFileManager { CWideString __fastcall GetHeaderDir(); }                 // 0x009A76A0\n')
he.edit(A, 'src', '''class CWideStringVector {                              // STLport vector<CWideString> (12 bytes)
public:
    CWideString* _M_start;
    CWideString* _M_finish;
    CWideString* _M_end_of_storage;
    CWideStringVector() : _M_start(0), _M_finish(0), _M_end_of_storage(0) { }
    ~CWideStringVector();                              // 0x00412130
    void push_back(const CWideString& x);              // 0x004128A0
    CWideStringVector& operator=(const CWideStringVector& other);   // 0x00433DF0
};''', '''namespace std {
template <class _Tp> class allocator { };
template <class _Tp, class _Alloc = allocator<_Tp> > class vector {   // STLport vector (12 bytes)
public:
    _Tp* _M_start;
    _Tp* _M_finish;
    _Tp* _M_end_of_storage;
    vector() : _M_start(0), _M_finish(0), _M_end_of_storage(0) { }
    ~vector();                                         // 0x00412130
    void push_back(const _Tp& __x);                    // 0x004128A0
    vector<_Tp, _Alloc>& operator=(const vector<_Tp, _Alloc>& __x);   // 0x00433DF0
};
}''')
he.edit(A, 'src', '''    CWideStringVector HeaderFiles;                     // +0x40
    CWideStringVector DefFiles;                        // +0x4c''', '''    std::vector<CWideString> SymbolPathList;           // +0x40
    std::vector<CWideString> CompilePathList;          // +0x4c''')
he.edit(A, 'src', '    void InitAndCompile();                             // 0x009B08C0', '    void Compile();                                    // 0x009B08C0')
he.edit(A, 'src', '    CWideStringVector headers;\n    CWideStringVector defs;', '    std::vector<CWideString> headers;\n    std::vector<CWideString> defs;')
he.edit_all(A, 'src', 'GetBankDirectory()', 'NBankFileManager::GetHeaderDir()')
he.edit(A, 'src', '    HeaderFiles = headers;\n    DefFiles = defs;\n    InitAndCompile();', '    SymbolPathList = headers;\n    CompilePathList = defs;\n    Compile();')
he.edit('009b08c0', 'src', '// Leaf: Compile\n', '// Leaf: ?Compile@CDefinitionManager@@QAEXXZ\n')
