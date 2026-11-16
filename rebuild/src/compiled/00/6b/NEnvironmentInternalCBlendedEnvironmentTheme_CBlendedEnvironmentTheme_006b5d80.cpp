// NEnvironmentInternal::CBlendedEnvironmentTheme::~CBlendedEnvironmentTheme() -- frees the three texture sets' CArray buffers
//   (TextureSets[2..0], Blends then Textures each)
// Leaf: ??1CBlendedEnvironmentTheme@NEnvironmentInternal@@QAE@XZ
// Retail: Fable.exe (TLC, VC7.1) @ 0x006B5D80.
// Name: Ego_r.pdb 0x006F1D10 + FableWin.pdb 0x0244D5F0 ??1CBlendedEnvironmentTheme@NEnvironmentInternal@@QAE@XZ; the landed 0x006B3FF0
//   CEnvironment::Update (500-byte theme local, ctor 0x008864A0), 0x00888410, 0x008884D0, 0x008886E0, 0x0088A180 and the atexit stub
//   0x01229390 bind it. Over engine/CBlendedEnvironmentTheme.h (500 bytes; TextureSets at +0x1AC). NF33 2026-11-16: was the stand-in
//   CBufferPairTriple_006b5d80.
#include "engine/CBlendedEnvironmentTheme.h"
NEnvironmentInternal::CBlendedEnvironmentTheme::~CBlendedEnvironmentTheme()
{
}
