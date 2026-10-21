// CHybridMultifractal::GetFractalHeight(const C2DVector& pos) -- Musgrave hybrid multifractal over the 2D Perlin noise: a cosine-shaped first octave plus offset, then octaves weighted by the running weight (clamped to 1) while the weight stays above 0.001, a fractional last octave, and the result rescaled by (h + 0.3) / 3.2 and clamped to [0, 1]
// Grade: DIFFER(461v451) -- x87 value caching only: retail keeps `weight` live in st(0) after its `fst` stores (the 0.001 loop test is `fcom` with no reload, and the exit pops it with `fstp st(0)`); ours stores with `fstp` and reloads weight/result for each test. Constants, call order and loop shape match
// Leaf: GetFractalHeight
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00F36860, length 449 bytes (`ret 4`).
// Callees: CNoise::PerlinNoise(C2DVector*) @ 0x00F35C60 (returns float), _ftol @ 0x00BFEA70.
// Layout: Ego_r.pdb CFractal (0x420, base of CHybridMultifractal): ExponentArray double[128] +0x08, Lacunarity +0x408,
//   Octaves +0x410, Noise +0x418. Constants: 1.0, 0.5, 0.25, (double)6.2831855f, offset 0.7f, 0.001, 0.0, 0.3f, 1/3.2f.
#include <math.h>
struct C2DVector { float X, Y; };
class CNoise {
public:
    float PerlinNoise(C2DVector* pos);             // 0x00F35C60
};
class CFractal {
public:
    virtual void v00();
    double  ExponentArray[128];                    // +0x08
    double  Lacunarity;                            // +0x408
    double  Octaves;                               // +0x410
    CNoise* Noise;                                 // +0x418
};
class CHybridMultifractal : public CFractal {
public:
    double GetFractalHeight(const C2DVector& pos);
};

double CHybridMultifractal::GetFractalHeight(const C2DVector& pos)
{
    const double offset = 0.7f;
    C2DVector point = pos;
    CNoise* noise = Noise;
    double height = (noise->PerlinNoise(&point) + 1.0) * 0.5;
    height *= 0.25;
    height = (1.0 - cos(height * 6.2831855f)) * 2.0 - 1.0;
    double result = height + offset;
    double weight = result;
    point.X = (float)(point.X * Lacunarity);
    point.Y = (float)(point.Y * Lacunarity);
    int i;
    for (i = 1; weight > 0.001 && i < Octaves; i++) {
        if (!(weight < 1.0))
            weight = 1.0;
        double signal = (noise->PerlinNoise(&point) + offset) * ExponentArray[i];
        result += weight * signal;
        weight *= signal;
        point.X = (float)(point.X * Lacunarity);
        point.Y = (float)(point.Y * Lacunarity);
    }
    double remainder = Octaves - (int)Octaves;
    if (remainder != 0.0)
        result += noise->PerlinNoise(&point) * ExponentArray[i] * remainder;
    result = (result + 0.3f) / 3.2f;
    if (result < 0.0)
        return 0.0;
    if (result > 1.0)
        result = 1.0;
    return result;
}
