// CStringPairRecordVector_00cb7210::push_back(const SStringPairRecord&) -- STLport vector push_back of a 24-byte record (two CCharStrings): copy-construct at the finish when there is room, else the overflow path
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00CB7210, length 109 bytes (padded to 112).
// Leaf: push_back
// Label correction: the catalog "CParticleSkeletalMorphMap::EmplaceOrAppend" is a synthetic propagated name; the body is the
// STLport vector::push_back of a 24-byte record {CCharString, long, long, CCharString, long, bool} (`this` is the vector:
// start +0, finish +4, end_of_storage +8; callers 0x00CB5D47 / 0x00CB72E8). The element type is not identified; named by address.
// Callees: CCharString copy ctor @ 0x0099EC30 (x2), _M_insert_overflow(pos, x, __false_type, 1, true) @ 0x00CB6FD0
// (manifest label "CCombatWheel_ReallocateElements").
// Shape: with room left, placement new under VC's null test copies the record by a memberwise copy ctor that must be
// `__forceinline` (the implicit one and a plain inline one are both emitted out of line here), then finish is re-read and
// advanced by 24; otherwise the overflow helper gets the empty __false_type temporary homed in the dead `x` slot.
struct CCharString {
    void* m_pData;
    CCharString(const CCharString& other);  // 0x0099EC30
};
struct SStringPairRecord {                  // 24 bytes
    CCharString   m_name;                   // +0x00
    long          m_04;                     // +0x04
    long          m_08;                     // +0x08
    CCharString   m_value;                  // +0x0c
    long          m_10;                     // +0x10
    bool          m_14;                     // +0x14
    __forceinline SStringPairRecord(const SStringPairRecord& o)
        : m_name(o.m_name), m_04(o.m_04), m_08(o.m_08), m_value(o.m_value), m_10(o.m_10), m_14(o.m_14) {}
};
struct __false_type { __false_type() {} };  // empty tag with a user constructor (no byte store)
inline void* operator new(unsigned int, void* where) { return where; }
struct CStringPairRecordVector_00cb7210 {
    SStringPairRecord* m_start;             // +0x00
    SStringPairRecord* m_finish;            // +0x04
    SStringPairRecord* m_end_of_storage;    // +0x08
    void _M_insert_overflow(SStringPairRecord* position, const SStringPairRecord& x, const __false_type&,
                            unsigned long fill_len, bool atend);   // 0x00CB6FD0
    void push_back(const SStringPairRecord& x);
};
void CStringPairRecordVector_00cb7210::push_back(const SStringPairRecord& x)
{
    if (m_finish != m_end_of_storage) {
        new (m_finish) SStringPairRecord(x);
        ++m_finish;
    }
    else
        _M_insert_overflow(m_finish, x, __false_type(), 1, true);
}