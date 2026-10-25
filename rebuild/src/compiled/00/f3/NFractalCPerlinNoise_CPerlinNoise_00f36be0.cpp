// NFractal::CPerlinNoise::CPerlinNoise(unsigned long seed) -- build the permutation tables for the given seed through Initialise
// Leaf: ??0CPerlinNoise@NFractal@@QAE@K@Z
// Retail: Fable.exe (TLC, VC7.1) @ 0x00F36BE0, 19 bytes, __fastcall `ret 4`, returns `this`.
// Identity: FableWin.pdb ??0CPerlinNoise@NFractal@@QAE@K@Z next to ?Initialise@CPerlinNoise@NFractal@@QAEXK@Z (0x00F35ED0,
//   which fills the 256-entry permutation from the seed through the LCG at 0x00497890). Callers: the
//   NParticleEngine::CParticleEmitterManager constructor 0x00AEBA90 (`PerlinNoise(0)` at +8) and the
//   CEngineSimulationManager constructor 0x00B81BC0 (`PerlinNoise(0)` at +0x28); both landed sources already declare this
//   address as CPerlinNoise(unsigned long). The catalog's "operator&&<CPredicate_And<...>, CIsThingAlive>" label is propagated.
// Callee: NFractal::CPerlinNoise::Initialise(unsigned long) @ 0x00F35ED0.
// Model: 0x1000 bytes (the landed callers' member spacing); the tables are opaque to this body.
namespace NFractal {
class CPerlinNoise {
public:
    long  Permutation[0x100];               // +0x000 (filled by Initialise)
    float Tables[0x300];                    // +0x400 (gradient tables, opaque here)
    CPerlinNoise(unsigned long seed);
    void Initialise(unsigned long seed);    // 0x00F35ED0
};

CPerlinNoise::CPerlinNoise(unsigned long seed)
{
    Initialise(seed);
}
}