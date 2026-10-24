// CStringTriple_0056d190::~CStringTriple_0056d190() -- destructor of a record whose only non-trivial members are a CCharString (+0x10) and two CWideStrings (+0x14, +0x18): the wide strings in reverse order, then the narrow one
// Leaf: ??1CStringTriple_0056d190
// Retail: Fable.exe (TLC, VC7.1) function @ 0x0056D190, length 28 bytes, `jmp` tail; no call or address references (an
// unreferenced COMDAT). Byte-identical (call targets aside) to the landed 0x0056C820 / 0x0056C840. The record type is not
// recoverable from the bytes or the neighbours (no Ego_r.pdb class has this member pattern), so the name is descriptive;
// the manifest label (_Dest_val<allocator<pair<CCharString,CSymbolMap::CSymbolInfo>>>) is propagated and wrong
// (CSymbolInfo holds two CCharStrings and a long).
// Callees: CWideString::~CWideString @ 0x0099B510 (x2), CCharString::~CCharString @ 0x0099EAE0 (tail).
// Shape: members destroyed in reverse declaration order with direct `lea ecx,[esi+n]`; the first-declared string is the
// tail jmp. Bytes +0x00..+0x0f are trivially destructible.
class CCharString {
public:
    void* PStringData;
    ~CCharString();                                          // 0x0099EAE0
};
class CWideString {
public:
    void* PStringData;
    ~CWideString();                                          // 0x0099B510
};

struct CStringTriple_0056d190 {
    unsigned char _pad00[0x10];                              // +0x00 trivially destructible members
    CCharString   m_name;                                    // +0x10
    CWideString   m_text;                                    // +0x14
    CWideString   m_altText;                                 // +0x18
    ~CStringTriple_0056d190();
};

CStringTriple_0056d190::~CStringTriple_0056d190()
{
}