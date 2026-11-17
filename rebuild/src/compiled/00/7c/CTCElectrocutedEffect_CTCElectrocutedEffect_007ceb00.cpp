// CTCElectrocutedEffect::CTCElectrocutedEffect(CThing& thing) -- constructor: CTCBase, a null def, zero timers, a null effect pointer and an empty tint list; load SPECIAL_ABILITIES_ELECTROCUTED_EFFECT_DEF, copy the things the parent carries into TintList, then Reset
// Leaf: ??0CTCElectrocutedEffect@@QAE@AAVCThing@@@Z
// Retail: Fable.exe (TLC, VC7.1) function @ 0x007CEB00, 297 bytes, `ret 4`; vtable 0x0126F044.
// Callees: CTCBase::CTCBase(CThing&) 0x00686800; CIntelligentPointer<CThing>(0) (base ctor 0x00A01B10, vtable 0x01238C6C, SetPItem 0x00A01B90);
//   CCharString(const char*, long) 0x0099EBF0 / ~ 0x0099EAE0; CGameDefinitionManager::Get 0x0044C6B0 (twice); GetGlobalIndexFromClassIndex
//   0x009AD390; GetDef<CSpecialAbilitiesElectrocutedEffectDef> 0x007CEC90; the inline GetTC<CTCCarrying> (bitset +0x28 bit 6, LowerBound
//   0x0040F020); CTCCarrying::GetAllCarriedThings 0x006A2220; vector<CIntelligentPointer<CThing> >::reserve 0x0077E580; the random-access
//   __copy<CThing**, back_insert_iterator<..>, int> 0x007CE1F0 (__fastcall); free 0x00BFEA14; Reset 0x007CE7A0.
// NF62 (2026-11-17): ported onto engine/CTCElectrocutedEffect.h (ELECTROCUTED_ORIGINAL_PASS23) / CThing.h (GETTC_EXPANDED) / CTCCarrying.h
//   (ALLCARRIED) / CGameDefinitionManager.h / CIntelligentPointer.h / stlport vector + back_inserter + copy (overlay opt-in
//   FABLE_RETAIL_STLP_COPY_GENERIC_INLINE: STLport's inline copy() dispatching to the out-of-line __copy); every local model deleted. The
//   reserve and __copy bindings are queue entries q10457 / q10458.
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
#define FABLE_RETAIL_ELECTROCUTED_ORIGINAL_PASS23
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
#include "engine/CTCElectrocutedEffect.h"

CTCElectrocutedEffect::CTCElectrocutedEffect(CThing& thing)
    : CTCBase(thing), CurTime(0), FadeInTime(0), FadeOutTimeStart(0), FadeOutTime(0), PEffect(0)
{
    long def_index = CGameDefinitionManager::Get().GetGlobalIndexFromClassIndex("SPECIAL_ABILITIES_ELECTROCUTED_EFFECT_DEF", 1);
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
