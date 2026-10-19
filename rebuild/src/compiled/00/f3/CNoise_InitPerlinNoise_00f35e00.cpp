// CNoise::InitPerlinNoise() -- build the Perlin tables from the noise's own seed: identity permutation with random unit gradients, a Fisher-Yates shuffle of the permutation, then both tables duplicated into their upper halves for wrap-free lookups
// Leaf: InitPerlinNoise
// Retail: Fable.exe (TLC, VC7.1) @ 0x00F35E00, 204 bytes (+ int3 padding). __fastcall, `ret`.
// Callees: GFFloatRandom(float range, unsigned long* seed) 0x004978A0 (__fastcall: seed in ecx, range on the stack; as landed
//   COrbitState::Initialise 0x00B161A0), C2DVector::Normalise() 0x00A14540, GFROR13(unsigned long) 0x00497890 (__fastcall); 1.0f
//   0x0122DED8.
// Layout = Ego_r.pdb CNoise (6148 bytes): P[512] +0x0000, G2[512] (C2DVector) +0x0800, Seed +0x1800 (as the landed ctor 0x00F36BA0).
// Shape: each gradient component is `GFFloatRandom(2, &Seed) - 1` (the seed address and the gradient pointer strength-reduced
//   into ebx/ebp); the shuffle runs `while (--i)` from 255 with the inline LCG step `Seed = GFROR13(Seed * 0x24A1 + 0x24DF)`
//   and a 0..255 pick; the duplication copies P then G2 per index.
float __fastcall GFFloatRandom(float range, unsigned long* seed);   // 0x004978A0
long __fastcall GFROR13(unsigned long value);                       // 0x00497890
struct C2DVector {
    float X, Y;
    C2DVector& Normalise();                        // 0x00A14540
};
class CNoise {
public:
    CNoise();
    void InitPerlinNoise();
    long          P[512];                          // +0x0000
    C2DVector     G2[512];                         // +0x0800
    unsigned long Seed;                            // +0x1800
};

void CNoise::InitPerlinNoise()
{
    unsigned long i;
    for (i = 0; i < 256; ++i) {
        P[i] = i;
        G2[i].X = GFFloatRandom(2.0f, &Seed) - 1.0f;
        G2[i].Y = GFFloatRandom(2.0f, &Seed) - 1.0f;
        G2[i].Normalise();
    }
    while (--i) {
        long k = P[i];
        Seed = Seed * 0x24A1 + 0x24DF;
        Seed = GFROR13(Seed);
        unsigned long j = Seed & 0xff;
        P[i] = P[j];
        P[j] = k;
    }
    for (i = 0; i < 256; ++i) {
        P[256 + i] = P[i];
        G2[256 + i] = G2[i];
    }
}