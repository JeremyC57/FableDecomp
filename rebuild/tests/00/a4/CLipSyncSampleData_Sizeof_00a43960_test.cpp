// Behaviour test for 0x00A43960 (CLipSyncSampleData::Sizeof): nested array sizes.
#include <cstdio>
struct Ph { int a, b, c; unsigned int Sizeof() const { return 12; } };
template <class T> struct Arr {
    T* m_first; T* m_last;
    unsigned int Size() const { return (unsigned int)(m_last - m_first); }
    unsigned int Sizeof() const { unsigned int s = Size() * 4 + 12; for (const T* it = m_first; it != m_last; ++it) s += it->Sizeof(); return s; }
};
struct Frame { Arr<Ph> P; unsigned int Sizeof() const { return P.Sizeof(); } };
struct Sample { Arr<Frame> F; unsigned int Sizeof() const { return F.Sizeof() + 4; } };
int main()
{
    Ph ph[5]; Frame fr[2]; fr[0].P.m_first = ph; fr[0].P.m_last = ph + 2; fr[1].P.m_first = ph; fr[1].P.m_last = ph + 5;
    Sample s; s.F.m_first = fr; s.F.m_last = fr + 2;
    // frames: 2*4+12 = 20; frame0: 2*4+12+24 = 44; frame1: 5*4+12+60 = 92; +4
    int ok = s.Sizeof() == 20 + 44 + 92 + 4;
    s.F.m_last = fr; ok &= s.Sizeof() == 16;
    if (!ok) { std::printf("00a43960_TEST FAIL\n"); return 1; }
    std::printf("00a43960_TEST PASS\n");
    return 0;
}