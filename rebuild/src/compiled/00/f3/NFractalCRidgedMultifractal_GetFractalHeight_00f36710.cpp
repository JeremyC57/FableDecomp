// NFractal::CRidgedMultifractal::GetFractalHeight(const C2DVector& pos) -- Musgrave ridged multifractal over Perlin noise: the first octave's signal is (1 - |noise|)^2, each later octave scales the point by Lacunarity, weights its squared ridge by the previous signal * 2 clamped to [0, 1] and adds it times ExponentArray[i], while the weight stays above 0.001 and i < Octaves; the sum is scaled by 1/3.4 and clamped to [0, 1]
// Leaf: ?GetFractalHeight@CRidgedMultifractal@NFractal@@UBENABVC2DVector@@@Z
// Retail: Fable.exe (TLC, VC7.1) @ 0x00F36710, 327 bytes, thiscall `ret 4`, `and esp, -8` frame (double locals).
// Identity: Ego_r.pdb NFractal donor order: GetFractalHeight of CRidgedMultifractal 0x00BC8020 (0x170 bytes), CHybridMultifractal
//   0x00BC8190, CHeteroTerrainFractal 0x00BC8370; retail 0x00F36710 (0x147), then the manifest's CHybridMultifractal 0x00F36860
//   and CHeteroTerrainFractal 0x00F36A30. The body is the ridged algorithm (offset - |noise|, squared, weight = signal * gain
//   clamped), which neither the hybrid nor the hetero-terrain fractal computes. No data reference to the body: the TU's
//   vtables were discarded (the functions are not COMDATs).
// Callee: NFractal::CNoise::PerlinNoise(const C2DVector&) const 0x00F35C60 (manifest name; Ego_r 0x00BC7400 is first in the
//   group as 0x00F35C60 is in retail; `QBEMABVC2DVector@@@Z` = const, float, by const reference, as the call passes &point).
// Layout: Ego_r.pdb NFractal::CFractal, 0x420 bytes: vptr +0x0, double ExponentArray[128] +0x8, double Lacunarity +0x408,
//   double Octaves +0x410, CNoise* Noise +0x418; CRidgedMultifractal adds nothing (0x420). C2DVector {float X, Y}.
// Shape: the noise object is read once into a local (retail keeps it in ebx across both calls); the weight is stored first
//   and then clamped on the `signal * 2.0` value still in st(0) (testing `weight` itself reloads it from memory, 329v327);
//   the ExponentArray walk is VC's strength-reduced esi pointer.
// Constants: 1.0 0x01237F00, 0.0 0x0122ED70, 0.001 0x0126F430, (double)(1.0f / 3.4f) 0x01301CA0.
extern "C" double __cdecl fabs(double x);
#pragma intrinsic(fabs)

class C2DVector {
public:
    float X;
    float Y;
};

namespace NFractal {
class CNoise {
public:
    float PerlinNoise(const C2DVector& pos) const;            // 0x00F35C60
};

class CFractal {
public:
    virtual double GetFractalHeight(const C2DVector& pos) const = 0;
    double  ExponentArray[128];                                // +0x008
    double  Lacunarity;                                        // +0x408
    double  Octaves;                                           // +0x410
    CNoise* Noise;                                             // +0x418
};

class CRidgedMultifractal : public CFractal {
public:
    virtual double GetFractalHeight(const C2DVector& pos) const;
};

double CRidgedMultifractal::GetFractalHeight(const C2DVector& pos) const
{
    const CNoise* noise = Noise;
    C2DVector point = pos;
    double signal = noise->PerlinNoise(point);
    signal = fabs(signal);
    signal = 1.0 - signal;
    signal *= signal;
    double result = signal;
    double weight = 1.0;
    for (int i = 1; weight > 0.001 && i < Octaves; i++) {
        point.X *= Lacunarity;
        point.Y *= Lacunarity;
        weight = signal * 2.0;
        if (signal * 2.0 < 0.0)
            weight = 0.0;
        else if (signal * 2.0 > 1.0)
            weight = 1.0;
        signal = noise->PerlinNoise(point);
        signal = fabs(signal);
        signal = 1.0 - signal;
        signal *= signal;
        signal *= weight;
        result += signal * ExponentArray[i];
    }
    result *= 1.0f / 3.4f;
    if (result < 0.0)
        return 0.0;
    if (result > 1.0)
        return 1.0;
    return result;
}
}