// NPlayerGui::CDrawSpiritTargetingCursor::Draw(const CEnginePrimitivePrepareInfo& info) -- for each of the four player slots whose
// player is not the main player and whose controlled creature is alive: follow that hero's targeting component to its targeted thing
// (remembering it in PTargetedThings[i] when it changed) and, while that thing is alive and the display is not in letter-box mode,
// mark the illustrator as drawing and add the spirit targeting cursor sprite (the gui def's TargetingCursorSectionEnemyEmptyGraphic)
// at the thing's targeting position raised by its physics radius + 0.35, tinted with the player's multiplayer colour.
// Leaf: ?Draw@CDrawSpiritTargetingCursor@NPlayerGui@@QAEXABVCEnginePrimitivePrepareInfo@@@Z
// Retail: Fable.exe (TLC, VC7.1) @ 0x0064C869, 428 bytes, __fastcall, `ret 4`; ebp frame (`push ebp ; mov ebp,esp ; sub esp,74h`)
// and size idioms (`push 8 ; pop ebx`, `and [ebp-4],0`, `leave`): #pragma optimize("y", off) + ("s", on), i.e. /O1 /Oy- like the
// next row CDrawWorldTime::Draw 0x0064CA15. esi = this, i lives in [ebp-4], edi = hero then i*8, [ebp-8] = the player.
// Identity: Ego_r.pdb (0x0068DAF6) / FableWin.pdb (0x017CB01E) ?Draw@CDrawSpiritTargetingCursor@NPlayerGui@@QAEXABVCEnginePrimitivePrepareInfo@@@Z.
// Callees (retail order): CPlayerManager::GetPlayer 0x004498C0, CPlayer::GetPControlledCreature 0x00487DC0, CPlayer::IsMainPlayer
// 0x00487520, IS_THING_ALIVE<const CThingPlayerCreature*> 0x0043DBF0, std::bitset::test 0x00410DE0 + CVectorMap::LowerBound const
// 0x004365B0 (CThing::PeekTC expanded in place), CTCTargetingBase::GetPTargetedThing slot 33 (+0x84), operator!=(const CBaseObject*,
// const CIntelligentPointer<CThing>&) 0x005CBDD2, CIntelligentPointer<CThing>::CIntelligentPointer(CThing*) 0x004A96F0 / operator=
// 0x004A9760 / ~CBaseIntelligentPointer 0x00A01C10 (the converted temporary of the assignment), IS_THING_ALIVE<CIntelligentPointer
// <CThing> > 0x004CC340, CPlayerGui::PeekGuiDef 0x00438570, CBaseIntelligentPointer::GetPItem 0x00A01B50,
// NTargeting::GetTargetingPositionOnThing 0x007EA2F0, CPlayerManager::GetMultiplayerColour 0x00449B60, CEnginePrimitiveSprite's
// 15-argument constructor 0x006558CE, CIEngine::AddPrimitive slot 20 (+0x50). Inline in retail: CMainGameComponent::PeekDisplayEngine
// (+0x28), CDisplayEngine::IsLetterBoxMode (+0x70), CDrawBase::SetAsDrawing (+0x18), CPlayer::GetNumber (+0x28),
// CThing::GetPhysicsTC (+0x60), CTCPhysicsBase::GetRadius (+0x34). Float pool: 0.35f (0x0125A264).
// Shape: the hero pointer is homed at [ebp-10h] for IS_THING_ALIVE's `const T&` yet stays in edi afterwards, and the same word is
// reused for PeekTC's key and the gui def reference. VC7.1 only does that when it can see IS_THING_ALIVE's body (it then knows the
// callee keeps no reference to the local): the real body from engine/ThingAlive.h, kept out of line (FABLE_RETAIL_IS_THING_ALIVE_NOINLINE)
// because retail calls both COMDATs. With a bare declaration the hero gets its own slot and is reloaded (429 bytes).
// Layout (retail, this+): CDrawBase PlayerManager +0x08, World +0x0C, Component +0x10, Drawing +0x18; PTargetedThings +0x28
// (vector of 8-byte CIntelligentPointer<CThing>; `[esi+28h] + i*8`).
#pragma optimize("y", off)
#pragma optimize("s", on)
#define FABLE_RETAIL_CCHARSTRING
#define FABLE_RETAIL_CWIDESTRING
#define FABLE_RETAIL_CDEFSTRINGTABLE
#define FABLE_RETAIL_CCLASS
#define FABLE_RETAIL_CTCBASE
#define FABLE_RETAIL_DEF_CLASS_BASE
#define FABLE_RETAIL_CTHING
#define FABLE_RETAIL_CTHING_GETTC_EXPANDED
#define FABLE_RETAIL_STLP_BITSET_TEST_DECL
#define FABLE_RETAIL_CINTELLIGENTPOINTER
#define FABLE_RETAIL_CINTELLIGENTPOINTER_CTOR_OUTOFLINE
#define FABLE_RETAIL_CINTELLIGENTPOINTER_ASSIGN_OUTOFLINE
#define FABLE_RETAIL_CINTELLIGENTPOINTER_COMPARE
#define FABLE_RETAIL_CTHINGPHYSICAL
#define FABLE_RETAIL_CTHINGGAMEOBJECT
#define FABLE_RETAIL_CTHINGCREATUREBASE
#define FABLE_RETAIL_CTHINGPLAYERCREATURE
#define FABLE_RETAIL_PLAYER
#define FABLE_RETAIL_PLAYER_GETNUMBER
#define FABLE_RETAIL_CPLAYERMANAGER
#define FABLE_RETAIL_CPLAYERMANAGER_MULTIPLAYERCOLOUR
#define FABLE_RETAIL_PLAYER_ISMAINPLAYER_OUTOFLINE
#define FABLE_RETAIL_CMAINGAMECOMPONENT
#define FABLE_RETAIL_CPLAYERGUI
#define FABLE_RETAIL_CPLAYERGUI_PEEKGUIDEF
#define FABLE_RETAIL_CIENGINE
#define FABLE_RETAIL_CTCTARGETINGBASE_OFTYPE
#define FABLE_RETAIL_CTCPHYSICSBASE
#define FABLE_RETAIL_CMAINGAMECOMPONENT_PEEKDISPLAYENGINE
#define FABLE_RETAIL_CDISPLAYENGINE_ISLETTERBOXMODE
#define FABLE_RETAIL_NPLAYERGUI_CDRAWBASE_SETASDRAWING
#define FABLE_RETAIL_CENGINEPRIMITIVEPREPAREINFO
#define FABLE_RETAIL_CENGINEPRIMITIVE2DSPRITE_ENUM_CTORS
#define FABLE_RETAIL_CRGBCOLOUR
#define FABLE_RETAIL_C2DVECTOR
#define FABLE_RETAIL_CENGINEPERSISTENTPRIMITIVEBASE
#define FABLE_RETAIL_BASE_OBJECT
#define FABLE_RETAIL_IS_THING_ALIVE
#define FABLE_RETAIL_CINTELLIGENTPOINTER_COMPARE_NULL_INT
#define FABLE_RETAIL_CINTELLIGENTPOINTER_DEREFERENCE
#define FABLE_RETAIL_IS_THING_ALIVE_NOINLINE
#include "fable_string.h"
#include "stlport/fable_containers.h"
#include "engine/C3DVector.h"
#include "engine/CRGBColour.h"
#include "engine/CIntelligentPointer.h"
#include "engine/CThing.h"
#include "engine/CThingCreatureBase.h"
#include "engine/CThingPlayerCreature.h"
#include "engine/CTCTargetingBase.h"
#include "engine/CTCPhysicsBase.h"
#include "engine/NTargeting.h"
#include "engine/ThingAlive.h"
#include "engine/CPlayer.h"
#include "engine/CPlayerManager.h"
#include "engine/CPlayerGui.h"
#include "engine/CPlayerGuiDef.h"
#include "engine/CWorld.h"
#include "engine/CMainGameComponent.h"
#include "engine/CDisplayEngine.h"
#include "engine/CIEngine.h"
#include "engine/CEnginePrimitivePrepareInfo.h"
#include "engine/CEnginePrimitiveSprite.h"
#include "engine/NPlayerGui_CDrawBase.h"
#include "engine/NPlayerGui_CDrawSpiritTargetingCursor.h"


void NPlayerGui::CDrawSpiritTargetingCursor::Draw(const CEnginePrimitivePrepareInfo& info)
{
    for (long i = 0; i < 4; ++i) {
        CPlayer& player = PlayerManager->GetPlayer(i);
        const CThingPlayerCreature* hero = player.GetPControlledCreature();
        if (player.IsMainPlayer() || !IS_THING_ALIVE(hero))
            continue;
        const CTCTargetingBase* targeting;
        hero->PeekTC(TCI_TARGETING, &targeting);
        if (targeting->GetPTargetedThing() != PTargetedThings[i])
            PTargetedThings[i] = targeting->GetPTargetedThing();
        if (IS_THING_ALIVE(PTargetedThings[i]) && !Component->PeekDisplayEngine().IsLetterBoxMode()) {
            SetAsDrawing(true);
            const CPlayerGuiDef& guiDef = CPlayerGui::PeekGuiDef();
            CThing* target = PTargetedThings[i].Get();
            C3DVector pos = NTargeting::GetTargetingPositionOnThing(*World, *target, info.Interpolate.Current.WFInterpolate);
            pos.Z = pos.Z + target->GetPhysicsTC()->GetRadius() + 0.35f;
            CRGBColour colour = PlayerManager->GetMultiplayerColour(player.GetNumber());
            CEnginePrimitiveSprite sprite(pos, guiDef.TargetingCursorSectionEnemyEmptyGraphic, 0, 0x81, 0.0f, 1.0f, 1.0f,
                                          (EEnginePrimitiveBlendMode)2, 0xff, colour.R, colour.G, colour.B, 0.0f,
                                          (EEngineInterpolationSet)0, (EEnginePrimitiveBlendOp)0);
            info.Engine.AddPrimitive(sprite);
        }
    }
}