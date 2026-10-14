#include <cstdio>
static long g_frame = 5;
long __fastcall GFGetFrameCount() { return g_frame; }
struct CTransitionBlendAlpha { float Current; float Last; };
template<class T> struct CRetailVector { T* first; T* last; T* end_of_storage; T& operator[](unsigned long n) { return first[n]; } unsigned long size() const { return (unsigned long)(last - first); } };
struct CAnimSet {
    long UpdateMode; CRetailVector<short> Anims; CRetailVector<CTransitionBlendAlpha> InternalAlphas;
    float a, b, c; unsigned long FrameInternalAlphasUpdated;
    void UpdateInternalAlphas() {
        if (FrameInternalAlphasUpdated < (unsigned long)GFGetFrameCount()) {
            for (unsigned long i = 0; i < InternalAlphas.size(); ++i) InternalAlphas[i].Last = InternalAlphas[i].Current;
            FrameInternalAlphasUpdated = GFGetFrameCount();
        }
    }
};
int main()
{
    CTransitionBlendAlpha e[2] = { {1,2}, {3,4} };
    CAnimSet s; s.InternalAlphas.first = e; s.InternalAlphas.last = e + 2; s.InternalAlphas.end_of_storage = e + 2; s.FrameInternalAlphasUpdated = 5;
    s.UpdateInternalAlphas();
    if (e[0].Last != 2.0f || s.FrameInternalAlphasUpdated != 5) { std::printf("0070c2c0_TEST FAIL\n"); return 1; }
    s.FrameInternalAlphasUpdated = 4; s.UpdateInternalAlphas();
    if (e[0].Last != 1.0f || e[1].Last != 3.0f || s.FrameInternalAlphasUpdated != 5) { std::printf("0070c2c0_TEST FAIL\n"); return 1; }
    std::printf("0070c2c0_TEST PASS\n");
    return 0;
}