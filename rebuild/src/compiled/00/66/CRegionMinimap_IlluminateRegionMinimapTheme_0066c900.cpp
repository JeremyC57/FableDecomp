// CRegionMinimap::IlluminateRegionMinimapTheme -- normalised directional illumination at a minimap position.
// Leaf: ?IlluminateRegionMinimapTheme@CRegionMinimap@@QBEMABVCWorldMap@@ABVC3DVector@@@Z
// Flags: (none)
// Inline-budget: the two original reciprocal-root computations expand at 0x0066C955 and 0x0066C9DE.
// Bracket: Group the final dot expression after the two original normalised vector copies.
#define FABLE_RETAIL_C3DVECTOR
#define FABLE_RETAIL_GFFASTONEOVERSQRT_ORIGINAL_UNION
#define FABLE_RETAIL_GFFASTONEOVERSQRT_ORIGINAL_UNION_SQUARE_PAREN_PASS26
#define FABLE_RETAIL_C3DVECTOR_DOT_ORIGINAL_SCOPE_PASS23
#define FABLE_RETAIL_C3DVECTOR_NORMALISE_SQUARE_HELPER
#define FABLE_RETAIL_C3DVECTOR_SQUARE_GROUPED_PASS23
#define FABLE_RETAIL_C3DVECTOR_SQUARE_ORIGINAL_FORMAL
#define FABLE_RETAIL_C3DVECTOR_ORIGINAL_COPY_FORMAL
#define FABLE_RETAIL_C3DVECTOR_ORIGINAL_COMPONENT_CTOR
#define FABLE_RETAIL_CWORLDMAP
#define FABLE_RETAIL_CWORLDMAP_GROUNDFACENORMAL
#define FABLE_RETAIL_CMAINGAMECOMPONENT
#define FABLE_RETAIL_C2DBOXI
#define FABLE_RETAIL_CREGIONMINIMAP
#define FABLE_RETAIL_CREGIONMINIMAP_POPULATE
#define FABLE_RETAIL_CREGIONMINIMAP_ILLUMINATION
#define FABLE_RETAIL_CREGIONMINIMAP_ILLUMINATION_GETTER_PASS26
#include "engine/C3DVector.h"
#include "engine/CWorldMap.h"
#include "engine/CRegionMinimap.h"
float CRegionMinimap::IlluminateRegionMinimapTheme(const CWorldMap& worldMap,const C3DVector& worldPos) const
{
 C3DVector faceNormal=worldMap.GetGroundFaceNormalAt(worldPos);
 C3DVector lightDir(0.0f,0.0f,0.0f);
 GetRegionMinimapIlluminationDirection(lightDir);
 C3DVector faceNormalNorm=faceNormal.Normalise();
 C3DVector lightDirNorm=lightDir.Normalise();
 return (lightDirNorm.Dot(faceNormalNorm));
}