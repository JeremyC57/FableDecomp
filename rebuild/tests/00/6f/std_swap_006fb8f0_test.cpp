// Behaviour test for std::swap<pair<unsigned long, CSmallVector<CEvent,32>>> (0x006FB8F0): swaps keys and event vectors through a deep-copied temporary.
#include <cstdio>
struct CEvent { unsigned int Index; float Time; };
struct SmallVector {
    CEvent* Start; int Size;
    SmallVector() : Start(0), Size(0) {}
    SmallVector(const SmallVector& o) : Start(0), Size(0) { Copy(o); }
    ~SmallVector() { delete[] Start; }
    SmallVector& operator=(const SmallVector& o) { if (&o != this) Copy(o); return *this; }
    void Copy(const SmallVector& o) { delete[] Start; Start = 0; Size = 0; Start = new CEvent[o.Size]; Size = o.Size; for (int i = 0; i < o.Size; ++i) Start[i] = o.Start[i]; }
};
struct Pair { unsigned int first; SmallVector second; };
static void swap_pairs(Pair& a, Pair& b) { Pair tmp = a; a = b; b = tmp; }
int main()
{
    Pair a, b;
    a.first = 1; a.second.Start = new CEvent[2]; a.second.Size = 2; a.second.Start[0].Time = 0.5f; a.second.Start[1].Time = 1.5f;
    b.first = 2; b.second.Start = new CEvent[1]; b.second.Size = 1; b.second.Start[0].Time = 9.0f;
    swap_pairs(a, b);
    if (a.first != 2 || a.second.Size != 1 || a.second.Start[0].Time != 9.0f || b.first != 1 || b.second.Size != 2 || b.second.Start[1].Time != 1.5f) { std::printf("006fb8f0_TEST FAIL\n"); return 1; }
    std::printf("006fb8f0_TEST PASS\n");
    return 0;
}