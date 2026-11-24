// The fixture constructs shared types and supplies the boundaries exercised below.
#define private public
#define protected public
#define FABLE_RETAIL_QUICKACCESS_MAGIC_TEST_PASS27
#define FABLE_RETAIL_RANGED_COMBAT_FIXTURE_PASS27
#define FABLE_RETAIL_CTCINVENTORYBASE
#define FABLE_RETAIL_STLP_BITSET_INDEX
#define FABLE_RETAIL_CDRAWABILITYBUTTONS_TEST_CTORS_PASS27
#define FABLE_RETAIL_NPLAYERGUI_CDRAWBASE_CTOR
#define FABLE_RETAIL_DEF_CLASS_BASE_CTORS
#define FABLE_RETAIL_CDEFPOINTER_POINTER_CTOR_ADOPT
// NPlayerGui::CDrawAbilityButtons::UpdateWeaponAbilities
// Leaf: ?UpdateWeaponAbilities@CDrawAbilityButtons@NPlayerGui@@IAEXXZ
// Flags: /Os /Oy- /d2inlT59
// Inline-budget: /d2inlT59 allows the speed HasTC owner to expand STLport test and keeps the seven leaf calls out of line.
#define FABLE_RETAIL_CDRAWABILITYBUTTONS_WEAPONABILITIES_PASS27
#define FABLE_RETAIL_BASE_OBJECT
#define FABLE_RETAIL_CTCBASE
#define FABLE_RETAIL_CVECTORMAP_CONST_LOWERBOUND_SPEED_OWNER_PASS27
#define FABLE_RETAIL_DEF_CLASS_BASE
#define FABLE_RETAIL_CTHING
#define FABLE_RETAIL_CTHING_GETTC_EXPANDED
#define FABLE_RETAIL_CTHING_HASTC_FORCEINLINE
#define FABLE_RETAIL_CTHING_HASTC_SPEED_OWNER_PASS27
#define FABLE_RETAIL_CTHINGPHYSICAL
#define FABLE_RETAIL_CTHINGGAMEOBJECT
#define FABLE_RETAIL_CTHINGCREATUREBASE
#define FABLE_RETAIL_CTHINGPLAYERCREATURE
#define FABLE_RETAIL_PLAYER
#define FABLE_RETAIL_STLP_THING_BITSET_SPEED_OWNER_PASS26
#define FABLE_RETAIL_STLP_BITSET_TEST_DECL
#define FABLE_RETAIL_CCHARSTRING
#define FABLE_RETAIL_CCLASS
#define FABLE_RETAIL_CDEFSTRINGTABLE
#define FABLE_RETAIL_CDEFSTRING
#define FABLE_RETAIL_STLP_LIST
#define FABLE_RETAIL_STLP_MAP
#define FABLE_RETAIL_CINTELLIGENTPOINTER
#define FABLE_RETAIL_CINTELLIGENTPOINTER_GET_ORIGINAL_CHAIN
#define FABLE_RETAIL_CCOMBATABILITY
#define FABLE_RETAIL_CCOMBATABILITYDATA
#define FABLE_RETAIL_CTCCOMBAT
#define FABLE_RETAIL_CTCCOMBAT_WEAPONTRAIL
#define FABLE_RETAIL_CTCCOMBAT_GETPMELEEFLOURISHABILITY
#define FABLE_RETAIL_CTCCOMBAT_GETPMELEEBLOCKCOUNTERABILITY
#define FABLE_RETAIL_CTCCOMBAT_ISMELEEBLOCKCOUNTERVALID
#define FABLE_RETAIL_IS_THING_ALIVE
#define FABLE_RETAIL_IS_THING_ALIVE_NOINLINE
#define FABLE_RETAIL_NPLAYERGUI_CDRAWABILITYBUTTONS_PASS23
#define FABLE_RETAIL_NPLAYERGUI_CDRAWBASE_ORIGINAL_REFERENCES
#define FABLE_RETAIL_NPLAYERGUI_CDRAWBASE_HANDLES
#define FABLE_RETAIL_CPLAYERGUI
#define FABLE_RETAIL_CPLAYERGUI_CTOR
#define FABLE_RETAIL_CENGINEPERSISTENTPRIMITIVEBASE
#define FABLE_RETAIL_CPLAYERGUI_GETINPUTINTERFACE
#define FABLE_RETAIL_CPLAYERGUI_PEEKGUIDEF
#define FABLE_RETAIL_CPLAYERGUIDEF_CLASS
#define FABLE_RETAIL_C3DVECTOR
#define FABLE_RETAIL_C2DVECTOR
#define FABLE_RETAIL_CTCHERO_TITLE
#define FABLE_RETAIL_CTCHERO_WEAPONUSE
#define FABLE_RETAIL_CTCINVENTORYWEAPONS_ACTIVEMELEE
#define FABLE_RETAIL_CTCINVENTORYWEAPONS_ACTIVERANGED
#define FABLE_RETAIL_CTCWEAPON
#define FABLE_RETAIL_CTCHERORANGEDATTACKS
#include "fable_string.h"

// Fidelity: scaffolding, not retail. Emit the existing speed owner for
// 0x00410DE0 with its default /O2 /Oy mode and restore the caller mode.
#include "stlport/fable_containers.h"
#ifdef _MSC_VER
#pragma optimize("ty", on)
#endif
#include "stlport/fable_thing_bitset.h"
#ifdef _MSC_VER
#pragma optimize("", on)
#endif

#include "engine/CThingPlayerCreature.h"

// Fidelity: scaffolding, not retail. The linked 0x004118C0 HasTC copy follows
// the default /O2 /Oy CThing owner flags, including speed mode and FPO.
#ifdef _MSC_VER
#pragma optimize("s", off)
#pragma optimize("ty", on)
#endif
inline bool CThing::HasTC(ETCInterfaceType type) const
{
    return UsingComponents.test(type);
}
#ifdef _MSC_VER
#pragma optimize("", on)
#endif


#include "engine/CPlayer.h"
#include "engine/CPlayerManager.h"
#include "engine/ThingAlive.h"
#include "engine/CPlayerGui.h"
#include "engine/CPlayerGuiDef.h"
#include "engine/CGamePlayerInterface.h"
#include "engine/CEnginePrimitive2DViewport.h"
#include "engine/NPlayerGui_CDrawAbilityButtons.h"
#include "engine/CTCCombat.h"
#include "engine/CTCHero.h"
#include "engine/CTCInventoryWeapons.h"
#include "engine/CTCCarrying.h"
#include "engine/CTCWeapon.h"
#include "engine/CTCHeroRangedAttacks.h"
#include "engine/CMeleeCombatAbilityDef.h"

void NPlayerGui::CDrawAbilityButtons::UpdateWeaponAbilities()
{
    const CThingPlayerCreature* phero = GetPlayer().PeekPControlledCreature();
    if (!IS_THING_ALIVE(phero))
        return;

    const CTCCombat* ptc_combat;
    phero->PeekTC(&ptc_combat);
    bool melee_weapon_allowed = false;
    bool ranged_weapon_allowed = false;
    bool block_allowed = false;
    const CTCInventoryWeapons* ptc_weapons_inventory;
    phero->PeekTC(&ptc_weapons_inventory);
    const CTCHero* ptc_hero;
    if (phero->PeekTC(&ptc_hero) && ptc_hero->IsWeaponUseAllowed() && ptc_weapons_inventory) {
        if (ptc_weapons_inventory->IsMeleeWeaponDrawn())
            melee_weapon_allowed = IS_THING_ALIVE(ptc_weapons_inventory->GetActiveMeleeWeapon());
        if (ptc_weapons_inventory->IsRangedWeaponDrawn())
            ranged_weapon_allowed = IS_THING_ALIVE(ptc_weapons_inventory->GetActiveRangedWeapon());
    }
    const CTCCarrying* ptc_carrying;
    if (phero->PeekTC(&ptc_carrying)) {
        const CThing* pcarried_thing = ptc_carrying->GetThingInPrimarySlot();
        if (pcarried_thing) {
            const CTCWeapon* ptc_weapon;
            if (pcarried_thing->PeekTC(&ptc_weapon) && ptc_weapon->IsUsableBy(*phero))
                block_allowed = true;
        } else {
            block_allowed = true;
        }
    }
    bool unarmed_attack_allowed = !melee_weapon_allowed && !ranged_weapon_allowed;
    if (phero->HasTC(TCI_BERSERK))
        block_allowed = false;

    const CPlayerGuiDef& gui_def = CPlayerGui::PeekGuiDef();
    const CGamePlayerInterface& player_interface = PlayerGui.GetInputInterface();
    for (long index = 0; index < gui_def.AbilityButtons.size(); ++index) {
        // Inferred: keep the original signed loop index and convert to the vector's size_type for subscripting.
        const std::vector<CAbilityButtonGuiDef>::size_type button_index = index;
        if (player_interface.IsButtonForGameEvent(gui_def.AbilityButtons[button_index].ControllerButton, GAME_ACTION_ATTACK) ||
            player_interface.IsMouseButtonForGameEvent(gui_def.AbilityButtons[button_index].MouseButton, GAME_ACTION_ATTACK)) {
            if (melee_weapon_allowed)
                PlayerGui.SetAbilityButton(index, gui_def.AbilityButtonMeleeGraphic);
            else if (ranged_weapon_allowed)
                PlayerGui.SetAbilityButton(index, gui_def.AbilityButtonRangedGraphic);
            else if (unarmed_attack_allowed)
                PlayerGui.SetAbilityButton(index, gui_def.AbilityButtonUnarmedGraphic);
        } else if (player_interface.IsButtonForGameEvent(gui_def.AbilityButtons[button_index].ControllerButton, GAME_ACTION_BLOCK) ||
                   player_interface.IsMouseButtonForGameEvent(gui_def.AbilityButtons[button_index].MouseButton, GAME_ACTION_BLOCK)) {
            if (block_allowed)
                PlayerGui.SetAbilityButton(index, gui_def.AbilityButtonBlockGraphic);
            else
                PlayerGui.SetAbilityButton(index, 0);
        } else if (player_interface.IsButtonForGameEvent(gui_def.AbilityButtons[button_index].ControllerButton, GAME_ACTION_SPECIAL_ATTACK) ||
                   player_interface.IsMouseButtonForGameEvent(gui_def.AbilityButtons[button_index].MouseButton, GAME_ACTION_SPECIAL_ATTACK)) {
            long flourish_override_graphic;
            if (ptc_combat->IsMeleeFlourishValid()) {
                const CDefPointer<const CMeleeCombatAbilityDef> pmelee_ability = ptc_combat->GetPMeleeFlourishAbility();
                flourish_override_graphic = pmelee_ability->AbilityButtonGraphicOverride;
            } else if (ptc_combat->IsMeleeBlockCounterValid()) {
                const CDefPointer<const CMeleeCombatAbilityDef> pmelee_ability = ptc_combat->GetPMeleeBlockCounterAbility();
                flourish_override_graphic = pmelee_ability->AbilityButtonGraphicOverride;
            } else {
                const CTCHeroRangedAttacks* ptc_ranged;
                if (phero->PeekTC(&ptc_ranged) && ptc_ranged->IsInProcessOfFiringRangedWeapon() && !GetPlayer().IsFirstPersonTargeting())
                    PlayerGui.SetAbilityButton(index, gui_def.ABXYTargetingEnterGraphic);
                else if (PlayerGui.IsDrawingTargetingCentre())
                    PlayerGui.SetAbilityButton(index, gui_def.ABXYTargetingCancelGraphic);
                else
                    PlayerGui.SetAbilityButton(index, gui_def.ABXYRunGraphic);
                continue;
            }
            PlayerGui.SetFlourishEffect(flourish_override_graphic);
            PlayerGui.SetAbilityButton(index, 0);
        }
    }
}

#undef private
#undef protected

#include "engine/CEnginePrimitiveHandle.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
CBaseClass::CBaseClass() {}
CBaseClass::~CBaseClass() {}
CBaseObject::CBaseObject() : IntelligentPointer(0) {}
CBaseObject::~CBaseObject() {}
void CBaseObject::Validate() const {}
void CClass::Save(CCharString&) {}
CCharString::CCharString() : PStringData(0) {}
CCharString::~CCharString() {}
CBaseClassNonCopyable::CBaseClassNonCopyable() {}
CBaseClassNonCopyable::~CBaseClassNonCopyable() {}
CTCBase::CTCBase(CThing& thing) : ParentThing(&thing) {}
CTCBase::~CTCBase() {}
void CTCBase::OnSerialise(CPersistContext&, bool) {}
void CTCBase::OnRemove() {}
void CTCBase::ConstructFromParams(const CTCParamsBase&) {}
void CTCBase::OnCreate() {}
void CTCBase::InitialActivate() {}
void CTCBase::OnInitialActivate() {}
void CTCBase::RenderUpdate(float) {}
bool CTCBase::HasFrameUpdate() const { return false; }
void CTCBase::FrameUpdate() {}
void CTCBase::BulletTimeFrameUpdate() {}
void CTCBase::DrawPreparePrimitivesForRendering(const CEnginePrimitivePrepareInfo&) {}
void CTCBase::Copy(const CTCBase&) {}
void CTCBase::OnDie() {}
void CTCBase::OnKill() {}
void CTCBase::OnLevelUnload() {}
void CTCBase::OnSetInLimbo(bool) {}
bool CTCBase::Validate(CCharString&) const { return true; }
bool CTCBase::IsFreeForDeletion() const { return true; }
bool CTCBase::IsSerialiseCreatable() const { return false; }
bool CTCBase::IsSerialisableForHeroState() const { return false; }
bool CTCBase::IsComponentSerialisable() const { return false; }
bool CTCBase::IsDriver() const { return false; }
bool CTCBase::IsCopyable() const { return false; }
void CTCBase::GlobalUpdate(bool) {}
void CTCBase::HandleMessage(const NTCMessages::CMessageBase&) {}


class CMainGameComponent {};
class CWorld {};
class CGraphicDataBank {};
class CCombatManager {};
static CCombatManager combat_manager;

CInitBaseClass::CInitBaseClass() : Valid(true) {}
CInitBaseClass::~CInitBaseClass() {}
CThingPlayerCreature::CThingPlayerCreature() {}
bool CThingPlayerCreature::DrawPreparePrimitivesForRendering(const CEnginePrimitivePrepareInfo&) { abort(); }
bool CThingPlayerCreature::SetActionForMelee(const CCreatureAbilityDef&) { abort(); }
bool CThingCreatureBase::DrawPreparePrimitivesForRendering(const CEnginePrimitivePrepareInfo&) { abort(); }
CBaseIntelligentPointer::~CBaseIntelligentPointer() {}
CBaseIntelligentPointer::CBaseIntelligentPointer() {}

static const CThingPlayerCreature* creature;
static CThing* carried;
static CThing* melee_item;
static CThing* ranged_item;
static bool melee_drawn, ranged_drawn, weapon_usable, flourish, counter, first_person, targeting;
static long graphics[3], last_effect, updates, released;
static CPlayer player;
static CPlayerGuiDef gui_def;

CPlayer& CPlayerManager::GetMainPlayer() { return player; }
const CThingPlayerCreature* CPlayer::PeekPControlledCreature() const { return creature; }
bool CPlayer::IsFirstPersonTargeting() const { return first_person; }
CThing* CTCCarrying::GetThingInPrimarySlot() const { return carried; }
bool CTCWeapon::IsUsableBy(const CThing&) const { return weapon_usable; }
bool CTCInventoryWeapons::IsMeleeWeaponDrawn() const { return melee_drawn; }
bool CTCInventoryWeapons::IsRangedWeaponDrawn() const { return ranged_drawn; }
CBaseObject* CBaseIntelligentPointer::GetPItem() const
{
    if (creature) {
        const CTCInventoryWeapons* weapons;
        if (creature->PeekTC(&weapons)) {
            if (this == &weapons->PActiveMeleeWeapon) return melee_item;
            if (this == &weapons->PActiveRangedWeapon) return ranged_item;
        }
    }
    return 0;
}
bool CTCCombat::IsMeleeFlourishValid() const { return flourish; }
bool CTCCombat::IsMeleeBlockCounterValid() const { return counter; }
class TestAbility : public CMeleeCombatAbilityDef {
public:
    void Delete() const { ++released; delete this; }
};
static CDefPointer<const CMeleeCombatAbilityDef> Ability(long graphic)
{
    TestAbility* definition = new TestAbility;
    definition->AbilityButtonGraphicOverride = graphic;
    const CDefPointer<CMeleeCombatAbilityDef> result(definition);
    return result;
}
CDefPointer<const CMeleeCombatAbilityDef> CTCCombat::GetPMeleeFlourishAbility() const { return Ability(99); }
CDefPointer<const CMeleeCombatAbilityDef> CTCCombat::GetPMeleeBlockCounterAbility() const { return Ability(88); }

bool CGamePlayerInterface::IsButtonForGameEvent(EXboxControllerButton button, EGameAction action) const
{
    return (long)button == (long)action;
}
bool CGamePlayerInterface::IsMouseButtonForGameEvent(EMouseButtonControl button, EGameAction action) const
{
    return (long)button == (long)action;
}
CPlayerGui::CPlayerGui(const CMainGameComponent& component, CWorld& world, CGamePlayerInterface& input,
                      CPlayerManager& manager, const CGraphicDataBank& bank)
    : World(&world), PlayerManager(&manager), InputInterface(&input), Component(&component), Graphics(&bank) {}
template<> CTBaseSingletonNoDefaultAlloc<CPlayerGui>::~CTBaseSingletonNoDefaultAlloc() {}
const CPlayerGuiDef& FABLE_FASTCALL CPlayerGui::PeekGuiDef() { return gui_def; }
bool CPlayerGui::IsDrawingTargetingCentre() const { return targeting; }
void CPlayerGui::SetFlourishEffect(long effect) { last_effect = effect; }
void CPlayerGui::SetAbilityButton(long button, long graphic)
{
    if (button < 0 || button >= 3) abort();
    graphics[button] = graphic;
    ++updates;
}

NPlayerGui::CDrawBase::CDrawBase(CPlayerGui& gui, std::vector<CDrawBase*>&) : PlayerGui(gui),
    PlayerManager(*gui.PlayerManager), World(*gui.World), Component(*gui.Component), Bank(*gui.Graphics), Drawing(false) {}
NPlayerGui::CDrawBase::~CDrawBase() {}
NPlayerGui::CDrawAbilityButtons::CDrawAbilityButtons(CPlayerGui& gui, std::vector<CDrawBase*>& list)
    : CDrawBase(gui, list) {}
NPlayerGui::CDrawAbilityButtons::~CDrawAbilityButtons() {}
void NPlayerGui::CDrawBase::ClearPrimitives() {}
void NPlayerGui::CDrawBase::ClearPrimitivesIfNecessary() {}
void NPlayerGui::CDrawAbilityButtons::Draw(const CEnginePrimitivePrepareInfo&) { abort(); }
void NPlayerGui::CDrawAbilityButtons::ClearPrimitivesIfNecessary() { abort(); }
long NPlayerGui::CDrawAbilityButtons::GetAssignableSpecialMoveIndexForButtonDef(const CAbilityButtonGuiDef&) const { abort(); }
void NPlayerGui::CDrawAbilityButtons::DrawAbilityList(const CEnginePrimitivePrepareInfo&, float) { abort(); }
void NPlayerGui::CDrawAbilityButtons::GetAbilityButtonMapping(std::vector<long>&) { abort(); }
void NPlayerGui::CDrawAbilityButtons::GetAbilityPositionMapping(std::vector<long>&) { abort(); }
void NPlayerGui::CDrawAbilityButtons::DrawExpressionList(const CEnginePrimitivePrepareInfo&, float) { abort(); }
void NPlayerGui::CDrawAbilityButtons::DrawExpressionButton(const CEnginePrimitivePrepareInfo&, std::vector<CDefPointer<const CExpressionDef> >&, long, long, long, float) { abort(); }

CResource::CResource() { IVRefCount = 1; }
CResource::~CResource() {}
void CIVCountedPointeeBase::Delete() const { abort(); }
bool CResource::UnloadResource() { abort(); }
unsigned long CResource::GetUnloadableMemory() const { abort(); }
bool CResource::IsResourceLoaded() const { abort(); }
void CResource::OnResourceLoaded() { abort(); }
bool CResource::CanAbortResourceLoad(long) const { abort(); }
void CResource::OnResourceLoadAborted() { abort(); }
unsigned long CResource::GetDebugMemoryInUse() const { abort(); }
void CResource::ResetDebugMemoryInUse() { abort(); }
bool CDefClassBase::UnloadResource() { abort(); }
void CDefClassBase::PostTransfer() { abort(); }
void CDefClassBase::Validate(const CValidateDefinitionInitBase&, const CCharString&) const { abort(); }
CParentDefClassBase::~CParentDefClassBase() {}
CParentDefClassBase::CParentDefClassBase(const CParentDefClassBase&) {}
void CThingCreatureBase::OnHit(CHitParameters&) { abort(); }
void CThingGameObject::OnHit(CHitParameters&) { abort(); }
CDefString CDefClassBase::GetInstantiationName() const { abort(); }
void CDefClassBase::SetInstantiationName(CDefString) { abort(); }
void CDefClassBase::AddSubDef(const CCharString&, long, long) { abort(); }
void CDefClassBase::RemoveSubDef(const CCharString&) { abort(); }
CDefClassBase::CSubDefInfo* CDefClassBase::GetPSubDefInfo(const CCharString&) { abort(); }
const CDefClassBase::CSubDefInfo* CDefClassBase::GetPSubDefInfo(const CCharString&) const { abort(); }
bool CDefClassBase::IsParentDef() const { abort(); }
void CDefClassBase::InternalSaveBinary(CDataOutputStream&) { abort(); }
void CDefClassBase::InternalLoadBinary(CDataInputStream&) { abort(); }
CVectorMap<unsigned long, CDefClassBase::CSubDefInfo, CKeyPairCompareLess<unsigned long, CDefClassBase::CSubDefInfo> >* CDefClassBase::GetPSubDefInfoMap() { abort(); }
CDefString CParentDefClassBase::GetInstantiationName() const { abort(); }
void CParentDefClassBase::SetInstantiationName(CDefString) { abort(); }
void CParentDefClassBase::AddSubDef(const CCharString&, long, long) { abort(); }
void CParentDefClassBase::RemoveSubDef(const CCharString&) { abort(); }
CDefClassBase::CSubDefInfo* CParentDefClassBase::GetPSubDefInfo(const CCharString&) { abort(); }
const CDefClassBase::CSubDefInfo* CParentDefClassBase::GetPSubDefInfo(const CCharString&) const { abort(); }
bool CParentDefClassBase::IsParentDef() const { abort(); }
void CParentDefClassBase::InternalSaveBinary(CDataOutputStream&) { abort(); }
void CParentDefClassBase::InternalLoadBinary(CDataInputStream&) { abort(); }
CVectorMap<unsigned long, CDefClassBase::CSubDefInfo, CKeyPairCompareLess<unsigned long, CDefClassBase::CSubDefInfo> >* CParentDefClassBase::GetPSubDefInfoMap() { abort(); }
void CAbilityButtonGuiDef::Transfer(CPersistContext&) { abort(); }
void CAbilityButtonGuiDef::Copy(const CDefClassBase*) { abort(); }
unsigned long CAbilityButtonGuiDef::GetSizeofClass() { return sizeof(*this); }
void CMeleeCombatAbilityDef::Transfer(CPersistContext&) { abort(); }
void CMeleeCombatAbilityDef::Copy(const CDefClassBase*) { abort(); }
unsigned long CMeleeCombatAbilityDef::GetSizeofClass() { return sizeof(*this); }
void CPlayerGuiDef::Transfer(CPersistContext&) { abort(); }
void CPlayerGuiDef::Copy(const CDefClassBase*) { abort(); }
unsigned long CPlayerGuiDef::GetSizeofClass() { return sizeof(*this); }

class TestCarrying : public CTCCarrying {
public:
    TestCarrying(CThing& thing) : CTCCarrying(thing) {}
    ETCInterfaceType GetInterfaceType() const { return TCI_CARRYING; }
    CCharString GetClassTypeName() const { return CCharString(); }
};
class TestWeapon : public CTCWeapon {
public:
    TestWeapon(CThing& thing) : CTCWeapon(thing) {}
    ETCInterfaceType GetInterfaceType() const { return TCI_WEAPON; }
    CCharString GetClassTypeName() const { return CCharString(); }
};
class TestCombat : public CTCCombat {
public:
    TestCombat(CThing& thing) : CTCCombat(thing) {}
    ETCInterfaceType GetInterfaceType() const { return TCI_COMBAT; }
    CCharString GetClassTypeName() const { return CCharString(); }
};
class TestHero : public CTCHero {
public:
    TestHero(CThing& thing) : CTCHero(thing) {}
    ETCInterfaceType GetInterfaceType() const { return TCI_HERO; }
    CCharString GetClassTypeName() const { return CCharString(); }
};
class TestRanged : public CTCHeroRangedAttacks {
public:
    TestRanged(CThing& thing) : CTCHeroRangedAttacks(thing) {}
    ETCInterfaceType GetInterfaceType() const { return TCI_HERO_RANGED_ATTACKS; }
    CCharString GetClassTypeName() const { return CCharString(); }
};
class TestInventory : public CTCInventoryWeapons {
public:
    explicit TestInventory(CThing& thing) : CTCInventoryWeapons(thing) {}
    ETCInterfaceType GetInterfaceType() const { return TCI_INVENTORY_WEAPONS; }
    CCharString GetClassTypeName() const { return CCharString(); }
};
CTCWeapon::CTCWeapon(CThing& thing) : CTCBase(thing) {}
NInventory::CTCInventoryBase::CTCInventoryBase(CThing& thing) : CTCBase(thing) {}
CTCInventoryWeapons::CTCInventoryWeapons(CThing& thing) : NInventory::CTCInventoryBase(thing) {}
NUISystem::CComponent* CTCInventoryWeapons::ConstructScreen(NUISystem::CComponent*) { abort(); }
NUISystem::CComponent* CTCInventoryWeapons::ConstructScreenPC(NUISystem::CComponent*) { abort(); }
void NInventory::CTCInventoryBase::OnSerialise(CPersistContext&, bool) { abort(); }
void NInventory::CTCInventoryBase::OpenInventory() { abort(); }
CWideString NHeroInformationScreens::CBase::GetScreenName() const { abort(); }
EPlayerMode NHeroInformationScreens::CBase::GetPlayerMode() const { abort(); }
NHeroInformationScreens::EScreenType NHeroInformationScreens::CBase::GetScreenType() const { abort(); }
bool NHeroInformationScreens::CBase::IsInventoryScreen() const { abort(); }
CTCCombat::CTCCombat(CThing& thing) : CTCBase(thing), CombatManager(combat_manager) {}
CTCHero::CTCHero(CThing& thing) : CTCBase(thing) {}
CTCCarrying::CTCCarrying(CThing& thing) : CTCBase(thing) {}
CTCHeroRangedAttacks::CTCHeroRangedAttacks(CThing& thing) : CTCBase(thing) {}
void CTCHero::FrameUpdate() { abort(); }
template<size_t N> bool std::bitset<N>::test(size_t key) const { return _Unchecked_test(key); }
static void AddComponent(CThing& thing, ETCInterfaceType id, CTCBase* component)
{
    thing.UsingComponents[id] = true;
    thing.Components.push_back(std::pair<ETCInterfaceType, CTCBase*>(id, component));
    for (unsigned index = thing.Components.size() - 1; index > 0; --index) {
        if (thing.Components[index - 1].first <= thing.Components[index].first)
            break;
        const std::pair<ETCInterfaceType, CTCBase*> previous = thing.Components[index - 1];
        thing.Components[index - 1] = thing.Components[index];
        thing.Components[index] = previous;
    }
}
int main()
{
    CMainGameComponent component;
    CWorld world;
    CGraphicDataBank bank;
    CGamePlayerInterface input;
    CPlayerManager manager;
    CPlayerGui gui(component, world, input, manager, bank);
    std::vector<NPlayerGui::CDrawBase*> drawers;
    NPlayerGui::CDrawAbilityButtons draw(gui, drawers);
    draw.UpdateWeaponAbilities();
    if (updates) return 1;
    CThingPlayerCreature hero;
    creature = &hero;
    hero.Dead = true;
    draw.UpdateWeaponAbilities();
    if (updates) return 2;
    hero.Dead = false;
    memset(hero.UsingComponents._M_w, 0, sizeof(hero.UsingComponents._M_w));
    TestCombat combat(hero);
    TestHero hero_component(hero);
    TestCarrying carrying(hero);
    TestRanged ranged(hero);
    // Start with the disabled weapon component and an empty inventory.
    hero_component.WeaponsUsable = false;
    ranged.FireCycleInProgress = false;
    AddComponent(hero, TCI_HERO, &hero_component);
    AddComponent(hero, TCI_COMBAT, &combat);
    AddComponent(hero, TCI_CARRYING, &carrying);
    AddComponent(hero, TCI_HERO_RANGED_ATTACKS, &ranged);
    gui_def.AbilityButtonUnarmedGraphic = 17;
    gui_def.AbilityButtonBlockGraphic = 21;
    gui_def.ABXYRunGraphic = 31;
    gui_def.ABXYTargetingEnterGraphic = 32;
    gui_def.ABXYTargetingCancelGraphic = 33;
    CAbilityButtonGuiDef button;
    button.ControllerButton = static_cast<EXboxControllerButton>(GAME_ACTION_ATTACK);
    button.MouseButton = static_cast<EMouseButtonControl>(0);
    gui_def.AbilityButtons.push_back(button);
    button.ControllerButton = static_cast<EXboxControllerButton>(0);
    button.MouseButton = static_cast<EMouseButtonControl>(GAME_ACTION_BLOCK);
    gui_def.AbilityButtons.push_back(button);
    button.ControllerButton = static_cast<EXboxControllerButton>(GAME_ACTION_SPECIAL_ATTACK);
    button.MouseButton = static_cast<EMouseButtonControl>(0);
    gui_def.AbilityButtons.push_back(button);
    draw.UpdateWeaponAbilities();
    if (updates != 3 || graphics[0] != 17 || graphics[1] != 21 || graphics[2] != 31) return 3;
    hero.UsingComponents[TCI_BERSERK] = true;
    targeting = true;
    draw.UpdateWeaponAbilities();
    if (graphics[1] != 0 || graphics[2] != 33) return 4;
    ranged.FireCycleInProgress = true;
    draw.UpdateWeaponAbilities();
    if (graphics[2] != 32) return 5;
    first_person = true;
    draw.UpdateWeaponAbilities();
    if (graphics[2] != 33) return 6;
    flourish = true;
    draw.UpdateWeaponAbilities();
    if (graphics[2] != 0 || last_effect != 99 || released != 1) return 7;
    flourish = false;
    counter = true;
    draw.UpdateWeaponAbilities();
    if (graphics[2] != 0 || last_effect != 88 || released != 2) return 8;
    counter = false;
    hero.UsingComponents[TCI_BERSERK] = false;
    CThing melee_weapon, ranged_weapon;
    melee_item = &melee_weapon;
    ranged_item = &ranged_weapon;
    melee_weapon.Dead = false;
    ranged_weapon.Dead = false;
    gui_def.AbilityButtonMeleeGraphic = 41;
    gui_def.AbilityButtonRangedGraphic = 42;
    TestInventory inventory(hero);
    AddComponent(hero, TCI_INVENTORY_WEAPONS, &inventory);
    melee_drawn = ranged_drawn = true;
    draw.UpdateWeaponAbilities();
    if (graphics[0] != 17) return 9;
    hero_component.WeaponsUsable = true;
    draw.UpdateWeaponAbilities();
    if (graphics[0] != 41) return 10;
    melee_weapon.Dead = true;
    draw.UpdateWeaponAbilities();
    if (graphics[0] != 42) return 11;
    ranged_drawn = false;
    draw.UpdateWeaponAbilities();
    if (graphics[0] != 17) return 12;
    carried = &melee_weapon;
    memset(carried->UsingComponents._M_w, 0, sizeof(carried->UsingComponents._M_w));
    draw.UpdateWeaponAbilities();
    if (graphics[1] != 0) return 13;
    TestWeapon weapon(*carried);
    AddComponent(*carried, TCI_WEAPON, &weapon);
    draw.UpdateWeaponAbilities();
    if (graphics[1] != 0) return 14;
    weapon_usable = true;
    draw.UpdateWeaponAbilities();
    if (graphics[1] != 21) return 15;
    puts("0064b1ac_TEST PASS");
    return 0;
}