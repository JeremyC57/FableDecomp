// CSubtitledMessage::Initialise() -- load the misc def-type symbols: the CRC symbol map reads "Data\Defs\" + "misc_def_types.h" with an empty prefix
// Leaf: ?Initialise@CSubtitledMessage@@SIXXZ
// Retail: Fable.exe (TLC, VC7.1) function @ 0x004CDB10, length 96 bytes (89 + int3 padding), static __fastcall, plain `ret`.
// Identity: Ego_r.pdb `?Initialise@CSubtitledMessage@@SIXXZ` (the only static no-argument Initialise in the PDB next to
// the landed CSubtitledMessage members 0x004CDB70 / 0x004CDBD0 / 0x004CE4E0); its single caller is
// CMainGameComponent::Init (0x00418637). The catalog "Initialise" is right but unscoped.
// Callees: CCharString(const char*, long) @ 0x0099EBF0, GetDefsDirectory() @ 0x0041A080 ("Data\Defs\", __fastcall,
// hidden result in ecx), operator+(const CWideString&, const unsigned short*) @ 0x0099BF30 (__fastcall), CCRCSymbolMap::
// LoadFromFile(const CWideString&, const CCharString&) @ 0x00A39010 (Ego_r.pdb `?LoadFromFile@CCRCSymbolMap@@QAE_N...`),
// ~CWideString @ 0x0099B510, ~CCharString @ 0x0099EAE0 (landed names).
// Data: L"misc_def_types.h" 0x01239E74, "" 0x0122D70E; the CCRCSymbolMap at 0x013B8A54 (default-constructed by the static
// initialiser 0x0121A630; its declared name is not in the PDB, so it is named by use).
// Shape: right-to-left argument evaluation -- the CCharString("") temporary first, then the directory and the sum; the
// temporaries die in reverse order (sum, directory, prefix). GetDefsDirectory leaves its stack untouched, so the two
// pushes ahead of it are operator+'s literal and LoadFromFile's second argument.
class CCharStringData;
class CCharString {
public:
    CCharStringData* PStringData;
    CCharString(const char* text, long length = -1);   // 0x0099EBF0
    ~CCharString();                                     // 0x0099EAE0
};

class CWideString {
public:
    void* PStringData;
    ~CWideString();                                     // 0x0099B510
};

CWideString __fastcall GetDefsDirectory();             // 0x0041A080
CWideString __fastcall operator+(const CWideString& left, const unsigned short* right);   // 0x0099BF30 (wchar_t is
                                                                                         // unsigned short without /Zc:wchar_t)

class CCRCSymbolMap {
public:
    bool LoadFromFile(const CWideString& filename, const CCharString& prefix);   // 0x00A39010
};
extern CCRCSymbolMap g_MiscDefTypesSymbolMap;           // 0x013B8A54

class CSubtitledMessage {
public:
    static void __fastcall Initialise();
};

void __fastcall CSubtitledMessage::Initialise()
{
    g_MiscDefTypesSymbolMap.LoadFromFile(GetDefsDirectory() + L"misc_def_types.h", "");
}