// Behaviour test for 0x0075B800 vector<C3DVector>::_M_allocate_and_copy: a block of n vectors is allocated (null
// for n == 0) and [first, last) is copy-constructed into its front.
#include <cstdio>
#include <cstdlib>
#include <new>

class C3DVector { public:
    float X, Y, Z;
    C3DVector(float x, float y, float z) : X(x), Y(y), Z(z) { }
    C3DVector(const C3DVector& o) : X(o.X), Y(o.Y), Z(o.Z) { }
};

static C3DVector* AllocateAndCopy(unsigned int n, const C3DVector* first, const C3DVector* last)
{
    C3DVector* result = n != 0 ? (C3DVector*)std::malloc(n * sizeof(C3DVector)) : 0;
    C3DVector* cur = result;
    for (; first != last; ++first, ++cur)
        if (cur) new (cur) C3DVector(*first);
    return result;
}

int main()
{
    C3DVector src[3] = { C3DVector(1.0f, 2.0f, 3.0f), C3DVector(4.0f, 5.0f, 6.0f), C3DVector(7.0f, 8.0f, 9.0f) };
    C3DVector* block = AllocateAndCopy(5u, src, src + 3);
    int ok = block != 0 && block != src;
    if (ok && (block[0].X != 1.0f || block[1].Y != 5.0f || block[2].Z != 9.0f)) ok = 0;
    std::free(block);
    if (AllocateAndCopy(0u, src, src) != 0) ok = 0;
    if (!ok) { std::printf("0075b800_TEST FAIL\n"); return 1; }
    std::printf("0075b800_TEST PASS\n");
    return 0;
}