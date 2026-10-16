// CBankFile::SplitSymbol(const CCharString& symbol, CCharString* pName, CCharString* pSuffix) -- split "name[suffix]": cut the text before '[' and between the brackets; without both brackets the name is the whole symbol and the suffix empty
// Leaf: ?SplitSymbol@CBankFile@@
// Retail: Fable.exe (TLC, VC7.1) function @ 0x009CBAA0, length 212 bytes (+12 int3), ret 0xc (`this` unused).
// Ego_r.pdb ?SplitSymbol@CBankFile@@QAEXABVCCharString@@PAV2@1@Z.
// Callees: CCharString::FindCharPos(char) @ 0x0099E5A0, CCharString::CCharString() @ 0x0099E4B0,
// CCharString::GetSubString(ulong, ulong) @ 0x0099EC70 (by value), CCharString::operator=(const CCharString&) @ 0x0099EFB0,
// CCharString::operator=(const char*) @ 0x0099EFE0 ("" @ 0x0122D70E), CCharString::~CCharString @ 0x0099EAE0.
// Note: retail copies the WHOLE symbol (not the cut name) into *pName -- the cut name is computed and dropped; this is
// reproduced as compiled.
// Shape: name is the frame local, suffix is homed in the dead symbol-argument slot (symbol stays in esi); the sub-string
// temporaries share one frame word.
struct CCharString {
    void* m_pData;
    CCharString();                                 // 0x0099E4B0
    ~CCharString();                                // 0x0099EAE0
    long FindCharPos(char c) const;                // 0x0099E5A0
    CCharString GetSubString(unsigned long start, unsigned long end) const;   // 0x0099EC70
    CCharString& operator=(const CCharString& other);   // 0x0099EFB0
    CCharString& operator=(const char* text);      // 0x0099EFE0
};

class CBankFile {
public:
    void SplitSymbol(const CCharString& symbol, CCharString* pName, CCharString* pSuffix);
};

void CBankFile::SplitSymbol(const CCharString& symbol, CCharString* pName, CCharString* pSuffix)
{
    long open = symbol.FindCharPos('[');
    long close = symbol.FindCharPos(']');
    CCharString name;
    CCharString suffix;
    if (open != -1 && close != -1) {
        name = symbol.GetSubString(0, open);
        suffix = symbol.GetSubString(open + 1, close);
    } else {
        name = symbol;
        suffix = "";
    }
    if (pName)
        *pName = symbol;
    if (pSuffix)
        *pSuffix = suffix;
}
