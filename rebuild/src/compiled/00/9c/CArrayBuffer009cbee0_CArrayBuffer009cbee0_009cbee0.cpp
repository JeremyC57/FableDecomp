// CArrayBuffer_009cbee0::~CArrayBuffer_009cbee0() -- delete[] the array at +0x8 (then null it) and zero the count at +0x4
// Leaf: ??1CArrayBuffer_009cbee0
// Retail: Fable.exe (TLC, VC7.1) function @ 0x009CBEE0, 35 bytes (+ int3 padding).
// Callee: operator delete[] @ 0x00BFEB1C (landed).  No call/jmp or absolute reference reaches this
// body (xref), so its owner is unknown; the manifest label ~Stack<unsigned long,64> is propagated
// (the PDB's Stack<unsigned long,64> is an empty 1-byte struct).
// Shape: `if (m_data) { delete[] m_data; m_data = 0; } m_count = 0;` -- the pointer reset sits
// inside the null test, the count reset after it; the array delete of a scalar element type is a
// direct `operator delete[]` call (declared here: the lane's <new> lacks the plain form).
void operator delete[](void* block);       // 0x00BFEB1C

struct CArrayBuffer_009cbee0 {
    unsigned char _pad[0x4];
    unsigned long m_count;                 // +0x4
    unsigned long* m_data;                 // +0x8
    ~CArrayBuffer_009cbee0();
};

CArrayBuffer_009cbee0::~CArrayBuffer_009cbee0()
{
    if (m_data) {
        delete[] m_data;
        m_data = 0;
    }
    m_count = 0;
}