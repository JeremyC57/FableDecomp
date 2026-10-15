// CPackedUIntArray::~CPackedUIntArray() -- releases the packed word buffer with delete[] and clears the buffer pointer and the element count
// Retail: Fable.exe (TLC, VC7.1) function @ 0x0066C150, length 29 bytes.
// Callees: operator delete[] @ 0x00BFEB1C.
// Shape: `push [esi] ; call delete[] ; add esp,4` is `delete[] m_pData` on a scalar element type (no
// count cookie, no destructor iterator), then two immediate zero stores in declaration order. Only
// the first two words are touched, so this body is a two-word {buffer, count} array; the full
// CPackedUIntArray destructor at 0x00A629D0 also clears +8/+0xc. The label is propagated over the
// byte-identical twin at 0x0066C000 (no direct callers of either).
struct CPackedUIntArray {
    unsigned long* m_pData;                 // +0x00 packed words (new[])
    unsigned long  m_count;                 // +0x04
    ~CPackedUIntArray();
};

CPackedUIntArray::~CPackedUIntArray()
{
    delete[] m_pData;
    m_pData = 0;
    m_count = 0;
}
