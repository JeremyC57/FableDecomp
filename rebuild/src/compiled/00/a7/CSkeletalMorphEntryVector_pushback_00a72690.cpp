// CSkeletalMorphEntryVector::push_back(const SSkeletalMorphEntry& x) -- STLport vector push_back of a 20-byte record {CCharString, four longs}: copy-construct at the finish when there is room, else the overflow path
// Leaf: push_back
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00A72690, length 112 (97 + int3 padding) bytes.
// Label correction: the catalog "CScriptModuleFullTransition::OnInitLeaf" is a propagated name; the body is
// an STLport vector<T>::push_back (`this` is the vector: start +0, finish +4, end_of_storage +8) called
// from CTCSkeletalMorph::AddSkeletalMorph (0x00A72BCA). The element type is not identified; it is named after that caller.
// Callees: CCharString copy ctor @ 0x0099EC30, _M_insert_overflow(pos, x, __false_type, 1, true) @ 0x00A71FC0.
// Shape: the landed 0x00CB7210 push_back: placement new under VC's null test with a `__forceinline`
// memberwise copy ctor, finish re-read and advanced by 0x14; the empty __false_type temporary is homed in
// the dead `x` argument slot.
struct CCharString {
    void* m_pData;
    CCharString(const CCharString& other);  // 0x0099EC30
};
struct SSkeletalMorphEntry {                // 20 bytes
    CCharString   m_name;                   // +0x00
    long          m_04;                     // +0x04
    long          m_08;                     // +0x08
    long          m_0c;                     // +0x0c
    long          m_10;                     // +0x10
    __forceinline SSkeletalMorphEntry(const SSkeletalMorphEntry& o)
        : m_name(o.m_name), m_04(o.m_04), m_08(o.m_08), m_0c(o.m_0c), m_10(o.m_10) {}
};
struct __false_type { __false_type() {} };  // empty tag with a user constructor (no byte store)
inline void* operator new(unsigned int, void* where) { return where; }
struct CSkeletalMorphEntryVector {
    SSkeletalMorphEntry* m_start;             // +0x00
    SSkeletalMorphEntry* m_finish;            // +0x04
    SSkeletalMorphEntry* m_end_of_storage;    // +0x08
    void _M_insert_overflow(SSkeletalMorphEntry* position, const SSkeletalMorphEntry& x, const __false_type&,
                            unsigned long fill_len, bool atend);   // 0x00A71FC0
    void push_back(const SSkeletalMorphEntry& x);
};
void CSkeletalMorphEntryVector::push_back(const SSkeletalMorphEntry& x)
{
    if (m_finish != m_end_of_storage) {
        new (m_finish) SSkeletalMorphEntry(x);
        ++m_finish;
    }
    else
        _M_insert_overflow(m_finish, x, __false_type(), 1, true);
}