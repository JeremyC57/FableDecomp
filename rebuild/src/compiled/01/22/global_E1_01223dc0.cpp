// _$E1() -- `dynamic initializer for 'CSystemRegistry::GAME_REG_KEY'': CSystemRegistry::GAME_REG_KEY = CWideString::CreateFromCharString("Software\\Microsoft\\Microsoft Games\\Fable TLC"), then atexit(its destructor thunk)
// Leaf: _$E1
// Retail: Fable.exe (TLC, VC7.1) @ 0x01223DC0, 52 bytes, in `.text$yc` (its address is in the CRT initializer table at 0x01374784).
// Identity (proven): it constructs the CWideString global 0x013BCA28 from the literal 0x0129A8E4 "Software\\Microsoft\\Microsoft Games\\Fable TLC" and registers the
//   atexit thunk 0x0122A5A0 (`mov ecx,0x013BCA28 ; jmp ~CWideString`). Ego_r.pdb and FableWin.pdb list exactly four
//   `dynamic initializer for 'CSystemRegistry::...'` symbols -- GAME_REG_KEY, GFX_RESET_KEY_NAME, GAME_LOCAL_MACHINE_REG_KEY,
//   PID_KEY_NAME -- in the order of the four retail initializers 0x01223DC0/0x01223E00/0x01223E40/0x01223E80 (globals
//   0x013BCA28..0x013BCA34), and their names fit the literals ("...\Fable TLC" key, "GFX_RESET" value, "...\Fable\1.0"
//   local-machine key, "PID" value). Landed callers bind 0x013BCA28..34 under descriptive spellings (g_ConfigRegistryKey,
//   g_RegistryKeyLocation, g_RegistryProductKey, ...); this row uses the PDB member name. The `const` is probable (no PDB
//   symbol for the member itself survives).
// Callees: CCharString::CCharString(const char*, long) 0x0099EBF0, CWideString::CreateFromCharString 0x0099B6A0 (static
//   __fastcall: hidden result in ecx = &global, the string in edx), CCharString::~CCharString 0x0099EAE0, atexit 0x004012BC.
// Shape (family M001, representative 0x01223DC0): the literal is an IMPLICIT conversion to `const CCharString&` (the
//   temporary sits in the `push ecx` word and its address is re-taken with `lea edx,[esp]`; an explicit CCharString(...)
//   would pass the ctor's eax); VC7.1 names the TU's initializer `$E1` and the atexit thunk `$E2`.
class CCharString {
public:
    CCharString(const char* text, long length = -1);      // 0x0099EBF0
    ~CCharString();                                       // 0x0099EAE0
    void* PStringData;
};
class CWideString {
public:
    ~CWideString();                                       // 0x0099B510
    static CWideString __fastcall CreateFromCharString(const CCharString& text);   // 0x0099B6A0
    void* PStringData;
};
class CSystemRegistry {                                   // Ego_r.pdb: 8 bytes (hRoot, hKey)
public:
    static const CWideString GAME_REG_KEY;          // 0x013BCA28
    void* hRoot;
    void* hKey;
};
const CWideString CSystemRegistry::GAME_REG_KEY = CWideString::CreateFromCharString("Software\\Microsoft\\Microsoft Games\\Fable TLC");