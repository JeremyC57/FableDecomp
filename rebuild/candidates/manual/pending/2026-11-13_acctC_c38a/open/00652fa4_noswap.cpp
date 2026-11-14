// NPlayerGui::CDrawQuickAccessIcons::Draw(const CEnginePrimitivePrepareInfo& info) -- draws the d-pad quick-access HUD: the ring sprites, the guild-seal top ring with its tooltip, the context-sensitive and quick-access item icons with their seal / lamp effects, ability crosses, charge graphics and key tooltips, kills unused effects, and the flash / d-pad middle sprites.
// Grade: DIFFER(4654v4653) -- registers as retail, 16 masked hunks: loop 2 keeps the `offset` reference of the C39 draft (1 byte over, lea form); X orders tip / loop 2 / seal / flash / d-pad, d-pad Y, flash fst position.
// Leaf: ?Draw@CDrawQuickAccessIcons@NPlayerGui@@UAEXABVCEnginePrimitivePrepareInfo@@@Z
// Flags: /GS
// Retail: Fable.exe (TLC, VC7.1) @ 0x00652FA4, 4,653 bytes (ret 4 at 0x006541CE). Frame pointer (`lea ebp,[esp-74h]`), /GS cookie, size TU.
// Identity: RTTI vtable 0x0125A704 slot 2; Ego_r.pdb ?Draw@CDrawQuickAccessIcons@NPlayerGui@@UAEXABVCEnginePrimitivePrepareInfo@@@Z.
#pragma optimize("s", on)
#define FABLE_RETAIL_DEF_CLASS_BASE
#define FABLE_RETAIL_CRGBCOLOUR
#define FABLE_RETAIL_CRGBCOLOUR_CTOR4_INITIALISE
#define FABLE_RETAIL_CABILITYDEF_STATGETTYPENAME
#define FABLE_RETAIL_CDEFINITIONMANAGER_GETDEF_GLOBAL_BODY
#define FABLE_RETAIL_CDEFCLASSBASE_GETSUBDEF_BODY
#define FABLE_RETAIL_C2DVECTOR
#define FABLE_RETAIL_C2DVECTOR_NORMALISE
#define FABLE_RETAIL_C2DVECTOR_ISNONZERO_FORCEINLINE
#define FABLE_RETAIL_C2DVECTOR_SQUAREDMAGNITUDE_YX
#define FABLE_RETAIL_C2DCOORDI_TOFLOAT
#define FABLE_RETAIL_C2DCOORDI_NOTEQUAL
#define FABLE_RETAIL_C2DVECTOR_TOC2DCOORDI
#define FABLE_RETAIL_C2DVECTOR_TOC2DVECTOR
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
#define FABLE_RETAIL_CPLAYERGUIDEF_CLASS
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
#define FABLE_RETAIL_CPLAYERGUI_GUILDSEAL
#define FABLE_RETAIL_CPLAYERGUI_CTOR
#define FABLE_RETAIL_CPLAYERGUI_GETINPUTINTERFACE
#define FABLE_RETAIL_CGAMEPLAYERINTERFACE_QUICKACCESSQUEUE
#define FABLE_RETAIL_CWORLD_PEEKDEFINITIONMANAGER
#define FABLE_RETAIL_CWORLD
#define FABLE_RETAIL_CLIGHTDEF
#define FABLE_RETAIL_C3DVECTOR
#define FABLE_RETAIL_C3DVECTOR_ISNONZERO_ZYX
#define FABLE_RETAIL_CPARTICLEEMITTER
#define FABLE_RETAIL_CPARTICLEEMITTER_KILL
#define FABLE_RETAIL_CTCINVENTORY
#define FABLE_RETAIL_CTCINVENTORY_QUICKACCESS
#define FABLE_RETAIL_CTCINVENTORY_DPADICONS
#define FABLE_RETAIL_CTCINVENTORYBASE_SELECTEDOWNED
#define FABLE_RETAIL_CUSERPROFILEMANAGER_CONTEXTITEMINPUT
#define FABLE_RETAIL_CFADEINFADEOUTBASE_DECLS
#define FABLE_RETAIL_CABILITYDEF_SUBDEF
#define FABLE_RETAIL_CWORLD_GUILDSEALRECALLPOS
#define FABLE_RETAIL_CTHINGOBJECTDEF_VALIDATE
#define FABLE_RETAIL_CDEFINITIONMANAGER
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
#include "engine/C2DCoordI.h"
#include "engine/CParticleEmitter.h"
#include "engine/CFadeInFadeOutBase.h"
#include "engine/CDefinitionManager.h"
#include "engine/CGameDefinitionManager.h"
#include "engine/CThingObjectDef.h"
#include "engine/_quarantine/CAbilityDef.h"
#include "engine/CTCInventory.h"
#include "engine/CTCLight.h"
extern long HUDRingAlpha;                                             // 0x01375744 (unnamed .data dword, 255; descriptive name; read as byte and as dword here)
extern C2DVector QuickAccessNumberOffset;                             // 0x01375404 (unnamed static C2DVector (0.1f, 0.01f); descriptive name)
void __fastcall GFACoordToVCoord(const C2DVector& in, C2DVector& out);   // 0x009E21D0
CWideString __fastcall GFIntToWideString(long value);                 // 0x00405F00
namespace NParticleEngine {
CParticleEmitterManager& __fastcall GetParticleEmitterManager();      // 0x00AEAAF0
}
using NParticleEngine::CParticleEmitter;
inline bool CTCLight::IsActive() const { return Active; }             // FableWin ?IsActive@CTCLight@@QBE_NXZ, expanded here ([light+1Ch])
extern CDisplayManager* GPDisplayManager;                             // 0x013B8390 ?GPDisplayManager@@3PAVCDisplayManager@@A
namespace NPlayerGui {
#pragma auto_inline(off)
C2DVector __fastcall GFAnchorToBottomLeft(const C2DVector& pos)      // 0x00643CDA (landed; defined earlier in this source file)
{
    return C2DVector(pos.X, (float)GPDisplayManager->GetRenderTargetDimensions().Y + pos.Y - 480.0f);
}
#pragma auto_inline(on)

class CDrawQuickAccessIcons : public CDrawBase {
public:
    virtual void Draw(const CEnginePrimitivePrepareInfo& info);                                         // slot 2
private:
    void DrawEffectAtPos(const C2DCoordI& pos, long effect, long index);                                // 0x0064546F
    void DrawIcon(const CEnginePrimitivePrepareInfo& info, const C2DVector& position, float alpha, long index, long back, long icon,
                  bool special);                                                                        // 0x00645292
    void DrawChargeGraphic(const CEnginePrimitivePrepareInfo& info, const C2DVector& offset, long index);   // 0x0064D7A6
    long                   ChargingSoundID;                // +0x28
    long                   ChargingButtonIndex;            // +0x2c
    float                  ChargeLevel;                    // +0x30
    bool                   Charging;                       // +0x34
    bool                   ChargeComplete;                 // +0x35
    float                  ChargeFade;                     // +0x38
    long                   FlashButton;                    // +0x3c
    CFadeInFadeOutBase     FlashFade;                      // +0x40
    CEnginePrimitiveHandle ChargeUpPrimitiveHandle;        // +0x5c
    std::vector<CEnginePrimitiveHandle> SpecialPrimitiveHandles;        // +0x64
    std::vector<CEnginePrimitiveHandle> QuickAccessSpellCrossHandles;   // +0x70
    std::vector<CIntelligentPointer<CParticleEmitter> > Effects;        // +0x7c
    std::vector<C2DCoordI> EffectPositions;                             // +0x88
};

#pragma auto_inline(off)
void CDrawQuickAccessIcons::DrawIcon(const CEnginePrimitivePrepareInfo& info, const C2DVector& position, float alpha, long index, long back,
                                     long icon, bool special)   // 0x00645292 (landed; earlier in this source file)
{
    const CPlayerGuiDef* def = &CPlayerGui::PeekGuiDef();
    std::vector<CEnginePrimitiveHandle>& handles = special ? SpecialPrimitiveHandles : PrimitiveHandles;
    unsigned char guiAlpha = PlayerGui->GetAlpha();
    if (PlayerGui->IsShowSpecialAbilityButtons() && guiAlpha < 0xff)
        guiAlpha = 0xff;
    if (back > 0) {
        unsigned char backAlpha = guiAlpha;
        if (back == def->QuickAccessMenuGraphic)
            backAlpha = (unsigned char)(backAlpha * 0.5f);
        CEnginePrimitive2DSprite sprite(0, C2DVector(position.X, position.Y), (EEngine2DLayer)0x45, back, 0, ENGINE_PRIMITIVE_BLEND_ALPHA,
            ENGINE_INTERPOLATION_SET_0, 0.0f, 0.0f, ScaleAlpha(backAlpha), 0xff, 0xff, 0xff, 0, C2DVector(0.0f, 0.0f), C2DVector(0.0f, 0.0f));
        info.Engine.SetPersistentPrimitive(PrimitiveHandles[7], sprite, 0xc0);
    }
    if (icon > 0 && alpha > 0.0001f) {
        float iconAlpha = guiAlpha * alpha;
        if (PlayerGui->IsShowSpecialAbilityButtons() && guiAlpha < 0xff)
            iconAlpha = special ? 255.0f : 0.0f;
        CEnginePrimitive2DSprite sprite(0, C2DVector(position.X, position.Y), (EEngine2DLayer)0x44, icon, 0, ENGINE_PRIMITIVE_BLEND_ALPHA,
            ENGINE_INTERPOLATION_SET_0, 0.0f, 0.0f, ScaleAlpha((unsigned char)iconAlpha), 0xff, 0xff, 0xff, 0, C2DVector(0.0f, 0.0f),
            C2DVector(0.0f, 0.0f));
        info.Engine.SetPersistentPrimitive(handles[index], sprite, 0xc0);
    }
}

void CDrawQuickAccessIcons::DrawEffectAtPos(const C2DCoordI& pos, long effect, long index)   // 0x0064546F (landed; earlier in this source file)
{
    if (pos.ToC2DCoordF().IsNonZero()) {
        if (Effects[index].GetPItem() && pos != EffectPositions[index]) {
            Effects[index]->Kill();
            Effects[index] = 0;
        }
        if (!Effects[index].GetPItem()) {
            const C2DVector& offset = CPlayerGui::PeekGuiDef().QuickAccessEffectOffset;
            C2DVector position(pos.ToC2DCoordF() + offset);
            Effects[index] = NParticleEngine::GetParticleEmitterManager().Create2DEmitter(
                effect, position, ENGINE_2D_LAYER_0, true,
                CRightHandedSet(C3DVector(0.0f, 0.0f, 1.0f), C3DVector(0.0f, 1.0f, 0.0f)));
            EffectPositions[index] = pos;
        }
    } else {
        if (Effects[index].GetPItem()) {
            Effects[index]->Kill();
            Effects[index] = 0;
            EffectPositions[index] = C2DCoordI(0, 0);
        }
    }
}
#pragma auto_inline(on)


void CDrawQuickAccessIcons::Draw(const CEnginePrimitivePrepareInfo& info)
{
    PlayerGui->SetGuildSealLevelUpPos(C2DVector(0.0f, 0.0f));
    CThingPlayerCreature* hero = PlayerManager->GetMainPlayer().GetPControlledCreature();
    if (!IS_THING_ALIVE(hero))
        return;
    SetAsDrawing(true);
    const CPlayerGuiDef& def = CPlayerGui::PeekGuiDef();
    PrimitiveHandles.clear();
    SpecialPrimitiveHandles.clear();
    QuickAccessSpellCrossHandles.clear();
    PrimitiveHandles.resize(def.ContextSensitiveItemButtons.size() + 5);
    SpecialPrimitiveHandles.resize(def.QuickAccessItemButtons.size());
    QuickAccessSpellCrossHandles.resize(def.QuickAccessItemButtons.size());
    ChargeUpPrimitiveHandle.Clear();
    CTCInventory* inventory;
    hero->GetTC(TCI_INVENTORY, &inventory);
    inventory->UpdateContextSensitiveItemFades(info);
    long dpadGraphic = 0;
    const C2DVector& ringsPos = def.DPadRingsPos;
    C2DVector pos = GFAnchorToBottomLeft(ringsPos);
    if (!PlayerGui->IsNormalToSpecial() && !PlayerGui->IsSpecialToNormal() && PlayerGui->IsShowSpecialAbilityButtons()) {
        if (def.DPadRingsFavouriteGraphic > 0) {
            CEnginePrimitive2DSprite sprite(0, C2DVector(pos), (EEngine2DLayer)0x46, def.DPadRingsFavouriteGraphic, 0, ENGINE_PRIMITIVE_BLEND_ALPHA,
                ENGINE_INTERPOLATION_SET_0, 0.0f, 0.0f, ScaleAlpha((unsigned char)HUDRingAlpha), 0xff, 0xff, 0xff, 0, C2DVector(0.0f, 0.0f), C2DVector(0.0f, 0.0f));
            info.Engine.SetPersistentPrimitive(PrimitiveHandles[0], sprite, 0xc0);
        }
    } else if (def.DPadRingsGraphic > 0) {
        CEnginePrimitive2DSprite sprite(0, C2DVector(pos), (EEngine2DLayer)0x46, def.DPadRingsGraphic, 0, ENGINE_PRIMITIVE_BLEND_ALPHA,
            ENGINE_INTERPOLATION_SET_0, 0.0f, 0.0f, ScaleAlpha((unsigned char)HUDRingAlpha), 0xff, 0xff, 0xff, 0, C2DVector(0.0f, 0.0f), C2DVector(0.0f, 0.0f));
        info.Engine.SetPersistentPrimitive(PrimitiveHandles[0], sprite, 0xc0);
    }
    pos += def.DPadRingsTopOffset;
    long topGraphic;
    if (!PlayerGui->IsNormalToSpecial() && !PlayerGui->IsSpecialToNormal() && PlayerGui->IsShowSpecialAbilityButtons())
        topGraphic = def.DPadRingsTopFavouriteGraphic;
    else {
        topGraphic = def.DPadRingsTopGraphic;
        if (World->GetGuildSealRecallPos().IsNonZero())
            topGraphic = def.GuildSealRecallGraphic;
    }
    long alpha = HUDRingAlpha;
    long sealIndex = World->PeekDefinitionManager().GetDefGlobalIndexFromName("OBJECT_GUILD_SEAL_1");
    if (sealIndex > 0 && inventory->GetNumberOfItemsOfTypeInInventory(sealIndex) <= 0)
        alpha /= 2;
    if (topGraphic > 0) {
        if (PlayerManager->GetMainPlayer().AggressiveMode) {
            topGraphic = def.DPadRingsTopAggressiveGraphic;
            alpha = HUDRingAlpha;
        }
        alpha = (long)(PlayerGui->GetGuildSealAlpha() * alpha);
        CEnginePrimitive2DSprite sprite(0, C2DVector(pos), (EEngine2DLayer)0x46, topGraphic, 0, ENGINE_PRIMITIVE_BLEND_ALPHA,
            ENGINE_INTERPOLATION_SET_0, 0.0f, 0.0f, ScaleAlpha((unsigned char)alpha), 0xff, 0xff, 0xff, 0,
            C2DVector(0.0f, 0.0f), C2DVector(0.0f, 0.0f));
        info.Engine.SetPersistentPrimitive(PrimitiveHandles[1], sprite, 0xc0);
        if (CUserProfileManager::Get().IsHUDTooltipEnabled() && inventory->GetNumberOfItemsOfTypeInInventory(sealIndex) > 0) {
            CActionInputControl control;
            if (CUserProfileManager::Get().GetAssignedInputForAction((EGameAction)0x4e, true, &control)
                || CUserProfileManager::Get().GetAssignedInputForAction((EGameAction)0x4e, false, &control)) {
                CWideString name;
                if (CGamePlayerInterface::GetActionName(control, name, false)) {
                    C2DVector tip = ringsPos + def.DPadGuildSealTooltipOffset;
                    C2DVector anchored = GFAnchorToBottomLeft(tip);
                    C2DVector text;
                    GFACoordToVCoord(anchored, text);
                    unsigned char alpha = ScaleAlpha(0xff);
                    const CFontBank& font = Component->PeekCutSceneMainFont();
                    PlayerGui->Draw2DText(text, name, font, CRGBColour(0xff, 0xff, 0xff, alpha), 0.6f,
                                          (EEngine2DLayer)0x49, true);
                }
            }
        }
    }
    long i = 0;
    bool effectUsed[12] = { false, false, false, false, false, false, false, false, false, false, false, false };
    for (; i < def.ContextSensitiveItemButtons.size(); ++i) {
        PlayerGui->GetButtonAlpha(i, false, !PlayerGui->IsShowSpecialAbilityButtons(), true);
        float fade = 1.0f;
        if (dpadGraphic == 0) {
            if (PlayerGui->GetInputInterface().IsQuickAccessEventInQueue(i, (EInputButtonEventType)2,
                                                                     PlayerManager->GetMainPlayer().GetJoystickDeviceNumber()))
                dpadGraphic = def.ContextSensitiveItemButtons[i].ButtonGraphic;
        }
        const CQuickAccessItemButtonGuiDef& button = def.ContextSensitiveItemButtons[i];
        long graphic = inventory->GetContextSensitiveItemGraphicIndex(i);
        const C2DVector& offset = button.Offset;
        C2DVector pos = offset + GFAnchorToBottomLeft(ringsPos);
        long item = inventory->GetQuickAccessItem(i);
        CDefPointer<const CThingObjectDef> objDef;
        if (item > 0 && World->PeekDefinitionManager().CDefinitionManager::GetDef(item, &objDef)) {
            if (objDef->Property & 0x20) {
                fade = PlayerGui->GetGuildSealAlpha();
                PlayerGui->SetGuildSealLevelUpPos(pos + def.GuildSealLevelUpOffset);
                if (World->GetGuildSealRecallPos().IsNonZero())
                    graphic = def.DPadGuildSealRecallGraphic;
                if (ChargingButtonIndex != -1) {
                    DrawEffectAtPos(pos.ToC2DCoordI(), def.QuickAccessGuildSealEffect, i + 9);
                    effectUsed[i + 9] = true;
                }
            }
            const CTCLight* light;
            if ((objDef->Property & 1) && hero->PeekTC(TCI_LIGHT, &light) && light->IsActive()) {
                DrawEffectAtPos(pos.ToC2DCoordI(), def.QuickAccessLampEffect, i + 9);
                effectUsed[i + 9] = true;
            }
        }
        DrawIcon(info, pos, fade, i + 2, 0, graphic, false);
        if (ChargingButtonIndex == i)
            DrawChargeGraphic(info, button.Offset, i);
        if (graphic > 0 && CUserProfileManager::Get().IsHUDTooltipEnabled()) {
            CActionInputControl control;
            if (CUserProfileManager::Get().GetAssignedInputForContextSensitiveItemAction((EGameAction)0x38, true, i, control)
                || CUserProfileManager::Get().GetAssignedInputForContextSensitiveItemAction((EGameAction)0x38, false, i, control)) {
                CWideString name;
                if (CGamePlayerInterface::GetActionName(control, name, false)) {
                    const CFontBank& font = Component->PeekCutSceneMainFont();
                    C2DVector text(GFAXToVX(pos.X), GFAYToVY(pos.Y - 12.0f));
                    if (i == 3) {
                        text.X += GFAXToVX(32.0f);
                        text.Y += GFAXToVX(16.0f);
                    }
                    PlayerGui->Draw2DText(text, name, font, CRGBColour(0xff, 0xff, 0xff, ScaleAlpha(0xff)), 0.6f, (EEngine2DLayer)0x43, true);
                }
            }
        }
    }
    for (i = 0; i < def.QuickAccessItemButtons.size(); ++i) {
        PlayerGui->GetButtonAlpha(i, false, PlayerGui->IsShowSpecialAbilityButtons(), false);
        float fade = 1.0f;
        long graphic = inventory->GetQuickAccessItemGraphicIndex(i);
        long slot = (long)def.ContextSensitiveItemButtons.size() + i;
        const C2DVector& offset = def.QuickAccessItemButtons[i].Offset;
        C2DVector pos = GFAnchorToBottomLeft(ringsPos + offset);
        long item = inventory->GetQuickAccessItem(slot);
        CDefPointer<const CThingObjectDef> objDef;
        if (item > 0) {
            if (World->PeekDefinitionManager().CDefinitionManager::GetDef(item, &objDef)) {
                if (objDef->Property & 0x20) {
                    fade = PlayerGui->GetGuildSealAlpha();
                    PlayerGui->SetGuildSealLevelUpPos(pos + def.GuildSealLevelUpOffset);
                    if (World->GetGuildSealRecallPos().IsNonZero())
                        graphic = def.DPadGuildSealRecallGraphic;
                    if (ChargingButtonIndex != -1) {
                        DrawEffectAtPos(pos.ToC2DCoordI(), def.QuickAccessGuildSealEffect, i);
                        effectUsed[i] = true;
                    }
                }
                const CTCLight* light;
                if ((objDef->Property & 1) && hero->PeekTC(TCI_LIGHT, &light) && light->IsActive()) {
                    DrawEffectAtPos(pos.ToC2DCoordI(), def.QuickAccessLampEffect, i);
                    effectUsed[i] = true;
                }
            }
            CDefPointer<const CThingBaseDef> baseDef;
            if (World->PeekDefinitionManager().CDefinitionManager::GetDef(item, &baseDef)) {
                CDefPointer<const CAbilityDef> abilityDef;
                if (baseDef->GetSubDef(&abilityDef)) {
                    const CTCInventoryAbilities* abilities;
                    if (hero->PeekTC(TCI_HERO_ABILITIES, &abilities) && abilities) {
                        if (abilities->IsAbilityAvailable(abilityDef->Ability)) {
                            const CTCSpecialAbilities* special;
                            if (!(!abilities->IsAbleToUseAbility(abilityDef->Ability, true)
                                      ? abilityDef->Ability == 4 && hero->HasTC(TCI_PHYSICAL_SHIELD_SPELL)
                                      : !hero->PeekTC(TCI_SPECIAL_ABILITIES, &special) || special->IsAbilityAvailable(abilityDef->Ability))) {
                                CEnginePrimitive2DSprite cross(0, pos + def.QuickAccessAbilityCrossOffset, (EEngine2DLayer)0x43,
                                    def.ABXYAbilityUncastableGraphic, 0, ENGINE_PRIMITIVE_BLEND_ALPHA, ENGINE_INTERPOLATION_SET_0, 0.0f, 0.0f,
                                    ScaleAlpha((unsigned char)(long)(fade * 255.0f)), 0xff, 0xff, 0xff, 0, C2DVector(0.0f, 0.0f), C2DVector(0.0f, 0.0f));
                                info.Engine.SetPersistentPrimitive(QuickAccessSpellCrossHandles[i], cross, 0xc0);
                                fade *= 1.0f / 3;
                            }
                            if (abilityDef->Ability == 4 && hero->HasTC(TCI_PHYSICAL_SHIELD_SPELL))
                                graphic = def.ABXYAbilityShieldOffGraphic;
                        } else
                            graphic = 0;
                    }
                }
            }
        }
        DrawIcon(info, pos, fade, i, 0, graphic, true);
        if (ChargingButtonIndex == slot)
            DrawChargeGraphic(info, offset, slot);
        else if (ChargingButtonIndex == -2)
            DrawChargeGraphic(info, C2DVector(0.0f, 0.0f), -2);
        if (graphic > 0 && CUserProfileManager::Get().IsHUDTooltipEnabled()) {
            const CFontBank& font = Component->PeekCutSceneMainFont();
            CRGBColour colour(0xff, 0xff, 0xff, ScaleAlpha(0xff));
            C2DVector text(GFAXToVX(pos.X + QuickAccessNumberOffset.X + 12.0f), GFAYToVY(pos.Y + QuickAccessNumberOffset.Y + 32.0f));
            PlayerGui->Draw2DText(text, GFIntToWideString((i + 1) % 10), font, colour, 0.6f, (EEngine2DLayer)0x43, true);
        }
    }
    for (unsigned long e = 0; e < Effects.size() && e < 12; ++e) {
        if (e != 9 && !effectUsed[e] && Effects[e].GetPItem()) {
            Effects[e]->Kill();
            Effects[e] = 0;
        }
    }
    if (ChargingButtonIndex != -1) {
        DrawEffectAtPos(GFAnchorToBottomLeft(def.GuildSealMainLevelUpOffset + def.DPadRingsPos).ToC2DCoordI(), def.QuickAccessMainGuildSealEffect, 9);
    } else if (Effects[9].GetPItem()) {
        Effects[9]->Kill();
        Effects[9] = 0;
    }
    if (dpadGraphic == 0)
        dpadGraphic = def.QuickAccessDPadGraphic;
    if (FlashButton != 0) {
        dpadGraphic = def.QuickAccessDPadGraphic;
        if (!FlashFade.Active)
            FlashFade.Open(0.5f, 0.5f, CRGBColour(0, 0, 0, 0xff));
        FlashFade.Update(info.Interpolate.Current.GTPredictedTimeSinceLastRenderFrame);
        if (FlashFade.IsOpen())
            FlashFade.Close();
        long flashGraphic = 0;
        switch (FlashButton) {
        case 13: flashGraphic = def.QuickAccessItemButtons[0].ButtonGraphic; break;
        case 14: flashGraphic = def.QuickAccessItemButtons[1].ButtonGraphic; break;
        case 15: flashGraphic = def.QuickAccessItemButtons[2].ButtonGraphic; break;
        case 16: flashGraphic = def.QuickAccessItemButtons[3].ButtonGraphic; break;
        }
        if (flashGraphic > 0) {
            unsigned char flashAlpha = (unsigned char)(long)(FlashFade.GetFadeInOutAlpha() * 255.0f);
            CEnginePrimitive2DSprite sprite(0, (GFAnchorToBottomLeft(ringsPos) + def.DPadMiddleOffset).ToC2DVector(), (EEngine2DLayer)0x43, flashGraphic, 0,
                ENGINE_PRIMITIVE_BLEND_ALPHA, ENGINE_INTERPOLATION_SET_0, 0.0f, 0.0f, ScaleAlpha(flashAlpha), 0xff, 0xff, 0xff, 0,
                C2DVector(0.0f, 0.0f), C2DVector(0.0f, 0.0f));
            info.Engine.SetPersistentPrimitive(PrimitiveHandles[7], sprite, 0xc0);
        }
    }
    if (dpadGraphic > 0) {
        CEnginePrimitive2DSprite sprite(0, (GFAnchorToBottomLeft(ringsPos) + def.DPadMiddleOffset).ToC2DVector(), (EEngine2DLayer)0x44, dpadGraphic, 0,
            ENGINE_PRIMITIVE_BLEND_ALPHA, ENGINE_INTERPOLATION_SET_0, 0.0f, 0.0f, ScaleAlpha(0xff), 0xff, 0xff, 0xff, 0,
            C2DVector(0.0f, 0.0f), C2DVector(0.0f, 0.0f));
        info.Engine.SetPersistentPrimitive(PrimitiveHandles[6], sprite, 0xc0);
    }
}
}
