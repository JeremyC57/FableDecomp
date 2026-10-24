// Behaviour test for _STL::vector<COptimisedVertex::CGroup>::_M_allocate_and_copy<const CGroup*> @ 0x00AD04E0
#include <cstdio>
#include <cstdlib>
#include <new>

struct CGroup { unsigned int Bone; float Weight; };

static CGroup* allocate_and_copy(unsigned int n, const CGroup* first, const CGroup* last)
{
    CGroup* result = n != 0 ? (CGroup*)std::malloc(n * sizeof(CGroup)) : 0;
    CGroup* cur = result;
    for ( ; first != last; ++first, ++cur)
        new (cur) CGroup(*first);
    return result;
}

int main()
{
    CGroup src[3] = { { 1, 0.5f }, { 4, 0.25f }, { 9, 0.25f } };
    CGroup* block = allocate_and_copy(5, src, src + 3);
    int ok = block != 0 && sizeof(CGroup) == 8u;
    for (int i = 0; i < 3 && ok; ++i) {
        float d = block[i].Weight - src[i].Weight;
        if (block[i].Bone != src[i].Bone || d > 0.0001f || d < -0.0001f) ok = 0;
    }
    if (allocate_and_copy(0, src, src) != 0) ok = 0;
    std::free(block);
    if (!ok) { std::printf("00ad04e0_TEST FAIL\n"); return 1; }
    std::printf("00ad04e0_TEST PASS\n");
    return 0;
}