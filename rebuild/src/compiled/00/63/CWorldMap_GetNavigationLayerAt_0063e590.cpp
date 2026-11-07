// CWorldMap::GetNavigationLayerAt(const C3DVector& pos, const CThing* ignore) const -- the navigation layer under a
// Grade: RELOCATION_MATCH (340 bytes) -- each search branch reads `CThingSearchTools* tools = &World.GetThingSearchTools();` into a local before the call, and the Bind3 branch names its `CThingFilter_IsNot isNot(ignore)` object first (the direct `World->ThingSearchTools->ForEachInRoughArea(...)` calls stored the Bind2 F1 / Bind3 F3 pointer one slot early: DIFFER(340v340), 4 masked lines, hand757)
// Leaf: GetNavigationLayerAt@CWorldMap
// world position: with per-map navigation enabled, no thing to ignore and the editor inactive, the loaded map's own
// GetNavigationLayerAt answers (0 when the map is not loaded); otherwise every physical thing with navigation
// information whose bounding box overlaps the position (radius 0.1), skipping `ignore`, is asked through a
// CThingFindNavigationLayerAt finder in a 2x2 box round the position
// Retail: Fable.exe (TLC, VC7.1) @ 0x0063E590, 340 bytes, `ret 8`.
//   PDB ?GetNavigationLayerAt@CWorldMap@@QBEJABVC3DVector@@PBVCThing@@@Z (Ego_r.pdb; the shared engine/CWorldMap.h declares it).
// Callees: CMainGameComponent::PMainGameComponent 0x013B86A0 (?PMainGameComponent@CMainGameComponent@@1PAV1@A, through the inline
//   Get()) -> IsEditorActive() slot 9 (+0x24, ?IsEditorActive@CMainGameComponent@@UBE_NXZ, shared header opt-in EDITOR);
//   GetMapNumberAt(const C3DVector&) slot 16 (+0x40), IsMapLoaded slot 10 (+0x28), PeekMap slot 19 (+0x4c);
//   CMap::GetNavigationLayerAt(const C3DVector&) const 0x0081E920 (?GetNavigationLayerAt@CMap@@QBEJABVC3DVector@@@Z; the catalog
//   already carries CMap::GetNavigationLayerAt since chunk 256, the older _Cons_val label is gone); __ftol2 0x00BFEA70 (x3);
//   CThingSearchTools::ForEachInRoughArea<CThingPhysical, CFilter_Bind2<CThingFilter_HasNavigationInformation,
//   CThingFilter_OverlappingBoundingBox>, CThingFindNavigationLayerAt> const 0x0063F4B0 and the CFilter_Bind3<CThingFilter_IsNot,
//   ...> twin 0x0063F7C0 (both Ego_r.pdb spellings, ghidra_out/naming_stragglers/ego_r.exe.signatures.tsv).
// Layout: shared headers -- CWorldMap::World (CWorld&, +0x08), CWorld::PThingSearchTools_ptr +0x20 through GetThingSearchTools();
//   CThingFindNavigationLayerAt {Pos, Layer} 16 bytes, GetLayer() const returns float (PDB QBEMXZ: fild + ftol);
//   CThingFilter_OverlappingBoundingBox {Pos, Radius}; CFilter_Bind2/3 hold pointers F1/F2/F3 to their filters (Ego_r.pdb
//   struct_layouts: F1, F2). All of these are PDB classes (V): no shared header yet for the filters / finder (header debt).
//   The per-map flag byte 0x0137571C is unnamed in the labels (descriptive g_bMapNavigationLayers).
// Shape: the thing-search-tools pointer is a named local in each branch (`tools`) and the IsNot filter a named local in the
//   Bind3 branch; the other filters are temporaries in the Bind2 / Bind3 constructor calls (named HasNavigationInformation /
//   OverlappingBoundingBox objects or a named Bind2 / Bind3 object move the pointer stores again).
#define FABLE_RETAIL_C3DVECTOR
#define FABLE_RETAIL_C2DBOXI
#define FABLE_RETAIL_C2DBOXI_CTORS
#define FABLE_RETAIL_CWORLDMAP
#define FABLE_RETAIL_CWORLD
#define FABLE_RETAIL_CTHINGSEARCHTOOLS
#define FABLE_RETAIL_CTHINGSEARCHTOOLS_FOREACHINROUGHAREA
#define FABLE_RETAIL_CMAP_NAVIGATIONLAYER
#define FABLE_RETAIL_CMAINGAMECOMPONENT
#define FABLE_RETAIL_CMAINGAMECOMPONENT_EDITOR
#include "engine/C3DVector.h"
#include "engine/C2DBoxI.h"
#include "engine/CWorldMap.h"
#include "engine/CWorld.h"
#include "engine/CThingSearchTools.h"
#include "engine/CMap.h"
#include "engine/CMainGameComponent.h"
#include "engine/CFilter_Bind2.h"
class CThingPhysical;
class CThingFilter_HasNavigationInformation { public: CThingFilter_HasNavigationInformation() { } };
class CThingFilter_IsNot {
public:
    const CThing* Thing;
    CThingFilter_IsNot(const CThing* thing) : Thing(thing) { }
};
class CThingFilter_OverlappingBoundingBox {
public:
    C3DVector Pos;
    float     Radius;
    CThingFilter_OverlappingBoundingBox(const C3DVector& pos, float radius) : Pos(pos), Radius(radius) { }
};
template <class F1T, class F2T, class F3T> class CFilter_Bind3 {
public:
    const F1T* F1; const F2T* F2; const F3T* F3;
    CFilter_Bind3(const F1T& f1, const F2T& f2, const F3T& f3) : F1(&f1), F2(&f2), F3(&f3) { }
};
class CThingFindNavigationLayerAt {
public:
    C3DVector Pos;
    long      Layer;
    CThingFindNavigationLayerAt(const C3DVector& pos) : Pos(pos), Layer(0) { }
    float GetLayer() const { return (float)Layer; }
};
typedef CFilter_Bind2<CThingFilter_HasNavigationInformation, CThingFilter_OverlappingBoundingBox> CNavFilter2;
typedef CFilter_Bind3<CThingFilter_IsNot, CThingFilter_HasNavigationInformation, CThingFilter_OverlappingBoundingBox> CNavFilter3;
extern bool g_bMapNavigationLayers;                // 0x0137571C (unnamed flag byte)
long CWorldMap::GetNavigationLayerAt(const C3DVector& pos, const CThing* ignore) const
{
    if (g_bMapNavigationLayers && ignore == 0 && !CMainGameComponent::Get()->IsEditorActive()) {
        long map = GetMapNumberAt(pos);
        if (map != 0 && IsMapLoaded(map))
        {
            const CMap& theMap = PeekMap(map);
            return theMap.GetNavigationLayerAt(pos);
        }
        return 0;
    }
    long x = (long)pos.X;
    long y = (long)pos.Y;
    C2DBoxI box(x - 1, y - 1, x + 1, y + 1);
    CThingFindNavigationLayerAt finder(pos);
    if (ignore == 0) {
        CThingSearchTools* tools = &World.GetThingSearchTools();
        tools->ForEachInRoughArea<CThingPhysical>(box, CNavFilter2(CThingFilter_HasNavigationInformation(), CThingFilter_OverlappingBoundingBox(pos, 0.1f)), finder);
    }
    else {
        CThingFilter_IsNot isNot(ignore);
        CThingSearchTools* tools = &World.GetThingSearchTools();
        tools->ForEachInRoughArea<CThingPhysical>(box, CNavFilter3(isNot, CThingFilter_HasNavigationInformation(), CThingFilter_OverlappingBoundingBox(pos, 0.1f)), finder);
    }
    return finder.GetLayer();
}