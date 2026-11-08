// PARKED (chunk 867, 10 grades + 24 compiler probes): best grade v6 = DIFFER(2223v2223) md=40 / 20 masked bytes, every hunk a frame
// slot number. Retail gives the discarded GFAnchorToTopRight result and the two GetRenderTargetDimensions temporaries of the right
// border their own 8-byte slot (+0x38) and keeps the targeting key / CDoorDef handle / CIntelligentPointer temporary above the
// dimension temporaries (+0x64 / +0x60 over +0x58); VC7.1 here packs the anchor temporary into the dimension slot (frame 8 bytes
// smaller) or, with the named `rightPos` below, keeps the named local apart from the later temporaries. Spellings tried (grades v5-v10q,
// probes probe/slots_*.cpp): unnamed call, named local, const reference, nested / sibling scopes on either half, a second named box,
// the left sprite as a temporary joined to the anchor by a comma (reproduces the three-slot pattern but moves the sprite objects),
// inlined helpers, destructors on the sprite, QFE 4035 toolset: none reproduces the third slot with the sprite objects in place.
// The open quantity is c2's temporary-slot grouping across the two sprite statements (see NOTES.md).
// NPlayerGui::CDrawHighlightingTargetingCursor::Draw(const CEnginePrimitivePrepareInfo& info) -- per-frame update of the targeting
// cursor: follow the hero's targeting component to its current target (clearing the mesh glow of the one it leaves), work out which
// cursor the target deserves (use / attack / talk / talk-with-interest, demoted to none for an unusable action-use thing and promoted
// by the hero guide, the demon-door faces and the Z-targeted battle-charge victim), fade the glow colour / widths from the gui def's
// start values to its end values over HighlightFadeSteps frames and apply them to the target's mesh, and while the hero Z-targets an
// attackable thing draw the two targeting-border sprites (left anchored at TargetingBorderLeftPos.X, right mirrored from the render
// target's width, both scaled from 640x480 to the render target) as persistent primitives, else clear the primitive handles.
// Leaf: ?Draw@CDrawHighlightingTargetingCursor@NPlayerGui@@QAEXABVCEnginePrimitivePrepareInfo@@@Z
// Retail: 0x0064BFBA, 2223 bytes, `ret 4`; biased frame (`push ebp ; lea ebp,[esp-74h] ; sub esp,0E4h`: /Os alone, as the other
// NPlayerGui::CDraw*::Draw rows), esi = this, edi = the controlled creature then the gui def, ebx = the targeting component.
// Identity: Ego_r.pdb / FableWin.pdb ?Draw@CDrawHighlightingTargetingCursor@NPlayerGui@@QAEXABVCEnginePrimitivePrepareInfo@@@Z; the
// row reads this+0x28..+0x44 as the Ego_r members of the class (struct_layouts_egor.tsv) and calls the CDrawBase members.
// Callees (retail order): CPlayerManager::GetMainPlayer 0x00449970, CPlayer::PeekPControlledCreature 0x00487DD0, bitset::test
// 0x00410DE0 (HasTC), CVectorMap::LowerBound const 0x004365B0 / non-const 0x0040F020 (PeekTC / GetTC), CPlayerGui::PeekGuiDef
// 0x00438570, CTCTargetingBase::GetPTargetedThing slot 33 (+0x84), operator!=<CThing> 0x005CBDD2, IS_THING_ALIVE<CIntelligentPointer
// <CThing> > 0x004CC340, CBaseIntelligentPointer::GetPItem 0x00A01B50, CTCGraphicAppearance::ClearMeshEffect 0x004BFFC0,
// CIntelligentPointer<CThing>::CIntelligentPointer(CThing*) 0x004A96F0 / operator= 0x004A9760 / ~CBaseIntelligentPointer 0x00A01C10,
// CTCTargetingBase::IsTargetedThingOfType 0x007F5A70, IsTargetedThingInRange slot 38 (+0x98),
// NTargeting::IsThingVulnerableToMeleeStrikesFromThing 0x007F59F0, CDefClassBase::GetSubDef<CDoorDef> 0x00655D28 (this TU's COMDAT),
// CThingAICreature::GetHasInformation 0x008300D0, CTCShopKeeper::GetShopTC 0x00798F50, CTCShop::IsShopKeeperAvailable 0x00773AA0,
// CTCWife::IsMarriedToThePlayer 0x006CEC80 / IsInLoveWithHero 0x006CF900 / IsOfferingSex 0x006CEEA0, CThing::GetDefName 0x004C7CC0,
// CDefString::operator==(const char*) 0x005B3440, CTCGraphicAppearance::SetAsHighlighted 0x004C04C0, CUserProfileManager::Get
// 0x0040D2A0, CPlayer::GetCurrentMode 0x00633BE0 / HasMode 0x00634160, CDisplayManager::GetRenderTargetDimensions 0x009BEDC0,
// CGraphicDataBank::GetGraphicMaxWidth slot 23 (+0x5C) / GetGraphicMaxHeight slot 24 (+0x60), CDrawBase::ScaleAlpha 0x00643D5D,
// CEnginePrimitive2DSprite::CEnginePrimitive2DSprite 0x0041BEB0, CIEngine::SetPersistentPrimitive slot 23 (+0x5C),
// NPlayerGui::GFAnchorToTopRight 0x00643CA1, std::for_each 0x0043F360 (CEnginePrimitiveHandle::Clear 0x0041BE70 as the functor).
// Float pool: 4294967296.0 (unsigned fild fix-up), 1/640 and 1/480 (the 640x480 GUI space to render-target scale), 0.5.
// Layout (retail, this+): CDrawBase PlayerManager +0x08, World +0x0C, Bank +0x14, Drawing +0x18, PrimitiveHandles +0x1C;
// PTargetedThing +0x28, CurrentColour +0x30, CurrentWidthScreenSpace +0x34, CurrentWidthWorldSpace +0x38, CurrentFadeStep +0x3C,
// CursorType +0x44. CPlayerGuiDef: TargetingBorderLeftGraphic +0x7D0, TargetingBorderLeftPos +0x7D4, TargetingBorderRightGraphic
// +0x7DC, TargetingBorderRightPos +0x7E0, HighlightStartColour +0x86C, HighlightEndColour +0x878, HighlightStartWidthWorldSpace
// +0x884, HighlightEndWidthWorldSpace +0x890, HighlightStartWidthScreenSpace +0x89C, HighlightEndWidthScreenSpace +0x8A8,
// HighlightFadeSteps +0x8B4. CUserProfileManager: UseNewProfileValues +0x0C, ShowTargetingStatus +0x6C,
// ShowTargetingStatusNewProfile +0x134. CTCBattleCharge: PTarget +0x0C, IsZTargetting +0x88. CTCTavernGameMaster::TavernGame +0x0C.
// CTCActionUseBase::Usable +0x10, CTCActionUseScriptedHook::Usable +0x0C, CTCTargeted::UseTargetGlow +0x11 (IsToUseTargetGlow),
// CWorld::MapTableShowQuestCardsOnUsed +0xDA, CEnginePrimitivePrepareInfo::Engine +0x34.
// Names: the enumerators of EHighlightingCursorType, ETargetingType and EAppearanceMeshEffectPriority are in no PDB on disk; the
// values are retail's (1, 1 and 3) and the spellings used here are PLACEHOLDERS flagged in NOTES.md.
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
#define FABLE_RETAIL_CTHINGAICREATURE
#define FABLE_RETAIL_CTHINGAICREATURE_INFORMATION
#define FABLE_RETAIL_PLAYER
#define FABLE_RETAIL_CPLAYERMANAGER
#define FABLE_RETAIL_CPLAYERGUI
#define FABLE_RETAIL_CPLAYERGUI_PEEKGUIDEF
#define FABLE_RETAIL_CDEFINITIONMANAGER
#define FABLE_RETAIL_CRESOURCELIST
#define FABLE_RETAIL_CTEXTURE
#define FABLE_RETAIL_CPIXELFORMAT
#define FABLE_RETAIL_CPLAYERGUIDEF_TARGETING
#define FABLE_RETAIL_CUSERPROFILEMANAGER_TARGETINGSTATUS
#define FABLE_RETAIL_CDISPLAYMANAGER
#define FABLE_RETAIL_CIENGINE
#define FABLE_RETAIL_CENGINEPERSISTENTPRIMITIVEBASE
#define FABLE_RETAIL_CGRAPHICDATABANK_GRAPHICSIZE
#define FABLE_RETAIL_NPLAYERGUI_CDRAWBASE_HANDLES
#define FABLE_RETAIL_CTCTARGETINGBASE_OFTYPE
#define FABLE_RETAIL_CTCGRAPHICAPPEARANCE_HIGHLIGHT
#define FABLE_RETAIL_NTARGETING_MELEE
#define FABLE_RETAIL_CDEFSTRING_EQUALS
#define FABLE_RETAIL_CDOORDEF
#define FABLE_RETAIL_CTCSHOPKEEPER_SHOP
#define FABLE_RETAIL_CTCSHOP_AVAILABLE
#define FABLE_RETAIL_CTCWIFE
#define FABLE_RETAIL_CTCTARGETED
#define FABLE_RETAIL_CTCTARGETED_GLOW
#define FABLE_RETAIL_CTHING_STATEACCESS
#define FABLE_RETAIL_SCOPED_PTR
#define FABLE_RETAIL_CTCACTIONUSESCRIPTEDHOOK_ISUSABLE
#include "fable_string.h"
#include "stlport/fable_containers.h"
#include "stlport/fable_algorithm.h"
#include "engine/C2DVector.h"
#include "engine/C2DBoxF.h"
#include "engine/C2DExtentsI.h"
#include "engine/CRGBColour.h"
#include "engine/CDefString.h"
#include "engine/CDefPointer.h"
#include "engine/CDefClassBase.h"
#include "engine/CSubDefClassBase.h"
#include "engine/CThingBaseDef.h"
#include "engine/CDoorDef.h"
#include "engine/CDefinitionManager.h"
#include "engine/CIntelligentPointer.h"
#include "engine/CThing.h"
#include "engine/CThingCreatureBase.h"
#include "engine/CThingPlayerCreature.h"
#include "engine/CThingAICreature.h"
#include "engine/CTCTargetingBase.h"
#include "engine/CTCTargeted.h"
#include "engine/CTCShopKeeper.h"
#include "engine/CTCShop.h"
#include "engine/CTCWife.h"
#include "engine/CTCBattleCharge.h"
#include "engine/CTCTavernGameMaster.h"
#include "engine/CTCActionUseBase.h"
#include "engine/CTCActionUseScriptedHook.h"
#include "engine/NGraphicAppearance_CTCGraphicAppearance.h"
#include "engine/NTargeting.h"
#include "engine/CPlayer.h"
#include "engine/CPlayerManager.h"
#include "engine/CPlayerGui.h"
#include "engine/CPlayerGuiDef.h"
#include "engine/CWorld.h"
#include "engine/CUserProfileManager.h"
#include "engine/CDisplayManager.h"
#include "engine/CGraphicDataBank.h"
#include "engine/CIEngine.h"
#include "engine/CEnginePrimitivePrepareInfo.h"
#include "engine/CEnginePrimitive2DSprite.h"
#include "engine/CEnginePrimitiveHandle.h"
#include "engine/NPlayerGui_CDrawBase.h"
#include "engine/NPlayerGui_CDrawHighlightingTargetingCursor.h"

using namespace NGraphicAppearance;

extern CDisplayManager* GPDisplayManager;                                 // 0x013B8390
template <class T> bool FABLE_FASTCALL IS_THING_ALIVE(const T& pointer);   // 0x004CC340 (CIntelligentPointer<CThing>)
namespace NPlayerGui { C2DVector FABLE_FASTCALL GFAnchorToTopRight(const C2DVector& pos); }   // 0x00643CA1

// ETargetingType / EAppearanceMeshEffectPriority: the enumerators are in no PDB on disk (PLACEHOLDER spellings, the retail values).
enum ETargetingType { TARGETING_TYPE_ACTION_USE = 1 };
namespace NGraphicAppearance { enum EAppearanceMeshEffectPriority { APPEARANCE_MESH_EFFECT_PRIORITY_HIGHLIGHT = 3 }; }

// CDefClassBase::GetSubDef<CDoorDef> 0x00655D28: this TU's instantiation of the sub-def lookup (the same body as the landed
// GetSubDef<T> rows, e.g. BuyItem's GetSubDef<CHeroTitleDef> 0x005B1E5A): the sub-def info by class name, the def by global index,
// the dynamic-cast assignment. Out of line in retail (called `lea eax,<doorDef> ; push eax ; call`).
#pragma auto_inline(off)
template <> bool CDefClassBase::GetSubDef<CDoorDef>(CDefPointer<const CDoorDef>* out) const
{
    const CSubDefInfo* info = GetPSubDefInfo(CCharString("CDoorDef"));
    if (info) {
        CDefPointer<const CDefClassBase> def = PDefManager->GetPDefFromGlobalIndex(info->DefIndex);
        out->SetDynamicCast(def);
        return true;
    }
    return false;
}
#pragma auto_inline(on)

void NPlayerGui::CDrawHighlightingTargetingCursor::Draw(const CEnginePrimitivePrepareInfo& info)
{
    const CThingPlayerCreature* hero = PlayerManager->GetMainPlayer().PeekPControlledCreature();
    const CTCTargetingBase* targeting;
    hero->PeekTC(TCI_TARGETING, &targeting);
    const CPlayerGuiDef& guiDef = CPlayerGui::PeekGuiDef();
    // A new target: the old one loses its mesh glow (if it still exists and has a mesh) and the fade starts over.
    if (targeting->GetPTargetedThing() != PTargetedThing) {
        if (IS_THING_ALIVE(PTargetedThing) && PTargetedThing->GetTCGraphicAppearance() != 0)
            PTargetedThing->GetTCGraphicAppearance()->ClearMeshEffect(this);
        PTargetedThing = targeting->GetPTargetedThing();
        CursorType = HIGHLIGHTING_CURSOR_NONE;
    }
    if (PTargetedThing.Get() != 0) {
        CDefPointer<const CDoorDef> doorDef;
        EHighlightingCursorType cursorType = HIGHLIGHTING_CURSOR_NONE;
        Drawing = true;
        // Which cursor: an action-use target in range is "use"; a thing the hero can hit, or a smashable that is not a door, is
        // "attack"; anything else is "talk", raised to "interest" for a villager with information, an available shopkeeper, a
        // tavern game master with a game, or a wife (or would-be wife) with something to offer.
        if (targeting->IsTargetedThingOfType(TARGETING_TYPE_ACTION_USE)) {
            if (targeting->IsTargetedThingInRange(1))
                cursorType = HIGHLIGHTING_CURSOR_USE;
        } else if (NTargeting::IsThingVulnerableToMeleeStrikesFromThing(*PTargetedThing.Get(), *hero)) {
            cursorType = HIGHLIGHTING_CURSOR_ATTACK;
        } else if (!PTargetedThing->GetPBaseDef()->GetSubDef(&doorDef) && PTargetedThing->HasTC(TCI_SMASHABLE)) {
            cursorType = HIGHLIGHTING_CURSOR_ATTACK;
        } else {
            cursorType = HIGHLIGHTING_CURSOR_TALK;
            if (PTargetedThing->GetType() == 1) {
                CThingAICreature* creature = (CThingAICreature*)PTargetedThing.Get();
                CTCShopKeeper* shopKeeper;
                CTCTavernGameMaster* gameMaster;
                const CTCWife* wife;
                if (creature->GetHasInformation()
                    || (creature->GetTC(TCI_SHOP_KEEPER, &shopKeeper) && shopKeeper->GetShopTC() != 0
                        && shopKeeper->GetShopTC()->IsShopKeeperAvailable())
                    || (creature->GetTC(TCI_TAVERN_GAME_MASTER, &gameMaster) && gameMaster->GetTavernGame() != 0)
                    || (creature->PeekTC(TCI_WIFE, &wife)
                        && ((!wife->IsMarriedToThePlayer() && wife->IsInLoveWithHero())
                            || (wife->IsMarriedToThePlayer() && wife->IsOfferingSex()))))
                    cursorType = HIGHLIGHTING_CURSOR_INTEREST;
            }
        }
        if (IS_THING_ALIVE(PTargetedThing)) {
            // The hero guide is "use" only while the map table shows quest cards; the demon-door faces are always "use"; the
            // battle-charge victim the hero Z-targets is "attack".
            if (PTargetedThing->HasTC(TCI_HERO_GUIDE)) {
                if (World->MapTableShowQuestCardsOnUsed)
                    cursorType = HIGHLIGHTING_CURSOR_USE;
                else
                    cursorType = HIGHLIGHTING_CURSOR_NONE;
            } else if (PTargetedThing->GetDefName() == "CREATURE_DEMON_DOOR_FACE_01") {
                cursorType = HIGHLIGHTING_CURSOR_USE;
            } else if (PTargetedThing->GetDefName() == "CREATURE_NW_DEMON_DOOR_FACE_01") {
                cursorType = HIGHLIGHTING_CURSOR_USE;
            } else {
                const CThingPlayerCreature* player = PlayerManager->GetMainPlayer().PeekPControlledCreature();
                const CTCBattleCharge* battleCharge;
                if (player->PeekTC(TCI_BATTLE_CHARGE, &battleCharge) && battleCharge->GetIsZTargetting()
                    && PTargetedThing.Get() == battleCharge->GetPTarget().Get())
                    cursorType = HIGHLIGHTING_CURSOR_ATTACK;
            }
        }
        // An action-use thing that cannot be used right now gets no cursor.
        if (cursorType == HIGHLIGHTING_CURSOR_USE) {
            const CTCActionUseBase* actionUse;
            const CTCActionUseScriptedHook* scriptedHook;
            if (PTargetedThing->PeekTC(TCI_ON_ACTION_USE, &actionUse)) {
                if (!actionUse->IsUsable())
                    cursorType = HIGHLIGHTING_CURSOR_NONE;
            } else if (PTargetedThing->PeekTC(TCI_ACTION_USE_SCRIPTED_HOOK, &scriptedHook)) {
                if (!scriptedHook->IsUsable())
                    cursorType = HIGHLIGHTING_CURSOR_NONE;
            }
        }
        // A changed cursor restarts the glow from the def's start values.
        if (cursorType != CursorType) {
            CursorType = cursorType;
            CurrentColour = guiDef.HighlightStartColour[cursorType];
            CurrentWidthWorldSpace = guiDef.HighlightStartWidthWorldSpace[cursorType];
            CurrentWidthScreenSpace = guiDef.HighlightStartWidthScreenSpace[cursorType];
            CurrentFadeStep = 0;
        }
        // Fade one step towards the end values and apply the glow (unless the target asks for no glow).
        if (CurrentFadeStep < guiDef.HighlightFadeSteps[CursorType]) {
            CurrentWidthWorldSpace += (guiDef.HighlightEndWidthWorldSpace[CursorType] - guiDef.HighlightStartWidthWorldSpace[CursorType])
                                      / (float)guiDef.HighlightFadeSteps[CursorType];
            CurrentWidthScreenSpace += (guiDef.HighlightEndWidthScreenSpace[CursorType] - guiDef.HighlightStartWidthScreenSpace[CursorType])
                                       / (float)guiDef.HighlightFadeSteps[CursorType];
            CRGBColour startColour = guiDef.HighlightStartColour[CursorType];
            CRGBColour endColour = guiDef.HighlightEndColour[CursorType];
            long fadeSteps = guiDef.HighlightFadeSteps[CursorType];
            CurrentColour.R += (unsigned char)((endColour.R - startColour.R) / fadeSteps);
            CurrentColour.G += (unsigned char)((endColour.G - startColour.G) / fadeSteps);
            CurrentColour.B += (unsigned char)((endColour.B - startColour.B) / fadeSteps);
            CurrentColour.A += (unsigned char)((endColour.A - startColour.A) / fadeSteps);
            CTCGraphicAppearance* appearance;
            if (PTargetedThing->GetTC(TCI_GRAPHIC_APPEARANCE_NEW, &appearance)) {
                const CTCTargeted* targeted;
                if (!PTargetedThing->PeekTC(TCI_TARGETED, &targeted) || targeted->IsToUseTargetGlow())
                    appearance->SetAsHighlighted(CurrentWidthWorldSpace, CurrentWidthScreenSpace, CurrentColour, true, true,
                                                 APPEARANCE_MESH_EFFECT_PRIORITY_HIGHLIGHT, this);
            }
            ++CurrentFadeStep;
        }
        // The targeting border: only with the profile's targeting status on, an attack cursor, and the hero Z-targeting (or
        // charging lightning with Z-target held).
        CUserProfileManager& profile = CUserProfileManager::Get();
        bool showTargetingStatus;
        if (profile.UseNewProfileValues)
            showTargetingStatus = profile.ShowTargetingStatusNewProfile;
        else
            showTargetingStatus = profile.ShowTargetingStatus;
        if (showTargetingStatus && CursorType == HIGHLIGHTING_CURSOR_ATTACK
            && (PlayerManager->GetMainPlayer().GetCurrentMode() == PLAYER_MODE_Z_TARGET
                || (PlayerManager->GetMainPlayer().GetCurrentMode() == PLAYER_MODE_LIGHTNING
                    && PlayerManager->GetMainPlayer().HasMode(PLAYER_MODE_Z_TARGET)))) {
            const CPlayerGuiDef& def = CPlayerGui::PeekGuiDef();
            float width = (float)(Bank->GetGraphicMaxWidth(def.TargetingBorderLeftGraphic) * GPDisplayManager->GetRenderTargetDimensions().X)
                          * (1.0f / 640.0f);
            float height = (float)(GPDisplayManager->GetRenderTargetDimensions().Y * Bank->GetGraphicMaxHeight(def.TargetingBorderLeftGraphic))
                           * (1.0f / 480.0f);
            C2DBoxF box;
            box.TLX = def.TargetingBorderLeftPos.X;
            box.BRX = box.TLX + width;
            box.TLY = ((float)GPDisplayManager->GetRenderTargetDimensions().Y - height) * 0.5f;
            box.BRY = box.TLY + height;
            CEnginePrimitive2DSprite left(0, box, 0x43, def.TargetingBorderLeftGraphic, 0, 2, 0, 0.0f, 0.0f, ScaleAlpha(0xff),
                                          0xff, 0xff, 0xff, 2, C2DVector(0.0f, 0.0f), C2DVector(0.0f, 0.0f));
            info.Engine->SetPersistentPrimitive(PrimitiveHandles[0], left, 0xc0);
            C2DVector rightPos = NPlayerGui::GFAnchorToTopRight(def.TargetingBorderRightPos);   // computed, then not used: the right border mirrors the left one
            box.BRX = (float)GPDisplayManager->GetRenderTargetDimensions().X - def.TargetingBorderLeftPos.X;
            box.TLX = box.BRX - width;
            box.TLY = ((float)GPDisplayManager->GetRenderTargetDimensions().Y - height) * 0.5f;
            box.BRY = box.TLY + height;
            CEnginePrimitive2DSprite right(0, box, 0x43, def.TargetingBorderRightGraphic, 0, 2, 0, 0.0f, 0.0f, ScaleAlpha(0xff),
                                           0xff, 0xff, 0xff, 2, C2DVector(0.0f, 0.0f), C2DVector(0.0f, 0.0f));
            info.Engine->SetPersistentPrimitive(PrimitiveHandles[1], right, 0xc0);
        } else {
            std::for_each(PrimitiveHandles.begin(), PrimitiveHandles.end(), std::mem_fun_ref(&CEnginePrimitiveHandle::Clear));
        }
    }
}
