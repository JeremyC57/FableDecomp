// NFractal::CFractal::NewFractalData(double lacunarity, double fractalIncrement, double octaves) -- store the lacunarity and octave count and rebuild the 128 spectral weights ExponentArray[i] = lacunarity^(-i * fractalIncrement)
// Leaf: ?NewFractalData@CFractal@NFractal@@QAEXNNN@Z
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00F35DA0, 90 bytes, thiscall, `ret 18h` (three doubles).
// Identity (proven): called only by the two CFractal constructors 0x00F36690 (CFractal(CNoise&): defaults) and 0x00F366D0
//   (CFractal(CNoise&, double, double, double): its three doubles passed on in order), the Ego_r.pdb pair
//   ??0CFractal@NFractal@@QAE@AAVCNoise@1@@Z / ??0CFractal@NFractal@@QAE@AAVCNoise@1@NNN@Z, whose only (double, double, double)
//   member is ?NewFractalData@CFractal@NFractal@@QAEXNNN@Z (Ego_r.pdb and FableWin.pdb); it writes exactly the PDB members
//   ExponentArray (double[128] +0x08), Lacunarity (+0x408) and Octaves (+0x410). Parameter names are descriptive.
// Label: the manifest's "sub_00f35da0" is a gap-scan placeholder.
// Callees: the CRT pow intrinsic _CIpow 0x00BFEB40.
// Layout: Ego_r.pdb CFractal (0x420 bytes): vptr (vtable 0x01301C9C), ExponentArray +0x08, Lacunarity +0x408, Octaves +0x410, Noise +0x418.
// Shape: the loop runs on a pointer and a 128 down-counter, and the exponent is its own decrementing int (kept in the dead
//   first-argument slot for the `fild`); written as `-i` instead, VC7.1 folds the index into that one variable and compares it
//   with -128. Lacunarity is re-read from the member every pass.
#include <math.h>

namespace NFractal {
class CNoise;
class CFractal {
public:
    virtual ~CFractal();
    double  ExponentArray[128];                           // +0x08
    double  Lacunarity;                                   // +0x408
    double  Octaves;                                      // +0x410
    CNoise* Noise;                                        // +0x418
    void NewFractalData(double lacunarity, double fractalIncrement, double octaves);
};
}

void NFractal::CFractal::NewFractalData(double lacunarity, double fractalIncrement, double octaves)
{
    Lacunarity = lacunarity;
    Octaves = octaves;
    long exponent = 0;
    for (long i = 0; i < 128; ++i) {
        ExponentArray[i] = pow(Lacunarity, exponent * fractalIncrement);
        --exponent;
    }
}