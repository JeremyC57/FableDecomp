#define FABLE_RETAIL_STLP_PASS22_FLARE_ABI
// Leaf: ?OnInitialActivate@CTCHeroCentre@@UAEXXZ
// Flags: /GS
#define FABLE_RETAIL_BASE_OBJECT
#define FABLE_RETAIL_CCLASS
#define FABLE_RETAIL_DEF_CLASS_BASE
#define FABLE_RETAIL_CCHARSTRING
#define FABLE_RETAIL_CCHARSTRING_STARTSWITH
#define FABLE_RETAIL_CTHING
#define FABLE_RETAIL_CTHING_STATEACCESS
#define FABLE_RETAIL_CTHING_GETSCRIPTNAME
#define FABLE_RETAIL_CTHING_GETTC_ORIGINAL_CHAIN
#define FABLE_RETAIL_IS_THING_ALIVE
#define FABLE_RETAIL_CTCBASE
#define FABLE_RETAIL_CTCTHINGOWNER
#define FABLE_RETAIL_CINTELLIGENTPOINTER
#define FABLE_RETAIL_CINTELLIGENTPOINTER_COPY
#define FABLE_RETAIL_CINTELLIGENTPOINTER_COPY_ORIGINAL_CHAIN
#define FABLE_RETAIL_C3DVECTOR
#define FABLE_RETAIL_STLP_VECTOR
#define FABLE_RETAIL_STLP_LIST
#define FABLE_RETAIL_STLP_SORT_FASTCALL
#define FABLE_RETAIL_CARRAY
#define FABLE_RETAIL_CVECTORMAP_CTOR
#define FABLE_RETAIL_CVECTORMAP_INSERT
#define FABLE_RETAIL_CVECTORMAP_INSERT_KEYVALUE
#define FABLE_RETAIL_CTCHEROCENTRE_CLASS
#define FABLE_RETAIL_HERO_CENTRE_ACTIVATE_PASS29
#include "engine/ETCInterfaceType.h"
#include "engine/CThing.h"
#include "engine/CTCHeroCentre.h"
#include "engine/CTCThingOwner.h"
#include "engine/ThingAlive.h"
namespace { extern CCharString QUEST_CARD_POS_MARKER_NAME_PREFIX; }

void CTCHeroCentre::OnInitialActivate()
{
    const CTCThingOwner* ptc_thing_owner;
    if (GetParentThing().PeekTC(&ptc_thing_owner)) {
        std::list<CIntelligentPointer<CThing> > owned_objects(ptc_thing_owner->GetObjectList());
        CVectorMap<CCharString, C3DVector> quest_card_positions;
        for (std::list<CIntelligentPointer<CThing> >::const_iterator it = owned_objects.begin(); it != owned_objects.end(); ++it) {
            const CThing* powned_thing = it->Get();
            if (IS_THING_ALIVE(powned_thing) && powned_thing->GetScriptName().StartsWith(QUEST_CARD_POS_MARKER_NAME_PREFIX))
                quest_card_positions.Insert(powned_thing->GetScriptName(), powned_thing->GetPos());
        }
        quest_card_positions.Update();
        for (long c1 = 0; c1 < quest_card_positions.size(); ++c1)
            QuestCardPositions.push_back(quest_card_positions[c1].second);
    }
}