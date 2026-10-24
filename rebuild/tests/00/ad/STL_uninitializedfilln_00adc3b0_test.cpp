// Behaviour test for 0x00ADC3B0 __uninitialized_fill_n over CVertexGroupBlock (n copies, each with its own group storage).
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <new>

static int g_treeCopies = 0;
struct STree { void* Header; unsigned int Count; int Compare;
    STree(const STree& o) : Header(o.Header), Count(o.Count), Compare(o.Compare) { ++g_treeCopies; } };   // stand-in for 0x00A96E40
struct SBlock {
    unsigned long* Start; unsigned long* Finish; unsigned long* End; STree Map;
    SBlock(const SBlock& x) : Map(x.Map)
    {
        unsigned int n = (unsigned int)(x.Finish - x.Start);
        Start = Finish = End = 0;
        Start = n ? (unsigned long*)std::malloc(n * sizeof(unsigned long)) : 0;
        Finish = Start; End = Start + n;
        Finish = x.Finish == x.Start ? Start : (unsigned long*)((char*)std::memmove(Start, x.Start, (char*)x.Finish - (char*)x.Start) + ((char*)x.Finish - (char*)x.Start));
    }
};
// reconstructed body
static SBlock* UninitializedFillN(SBlock* first, unsigned int n, const SBlock& x)
{
    SBlock* cur = first;
    for (; n > 0; --n, ++cur) new (cur) SBlock(x);
    return cur;
}

int main()
{
    unsigned long words[2] = { 23, 42 };
    char proto[sizeof(SBlock)];
    SBlock* x = (SBlock*)proto;
    x->Start = words; x->Finish = words + 2; x->End = words + 2; x->Map.Header = (void*)0x30; x->Map.Count = 2;
    char out[4 * sizeof(SBlock)];
    SBlock* dst = (SBlock*)out;
    SBlock* end = UninitializedFillN(dst, 4, *x);
    if (end != dst + 4 || g_treeCopies != 4) { std::printf("00adc3b0_TEST FAIL\n"); return 1; }
    for (int i = 0; i < 4; ++i) {
        if (dst[i].Finish - dst[i].Start != 2 || dst[i].Start[0] != 23 || dst[i].Start[1] != 42 || dst[i].Map.Header != (void*)0x30) { std::printf("00adc3b0_TEST FAIL\n"); return 1; }
        if (i > 0 && dst[i].Start == dst[i - 1].Start) { std::printf("00adc3b0_TEST FAIL\n"); return 1; }
    }
    if (UninitializedFillN(dst, 0, *x) != dst || g_treeCopies != 4) { std::printf("00adc3b0_TEST FAIL\n"); return 1; }
    for (int i = 0; i < 4; ++i) std::free(dst[i].Start);
    std::printf("00adc3b0_TEST PASS\n");
    return 0;
}