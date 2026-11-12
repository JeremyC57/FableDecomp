// CAIStateGroup_BanditPlot::Act() -- listen to the bandits' plans for 375..674 frames: now and then (one in five, with no current player and two or more players at the plot) take the lead and act out the sword-on-the-floor plan, otherwise loop the listening animation (one in ten a breakdown); return the first failing action result, else the result of the closing animation
// Leaf: ?Act@CAIStateGroup_BanditPlot@@MAE?AW4EAIActionReturn@@XZ
// Retail: Fable.exe (TLC, VC7.1) @ 0x009289B0, 1131 bytes (aligned frame, no EH frame).
// Callees: CCharString(const char*, long) 0x0099EBF0 / ~CCharString 0x0099EAE0, CCreatureAction_PlayAnimation ctor 0x008424F0,
//   AIAction_PerformActionAndWait 0x0097DB70, ~CCreatureActionBase 0x00693EF0 (implicit PlayAnimation dtor), GetPWorldSeed
//   0x008FCFD0, GFROR13 0x00497890 (GFRandom inline), CWorld::GetFrame 0x0049D870, GFFloatRandom 0x004978A0, rand 0x00BFEB16,
//   CTCDiceGame::SetCurrentPlayer 0x00929550 (Ego_r.pdb ?SetCurrentPlayer@CTCDiceGame@@QAEXPAVCThing@@@Z; the landed row there is
//   spelled _unattributed::AssignTarget_00929550, and the manifest's CVillageTaskCarryCrate::AssignCreatureToTask is a folded twin),
//   CBaseIntelligentPointer ctor 0x00A01B10 / GetPItem 0x00A01B50 / SetPItem 0x00A01B90 / dtor 0x00A01C10. Chance 0.2f @ 0x012917C8.
// Layout: RTTI .?AVCAIStateGroup_BanditPlot@@ derives CAIStateGroup_BanditPlayDice (both sizeof 0x28 in Ego_r.pdb, no own members),
//   so the plot is the inherited DiceGame +0x18 (CTCDiceGame: NumberOfPlayers +0xc, CurrentPlayer +0x10; CTCPlot derives
//   CTCDiceGame). Overlay opt-ins FABLE_RETAIL_CAISTATEGROUP_BANDITPLAYDICE / _BANDITPLOT (hdr/README.md).
// Shape: the landed sibling 0x009285D0 BanditPlayDice::Act's flow -- early `return result` after the INTO animation and inside
//   the loop, `return` of the OUTOF call -- puts the end frame in ebp and the constant 2 in ebx; wrapping the body in
//   `if (result == 2) {...}` with one shared final return costs a duplicated tail (1133v1131). The table pointer is named
//   (`CTCDiceGame* game = DiceGame;`) before the CIntelligentPointer<CThing>(0) temporary, as in the sibling.
#define FABLE_RETAIL_CCHARSTRING
#define FABLE_RETAIL_DEF_CLASS_BASE
#define FABLE_RETAIL_CTCBASE
#define FABLE_RETAIL_CTHING
#define FABLE_RETAIL_CTHING_GETTC_EXPANDED
#define FABLE_RETAIL_CTHINGPHYSICAL
#define FABLE_RETAIL_CTHINGGAMEOBJECT
#define FABLE_RETAIL_CTHINGCREATUREBASE
#define FABLE_RETAIL_CTHINGAICREATURE
#define FABLE_RETAIL_CINTELLIGENTPOINTER
#define FABLE_RETAIL_CINTELLIGENTPOINTER_ASSIGN
#define FABLE_RETAIL_CCREATUREACTIONBASE
#define FABLE_RETAIL_CCREATUREACTION_PLAYANIMATION
#define FABLE_RETAIL_CCREATUREACTION_PLAYANIMATION_IMPLICIT_DTOR
#define FABLE_RETAIL_CAISTATEGROUPBASE
#define FABLE_RETAIL_CAISTATEGROUPBASE_MEMBERS
#define FABLE_RETAIL_CAISTATEGROUPBASE_GETCREATURE
#define FABLE_RETAIL_CAISTATEGROUPBASE_PERFORMACTION
#define FABLE_RETAIL_CAIBRAIN_GETCREATURE
#define FABLE_RETAIL_CAIBRAIN_CREATURE_REFERENCE
#define FABLE_RETAIL_CWORLD
#define FABLE_RETAIL_CAISTATEGROUP_BANDITPLAYDICE
#define FABLE_RETAIL_CAISTATEGROUP_BANDITPLOT
#include "fable_string.h"
#include "stlport/fable_containers.h"
#include "engine/fable_ai_action_enums.h"
#include "engine/ETCInterfaceType.h"
#include "engine/CTCBase.h"
#include "engine/CThing.h"
#include "engine/CThingAICreature.h"
#include "engine/CIntelligentPointer.h"
#include "engine/CSoundPair.h"
#include "engine/CCreatureAction_PlayAnimation.h"
#include "engine/CAIStateGroupBase.h"
#include "engine/CAIBrain.h"
#include "engine/CWorld.h"
#include "engine/GFRandom.h"
#include "engine/GFFloatRandom.h"
#include "engine/CAIStateGroup_BanditPlot.h"
extern "C" int __cdecl rand();                                          // 0x00BFEB16

// CTCDiceGame has no shared header: Ego_r.pdb NumberOfPlayers +0x0c, CurrentPlayer +0x10 (CIntelligentPointer<CThing>);
// ?SetCurrentPlayer@CTCDiceGame@@QAEXPAVCThing@@@Z (retail 0x00929550).
class CTCDiceGame : public CTCBase {
public:
    void SetCurrentPlayer(CThing* player);                              // 0x00929550
    long NumberOfPlayers;                                               // +0x0c
    CIntelligentPointer<CThing> CurrentPlayer;                          // +0x10
};

EAIActionReturn CAIStateGroup_BanditPlot::Act()
{
    EAIActionReturn result = AIAction_PerformActionAndWait(CCreatureAction_PlayAnimation(GetCreature(),
        "ST_LISTENING_TO_PLANS_INTO", CSoundPair(), false, true, false, 0, 0), false);
    if (result != AI_ACTION_FINISHED)
        return result;
    unsigned long wait = GFRandom(300, GetPWorldSeed());
    long endFrame = CWorld::GetFrame() + wait + 375;
    while (CWorld::GetFrame() < endFrame) {
        if (DiceGame->NumberOfPlayers >= 2 && !DiceGame->CurrentPlayer.Get() && GFFloatRandom(1.0f, GetPWorldSeed()) < 0.2f) {
            DiceGame->SetCurrentPlayer(&GetCreature());
            AIAction_PerformActionAndWait(CCreatureAction_PlayAnimation(GetCreature(), "ST_LISTENING_TO_PLANS_OUTOF",
                CSoundPair(), false, true, false, 0, 0), false);
            AIAction_PerformActionAndWait(CCreatureAction_PlayAnimation(GetCreature(),
                "ST_MAKING_PLANS_ON_FLOOR_WITH_SWORD_INTO", CSoundPair(), false, true, false, 0, 0), false);
            for (long i = 0; i < 2; ++i)
                AIAction_PerformActionAndWait(CCreatureAction_PlayAnimation(GetCreature(),
                    "ST_MAKING_PLANS_ON_FLOOR_WITH_SWORD_LOOP", CSoundPair(), false, true, false, 0, 0), false);
            AIAction_PerformActionAndWait(CCreatureAction_PlayAnimation(GetCreature(),
                "ST_MAKING_PLANS_ON_FLOOR_WITH_SWORD_OUTOF", CSoundPair(), false, true, false, 0, 0), false);
            result = AIAction_PerformActionAndWait(CCreatureAction_PlayAnimation(GetCreature(),
                "ST_LISTENING_TO_PLANS_INTO", CSoundPair(), false, true, false, 0, 0), false);
            CTCDiceGame* game = DiceGame;
            game->CurrentPlayer = CIntelligentPointer<CThing>(0);
        } else if (rand() % 10 == 0)
            result = AIAction_PerformActionAndWait(CCreatureAction_PlayAnimation(GetCreature(),
                "ST_LISTENING_TO_PLANS_BREAKDOWN", CSoundPair(), false, true, false, 0, 0), false);
        else
            result = AIAction_PerformActionAndWait(CCreatureAction_PlayAnimation(GetCreature(),
                "ST_LISTENING_TO_PLANS_LOOP", CSoundPair(), false, true, false, 0, 0), false);
        if (result != AI_ACTION_FINISHED)
            return result;
    }
    return AIAction_PerformActionAndWait(CCreatureAction_PlayAnimation(GetCreature(), "ST_LISTENING_TO_PLANS_OUTOF",
        CSoundPair(), false, true, false, 0, 0), false);
}