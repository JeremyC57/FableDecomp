// CRegion::SetMiniMapRegionExitTextOffsetX(const CCharString& exitName, float x) -- set the X of the minimap exit-text offset stored for the named region exit (the entry is created on demand)
// Leaf: ?SetMiniMapRegionExitTextOffsetX@CRegion@@QAEXABVCCharString@@M@Z
// Retail: Fable.exe (TLC, VC7.1) function @ 0x006BC4D0, length 22 bytes (`ret 8`).
// Identity: Ego_r.pdb ?SetMiniMapRegionExitTextOffsetX@CRegion@@QAEXABVCCharString@@M@Z (engine/CRegion.h LOADFROMFILE declares it).
// Callees: std::map<CCharString, C2DVector>::operator[](const CCharString&) @ 0x006BD6A0 (NF85, STLport spelling).
// Layout: engine/CRegion.h retail branch: MiniMapRegionExitTextOffsets +0x38. Twin: SetMiniMapRegionExitTextOffsetY @ 0x006BC4F0 stores the Y (+4).
// Shape: the key reference is forwarded, `this` is adjusted in place (`add ecx,38h`), and the float argument is copied with an integer
//   move into the returned C2DVector.
// NF87: moved off the row-local CRegion / C2DVector / exit-text map models onto engine/CRegion.h (FABLE_RETAIL_CREGION +
//   _LOADFROMFILE), so operator[] binds the STLport map<CCharString, C2DVector> spelling.
#define FABLE_RETAIL_CCHARSTRING
#define FABLE_RETAIL_C2DVECTOR
#define FABLE_RETAIL_STLP_MAP
#define FABLE_RETAIL_CREGION
#define FABLE_RETAIL_CREGION_LOADFROMFILE
#define FABLE_RETAIL_DEF_CLASS_BASE
#include "fable_string.h"
#include "stlport/fable_map.h"
#include "engine/C2DVector.h"
#include "engine/CRegion.h"

void CRegion::SetMiniMapRegionExitTextOffsetX(const CCharString& exitName, float x)
{
    MiniMapRegionExitTextOffsets[exitName].X = x;
}
