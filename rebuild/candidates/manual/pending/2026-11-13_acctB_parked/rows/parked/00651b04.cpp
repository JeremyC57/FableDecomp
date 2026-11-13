// NPlayerGui::CDrawAbilityButtons::Draw(const CEnginePrimitivePrepareInfo& info) -- draw the hero's ABXY ability HUD: clear and resize the
// Grade: DIFFER(5126v5126) -- 26 hunks: x87 sum X/Y order (retail X-first direct stores) at the uncastable / special / switch DrawButton positions, middle-ring sprite position (uv zero stores before the GFAnchor call, X-first + register copy), special-loop position copy pattern (ours), loop-1 `cmp [eax+ecx+3Ch]` scheduled after the drawPos copy stores
// Leaf: ?Draw@CDrawAbilityButtons@NPlayerGui@@UAEXABVCEnginePrimitivePrepareInfo@@@Z
//   primitive handle tables, draw the (favourite) ring and top-ring sprites, the white / black button weapon icons with their HUD
//   tooltips (sheathe graphics by weapon type, OBJECT_HERO_STICK), the normal ABXY buttons with tooltips and the interact border, or
//   the special-ability buttons (uncastable overlay, shield-off graphic, effects) with the ability / expression list, the middle ring,
//   and advance the normal<->special button scale.
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00651B04, 5126 bytes, `ret 4`, biased frame (`push ebp ; lea ebp,[esp-74h] ; sub esp,0E0h`).
// Flags: none (size TU: `#pragma optimize("s", on)` only, as CDrawPerceivers::Draw 0x0064499C).
// Callees: CPlayerManager::GetMainPlayer 0x00449970, CPlayer::PeekPControlledCreature 0x00487DD0, IS_THING_ALIVE 0x0043DBF0 / 0x0040DCE0,
//   CPlayerGui::PeekGuiDef 0x00438570, vector<CEnginePrimitiveHandle>::erase 0x00440D70 / resize 0x00443F90, bitset<274>::test 0x00410DE0,
//   CVectorMap::LowerBound 0x004365B0, NPlayerGui::GFAnchorToBottomRight 0x00643D0C (real body, same source file), CDrawBase::ScaleAlpha
//   0x00643D5D, CEnginePrimitive2DSprite ctor 0x00493AC2, CIEngine::SetPersistentPrimitive (vslot 23), CTCQuickAccessMenu::IsActive
//   0x00625930, CBaseIntelligentPointer::GetPItem 0x00A01B50, CTCCarrying::GetThingInPrimarySlot 0x006A1440, CTCWeapon::GetWeaponType
//   0x005DA7D0, CThing::GetDefName 0x004C7CC0, CDefString::operator== 0x005B3440, CTCInventoryItem::GetEngineGraphic 0x005D92D0,
//   CUserProfileManager::Get 0x0040D2A0 / GetAssignedInputForAction 0x00408C90, CWideString ctor 0x0099AED0 / dtor 0x0099B510,
//   CGamePlayerInterface::GetActionName 0x00446720, GFACoordToVCoord 0x009E21D0, CFontBank::GetVStringExtents 0x00437210,
//   CPlayerGui::Draw2DText 0x00438010, UpdateWeaponAbilities 0x0064B1AC, CPlayerGui::GetButtonAlpha 0x00439AB0 / GetAbilityButton
//   0x00438990 / GetAbilityButtonScale 0x004375E0, CInputEvent ctor 0x00A04410 / dtor 0x00A03B30, CPlayer::GetJoystickDeviceNumber
//   0x00487510, CInputManager::IsJoystickEventInQueue 0x009F4C60, NCreatureMode::IsAvailableForHeroRun 0x0085F560, DrawButton 0x00645099,
//   CTCInventoryAbilities 0x005EE31D / 0x005EE365 / 0x005EDF08 / 0x005EE49A / 0x005EE146 / 0x005EE183 / 0x005EE24B,
//   CTCSpecialAbilities::IsAbilityAvailable 0x0072C430, DrawEffects 0x006451DD, CDrawBase::ClearEffects 0x006445BA, __ftol2 0x00BFEA70,
//   virtual slots 3 / 4 / 7 (GetAssignableSpecialMoveIndexForButtonDef, DrawAbilityList, DrawExpressionList).
// Shape: `normal` set in both arms (`if (NTS || STN || !Show) normal = true; else { ...; if (menu->IsActive()) normal = true; }`) --
//   this spelling alone gave retail's whole frame layout; CEnginePrimitiveHandle::Clear expanded in place (forceinline header opt-ins);
//   the usable test as one `?:` condition with the not-able arm first.
// Made-up name: HUDRingAlpha (0x01375744, unnamed .data byte, found in no PDB / FSE / EgoCore list).

#pragma optimize("s", on)
#define FABLE_RETAIL_DEF_CLASS_BASE
#define FABLE_RETAIL_C2DVECTOR
#define FABLE_RETAIL_CRIGHTHANDEDSET
#define FABLE_RETAIL_CRIGHTHANDEDSET_CTOR
#define FABLE_RETAIL_CRIGHTHANDEDSET_RIGHT_COMPONENTS
#define FABLE_RETAIL_CINTELLIGENTPOINTER
#define FABLE_RETAIL_CINTELLIGENTPOINTER_CTOR_OUTOFLINE
#define FABLE_RETAIL_CINTELLIGENTPOINTER_ASSIGN_OUTOFLINE
#define FABLE_RETAIL_CENGINEPERSISTENTPRIMITIVEBASE
#define FABLE_RETAIL_CIVCOUNTEDPOINTER
#define FABLE_RETAIL_CENGINEPRIMITIVEHANDLE
#define FABLE_RETAIL_CENGINEPRIMITIVEHANDLE_CLEAR_FORCEINLINE
#define FABLE_RETAIL_CIVCOUNTEDPOINTER_RESET_ADOPT
#define FABLE_RETAIL_CIVCOUNTEDPOINTER_RESET_FORCEINLINE
#define FABLE_RETAIL_CIVCOUNTEDPOINTER_RELEASE_FORCEINLINE
#define FABLE_RETAIL_CENGINEPRIMITIVE2DSPRITE_ENUM_CTORS
#define FABLE_RETAIL_CENGINEPRIMITIVEPREPAREINFO
#define FABLE_RETAIL_CIENGINE
#define FABLE_RETAIL_CPARTICLEEMITTERMANAGER_CREATE2D
#define FABLE_RETAIL_SCOPED_PTR
#define FABLE_RETAIL_CRESOURCELIST
#define FABLE_RETAIL_CDISPLAYMANAGER
#define FABLE_RETAIL_NHEROINFORMATIONSCREENS_CMANAGER
#define FABLE_RETAIL_CPLAYERGUIDEF_FLOURISH
#define FABLE_RETAIL_CPLAYERGUIDEF_ABILITYBUTTONS
#define FABLE_RETAIL_CPLAYERGUIDEF_ABXY
#define FABLE_RETAIL_CPLAYERGUI
#define FABLE_RETAIL_CPLAYERGUI_MEMBERS
#define FABLE_RETAIL_CPLAYERGUI_PEEKGUIDEF
#define FABLE_RETAIL_CPLAYERGUI_ISSHOWSPECIALABILITYBUTTONS
#define FABLE_RETAIL_CPLAYERGUI_QUEST_DRAW
#define FABLE_RETAIL_CPLAYERGUI_DRAWTEXT
#define FABLE_RETAIL_CPLAYERGUI_ABILITYBUTTONS_DRAW
#define FABLE_RETAIL_NPLAYERGUI_CDRAWBASE_HANDLES
#define FABLE_RETAIL_NPLAYERGUI_CDRAWBASE_SETASDRAWING
#define FABLE_RETAIL_NPLAYERGUI_CDRAWBASE_CLEAREFFECTS
#define FABLE_RETAIL_CCLASS
#define FABLE_RETAIL_CTHING
#define FABLE_RETAIL_CTHINGPHYSICAL
#define FABLE_RETAIL_CTHINGGAMEOBJECT
#define FABLE_RETAIL_CTHING_STATEACCESS
#define FABLE_RETAIL_CTCHERO_TITLE
#define FABLE_RETAIL_CTCINVENTORYITEM
#define FABLE_RETAIL_CTHINGCREATUREBASE
#define FABLE_RETAIL_CTHINGPLAYERCREATURE
#define FABLE_RETAIL_CTHING_GETTC_EXPANDED
#define FABLE_RETAIL_IS_THING_ALIVE
#define FABLE_RETAIL_IS_THING_ALIVE_NOINLINE
#define FABLE_RETAIL_CTCBASE
#define FABLE_RETAIL_CTCINVENTORYABILITIES
#define FABLE_RETAIL_CTCINVENTORYABILITIES_ASSIGN
#define FABLE_RETAIL_CTCINVENTORYABILITIES_SPECIALBUTTONS
#define FABLE_RETAIL_CTCQUICKACCESSMENU_ISACTIVE
#define FABLE_RETAIL_CTCSPECIALABILITIES
#define FABLE_RETAIL_CTCSPECIALABILITIES_ISABILITYAVAILABLE
#define FABLE_RETAIL_CTCWEAPON
#define FABLE_RETAIL_CTCWEAPON_GETWEAPONTYPE
#define FABLE_RETAIL_CTCINVENTORYITEM_ENGINEGRAPHIC
#define FABLE_RETAIL_CUSERPROFILEMANAGER_HUDTOOLTIP
#define FABLE_RETAIL_CACTIONINPUTCONTROL_DEFAULT_CTOR
#define FABLE_RETAIL_CINPUTMANAGER_JOYSTICKQUEUE
#define FABLE_RETAIL_NCREATUREMODE_HERORUN
#define FABLE_RETAIL_CFONTBANK
#define FABLE_RETAIL_CFONTBANK_VSTRINGEXTENTS_OUTOFLINE
#define FABLE_RETAIL_PLAYER
#define FABLE_RETAIL_CGAMEPLAYERINTERFACE_GETACTIONNAME
#define FABLE_RETAIL_CWORLD_UPDATEINPUT
#define FABLE_RETAIL_CMAINGAMECOMPONENT
#define FABLE_RETAIL_CMAINGAMECOMPONENT_TEXTBANK
#define FABLE_RETAIL_CDEFSTRING_EQUALS
#define FABLE_RETAIL_CWIDESTRING
#define FABLE_RETAIL_CDEFSTRINGTABLE
#define FABLE_RETAIL_CCHARSTRING
#include "stlport/fable_containers.h"
#include "fable_string.h"
#include "engine/C2DVector.h"
#include "engine/C3DVector.h"
#include "engine/C2DExtentsF.h"
#include "engine/CRGBColour.h"
#include "engine/CRightHandedSet.h"
#include "engine/CIntelligentPointer.h"
#include "engine/CEnginePrimitive2DSprite.h"
#include "engine/CEnginePrimitiveHandle.h"
#include "engine/CEnginePrimitivePrepareInfo.h"
#include "engine/CIEngine.h"
#include "engine/CParticleEmitterManager.h"
#include "engine/Direct3D9Types.h"
#include "engine/CDisplayManager.h"
#include "engine/CPlayerGuiDef.h"
#include "engine/CPlayerGui.h"
#include "engine/NPlayerGui_CDrawBase.h"
#include "engine/ETCInterfaceType.h"
#include "engine/EHeroAbility.h"
#include "engine/CThing.h"
#include "engine/CThingPlayerCreature.h"
#include "engine/ThingAlive.h"
#include "engine/CPlayer.h"
#include "engine/CPlayerManager.h"
#include "engine/CDefPointer.h"
#include "engine/CHeroAbilityDef.h"
#include "engine/CDefString.h"
#include "engine/CEngineGraphic.h"
#include "engine/CTCInventoryAbilities.h"
#include "engine/CTCQuickAccessMenu.h"
#include "engine/CTCInventoryWeapons.h"
#include "engine/CTCCarrying.h"
#include "engine/CTCHero.h"
#include "engine/CTCInventoryItem.h"
#include "engine/CTCWeapon.h"
#include "engine/CTCSpecialAbilities.h"
#include "engine/CTCHeroSpecialMovement.h"
#include "engine/CActionInputControl.h"
#include "engine/CUserProfileManager.h"
#include "engine/CInputEvent.h"
#include "engine/CInputManager.h"
#include "engine/CFontBank.h"
#include "engine/CWorld.h"
#include "engine/CMainGameComponent.h"
#include "engine/CGamePlayerInterface.h"
#include "engine/NCreatureMode.h"
extern unsigned char HUDRingAlpha;                                    // 0x01375744 (unnamed .data byte, 0xFF; descriptive name)
void __fastcall GFACoordToVCoord(const C2DVector& in, C2DVector& out);   // 0x009E21D0
using NParticleEngine::CParticleEmitter;
extern CDisplayManager* GPDisplayManager;                             // 0x013B8390 ?GPDisplayManager@@3PAVCDisplayManager@@A
namespace NPlayerGui {
#pragma auto_inline(off)
C2DVector __fastcall GFAnchorToBottomRight(const C2DVector& pos)     // 0x00643D0C (landed; defined earlier in this source file)
{
    return C2DVector((float)GPDisplayManager->GetRenderTargetDimensions().X + pos.X - 640.0f,
                     (float)GPDisplayManager->GetRenderTargetDimensions().Y + pos.Y - 480.0f);
}
#pragma auto_inline(on)

class CDrawAbilityButtons : public CDrawBase {
public:
    virtual void Draw(const CEnginePrimitivePrepareInfo& info);                                         // slot 2
protected:
    virtual long GetAssignableSpecialMoveIndexForButtonDef(const CAbilityButtonGuiDef& button_def) const;   // slot 3 0x00643FC8
    virtual void DrawAbilityList(const CEnginePrimitivePrepareInfo& info, float alpha);                 // slot 4 0x0064DB2B
    virtual void GetAbilityButtonMapping(std::vector<long>& mapping);                                   // slot 5 0x0064D593
    virtual void GetAbilityPositionMapping(std::vector<long>& mapping);                                 // slot 6 0x0064D5E1
    virtual void DrawExpressionList(const CEnginePrimitivePrepareInfo& info, float alpha);              // slot 7 0x006571B4
    void DrawButton(const CEnginePrimitivePrepareInfo& info, const C2DVector& position, float alpha, long index, long graphic,
                    bool special, EEngine2DLayer layer);                                               // 0x00645099
    void DrawEffects(long index, const C2DVector& position, const CHeroAbilityDef& ability_def);        // 0x006451DD
    void UpdateWeaponAbilities();                                                                       // 0x0064B1AC
    long                   Columns;                        // +0x28
    CEnginePrimitiveHandle BackgroundPrimitiveHandle;      // +0x2c
    CEnginePrimitiveHandle InteractBorderPrimitiveHandle;  // +0x34
    std::vector<CEnginePrimitiveHandle> SpecialPrimitiveHandles;   // +0x3c
    std::vector<CIntelligentPointer<CParticleEmitter> > Effects;   // +0x48
};

void CDrawAbilityButtons::Draw(const CEnginePrimitivePrepareInfo& info)
{
    const CThingPlayerCreature* creature = PlayerManager->GetMainPlayer().PeekPControlledCreature();
    if (!IS_THING_ALIVE(creature))
        return;
    SetAsDrawing(true);
    const CPlayerGuiDef& def = CPlayerGui::PeekGuiDef();
    PrimitiveHandles.clear();
    SpecialPrimitiveHandles.clear();
    InteractBorderPrimitiveHandle.Clear();
    PrimitiveHandles.resize(def.AbilityButtons.size() + 48);
    SpecialPrimitiveHandles.resize(def.SpecialAbilityButtons.size() + 3);
    const CTCInventoryAbilities* abilities = 0;
    creature->PeekTC(TCI_HERO_ABILITIES, &abilities);
    long ringsGraphic = 0;
    const C2DVector& ringsPos = def.ABXYRingsPos;
    C2DVector pos = GFAnchorToBottomRight(ringsPos);
    bool normal = false;
    if (PlayerGui->IsNormalToSpecial() || PlayerGui->IsSpecialToNormal() || !PlayerGui->IsShowSpecialAbilityButtons())
        normal = true;
    else {
        const CTCQuickAccessMenu* menu = 0;
        creature->PeekTC(TCI_QUICK_ACCESS_MENU, &menu);
        if (menu->IsActive())
            normal = true;
    }
    if (normal) {
        if (def.ABXYRingsGraphic > 0) {
            CEnginePrimitive2DSprite sprite(0, C2DVector(pos), (EEngine2DLayer)0x46, def.ABXYRingsGraphic, 0, ENGINE_PRIMITIVE_BLEND_ALPHA,
                ENGINE_INTERPOLATION_SET_0, 0.0f, 0.0f, ScaleAlpha(HUDRingAlpha), 0xff, 0xff, 0xff, 0, C2DVector(0.0f, 0.0f), C2DVector(0.0f, 0.0f));
            info.Engine.SetPersistentPrimitive(PrimitiveHandles[0], sprite, 0xc0);
        }
    } else if (def.ABXYRingsFavouriteGraphic > 0) {
        CEnginePrimitive2DSprite sprite(0, C2DVector(pos), (EEngine2DLayer)0x46, def.ABXYRingsFavouriteGraphic, 0, ENGINE_PRIMITIVE_BLEND_ALPHA,
            ENGINE_INTERPOLATION_SET_0, 0.0f, 0.0f, ScaleAlpha(HUDRingAlpha), 0xff, 0xff, 0xff, 0, C2DVector(0.0f, 0.0f), C2DVector(0.0f, 0.0f));
        info.Engine.SetPersistentPrimitive(PrimitiveHandles[0], sprite, 0xc0);
    }
    pos += def.ABXYRingsTopOffset;
    if (normal) {
        if (def.ABXYRingsTopGraphic > 0) {
            CEnginePrimitive2DSprite sprite(0, C2DVector(pos), (EEngine2DLayer)0x46, def.ABXYRingsTopGraphic, 0, ENGINE_PRIMITIVE_BLEND_ALPHA,
                ENGINE_INTERPOLATION_SET_0, 0.0f, 0.0f, ScaleAlpha(HUDRingAlpha), 0xff, 0xff, 0xff, 0, C2DVector(0.0f, 0.0f), C2DVector(0.0f, 0.0f));
            info.Engine.SetPersistentPrimitive(PrimitiveHandles[1], sprite, 0xc0);
        }
    } else if (def.ABXYRingsTopFavouriteGraphic > 0) {
        CEnginePrimitive2DSprite sprite(0, C2DVector(pos), (EEngine2DLayer)0x46, def.ABXYRingsTopFavouriteGraphic, 0, ENGINE_PRIMITIVE_BLEND_ALPHA,
            ENGINE_INTERPOLATION_SET_0, 0.0f, 0.0f, ScaleAlpha(HUDRingAlpha), 0xff, 0xff, 0xff, 0, C2DVector(0.0f, 0.0f), C2DVector(0.0f, 0.0f));
        info.Engine.SetPersistentPrimitive(PrimitiveHandles[1], sprite, 0xc0);
    }
    const CTCInventoryWeapons* weapons = 0;
    creature->PeekTC(TCI_INVENTORY_WEAPONS, &weapons);
    const CTCCarrying* carrying = 0;
    creature->PeekTC(TCI_CARRYING, &carrying);
    pos = GFAnchorToBottomRight(def.WhiteButtonPos);
    if (def.WhiteButtonGraphic > 0) {
        CEnginePrimitive2DSprite sprite(0, C2DVector(pos), (EEngine2DLayer)0x46, def.WhiteButtonGraphic, 0, ENGINE_PRIMITIVE_BLEND_ALPHA,
            ENGINE_INTERPOLATION_SET_0, 0.0f, 0.0f, ScaleAlpha(0xff), 0xff, 0xff, 0xff, 0, C2DVector(0.0f, 0.0f), C2DVector(0.0f, 0.0f));
        info.Engine.SetPersistentPrimitive(PrimitiveHandles[6], sprite, 0xc0);
    }
    pos += def.WhiteButtonIconOffset;
    const CTCHero* hero = 0;
    creature->PeekTC(TCI_HERO, &hero);
    if (hero->WeaponsUsable) {
        CThing* melee = weapons->PActiveMeleeWeapon.Get();
        if (IS_THING_ALIVE(melee)) {
            const CTCInventoryItem* item;
            if (melee->PeekTC(TCI_INVENTORY_ITEM, &item)) {
                long graphic = 0;
                if (carrying->GetThingInPrimarySlot() == melee) {
                    const CTCWeapon* weapon;
                    if (melee->PeekTC(TCI_WEAPON, &weapon)) {
                        if (weapon->GetWeaponType() == 2 && melee->GetDefName() == "OBJECT_HERO_STICK")
                            graphic = def.AbilityButtonSheatheStickGraphic;
                        else if (weapon->GetWeaponType() == 2)
                            graphic = def.AbilityButtonSheatheMaceGraphic;
                        else if (weapon->GetWeaponType() == 1)
                            graphic = def.AbilityButtonSheatheAxeGraphic;
                        else
                            graphic = def.AbilityButtonSheatheSwordGraphic;
                    }
                } else
                    graphic = item->GetEngineGraphic().BankIndex;
                if (graphic > 0) {
                    CEnginePrimitive2DSprite sprite(0, C2DVector(pos), (EEngine2DLayer)0x44, graphic, 0, ENGINE_PRIMITIVE_BLEND_ALPHA,
                        ENGINE_INTERPOLATION_SET_0, 0.0f, 0.0f, ScaleAlpha(0xff), 0xff, 0xff, 0xff, 0, C2DVector(0.0f, 0.0f), C2DVector(0.0f, 0.0f));
                    info.Engine.SetPersistentPrimitive(PrimitiveHandles[7], sprite, 0xc0);
                    if (CUserProfileManager::Get().IsHUDTooltipEnabled()) {
                        CActionInputControl control;
                        if (CUserProfileManager::Get().GetAssignedInputForAction((EGameAction)0x0d, true, &control)
                            || CUserProfileManager::Get().GetAssignedInputForAction((EGameAction)0x0d, false, &control)) {
                            CWideString name;
                            if (CGamePlayerInterface::GetActionName(control, name, false)) {
                                const CFontBank& font = Component->PeekCutSceneMainFont();
                                C2DVector tip = def.WhiteButtonPos + def.WhiteButtonTooltipOffset;
                                C2DVector anchored = GFAnchorToBottomRight(tip);
                                C2DVector text;
                                GFACoordToVCoord(anchored, text);
                                text.X -= font.GetVStringExtents(name).X * 0.6f;
                                PlayerGui->Draw2DText(text, name, font, CRGBColour(0xff, 0xff, 0xff, ScaleAlpha(0xff)), 0.6f, (EEngine2DLayer)0x43, true);
                            }
                        }
                    }
                }
            }
        }
    }
    pos = GFAnchorToBottomRight(def.BlackButtonPos);
    if (def.BlackButtonGraphic > 0) {
        CEnginePrimitive2DSprite sprite(0, C2DVector(pos), (EEngine2DLayer)0x46, def.BlackButtonGraphic, 0, ENGINE_PRIMITIVE_BLEND_ALPHA,
            ENGINE_INTERPOLATION_SET_0, 0.0f, 0.0f, ScaleAlpha(0xff), 0xff, 0xff, 0xff, 0, C2DVector(0.0f, 0.0f), C2DVector(0.0f, 0.0f));
        info.Engine.SetPersistentPrimitive(PrimitiveHandles[8], sprite, 0xc0);
    }
    pos += def.BlackButtonIconOffset;
    if (hero->WeaponsUsable) {
        CThing* ranged = weapons->PActiveRangedWeapon.Get();
        if (IS_THING_ALIVE(ranged)) {
            const CTCInventoryItem* item;
            if (ranged->PeekTC(TCI_INVENTORY_ITEM, &item)) {
                long graphic;
                if (carrying->GetThingInPrimarySlot() == ranged)
                    graphic = def.AbilityButtonSheatheBowGraphic;
                else
                    graphic = item->GetEngineGraphic().BankIndex;
                if (graphic > 0) {
                    CEnginePrimitive2DSprite sprite(0, C2DVector(pos), (EEngine2DLayer)0x44, graphic, 0, ENGINE_PRIMITIVE_BLEND_ALPHA,
                        ENGINE_INTERPOLATION_SET_0, 0.0f, 0.0f, ScaleAlpha(0xff), 0xff, 0xff, 0xff, 0, C2DVector(0.0f, 0.0f), C2DVector(0.0f, 0.0f));
                    info.Engine.SetPersistentPrimitive(PrimitiveHandles[9], sprite, 0xc0);
                    if (CUserProfileManager::Get().IsHUDTooltipEnabled()) {
                        CActionInputControl control;
                        if (CUserProfileManager::Get().GetAssignedInputForAction((EGameAction)0x0e, true, &control)
                            || CUserProfileManager::Get().GetAssignedInputForAction((EGameAction)0x0e, false, &control)) {
                            CWideString name;
                            if (CGamePlayerInterface::GetActionName(control, name, false)) {
                                const CFontBank& font = Component->PeekCutSceneMainFont();
                                C2DVector tip = def.BlackButtonPos + def.BlackButtonTooltipOffset;
                                C2DVector anchored = GFAnchorToBottomRight(tip);
                                C2DVector text;
                                GFACoordToVCoord(anchored, text);
                                text.X -= font.GetVStringExtents(name).X * 0.6f;
                                PlayerGui->Draw2DText(text, name, font, CRGBColour(0xff, 0xff, 0xff, ScaleAlpha(0xff)), 0.6f, (EEngine2DLayer)0x43, true);
                            }
                        }
                    }
                }
            }
        }
    }
    UpdateWeaponAbilities();
    if (normal) {
        for (unsigned long i = 0; i < def.AbilityButtons.size(); ++i) {
            float alpha = PlayerGui->GetButtonAlpha(i, true, !PlayerGui->IsShowSpecialAbilityButtons(), false);
            if (ringsGraphic == 0) {
                CInputEvent event;
                if (PInputManager->IsJoystickEventInQueue((EInputEventType)0x14, def.AbilityButtons[i].ControllerButton, &event,
                                                          PlayerManager->GetMainPlayer().GetJoystickDeviceNumber()))
                    ringsGraphic = def.AbilityButtons[i].ButtonGraphic;
            }
            long ability = PlayerGui->GetAbilityButton(i);
            if (ability == def.ABXYRunGraphic) {
                const CTCHeroSpecialMovement* movement;
                if (creature->PeekTC(TCI_HERO_SPECIAL_MOVEMENT, &movement) && movement != 0 && !NCreatureMode::IsAvailableForHeroRun(*creature))
                    alpha *= 0.5f;
            }
            C2DVector buttonPos = GFAnchorToBottomRight(ringsPos) + def.AbilityButtons[i].Offset;
            C2DVector drawPos(buttonPos);
            if (def.AbilityButtons[i].ControllerType == 0 && ability > 0 && CUserProfileManager::Get().IsHUDTooltipEnabled()) {
                CActionInputControl control;
                if (CUserProfileManager::Get().GetAssignedInputForAction((EGameAction)0x06, true, &control)
                    || CUserProfileManager::Get().GetAssignedInputForAction((EGameAction)0x06, false, &control)) {
                    CWideString name;
                    if (CGamePlayerInterface::GetActionName(control, name, false)) {
                        C2DVector tip = buttonPos + def.InteractButtonTooltipOffset;
                        C2DVector text;
                        GFACoordToVCoord(tip, text);
                        const CFontBank& font = Component->PeekCutSceneMainFont();
                        text.X -= font.GetVStringExtents(name).X * 0.6f;
                        PlayerGui->Draw2DText(text, name, font, CRGBColour(0xff, 0xff, 0xff, ScaleAlpha(0xff)), 0.6f, (EEngine2DLayer)0x49, true);
                    }
                }
            }
            DrawButton(info, drawPos, alpha, i + 2, ability, false, (EEngine2DLayer)0x44);
        }
        for (unsigned long j = 0; j < def.AbilityButtons.size(); ++j) {
            if (def.AbilityButtons[j].ControllerType == 0) {
                C2DVector borderPos = GFAnchorToBottomRight(ringsPos) + def.AbilityButtons[j].Offset;
                CEnginePrimitive2DSprite sprite(0, C2DVector(borderPos + def.InteractButtonBorderOffset), (EEngine2DLayer)0x45, def.InteractButtonBorderGraphic, 0,
                    ENGINE_PRIMITIVE_BLEND_ALPHA, ENGINE_INTERPOLATION_SET_0, 0.0f, 0.0f, ScaleAlpha(0xff), 0xff, 0xff, 0xff, 0, C2DVector(0.0f, 0.0f),
                    C2DVector(0.0f, 0.0f));
                info.Engine.SetPersistentPrimitive(InteractBorderPrimitiveHandle, sprite, 0xc0);
            }
        }
    }
    if ((PlayerGui->IsNormalToSpecial() || PlayerGui->IsSpecialToNormal() || PlayerGui->IsShowSpecialAbilityButtons()) && !normal) {
        float specialAlpha = 0.0f;
        for (unsigned long k = 0; k < def.SpecialAbilityButtons.size(); ++k) {
            specialAlpha = PlayerGui->GetButtonAlpha(k, true, PlayerGui->IsShowSpecialAbilityButtons(), false);
            if (ringsGraphic == 0) {
                CInputEvent event;
                if (PInputManager->IsJoystickEventInQueue((EInputEventType)0x14, def.SpecialAbilityButtons[k].ControllerButton, &event,
                                                          PlayerManager->GetMainPlayer().GetJoystickDeviceNumber()))
                    ringsGraphic = def.SpecialAbilityButtons[k].ButtonGraphic;
            }
            long moveIndex = GetAssignableSpecialMoveIndexForButtonDef(def.SpecialAbilityButtons[k]);
            long graphic = 0;
            if (abilities)
                graphic = abilities->GetSpecialAbilityGraphicSetToButton(moveIndex);
            float drawAlpha = specialAlpha;
            CDefPointer<const CHeroAbilityDef> abilityDef;
            if (abilities) {
                abilityDef = abilities->GetSpecialAbilityPDefSetToButton(moveIndex);
                if (!abilityDef.IsNull()) {
                    if (abilities->IsAbilityAvailable(abilityDef->Ability)) {
                        const CTCSpecialAbilities* special;
                        if (!(!abilities->IsAbleToUseAbility(abilityDef->Ability, true)
                                  ? abilityDef->Ability == 4 && creature->HasTC(TCI_PHYSICAL_SHIELD_SPELL)
                                  : !creature->PeekTC(TCI_SPECIAL_ABILITIES, &special) || special->IsAbilityAvailable(abilityDef->Ability))) {
                            drawAlpha = specialAlpha * (1.0f / 3.0f);
                            long index = SpecialPrimitiveHandles.size() - k;
                            C2DVector uncastablePos = GFAnchorToBottomRight(ringsPos) + def.SpecialAbilityButtons[k].Offset;
                            DrawButton(info, uncastablePos, drawAlpha, index, def.ABXYAbilityUncastableGraphic, true, (EEngine2DLayer)0x43);
                        }
                        if (abilityDef->Ability == 4 && creature->HasTC(TCI_PHYSICAL_SHIELD_SPELL))
                            graphic = def.ABXYAbilityShieldOffGraphic;
                    } else
                        graphic = 0;
                }
            }
            C2DVector buttonPos = GFAnchorToBottomRight(ringsPos) + def.SpecialAbilityButtons[k].Offset;
            DrawButton(info, C2DVector(buttonPos), drawAlpha, k, graphic, true, (EEngine2DLayer)0x44);
            if (!abilityDef.IsNull())
                DrawEffects(k, def.SpecialAbilityButtons[k].Position, *abilityDef);
        }
        if (abilities) {
            if (PlayerGui->IsShowActualAbilityButtons()) {
                if (abilities->GetNumAssignedReceivedAbilities() > 0) {
                    if (abilities->GetNumAssignedAbilityRows() > 1) {
                        C2DVector switchPos = GFAnchorToBottomRight(ringsPos) + def.AbilityButtons[0].Offset;
                        DrawButton(info, switchPos, specialAlpha, 0, def.ABXYAbilitySwitchGraphic, true, (EEngine2DLayer)0x44);
                    }
                    DrawAbilityList(info, specialAlpha);
                }
            } else if (abilities->GetNumAssignedReceivedExpressions() > 0)
                DrawExpressionList(info, specialAlpha);
        }
    } else
        ClearEffects(Effects);
    unsigned char alpha = PlayerGui->GetAlpha();
    if (PlayerGui->IsShowSpecialAbilityButtons() && alpha < 255)
        alpha = 255;
    if (ringsGraphic == 0)
        ringsGraphic = def.ABXYButtonMiddleGraphic;
    if (ringsGraphic > 0) {
        CEnginePrimitive2DSprite sprite(0, GFAnchorToBottomRight(ringsPos) + def.ABXYMiddleOffset, (EEngine2DLayer)0x43, ringsGraphic, 0, ENGINE_PRIMITIVE_BLEND_ALPHA,
            ENGINE_INTERPOLATION_SET_0, 0.0f, 0.0f, ScaleAlpha(alpha), 0xff, 0xff, 0xff, 0, C2DVector(0.0f, 0.0f), C2DVector(0.0f, 0.0f));
        info.Engine.SetPersistentPrimitive(BackgroundPrimitiveHandle, sprite, 0xc0);
    }
    if (!World->IsPaused() && (PlayerGui->IsNormalToSpecial() || PlayerGui->IsSpecialToNormal())) {
        PlayerGui->SetAbilityButtonScale(PlayerGui->GetAbilityButtonScale() + info.Interpolate.Current.GTPredictedTimeSinceLastRenderFrame * 5.0f);
        if (PlayerGui->GetAbilityButtonScale() >= 1.0f) {
            PlayerGui->SetAbilityButtonScale(1.0f);
            PlayerGui->ResetNormalToSpecialState();
        }
    }
}

}
