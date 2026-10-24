// Behaviour test for 0x00ADC440 __uninitialized_copy over CVertexGroupBlock (group words copied, tree copy constructor per block).
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <new>

static int g_treeCopies = 0;
struct STree { void* Header; unsigned int Count; int Compare;
    STree(const STree& o) : Header(o.Header), Count(o.Count), Compare(o.Compare) { ++g_treeCopies; } };   // stand-in for 0x00A96E40
struct SBlock {
    unsigned long* Start; unsigned long* Finish; unsigned long* End; STree Map;
    SBlock(const SBlock& x) : Map(x.Map)   // reconstructed implicit copy: vector copy then map copy
    {
        unsigned int n = (unsigned int)(x.Finish - x.Start);
        Start = Finish = End = 0;
        Start = n ? (unsigned long*)std::malloc(n * sizeof(unsigned long)) : 0;
        Finish = Start; End = Start + n;
        Finish = x.Finish == x.Start ? Start : (unsigned long*)((char*)std::memmove(Start, x.Start, (char*)x.Finish - (char*)x.Start) + ((char*)x.Finish - (char*)x.Start));
    }
};
// reconstructed body
static SBlock* UninitializedCopy(const SBlock* first, const SBlock* last, SBlock* result)
{
    SBlock* cur = result;
    for (; first != last; ++first, ++cur) new (cur) SBlock(*first);
    return cur;
}

int main()
{
    unsigned long words[3] = { 4, 8, 15 };
    char storage[3 * sizeof(SBlock)];
    SBlock* src = (SBlock*)storage;
    src[0].Start = words; src[0].Finish = words + 3; src[0].End = words + 3; src[0].Map.Header = (void*)0x10; src[0].Map.Count = 3;
    src[1].Start = src[1].Finish = src[1].End = 0; src[1].Map.Header = (void*)0x20; src[1].Map.Count = 0;
    char out[3 * sizeof(SBlock)];
    SBlock* dst = (SBlock*)out;
    SBlock* end = UninitializedCopy(src, src + 2, dst);
    if (end != dst + 2 || g_treeCopies != 2) { std::printf("00adc440_TEST FAIL\n"); return 1; }
    if (dst[0].Finish - dst[0].Start != 3 || dst[0].End != dst[0].Finish || dst[0].Start == words || dst[0].Start[2] != 15 || dst[0].Map.Count != 3) { std::printf("00adc440_TEST FAIL\n"); return 1; }
    if (dst[1].Start != 0 || dst[1].Finish != 0 || dst[1].End != 0 || dst[1].Map.Header != (void*)0x20) { std::printf("00adc440_TEST FAIL\n"); return 1; }
    if (UninitializedCopy(src, src, dst) != dst || g_treeCopies != 2) { std::printf("00adc440_TEST FAIL\n"); return 1; }
    std::free(dst[0].Start);
    std::printf("00adc440_TEST PASS\n");
    return 0;
}