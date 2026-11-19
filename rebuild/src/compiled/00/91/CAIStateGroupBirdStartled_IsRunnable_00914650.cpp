// CAIStateGroup_BirdStartled::IsRunnable() -- a bird on the A* navigator is startled when a creature that scares birds is in the rough area
// Leaf: ?IsRunnable@CAIStateGroup_BirdStartled@@UAE_NXZ
// Flags: none
// Retail: Fable.exe (TLC, VC7.1) @ 0x00914650, 340 bytes, frame pointer, plain `ret` (vtable 0x012896C0 slot 11). Callees:
//   CVectorMap<..>::LowerBound 0x0040F020 (GetTC<CTCCreatureNavigation>, TC 7), CTCCreatureNavigation::GetNavigatorType 0x006CB6A0,
//   CThingPhysical::PeekPos 0x006A4D00 (four times), __ftol2 0x00BFEA70 (C3DVector::ToC2DCoordI), CAIStateGroupBase::
//   PeekThingSearchTools 0x008FD000, CThingSearchTools::GetPBestThingInRoughArea<CThingFilter_ScaresBirds, CThingCompare_Nearest,
//   CThingCreatureBase> 0x009147E0 (the next function in the TU). Global ?NAVIGATOR_TYPE_A_STAR@@3KB 0x0129CBA4 (FableWin public 0x044DCE00).
// Retail-asm: 0x009146BB `fld dword ptr [ebp-10h]` + 0x009146BE `fadd dword ptr [ebp-0Ch]` + 0x009146C1 `fistp dword ptr [ebp-4]` ... 0x009146D5
//   `dec dword ptr [ebp-4]`: the engine inline-assembly ceiling, through the shared engine/GFFloatToLongCeil.h helper. FableWin calls
//   ?GFFloatToLongCeil@@YIJM@Z. No asm in this source.
// Shape: FableWin.pdb 0x02B8D570 (Ego_d names pnav, area, pcreature). GetTC<CTCCreatureNavigation>(&pnav), whose debug-only failure
//   ASSERT is compiled out: retail reads the unset pnav slot on that path, as here. Then GetNavigatorType() == NAVIGATOR_TYPE_A_STAR;
//   area = C2DBoxI(PeekPos().ToC2DCoordI(), GFFloatToLongCeil(DIST_CREATURES_SCARE_BIRDS * 0.5)) (a double product, folded to 2.5f);
//   GetPBestThingInRoughArea(area, CThingFilter_ScaresBirds(GetCreature()), CThingCompare_Nearest(PeekPos()), &pcreature) &&
//   GFWorld2DDistanceSmallerThan(PeekPos(), PeekPos(), DIST_CREATURES_SCARE_BIRDS, false). Both FableWin and retail measure the bird
//   against itself (the four PeekPos calls are on the creature), so that test is always true; the original's bug is kept.
//   DIST_CREATURES_SCARE_BIRDS is FableWin's file-static const float (S_LDATA32, 5.0f at 0x043E1870), folded in retail (25.0f = its square).
//   The search body is visible (FABLE_RETAIL_CTHINGSEARCHTOOLS_BESTROUGH_BODY; its instance follows in retail's TU). VC7.1 then sees
//   that the search always writes the out pointer, and drops the `pcreature = 0` store as retail does (without it, DIFFER 347v340).
//   That needs the filter's FableWin inline test (??R..QBE_NABVCThingCreatureBase@@@Z 0x02B8E030: GetType() != CThingDeadCreature::
//   StatGetType() && GetDefGlobalIndex() differs), defined here as the occlusion rows define their inlines.
// Headers: NEW engine/CAIStateGroup_BirdStartled.h and engine/CThingFilter_ScaresBirds.h; CThingDeadCreature.h +
//   FABLE_RETAIL_CTHINGDEADCREATURE_STATGETTYPE (13). The earlier park (DIFFER 341v340) used a local model.
#define FABLE_RETAIL_CAISTATEGROUPBASE
#define FABLE_RETAIL_CAISTATEGROUPBASE_MEMBERS
#define FABLE_RETAIL_CAISTATEGROUPBASE_GETCREATURE
#define FABLE_RETAIL_CAISTATEGROUPBASE_PEEK
#define FABLE_RETAIL_CAIBRAIN_GETCREATURE
#define FABLE_RETAIL_CTCBASE
#define FABLE_RETAIL_CTHING
#define FABLE_RETAIL_CTHINGGAMEOBJECT
#define FABLE_RETAIL_CTHINGPHYSICAL
#define FABLE_RETAIL_CTHINGCREATUREBASE
#define FABLE_RETAIL_CTHINGAICREATURE
#define FABLE_RETAIL_CTHING_GETTC_EXPANDED
#define FABLE_RETAIL_C3DVECTOR
#define FABLE_RETAIL_DEF_CLASS_BASE
#define FABLE_RETAIL_CCHARSTRING
#define FABLE_RETAIL_CTHINGSEARCHTOOLS
#define FABLE_RETAIL_CTHINGSEARCHTOOLS_BESTROUGH
#define FABLE_RETAIL_CTHINGSEARCHTOOLS_BESTROUGH_BODY
#define FABLE_RETAIL_CTHING_STATTYPEFLAGS
#define FABLE_RETAIL_CTHING_TYPEFLAGS
#define FABLE_RETAIL_CTCMAPWHO
#define FABLE_RETAIL_CTCMAPWHO_SEARCHID
#define FABLE_RETAIL_CCLASS
#define FABLE_RETAIL_CTHINGDEADCREATURE_STATGETTYPE
#define FABLE_RETAIL_C2DBOXI_CTORS
#define FABLE_RETAIL_C2DBOXI
#define FABLE_RETAIL_CTCCREATURENAVIGATION
#define FABLE_RETAIL_CTCCREATURENAVIGATION_NAVIGABLE
#define FABLE_RETAIL_CTCCREATURENAVIGATION_NAVIGATORTYPE
#include "engine/CThingAICreature.h"
#include "engine/CAIStateGroupBase.h"
#include "engine/CAIBrain.h"
#include "engine/CTCMapwho.h"
#include "engine/NThingMap_CThingMap.h"
#include "engine/CThingSearchTools.h"
#include "engine/CThingDeadCreature.h"
#include "engine/C2DBoxI.h"
#include "engine/CThingCompare_Nearest.h"
#include "engine/CTCCreatureNavigation.h"
#include "engine/GFFloatToLongCeil.h"
#include "engine/GFWorldDistance.h"
#include "engine/CThingFilter_ScaresBirds.h"
#include "engine/CAIStateGroup_BirdStartled.h"

extern const unsigned long NAVIGATOR_TYPE_A_STAR;                          // 0x0129CBA4 ?NAVIGATOR_TYPE_A_STAR@@3KB
inline bool CThingFilter_ScaresBirds::operator()(const CThingCreatureBase& thing) const
{
    return thing.GetType() != CThingDeadCreature::StatGetType() && thing.GetDefGlobalIndex() != Bird->GetDefGlobalIndex();
}
static const float DIST_CREATURES_SCARE_BIRDS = 5.0f;                      // FableWin S_LDATA32 (file static), folded in retail

bool CAIStateGroup_BirdStartled::IsRunnable()
{
    CTCCreatureNavigation* pnav;
    GetCreature().GetTC(&pnav);
    if (pnav->GetNavigatorType() == NAVIGATOR_TYPE_A_STAR)
    {
        CThingCreatureBase* pcreature = 0;
        C2DBoxI area(GetCreature().PeekPos().ToC2DCoordI(), GFFloatToLongCeil(DIST_CREATURES_SCARE_BIRDS * 0.5));
        if (PeekThingSearchTools().GetPBestThingInRoughArea(area, CThingFilter_ScaresBirds(GetCreature()),
                CThingCompare_Nearest(GetCreature().PeekPos()), &pcreature) &&
            GFWorld2DDistanceSmallerThan(GetCreature().PeekPos(), GetCreature().PeekPos(), DIST_CREATURES_SCARE_BIRDS, false))
            return true;
    }
    return false;
}