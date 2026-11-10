// CGameScriptInterface::ScreenFilterFadeTo(float time, float alpha, float saturation, float contrast, float brightness, const CRGBFloatColour& tint, const std::vector<CScreenFilterSThingByPass>& bypass) const -- convert the script-thing bypass list into the engine's thing/flag records and start a screen-filter fade at script priority on the environment's spell-effect interpolator; returns its handle
// Grade: DIFFER(347v363) -- on shared headers (overlay ARGPASSER/FADETO opt-ins + CScreenFilterSThingByPass.h, FABLE_HDR_OVERLAY=$A/hdr): the constant-0 piece takes edi (retail ebx) and the bypass-vector destroy-loop end takes ebx (retail edi; c2prio: piece #6 copy-prefers the offset temp #22, +1 edi cost), and retail's loop-head `xor edi,edi ; jmp ; lea esp,[esp]` alignment block is missing (16 bytes)
// Leaf: ?ScreenFilterFadeTo@CGameScriptInterface@@UBEPAXMMMMMABVCRGBFloatColour@@ABV?$vector@UCScreenFilterSThingByPass@@V?$allocator@UCScreenFilterSThingByPass@@@std@@@std@@@Z
// Retail: Fable.exe (TLC, VC7.1) function @ 0x008A9880, 361 bytes (+ int3 padding), thiscall `ret 1Ch`.
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

void* CGameScriptInterface::ScreenFilterFadeTo(float time, float alpha, float saturation, float contrast, float brightness,
    const CRGBFloatColour& tint, const std::vector<CScreenFilterSThingByPass, std::allocator<CScreenFilterSThingByPass> >& bypass) const
{
    NScreenEffect::CScreenEffectPriorityStack::ThingGraphicArgPassers things;
    for (int i = 0; i < (int)bypass.size(); ++i) {
        NScreenEffect::CScreenEffectPriorityStack::CThingGraphicArgPasser thing(bypass[i].thing.GetPThing(), bypass[i].bypassSet);
        things.push_back(thing);
    }
    return World->GetEnvironment().GetSpellEffectInterpolator().ScreenFilterFadeTo(time, NScreenEffect::SCREEN_EFFECT_PRIORITY_SCRIPT,
        alpha, saturation, contrast, brightness, tint, things);
}
