// CPairVector::push_back(const SPair8&) -- STLport vector push_back of an 8-byte POD record: copy at the finish when there is room, else the overflow path
// Leaf: push_back
// Retail: Fable.exe (TLC, VC7.1) @ 0x004CD2F0, 58 bytes. Callees: _M_insert_overflow(pos, x, __true_type, 1, true)
// @ 0x004CCCE0 (its element step is `sar eax,3`: 8-byte elements). Unreferenced in the image (recovered
// from a mis-cut catalog row); the same 8-byte vector is copied by 0x004CC290 (Vector_CopyFrom). The
// element type is not identified; it is modelled as two dwords.
// Shape: the landed 0x008EC1A0 push_back: placement new under VC's null test, `++m_finish` as a memory
// `add [ecx+4],8`; the empty __true_type temporary is homed in the dead `x` argument slot.
struct SPair8 {                       // 8 bytes, trivially copied
    long first;                       // +0
    long second;                      // +4
};
struct __true_type { __true_type() {} };   // empty tag with a user constructor (no byte store)
inline void* operator new(unsigned int, void* where) { return where; }
struct CPairVector {
    SPair8* m_start;                  // +0x00
    SPair8* m_finish;                 // +0x04
    SPair8* m_end_of_storage;         // +0x08
    void _M_insert_overflow(SPair8* position, const SPair8& x, const __true_type&,
                            unsigned long fill_len, bool atend);   // 0x004CCCE0
    void push_back(const SPair8& x);
};

void CPairVector::push_back(const SPair8& x)
{
    if (m_finish != m_end_of_storage) {
        new (m_finish) SPair8(x);
        ++m_finish;
    }
    else
        _M_insert_overflow(m_finish, x, __true_type(), 1, true);
}