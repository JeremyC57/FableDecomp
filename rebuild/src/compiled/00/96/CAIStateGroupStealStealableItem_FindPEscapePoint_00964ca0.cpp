// CAIStateGroup_StealStealableItem::FindPEscapePoint(bool nearest) -- pick an escape point thing: the nearest one to
// Leaf: ?FindPEscapePoint@CAIStateGroup_StealStealableItem@@ABEPBVCThing@@_N@Z
// Plan phase 4 (2026-11-05), C3DVector wave (rows with a second value class, from their landed source): the local C3DVector stand-in replaced by the shared engine/C3DVector.h (PDB C3DVector TPI 0x2783: public X/Y/Z at 0/4/8, size 12). Member accesses x/y/z respelled to the PDB X/Y/Z.
// Plan phase 3 (2026-11-04), CVectorMap wave: the local CVectorMap / CKeyPairCompareLess / CArray / STLport stand-ins replaced by the shared engine/CVectorMap.h (Ego_r: CVectorMap<K,V,CompareType> derives CArray<pair<K,V> >, Compare +0x0C, Dirty +0x0D); map ends read through the inherited end(). Also: re-padded by-value maps +8.
// the thief, or a random one of all of them.
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00964CA0, length 245 bytes (+ int3 padding), `ret 4`.
// Callees: CThing::GetPosition @ 0x004C73D0, CAIStateGroupBase::PeekThingSearchTools @ 0x008FD000 (x2),
// CThingSearchTools::GetPBestThing<filter, CThingCompare_Nearest, CThing> @ 0x009642D0 and GetAllLeafThings<CThing,
// vector, filter> @ 0x00964EF0 (the PDB instances <CThingFilter_MarkerHasProperty, CThingCompare_Nearest, const CThingMarker> / <CThingMarker, vector<CThingMarker*>, CThingFilter_MarkerHasProperty>),
// CAIStateGroupBase::GetPWorldSeed @ 0x008FCFD0, GFROR13 @ 0x00497890, free @ 0x00BFEA14 (the vector block).
// Shape: the one-word filter object is homed in the dead `nearest` argument slot (`lea eax,[esp+24h]`); the result
// is zero-initialised up front (edi, also the best-thing out slot); the random index is the landed
// `range ? seed % range : 0` step over the vector size.
// NF141 (2026-11-20, q4994 / q5140): ported onto the shared headers. Own leaf QAEPAVCThing -> the PDB
//   ?FindPEscapePoint@CAIStateGroup_StealStealableItem@@ABEPBVCThing@@_N@Z (private const, returns const CThing*; Ego_r.pdb 0x00989660 +
//   FableWin.pdb 0x02C3AED0), declared by the overlay opt-in FABLE_RETAIL_CAISTATEGROUP_STEALSTEALABLEITEM_FINDPESCAPEPOINT on the retail
//   engine/CAIStateGroup_StealStealableItem.h class; its one landed caller 0x00964DA0 moves with it. PeekThingSearchTools 0x008FD000 now
//   binds the PDB ?PeekThingSearchTools@CAIStateGroupBase@@IBEABVCThingSearchTools@@XZ (_PEEK). The searches are the real same-TU noinline
//   bodies of engine/CThingSearchTools.h under their PDB template spellings: 0x009642D0 GetPBestThing<CThingFilter_MarkerHasProperty,
//   CThingCompare_Nearest, const CThingMarker> (BESTTHING_BODY; Ego_r.pdb 0x00988BA0 + FableWin.pdb 0x02C3B1E0, the self row's leaf) and
//   0x00964EF0 GetAllLeafThings<CThingMarker, vector<CThingMarker*>, CThingFilter_MarkerHasProperty> (LEAFBODY; Ego_r.pdb 0x009898C0 +
//   FableWin.pdb 0x02C3B370); were QAE stand-ins over made-up helpers. The stand-in filter CThingFilter_EscapePoint is
//   CThingFilter_MarkerHasProperty (both callees' PDB names; one word, FableWin ??0CThingFilter_MarkerHasProperty@@QAE@W4EMarkerProperty@@@Z;
//   no shared header: row-local like the landed 0x009642D0 / 0x00964EF0 / 0x006A75B0). The result vector is STLport std::vector
//   (stlport/fable_containers.h). Local CBaseIntelligentPointer / CIntelligentPointerThing / CAIBrain / CAIStateGroupBase / CThingPos /
//   CThingVector / CThingCompare_Nearest / CThingSearchTools models dropped.
// Inferred: the EMarkerProperty enumerator for value 1 is not in any name source (the retail Defs headers do not list EMarkerProperty);
//   the row passes `(EMarkerProperty)1`.
#define FABLE_RETAIL_CCHARSTRING
#define FABLE_RETAIL_CCLASS
#define FABLE_RETAIL_DEF_CLASS_BASE
#define FABLE_RETAIL_CTCBASE
#define FABLE_RETAIL_CTHING
#define FABLE_RETAIL_CINTELLIGENTPOINTER
#define FABLE_RETAIL_CTHINGPHYSICAL
#define FABLE_RETAIL_CTHINGGAMEOBJECT
#define FABLE_RETAIL_CTHINGCREATUREBASE
#define FABLE_RETAIL_CTHINGAICREATURE
#define FABLE_RETAIL_CTHINGMARKER_STATTYPEFLAGS
#define FABLE_RETAIL_C3DVECTOR
#define FABLE_RETAIL_CWORLD
#define FABLE_RETAIL_CAISTATEGROUPBASE
#define FABLE_RETAIL_CAISTATEGROUPBASE_MEMBERS
#define FABLE_RETAIL_CAISTATEGROUPBASE_GETCREATURE
#define FABLE_RETAIL_CAISTATEGROUPBASE_PEEK
#define FABLE_RETAIL_CAIBRAIN_GETCREATURE
#define FABLE_RETAIL_CAISTATEGROUP_STEALSTEALABLEITEM
#define FABLE_RETAIL_CAISTATEGROUP_STEALSTEALABLEITEM_FINDPESCAPEPOINT
#define FABLE_RETAIL_CTHINGSEARCHTOOLS
#define FABLE_RETAIL_CTHINGSEARCHTOOLS_ALLTHINGSBODY
#define FABLE_RETAIL_CTHINGSEARCHTOOLS_BESTTHING_BODY
#define FABLE_RETAIL_CTHINGSEARCHTOOLS_LEAFBODY
#include "fable_compiler.h"
#include "fable_string.h"
#include "stlport/fable_containers.h"
#include "engine/C3DVector.h"
#include "engine/CThing.h"
#include "engine/CTCBase.h"
#include "engine/CIntelligentPointer.h"
#include "engine/CThingCreatureBase.h"
#include "engine/CThingAICreature.h"
#include "engine/CThingMarker.h"
#include "engine/CWorld.h"
#include "engine/CThingSearchTools.h"
#include "engine/CThingCompare_Nearest.h"
#include "engine/CAIStateGroupBase.h"
#include "engine/CAIBrain.h"
#include "engine/CAIStateGroup_StealStealableItem.h"

long __fastcall GFROR13(unsigned long);   // 0x00497890

enum EMarkerProperty;                                   // MS forward enum (FableWin ??0CThingFilter_MarkerHasProperty@@QAE@W4EMarkerProperty@@@Z)
class CThingFilter_MarkerHasProperty { public:         // one word: the marker property (no shared header)
    EMarkerProperty Property;
    CThingFilter_MarkerHasProperty(EMarkerProperty property) : Property(property) {}
    bool operator()(const CThingMarker& thing) const;   // 0x006A75B0
};

const CThing* CAIStateGroup_StealStealableItem::FindPEscapePoint(bool nearest) const
{
    const CThingMarker* result = 0;
    if (nearest) {
        PeekThingSearchTools().GetPBestThing(CThingFilter_MarkerHasProperty((EMarkerProperty)1),
            CThingCompare_Nearest(GetCreature().GetPos()), &result);
        return result;
    } else {
    std::vector<CThingMarker*> points;
    PeekThingSearchTools().GetAllLeafThings<CThingMarker>(CThingFilter_MarkerHasProperty((EMarkerProperty)1), points);
    if (!points.empty()) {
        unsigned long count = points.size();
        unsigned long* seed = GetPWorldSeed();
        unsigned long next = *seed * 0x24A1U + 0x24DFU;
        *seed = next;
        unsigned long rotated = GFROR13(next);
        *seed = rotated;
        unsigned long index = count ? rotated % count : 0;
        result = points[index];
    }
    return result;
    }
}
