// CTCCreatureGenerator::DoNavCheck(const CThingCreatureBase& creature) -- drop every open generation position the creature cannot navigate to from the generator (the last position is copied over it and popped)
// Grade: DIFFER(232v234) -- (w22 agent1016) the creature reference is loaded once into eax with `lea ebx,[eax+44h]` where retail reloads `mov ebx,[esp+20h]` after GetPos and `add ebx,44h`; the constant-0 split piece takes ebx (`xor ebx,ebx; cmp edi,ebx; mov [i],ebx`) where retail has `test edi,edi`, an immediate store of i and `xor ebx,ebx` after the jle. For/while/i-at-top/PeekTC-default variants inert or worse.
// Leaf: ?DoNavCheck@CTCCreatureGenerator@@IAEXABVCThingCreatureBase@@@Z
// Retail: Fable.exe (TLC, VC7.1) @ 0x0074A4E0, 234 bytes, thiscall, `ret 4`.
#define FABLE_RETAIL_DEF_CLASS_BASE
#define FABLE_RETAIL_BASE_OBJECT
#define FABLE_RETAIL_CTCBASE
#define FABLE_RETAIL_CINTELLIGENTPOINTER
#define FABLE_RETAIL_STLP_LIST
#define FABLE_RETAIL_CTHING
#define FABLE_RETAIL_CTHING_GETTC_EXPANDED
#define FABLE_RETAIL_CTHINGPHYSICAL
#define FABLE_RETAIL_CTHINGGAMEOBJECT
#define FABLE_RETAIL_CTHINGCREATUREBASE
#define FABLE_RETAIL_CTCCREATUREGENERATOR
#define FABLE_RETAIL_CENGINEPERSISTENTPRIMITIVEBASE
#define FABLE_RETAIL_CTCCREATURENAVIGATION
#define FABLE_RETAIL_CTCCREATURENAVIGATION_NAVIGABLE
#include "fable_string.h"
#include "engine/CThingCreatureBase.h"
#include "engine/CTCCreatureNavigation.h"
#include "engine/CTCCreatureGenerator.h"

void CTCCreatureGenerator::DoNavCheck(const CThingCreatureBase& creature)
{
    long numPositions = OpenPositions.size();
    if (numPositions > 0) {
        C3DVector generatorPos = GetParentThing().GetPos();
        const CTCCreatureNavigation* navigation;
        creature.PeekTC(&navigation);
        long count = numPositions;
        for (long i = 0; i < count;) {
            if (!navigation->IsAbleToNavigateFromPosToPos(generatorPos, OpenPositions[i], 0.0f)) {
                OpenPositions[i] = OpenPositions[--count];
                OpenPositions.pop_back();
            } else {
                ++i;
            }
        }
    }
}
