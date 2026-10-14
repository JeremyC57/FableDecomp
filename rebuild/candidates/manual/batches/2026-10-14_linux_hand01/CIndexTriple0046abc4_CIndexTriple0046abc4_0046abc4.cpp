#pragma optimize("s", on)
// CIndexTriple_0046abc4::CIndexTriple_0046abc4() -- a zero count followed by three -1 indices
// Retail: Fable.exe (TLC, VC7.1) function @ 0x0046ABC4, length 18 bytes. No callees.
// The manifest label (C3DGroup2::C3DGroup2) is propagated: C3DGroup2 starts with BoneIndex = -1
// (see 0x00AC40C0); this object is {0, -1, -1, -1}. Size-optimised TU: `and [m],0` / `or [m],-1`
// stores. Named by what it holds.
struct CIndexTriple_0046abc4 {
    long m_count;                          // +0 = 0
    long m_index0;                         // +4 = -1
    long m_index1;                         // +8 = -1
    long m_index2;                         // +0xc = -1
    CIndexTriple_0046abc4();
};

CIndexTriple_0046abc4::CIndexTriple_0046abc4()
    : m_count(0), m_index0(-1), m_index1(-1), m_index2(-1)
{
}
