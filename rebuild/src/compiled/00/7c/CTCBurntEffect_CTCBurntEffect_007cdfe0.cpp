// CTCBurntEffect::CTCBurntEffect(CThing& parent) -- constructor: CTCBase, null def, zero timers, two empty effect vectors; load SPECIAL_ABILITIES_BURNT_EFFECT_DEF, copy the things the parent carries into TintList, then Reset
// Leaf: ??0CTCBurntEffect@@QAE@AAVCThing@@@Z
// Retail: Fable.exe (TLC, VC7.1) @ 0x007CDFE0, 284 bytes, `ret 4`; vtable 0x0126EFBC; the factory 0x004D58C6 allocates 0x38 bytes.
// Callees: CTCBase::CTCBase(CThing&) 0x00686800; CCharString(const char*, long) 0x0099EBF0 / ~ 0x0099EAE0; CGameDefinitionManager::Get 0x0044C6B0
//   (twice); GetGlobalIndexFromClassIndex 0x009AD390; GetDef<CSpecialAbilitiesBurntEffectDef> 0x007CE100; the inline GetTC<CTCCarrying>
//   (LowerBound 0x0040F020); CTCCarrying::GetAllCarriedThings 0x006A2220; vector<CIntelligentPointer<CThing> >::reserve 0x0077E580; the
//   random-access __copy<CThing**, back_insert_iterator<..>, int> 0x007CE1F0 (__fastcall); free 0x00BFEA14; CTCBurntEffect::Reset 0x007CDD10.
// NF62 (2026-11-17): ported onto engine/CTCBase.h / CThing.h (GETTC_EXPANDED) / CTCCarrying.h (ALLCARRIED) / CGameDefinitionManager.h /
//   CIntelligentPointer.h / CDefPointer.h / stlport vector + back_inserter + inline copy (overlay FABLE_RETAIL_STLP_COPY_GENERIC_INLINE), as
//   its twin 0x007CEB00. CTCBurntEffect has no shared header: the local class carries the Ego_r.pdb members at their retail offsets
//   (PDef +0x0C, the four timers, Effects +0x20 and TintList +0x2C as 12-byte STLport vectors) and the members this row uses.
#define FABLE_RETAIL_CCHARSTRING
#define FABLE_RETAIL_CCLASS
#define FABLE_RETAIL_DEF_CLASS_BASE
#define FABLE_RETAIL_CTCBASE
#define FABLE_RETAIL_CTHING
#define FABLE_RETAIL_CTHING_GETTC_EXPANDED
#define FABLE_RETAIL_CINTELLIGENTPOINTER
#define FABLE_RETAIL_CRESOURCELIST
#define FABLE_RETAIL_CDEFINITIONMANAGER
#define FABLE_RETAIL_CGAMEDEFINITIONMANAGER
#define FABLE_RETAIL_CDEFSTRINGTABLE
#define FABLE_RETAIL_CTCCARRYING_ALLCARRIED
#define FABLE_RETAIL_STLP_VECTOR
#define FABLE_RETAIL_STLP_BACK_INSERTER
#define FABLE_RETAIL_STLP_COPY_GENERIC_INLINE
#define FABLE_RETAIL_STLP_ORIGINAL_COPY_DISPATCH_PASS22
#define FABLE_RETAIL_STLP_COPY_NOINLINE
#include "fable_string.h"
#include "stlport/fable_containers.h"
#include "engine/CThing.h"
#include "engine/CTCBase.h"
#include "engine/CIntelligentPointer.h"
#include "engine/CDefPointer.h"
#include "engine/CTCCarrying.h"
#include "engine/CGameDefinitionManager.h"

class CSpecialAbilitiesBurntEffectDef;
class CTCBurntEffect : public CTCBase {
public:
    CTCBurntEffect(CThing& parent);
    virtual ~CTCBurntEffect();
    void Reset();                                                        // 0x007CDD10
private:
    CDefPointer<const CSpecialAbilitiesBurntEffectDef> PDef;             // +0x0C
    long CurTime;                                                        // +0x10
    long FadeInTime;                                                     // +0x14
    long FadeOutTimeStart;                                               // +0x18
    long FadeOutTime;                                                    // +0x1C
    std::vector<CIntelligentPointer<CThing> > Effects;                   // +0x20
    std::vector<CIntelligentPointer<CThing> > TintList;                  // +0x2C
};

CTCBurntEffect::CTCBurntEffect(CThing& parent)
    : CTCBase(parent), CurTime(0), FadeInTime(0), FadeOutTimeStart(0), FadeOutTime(0)
{
    long def_index = CGameDefinitionManager::Get().GetGlobalIndexFromClassIndex("SPECIAL_ABILITIES_BURNT_EFFECT_DEF", 1);
    CGameDefinitionManager::Get().GetDef(def_index, &PDef);
    CTCCarrying* carrying;
    if (GetParentThing().GetTC(&carrying))
    {
        std::vector<CThing*> things;
        carrying->GetAllCarriedThings(things);
        TintList.reserve(things.size());
        std::copy(things.begin(), things.end(), std::back_inserter(TintList));
    }
    Reset();
}
