// CMap::GetThemeMaxCameraHeightAtPosition(const C2DVector& pos) -- the theme's maximum camera-size Z offset (the 0x0081FA90 twin of the Min routine) at the position's cell plus the interpolated terrain height there
// Leaf: ?GetThemeMinCameraSizeZAt@CMap@@IBEMABVC3DVector@@@Z
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00820140, length 80 bytes.
// Callees: __ftol2 @ 0x00BFEA70 (x2), CMap::GetThemeMaxCameraSizeZOffsetAt(C2DCoordI*) @ 0x0081FA90 (manifest label "GetThemeMinCameraSizeZOffsetAt" is the propagated twin), CMap::
// InterpolateHeightAtPosition(const C2DVector&) @ 0x0081EA50 (manifest "Terrain_InterpolateHeightAtPosition").
// Class attribution: manifest label "CGameScriptInterface::MsgIsActionModeButtonPressed" is propagated; `this` is handed
// unchanged to two CMap methods, so this is a CMap method.
// Shape: the C2DCoordI cell is built in the 8-byte frame from two __ftol2 conversions, the offset is a named float local
// parked in the dead argument slot (`fstp [esp+14h]`) across the second call and added last (`fadd [esp+14h]`).
// NF63 (round 7): moved onto engine/CMap.h (FABLE_RETAIL_CMAP_THEMESIZEZ in the overlay; the local CMap / C2DCoordI / C2DVector /
// grid models are gone). Self ?GetThemeMinCameraSizeZAt@CMap@@IBEMABVC3DVector@@@Z (protected const in Ego_r.pdb 0x0067B370 and FableWin.pdb 0x02232270; Ego_r order
// GetThemeSizeZAt(C3DVector) / GetThemeSizeZAt(C2DCoordI) / GetThemeMinCameraSizeZAt(C3DVector) = retail 0x008200B0 / 0x00820100 /
// 0x00820140, each calling its offset twin 0x0081FA20 / 0x0081FA20 / 0x0081FA90; the landed CWorldMap::GetThemeSizeZAt 0x0063A390
// calls 0x008200B0). Callees ?GetThemeMinCameraSizeZOffsetAt@CMap@@IBEMABVC2DCoordI@@@Z 0x0081FA90 and ?GetGroundSizeZAt@CMap@@IBEMABVC3DVector@@@Z 0x0081EA50 (both PDBs; was a made-up GetThemeMaxCameraSizeZOffsetAt(C2DCoordI*) / InterpolateHeightAtPosition(const C2DVector&)).
#define FABLE_RETAIL_CMAP_THEMESIZEZ
#define FABLE_RETAIL_CMAP_THEMEPALETTE
#define FABLE_RETAIL_CMAP_ENGINEACCESSORS
#define FABLE_RETAIL_CHEIGHTMAP_GETCELL
#define FABLE_RETAIL_CHEIGHTMAPCELL_ACCESSORS
#define FABLE_RETAIL_CHEIGHTMAPCELL
#define FABLE_RETAIL_C2DBOXI
#define FABLE_RETAIL_SCOPED_PTR
#include "engine/C3DVector.h"
#include "engine/CMap.h"
#include "engine/CHeightMap.h"
float CMap::GetThemeMinCameraSizeZAt(const C3DVector& pos) const
{
    C2DCoordI cell;
    cell.X = (long)pos.X;
    cell.Y = (long)pos.Y;
    float offset = GetThemeMinCameraSizeZOffsetAt(cell);
    return offset + GetGroundSizeZAt(pos);
}
