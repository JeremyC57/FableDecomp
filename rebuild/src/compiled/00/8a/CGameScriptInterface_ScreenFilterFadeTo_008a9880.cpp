// 008a9880 CGameScriptInterface::ScreenFilterFadeTo
// Flags: /Gr
// Leaf: ?ScreenFilterFadeTo@CGameScriptInterface@@UBEPAXMMMMMABVCRGBFloatColour@@ABV?$vector@UCScreenFilterSThingByPass@@V?$allocator@UCScreenFilterSThingByPass@@@std@@@std@@@Z
#define FABLE_RETAIL_CINTELLIGENTPOINTER_ORIGINAL_SETPTR_PASS22
#define FABLE_RETAIL_CINTELLIGENTPOINTER_COPY_ORIGINAL_CHAIN

#define FABLE_RETAIL_CCHARSTRING
#define FABLE_RETAIL_CCLASS
#define FABLE_RETAIL_DEF_CLASS_BASE
#define FABLE_RETAIL_CCLASSFACTORY
#define FABLE_RETAIL_CTCBASE
#define FABLE_RETAIL_CTHING
#define FABLE_RETAIL_CWORLD
#define FABLE_RETAIL_CWORLD_ENVIRONMENT
#define FABLE_RETAIL_CENVIRONMENT_SPELLEFFECTINTERPOLATOR
#define FABLE_RETAIL_CENVIRONMENTSPELLEFFECTINTERPOLATOR_FADETO
#define FABLE_RETAIL_CSCREENEFFECTPRIORITYSTACK_ARGPASSER
#define FABLE_RETAIL_CINTELLIGENTPOINTER
#define FABLE_RETAIL_CINTELLIGENTPOINTER_COPY
#define FABLE_RETAIL_NSCRIPT
#define FABLE_RETAIL_SCOPED_PTR
#include "fable_string.h"
#include "stlport/fable_containers.h"
#include "engine/CCountedPointer.h"
#include "engine/CThing.h"
#include "engine/CIntelligentPointer.h"
#include "engine/CWorld.h"
#include "engine/CEnvironment.h"
#include "engine/NScreenEffect_CScreenEffectPriorityStack.h"
#include "engine/CEnvironmentSpellEffectInterpolator.h"
#include "engine/CGameScriptInterface.h"
#include "engine/CScriptThing.h"
#include "engine/CScreenFilterSThingByPass.h"

void* CGameScriptInterface::ScreenFilterFadeTo(float duration, float alpha, float sat, float contrast, float brightness,
    const CRGBFloatColour& tint, const std::vector<CScreenFilterSThingByPass, std::allocator<CScreenFilterSThingByPass> >& s) const
{
    NScreenEffect::CScreenEffectPriorityStack::ThingGraphicArgPassers ss;
    if (false) {} else for (long i = 0; i < static_cast<long>(s.size()); ++i) {
        NScreenEffect::CScreenEffectPriorityStack::CThingGraphicArgPasser arg(s[i].thing.GetPThing(), s[i].bypassSet);
        ss.push_back(arg);
    }
    return World->GetEnvironment().GetSpellEffectInterpolator().ScreenFilterFadeTo(duration, NScreenEffect::SCREEN_EFFECT_PRIORITY_SCRIPT,
        alpha, sat, contrast, brightness, tint, ss);
}