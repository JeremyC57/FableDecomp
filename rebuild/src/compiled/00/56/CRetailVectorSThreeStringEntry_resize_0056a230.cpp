// CRetailVector<SThreeStringEntry>::resize(unsigned int newSize) -- grow/shrink a vector of 16-byte string triples.
// Retail: Fable.exe (TLC, VC7.1) function @ 0x0056A230, length 94 bytes.
// Manifest label "~CAIStateGroup_JackOfBladesBase" is propagated: the body is the one-argument `resize` of a
// vector whose element is {CCharString, CCharString, CCharString, bool, bool} (16 bytes: the callee @ 0x00569D90
// strides its range with `sar edi,4`); the only caller (0x00562718) resizes the vector at +0x28 of its owner to
// 0x27 entries. Callees: CCharString::CCharString() @ 0x0099E4B0 x3 and ~CCharString @ 0x0099EAE0 x3 (the default
// element's members), resize(unsigned int, const SThreeStringEntry&) @ 0x00569D90 (manifest "vector<unsigned
// int>::resize", a propagated label).
//
// Shape: STLport's `resize(n) { resize(n, T()); }`: the default element is built in a 0x10-byte frame slot
// (three string constructors, then the two bools zeroed), passed BY ADDRESS together with the count, and
// destroyed member by member in reverse order after the call. `ret 4`: one stack argument.
class CCharString { public:
    void* m_pData;
    CCharString();                              // 0x0099E4B0
    ~CCharString();                             // 0x0099EAE0
};

struct SThreeStringEntry {                      // 16 bytes (14 + 2 padding)
    CCharString m_name;
    CCharString m_text;
    CCharString m_extra;
    bool m_flagA;
    bool m_flagB;
    SThreeStringEntry() : m_name(), m_text(), m_extra(), m_flagA(false), m_flagB(false) { }
};

struct CRetailVectorSThreeStringEntry {         // std::vector<SThreeStringEntry>: first/last/end
    SThreeStringEntry* first;
    SThreeStringEntry* last;
    SThreeStringEntry* end_of_storage;
    void resize(unsigned int newSize, const SThreeStringEntry& value);   // 0x00569D90
    void resize(unsigned int newSize);
};

void CRetailVectorSThreeStringEntry::resize(unsigned int newSize)
{
    resize(newSize, SThreeStringEntry());
}