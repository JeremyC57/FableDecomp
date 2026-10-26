// CNoise::CNoise() -- constructor: clear the seed, then build the Perlin permutation and gradient tables
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00F36BA0, length 22 bytes. Callee: CNoise::InitPerlinNoise @ 0x00F35E00.
// Layout from the PDB (CNoise, 6148 bytes): P[512] +0x0000, G2[512] (C2DVector) +0x0800, Seed +0x1800. Shape: the
// seed is an initialiser (an immediate store before the call), the table fill is the body call on `this`.
// Leaf: ??0CNoise
class C2DVector { public:
    float x;
    float y;
};

class CNoise {
public:
    CNoise();
    void InitPerlinNoise();                  // 0x00F35E00
    long          P[512];                    // +0x0000
    C2DVector     G2[512];                   // +0x0800
    unsigned long Seed;                      // +0x1800
};

CNoise::CNoise()
    : Seed(0)
{
    InitPerlinNoise();
}