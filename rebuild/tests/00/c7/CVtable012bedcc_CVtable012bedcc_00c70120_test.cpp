// Behaviour test for 0x00C70120 CVtable_012bedcc::CVtable_012bedcc(): base constructed, the scratch pointer and the four
// block pointers 16-byte aligned inside their raw buffers, both blocks otherwise zero, front/back pointing at the
// blocks, the four counters zero.
#include <cstdio>
#include <cstring>

static int g_baseCtors = 0;
struct CVtable_012beda4 {
    unsigned char _body[0xa4];
    CVtable_012beda4() { ++g_baseCtors; }
    virtual ~CVtable_012beda4() { }
};

template <int N> struct SAlignedFloats {
    unsigned char Raw[N * 4 + 16];
    float*        Aligned;
};

template <int N> struct SAlignedScratch : SAlignedFloats<N> {
    SAlignedScratch() { this->Aligned = (float*)(((unsigned long)this->Raw + 15) & ~15); }
};

template <int N> struct SDoubleBlock {
    SAlignedFloats<N> A;
    unsigned char     GapA[0x10];
    SAlignedFloats<N> B;
    unsigned char     GapB[0x10];
};

struct CVtable_012bedcc : CVtable_012beda4, SAlignedScratch<64> {
    SDoubleBlock<64>  Block0;
    SDoubleBlock<64>  Block1;
    SDoubleBlock<64>* PFront;
    SDoubleBlock<64>* PBack;
    long              Count0;
    long              Count1;
    long              Unset65C;
    long              Unset660;
    long              Count2;
    long              Count3;
    CVtable_012bedcc();
    virtual ~CVtable_012bedcc() { }
};

CVtable_012bedcc::CVtable_012bedcc()
{
    memset(&Block0, 0, sizeof(Block0));
    memset(&Block1, 0, sizeof(Block1));
    Block0.A.Aligned = (float*)(((unsigned long)Block0.A.Raw + 15) & ~15);
    Block0.B.Aligned = (float*)(((unsigned long)Block0.B.Raw + 15) & ~15);
    Block1.A.Aligned = (float*)(((unsigned long)Block1.A.Raw + 15) & ~15);
    Block1.B.Aligned = (float*)(((unsigned long)Block1.B.Raw + 15) & ~15);
    PFront = &Block0;
    PBack = &Block1;
    Count2 = 0;
    Count3 = 0;
    Count0 = 0;
    Count1 = 0;
}

template <int N> static int AlignedInside(const SAlignedFloats<N>& f)
{
    const unsigned char* p = (const unsigned char*)f.Aligned;
    return ((unsigned long)p & 15) == 0 && p >= f.Raw && p + N * 4 <= f.Raw + sizeof(f.Raw);
}

int main()
{
    for (int round = 0; round < 3; ++round) {
        CVtable_012bedcc* obj = new CVtable_012bedcc;                   // heap objects land at varying 16-byte phases
        int ok = g_baseCtors == round + 1 && AlignedInside<64>(*obj) && AlignedInside<64>(obj->Block0.A) &&
                 AlignedInside<64>(obj->Block0.B) && AlignedInside<64>(obj->Block1.A) && AlignedInside<64>(obj->Block1.B) &&
                 obj->PFront == &obj->Block0 && obj->PBack == &obj->Block1 &&
                 obj->Count0 == 0 && obj->Count1 == 0 && obj->Count2 == 0 && obj->Count3 == 0 &&
                 obj->Block0.GapA[3] == 0 && obj->Block1.B.Raw[5] == 0 && obj->Block1.GapB[15] == 0;
        delete obj;
        if (!ok) { std::printf("00c70120_TEST FAIL\n"); return 1; }
    }
    std::printf("00c70120_TEST PASS\n");
    return 0;
}