// CEnvironment::GetRainStrength() const -- the blended weather theme's rain strength
// Leaf: ?GetRainStrength@CEnvironment@@QBEMXZ
// Retail: Fable.exe (TLC, VC7.1) @ 0x006B2BD0: fld dword ptr [ecx+1C4h] ; ret.
// Name: Ego_r.pdb 0x006EE650 + FableWin.pdb 0x0244E1E0 ?GetRainStrength@CEnvironment@@QBEMXZ; the landed 0x006B2260 CAtmosProcess::Update
//   binds it. The word is the game tick state's first blended theme rain scaler (engine/CEnvironment.h _WEATHERSCALERS: RainStrength
//   +0x1C4, a descriptive member name there). NF33 2026-11-16: was the stand-in C006b2bd0::M.
#define FABLE_RETAIL_CENVIRONMENT_RAINSTRENGTH
#define FABLE_RETAIL_CENVIRONMENT_WEATHERSCALERS
#include "engine/CEnvironment.h"
float CEnvironment::GetRainStrength() const
{
    return RainStrength;
}
