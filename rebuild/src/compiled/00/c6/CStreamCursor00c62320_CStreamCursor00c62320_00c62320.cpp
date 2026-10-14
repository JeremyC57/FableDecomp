// CStreamCursor_00c62320::CStreamCursor_00c62320(void* owner) -- six-word cursor record: the owner pointer, a zero position, an invalid (-1) index, three zero words
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00C62320, length 32 bytes.
// Callees: none. `this` moves to eax at once (the constructor returns it) and every store goes through eax; the
// zero words share `xor ecx,ecx`, the -1 is an immediate. The manifest label "CDataInputStream::Close" is
// propagated from a same-shape body (Close takes no argument and returns nothing; this body stores an argument
// at +0 and returns `this`), so the class is named by address.
struct CStreamCursor_00c62320 {
    void* m_pOwner;          // +0x00 the argument
    long  m_position;        // +0x04 0
    long  m_index;           // +0x08 -1
    long  m_a;               // +0x0c 0
    long  m_b;               // +0x10 0
    long  m_c;               // +0x14 0
    CStreamCursor_00c62320(void* owner);
};

CStreamCursor_00c62320::CStreamCursor_00c62320(void* owner)
    : m_pOwner(owner), m_position(0), m_index(-1), m_a(0), m_b(0), m_c(0)
{
}