// Grade: DIFFER(361v361) -- scheduling only: inside the octave loop retail loads the counter (mov ecx,[esp+0Ch]) before the first fmul and stores it after the second; ours interleaves the fmuls one slot earlier
// CHeteroTerrainFractal::GetFractalHeight(const C2DVector& position) const -- hetero-terrain multifractal height at a 2D position: Perlin noise at the first octave, then each further octave adds noise * spectral weight * the running value (the position scaled by the lacunarity each time), a fractional last octave adds its share, and the result is remapped by (h + 0.48) * 2/3 and clamped to [0,1]
// Leaf: GetFractalHeight
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00F36A30, length 359 bytes, `ret 4`, 8-byte aligned frame (`and esp,-8`).
// Callees: CNoise::PerlinNoise(const C2DVector&) @ 0x00F35C60 (float), __ftol2 @ 0x00BFEA70.
//   Double pool: 1.0 @ 0x01237F00, 0.0 @ 0x0122ED70, 0.48 @ 0x01301CC8, 2/3 @ 0x01301CC0.
// Layout: Ego_r.pdb CFractal (1056 bytes): vptr, ExponentArray double[128] +0x08, Lacunarity +0x408, Octaves +0x410,
//   Noise +0x418. No engine header.
// Shape: Octaves is copied into a local double for the loop test; the octave increment is its own statement (inline
//   `value += n * w * value` is refactored by VC7.1 into `value * (n * w + 1.0)`); the octave counter is a long; its weight pointer is strength-reduced from
//   ExponentArray[1]; the position components are scaled in double precision and stored back as floats.
struct C2DVector {
    float X, Y;
};
struct CNoise {
    float PerlinNoise(const C2DVector& position);  // 0x00F35C60
};
class CFractal {
public:
    virtual ~CFractal();
    double  ExponentArray[128];                    // +0x08
    double  Lacunarity;                            // +0x408
    double  Octaves;                               // +0x410
    CNoise* Noise;                                 // +0x418
};
class CHeteroTerrainFractal : public CFractal {
public:
    double GetFractalHeight(const C2DVector& position) const;
};

double CHeteroTerrainFractal::GetFractalHeight(const C2DVector& position) const
{
    C2DVector p = position;
    CNoise* noise = Noise;
    double value = noise->PerlinNoise(p);
    p.X *= Lacunarity;
    p.Y *= Lacunarity;
    double octaves = Octaves;
    long i;
    for (i = 1; i < octaves; ++i) {
        double increment = noise->PerlinNoise(p) * ExponentArray[i] * value;
        value += increment;
        p.X *= Lacunarity;
        p.Y *= Lacunarity;
    }
    double remainder = Octaves - (long)Octaves;
    if (remainder != 0.0)
        value *= noise->PerlinNoise(p) * ExponentArray[i] * remainder + 1.0;
    double height = (value + 0.48) * (2.0 / 3.0);
    if (height < 0.0)
        return 0.0;
    if (height > 1.0)
        return 1.0;
    return height;
}
