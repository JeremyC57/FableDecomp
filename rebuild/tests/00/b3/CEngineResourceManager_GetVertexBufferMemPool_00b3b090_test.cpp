#include <cstdio>
int __fastcall GetVertexBufferMemPool()
{
    return 1;
}
int main()
{
    if (GetVertexBufferMemPool() == 1) { std::printf("AUTO_TINY_00b3b090_TEST PASS\n"); return 0; }
    std::printf("AUTO_TINY_00b3b090_TEST FAIL\n");
    return 1;
}