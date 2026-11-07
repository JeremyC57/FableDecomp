// CHeteroTerrainFractal::GetFractalHeight(const C2DVector& position) const -- hetero-terrain multifractal height at a 2D position: Perlin noise at the first octave, then each further octave adds noise * spectral weight * the running value (the position scaled by the lacunarity each time), a fractional last octave adds its share, and the result is remapped by (h + 0.48) * 2/3 and clamped to [0,1]
// Grade: DIFFER(361v361) -- loop-counter scheduling: retail reloads the octave counter (`mov ecx,[esp+0Ch]`) before `fmul [edi]`, ours after; 4 masked lines, 60 attempts this pass (chunk 847) after 59 earlier; chunk 859: 16 more (135 total): post-increment index, while form, index local / int& / unsigned, a struct-held counter, a noise local, pointer walks (md8), value-first multiplies (363 B) all keep the slot; the Ego_r spelling (NFractal namespace, PerlinNoise const, GetFractalHeight virtual const, leaf ?GetFractalHeight@CHeteroTerrainFractal@NFractal@@UBENABVC2DVector@@@Z) is byte-identical (this file); a same-TU stand-in body for CNoise::PerlinNoise (0x00F35C60 is in the same TU by address) gives 350 B / md84, so retail compiled with PerlinNoise declared only; chunk 859 total 126 grades, cross-product grids of behaviour-identical spellings (increment forms (4) x while/for x int/long counter x lacunarity local x member-wise p copy x remainder forms (3) x Noise direct (110 of 192)): best unchanged
// Next idea: a post-RA scheduler slot tie on the counter reload around `fmul [edi]`; nothing textual left -- a documented residue unless a lane-level scheduling answer appears
// Was: DIFFER(ours v retail) -- loop-counter scheduling: retail reloads the counter (`mov ecx,[esp+0Ch]`) before the fmul, ours after; best build DIFFER(361v361), 4 masked diff lines (chunk 757, 10 attempts)
// Leaf: ?GetFractalHeight@CHeteroTerrainFractal@NFractal@@UBENABVC2DVector@@@Z
// Flags: none
// Earlier note: DIFFER(361v361) -- scheduling only: inside the octave loop retail loads the counter (mov ecx,[esp+0Ch]) before the first fmul and stores it after the second; ours interleaves the fmuls one slot earlier
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00F36A30, length 359 bytes, `ret 4`, 8-byte aligned frame (`and esp,-8`).
// Callees: CNoise::PerlinNoise(const C2DVector&) @ 0x00F35C60 (float), __ftol2 @ 0x00BFEA70.
//   Double pool: 1.0 @ 0x01237F00, 0.0 @ 0x0122ED70, 0.48 @ 0x01301CC8, 2/3 @ 0x01301CC0.
// Layout: Ego_r.pdb CFractal (1056 bytes): vptr, ExponentArray double[128] +0x08, Lacunarity +0x408, Octaves +0x410,
//   Noise +0x418. No engine header.
// Shape: Octaves is copied into a local double for the loop test; the octave increment is its own statement (inline
//   `value += n * w * value` is refactored by VC7.1 into `value * (n * w + 1.0)`); the octave counter is a long; its weight pointer is strength-reduced from
//   ExponentArray[1]; the position components are scaled in double precision and stored back as floats.
class C2DVector {
public:
    float X, Y;
};
namespace NFractal {
class CNoise {
public:
    float PerlinNoise(const C2DVector& position) const;   // 0x00F35C60 (Ego_r ?PerlinNoise@CNoise@NFractal@@QBEMABVC2DVector@@@Z)
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
    virtual double GetFractalHeight(const C2DVector& position) const;   // Ego_r ?GetFractalHeight@CHeteroTerrainFractal@NFractal@@UBENABVC2DVector@@@Z
};
}

double NFractal::CHeteroTerrainFractal::GetFractalHeight(const C2DVector& position) const
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
