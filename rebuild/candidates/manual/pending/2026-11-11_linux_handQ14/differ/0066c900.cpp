// CRegionMinimap::IlluminateRegionMinimapTheme(const CWorldMap& worldMap, const C3DVector& position) const -- the light on the region minimap at a position: the dot of the ground face normal there with the minimap's illumination direction, both normalised
// Grade: DIFFER(ours v retail) -- 346v354 on engine/CRegionMinimap.h (new opt-in FABLE_RETAIL_CREGIONMINIMAP_ILLUMINATION) + CWorldMap.h (new opt-in FABLE_RETAIL_CWORLDMAP_GROUNDFACENORMAL) + C3DVector Z,Y,X sums (SQUAREDMAGNITUDE_ZYX, DOT_ORDERED_ZYX): retail copies IlluminationDirection through a pointer (add esi,3Ch; three integer moves) interleaved with the normal's squared sum, ours loads X/Y as floats; retail also keeps light.Z' alive for `fmul st(1)` and pops it at the end. The local-model draft (348v354) copied through a cast pointer and matched the copy, so the residue is the C3DVector copy constructor's member-wise copy.
// Leaf: ?IlluminateRegionMinimapTheme@CRegionMinimap@@QBEMABVCWorldMap@@ABVC3DVector@@@Z
// Retail: Fable.exe (TLC, VC7.1) function @ 0x0066C900, length 354 bytes, __thiscall, `ret 8`.
// Callee: CWorldMap::GetGroundFaceNormalAt(const C3DVector&) const 0x0063AB10 (hidden result).
#define FABLE_RETAIL_C3DVECTOR
#define FABLE_RETAIL_C3DVECTOR_SQUAREDMAGNITUDE_ZYX
#define FABLE_RETAIL_C3DVECTOR_DOT_ORDERED_ZYX
#define FABLE_RETAIL_CWORLDMAP
#define FABLE_RETAIL_CWORLDMAP_GROUNDFACENORMAL
#define FABLE_RETAIL_CMAINGAMECOMPONENT
#define FABLE_RETAIL_C2DBOXI
#define FABLE_RETAIL_CREGIONMINIMAP
#define FABLE_RETAIL_CREGIONMINIMAP_POPULATE
#define FABLE_RETAIL_CREGIONMINIMAP_ILLUMINATION
#include "engine/C3DVector.h"
#include "engine/CWorldMap.h"
#include "engine/CRegionMinimap.h"

float CRegionMinimap::IlluminateRegionMinimapTheme(const CWorldMap& worldMap, const C3DVector& position) const
{
    C3DVector normal = worldMap.GetGroundFaceNormalAt(position);
    C3DVector light = IlluminationDirection;
    normal.Normalise();
    light.Normalise();
    return normal.Dot(light);
}
