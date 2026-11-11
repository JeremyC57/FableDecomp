// NFractal::CNoise::CNoise() -- constructor: clear the seed, then build the Perlin permutation and gradient tables
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00F36BA0, length 22 bytes. Callee: NFractal::CNoise::InitPerlinNoise @ 0x00F35E00.
// Name (agentNF20, 2026-11-11): Ego_r.pdb / FableWin.pdb ??0CNoise@NFractal@@QAE@XZ (the no-argument ctor: plain `ret`, Seed = 0
// immediate); the class is the shared engine/NFractal_CNoise.h (opt-in FABLE_RETAIL_NFRACTAL_CNOISE_CTOR declares the ctor).
// Layout from the PDB (CNoise, 6148 bytes): P[512] +0x0000, G2[512] (C2DVector) +0x0800, Seed +0x1800. Shape: the
// seed is an initialiser (an immediate store before the call), the table fill is the body call on `this`.
// Leaf: ??0CNoise@NFractal@@QAE@XZ
#define FABLE_RETAIL_NFRACTAL_CNOISE_CTOR
#include "engine/NFractal_CNoise.h"

NFractal::CNoise::CNoise()
    : Seed(0)
{
    InitPerlinNoise();
}