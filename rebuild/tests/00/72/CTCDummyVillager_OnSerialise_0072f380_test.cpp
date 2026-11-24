// CTCDummyVillager::OnSerialise()
// Leaf: ?OnSerialise@CTCDummyVillager@@UAEXAAVCPersistContext@@_N@Z
// Flags: default
#define FABLE_RETAIL_CTHING
#define FABLE_RETAIL_CTHINGPHYSICAL
#define FABLE_RETAIL_CTHINGGAMEOBJECT
#define FABLE_RETAIL_CTHINGCREATUREBASE
#define FABLE_RETAIL_CTHINGAICREATURE
#define FABLE_RETAIL_CTHINGBUILDING
#define FABLE_RETAIL_CTCBASE
#define FABLE_RETAIL_CINTELLIGENTPOINTER
#define FABLE_RETAIL_CINTELLIGENTPOINTER_COMPARE_NULL_INT
#define FABLE_RETAIL_CINTELLIGENTPOINTER_GET_ORIGINAL_CHAIN
#define FABLE_RETAIL_IS_THING_ALIVE
#define FABLE_RETAIL_C3DVECTOR
#define FABLE_RETAIL_CCHARSTRING
#define FABLE_RETAIL_DEF_CLASS_BASE
#define FABLE_RETAIL_CDEFSTRING
#define FABLE_RETAIL_CDEFSTRINGTABLE
#define FABLE_RETAIL_CDEFINITIONMANAGER
#define FABLE_RETAIL_CRESOURCELIST
#define FABLE_RETAIL_CTHINGMANAGER
#define FABLE_RETAIL_CCLASSFACTORY
#define FABLE_RETAIL_CPERSISTCONTEXT
#define FABLE_RETAIL_CPERSISTCONTEXT_NAMED_DEFAULTS
#define FABLE_RETAIL_CPERSISTCONTEXT_GETDEFAULTVAL_BODY
#define FABLE_RETAIL_CPERSISTTRAITS
#define FABLE_RETAIL_CPERSISTTRAITS_UID_DEFAULT_PASS27
#define FABLE_RETAIL_CDATAOUTPUTSTREAM
#define private public
#define protected public
#include "engine/CThingAICreature.h"
#include "engine/CThingBuilding.h"
#include "engine/CTCDummyVillager.h"
#include "engine/CThingManager.h"
#include "engine/CDefString.h"
#include "engine/CGameDefinitionManager.h"
#include "engine/CDataOutputStream.h"
#include "engine/CPersistTraits.h"
#include "engine/ThingAlive.h"
#undef private
#undef protected

template<> void CPersistContext::Transfer(const char*, CCharString&, const CCharString&);
template<> void CPersistContext::Transfer(const char*, unsigned __int64&, const unsigned __int64&);
template<> void CPersistContext::Transfer(const char*, long&, const long&);
template<> void CPersistContext::Transfer(const char*, bool&, const bool&);

void CTCDummyVillager::OnSerialise(CPersistContext& context, bool)
{
    if (context.IsSaving()) {
        CCharString def_name = GetDefinitionManager().GetDefNameFromGlobalIndex(DefType);
        context.Transfer("DefType", def_name);
        HomeBuildingUID = 0;
        if (IS_THING_ALIVE(HomeBuilding)) {
            HomeBuildingUID = GetThingManager().RemapUIDForSerialisation(true, GetParentThing().IsLevelLess(),
                GetParentThing().GetCurrentMapNumber(), HomeBuilding->GetUniqueID());
        }
        context.Transfer("HomeBuildingUID", HomeBuildingUID);
        WorkBuildingUID = 0;
        if (IS_THING_ALIVE(WorkBuilding)) {
            WorkBuildingUID = GetThingManager().RemapUIDForSerialisation(true, GetParentThing().IsLevelLess(),
                GetParentThing().GetCurrentMapNumber(), WorkBuilding->GetUniqueID());
        }
        context.Transfer("WorkBuildingUID", WorkBuildingUID);
        if (IS_THING_ALIVE(Creature)) {
            CreatureUID = GetThingManager().RemapUIDForSerialisation(true, GetParentThing().IsLevelLess(),
                GetParentThing().GetCurrentMapNumber(), Creature->GetUniqueID());
        }
        context.Transfer("CreatureUID", CreatureUID);
    } else {
        CCharString def_name;
        context.Transfer("DefType", def_name);
        DefType = GetDefinitionManager().GetDefGlobalIndexFromName(def_name);
        context.Transfer("HomeBuildingUID", HomeBuildingUID, 0ui64);
        if (HomeBuildingUID) {
            HomeBuildingUID = GetThingManager().RemapUIDForSerialisation(false, GetParentThing().IsLevelLess(),
                GetParentThing().GetCurrentMapNumber(), HomeBuildingUID);
        }
        context.Transfer("WorkBuildingUID", WorkBuildingUID, 0ui64);
        if (WorkBuildingUID) {
            WorkBuildingUID = GetThingManager().RemapUIDForSerialisation(false, GetParentThing().IsLevelLess(),
                GetParentThing().GetCurrentMapNumber(), WorkBuildingUID);
        }
        context.Transfer("CreatureUID", CreatureUID, 0ui64);
        if (CreatureUID) {
            CreatureUID = GetThingManager().RemapUIDForSerialisation(false, GetParentThing().IsLevelLess(),
                GetParentThing().GetCurrentMapNumber(), CreatureUID);
        }
    }

    context.Transfer("DayToBeRecreated", DayToBeRecreated);
    context.Transfer("FrameToBeRecreated", FrameToBeRecreated);
    context.Transfer("Respawnable", Respawnable, true);
    context.Transfer("Guard", Guard, false);
}
#include <stdio.h>

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

#include <stdlib.h>
#include "engine/CSoundPair.h"
#include "engine/CAnimationEntry.h"

CBaseIntelligentPointer::CBaseIntelligentPointer() : PData(0) {}
CBaseIntelligentPointer::~CBaseIntelligentPointer() {}
CBaseObject* CBaseIntelligentPointer::GetPItem() const { return reinterpret_cast<CBaseObject*>(PData); }
void CBaseIntelligentPointer::SetPItem(CBaseObject* item) { PData = reinterpret_cast<CBaseObjectPointer*>(item); }


static void RequireAt(bool check, int line)
{
    if (!check) {
        printf("check failed on line %d\n", line);
        fflush(0);
        abort();
    }
}
#define Require(check) RequireAt((check), __LINE__)

CCharString::CCharString(const char* text, long length)
    : PStringData(reinterpret_cast<CCharStringData*>(const_cast<char*>(text))) { Require(length == -1); }
CCharString::CCharString(const CCharString& other) : PStringData(other.PStringData) {}
CWideString::CWideString() {}
CWideString::~CWideString() {}
static const char* Text(const CCharString& s)
{
    return s.PStringData ? reinterpret_cast<const char*>(s.PStringData) : "";
}

CResource::CResource() {}
CResource::~CResource() {}
void CIVCountedPointeeBase::Delete() const { abort(); }
CFailedAllocationHandler::~CFailedAllocationHandler() {}
CResourceList::~CResourceList() {}
bool CResourceList::HandleFailedAllocation(unsigned long) { abort(); }
CDefinitionManager::~CDefinitionManager() {}
CGameDefinitionManager::CGameDefinitionManager() {}
CGameDefinitionManager::~CGameDefinitionManager() {}
CClassFactory::CClassFactory(unsigned char) {}
CClassFactory::~CClassFactory() {}
CThingManager::CThingManager(const CMainGameComponent& component, const CGameDefinitionManager& definitions, CWorld& world,
    CPlayerManager& player, CNavigatorManager& nav, unsigned long* seed, unsigned long, unsigned long, unsigned long)
    : CClassFactory(0), Component(component), DefinitionManager(definitions), World(world),
      WorldMap(*reinterpret_cast<CWorldMap*>(&world)), WorldSeed(seed), PlayerManager(player), NavigationManager(nav) {}
CThingManager::~CThingManager() {}

CTCDummyVillager::CTCDummyVillager(CThing& thing) : CTCBase(thing) {}
CTCDummyVillager::~CTCDummyVillager() {}
ETCInterfaceType CTCDummyVillager::GetInterfaceType() const { abort(); }
CCharString CTCDummyVillager::GetClassTypeName() const { abort(); }

static CThing* fixture_parent;
static const CGameDefinitionManager* fixture_definitions;
static CThingManager* fixture_manager;
CDefStringTable GDefStringTable;
static CPersistContext* fixture_context;
static bool level_less;
static unsigned remaps, looked_up;
static unsigned fields;
static const char* field_order[] = {"DefType", "HomeBuildingUID", "WorkBuildingUID", "CreatureUID",
    "DayToBeRecreated", "FrameToBeRecreated", "Respawnable", "Guard"};
static unsigned __int64 recorded_uids[3];
static long recorded_long[2];
static bool recorded_flags[2];
static unsigned __int64 desired_uids[3];
static long desired_long[2];
static bool desired_flags[2];

const CGameDefinitionManager& CTCBase::GetDefinitionManager() const { return *fixture_definitions; }
CThingManager& CTCBase::GetThingManager() { return *fixture_manager; }
bool CThing::IsLevelLess() const { Require(this == fixture_parent); return level_less; }
CDefString CDefinitionManager::GetDefNameFromGlobalIndex(long index) const
{
    Require(this == fixture_definitions && index == 41);
    CDefString name;
    name.TablePos = 29;
    ++looked_up;
    return name;
}
CCharString CDefStringTable::GetString(long position) const
{
    Require(this == &GDefStringTable && position == 29);
    return CCharString("saved-def");
}
long CDefinitionManager::GetDefGlobalIndexFromName(const CCharString& name) const
{
    Require(this == fixture_definitions && strcmp(Text(name), "loaded-def") == 0);
    ++looked_up;
    return 56;
}
unsigned __int64 CThingManager::RemapUIDForSerialisation(bool saving, bool is_level_less, unsigned long map, unsigned __int64 uid) const
{
    Require(this == fixture_manager && saving == fixture_context->IsSaving() && is_level_less == level_less);
    Require(map == static_cast<unsigned long>(fixture_parent->GetCurrentMapNumber()) && uid != 0);
    ++remaps;
    return uid + (saving ? 0x100000000ui64 : 0x200000000ui64);
}

template<> void CPersistContext::Transfer(const char* name, CCharString& value, const CCharString& fallback)
{
    Require(this == fixture_context && fields == 0 && strcmp(name, field_order[fields++]) == 0 && !*Text(fallback));
    if (IsSaving())
        Require(strcmp(Text(value), "saved-def") == 0);
    else {
        Require(!*Text(value));
        value.PStringData = CCharString("loaded-def").PStringData;
    }
}
template<> void CPersistContext::Transfer(const char* name, unsigned __int64& value, const unsigned __int64& fallback)
{
    Require(this == fixture_context && fields >= 1 && fields <= 3 && strcmp(name, field_order[fields]) == 0 && fallback == 0);
    const unsigned index = fields++ - 1;
    if (IsSaving())
        recorded_uids[index] = value;
    else
        value = desired_uids[index];
}
template<> void CPersistContext::Transfer(const char* name, long& value, const long& fallback)
{
    Require(this == fixture_context && fields >= 4 && fields <= 5 && strcmp(name, field_order[fields]) == 0 && fallback == 0);
    const unsigned index = fields++ - 4;
    if (IsSaving())
        recorded_long[index] = value;
    else
        value = desired_long[index];
}
template<> void CPersistContext::Transfer(const char* name, bool& value, const bool& fallback)
{
    Require(this == fixture_context && fields >= 6 && fields <= 7 && strcmp(name, field_order[fields]) == 0 && fallback == (fields == 6));
    const unsigned index = fields++ - 6;
    if (IsSaving())
        recorded_flags[index] = value;
    else
        value = desired_flags[index];
}
static void Reset(CTCDummyVillager& tc, CPersistContext::EMode mode)
{
    fixture_context->Mode = mode;
    tc.DefType = 41;
    tc.HomeBuildingUID = 411;
    tc.WorkBuildingUID = 412;
    tc.CreatureUID = 413;
    tc.DayToBeRecreated = 6;
    tc.FrameToBeRecreated = 8000;
    tc.Respawnable = false;
    tc.Guard = true;
    level_less = false;
    fields = remaps = looked_up = 0;
}
int main()
{
    CThing parent;
    CThingBuilding home, work;
    CThingAICreature creature;
    CGameDefinitionManager def;
    static unsigned __int64 game_storage[4], world_storage[4], player_storage[4], nav_storage[4];
    // The fixture constructor only retains these engine references. The leaf uses the typed definition and thing managers.
    CThingManager manager(*reinterpret_cast<CMainGameComponent*>(game_storage), def, *reinterpret_cast<CWorld*>(world_storage),
        *reinterpret_cast<CPlayerManager*>(player_storage), *reinterpret_cast<CNavigatorManager*>(nav_storage), 0, 0, 0, 0);
    CPersistContext context;
    CTCDummyVillager tc(parent);
    fixture_parent = &parent;
    fixture_definitions = &def;
    fixture_manager = &manager;
    fixture_context = &context;
    parent.CurrentMapNumber = -7;
    home.UniqueID = 22;
    work.UniqueID = 44;
    creature.UniqueID = 88;
    home.Dead = work.Dead = creature.Dead = false;
    tc.HomeBuilding.SetPItem(&home);
    tc.WorkBuilding.SetPItem(&work);
    tc.Creature.SetPItem(&creature);

    Reset(tc, CPersistContext::MODE_SAVE_BINARY);
    tc.OnSerialise(context, true);
    Require(fields == 8 && remaps == 3 && looked_up == 1);
    Require(recorded_uids[0] == 0x100000016ui64 && recorded_uids[1] == 0x10000002cui64 && recorded_uids[2] == 0x100000058ui64);
    Require(recorded_uids[0] == tc.HomeBuildingUID && recorded_uids[1] == tc.WorkBuildingUID && recorded_uids[2] == tc.CreatureUID);
    Require(recorded_long[0] == 6 && recorded_long[1] == 8000 && !recorded_flags[0] && recorded_flags[1]);

    Reset(tc, CPersistContext::MODE_SAVE_TEXT);
    level_less = true;
    tc.HomeBuilding.SetPItem(0);
    work.Dead = true;
    creature.Dead = true;
    tc.OnSerialise(context, false);
    Require(fields == 8 && remaps == 0 && looked_up == 1);
    Require(recorded_uids[0] == 0 && recorded_uids[1] == 0 && recorded_uids[2] == 413);

    Reset(tc, CPersistContext::MODE_LOAD_BINARY);
    desired_uids[0] = 5;
    desired_uids[1] = 6;
    desired_uids[2] = 7;
    desired_long[0] = 9;
    desired_long[1] = 16;
    desired_flags[0] = true;
    desired_flags[1] = false;
    tc.OnSerialise(context, true);
    Require(fields == 8 && remaps == 3 && looked_up == 1 && tc.DefType == 56);
    Require(tc.HomeBuildingUID == 0x200000005ui64 && tc.WorkBuildingUID == 0x200000006ui64 && tc.CreatureUID == 0x200000007ui64);
    Require(tc.DayToBeRecreated == 9 && tc.FrameToBeRecreated == 16 && tc.Respawnable && !tc.Guard);

    Reset(tc, CPersistContext::MODE_LOAD_TEXT);
    level_less = true;
    desired_uids[0] = 0;
    desired_uids[1] = 6;
    desired_uids[2] = 0;
    tc.OnSerialise(context, false);
    Require(fields == 8 && remaps == 1 && looked_up == 1 && tc.DefType == 56);
    Require(tc.HomeBuildingUID == 0 && tc.WorkBuildingUID == 0x200000006ui64 && tc.CreatureUID == 0);

    puts("0072f380_TEST PASS");
}

bool CResource::UnloadResource() { fprintf(stderr, "unexpected CResource::UnloadResource\n"); abort(); }
unsigned long CResource::GetUnloadableMemory() const { fprintf(stderr, "unexpected CResource::GetUnloadableMemory\n"); abort(); }
bool CResource::IsResourceLoaded() const { fprintf(stderr, "unexpected CResource::IsResourceLoaded\n"); abort(); }
void CResource::OnResourceLoaded() { fprintf(stderr, "unexpected CResource::OnResourceLoaded\n"); abort(); }
bool CResource::CanAbortResourceLoad(long) const { fprintf(stderr, "unexpected CResource::CanAbortResourceLoad\n"); abort(); }
void CResource::OnResourceLoadAborted() { fprintf(stderr, "unexpected CResource::OnResourceLoadAborted\n"); abort(); }
unsigned long CResource::GetDebugMemoryInUse() const { fprintf(stderr, "unexpected CResource::GetDebugMemoryInUse\n"); abort(); }
void CResource::ResetDebugMemoryInUse() { fprintf(stderr, "unexpected CResource::ResetDebugMemoryInUse\n"); abort(); }
void CThingGameObject::OnHit(CHitParameters&) { fprintf(stderr, "unexpected CThingGameObject::OnHit\n"); abort(); }
void CThingCreatureBase::OnHit(CHitParameters&) { fprintf(stderr, "unexpected CThingCreatureBase::OnHit\n"); abort(); }
bool CThingCreatureBase::DrawPreparePrimitivesForRendering(const CEnginePrimitivePrepareInfo& info) { fprintf(stderr, "unexpected CThingCreatureBase::DrawPreparePrimitivesForRendering\n"); abort(); }
void CClassFactory::AddNewClassType(const CClassTypeInit&) { fprintf(stderr, "unexpected CClassFactory::AddNewClassType\n"); abort(); }
void CClassFactory::OnCreateClass(CClass&) { fprintf(stderr, "unexpected CClassFactory::OnCreateClass\n"); abort(); }
void CClassFactory::OnDeleteClass(CClass&) { fprintf(stderr, "unexpected CClassFactory::OnDeleteClass\n"); abort(); }
void CThingManager::OnCreateClass(CClass&) { fprintf(stderr, "unexpected CThingManager::OnCreateClass\n"); abort(); }
void CThingManager::OnDeleteClass(CClass&) { fprintf(stderr, "unexpected CThingManager::OnDeleteClass\n"); abort(); }
void CTCDummyVillager::OnCreate() { fprintf(stderr, "unexpected CTCDummyVillager::OnCreate\n"); abort(); }
void CTCDummyVillager::OnInitialActivate() { fprintf(stderr, "unexpected CTCDummyVillager::OnInitialActivate\n"); abort(); }
bool CTCDummyVillager::IsComponentSerialisable() const { fprintf(stderr, "unexpected CTCDummyVillager::IsComponentSerialisable\n"); abort(); }
bool CTCDummyVillager::HasFrameUpdate() const { fprintf(stderr, "unexpected CTCDummyVillager::HasFrameUpdate\n"); abort(); }
void CTCDummyVillager::FrameUpdate() { fprintf(stderr, "unexpected CTCDummyVillager::FrameUpdate\n"); abort(); }
void CGameDefinitionManager::InitAndCompile() { fprintf(stderr, "unexpected CGameDefinitionManager::InitAndCompile\n"); abort(); }