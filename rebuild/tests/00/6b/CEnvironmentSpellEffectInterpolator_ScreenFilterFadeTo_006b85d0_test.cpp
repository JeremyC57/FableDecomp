#define FABLE_RETAIL_CENVIRONMENTSPELLEFFECTINTERPOLATOR_FADETO
#define FABLE_RETAIL_BASE_OBJECT
#define FABLE_RETAIL_CCLASS
#define FABLE_RETAIL_DEF_CLASS_BASE
#define FABLE_RETAIL_CTCBASE
#define FABLE_RETAIL_CTHING
#define FABLE_RETAIL_CCHARSTRING
#define FABLE_RETAIL_C3DVECTOR
#define FABLE_RETAIL_CINTELLIGENTPOINTER
#define FABLE_RETAIL_CINTELLIGENTPOINTER_ASSIGN
#define FABLE_RETAIL_CTCGRAPHICAPPEARANCE_MOVEMENT
#define FABLE_RETAIL_CTCGRAPHICAPPEARANCE_BYPASSCOLORFILTER
#define FABLE_RETAIL_CSCREENEFFECTPRIORITYSTACK_FULL
#define FABLE_RETAIL_CSCREENEFFECTPRIORITYSTACK_SCREENFILTERENTRY
#define FABLE_RETAIL_STLP_LIST
#define FABLE_RETAIL_STLP_MAP
#define FABLE_RETAIL_STLP_TREE_ERASE
#define FABLE_RETAIL_STLP_TREE_ERASE_BODY
#define FABLE_RETAIL_CSCREENEFFECTPRIORITYSTACK_INSERT_FILTER_PASS30
#define FABLE_RETAIL_CINTELLIGENTPOINTER_DEREFERENCE
#define FABLE_RETAIL_CINTELLIGENTPOINTER_COMPARE_NULL_INT
#define FABLE_RETAIL_IS_THING_ALIVE
#define FABLE_RETAIL_STLP_MAP_COPY_OUTOFLINE
#define FABLE_RETAIL_STLP_MAP_ORIGINAL_INSERT_BODY
#define FABLE_RETAIL_STLP_RB_GLOBAL_DECREMENT_SIZE_BODY
#define FABLE_RETAIL_CINTELLIGENTPOINTER_COPY
#include "fable_string.h"
#include "engine/CIntelligentPointer.h"
#include "stlport/fable_containers.h"
#include "stlport/fable_map.h"
#include "engine/CThing.h"
#include "engine/NGraphicAppearance_CTCGraphicAppearance.h"
#include "engine/NScreenEffect_CScreenEffectPriorityStack.h"
#include "engine/ThingAlive.h"
#include "engine/CMainGameComponent.h"

#include "engine/CEnvironmentSpellEffectInterpolator.h"

void* CEnvironmentSpellEffectInterpolator::ScreenFilterFadeTo(float time, NScreenEffect::ScreenEffectPriority priority,
    float alpha, float saturation, float contrast, float brightness, const CRGBFloatColour& tint,
    const NScreenEffect::CScreenEffectPriorityStack::ThingGraphicArgPassers& things)
{
    CEnvironmentSpellEffectInterface::CScreenFilterParams p;
    p.Alpha = alpha;
    p.Saturation = saturation;
    p.Contrast = contrast;
    p.Brightness = brightness;
    p.Tint = tint;
    return ScreenEffectPriorityStack.InsertScreenFilterEntry(time, p, things, priority);
}

#include <cstdio>
#include <cstdlib>
#include <cstring>
typedef NScreenEffect::CScreenEffectPriorityStack Stack;
typedef CEnvironmentSpellEffectInterface::CScreenFilterParams Filter;
static void check(bool v) { if (!v) std::abort(); }
static Stack* expectedStack;
static Stack::ThingGraphicArgPassers* expectedThings;
static Filter expected;
static float expectedDuration;
static NScreenEffect::ScreenEffectPriority expectedPriority;
static int handle, calls;
void* Stack::InsertScreenFilterEntry(float time,const Filter& p,const ThingGraphicArgPassers& things,NScreenEffect::ScreenEffectPriority priority) {
    check(this == expectedStack && &things == expectedThings && time == expectedDuration && priority == expectedPriority);
    check(!std::memcmp(&p,&expected,sizeof(p)));
    ++calls;
    return &handle;
}
Stack::~CScreenEffectPriorityStack() {}
CBaseIntelligentPointer::~CBaseIntelligentPointer() {}
int main() {
    CEnvironmentSpellEffectInterpolator interpolator;
    expectedStack = reinterpret_cast<Stack*>(reinterpret_cast<char*>(&interpolator) + 0x90);
    Stack::ThingGraphicArgPassers things;
    expectedThings = &things;
    for (int i = 0; i != 9; ++i) {
        expected.Alpha = float(i); expected.Saturation = -float(i);
        expected.Contrast = 4.75f; expected.Brightness = -0.625f;
        expected.Tint.Initialise(0.25f,0.5f,1.f,float(i+1));
        expectedPriority = (NScreenEffect::ScreenEffectPriority)(i%3);
        expectedDuration = (i-3)*0.5f;
        void* result = interpolator.ScreenFilterFadeTo(expectedDuration,expectedPriority,expected.Alpha,expected.Saturation,
            expected.Contrast,expected.Brightness,expected.Tint,things);
        check(result == &handle);
    }
    check(calls == 9);
    std::puts("006b85d0_TEST PASS");
    return 0;
}
