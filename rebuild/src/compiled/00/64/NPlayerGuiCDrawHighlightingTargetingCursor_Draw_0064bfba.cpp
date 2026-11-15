// NPlayerGui::CDrawHighlightingTargetingCursor::Draw -- update target glow and draw targeting borders.
// Inline-budget: native expands both fade-colour copies; S50 expands native-called alive checks, scoped depth16 leaves both copies called.
// Original CRGBColour user copies and four-channel construction delegate to Initialise; existing accepted body flags only.
// Original STLport mem_fun_ref_t empty base, typedef, member and formals restored.
// Original both-PDB attribute0x3 (compgenx clear): retain the user copy constructor of CIntelligentPointer<CThing>.
// Flags: /Gr
// Calling convention: native free for_each/anchor boundaries and both2013 donor TU commands use fastcall.
// Leaf: ?Draw@CDrawHighlightingTargetingCursor@NPlayerGui@@QAEXABVCEnginePrimitivePrepareInfo@@@Z
// Retail: 0x0064BFBA,2223bytes,thiscall ret4.
#define FABLE_RETAIL_NPLAYERGUI_CURSOR_ORIGINAL_DECLARATIONS_PASS22
#define FABLE_RETAIL_C2DEXTENTSI_ORIGINAL_CTOR_BODIES_PASS22
#define FABLE_RETAIL_C2DEXTENTSI_INITIALISE
#define FABLE_RETAIL_C2DVECTOR
#define FABLE_RETAIL_CRGBCOLOUR
#define FABLE_RETAIL_CRGBCOLOUR_CURSOR_COPY_INLINE_PASS22
#define FABLE_RETAIL_CRGBCOLOUR_CTOR4_INITIALISE
#define FABLE_RETAIL_STLP_MEM_FUN_REF_ORIGINAL_PASS22
#define FABLE_RETAIL_CENGINEPRIMITIVE2DSPRITE_HIGHLIGHT_BODY
#define FABLE_RETAIL_CENGINEPRIMITIVE_ID_CTOR
#define FABLE_RETAIL_C2DBOXF
#define FABLE_RETAIL_C2DBOXF_COPY
#define FABLE_RETAIL_NPLAYERGUI_HIGHLIGHT_ORIGINAL_NAMES
#define FABLE_RETAIL_CTCBATTLECHARGE_HIGHLIGHT_NAMES
#define FABLE_RETAIL_CTCTAVERNGAMEMASTER_GETGAME
#define FABLE_RETAIL_C2DBOXF_SETTLPOS
#define FABLE_RETAIL_CENGINEPRIMITIVE2DSPRITE_ENUM_CTORS
#define FABLE_RETAIL_CENGINEPRIMITIVEPREPAREINFO
#define FABLE_RETAIL_NPLAYERGUI_ANCHORS
#define FABLE_RETAIL_CCHARSTRING
#define FABLE_RETAIL_CWIDESTRING
#define FABLE_RETAIL_CDEFSTRINGTABLE
#define FABLE_RETAIL_CCLASS
#define FABLE_RETAIL_CTCBASE
#define FABLE_RETAIL_DEF_CLASS_BASE
#define FABLE_RETAIL_CTHING
#define FABLE_RETAIL_CTHING_GETTC_ORIGINAL_CHAIN
#define FABLE_RETAIL_CTHING_PEEKTC_ORIGINAL_CHAIN
#define FABLE_RETAIL_CTHING_ADDANDCOPYTC
#define FABLE_RETAIL_CVECTORMAP_ORIGINAL_FIND
#define FABLE_RETAIL_CVECTORMAP_FORCEINLINE_FIND
#define FABLE_RETAIL_STLP_BITSET_TEST_DECL
#define FABLE_RETAIL_CINTELLIGENTPOINTER
#define FABLE_RETAIL_CINTELLIGENTPOINTER_COMPARE_NULL_INT
#define FABLE_RETAIL_IS_THING_ALIVE
#define FABLE_RETAIL_CINTELLIGENTPOINTER_COPY
#define FABLE_RETAIL_CINTELLIGENTPOINTER_COPY_ORIGINAL_CHAIN
#define FABLE_RETAIL_CINTELLIGENTPOINTER_ORIGINAL_SETPTR_PASS22
#define FABLE_RETAIL_CINTELLIGENTPOINTER_CTOR_OUTOFLINE
#define FABLE_RETAIL_CINTELLIGENTPOINTER_ASSIGN_OUTOFLINE
#define FABLE_RETAIL_CINTELLIGENTPOINTER_COMPARE
#define FABLE_RETAIL_CINTELLIGENTPOINTER_DEREFERENCE
#define FABLE_RETAIL_CINTELLIGENTPOINTER_GET_ORIGINAL_CHAIN
#define FABLE_RETAIL_CINTELLIGENTPOINTER_EQUAL_RAW
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
#define FABLE_RETAIL_CUSERPROFILEMANAGER_ORIGINAL_TARGETING_QUERY
#define FABLE_RETAIL_CUSERPROFILEMANAGER_TARGETINGSTATUS
#define FABLE_RETAIL_CDISPLAYMANAGER
#define FABLE_RETAIL_CIENGINE
#define FABLE_RETAIL_CENGINEPRIMITIVEHANDLE
#define FABLE_RETAIL_CIVCOUNTEDPOINTER
#define FABLE_RETAIL_CENGINEPERSISTENTPRIMITIVEBASE
#define FABLE_RETAIL_CGRAPHICDATABANK_ORIGINAL_METHODS_PASS22
#define FABLE_RETAIL_STATICBLOCKS_TEXTURE_HELPERS
#define FABLE_RETAIL_CGRAPHICDATABANK_GRAPHICSIZE
#define FABLE_RETAIL_NPLAYERGUI_CDRAWBASE_ORIGINAL_REFERENCES
#define FABLE_RETAIL_NPLAYERGUI_CDRAWBASE_HANDLES
#define FABLE_RETAIL_NPLAYERGUI_CDRAWBASE_SETASDRAWING
#define FABLE_RETAIL_CTCTARGETINGBASE_OFTYPE
#define FABLE_RETAIL_CTCGRAPHICAPPEARANCE_HIGHLIGHT
#define FABLE_RETAIL_CTCGRAPHICAPPEARANCE_ORIGINAL_CURSOR_HELPERS
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
#define FABLE_RETAIL_CWORLD_MAP_TABLE_QUERY
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

extern CDisplayManager* GPDisplayManager;
#include "engine/ThingAlive.h"
#include "engine/NPlayerGui_Graphics.h"

#include "engine/ETargetingType.h"
namespace NGraphicAppearance { enum EAppearanceMeshEffectPriority { MESH_EFFECT_PRIORITY_NULL=0, MESH_EFFECT_PRIORITY_WEAPON_GLOWS=1, MESH_EFFECT_PRIORITY_SPELLS=2, MESH_EFFECT_PRIORITY_TARGETING=3, MESH_EFFECT_PRIORITY_SHIELD_SPELL_SPECIAL_OVERRIDE=4 }; }

// Original pointer-iterator LowerBound overloads: native0040F020 and004365B0.
template<> CVectorMap<ETCInterfaceType,CTCBase*,CKeyPairCompareLess<ETCInterfaceType,CTCBase*> >::iterator CVectorMap<ETCInterfaceType,CTCBase*,CKeyPairCompareLess<ETCInterfaceType,CTCBase*> >::LowerBound(const ETCInterfaceType& key)
{
    iterator first=this->begin();
    long count=this->end()-first;
    while(count>0) {
        long half=count/2;
        iterator middle=first+half;
        if(middle->first<key) { first=middle+1; count-=half+1; }
        else count=half;
    }
    return first;
}
template<> CVectorMap<ETCInterfaceType,CTCBase*,CKeyPairCompareLess<ETCInterfaceType,CTCBase*> >::const_iterator CVectorMap<ETCInterfaceType,CTCBase*,CKeyPairCompareLess<ETCInterfaceType,CTCBase*> >::LowerBound(const ETCInterfaceType& key) const
{
    const_iterator first=this->begin();
    long count=this->end()-first;
    while(count>0) {
        long half=count/2;
        const_iterator middle=first+half;
        if(middle->first<key) { first=middle+1; count-=half+1; }
        else count=half;
    }
    return first;
}


// Original cursor is size optimized; shared math and sprite bodies use the native speed shape.
#ifdef _MSC_VER
#pragma optimize("s", on)
#endif

#ifdef _MSC_VER
#pragma auto_inline(off)
#endif
// Both original PDBs and donor020D91D2..020D921A; native00643CA1..00643CDA.
C2DVector FABLE_FASTCALL NPlayerGui::GFAnchorToTopRight(const C2DVector& pos)
{
    return C2DVector(GPDisplayManager->GetRenderTargetDimensions().X + pos.X - 640.0f, pos.Y);
}

template <> bool CDefClassBase::GetSubDef<CDoorDef>(CDefPointer<const CDoorDef>* out) const
{
    const CSubDefInfo* info = GetPSubDefInfo("CDoorDef");
    if (info) {
        CDefPointer<const CDefClassBase> def = PDefManager->GetPDefFromGlobalIndex(info->DefIndex);
        out->SetDynamicCast(def);
        return true;
    }
    return false;
}

#ifdef _MSC_VER
#pragma auto_inline(on)
#endif

void NPlayerGui::CDrawHighlightingTargetingCursor::Draw(const CEnginePrimitivePrepareInfo& info)
{
    const CThingPlayerCreature* pcreature = GetPlayer().PeekPControlledCreature();
    const CTCTargetingBase* ptc_targeting;
    pcreature->PeekTC(&ptc_targeting);
    const CPlayerGuiDef& player_gui_def = CPlayerGui::PeekGuiDef();
    if (ptc_targeting->GetPTargetedThing() != PTargetedThing) {
        if (IS_THING_ALIVE(PTargetedThing) && PTargetedThing->GetTCGraphicAppearance() != 0)
            PTargetedThing->GetTCGraphicAppearance()->ClearHighlighted(this);
        PTargetedThing = ptc_targeting->GetPTargetedThing();
        CursorType = CURSOR_TYPE_NONE;
    }
    if (PTargetedThing.Get() != 0) {
        SetAsDrawing(true);
        CDefPointer<const CDoorDef> pdoor_def;
        EHighlightingCursorType cursor_type = CURSOR_TYPE_NONE;
        if (ptc_targeting->IsTargetedThingOfType(TARGETING_USABLE)) {
            if (ptc_targeting->IsTargetedThingInRange(1))
                cursor_type = CURSOR_TYPE_USABLE;
        } else if (NTargeting::IsThingVulnerableToMeleeStrikesFromThing(*PTargetedThing, *pcreature)) {
            cursor_type = CURSOR_TYPE_ENEMY;
        } else if (!PTargetedThing->GetPBaseDef()->GetSubDef(&pdoor_def) && PTargetedThing->HasTC(TCI_SMASHABLE)) {
            cursor_type = CURSOR_TYPE_ENEMY;
        } else {
            cursor_type = CURSOR_TYPE_ALLY;
            if (PTargetedThing->GetType() == 1) {
                CThingAICreature* pai_creature = (CThingAICreature*)PTargetedThing.Get();
                CTCShopKeeper* tc_shopkeeper;
                CTCTavernGameMaster* tc_taverngamemaster;
                const CTCWife* ptc_wife;
                if (pai_creature->GetHasInformation()
                    || (pai_creature->GetTC(&tc_shopkeeper) && tc_shopkeeper->GetShopTC() != 0
                        && tc_shopkeeper->GetShopTC()->IsShopKeeperAvailable())
                    || (pai_creature->GetTC(&tc_taverngamemaster) && tc_taverngamemaster->GetGame() != 0)
                    || (pai_creature->PeekTC(&ptc_wife)
                        && ((!ptc_wife->IsMarriedToThePlayer() && ptc_wife->IsInLoveWithHero())
                            || (ptc_wife->IsMarriedToThePlayer() && ptc_wife->IsOfferingSex()))))
                    cursor_type = CURSOR_TYPE_HAS_INFORMATION;
            }
        }
        if (IS_THING_ALIVE(PTargetedThing)) {
            const CTCBattleCharge* ptc_battlecharge;
            if (PTargetedThing->HasTC(TCI_HERO_GUIDE)) {
                if (World.IsMapTableToShowQuestCardsWhenUsed())
                    cursor_type = CURSOR_TYPE_USABLE;
                else
                    cursor_type = CURSOR_TYPE_NONE;
            } else if (PTargetedThing->GetDefName() == "CREATURE_DEMON_DOOR_FACE_01") {
                cursor_type = CURSOR_TYPE_USABLE;
            } else if (PTargetedThing->GetDefName() == "CREATURE_NW_DEMON_DOOR_FACE_01") {
                cursor_type = CURSOR_TYPE_USABLE;
            } else {
                if (GetPlayer().PeekPControlledCreature()->PeekTC(&ptc_battlecharge) && ptc_battlecharge->IsBattleChargeZTargetting()
                    && PTargetedThing == ptc_battlecharge->GetPTarget())
                    cursor_type = CURSOR_TYPE_ENEMY;
            }
        }
        if (cursor_type == CURSOR_TYPE_USABLE) {
            const CTCActionUseBase* ptc_action_use;
            if (PTargetedThing->PeekTC(&ptc_action_use)) {
                if (!ptc_action_use->IsUsable())
                    cursor_type = CURSOR_TYPE_NONE;
            } else {
                const CTCActionUseScriptedHook* ptc_action_use_hook = 0;
                if (PTargetedThing->PeekTC(&ptc_action_use_hook)) {
                    if (!ptc_action_use_hook->IsUsable())
                        cursor_type = CURSOR_TYPE_NONE;
                }
            }
        }
        if (cursor_type != CursorType) {
            CursorType = cursor_type;
            CurrentColour = player_gui_def.HighlightStartColour[cursor_type];
            CurrentWidthWorldSpace = player_gui_def.HighlightStartWidthWorldSpace[cursor_type];
            CurrentWidthScreenSpace = player_gui_def.HighlightStartWidthScreenSpace[cursor_type];
            CurrentFadeStep = 0;
        }
        if (CurrentFadeStep < player_gui_def.HighlightFadeSteps[CursorType]) {
            CurrentWidthWorldSpace += (player_gui_def.HighlightEndWidthWorldSpace[CursorType] - player_gui_def.HighlightStartWidthWorldSpace[CursorType])
                                      / (float)player_gui_def.HighlightFadeSteps[CursorType];
            CurrentWidthScreenSpace += (player_gui_def.HighlightEndWidthScreenSpace[CursorType] - player_gui_def.HighlightStartWidthScreenSpace[CursorType])
                                       / (float)player_gui_def.HighlightFadeSteps[CursorType];
            CRGBColour start_col = player_gui_def.HighlightStartColour[CursorType];
            CRGBColour end_col = player_gui_def.HighlightEndColour[CursorType];
            long fade_steps = player_gui_def.HighlightFadeSteps[CursorType];
            CurrentColour.R += (unsigned char)((end_col.R - start_col.R) / fade_steps);
            CurrentColour.G += (unsigned char)((end_col.G - start_col.G) / fade_steps);
            CurrentColour.B += (unsigned char)((end_col.B - start_col.B) / fade_steps);
            CurrentColour.A += (unsigned char)((end_col.A - start_col.A) / fade_steps);
            CTCGraphicAppearance* ptc_appearance;
            if (PTargetedThing->GetTC(&ptc_appearance)) {
                const CTCTargeted* ptc_targeted;
                if (!PTargetedThing->PeekTC(&ptc_targeted) || ptc_targeted->IsToUseTargetGlow())
                    ptc_appearance->SetAsHighlighted(CurrentWidthWorldSpace, CurrentWidthScreenSpace, CurrentColour, true, true,
                                                 MESH_EFFECT_PRIORITY_TARGETING, this);
            }
            ++CurrentFadeStep;
        }
        bool added_primitives = false;
        if (CUserProfileManager::Get().GetShowTargetingStatus() && CursorType == CURSOR_TYPE_ENEMY
            && (GetPlayer().GetCurrentMode() == PLAYER_MODE_Z_TARGET
                || (GetPlayer().GetCurrentMode() == PLAYER_MODE_LIGHTNING
                    && GetPlayer().HasMode(PLAYER_MODE_Z_TARGET)))) {
            const CPlayerGuiDef& gui_def = CPlayerGui::PeekGuiDef();
            // border_graphic (descriptive name; no PDB local survives for it): the left border's graphic index named once by reference for the
            // two bank queries. With the member spelled directly in the call, VC7.1 evaluates GetGraphicMaxHeight before
            // GetRenderTargetDimensions for the `.Y` product; through the reference it makes the dimensions call first and keeps the
            // returned pointer (retail 0x0064C69F call, 0x0064C6AE `mov [ebp+70h],eax`, 0x0064C6B7 `imul eax,[ecx+4]`), as for `.X`.
            const long& border_graphic = gui_def.TargetingBorderLeftGraphic;
            float graphic_width = (float)(Bank.GetGraphicMaxWidth(border_graphic) * GPDisplayManager->GetRenderTargetDimensions().X) / 640.0f;
            float graphic_height = (float)(Bank.GetGraphicMaxHeight(border_graphic) * GPDisplayManager->GetRenderTargetDimensions().Y) / 480.0f;
            C2DBoxF indicator_box;
            indicator_box.SetTLPos(NPlayerGui::GFAnchorToTopLeft(gui_def.TargetingBorderLeftPos));
            indicator_box.BRX = indicator_box.TLX + graphic_width;
            indicator_box.TLY = ((float)GPDisplayManager->GetRenderTargetDimensions().Y - graphic_height) * 0.5f;
            indicator_box.BRY = indicator_box.TLY + graphic_height;
            CEnginePrimitive2DSprite left_sprite(0, indicator_box, ENGINE_2D_LAYER_0, gui_def.TargetingBorderLeftGraphic, 0, ENGINE_PRIMITIVE_BLEND_ALPHA, (EEngineInterpolationSet)0, 0.0f, 0.0f, ScaleAlpha(0xff),
                                          0xff, 0xff, 0xff, 2);
            info.Engine.SetPersistentPrimitive(PrimitiveHandles[0], left_sprite, 0xc0);
            indicator_box.SetTLPos(NPlayerGui::GFAnchorToTopRight(gui_def.TargetingBorderRightPos));
            indicator_box.BRX = (float)GPDisplayManager->GetRenderTargetDimensions().X - gui_def.TargetingBorderLeftPos.X;
            indicator_box.TLX = indicator_box.BRX - graphic_width;
            indicator_box.TLY = ((float)GPDisplayManager->GetRenderTargetDimensions().Y - graphic_height) * 0.5f;
            indicator_box.BRY = indicator_box.TLY + graphic_height;
            CEnginePrimitive2DSprite right_sprite(0, indicator_box, ENGINE_2D_LAYER_0, gui_def.TargetingBorderRightGraphic, 0, ENGINE_PRIMITIVE_BLEND_ALPHA, (EEngineInterpolationSet)0, 0.0f, 0.0f, ScaleAlpha(0xff),
                                           0xff, 0xff, 0xff, 2);
            info.Engine.SetPersistentPrimitive(PrimitiveHandles[1], right_sprite, 0xc0);
            added_primitives = true;
        }
        if (!added_primitives) {
            std::for_each(PrimitiveHandles.begin(), PrimitiveHandles.end(), std::mem_fun_ref(&CEnginePrimitiveHandle::Clear));
        }
    }
}