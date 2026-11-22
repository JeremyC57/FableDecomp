// CTCTargetingPlayer::SetPTargetedThing()
// Leaf: ?SetPTargetedThing@CTCTargetingPlayer@@UAEXPAVCThing@@@Z
// Flags: default
#define FABLE_RETAIL_TARGETING_PLAYER_SET_PASS27
#define FABLE_RETAIL_CTHING
#define FABLE_RETAIL_CTCBASE
#define FABLE_RETAIL_CTHING_GETTC_EXPANDED
#define FABLE_RETAIL_CTHING_GETTCOFLEAFTYPE
#define FABLE_RETAIL_CTHING_MOREACCESS
#define FABLE_RETAIL_CINTELLIGENTPOINTER
#define FABLE_RETAIL_CINTELLIGENTPOINTER_DEREFERENCE
#define FABLE_RETAIL_CINTELLIGENTPOINTER_COMPARE_NULL_INT
#define FABLE_RETAIL_CINTELLIGENTPOINTER_GET_ORIGINAL_CHAIN
#define FABLE_RETAIL_CINTELLIGENTPOINTER_FORCEINLINE_CTOR
#define FABLE_RETAIL_CINTELLIGENTPOINTER_ASSIGN
#define FABLE_RETAIL_CINTELLIGENTPOINTER_ASSIGN_FORCEINLINE
#define FABLE_RETAIL_CINTELLIGENTPOINTER_ASSIGN_ORIGINAL_CHAIN
#define FABLE_RETAIL_IS_THING_ALIVE
#define FABLE_RETAIL_CTCTARGETINGPLAYER
#define FABLE_RETAIL_CTCTARGETINGPLAYER_UPDATE_LIST
#define FABLE_RETAIL_UPDATE_NEAREST_INTERESTING
#define FABLE_RETAIL_CTCACTIONUSEREADABLE_PASS24
#define FABLE_RETAIL_CREADABLEDEF_USEINFO_PASS24
#define FABLE_RETAIL_CTCHASNAME_NPCSHORTNAME
#define FABLE_RETAIL_CTCREGIONDISPLAY
#define FABLE_RETAIL_C3DVECTOR
#define FABLE_RETAIL_C2DVECTOR
#define FABLE_RETAIL_CCHARSTRING
#define FABLE_RETAIL_DEF_CLASS_BASE
#define FABLE_RETAIL_CMAINGAMECOMPONENT
#define FABLE_RETAIL_CMAINGAMECOMPONENT_GETPPLAYERINTERFACE
#define FABLE_RETAIL_CGAMEPLAYERINTERFACE_TOGGLE_ZTARGET
#define FABLE_RETAIL_PLAYER
#define FABLE_RETAIL_PLAYER_REMOVEMODE
#define FABLE_RETAIL_CPLAYERGUI
#define FABLE_RETAIL_CPLAYERGUI_NPCNAMEDISPLAY
#define FABLE_RETAIL_CSUBTITLERENDERER_INCUTSCENE
#define FABLE_RETAIL_TARGETING_PLAYER_TEST_PASS27
#define FABLE_RETAIL_CTCACTIONUSEBASE_DTOR
#define FABLE_RETAIL_CGAME_CORE_PASS23
#define FABLE_RETAIL_CHASNAMEDEF
#define FABLE_RETAIL_DEF_CLASS_BASE_CTOR_DECL
#define FABLE_RETAIL_CIVCOUNTEDPOINTEEBASE_CTOR
#define FABLE_RETAIL_STLP_BITSET_INDEX
#define FABLE_RETAIL_CDEFSTRINGTABLE
#define FABLE_RETAIL_CDEFSTRING
#define FABLE_RETAIL_CWIDESTRING
#define private public
#define protected public
#include "engine/CThing.h"
#include "engine/CTargetingDef.h"
#include "engine/CTCTargetingPlayer.h"
#include "engine/CTCActionUseReadable.h"
#include "engine/CTCRegionDisplay.h"
#include "engine/CTCHasName.h"
#include "engine/ThingAlive.h"
#include "engine/CGamePlayerInterface.h"
#include "engine/CMainGameComponent.h"
#include "engine/EPlayerMode.h"
#include "engine/CPlayer.h"
#include "engine/CPlayerGui.h"
#include "engine/CDefString.h"
#include "engine/CHasNameDef.h"
#include "engine/CGame.h"
#include "engine/CMainGameComponentInit.h"
#undef private
#undef protected

void CTCTargetingPlayer::SetPTargetedThing(CThing* pthing)
{
    if (CMainGameComponent::Get()->GetPPlayerInterface()->IsToggleZTarget()) {
        if (!pthing || pthing != GetPTargetLockedThing()) {
            if (GetParentThing().GetPlayer().HasMode(PLAYER_MODE_Z_TARGET))
                GetParentThing().GetPlayer().RemoveMode(PLAYER_MODE_Z_TARGET);
        }
    }

    if (pthing != PTargetedThing.Get()) {
        if (IS_THING_ALIVE(PTargetedThing)) {
            const CTCActionUseReadable* ptc_readable;
            if (PTargetedThing->GetTCOfLeafType(&ptc_readable)) {
                if (ptc_readable->PeekDef().UseInfoDisplay) {
                    CTCRegionDisplay* ptc_region_display;
                    GetParentThing().GetTC(&ptc_region_display);
                    if (ptc_region_display)
                        ptc_region_display->SetCurrentAreaName("", -1.0f, false, false, true);
                }
            }
        }

        if (pthing != GetPTargetLockedThing()) {
            PTargetLockedThing = CIntelligentPointer<CThing>(0);
            LockedOntoEnemy = false;
        }
        CTCTargetingBase::SetPTargetedThing(pthing);

        if (IS_THING_ALIVE(PTargetedThing)) {
            const CTCActionUseReadable* ptc_readable;
            if (PTargetedThing->GetTCOfLeafType(&ptc_readable)) {
                if (ptc_readable->PeekDef().UseInfoDisplay) {
                    CTCRegionDisplay* ptc_region_display;
                    GetParentThing().GetTC(&ptc_region_display);
                    if (ptc_region_display)
                        ptc_region_display->SetCurrentAreaName(ptc_readable->GetTextTag(), 0.0f, true, false, true);
                }
            }
        }

        if (IS_THING_ALIVE(PTargetedThing) && !CPlayerGui::Get().GetSubtitleRenderer().IsInCutscene()) {
            CTCHasName* ptc_has_name;
            if (PTargetedThing->GetTC(&ptc_has_name))
                CPlayerGui::Get().SetNPCNameDisplay(ptc_has_name->GetNPCNameAndFullTitle());
            else
                CPlayerGui::Get().SetNPCNameDisplay(CWideString());
        } else {
            CPlayerGui::Get().SetNPCNameDisplay(CWideString());
        }
    }
}
#include <stdio.h>

CBaseClass::CBaseClass() {}
CBaseClass::~CBaseClass() {}
CBaseObject::CBaseObject() : IntelligentPointer(0) {}
CBaseObject::~CBaseObject() {}
void CBaseObject::Validate() const {}
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

#include <stdlib.h>
#include "engine/CSoundPair.h"
#include "engine/CAnimationEntry.h"

CBaseIntelligentPointer::CBaseIntelligentPointer() : PData(0) {}
CBaseIntelligentPointer::~CBaseIntelligentPointer() {}
CBaseObject* CBaseIntelligentPointer::GetPItem() const { return reinterpret_cast<CBaseObject*>(PData); }
void CBaseIntelligentPointer::SetPItem(CBaseObject* item) { PData = reinterpret_cast<CBaseObjectPointer*>(item); }


void CIVCountedPointeeBase::Delete() const { abort(); }
CResource::CResource() {}
CResource::~CResource() {}
CDefClassBase::CDefClassBase() {}
CSubDefClassBase::CSubDefClassBase() {}
CInitBaseClass::CInitBaseClass() {}
CInitBaseClass::~CInitBaseClass() {}
CGame::CGame() {}
CGame::~CGame() {}
CMainGameComponentInit::CMainGameComponentInit() {}
CMainGameComponentInit::~CMainGameComponentInit() {}
CGameComponent::CGameComponent(CGame& game) : Game(game) {}
CGameComponent::~CGameComponent() {}
CMainGameComponent::CMainGameComponent(CGame& game, const CMainGameComponentInit&) : CGameComponent(game) {}
CMainGameComponent::~CMainGameComponent() {}
CMainGameComponent* CMainGameComponent::PMainGameComponent = 0;

template<> CCountedPointer<CPlayerGui> CTBaseSingletonNoDefaultAlloc<CPlayerGui>::PClass;
template<> CTBaseSingletonNoDefaultAlloc<CPlayerGui>::~CTBaseSingletonNoDefaultAlloc() {}

static void RequireAt(bool check, int line)
{
    if (!check) {
        printf("check failed on line %d\n", line);
        fflush(0);
        abort();
    }
}
#define Require(check) RequireAt((check), __LINE__)

CCharString::CCharString(const char* text, long length) : PStringData(new CCharStringData)
{
    Require(length == -1);
    PStringData->Data.PData = const_cast<char*>(text);
    PStringData->Data.StringLength = static_cast<unsigned long>(strlen(text));
}
CCharString::CCharString(const CCharString& other)
    : PStringData(other.PStringData ? new CCharStringData(*other.PStringData) : 0) {}
CWideString::CWideString() : PStringData(0) {}
CWideString::CWideString(const CWideString& other) : PStringData(other.PStringData) {}
CWideString::~CWideString() {}
template<> long __fastcall CBasicString<char>::Compare(const char* a, const char* b) { return strcmp(a, b); }
static const char* Text(const CCharString& s) { return s.PStringData ? s.PStringData->Data.PeekData() : ""; }
static const wchar_t* Text(const CWideString& s) { return reinterpret_cast<const wchar_t*>(s.PStringData); }
static void Write(CWideString& s, const wchar_t* p) { s.PStringData = reinterpret_cast<CWideStringData*>(const_cast<wchar_t*>(p)); }

CTCTargetingBase::CTCTargetingBase(CThing& thing) : CTCBase(thing) {}
CTCTargetingBase::~CTCTargetingBase() {}
CTCTargetingPlayer::CTCTargetingPlayer(CThing& thing) : CTCTargetingBase(thing) {}
CTCActionUseBase::CTCActionUseBase(CThing& thing) : CTCBase(thing) {}
CTCActionUseBase::~CTCActionUseBase() {}
CTCActionUseReadable::CTCActionUseReadable(CThing& thing) : CTCActionUseBase(thing) {}
CTCHasName::CTCHasName(CThing& thing) : CTCBase(thing) {}
CTCRegionDisplay::CTCRegionDisplay(CThing& thing) : CTCBase(thing) {}
ETCInterfaceType CTCTargetingBase::GetInterfaceType() const { return TCI_TARGETING; }
CThing* CTCTargetingPlayer::GetPTargetLockedThing() const { return PTargetLockedThing.Get(); }
CThing* CTCTargetingPlayer::GetPTargetedThing() const { return PTargetedThing.Get(); }

class TestTargeting : public CTCTargetingPlayer {
public:
    TestTargeting(CThing& thing) : CTCTargetingPlayer(thing) {}
    CCharString GetClassTypeName() const { return CCharString("CTCTargetingPlayer"); }
};
class TestReadable : public CTCActionUseReadable {
public:
    TestReadable(CThing& thing) : CTCActionUseReadable(thing), Accept(true) {}
    ETCInterfaceType GetInterfaceType() const { return TCI_ON_ACTION_USE; }
    CCharString GetClassTypeName() const { return CCharString(Accept ? "CTCActionUseReadable" : "CTCActionUseWindow"); }
    bool Accept;
};
class TestRegion : public CTCRegionDisplay {
public:
    TestRegion(CThing& thing) : CTCRegionDisplay(thing) {}
    ETCInterfaceType GetInterfaceType() const { return TCI_REGION_DISPLAY; }
    CCharString GetClassTypeName() const { return CCharString("CTCRegionDisplay"); }
};
class TestHasName : public CTCHasName {
public:
    TestHasName(CThing& thing) : CTCHasName(thing) {}
    ETCInterfaceType GetInterfaceType() const { return TCI_HAS_NAME; }
    CCharString GetClassTypeName() const { return CCharString("CTCHasName"); }
};
class TestReadableDef : public CReadableDef {
public:
    void Transfer(CPersistContext&) { abort(); }
    void Copy(const CDefClassBase*) { abort(); }
    unsigned long GetSizeofClass() { return sizeof(*this); }
};

static CPlayer player;
static CGamePlayerInterface input;
static CPlayerGui gui;
static CThing* parent;
static CThing* old_target;
static CThing* new_target;
static TestReadable* old_readable;
static TestReadable* new_readable;
static TestReadableDef* readable_def;
static TestRegion* region;
static TestHasName* named;
static bool toggle_mode, player_has_mode;
static int removals, old_clears, new_sets, changes, displays, name_queries;
static const wchar_t readable_tag[] = L"readable";
static const wchar_t full_title[] = L"Target and title";
static const wchar_t* displayed;

CGamePlayerInterface* CMainGameComponent::GetPPlayerInterface() { return &input; }
bool CGamePlayerInterface::IsToggleZTarget() const { return toggle_mode; }
CPlayer& CThing::GetPlayer() { Require(this == parent); return player; }
bool CPlayer::HasMode(EPlayerMode mode) const
{
    Require(this == &player && mode == PLAYER_MODE_Z_TARGET);
    return player_has_mode;
}
void CPlayer::RemoveMode(EPlayerMode mode)
{
    Require(this == &player && mode == PLAYER_MODE_Z_TARGET);
    player_has_mode = false;
    ++removals;
}
void CTCTargetingBase::SetPTargetedThing(CThing* thing)
{
    ++changes;
    PTargetedThing.SetPItem(thing);
}
void CTCRegionDisplay::SetCurrentAreaName(const CCharString& text, float duration, bool info, bool show, bool paused)
{
    Require(this == region && !*Text(text) && duration == -1.0f && !info && !show && paused);
    ++old_clears;
}
void CTCRegionDisplay::SetCurrentAreaName(const CWideString& text, float duration, bool info, bool show, bool paused)
{
    Require(this == region && Text(text) == readable_tag && duration == 0.0f && info && !show && paused);
    ++new_sets;
}
CWideString CTCHasName::GetNPCNameAndFullTitle()
{
    Require(this == named);
    ++name_queries;
    CWideString name;
    Write(name, full_title);
    return name;
}
void CPlayerGui::SetNPCNameDisplay(const CWideString& text)
{
    Require(this == &gui);
    ++displays;
    displayed = Text(text);
}

static void Put(CThing& thing, ETCInterfaceType key, CTCBase* tc)
{
    thing.UsingComponents[key] = true;
    thing.Components.push_back(std::pair<ETCInterfaceType, CTCBase*>(key, tc));
}
static void Reset(TestTargeting& targeting)
{
    targeting.PTargetedThing.SetPItem(old_target);
    targeting.PTargetLockedThing.SetPItem(old_target);
    targeting.LockedOntoEnemy = true;
    old_target->Dead = new_target->Dead = false;
    old_readable->Accept = new_readable->Accept = true;
    readable_def->UseInfoDisplay = true;
    new_target->UsingComponents[TCI_HAS_NAME] = true;
    parent->Components.begin()->second = region;
    toggle_mode = player_has_mode = true;
    gui.GetSubtitleRenderer().InCutscene = false;
    removals = old_clears = new_sets = changes = displays = name_queries = 0;
    displayed = 0;
}
static void Check(TestTargeting& targeting, CThing* value, int remove, int clear, int set, int change, int show, bool title, bool locked)
{
    targeting.SetPTargetedThing(value);
    Require(removals == remove && old_clears == clear && new_sets == set && changes == change);
    Require(displays == show && name_queries == (title ? 1 : 0) && displayed == (title ? full_title : 0));
    Require(targeting.PTargetedThing.Get() == value);
    Require(targeting.LockedOntoEnemy == locked);
    Require(targeting.PTargetLockedThing.Get() == (locked ? old_target : 0));
}
int main()
{
    CGame game;
    CMainGameComponentInit init;
    CMainGameComponent component(game, init);
    CMainGameComponent::PMainGameComponent = &component;
    CTBaseSingletonNoDefaultAlloc<CPlayerGui>::PClass.Data = &gui;

    CThing hero, old_thing, next_thing;
    parent = &hero;
    old_target = &old_thing;
    new_target = &next_thing;
    memset(hero.UsingComponents._M_w, 0, sizeof(hero.UsingComponents._M_w));
    memset(old_thing.UsingComponents._M_w, 0, sizeof(old_thing.UsingComponents._M_w));
    memset(next_thing.UsingComponents._M_w, 0, sizeof(next_thing.UsingComponents._M_w));
    TestRegion reg(hero);
    TestReadableDef def;
    TestReadable old_component(old_thing), new_component(next_thing);
    TestHasName name_component(next_thing);
    TestTargeting targeting(hero);
    region = &reg;
    old_readable = &old_component;
    new_readable = &new_component;
    named = &name_component;
    readable_def = &def;
    def.AddRef();
    def.AddRef();
    old_component.PDef.Object = new_component.PDef.Object = &def;
    Write(new_component.TextTag, readable_tag);
    Put(hero, TCI_REGION_DISPLAY, &reg);
    Put(old_thing, TCI_ON_ACTION_USE, &old_component);
    Put(next_thing, TCI_ON_ACTION_USE, &new_component);
    Put(next_thing, TCI_HAS_NAME, &name_component);

    Reset(targeting); Check(targeting, old_target, 0, 0, 0, 0, 0, false, true);
    Reset(targeting); Check(targeting, new_target, 1, 1, 1, 1, 1, true, false);
    Reset(targeting); Check(targeting, 0, 1, 1, 0, 1, 1, false, false);
    Reset(targeting); toggle_mode = false; Check(targeting, new_target, 0, 1, 1, 1, 1, true, false);
    Reset(targeting); player_has_mode = false; Check(targeting, new_target, 0, 1, 1, 1, 1, true, false);
    Reset(targeting); readable_def->UseInfoDisplay = false; Check(targeting, new_target, 1, 0, 0, 1, 1, true, false);
    Reset(targeting); old_readable->Accept = false; Check(targeting, new_target, 1, 0, 1, 1, 1, true, false);
    Reset(targeting); new_readable->Accept = false; Check(targeting, new_target, 1, 1, 0, 1, 1, true, false);
    Reset(targeting); old_target->Dead = true; new_target->Dead = true; Check(targeting, new_target, 1, 0, 0, 1, 1, false, false);
    Reset(targeting); new_target->UsingComponents[TCI_HAS_NAME] = false; Check(targeting, new_target, 1, 1, 1, 1, 1, false, false);
    Reset(targeting); gui.GetSubtitleRenderer().InCutscene = true; Check(targeting, new_target, 1, 1, 1, 1, 1, false, false);
    Reset(targeting); hero.Components.begin()->second = 0; Check(targeting, new_target, 1, 0, 0, 1, 1, true, false);
    CTBaseSingletonNoDefaultAlloc<CPlayerGui>::PClass.Data = 0;
    CMainGameComponent::PMainGameComponent = 0;
    puts("00802e00_TEST PASS");
}

bool CResource::UnloadResource() { fprintf(stderr, "unexpected CResource::UnloadResource\n"); abort(); }
unsigned long CResource::GetUnloadableMemory() const { fprintf(stderr, "unexpected CResource::GetUnloadableMemory\n"); abort(); }
bool CResource::IsResourceLoaded() const { fprintf(stderr, "unexpected CResource::IsResourceLoaded\n"); abort(); }
void CResource::OnResourceLoaded() { fprintf(stderr, "unexpected CResource::OnResourceLoaded\n"); abort(); }
bool CResource::CanAbortResourceLoad(long) const { fprintf(stderr, "unexpected CResource::CanAbortResourceLoad\n"); abort(); }
void CResource::OnResourceLoadAborted() { fprintf(stderr, "unexpected CResource::OnResourceLoadAborted\n"); abort(); }
unsigned long CResource::GetDebugMemoryInUse() const { fprintf(stderr, "unexpected CResource::GetDebugMemoryInUse\n"); abort(); }
void CResource::ResetDebugMemoryInUse() { fprintf(stderr, "unexpected CResource::ResetDebugMemoryInUse\n"); abort(); }
bool CDefClassBase::UnloadResource() { fprintf(stderr, "unexpected CDefClassBase::UnloadResource\n"); abort(); }
CDefString CDefClassBase::GetInstantiationName() const { fprintf(stderr, "unexpected CDefClassBase::GetInstantiationName\n"); abort(); }
void CDefClassBase::SetInstantiationName(CDefString) { fprintf(stderr, "unexpected CDefClassBase::SetInstantiationName\n"); abort(); }
void CDefClassBase::AddSubDef(const CCharString&, long, long) { fprintf(stderr, "unexpected CDefClassBase::AddSubDef\n"); abort(); }
void CDefClassBase::RemoveSubDef(const CCharString&) { fprintf(stderr, "unexpected CDefClassBase::RemoveSubDef\n"); abort(); }
CDefClassBase::CSubDefInfo* CDefClassBase::GetPSubDefInfo(const CCharString&) { fprintf(stderr, "unexpected CDefClassBase::GetPSubDefInfo\n"); abort(); }
const CDefClassBase::CSubDefInfo* CDefClassBase::GetPSubDefInfo(const CCharString&) const { fprintf(stderr, "unexpected CDefClassBase::GetPSubDefInfo\n"); abort(); }
bool CDefClassBase::IsParentDef() const { fprintf(stderr, "unexpected CDefClassBase::IsParentDef\n"); abort(); }
void CDefClassBase::PostTransfer() { fprintf(stderr, "unexpected CDefClassBase::PostTransfer\n"); abort(); }
void CDefClassBase::InternalSaveBinary(CDataOutputStream&) { fprintf(stderr, "unexpected CDefClassBase::InternalSaveBinary\n"); abort(); }
void CDefClassBase::InternalLoadBinary(CDataInputStream&) { fprintf(stderr, "unexpected CDefClassBase::InternalLoadBinary\n"); abort(); }
void CDefClassBase::Validate(const CValidateDefinitionInitBase&, const CCharString&) const { fprintf(stderr, "unexpected CDefClassBase::Validate\n"); abort(); }
CVectorMap<unsigned long, CDefClassBase::CSubDefInfo, CKeyPairCompareLess<unsigned long, CDefClassBase::CSubDefInfo> >* CDefClassBase::GetPSubDefInfoMap() { fprintf(stderr, "unexpected CDefClassBase::GetPSubDefInfoMap\n"); abort(); }
void CGameComponent::Init() { fprintf(stderr, "unexpected CGameComponent::Init\n"); abort(); }
void CGameComponent::SetQuit() { fprintf(stderr, "unexpected CGameComponent::SetQuit\n"); abort(); }
bool CMainGameComponent::Run(CGameComponent**) { fprintf(stderr, "unexpected CMainGameComponent::Run\n"); abort(); }
void CMainGameComponent::ChangeTextureColourDepth(long) { fprintf(stderr, "unexpected CMainGameComponent::ChangeTextureColourDepth\n"); abort(); }
void CMainGameComponent::OnPreDeviceReset() { fprintf(stderr, "unexpected CMainGameComponent::OnPreDeviceReset\n"); abort(); }
bool CMainGameComponent::OnPostDeviceReset() { fprintf(stderr, "unexpected CMainGameComponent::OnPostDeviceReset\n"); abort(); }
void CTCTargetingBase::OnInitialActivate() { fprintf(stderr, "unexpected CTCTargetingBase::OnInitialActivate\n"); abort(); }
void CTCTargetingBase::BulletTimeFrameUpdate() { fprintf(stderr, "unexpected CTCTargetingBase::BulletTimeFrameUpdate\n"); abort(); }
CThing * CTCTargetingBase::GetPBestThingForMeleeAttack() const { fprintf(stderr, "unexpected CTCTargetingBase::GetPBestThingForMeleeAttack\n"); abort(); }
CThingCreatureBase * CTCTargetingBase::GetPBestEnemyCreatureForAttack() const { fprintf(stderr, "unexpected CTCTargetingBase::GetPBestEnemyCreatureForAttack\n"); abort(); }
CThing * CTCTargetingBase::GetPBestThingForProjectileTargetingLock() const { fprintf(stderr, "unexpected CTCTargetingBase::GetPBestThingForProjectileTargetingLock\n"); abort(); }
CThingCreatureBase * CTCTargetingBase::GetPBestCreatureForInteractionTypeWithinRangeLimit(ECreatureInteractionType p0, float p1, float p2) const { fprintf(stderr, "unexpected CTCTargetingBase::GetPBestCreatureForInteractionTypeWithinRangeLimit\n"); abort(); }
CThingCreatureBase * CTCTargetingBase::GetPBestCreatureForInteractionType(ECreatureInteractionType p0) const { fprintf(stderr, "unexpected CTCTargetingBase::GetPBestCreatureForInteractionType\n"); abort(); }
CThing * CTCTargetingBase::GetPTargetedThing() const { fprintf(stderr, "unexpected CTCTargetingBase::GetPTargetedThing\n"); abort(); }
CThing * CTCTargetingBase::GetPProjectileTargetedThing() const { fprintf(stderr, "unexpected CTCTargetingBase::GetPProjectileTargetedThing\n"); abort(); }
CThing * CTCTargetingBase::GetPTargetLockedThing() const { fprintf(stderr, "unexpected CTCTargetingBase::GetPTargetLockedThing\n"); abort(); }
bool CTCTargetingBase::GetTargetedPos(C3DVector & p0, float p1) const { fprintf(stderr, "unexpected CTCTargetingBase::GetTargetedPos\n"); abort(); }
bool CTCTargetingBase::IsTargetedThingInRange(unsigned long p0) const { fprintf(stderr, "unexpected CTCTargetingBase::IsTargetedThingInRange\n"); abort(); }
void CTCTargetingBase::UpdateNearestInterestingThing() { fprintf(stderr, "unexpected CTCTargetingBase::UpdateNearestInterestingThing\n"); abort(); }