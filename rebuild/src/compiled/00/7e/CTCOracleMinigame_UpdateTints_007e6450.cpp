// CTCOracleMinigame::UpdateTints() -- when no symbol is firing and the parent thing has no expression component, take the next
//   pending input: raise game event 0x27 (the hero's player number + the expression 0..3 or -1), pop it, play the sequence-incorrect
//   sound and quit when it is not the expected output, play the correct sound, then start that symbol firing. Every firing symbol then
//   tints its graphic appearance (255 - def tint * GFSin(t * 0.5)) over TimeBetweenOutputs and removes the tint when done.
// Retail: Fable.exe (TLC, VC7.1) @ 0x007E6450, 756 bytes incl. the 4-entry jump table 0x007E6734, thiscall, plain `ret`, /GS cookie
//   (the local CGameEvent's char array).
// Identity: Ego_r.pdb ?UpdateTints@CTCOracleMinigame@@QAEXXZ 0x007E1910 between Quit and FrameUpdate (retail order the same); only
//   caller 0x007E676B in landed FrameUpdate 0x007E6750.
// Callees: CPlayerManager::GetMainPlayer const 0x00449960, CPlayer::GetPControlledCreature 0x00487DC0, CGameEvent::AddData 0x009F1760,
//   CMainGameComponent::AddGameEvent 0x0041649C, free 0x00BFEA14 (list pop_front), CCharString 0x0099EBF0 / ~CCharString 0x0099EAE0,
//   PlaySoundW 0x007E60F0, Quit 0x007E62D0, CWorld::GetFrame 0x0049D870, CVectorMap::LowerBound 0x0040F020 (GetTC),
//   CTCBase::GetWorld 0x00686A70, CTCGraphicAppearance::RemoveTintColour 0x004BFAE0 / SetTintColour 0x004C00B0, __ftol2 0x00BFEA70.
// Shape: OutputsFiring (0x007E5D00, same TU) is expanded in place (its landed body, CTCOracleMinigame_OutputsFiring_007e5d00.cpp, is inline here); the expression local is
//   initialised to -1 before the switch (`or ebp,-1`); GetWorld() is called and bound to a reference whose static GetFrame is used
//   (retail calls 0x00686A70 then reads ConstantFPS before GetFrame); s is a float (float 2^32 / 255.0f pool constants).
// Headers: overlay engine/NGraphicAppearance_CTCGraphicAppearance.h FABLE_RETAIL_CTCGRAPHICAPPEARANCE_REMOVETINT.
// Leaf: ?UpdateTints@CTCOracleMinigame@@QAEXXZ
// Flags: /GS
#define FABLE_RETAIL_CCHARSTRING
#define FABLE_RETAIL_STLP_LIST
#define FABLE_RETAIL_CENGINEPERSISTENTPRIMITIVEBASE
#define FABLE_RETAIL_DEF_CLASS_BASE
#define FABLE_RETAIL_CINTELLIGENTPOINTER
#define FABLE_RETAIL_CTCBASE
#define FABLE_RETAIL_CTCORACLEMINIGAME_CLASS
#define FABLE_RETAIL_CGAMECOMPONENT
#define FABLE_RETAIL_CMAINGAMECOMPONENT
#define FABLE_RETAIL_CMAINGAMECOMPONENT_PLAYERMANAGER
#define FABLE_RETAIL_CMAINGAMECOMPONENT_ADDGAMEEVENT
#define FABLE_RETAIL_CPLAYERMANAGER
#define FABLE_RETAIL_PLAYER
#define FABLE_RETAIL_CTHING
#define FABLE_RETAIL_CTHINGPLAYERCREATURE
#define FABLE_RETAIL_CTHINGPHYSICAL
#define FABLE_RETAIL_CTHINGGAMEOBJECT
#define FABLE_RETAIL_CTHINGCREATUREBASE
#define FABLE_RETAIL_CTHING_GETTC_EXPANDED
#define FABLE_RETAIL_CGAMEEVENT_INLINE
#define FABLE_RETAIL_CGAMEEVENT_INLINE_ADDMORE
#define FABLE_RETAIL_CTCGRAPHICAPPEARANCE_TINT
#define FABLE_RETAIL_CTCGRAPHICAPPEARANCE_REMOVETINT
#include "fable_string.h"
#include "engine/CRGBColour.h"
#include "engine/CThing.h"
#include "engine/CVectorMap.h"
#include "engine/CTCOracleMinigame.h"
#include "engine/CMainGameComponent.h"
#include "engine/CPlayerManager.h"
#include "engine/CPlayer.h"
#include "stlport/fable_containers.h"
#include "engine/C3DVector.h"
#include "engine/C2DVector.h"
#include "engine/CThingCreatureBase.h"
#include "engine/CThingPlayerCreature.h"
#include "engine/CGameEvent.h"
#include "engine/CWorld.h"
#include "engine/GFSin.h"
#include "engine/NGraphicAppearance_CTCGraphicAppearance.h"

// OutputsFiring 0x007E5D00: body copied verbatim from the landed row
//   rebuild/src/compiled/00/7e/CTCOracleMinigame_OutputsFiring_007e5d00.cpp (same retail TU; retail expands it here).
inline bool CTCOracleMinigame::OutputsFiring()
{
    for (int i = 0; i < 4; i++)
    {
        if (this->SymbolsFiring[i] != 0)
            return true;
    }
    return false;
}

void CTCOracleMinigame::UpdateTints()
{
    if (!OutputsFiring() && !ParentThing->HasTC(TCI_EXPRESSION)) {
        if (InputsPending.size()) {
            EOracleMinigameInput input = InputsPending.front();
            long expression = -1;
            switch (input) {
            // Retail jump table 0x007E6734: input 0 -> 0x007E64CB (0), 1 -> 0x007E64DD (3), 2 -> 0x007E64CF (1), 3 -> 0x007E64D6 (2).
            // The bodies are laid out in source order (0, 1, 2, 3 by the value stored), so the cases are written in that order.
            case 0: expression = 0; break;
            case 2: expression = 1; break;
            case 3: expression = 2; break;
            case 1: expression = 3; break;
            }
            const CThingPlayerCreature* hero = CMainGameComponent::Get()->PeekPlayerManager().GetMainPlayer().GetPControlledCreature();
            CGameEvent event(0x27);
            event.SetPlayer(hero->GetPlayerNumber());
            event.AddSLONG(expression);
            CMainGameComponent::Get()->AddGameEvent(event);
            InputsPending.pop_front();
            if (Output[NumOutputsFired++] != input) {
                PlaySoundW("SND_MM_EXPRESSION_SEQUENCE_INCORRECT");
                Quit();
            }
            PlaySoundW("SND_MM_EXPRESSION_SEQUENCE_CORRECT");
            FramesFired[input] = CWorld::GetFrame();
            ++SymbolsFiring[input];
        }
    }
    NGraphicAppearance::CTCGraphicAppearance* appearance;
    for (long i = 0; i < 4; ++i) {
        if (SymbolsFiring[i] == 0)
            continue;
        Symbols[i]->GetTC(TCI_GRAPHIC_APPEARANCE_NEW, &appearance);
        CWorld& world = GetWorld();
        long fps = CMainGameComponent::GetConstantFPS();
        float t = (float)(world.GetFrame() - FramesFired[i]) / (fps * TimeBetweenOutputs);
        if (t > 1.0f) {
            if (--SymbolsFiring[i])
                FramesFired[i] = CWorld::GetFrame();
            appearance->RemoveTintColour(this);
        } else {
            float s = GFSin(t * 0.5f);
            CRGBColour colour((unsigned char)(255.0f - PDef->TintRed * s), (unsigned char)(255.0f - PDef->TintGreen * s),
                              (unsigned char)(255.0f - PDef->TintBlue * s), 0xff);
            appearance->SetTintColour(colour, this);
        }
    }
}