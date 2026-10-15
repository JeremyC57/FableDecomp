// CHoverModeEntryVector::push_back(const SHoverModeEntry& x) -- STLport vector push_back of a 24-byte record {CCharString, C3DVector, two longs}: copy-construct at the finish when there is room, else the overflow path
// Leaf: push_back
// Retail: Fable.exe (TLC, VC7.1) function @ 0x008EC1A0, length 112 bytes (99 + int3 padding).
// Label correction: the catalog "CScriptModuleFullTransition::OnInitLeaf" is a propagated name; the body is
// an STLport vector<T>::push_back (`this` is the vector: start +0, finish +4, end_of_storage +8) called
// fifteen times from CModeHoverBase::OnActivate (0x008EAD89 ..). The element type is not identified; it is named after that caller.
// Callees: CCharString copy ctor @ 0x0099EC30, _M_insert_overflow(pos, x, __false_type, 1, true) @ 0x008EC070.
// Shape: the landed 0x00CB7210 push_back: placement new under VC's null test with a `__forceinline`
// memberwise copy ctor, finish re-read and advanced by 0x18; the empty __false_type temporary is homed in
// the dead `x` argument slot.
struct CCharString {
    void* m_pData;
    CCharString(const CCharString& other);  // 0x0099EC30
};
struct C3DVector { float X, Y, Z; };
struct SHoverModeEntry {                // 24 bytes
    CCharString   m_name;                   // +0x00
    C3DVector     m_offset;                 // +0x04
    long          m_10;                     // +0x10
    long          m_14;                     // +0x14
    __forceinline SHoverModeEntry(const SHoverModeEntry& o)
        : m_name(o.m_name), m_offset(o.m_offset), m_10(o.m_10), m_14(o.m_14) {}
};
struct __false_type { __false_type() {} };  // empty tag with a user constructor (no byte store)
inline void* operator new(unsigned int, void* where) { return where; }
struct CHoverModeEntryVector {
    SHoverModeEntry* m_start;             // +0x00
    SHoverModeEntry* m_finish;            // +0x04
    SHoverModeEntry* m_end_of_storage;    // +0x08
    void _M_insert_overflow(SHoverModeEntry* position, const SHoverModeEntry& x, const __false_type&,
                            unsigned long fill_len, bool atend);   // 0x008EC070
    void push_back(const SHoverModeEntry& x);
};
void CHoverModeEntryVector::push_back(const SHoverModeEntry& x)
{
    if (m_finish != m_end_of_storage) {
        new (m_finish) SHoverModeEntry(x);
        ++m_finish;
    }
    else
        _M_insert_overflow(m_finish, x, __false_type(), 1, true);
}
