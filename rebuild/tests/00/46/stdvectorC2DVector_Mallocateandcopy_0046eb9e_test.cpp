// Behaviour test for 0x0046EB9E vector<C2DVector>::_M_allocate_and_copy: n == 0 gives no block, else malloc n and copy-construct the range
#include <cstdio>
#include <cstdlib>
#include <new>
struct C2DVector { float X; float Y; };
static C2DVector* AllocateAndCopy(unsigned int n, const C2DVector* first, const C2DVector* last)
{
    C2DVector* result = n != 0 ? (C2DVector*)std::malloc(n * sizeof(C2DVector)) : 0;
    C2DVector* cur = result;
    for (; first != last; ++first, ++cur)
        if (cur) new (cur) C2DVector(*first);
    return result;
}
int main()
{
    C2DVector src[3] = { { 1.0f, 2.0f }, { 3.5f, -4.0f }, { 0.25f, 8.0f } };
    C2DVector* block = AllocateAndCopy(4, src, src + 3);
    bool ok = block != 0 && block[0].X == 1.0f && block[1].Y == -4.0f && block[2].X == 0.25f && block[2].Y == 8.0f;
    std::free(block);
    ok = ok && AllocateAndCopy(0, src, src) == 0;
    if (!ok) { std::printf("0046eb9e_TEST FAIL\n"); return 1; }
    std::printf("0046eb9e_TEST PASS\n");
    return 0;
}