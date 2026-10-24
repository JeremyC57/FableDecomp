// CompareTriangleAverageDepth(const void*, const void*) -- qsort comparator over the 0x60-byte triangle records: -1 when the first triangle's mean vertex depth is greater, else 1 (back-to-front order)
// Leaf: ?CompareTriangleAverageDepth@
// Retail: Fable.exe (TLC, VC7.1) function @ 0x009D8CB0, length 58 bytes (+ int3 padding).
// Descriptive name: referenced only by `push 0x9d8cb0` in the CRenderManager2D flush at 0x009DA633, which qsorts
// (CRT 0x00BFEDCA) the vector at +0x3E70 with element size 0x60 = three 0x20-byte vertices whose depth float sits at
// +8 of each vertex. The mean is `(z2 + z1 + z0) * (1/3)` with the 1/3 float at 0x0125A4DC. Shape: both sums are
// built on the FPU before one `fcompp`, then `test ah,5 ; jnp` = `a > b`, results selected from immediates.
struct STriangleVertex
{
    float         x, y;
    float         z;                 // +0x08
    unsigned char _pad0C[0x20 - 0x0C];
};

struct STriangleRecord
{
    STriangleVertex v[3];            // +0x00, +0x20, +0x40
};

static inline float AverageDepth(const STriangleRecord* t)
{
    return (t->v[2].z + t->v[1].z + t->v[0].z) * (1.0f / 3.0f);
}

int __cdecl CompareTriangleAverageDepth(const void* left, const void* right)
{
    const STriangleRecord* a = (const STriangleRecord*)left;
    const STriangleRecord* b = (const STriangleRecord*)right;
    return AverageDepth(a) > AverageDepth(b) ? -1 : 1;
}