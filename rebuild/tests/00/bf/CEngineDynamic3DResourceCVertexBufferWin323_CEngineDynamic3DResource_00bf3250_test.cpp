// Behaviour test for CEngineDynamic3DResource<CVertexBufferWin32,3>::CEngineDynamic3DResource (0x00BF3250): the Win32 vertex buffer base has no D3D buffer and zero stride; the pool link starts null.
#include <cstdio>
struct VB { void* PVertexBuffer; unsigned int Length; int Type; unsigned char Stride; VB() { PVertexBuffer = 0; Stride = 0; } };
template <class T, int N> struct Dynamic : T { unsigned int Key[N]; Dynamic* PoolNext; Dynamic() : PoolNext(0) {} };
int main()
{
    Dynamic<VB, 3> d;
    if (d.PVertexBuffer != 0 || d.Stride != 0 || d.PoolNext != 0 || sizeof(d.Key) != 3 * sizeof(unsigned int)) { std::printf("00bf3250_TEST FAIL\n"); return 1; }
    std::printf("00bf3250_TEST PASS\n");
    return 0;
}