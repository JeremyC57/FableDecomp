// CSharerParserOutputToDebugContextMapper::NewEntry() -- returns a default CEntry by value: a named local built by the out-of-line default constructor, copied member-wise into the hidden result (name string, two string sets), then destroyed
// Retail: Fable.exe (TLC, VC7.1) function @ 0x006BAB80, length 69 bytes.
// Leaf: NewEntry
// Callees: CEntry::CEntry() @ 0x006BA420, CCharString::CCharString(const CCharString&) @ 0x0099EC30,
// the STLport set copy constructor @ 0x00514400 (x2), CEntry::~CEntry() @ 0x006BA6D0.
// The manifest label "AddEntry" (four parameters) is propagated: the body takes only a hidden result pointer in ecx and
// returns it (`mov eax,esi ; ret`), i.e. a static `__fastcall` function returning the 0x1c-byte entry by value.
// Shape: VC7.1 has no named-return-value optimisation, so the local is built in the 0x1c frame, the implicit copy
// constructor is inlined into the result (string at +0, sets at +4 / +0x10, each source re-addressed with `lea`),
// and the local's destructor runs before the return.
struct CCharString {
    void* m_pData;
    CCharString(const CCharString& other);      // 0x0099EC30
    ~CCharString();
};

struct SStringSet {                             // STLport set<CCharString>: header node, node count, comparator (12 bytes)
    void*         m_pHeader;
    unsigned long m_count;
    unsigned long m_compare;
    SStringSet(const SStringSet& other);        // 0x00514400
    ~SStringSet();
};

struct CSharerParserOutputToDebugContextMapper {
    struct CEntry {                             // 0x1c bytes
        CCharString Name;                       // +0x00
        SStringSet  Sharers;                    // +0x04
        SStringSet  Contexts;                   // +0x10
        CEntry();                               // 0x006BA420
        ~CEntry();                              // 0x006BA6D0
    };
    static CEntry __fastcall NewEntry();
};

CSharerParserOutputToDebugContextMapper::CEntry __fastcall CSharerParserOutputToDebugContextMapper::NewEntry()
{
    CEntry entry;
    return entry;
}
