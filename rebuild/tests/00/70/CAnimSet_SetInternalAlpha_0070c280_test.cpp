#include <cstdio>
static long g_frame = 77;
long __fastcall GFGetFrameCount() { return g_frame; }
class CTransitionBlendAlpha { public: float Current; float Last; };
template<class T> struct CRetailVector { T* first; T* last; T* end_of_storage; T& operator[](unsigned long n) { return first[n]; } };
class CAnimSet { public:
    long UpdateMode; CRetailVector<short> Anims; CRetailVector<CTransitionBlendAlpha> InternalAlphas;
    float a, b, c; unsigned long FrameInternalAlphasUpdated;
    void SetInternalAlpha(long index, float alpha, bool setLastToo) {
        InternalAlphas[index].Last = InternalAlphas[index].Current;
        InternalAlphas[index].Current = alpha;
        if (setLastToo) InternalAlphas[index].Last = alpha;
        FrameInternalAlphasUpdated = GFGetFrameCount();
    }
};
int main()
{
    CTransitionBlendAlpha e[3] = { {1,2}, {3,4}, {5,6} };
    CAnimSet s; s.InternalAlphas.first = e; s.InternalAlphas.last = e + 3; s.InternalAlphas.end_of_storage = e + 3; s.FrameInternalAlphasUpdated = 0;
    s.SetInternalAlpha(1, 9.0f, false);
    if (e[1].Current != 9.0f || e[1].Last != 3.0f || s.FrameInternalAlphasUpdated != 77) { std::printf("0070c280_TEST FAIL\n"); return 1; }
    s.SetInternalAlpha(2, 11.0f, true);
    if (e[2].Current != 11.0f || e[2].Last != 11.0f || e[0].Current != 1.0f) { std::printf("0070c280_TEST FAIL\n"); return 1; }
    std::printf("0070c280_TEST PASS\n");
    return 0;
}