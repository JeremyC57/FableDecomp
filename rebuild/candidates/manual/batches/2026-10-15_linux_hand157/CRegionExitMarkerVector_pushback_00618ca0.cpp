// CRegionExitMarkerVector::push_back(const SRegionExitMarker& x) -- STLport vector push_back of a 28-byte record {long, CCharString, five longs}: copy-construct at the finish when there is room, else the overflow path
// Leaf: push_back
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00618CA0, length 111 bytes.
// Label correction: the catalog "CScriptModuleFullTransition::OnInitLeaf" is a propagated name; the body is
// an STLport vector<T>::push_back (`this` is the vector: start +0, finish +4, end_of_storage +8) called
// twice from CTCInventoryMap::ConstructRegionExitMarkers (0x005FECB0, 0x005FF025). The element type is not
// identified; it is named after that caller.
// Callees: CCharString copy ctor @ 0x0099EC30, _M_insert_overflow(pos, x, __false_type, 1, true) @ 0x006182F0.
// Shape: the landed 0x00CB7210 push_back: placement new under VC's null test with a `__forceinline`
// memberwise copy ctor, finish re-read and advanced by 0x1c; the empty __false_type temporary is homed in
// the dead `x` argument slot.
struct CCharString {
    void* m_pData;
    CCharString(const CCharString& other);  // 0x0099EC30
};
struct SRegionExitMarker {                  // 28 bytes
    long          m_00;                     // +0x00
    CCharString   m_name;                   // +0x04
    long          m_08;                     // +0x08
    long          m_0c;                     // +0x0c
    long          m_10;                     // +0x10
    long          m_14;                     // +0x14
    long          m_18;                     // +0x18
    __forceinline SRegionExitMarker(const SRegionExitMarker& o)
        : m_00(o.m_00), m_name(o.m_name), m_08(o.m_08), m_0c(o.m_0c), m_10(o.m_10), m_14(o.m_14), m_18(o.m_18) {}
};
struct __false_type { __false_type() {} };  // empty tag with a user constructor (no byte store)
inline void* operator new(unsigned int, void* where) { return where; }
struct CRegionExitMarkerVector {
    SRegionExitMarker* m_start;             // +0x00
    SRegionExitMarker* m_finish;            // +0x04
    SRegionExitMarker* m_end_of_storage;    // +0x08
    void _M_insert_overflow(SRegionExitMarker* position, const SRegionExitMarker& x, const __false_type&,
                            unsigned long fill_len, bool atend);   // 0x006182F0
    void push_back(const SRegionExitMarker& x);
};
void CRegionExitMarkerVector::push_back(const SRegionExitMarker& x)
{
    if (m_finish != m_end_of_storage) {
        new (m_finish) SRegionExitMarker(x);
        ++m_finish;
    }
    else
        _M_insert_overflow(m_finish, x, __false_type(), 1, true);
}
