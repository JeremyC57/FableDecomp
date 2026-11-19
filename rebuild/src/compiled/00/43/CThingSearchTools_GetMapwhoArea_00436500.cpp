// CThingSearchTools::GetMapwhoArea(const NThingMap::CThingMap& map, long layer, const C2DBoxI& area) const -- converts a world box into the mapwho cells of `layer` (min edges shifted down, max edges rounded up)
// Leaf: ?GetMapwhoArea@CThingSearchTools@@IBE?AVC2DBoxI@@ABVCThingMap@NThingMap@@JABV2@@Z
// Retail: Fable.exe 0x00436500, 66 bytes (`ret 10h`: hidden result pointer + three arguments; `this` and the CThingMap argument are unused):
//   the out-of-line copy of the protected inline the rough-area searches expand.
// Data: NThingMap::LayerInfo 0x0137DAD8 (16-byte CLayerInfo entries, ScaleShift at +4).
// Name fix (agentNF94, q11281 / q11282): Ego_r.pdb 0x00449BD0 + FableWin.pdb 0x01AE35F0
//   ?GetMapwhoArea@CThingSearchTools@@IBE?AVC2DBoxI@@ABVCThingMap@NThingMap@@JABV2@@Z (protected const inline; engine/CThingSearchTools.h
//   ROUGHAREA_EGOD carries the body, whose note records that the same text reproduces this copy). The old row defined
//   CThingSearchToolsMethods::GetMapwhoArea(CThingMap*, long, const C2DBoxI&) on local C2DBoxI / SMapwhoLevel models and read the table as
//   g_mapwhoLevels (Ego_d.pdb: ?LayerInfo@NThingMap@@3QBVCLayerInfo@1@B, NThingMap_CThingMap.h). This row now only takes the header body's
//   address from an anonymous derived class (protected member), so VC7.1 emits it here as a pick-any COMDAT (nf86 / nf88 scaffolding).
#define FABLE_RETAIL_CTHINGSEARCHTOOLS
#define FABLE_RETAIL_CTHINGSEARCHTOOLS_ORIGINAL_ROUGHAREA
#define FABLE_RETAIL_CTHINGSEARCHTOOLS_ROUGHAREA_EGOD
#define FABLE_RETAIL_CTHINGMAP_PEEKCELL_INLINE
#define FABLE_RETAIL_CTCMAPWHO
#define FABLE_RETAIL_CTCMAPWHO_SEARCHID
#define FABLE_RETAIL_CTHINGPHYSICAL
#define FABLE_RETAIL_CTHINGGAMEOBJECT
#define FABLE_RETAIL_CINTELLIGENTPOINTER
#define FABLE_RETAIL_CTHING
#define FABLE_RETAIL_CTHING_STATTYPEFLAGS
#define FABLE_RETAIL_CTCPHYSICSBASE
#define FABLE_RETAIL_CTCBASE
#define FABLE_RETAIL_C3DVECTOR
#define FABLE_RETAIL_C2DBOXI_RADIUS_INITIALIZERS
#define FABLE_RETAIL_C2DBOXI
#define FABLE_RETAIL_C2DBOXI_COPY
#define FABLE_RETAIL_C2DBOXI_GETWIDTH
#define FABLE_RETAIL_C2DBOXI_GETHEIGHT
#include "engine/C2DExtentsI.h"
#include "engine/C3DVector.h"
#include "engine/CThing.h"
#include "engine/CTCPhysicsBase.h"
class CTCMapwho;
#include "engine/CTCMapwho.h"
#include "engine/NThingMap_CThingMap.h"
#include "engine/CThingSearchTools.h"
#include "engine/C2DBoxI.h"
namespace {
// Fidelity: scaffolding, not retail -- takes the address of the header's protected inline GetMapwhoArea (from a never-instantiated
// derived class) so VC7.1 emits its COMDAT here.
class CEmitGetMapwhoArea : public CThingSearchTools {
public:
    static C2DBoxI (CThingSearchTools::*Emit())(const NThingMap::CThingMap&, long, const C2DBoxI&) const;
};
C2DBoxI (CThingSearchTools::*CEmitGetMapwhoArea::Emit())(const NThingMap::CThingMap&, long, const C2DBoxI&) const
{
    return &CEmitGetMapwhoArea::GetMapwhoArea;
}
}
