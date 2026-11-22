// Build the complete method and use typed fixture dependencies.
#define private public
#define protected public
#define FABLE_RETAIL_STLP_BITSET_INDEX
#define FABLE_RETAIL_CDEFSTRINGTABLE
#define FABLE_RETAIL_CDEFSTRING
#define FABLE_RETAIL_CTCINVENTORYABILITIES_LIFETIMES_PASS23
#define FABLE_RETAIL_QUICKACCESS_MAGIC_TEST_PASS27
// CTCQuickAccessMenu::GetMagicListUI — populate the quick-access spell list.
// Leaf: ?GetMagicListUI@CTCQuickAccessMenu@@AAEPAVCComponent@NUISystem@@XZ
// Flags: default
// Fidelity scaffolding: emit the linked size-optimised UI action and STLport
// companions while retaining the speed mode of this quick-access method.
#define FABLE_RETAIL_CCHARSTRING
#define FABLE_RETAIL_CWIDESTRING
#define FABLE_RETAIL_CCOUNTEDPOINTER
#define FABLE_RETAIL_CCOUNTEDPOINTER_RELEASEDATA
#define FABLE_RETAIL_DEF_CLASS_BASE
#define FABLE_RETAIL_CTCBASE
#define FABLE_RETAIL_CTHING
#define FABLE_RETAIL_CTHING_GETTC_EXPANDED
#define FABLE_RETAIL_STLP_LIST
#define FABLE_RETAIL_STLP_MAP
#define FABLE_RETAIL_CTCINVENTORYBASE
#define FABLE_RETAIL_CTCINVENTORYABILITIES
#define FABLE_RETAIL_CTCINVENTORYABILITIES_AVAILABILITY
#define FABLE_RETAIL_CTCINVENTORYEXPERIENCE
#define FABLE_RETAIL_CTCINVENTORYEXPERIENCE_INTERFACETYPE
#define FABLE_RETAIL_CMANAGER
#define FABLE_RETAIL_CLIST
#define FABLE_RETAIL_CCLICKABLE_SHARED_ACTION
#define FABLE_RETAIL_CMENUENTRY_ACTIONPARAMS
#define FABLE_RETAIL_CACTIONPARAMSLONG
#define FABLE_RETAIL_CACTIONPARAMSLONG_INLINE_DTOR_PASS27
#define FABLE_RETAIL_CINTELLIGENTPOINTER
#define FABLE_RETAIL_CTCQUICKACCESSMENU_MAGIC_LIST_PASS27
#define FABLE_RETAIL_CVECTORMAP_COMPONENT_LOWERBOUND_OWNER_PASS26
#define FABLE_RETAIL_STLP_BITSET_TEST_SPEED_OWNER_PASS27
#include "stlport/fable_containers.h"
#include "stlport/fable_map.h"
#include "engine/ETCInterfaceType.h"
#include "fable_string.h"
#include "engine/GFIntToWideString.h"
#include "engine/CCountedPointer.h"
#include "engine/CThing.h"
#include "engine/NUISystem_CComponent.h"
#include "engine/NUISystem_CActionParamSLONG.h"
#include "engine/CTCInventoryAbilities.h"
#include "engine/CTCInventoryExperience.h"
#include "engine/NUISystem_CManager.h"
#include "engine/NUISystem_CList.h"
#include "engine/NUISystem_CMenuEntry.h"
#include "engine/CTCQuickAccessMenu.h"

NUISystem::CComponent* CTCQuickAccessMenu::GetMagicListUI()
{
    using namespace NUISystem;
    CTCInventoryAbilities* ptc_abilities = 0;
    GetParentThing().GetTC(&ptc_abilities);
    std::list<EHeroAbility> abilities;
    for (long count = HERO_ABILITY_FORCE_PUSH; count < MAX_NUMBER_OF_HERO_ABILITIES; ++count) {
        if (ptc_abilities->IsAbilityAvailable(static_cast<EHeroAbility>(count)))
            abilities.push_back(static_cast<EHeroAbility>(count));
    }
    CCountedPointer<CComponent> pmagic_actions_list(CManager::GetInstance()->CreateComponent("UI_LIST_MAGIC_ACTIONS", false));
    CComponent* plist_component = 0;
    CManager* pmanager = CManager::GetInstance();
    if (!abilities.empty()) {
        plist_component = pmanager->CreateComponent("UI_LIST_WIELD_AUGMENT_TEMPLATE_QUICK_ACCESS", false);
        CList* plist = static_cast<CList*>(plist_component);
        plist->InitialiseOffsets(abilities.size());
        for (std::list<EHeroAbility>::iterator it = abilities.begin(); it != abilities.end(); ++it) {
            CWideString name;
            CTCInventoryExperience* ptc_experience;
            if (GetParentThing().GetTC(&ptc_experience)) {
                name = ptc_experience->GetExperienceAbilityName(*it);
                name += CWideString::CreateFromCharString(" ");
                name += GFIntToWideString(ptc_abilities->GetAbilityLevel(*it));
            }
            CCountedPointer<CComponent> pmenu_entry(pmanager->CreateMenuEntryComponent(name, 0.0f, 1.0f,
                "UI_WEAPON_MENU_ENTRY_BRANCH_TEMPLATE_FOR_QUICK_ACCESS", 0.0f, "", 0.0f, "PC_BUTTON_BASTARD"));
            CCountedPointer<CMenuEntry> pmenu_entry_real;
            pmenu_entry_real.SetStaticCast(pmenu_entry);
            CActionParam* pparam_ability = new CActionParamSLONG(*it);
            pmenu_entry_real->AddUIActionParam(pparam_ability);
            plist->AddScrollingChildAndSetOffsets(pmenu_entry);
        }
    }
    return plist_component;
}



#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>
#include <wchar.h>
#include "engine/NUISystem_CListState.h"
#include "engine/NUISystem_CDeletion.h"
#include "engine/C2DBoxF.h"
#include "engine/CRGBFloatColour.h"
#include "engine/CSoundPair.h"
#include "engine/CAnimationEntry.h"

CBaseClass::CBaseClass() {}
CBaseClass::~CBaseClass() {}
CBaseObject::CBaseObject() : IntelligentPointer(0) {}
CBaseObject::~CBaseObject() {}
void CClass::Save(CCharString&) {}
CCharString::CCharString() : PStringData(0) {}
CCharString::~CCharString() { delete PStringData; }
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


static void RequireCheck(bool value, long line)
{
    if (!value) {
        printf("check failed on line %ld\n", line);
        fflush(stdout);
        abort();
    }
}
#define REQUIRE(value) RequireCheck((value), __LINE__)

CThing::~CThing() {}
CTCQuickAccessMenu::CTCQuickAccessMenu(CThing& thing) : CTCBase(thing), Active(false), Quitting(false) {}
CTCQuickAccessMenu::~CTCQuickAccessMenu() {}
NInventory::CTCInventoryBase::CTCInventoryBase(CThing& thing) : CTCBase(thing) {}
CTCInventoryAbilities::CTCInventoryAbilities(CThing& thing) : NInventory::CTCInventoryBase(thing) {}
CTCInventoryAbilities::~CTCInventoryAbilities() {}
CTCInventoryExperience::CTCInventoryExperience(CThing& thing) : NInventory::CTCInventoryBase(thing) {}
CTCInventoryExperience::~CTCInventoryExperience() {}

CCharString::CCharString(const char* text, long length) : PStringData(new CCharStringData)
{
    REQUIRE(length == -1);
    PStringData->Data.PData = const_cast<char*>(text);
    PStringData->Data.StringLength = static_cast<unsigned long>(strlen(text));
}
CCharString::CCharString(const CCharString& other)
    : PStringData(other.PStringData ? new CCharStringData(*other.PStringData) : 0) {}
static const char* Text(const CCharString& text)
{
    return text.PStringData ? text.PStringData->Data.PeekData() : "";
}

struct TestWideStringData : CWideStringData {
    wchar_t Text[64];
    TestWideStringData() { Text[0] = 0; }
};
CWideString::CWideString() : PStringData(new TestWideStringData) {}
CWideString::CWideString(const CWideString& other) : PStringData(new TestWideStringData)
{
    wcscpy(static_cast<TestWideStringData*>(PStringData)->Text,
        static_cast<const TestWideStringData*>(other.PStringData)->Text);
}
CWideString::~CWideString() { delete static_cast<TestWideStringData*>(PStringData); }
const CWideString& CWideString::operator=(const CWideString& other)
{
    wcscpy(static_cast<TestWideStringData*>(PStringData)->Text,
        static_cast<const TestWideStringData*>(other.PStringData)->Text);
    return *this;
}
const CWideString& CWideString::operator+=(const CWideString& other)
{
    wcscat(static_cast<TestWideStringData*>(PStringData)->Text,
        static_cast<const TestWideStringData*>(other.PStringData)->Text);
    return *this;
}
void CWideString::Sprintf(const wchar_t* format, ...)
{
    va_list arguments;
    va_start(arguments, format);
    vswprintf(static_cast<TestWideStringData*>(PStringData)->Text, format, arguments);
    va_end(arguments);
}
CWideString __fastcall CWideString::CreateFromCharString(const CCharString& other)
{
    CWideString out;
    wchar_t* target = static_cast<TestWideStringData*>(out.PStringData)->Text;
    const char* source = Text(other);
    do { *target++ = static_cast<unsigned char>(*source); } while (*source++);
    return out;
}

static bool available[20];
static bool null_actions;
static unsigned int available_count, name_count, level_count, list_count, manager_count;
static unsigned int actions_created, actions_deleted, entries_created, entries_deleted, callbacks_count;
static long expected_ability[19];

bool CTCInventoryAbilities::IsAbilityAvailable(EHeroAbility ability) const
{
    REQUIRE(ability == static_cast<long>(++available_count) && ability < MAX_NUMBER_OF_HERO_ABILITIES);
    return available[ability];
}
long CTCInventoryAbilities::GetAbilityLevel(EHeroAbility ability) const
{
    REQUIRE(ability == expected_ability[level_count++]);
    return 100 + ability;
}
CWideString CTCInventoryExperience::GetExperienceAbilityName(long ability)
{
    REQUIRE(ability == expected_ability[name_count++]);
    CWideString result;
    result.Sprintf(L"Spell%d", ability);
    return result;
}

namespace NUISystem {
CUIInterface::~CUIInterface() {}
CObserver::CObserver() {}
CObserver::~CObserver() {}
CComponent::CComponent(long) {}
CComponent::~CComponent() {}
CChangingStateComponent::CChangingStateComponent(long id) : CComponent(id) {}
CChangingStateComponent::~CChangingStateComponent() {}
CHoverable::CHoverable(long id) : CChangingStateComponent(id) {}
CHoverable::~CHoverable() {}
CClickable::CClickable(long id) : CHoverable(id) {}
CClickable::~CClickable() {}
CMenuEntry::CMenuEntry(long id) : CClickable(id) {}
CMenuEntry::~CMenuEntry() { ++entries_deleted; }
CList::CList(long id) : CChangingStateComponent(id) {}
CList::~CList() {}
CManager::CManager() {}
CManager::~CManager() {}

class TestManager : public CManager {};
class ActionsComponent : public CComponent {
public:
    ActionsComponent() : CComponent(0) {}
    virtual ~ActionsComponent() { ++actions_deleted; }
};
class SpellList : public CList {
public:
    unsigned long expected_size;
    unsigned int entries;
    SpellList() : CList(0), expected_size(0), entries(0) {}
    virtual void InitialiseOffsets(unsigned long count) {
        REQUIRE(expected_size == 0 && entries == 0 && count > 0);
        expected_size = count;
    }
    virtual void AddScrollingChildAndSetOffsets(CCountedPointer<CComponent>& entry) {
        REQUIRE(entry.Get() != 0 && entry.GetReferenceCount() == 2);
        REQUIRE(callbacks_count == entries + 1);
        ++entries;
    }
};
class SpellEntry : public CMenuEntry {
public:
    SpellEntry() : CMenuEntry(0) {}
    virtual void AddUIActionParam(CActionParam* param) {
        REQUIRE(param && param->ActionParamType == UI_ACTION_PARAM_TYPE_SLONG);
        REQUIRE(static_cast<CActionParamSLONG*>(param)->Value == expected_ability[callbacks_count]);
        ++callbacks_count;
        delete param;
    }
};
static SpellList* last_list;
static bool have_experience;

CManager* __fastcall CManager::GetInstance()
{
    static TestManager manager;
    ++manager_count;
    return &manager;
}
CComponent* CManager::CreateComponent(const CCharString& name, bool option)
{
    REQUIRE(!option);
    if (!strcmp(Text(name), "UI_LIST_MAGIC_ACTIONS")) {
        ++actions_created;
        return null_actions ? 0 : new ActionsComponent;
    }
    REQUIRE(!strcmp(Text(name), "UI_LIST_WIELD_AUGMENT_TEMPLATE_QUICK_ACCESS"));
    REQUIRE(++list_count == 1);
    last_list = new SpellList;
    return last_list;
}
CComponent* CManager::CreateMenuEntryComponent(const CWideString& name, float a, float b,
    const CCharString& layout, float c, const CCharString text, float d, CCharString button)
{
    REQUIRE(a == 0.0f && b == 1.0f && c == 0.0f && d == 0.0f);
    REQUIRE(!strcmp(Text(layout), "UI_WEAPON_MENU_ENTRY_BRANCH_TEMPLATE_FOR_QUICK_ACCESS"));
    REQUIRE(!strcmp(Text(text), "") && !strcmp(Text(button), "PC_BUTTON_BASTARD"));
    const wchar_t* title = static_cast<const TestWideStringData*>(name.PStringData)->Text;
    wchar_t expected[64] = {0};
    if (have_experience) {
        const long ability = expected_ability[entries_created];
        swprintf(expected, L"Spell%d %d", ability, 100 + ability);
    }
    REQUIRE(!wcscmp(title, expected));
    ++entries_created;
    return new SpellEntry;
}
}

class TestAbilities : public CTCInventoryAbilities {
public:
    TestAbilities(CThing& thing) : CTCInventoryAbilities(thing) {}
    virtual ETCInterfaceType GetInterfaceType() const { return TCI_HERO_ABILITIES; }
    virtual CCharString GetClassTypeName() const { return CCharString("CTCInventoryAbilities"); }
};

class TestExperience : public CTCInventoryExperience {
public:
    TestExperience(CThing& thing) : CTCInventoryExperience(thing) {}
    virtual ETCInterfaceType GetInterfaceType() const { return TCI_INVENTORY_EXPERIENCE; }
    virtual CCharString GetClassTypeName() const { return CCharString("CTCInventoryExperience"); }
};

class TestThing : public CThing {
public:
    void SetTCs(CTCInventoryAbilities& abilities, CTCInventoryExperience* experience) {
        memset(&UsingComponents, 0, sizeof(UsingComponents));
        if (experience) {
            std::pair<ETCInterfaceType, CTCBase*> value(TCI_INVENTORY_EXPERIENCE, experience);
            Components.push_back(value);
            UsingComponents[TCI_INVENTORY_EXPERIENCE] = true;
        }
        std::pair<ETCInterfaceType, CTCBase*> value(TCI_HERO_ABILITIES, &abilities);
        Components.push_back(value);
        UsingComponents[TCI_HERO_ABILITIES] = true;
    }
};

static void RunCase(bool experience, bool dry_actions, unsigned long availability_mask)
{
    using namespace NUISystem;
    TestThing thing;
    TestAbilities abilities(thing);
    TestExperience inventory_experience(thing);
    CTCQuickAccessMenu menu(thing);
    thing.SetTCs(abilities, experience ? &inventory_experience : 0);
    have_experience = experience;
    null_actions = dry_actions;
    last_list = 0;
    unsigned int expected = 0;
    for (long i = 1; i < MAX_NUMBER_OF_HERO_ABILITIES; ++i) {
        available[i] = (availability_mask & (1ul << i)) != 0;
        if (available[i])
            expected_ability[expected++] = i;
    }
    available_count = name_count = level_count = list_count = manager_count = 0;
    actions_created = actions_deleted = entries_created = entries_deleted = callbacks_count = 0;
    CComponent* result = menu.GetMagicListUI();
    REQUIRE(result == last_list);
    REQUIRE(actions_created == 1 && actions_deleted == static_cast<unsigned>(!dry_actions));
    REQUIRE(manager_count == 2 && available_count == 19);
    REQUIRE(name_count == (experience ? expected : 0) && level_count == name_count);
    REQUIRE(entries_created == expected && entries_deleted == expected && callbacks_count == expected);
    REQUIRE(list_count == static_cast<unsigned>(expected != 0));
    if (last_list) {
        REQUIRE(last_list->expected_size == expected && last_list->entries == expected);
        delete last_list;
    }
}

int main()
{
    RunCase(false, false, 0);
    RunCase(true, true, 0);
    RunCase(false, false, 1ul << 1);
    RunCase(true, false, (1ul << 2) | (1ul << 19));
    RunCase(true, true, 0xFFFFEu);
    puts("00625af0_TEST PASS");
    return 0;
}

// Virtual dependencies outside this fixture fail if invoked.

void CBaseObject::Validate() const { fprintf(stderr, "unexpected CBaseObject::Validate\n"); abort(); }
ETCInterfaceType CTCQuickAccessMenu::GetInterfaceType() const { fprintf(stderr, "unexpected CTCQuickAccessMenu::GetInterfaceType\n"); abort(); }
CCharString CTCQuickAccessMenu::GetClassTypeName() const { fprintf(stderr, "unexpected CTCQuickAccessMenu::GetClassTypeName\n"); abort(); }
void CTCQuickAccessMenu::OnCreate() { fprintf(stderr, "unexpected CTCQuickAccessMenu::OnCreate\n"); abort(); }
void CTCQuickAccessMenu::DrawPreparePrimitivesForRendering(const CEnginePrimitivePrepareInfo &) { fprintf(stderr, "unexpected CTCQuickAccessMenu::DrawPreparePrimitivesForRendering\n"); abort(); }
void CTCQuickAccessMenu::GlobalUpdate(bool) { fprintf(stderr, "unexpected CTCQuickAccessMenu::GlobalUpdate\n"); abort(); }
void CTCInventoryExperience::OpenInventoryInternalPC(bool) { fprintf(stderr, "unexpected CTCInventoryExperience::OpenInventoryInternalPC\n"); abort(); }
namespace NUISystem {

void CUIInterface::Update(float) { fprintf(stderr, "unexpected CUIInterface::Update\n"); abort(); }
void CUIInterface::Draw(CIEngine &, CEnginePrimitiveHandle *, long, long *, NUISystem::CComponent *) { fprintf(stderr, "unexpected CUIInterface::Draw\n"); abort(); }
bool CObserver::IsObservingEvent(EEvent) { fprintf(stderr, "unexpected CObserver::IsObservingEvent\n"); abort(); }
void CObserver::ObserveEvent(EEvent) { fprintf(stderr, "unexpected CObserver::ObserveEvent\n"); abort(); }
void CObserver::IgnoreEvent(EEvent) { fprintf(stderr, "unexpected CObserver::IgnoreEvent\n"); abort(); }
void CObserver::IgnoreAllEvents() { fprintf(stderr, "unexpected CObserver::IgnoreAllEvents\n"); abort(); }
void CObserver::ObserveAllEvents() { fprintf(stderr, "unexpected CObserver::ObserveAllEvents\n"); abort(); }
void CObserver::SetPreventObserving(bool) { fprintf(stderr, "unexpected CObserver::SetPreventObserving\n"); abort(); }
void CObservable::SetEvent(NUISystem::EEvent) { fprintf(stderr, "unexpected CObservable::SetEvent\n"); abort(); }
void CObservable::SetOwnedEvent(NUISystem::CObserver *,NUISystem::EEvent) { fprintf(stderr, "unexpected CObservable::SetOwnedEvent\n"); abort(); }
void CObservable::AddObserver(NUISystem::CObserver *) { fprintf(stderr, "unexpected CObservable::AddObserver\n"); abort(); }
void CObservable::AddExclusiveObserver(NUISystem::CObserver *) { fprintf(stderr, "unexpected CObservable::AddExclusiveObserver\n"); abort(); }
void CObservable::RemoveExclusiveObserver(NUISystem::CObserver *) { fprintf(stderr, "unexpected CObservable::RemoveExclusiveObserver\n"); abort(); }
void CObservable::RemoveObserver(NUISystem::CObserver *) { fprintf(stderr, "unexpected CObservable::RemoveObserver\n"); abort(); }
void CObservable::ClearObservers() { fprintf(stderr, "unexpected CObservable::ClearObservers\n"); abort(); }
NUISystem::CObserver * CObservable::GetExclusiveObserver() { fprintf(stderr, "unexpected CObservable::GetExclusiveObserver\n"); abort(); }
bool CObservable::IsExclusivelyObserved() { fprintf(stderr, "unexpected CObservable::IsExclusivelyObserved\n"); abort(); }
void CObservable::AddConcurrentExclusiveObserver(NUISystem::CObserver *) { fprintf(stderr, "unexpected CObservable::AddConcurrentExclusiveObserver\n"); abort(); }
void CObservable::RemoveConcurrentExclusiveObserver(NUISystem::CObserver *) { fprintf(stderr, "unexpected CObservable::RemoveConcurrentExclusiveObserver\n"); abort(); }
bool CManager::GetMouseDisabled() { fprintf(stderr, "unexpected CManager::GetMouseDisabled\n"); abort(); }
void CManager::SetKeyPressed(unsigned short) { fprintf(stderr, "unexpected CManager::SetKeyPressed\n"); abort(); }
unsigned short CManager::GetKeyPressed() { fprintf(stderr, "unexpected CManager::GetKeyPressed\n"); abort(); }
void CManager::Action(CCountedPointer<NUISystem::CAction> &) { fprintf(stderr, "unexpected CManager::Action\n"); abort(); }
void CManager::SetWeaponsInventory(CTCInventoryWeapons *) { fprintf(stderr, "unexpected CManager::SetWeaponsInventory\n"); abort(); }
void CManager::SetMapInventory(CTCInventoryMap *) { fprintf(stderr, "unexpected CManager::SetMapInventory\n"); abort(); }
void CManager::SetQuestInventory(CTCInventoryQuests *) { fprintf(stderr, "unexpected CManager::SetQuestInventory\n"); abort(); }
void CManager::SetTCHeroExperienceInventory(CTCHeroExperience *) { fprintf(stderr, "unexpected CManager::SetTCHeroExperienceInventory\n"); abort(); }
void CManager::SetAbilitiesInventory(CTCInventoryAbilities *) { fprintf(stderr, "unexpected CManager::SetAbilitiesInventory\n"); abort(); }
void CManager::SetExperienceInventory(CTCInventoryExperience *) { fprintf(stderr, "unexpected CManager::SetExperienceInventory\n"); abort(); }
void CManager::SetInventory(CTCInventory *) { fprintf(stderr, "unexpected CManager::SetInventory\n"); abort(); }
void CManager::SetInventoryClothing(CTCInventoryClothing *) { fprintf(stderr, "unexpected CManager::SetInventoryClothing\n"); abort(); }
void CManager::SetInventoryTrade(CTCInventoryTrade *) { fprintf(stderr, "unexpected CManager::SetInventoryTrade\n"); abort(); }
void CManager::AddThingActionParam(long) { fprintf(stderr, "unexpected CManager::AddThingActionParam\n"); abort(); }
void CManager::RemoveThingActionParam(long) { fprintf(stderr, "unexpected CManager::RemoveThingActionParam\n"); abort(); }
void CManager::SetPlayerThing(CThing *) { fprintf(stderr, "unexpected CManager::SetPlayerThing\n"); abort(); }
void CManager::AddUIActionParam(NUISystem::CActionParam *) { fprintf(stderr, "unexpected CManager::AddUIActionParam\n"); abort(); }
void CManager::RemoveUIActionParam(NUISystem::CActionParam *) { fprintf(stderr, "unexpected CManager::RemoveUIActionParam\n"); abort(); }
void CManager::ClearActionParams() { fprintf(stderr, "unexpected CManager::ClearActionParams\n"); abort(); }
void CManager::SetNumberAugmentations(unsigned long) { fprintf(stderr, "unexpected CManager::SetNumberAugmentations\n"); abort(); }
unsigned long CManager::GetNumberAugmentations() const { fprintf(stderr, "unexpected CManager::GetNumberAugmentations\n"); abort(); }
void CManager::SetChange(NUISystem::CManager::EChange) { fprintf(stderr, "unexpected CManager::SetChange\n"); abort(); }
NUISystem::CManager::EChange CManager::GetChange() { fprintf(stderr, "unexpected CManager::GetChange\n"); abort(); }
void CManager::SetMetaLayer(long) { fprintf(stderr, "unexpected CManager::SetMetaLayer\n"); abort(); }
long CManager::GetMetaLayer() { fprintf(stderr, "unexpected CManager::GetMetaLayer\n"); abort(); }
EEngine2DLayer CManager::GetLayer(long) { fprintf(stderr, "unexpected CManager::GetLayer\n"); abort(); }
void CManager::ClearBastardChild() { fprintf(stderr, "unexpected CManager::ClearBastardChild\n"); abort(); }
void CManager::SetRightStickForce(C2DVector &) { fprintf(stderr, "unexpected CManager::SetRightStickForce\n"); abort(); }
C2DVector & CManager::GetRightStickForce() { fprintf(stderr, "unexpected CManager::GetRightStickForce\n"); abort(); }
void CManager::PlaySoundW(CCharString &,long) { fprintf(stderr, "unexpected CManager::PlaySoundW\n"); abort(); }
unsigned long CManager::PlaySample(CCharString &,long) { fprintf(stderr, "unexpected CManager::PlaySample\n"); abort(); }
CDefPointer<NUISystem::CUIMiscThingsDef> CManager::GetMiscThingsDef() { fprintf(stderr, "unexpected CManager::GetMiscThingsDef\n"); abort(); }
CDefPointer<NUISystem::CUIIconsDef> CManager::GetIconsDef() { fprintf(stderr, "unexpected CManager::GetIconsDef\n"); abort(); }
void CManager::AddActionsListenIgnoreOnList(NUISystem::CComponent *,NUISystem::CComponent *,NUISystem::CComponent *,NUISystem::EEvent) { fprintf(stderr, "unexpected CManager::AddActionsListenIgnoreOnList\n"); abort(); }
void CComponent::Update(float) { fprintf(stderr, "unexpected CComponent::Update\n"); abort(); }
void CComponent::Draw(CIEngine&,CEnginePrimitiveHandle*,long,long*,CComponent*) { fprintf(stderr, "unexpected CComponent::Draw\n"); abort(); }
NUISystem::CComponent * CComponent::FindChild(CCharString) { fprintf(stderr, "unexpected CComponent::FindChild\n"); abort(); }
NUISystem::CComponent * CComponent::FindChildForward(CCharString) { fprintf(stderr, "unexpected CComponent::FindChildForward\n"); abort(); }
void CComponent::FindAllChildren(CCharString, std::list<NUISystem::CComponent *,std::allocator<NUISystem::CComponent *> > &) { fprintf(stderr, "unexpected CComponent::FindAllChildren\n"); abort(); }
void CComponent::FindAllChildrenOfType(NUISystem::EType, std::list<NUISystem::CComponent *,std::allocator<NUISystem::CComponent *> > &) { fprintf(stderr, "unexpected CComponent::FindAllChildrenOfType\n"); abort(); }
void CComponent::SetPAcceptButton(NUISystem::CComponent *) { fprintf(stderr, "unexpected CComponent::SetPAcceptButton\n"); abort(); }
NUISystem::CComponent * CComponent::FindChildNonRecursive(CCharString) { fprintf(stderr, "unexpected CComponent::FindChildNonRecursive\n"); abort(); }
NUISystem::CComponent * CComponent::FindFirstChildOfType(NUISystem::EType) { fprintf(stderr, "unexpected CComponent::FindFirstChildOfType\n"); abort(); }
bool CComponent::HasChild(NUISystem::CComponent *) const { fprintf(stderr, "unexpected CComponent::HasChild\n"); abort(); }
void CComponent::SetZoom(const C2DVector &) { fprintf(stderr, "unexpected CComponent::SetZoom\n"); abort(); }
C2DVector CComponent::GetZoom() const { fprintf(stderr, "unexpected CComponent::GetZoom\n"); abort(); }
void CComponent::SetColour(const CRGBColour &) { fprintf(stderr, "unexpected CComponent::SetColour\n"); abort(); }
CRGBColour CComponent::GetColour() const { fprintf(stderr, "unexpected CComponent::GetColour\n"); abort(); }
void CComponent::SetPosition(const C2DVector &) { fprintf(stderr, "unexpected CComponent::SetPosition\n"); abort(); }
C2DVector CComponent::GetPosition() const { fprintf(stderr, "unexpected CComponent::GetPosition\n"); abort(); }
C2DVector CComponent::GetParentPosition() const { fprintf(stderr, "unexpected CComponent::GetParentPosition\n"); abort(); }
void CComponent::SetParentPosition(C2DVector &) { fprintf(stderr, "unexpected CComponent::SetParentPosition\n"); abort(); }
C2DVector CComponent::GetParentZoom() const { fprintf(stderr, "unexpected CComponent::GetParentZoom\n"); abort(); }
void CComponent::SetParentZoom(C2DVector &) { fprintf(stderr, "unexpected CComponent::SetParentZoom\n"); abort(); }
CRGBColour CComponent::GetParentColour() const { fprintf(stderr, "unexpected CComponent::GetParentColour\n"); abort(); }
void CComponent::SetParentColour(CRGBColour &) { fprintf(stderr, "unexpected CComponent::SetParentColour\n"); abort(); }
C2DVector CComponent::GetSize() const { fprintf(stderr, "unexpected CComponent::GetSize\n"); abort(); }
C2DBoxF CComponent::GetBoundingBox() const { fprintf(stderr, "unexpected CComponent::GetBoundingBox\n"); abort(); }
const C2DVector & CComponent::GetRenderPosition() const { fprintf(stderr, "unexpected CComponent::GetRenderPosition\n"); abort(); }
const C2DVector & CComponent::GetRenderZoom() const { fprintf(stderr, "unexpected CComponent::GetRenderZoom\n"); abort(); }
const CRGBColour & CComponent::GetRenderColour() const { fprintf(stderr, "unexpected CComponent::GetRenderColour\n"); abort(); }
void CComponent::SetRenderPosition(C2DVector &) { fprintf(stderr, "unexpected CComponent::SetRenderPosition\n"); abort(); }
void CComponent::SetRenderZoom(C2DVector &) { fprintf(stderr, "unexpected CComponent::SetRenderZoom\n"); abort(); }
void CComponent::SetRenderColour(CRGBColour &) { fprintf(stderr, "unexpected CComponent::SetRenderColour\n"); abort(); }
void CComponent::ChangePosition(const C2DVector &, float, bool) { fprintf(stderr, "unexpected CComponent::ChangePosition\n"); abort(); }
void CComponent::ChangePositionDelta(const C2DVector &, float, bool) { fprintf(stderr, "unexpected CComponent::ChangePositionDelta\n"); abort(); }
void CComponent::ChangeTargetPositionDelta(const C2DVector &, float, bool) { fprintf(stderr, "unexpected CComponent::ChangeTargetPositionDelta\n"); abort(); }
void CComponent::UpdatePosition(float) { fprintf(stderr, "unexpected CComponent::UpdatePosition\n"); abort(); }
void CComponent::ChangeZoom(const C2DVector &, float, bool) { fprintf(stderr, "unexpected CComponent::ChangeZoom\n"); abort(); }
void CComponent::ChangeZoomDelta(const C2DVector &, float, bool) { fprintf(stderr, "unexpected CComponent::ChangeZoomDelta\n"); abort(); }
void CComponent::UpdateZoom(float) { fprintf(stderr, "unexpected CComponent::UpdateZoom\n"); abort(); }
void CComponent::ChangeColour(const CRGBColour &, float, bool) { fprintf(stderr, "unexpected CComponent::ChangeColour\n"); abort(); }
void CComponent::ChangeColourDelta(const CRGBColour &, float, bool) { fprintf(stderr, "unexpected CComponent::ChangeColourDelta\n"); abort(); }
void CComponent::UpdateColour(float) { fprintf(stderr, "unexpected CComponent::UpdateColour\n"); abort(); }
void CComponent::ResetState() { fprintf(stderr, "unexpected CComponent::ResetState\n"); abort(); }
void CComponent::ForceState(unsigned long, bool) { fprintf(stderr, "unexpected CComponent::ForceState\n"); abort(); }
void CComponent::Initialise() { fprintf(stderr, "unexpected CComponent::Initialise\n"); abort(); }
bool CComponent::HasState(unsigned long) { fprintf(stderr, "unexpected CComponent::HasState\n"); abort(); }
bool CComponent::HasStateInChildren(unsigned long) { fprintf(stderr, "unexpected CComponent::HasStateInChildren\n"); abort(); }
void CComponent::ChangeState(unsigned long) { fprintf(stderr, "unexpected CComponent::ChangeState\n"); abort(); }
void CComponent::ChangeState(unsigned long, float) { fprintf(stderr, "unexpected CComponent::ChangeState\n"); abort(); }
void CComponent::ChangeState(NUISystem::CUIState &) { fprintf(stderr, "unexpected CComponent::ChangeState\n"); abort(); }
bool CComponent::ChangedState() { fprintf(stderr, "unexpected CComponent::ChangedState\n"); abort(); }
NUISystem::CUIState * CComponent::GetCurrentState() { fprintf(stderr, "unexpected CComponent::GetCurrentState\n"); abort(); }
void CComponent::SetParent(NUISystem::CComponent *) { fprintf(stderr, "unexpected CComponent::SetParent\n"); abort(); }
NUISystem::CComponent * CComponent::GetParent() const { fprintf(stderr, "unexpected CComponent::GetParent\n"); abort(); }
std::vector<CCountedPointer<NUISystem::CComponent>,std::allocator<CCountedPointer<NUISystem::CComponent> > > & CComponent::GetChildren() { fprintf(stderr, "unexpected CComponent::GetChildren\n"); abort(); }
float CComponent::GetUpdateTime() const { fprintf(stderr, "unexpected CComponent::GetUpdateTime\n"); abort(); }
C2DVector CComponent::GetTargetPosition() const { fprintf(stderr, "unexpected CComponent::GetTargetPosition\n"); abort(); }
CRGBColour CComponent::GetTargetColour() const { fprintf(stderr, "unexpected CComponent::GetTargetColour\n"); abort(); }
C2DVector CComponent::GetTargetZoom() const { fprintf(stderr, "unexpected CComponent::GetTargetZoom\n"); abort(); }
void CComponent::ProcessChangeState() { fprintf(stderr, "unexpected CComponent::ProcessChangeState\n"); abort(); }
void CComponent::AddChild(CCountedPointer<NUISystem::CComponent> &) { fprintf(stderr, "unexpected CComponent::AddChild\n"); abort(); }
void CComponent::AddChildDynamically(CCountedPointer<NUISystem::CComponent> &) { fprintf(stderr, "unexpected CComponent::AddChildDynamically\n"); abort(); }
void CComponent::AddChildFront(CCountedPointer<NUISystem::CComponent> &) { fprintf(stderr, "unexpected CComponent::AddChildFront\n"); abort(); }
void CComponent::SetDeletion(NUISystem::CDeletion) { fprintf(stderr, "unexpected CComponent::SetDeletion\n"); abort(); }
NUISystem::CDeletion & CComponent::GetDeletion() { fprintf(stderr, "unexpected CComponent::GetDeletion\n"); abort(); }
void CComponent::RemoveChild(unsigned long) { fprintf(stderr, "unexpected CComponent::RemoveChild\n"); abort(); }
NUISystem::EType CComponent::GetType() const { fprintf(stderr, "unexpected CComponent::GetType\n"); abort(); }
void CComponent::AddAction(CCountedPointer<NUISystem::CAction> &) { fprintf(stderr, "unexpected CComponent::AddAction\n"); abort(); }
void CComponent::AddActionOnSelected(CCountedPointer<NUISystem::CAction> &) { fprintf(stderr, "unexpected CComponent::AddActionOnSelected\n"); abort(); }
void CComponent::AddActionOnBack(CCountedPointer<NUISystem::CAction> &) { fprintf(stderr, "unexpected CComponent::AddActionOnBack\n"); abort(); }
void CComponent::AddPreAction(CCountedPointer<NUISystem::CAction> &) { fprintf(stderr, "unexpected CComponent::AddPreAction\n"); abort(); }
void CComponent::AddActionOnInactive(CCountedPointer<NUISystem::CAction> &) { fprintf(stderr, "unexpected CComponent::AddActionOnInactive\n"); abort(); }
void CComponent::AddActionOnLeftClicked(CCountedPointer<NUISystem::CAction> &) { fprintf(stderr, "unexpected CComponent::AddActionOnLeftClicked\n"); abort(); }
void CComponent::AddActionOnLeftHeld(CCountedPointer<NUISystem::CAction> &) { fprintf(stderr, "unexpected CComponent::AddActionOnLeftHeld\n"); abort(); }
void CComponent::AddActionOnRightClicked(CCountedPointer<NUISystem::CAction> &) { fprintf(stderr, "unexpected CComponent::AddActionOnRightClicked\n"); abort(); }
void CComponent::AddActionOnDropped(CCountedPointer<NUISystem::CAction> &) { fprintf(stderr, "unexpected CComponent::AddActionOnDropped\n"); abort(); }
void CComponent::AddActionOnDroppedNowhere(CCountedPointer<NUISystem::CAction> &) { fprintf(stderr, "unexpected CComponent::AddActionOnDroppedNowhere\n"); abort(); }
void CComponent::AddActionOnDraggedUp(CCountedPointer<NUISystem::CAction> &) { fprintf(stderr, "unexpected CComponent::AddActionOnDraggedUp\n"); abort(); }
void CComponent::AddActionOnDraggedDown(CCountedPointer<NUISystem::CAction> &) { fprintf(stderr, "unexpected CComponent::AddActionOnDraggedDown\n"); abort(); }
void CComponent::AddActionOnHovered(CCountedPointer<NUISystem::CAction> &) { fprintf(stderr, "unexpected CComponent::AddActionOnHovered\n"); abort(); }
void CComponent::AddActionOnUnhovered(CCountedPointer<NUISystem::CAction> &) { fprintf(stderr, "unexpected CComponent::AddActionOnUnhovered\n"); abort(); }
void CComponent::AddActionOnLeftUnclicked(CCountedPointer<NUISystem::CAction> &) { fprintf(stderr, "unexpected CComponent::AddActionOnLeftUnclicked\n"); abort(); }
void CComponent::Die() { fprintf(stderr, "unexpected CComponent::Die\n"); abort(); }
std::map<unsigned long,NUISystem::CUIState,std::less<unsigned long>,std::allocator<std::pair<unsigned long const ,NUISystem::CUIState> > > & CComponent::GetStates() { fprintf(stderr, "unexpected CComponent::GetStates\n"); abort(); }
void CComponent::SetName(const CCharString &) { fprintf(stderr, "unexpected CComponent::SetName\n"); abort(); }
CCharString CComponent::GetName() { fprintf(stderr, "unexpected CComponent::GetName\n"); abort(); }
NUISystem::CComponent * CComponent::FindComponent(NUISystem::CComponent *) { fprintf(stderr, "unexpected CComponent::FindComponent\n"); abort(); }
unsigned long CComponent::GetCurrentStateIndex() { fprintf(stderr, "unexpected CComponent::GetCurrentStateIndex\n"); abort(); }
unsigned long CComponent::GetTargetStateIndex() const { fprintf(stderr, "unexpected CComponent::GetTargetStateIndex\n"); abort(); }
float CComponent::CanScroll(bool, C2DBoxF &) { fprintf(stderr, "unexpected CComponent::CanScroll\n"); abort(); }
void CComponent::FakeChildDying(NUISystem::CComponent *) { fprintf(stderr, "unexpected CComponent::FakeChildDying\n"); abort(); }
void CComponent::SetParentViewport(NUISystem::CComponent *) { fprintf(stderr, "unexpected CComponent::SetParentViewport\n"); abort(); }
void CComponent::ClearHandle() { fprintf(stderr, "unexpected CComponent::ClearHandle\n"); abort(); }
void CComponent::MakeUnavailable() { fprintf(stderr, "unexpected CComponent::MakeUnavailable\n"); abort(); }
void CComponent::MakeUnavailableDynamically() { fprintf(stderr, "unexpected CComponent::MakeUnavailableDynamically\n"); abort(); }
void CComponent::MakeAvailable() { fprintf(stderr, "unexpected CComponent::MakeAvailable\n"); abort(); }
void CComponent::MakeAvailableDynamically() { fprintf(stderr, "unexpected CComponent::MakeAvailableDynamically\n"); abort(); }
void CComponent::AddActionIAmParamOf(CCountedPointer<NUISystem::CAction> &) { fprintf(stderr, "unexpected CComponent::AddActionIAmParamOf\n"); abort(); }
NUISystem::CComponent * CComponent::FindFirstChildOfTypeNonRecursive(NUISystem::EType) { fprintf(stderr, "unexpected CComponent::FindFirstChildOfTypeNonRecursive\n"); abort(); }
bool CComponent::IsAllowedToScroll() const { fprintf(stderr, "unexpected CComponent::IsAllowedToScroll\n"); abort(); }
NUISystem::CComponent * CComponent::GetParentViewport() { fprintf(stderr, "unexpected CComponent::GetParentViewport\n"); abort(); }
bool CComponent::IsBastardChild() const { fprintf(stderr, "unexpected CComponent::IsBastardChild\n"); abort(); }
bool CComponent::IsIndependent() const { fprintf(stderr, "unexpected CComponent::IsIndependent\n"); abort(); }
bool CComponent::IsPositionIndependent() const { fprintf(stderr, "unexpected CComponent::IsPositionIndependent\n"); abort(); }
bool CComponent::IsZoomIndependent() const { fprintf(stderr, "unexpected CComponent::IsZoomIndependent\n"); abort(); }
bool CComponent::IsLayerIndependent() const { fprintf(stderr, "unexpected CComponent::IsLayerIndependent\n"); abort(); }
bool CComponent::IsDrawFromViewport() const { fprintf(stderr, "unexpected CComponent::IsDrawFromViewport\n"); abort(); }
bool CComponent::IsPositionIsCenter() const { fprintf(stderr, "unexpected CComponent::IsPositionIsCenter\n"); abort(); }
long CComponent::GetDefIndex() const { fprintf(stderr, "unexpected CComponent::GetDefIndex\n"); abort(); }
CDefPointer<NUISystem::CUIDef const > CComponent::LoadAndReturnDef() const { fprintf(stderr, "unexpected CComponent::LoadAndReturnDef\n"); abort(); }
void CComponent::OnObserverRemove() { fprintf(stderr, "unexpected CComponent::OnObserverRemove\n"); abort(); }
C2DVector CComponent::CanMove(const C2DVector &) { fprintf(stderr, "unexpected CComponent::CanMove\n"); abort(); }
C2DVector CComponent::GetDynamicSize() const { fprintf(stderr, "unexpected CComponent::GetDynamicSize\n"); abort(); }
C2DVector CComponent::GetTargetRenderPosition() const { fprintf(stderr, "unexpected CComponent::GetTargetRenderPosition\n"); abort(); }
bool CComponent::IsPCStyle() const { fprintf(stderr, "unexpected CComponent::IsPCStyle\n"); abort(); }
void CComponent::SetRelativeParentPosition(C2DVector &) { fprintf(stderr, "unexpected CComponent::SetRelativeParentPosition\n"); abort(); }
void CComponent::SetRelativeParentZoom(C2DVector &) { fprintf(stderr, "unexpected CComponent::SetRelativeParentZoom\n"); abort(); }
bool CComponent::IsUsingRelativeZoom() const { fprintf(stderr, "unexpected CComponent::IsUsingRelativeZoom\n"); abort(); }
bool CComponent::IsUsingRelativePosition() const { fprintf(stderr, "unexpected CComponent::IsUsingRelativePosition\n"); abort(); }
C2DVector CComponent::GetRelativeRenderPosition() { fprintf(stderr, "unexpected CComponent::GetRelativeRenderPosition\n"); abort(); }
C2DVector CComponent::GetRelativeRenderZoom() { fprintf(stderr, "unexpected CComponent::GetRelativeRenderZoom\n"); abort(); }
void CComponent::SetRelativeRenderPosition(C2DVector) { fprintf(stderr, "unexpected CComponent::SetRelativeRenderPosition\n"); abort(); }
C2DVector CComponent::SetRelativeRenderZoom(C2DVector) { fprintf(stderr, "unexpected CComponent::SetRelativeRenderZoom\n"); abort(); }
C2DVector CComponent::GetTargetRelativeRenderPosition() const { fprintf(stderr, "unexpected CComponent::GetTargetRelativeRenderPosition\n"); abort(); }
C2DVector CComponent::GetTargetRelativeRenderZoom() const { fprintf(stderr, "unexpected CComponent::GetTargetRelativeRenderZoom\n"); abort(); }
void CComponent::AddPositionChild(NUISystem::CComponent *) { fprintf(stderr, "unexpected CComponent::AddPositionChild\n"); abort(); }
void CComponent::SetPositionParent(NUISystem::CComponent *) { fprintf(stderr, "unexpected CComponent::SetPositionParent\n"); abort(); }
NUISystem::CComponent * CComponent::GetPositionParent() { fprintf(stderr, "unexpected CComponent::GetPositionParent\n"); abort(); }
void CComponent::PositionChildDying(NUISystem::CComponent *) { fprintf(stderr, "unexpected CComponent::PositionChildDying\n"); abort(); }
void CComponent::PositionParentDying(NUISystem::CComponent *) { fprintf(stderr, "unexpected CComponent::PositionParentDying\n"); abort(); }
long CComponent::GetRenderLayer() const { fprintf(stderr, "unexpected CComponent::GetRenderLayer\n"); abort(); }
void CComponent::AddActionOnDesctruction(CCountedPointer<NUISystem::CAction> &) { fprintf(stderr, "unexpected CComponent::AddActionOnDesctruction\n"); abort(); }
void CComponent::ExecuteActions(std::list<CCountedPointer<NUISystem::CAction>,std::allocator<CCountedPointer<NUISystem::CAction> > > *) { fprintf(stderr, "unexpected CComponent::ExecuteActions\n"); abort(); }
bool CComponent::IsClickable() const { fprintf(stderr, "unexpected CComponent::IsClickable\n"); abort(); }
void CComponent::RemoveFromHoveredList() { fprintf(stderr, "unexpected CComponent::RemoveFromHoveredList\n"); abort(); }
void CComponent::ProcessEvent(EEvent) { fprintf(stderr, "unexpected CComponent::ProcessEvent\n"); abort(); }
void CChangingStateComponent::Initialise() { fprintf(stderr, "unexpected CChangingStateComponent::Initialise\n"); abort(); }
void CChangingStateComponent::Update(float) { fprintf(stderr, "unexpected CChangingStateComponent::Update\n"); abort(); }
void CChangingStateComponent::ForceState(unsigned long, bool) { fprintf(stderr, "unexpected CChangingStateComponent::ForceState\n"); abort(); }
void CChangingStateComponent::ChangeState(unsigned long, float) { fprintf(stderr, "unexpected CChangingStateComponent::ChangeState\n"); abort(); }
void CChangingStateComponent::ChangeState(unsigned long) { fprintf(stderr, "unexpected CChangingStateComponent::ChangeState\n"); abort(); }
NUISystem::CUIState * CChangingStateComponent::GetCurrentState() { fprintf(stderr, "unexpected CChangingStateComponent::GetCurrentState\n"); abort(); }
bool CChangingStateComponent::HasState(unsigned long) { fprintf(stderr, "unexpected CChangingStateComponent::HasState\n"); abort(); }
bool CChangingStateComponent::ChangedState() { fprintf(stderr, "unexpected CChangingStateComponent::ChangedState\n"); abort(); }
unsigned long CChangingStateComponent::GetCurrentStateIndex() { fprintf(stderr, "unexpected CChangingStateComponent::GetCurrentStateIndex\n"); abort(); }
unsigned long CChangingStateComponent::GetTargetStateIndex() const { fprintf(stderr, "unexpected CChangingStateComponent::GetTargetStateIndex\n"); abort(); }
float CChangingStateComponent::GetUpdateTime() const { fprintf(stderr, "unexpected CChangingStateComponent::GetUpdateTime\n"); abort(); }
void CChangingStateComponent::AddChildDynamically(CCountedPointer<NUISystem::CComponent> &) { fprintf(stderr, "unexpected CChangingStateComponent::AddChildDynamically\n"); abort(); }
bool CChangingStateComponent::IsPositionIndependent() const { fprintf(stderr, "unexpected CChangingStateComponent::IsPositionIndependent\n"); abort(); }
bool CChangingStateComponent::IsZoomIndependent() const { fprintf(stderr, "unexpected CChangingStateComponent::IsZoomIndependent\n"); abort(); }
void CChangingStateComponent::ProcessChangeState() { fprintf(stderr, "unexpected CChangingStateComponent::ProcessChangeState\n"); abort(); }
NUISystem::CUIState * CChangingStateComponent::GetState(unsigned long) { fprintf(stderr, "unexpected CChangingStateComponent::GetState\n"); abort(); }
const NUISystem::CUIState * CChangingStateComponent::GetState(unsigned long) const { fprintf(stderr, "unexpected CChangingStateComponent::GetState\n"); abort(); }
void CChangingStateComponent::UpdateStateChange() { fprintf(stderr, "unexpected CChangingStateComponent::UpdateStateChange\n"); abort(); }
bool CChangingStateComponent::ChangedStateLastUpdate() { fprintf(stderr, "unexpected CChangingStateComponent::ChangedStateLastUpdate\n"); abort(); }
bool CChangingStateComponent::InternalChanged() { fprintf(stderr, "unexpected CChangingStateComponent::InternalChanged\n"); abort(); }
bool CChangingStateComponent::AllChildrenChangedState() { fprintf(stderr, "unexpected CChangingStateComponent::AllChildrenChangedState\n"); abort(); }
void CChangingStateComponent::OnActivate() { fprintf(stderr, "unexpected CChangingStateComponent::OnActivate\n"); abort(); }
void CChangingStateComponent::OnDeactivate() { fprintf(stderr, "unexpected CChangingStateComponent::OnDeactivate\n"); abort(); }
void CHoverable::Initialise() { fprintf(stderr, "unexpected CHoverable::Initialise\n"); abort(); }
void CHoverable::Update(float) { fprintf(stderr, "unexpected CHoverable::Update\n"); abort(); }
void CHoverable::ProcessEvent(EEvent) { fprintf(stderr, "unexpected CHoverable::ProcessEvent\n"); abort(); }
void CHoverable::AddActionOnHovered(CCountedPointer<CAction>&) { fprintf(stderr, "unexpected CHoverable::AddActionOnHovered\n"); abort(); }
void CHoverable::AddActionOnUnhovered(CCountedPointer<CAction>&) { fprintf(stderr, "unexpected CHoverable::AddActionOnUnhovered\n"); abort(); }
void CHoverable::IgnoreEvent(EEvent) { fprintf(stderr, "unexpected CHoverable::IgnoreEvent\n"); abort(); }
void CHoverable::ChangePosition(const C2DVector&,float,bool) { fprintf(stderr, "unexpected CHoverable::ChangePosition\n"); abort(); }
void CHoverable::RemoveFromHoveredList() { fprintf(stderr, "unexpected CHoverable::RemoveFromHoveredList\n"); abort(); }
bool CHoverable::IsHovered(C2DBoxI&) const { fprintf(stderr, "unexpected CHoverable::IsHovered\n"); abort(); }
void CHoverable::OnHovered() { fprintf(stderr, "unexpected CHoverable::OnHovered\n"); abort(); }
void CHoverable::OnUnhovered() { fprintf(stderr, "unexpected CHoverable::OnUnhovered\n"); abort(); }
void CHoverable::UpdateHovering() { fprintf(stderr, "unexpected CHoverable::UpdateHovering\n"); abort(); }
void CHoverable::OnDeactivate() { fprintf(stderr, "unexpected CHoverable::OnDeactivate\n"); abort(); }
void CClickable::Initialise() { fprintf(stderr, "unexpected CClickable::Initialise\n"); abort(); }
void CClickable::Update(float) { fprintf(stderr, "unexpected CClickable::Update\n"); abort(); }
void CClickable::ProcessEvent(EEvent) { fprintf(stderr, "unexpected CClickable::ProcessEvent\n"); abort(); }
void CClickable::AddActionOnLeftClicked(CCountedPointer<CAction>&) { fprintf(stderr, "unexpected CClickable::AddActionOnLeftClicked\n"); abort(); }
void CClickable::AddActionOnLeftHeld(CCountedPointer<CAction>&) { fprintf(stderr, "unexpected CClickable::AddActionOnLeftHeld\n"); abort(); }
void CClickable::AddActionOnRightClicked(CCountedPointer<CAction>&) { fprintf(stderr, "unexpected CClickable::AddActionOnRightClicked\n"); abort(); }
void CClickable::AddActionOnLeftUnclicked(CCountedPointer<CAction>&) { fprintf(stderr, "unexpected CClickable::AddActionOnLeftUnclicked\n"); abort(); }
bool CClickable::IsClickable() const { fprintf(stderr, "unexpected CClickable::IsClickable\n"); abort(); }
void CClickable::OnLeftClicked() { fprintf(stderr, "unexpected CClickable::OnLeftClicked\n"); abort(); }
void CClickable::OnLeftUnclicked() { fprintf(stderr, "unexpected CClickable::OnLeftUnclicked\n"); abort(); }
void CClickable::OnRightClicked() { fprintf(stderr, "unexpected CClickable::OnRightClicked\n"); abort(); }
void CClickable::OnRightUnclicked() { fprintf(stderr, "unexpected CClickable::OnRightUnclicked\n"); abort(); }
void CClickable::OnHovered() { fprintf(stderr, "unexpected CClickable::OnHovered\n"); abort(); }
void CClickable::OnUnhovered() { fprintf(stderr, "unexpected CClickable::OnUnhovered\n"); abort(); }
void CClickable::OnDeactivate() { fprintf(stderr, "unexpected CClickable::OnDeactivate\n"); abort(); }
void CMenuEntry::AddThingActionParam(long param) { fprintf(stderr, "unexpected CMenuEntry::AddThingActionParam\n"); abort(); }
void CMenuEntry::AddUIActionParam(CActionParam* param) { fprintf(stderr, "unexpected CMenuEntry::AddUIActionParam\n"); abort(); }
void CList::Initialise() { fprintf(stderr, "unexpected CList::Initialise\n"); abort(); }
void CList::Update(float) { fprintf(stderr, "unexpected CList::Update\n"); abort(); }
void CList::ChangeState(unsigned long) { fprintf(stderr, "unexpected CList::ChangeState\n"); abort(); }
void CList::ProcessEvent(NUISystem::EEvent) { fprintf(stderr, "unexpected CList::ProcessEvent\n"); abort(); }
long CList::GetSelectedChild() { fprintf(stderr, "unexpected CList::GetSelectedChild\n"); abort(); }
void CList::RemoveChild(unsigned long) { fprintf(stderr, "unexpected CList::RemoveChild\n"); abort(); }
void CList::AddChildDynamically(CCountedPointer<NUISystem::CComponent> &) { fprintf(stderr, "unexpected CList::AddChildDynamically\n"); abort(); }
void CList::AddScrollingChildDynamically(CCountedPointer<NUISystem::CComponent> &) { fprintf(stderr, "unexpected CList::AddScrollingChildDynamically\n"); abort(); }
void CList::Die() { fprintf(stderr, "unexpected CList::Die\n"); abort(); }
void CList::AddScrollingChild(CCountedPointer<NUISystem::CComponent> &) { fprintf(stderr, "unexpected CList::AddScrollingChild\n"); abort(); }
void CList::AddNonScrollingChild(CCountedPointer<NUISystem::CComponent> &) { fprintf(stderr, "unexpected CList::AddNonScrollingChild\n"); abort(); }
void CList::AddNonScrollingChildDynamically(CCountedPointer<NUISystem::CComponent> &) { fprintf(stderr, "unexpected CList::AddNonScrollingChildDynamically\n"); abort(); }
C2DVector CList::GetPositionOffset() { fprintf(stderr, "unexpected CList::GetPositionOffset\n"); abort(); }
float CList::GetAlphaOffset() { fprintf(stderr, "unexpected CList::GetAlphaOffset\n"); abort(); }
void CList::SetLastInputTimeToBeNow() { fprintf(stderr, "unexpected CList::SetLastInputTimeToBeNow\n"); abort(); }
void CList::SetNextScrollingChildAlpha(float) { fprintf(stderr, "unexpected CList::SetNextScrollingChildAlpha\n"); abort(); }
void CList::SetNextScrollingChildPosition(C2DVector &) { fprintf(stderr, "unexpected CList::SetNextScrollingChildPosition\n"); abort(); }
void CList::AddScrollingChildAndSetOffsets(CCountedPointer<NUISystem::CComponent> &) { fprintf(stderr, "unexpected CList::AddScrollingChildAndSetOffsets\n"); abort(); }
void CList::AddScrollingChildDynamicallyAndSetOffsets(CCountedPointer<NUISystem::CComponent> &) { fprintf(stderr, "unexpected CList::AddScrollingChildDynamicallyAndSetOffsets\n"); abort(); }
void CList::AddActionOnInactive(CCountedPointer<NUISystem::CAction> &) { fprintf(stderr, "unexpected CList::AddActionOnInactive\n"); abort(); }
void CList::AddActionOnSelected(CCountedPointer<NUISystem::CAction> &) { fprintf(stderr, "unexpected CList::AddActionOnSelected\n"); abort(); }
void CList::SetActionOnScrollUp(CCountedPointer<NUISystem::CAction> &) { fprintf(stderr, "unexpected CList::SetActionOnScrollUp\n"); abort(); }
void CList::SetActionOnScrollDown(CCountedPointer<NUISystem::CAction> &) { fprintf(stderr, "unexpected CList::SetActionOnScrollDown\n"); abort(); }
void CList::InitialiseOffsets(unsigned long) { fprintf(stderr, "unexpected CList::InitialiseOffsets\n"); abort(); }
void CList::SetDefaultChild(long) { fprintf(stderr, "unexpected CList::SetDefaultChild\n"); abort(); }
void CList::SetDefaultChild(NUISystem::CComponent *) { fprintf(stderr, "unexpected CList::SetDefaultChild\n"); abort(); }
NUISystem::CListState CList::SaveState() { fprintf(stderr, "unexpected CList::SaveState\n"); abort(); }
void CList::RestoreState(NUISystem::CListState &) { fprintf(stderr, "unexpected CList::RestoreState\n"); abort(); }
void CList::SetSelectedChild(long) { fprintf(stderr, "unexpected CList::SetSelectedChild\n"); abort(); }
bool CList::AllScrollingChildrenDeleting() { fprintf(stderr, "unexpected CList::AllScrollingChildrenDeleting\n"); abort(); }
std::vector<NUISystem::CComponent *,std::allocator<NUISystem::CComponent *> > & CList::GetScrollingChildren() { fprintf(stderr, "unexpected CList::GetScrollingChildren\n"); abort(); }
std::vector<NUISystem::CComponent *,std::allocator<NUISystem::CComponent *> > & CList::GetNonScrollingChildren() { fprintf(stderr, "unexpected CList::GetNonScrollingChildren\n"); abort(); }
void CList::ScrollUp(bool,bool) { fprintf(stderr, "unexpected CList::ScrollUp\n"); abort(); }
void CList::ScrollUp(unsigned long,unsigned long,bool,bool,bool) { fprintf(stderr, "unexpected CList::ScrollUp\n"); abort(); }
void CList::ScrollDown(bool,bool) { fprintf(stderr, "unexpected CList::ScrollDown\n"); abort(); }
void CList::ScrollDown(unsigned long,unsigned long,bool,bool,bool) { fprintf(stderr, "unexpected CList::ScrollDown\n"); abort(); }
bool CList::AreThereAnyDeletions() { fprintf(stderr, "unexpected CList::AreThereAnyDeletions\n"); abort(); }
bool CList::IsWrapping() const { fprintf(stderr, "unexpected CList::IsWrapping\n"); abort(); }
bool CList::IsScrolling() const { fprintf(stderr, "unexpected CList::IsScrolling\n"); abort(); }
bool CList::IsInverted() const { fprintf(stderr, "unexpected CList::IsInverted\n"); abort(); }
void CList::ScrollUntilChild(NUISystem::CComponent *) { fprintf(stderr, "unexpected CList::ScrollUntilChild\n"); abort(); }
void CList::ScrollUntilChildContaining(NUISystem::CComponent *) { fprintf(stderr, "unexpected CList::ScrollUntilChildContaining\n"); abort(); }
C2DVector CList::CanMove(const C2DVector &) { fprintf(stderr, "unexpected CList::CanMove\n"); abort(); }
void CList::ChangePosition(const C2DVector &,float,bool) { fprintf(stderr, "unexpected CList::ChangePosition\n"); abort(); }
NUISystem::CComponent * CList::GetScrollBar() { fprintf(stderr, "unexpected CList::GetScrollBar\n"); abort(); }
NUISystem::CComponent * CList::GetScrollBarOutside() { fprintf(stderr, "unexpected CList::GetScrollBarOutside\n"); abort(); }
void CList::SetInitialPositionDummy(NUISystem::CComponent *) { fprintf(stderr, "unexpected CList::SetInitialPositionDummy\n"); abort(); }
void CList::ScrollToFirstChild() { fprintf(stderr, "unexpected CList::ScrollToFirstChild\n"); abort(); }
void CList::ScrollToLastChild() { fprintf(stderr, "unexpected CList::ScrollToLastChild\n"); abort(); }
void CList::ExecuteActions(std::list<CCountedPointer<NUISystem::CAction>,std::allocator<CCountedPointer<NUISystem::CAction> > > *) { fprintf(stderr, "unexpected CList::ExecuteActions\n"); abort(); }
void CList::SetNonScrollingChildrenStates() { fprintf(stderr, "unexpected CList::SetNonScrollingChildrenStates\n"); abort(); }
void CList::SetRecomputeOffsets(bool) { fprintf(stderr, "unexpected CList::SetRecomputeOffsets\n"); abort(); }
void CList::ScrollUpProcess() { fprintf(stderr, "unexpected CList::ScrollUpProcess\n"); abort(); }
void CList::ScrollDownProcess() { fprintf(stderr, "unexpected CList::ScrollDownProcess\n"); abort(); }
void CList::OnActivate() { fprintf(stderr, "unexpected CList::OnActivate\n"); abort(); }
void CList::DoRecomputeOffsets() { fprintf(stderr, "unexpected CList::DoRecomputeOffsets\n"); abort(); }
void CList::ComputeChildrenVisibility() { fprintf(stderr, "unexpected CList::ComputeChildrenVisibility\n"); abort(); }
void CList::ComputeScrollBarPosition() { fprintf(stderr, "unexpected CList::ComputeScrollBarPosition\n"); abort(); }
void CList::ComputeScrollBarZoom() { fprintf(stderr, "unexpected CList::ComputeScrollBarZoom\n"); abort(); }
}

namespace NHeroInformationScreens {

CWideString CBase::GetScreenName() const { fprintf(stderr, "unexpected CBase::GetScreenName\n"); abort(); }
EPlayerMode CBase::GetPlayerMode() const { fprintf(stderr, "unexpected CBase::GetPlayerMode\n"); abort(); }
NHeroInformationScreens::EScreenType CBase::GetScreenType() const { fprintf(stderr, "unexpected CBase::GetScreenType\n"); abort(); }
bool CBase::IsInventoryScreen() const { fprintf(stderr, "unexpected CBase::IsInventoryScreen\n"); abort(); }
}

namespace NInventory {

void CTCInventoryBase::OnSerialise(CPersistContext&,bool) { fprintf(stderr, "unexpected CTCInventoryBase::OnSerialise\n"); abort(); }
void CTCInventoryBase::OpenInventory() { fprintf(stderr, "unexpected CTCInventoryBase::OpenInventory\n"); abort(); }
}
