// Execute the full typed update with real coin and definition lifetimes.
#define protected public
#define private public
#define FABLE_RETAIL_DEF_CLASS_BASE_CTORS
#define FABLE_RETAIL_CDEFCLASSBASE_CTOR_SUBDEF_FRIEND
#define FABLE_RETAIL_STLP_BITSET_INDEX
#define FABLE_RETAIL_CDEFSTRINGTABLE
#define FABLE_RETAIL_SHOVE_FRAME_PASS23
#define FABLE_RETAIL_CTCSOUNDPLAYER_CTOR_PASS23
// CTCTavernGameCoinBase::UpdateCoins
// Leaf: ?UpdateCoins@CTCTavernGameCoinBase@@QAEXM@Z
// Flags: /Oi
// Bracket: grouping the angular friction scale preserves the retail x87 cleanup at 0x008EEC56.
#define FABLE_RETAIL_COIN_UPDATE_PASS27
#define FABLE_RETAIL_CDEFINITIONMANAGER
#define FABLE_RETAIL_CRESOURCELIST
#define FABLE_RETAIL_CTCTAVERNGAME
#define FABLE_RETAIL_CCHARSTRING
#define FABLE_RETAIL_CWIDESTRING
#define FABLE_RETAIL_BASE_OBJECT
#define FABLE_RETAIL_CCLASS
#define FABLE_RETAIL_CTHING
#define FABLE_RETAIL_CTHING_GETTC_EXPANDED
#define FABLE_RETAIL_CTHING_ORIGINAL_VTABLE
#define FABLE_RETAIL_CTHING_MOREACCESS
#define FABLE_RETAIL_C3DVECTOR
#define FABLE_RETAIL_C3DVECTOR_PACKED
#define FABLE_RETAIL_C2DVECTOR
#define FABLE_RETAIL_C2DVECTOR_NORMALISE
#define FABLE_RETAIL_C2DVECTOR_DIVASSIGN_PASS26
#define FABLE_RETAIL_CRIGHTHANDEDSET
#define FABLE_RETAIL_DEF_CLASS_BASE
#define FABLE_RETAIL_CTCSOUNDPLAYER
#define FABLE_RETAIL_IS_THING_ALIVE
#define FABLE_RETAIL_CINTELLIGENTPOINTER
#define FABLE_RETAIL_CINTELLIGENTPOINTER_COPY
#define FABLE_RETAIL_CINTELLIGENTPOINTER_ASSIGN
#define FABLE_RETAIL_CINTELLIGENTPOINTER_COPY_ORIGINAL_CHAIN
#define FABLE_RETAIL_CINTELLIGENTPOINTER_GET_ORIGINAL_CHAIN
#define FABLE_RETAIL_CINTELLIGENTPOINTER_DEREFERENCE
#define FABLE_RETAIL_CINTELLIGENTPOINTER_COMPARE_NULL_INT
#define FABLE_RETAIL_MASS_BACKPLANEFRICTION
#include "engine/CTCTavernGameCoinBase.h"
#include "engine/CTavernGameCoinBaseDef.h"
#include "engine/CTCSoundPlayer.h"
#include "engine/ThingAlive.h"
#include "engine/CDefinitionManager.h"
#include <math.h>

#ifdef _MSC_VER
__declspec(noinline)
#endif
void Mass::BackplaneFriction(float strength, float radius)
{
    float speed = Vel.GetMagnitude();
    float factor = speed <= strength ? 0.0f : 1.0f - strength / speed;
    Vel *= factor;
    strength /= radius;
    speed = fabsf(AVel);
    factor = speed <= strength ? 0.0f : (1.0f - (strength / speed));
    AVel *= factor;
}

template<> inline bool CDefClassBase::GetSubDef<CTavernGameCoinBaseDef>(CDefPointer<const CTavernGameCoinBaseDef>* out) const
{
    const CSubDefInfo* info;
    {
        CCharString name("CTavernGameCoinBaseDef", -1);
        info = GetPSubDefInfo(name);
    }
    if (info) {
        CDefPointer<const CTavernGameCoinBaseDef> result(PDefManager->GetPDefFromGlobalIndex(info->DefIndex));
        *out = result;
        return true;
    }
    return false;
}

void CTCTavernGameCoinBase::UpdateCoins(float time_delta)
{
    CDefPointer<const CTavernGameCoinBaseDef> pdef;
    GetParentThing().PeekBaseDef().GetSubDef(&pdef);
    for (std::vector<Coin>::iterator i = Coins.begin(); i != Coins.end(); ++i) {
        i->Update(time_delta);
        i->BackplaneFriction(pdef->CoinSurfaceFriction * time_delta, i->Radius);
    }
    for (std::vector<Coin>::iterator i = Coins.begin(); i != Coins.end(); ++i) {
        if (!IS_THING_ALIVE((*i).Thing)) {
            SetState(ETGS_LEAVE);
            return;
        }
        if (IS_THING_ALIVE((*i).Thing) && !(*i).Thing->DrawIsDrawable() && !((*i).Thing->GetTypeFlags() & 0x8000))
            continue;
        for (std::vector<Coin>::iterator j = Coins.begin(); j != i; ++j) {
            if (!IS_THING_ALIVE((*j).Thing)) {
                SetState(ETGS_LEAVE);
                return;
            }
            if (IS_THING_ALIVE((*j).Thing) && !(*j).Thing->DrawIsDrawable() && !((*j).Thing->GetTypeFlags() & 0x8000))
                continue;
            if (i->IMass == 0.0f && j->IMass == 0.0f)
                continue;
            C2DVector dir = i->Pos - j->Pos;
            float dist = dir.GetSquaredMagnitude();
            if (dist == 0.0f)
                continue;
            if (dist < (i->Radius + j->Radius) * (i->Radius + j->Radius)) {
                dist = sqrtf(dist);
                dir /= dist;
                if (Mass::Collide(&*i, &*j, i->Pos - dir * i->Radius, j->Pos + dir * j->Radius, dir,
                                  pdef->CoinEdgeRestitution, pdef->CoinEdgeFriction, 0.0f, time_delta)) {
                    CIntelligentPointer<CThing> thing(i->Thing);
                    if (thing == 0)
                        thing = j->Thing;
                    if (thing != 0) {
                        CTCSoundPlayer* sound_player;
                        thing->GetTC(&sound_player);
                        const CCharString hit("TAVERN_GAME_COIN_HIT_COIN");
                        sound_player->PlayAttachedCriteria(hit, true, false, false, -1);
                    }
                }
            }
        }
    }
}

#undef private
#undef protected
#include "engine/CThingBaseDef.h"
#include "engine/CDefString.h"
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

#include <wchar.h>
static const char* last_string;
CCharString::CCharString(const char* text, long) : PStringData(0) { last_string = text; }
CWideString::CWideString() : PStringData(0) {}
CWideString::~CWideString() {}

CSubDefClassBase::CSubDefClassBase() {}
void CTavernGameCoinBaseDef::Transfer(CPersistContext&) { abort(); }
void CTavernGameCoinBaseDef::Copy(const CDefClassBase*) { abort(); }
unsigned long CTavernGameCoinBaseDef::GetSizeofClass() { return sizeof(*this); }
CResourceList::~CResourceList() {}
bool CResourceList::HandleFailedAllocation(unsigned long) { abort(); }
CFailedAllocationHandler::~CFailedAllocationHandler() {}
CDefinitionManager::~CDefinitionManager() {}
static CDefinitionManager manager;
static CTavernGameCoinBaseDef coin_definition;
static int lookups;
CDefPointer<CDefClassBase> CDefinitionManager::GetPDefFromGlobalIndex(long id)
{
    if (id != 17) abort();
    ++lookups;
    CDefPointer<CDefClassBase> def;
    def.Object = &coin_definition;
    coin_definition.AddRef();
    return def;
}
class BoardDefinition : public CThingBaseDef {
public:
    BoardDefinition() { PDefManager = &manager; record.DefIndex = 17; }
    void Transfer(CPersistContext&) { abort(); }
    void Copy(const CDefClassBase*) { abort(); }
    unsigned long GetSizeofClass() { return sizeof(*this); }
    const CSubDefInfo* GetPSubDefInfo(const CCharString&) const
    {
        if (strcmp(last_string, "CTavernGameCoinBaseDef")) abort();
        return &record;
    }
private:
    CSubDefInfo record;
};

CBaseIntelligentPointer::CBaseIntelligentPointer() : PData(0) {}
CBaseIntelligentPointer::~CBaseIntelligentPointer() {}
CBaseObject* CBaseIntelligentPointer::GetPItem() const { return reinterpret_cast<CBaseObject*>(PData); }
void CBaseIntelligentPointer::SetPItem(CBaseObject* thing) { PData = reinterpret_cast<CBaseObjectPointer*>(thing); }
Mass::Mass() : Pos(0, 0), Vel(0, 0), IMass(1.0f), APos(0), AVel(0), AIMass(1) {}
Mass::~Mass() {}
Coin::Coin(float x, float radius, CThing* thing, bool owned)
    : Owned(owned), Thing(thing), Radius(radius), Owner(0), Sound(0) { Pos.X = x; }
Coin::Coin(const Coin& other) : Mass(other), Owned(other.Owned), Thing(other.Thing),
    Radius(other.Radius), Owner(other.Owner), Sound(other.Sound) {}

static bool drawable = true, contact_ok, report_contact;
static int state_calls, collision_calls, sound_calls;
static CTCSoundPlayer* expected_sound;
void CTCTavernGame::SetState(ETavernGameStates state)
{
    if (state != ETGS_LEAVE) abort();
    ++state_calls;
}
bool Mass::Collide(Mass* a, Mass* b, C2DVector a_contact, C2DVector b_contact, C2DVector normal,
                   float restitution, float friction, float separate, float time_delta)
{
    ++collision_calls;
    contact_ok = a != b && a->Pos.X == 1.5f && b->Pos.X == 0.0f && a_contact.X == 0.5f && b_contact.X == 1.0f
        && normal.X == 1.0f && normal.Y == 0.0f && restitution == 0.5f && friction == 0.25f && separate == 0 && time_delta == 0;
    return report_contact;
}
class SoundFixture : public CTCSoundPlayer {
public:
    explicit SoundFixture(CThing& thing) : CTCSoundPlayer(thing) {}
    ETCInterfaceType GetInterfaceType() const { return TCI_SOUND_PLAYER; }
    CCharString GetClassTypeName() const { return CCharString(); }
};
CTCSoundPlayer::CTCSoundPlayer(CThing& thing) : CTCBase(thing) {}
unsigned long CTCSoundPlayer::PlayAttachedCriteria(const CCharString&, bool a, bool b, bool c, long d)
{
    if (this != expected_sound || !a || b || c || d != -1 || strcmp(last_string, "TAVERN_GAME_COIN_HIT_COIN")) abort();
    ++sound_calls;
    return 1;
}
CTCTavernGame::CTCTavernGame(CThing& thing) : CTCBase(thing) {}
CTCTavernGame::~CTCTavernGame() {}
CTCTavernGameCoinBase::CTCTavernGameCoinBase(CThing& thing)
    : CTCTavernGame(thing), Arrow(0), Arrow1(0), Arrow2(0), Held(0) {}

class CoinGame : public CTCTavernGameCoinBase {
public:
    explicit CoinGame(CThing& thing) : CTCTavernGameCoinBase(thing) {}
    CCharString GetClassTypeName() const { return CCharString(); }
    CWideString GetBestScore() { abort(); }
    CWideString GetCurrentScore() { abort(); }
    CWideString GetRequiredScore() { abort(); }
};

int main()
{
    BoardDefinition def;
    CThing parent, first, second;
    parent.PBaseDef.Object = &def;
    def.AddRef();
    first.Dead = false;
    second.Dead = false;
    first.ThingTypeFlags = 0;
    second.ThingTypeFlags = 0;
    memset(second.UsingComponents._M_w, 0, sizeof(second.UsingComponents._M_w));
    SoundFixture sound(second);
    expected_sound = &sound;
    second.UsingComponents[TCI_SOUND_PLAYER] = true;
    second.Components.push_back(std::pair<ETCInterfaceType, CTCBase*>(TCI_SOUND_PLAYER, &sound));
    coin_definition.CoinSurfaceFriction = 0.0f;
    coin_definition.CoinEdgeRestitution = 0.5f;
    coin_definition.CoinEdgeFriction = 0.25f;
    CoinGame game(parent);
    game.UpdateCoins(0);
    if (lookups != 1 || coin_definition.GetRefCount() != 1 || collision_calls || state_calls) return 1;
    game.Coins.push_back(Coin(0, 1, &first, false));
    game.Coins[0].Vel.X = 2.0f;
    game.Coins[0].AVel = 3.0f;
    game.UpdateCoins(0.5f);
    if (game.Coins[0].Pos.X != 1.0f || game.Coins[0].APos != 1.5f || collision_calls) return 2;
    game.Coins[0].Pos.X = 0;
    game.Coins.push_back(Coin(1.5f, 1, &second, false));
    report_contact = false;
    game.UpdateCoins(0);
    if (collision_calls != 1 || !contact_ok || sound_calls) return 3;
    report_contact = true;
    game.UpdateCoins(0);
    if (collision_calls != 2 || !contact_ok || sound_calls != 1) return 4;
    game.Coins[0].IMass = game.Coins[1].IMass = 0;
    game.UpdateCoins(0);
    if (collision_calls != 2) return 5;
    game.Coins[0].IMass = game.Coins[1].IMass = 1;
    drawable = false;
    game.UpdateCoins(0);
    if (collision_calls != 2) return 6;
    drawable = true;
    game.Coins[1].Pos.X = 0;
    game.UpdateCoins(0);
    if (collision_calls != 2) return 7;
    game.Coins[1].Pos.X = 5;
    game.UpdateCoins(0);
    if (collision_calls != 2) return 8;
    first.Dead = true;
    game.UpdateCoins(0);
    if (state_calls != 1 || collision_calls != 2 || coin_definition.GetRefCount() != 1) return 9;
    puts("008ef520_TEST PASS");
    return 0;
}
bool CThing::SerialiseGameState(CPersistContext & p0) { abort(); }
bool CThing::ConstructFromLevelScript(const char * p0) { abort(); }
bool CThing::ConstructAlreadyExistingThingFromLevelScript(const char * p0) { abort(); }
bool CThing::ConstructFromGameStateScript(const char * p0) { abort(); }
void CThing::CopyGameState(const CThing & p0) { abort(); }
bool CThing::OnCreate() { abort(); }
void CThing::InitialActivate() { abort(); }
void CThing::OnInitialActivate() { abort(); }
void CThing::OnDie() { abort(); }
void CThing::OnKill() { abort(); }
bool CThing::PreOnKill() { abort(); }
void CThing::OnSetInLimbo(bool p0) { abort(); }
void CThing::CreateFromDef(const CThingBaseDef & p0) { abort(); }
void CThing::ConstructFromBinarySave(CMemoryDataInputStream & p0, const CThing::CSerialiseInfo & p1) { abort(); }
bool CThing::IsToBePlacedInMap() const { abort(); }
bool CThing::ValidateThing(CCharString & p0) const { abort(); }
bool CThing::WorldUpdate() { abort(); }
void CThing::WorldUpdateBulletTime() { abort(); }
bool CThing::RenderUpdate(const CInterpolationInfo * p0) { abort(); }
bool CThing::IsEntitySerialisable() const { abort(); }
unsigned long CThing::GetChecksum() const { abort(); }
void CThing::GetInfo(CThingInfo * p0) const { abort(); }
bool CThing::DrawGetEditDrawInfo(float p0, CEditEngineDrawInfo * p1) const { abort(); }
void CThing::DrawUpdate(const CInterpolationInfo * p0) { abort(); }
bool CThing::DrawIsDrawable() const { return drawable; }
void CThing::SetAsDrawable(bool p0) { abort(); }
bool CThing::DrawIsEditDrawable() const { abort(); }
bool CThing::DrawIsInDrawList() const { abort(); }
unsigned long CThing::DrawGetPhysicsMesh() const { abort(); }
bool CThing::DrawIsShowingFacingDirection() const { abort(); }
bool CThing::DrawIsDisplayingInformation() const { abort(); }
bool CThing::DrawGetThingDisplayInformation(CThingDrawInformation * p0) const { abort(); }
bool CThing::DrawGetPhysicsDebugInfo(CThingPhysicalDebugInfo & p0) const { abort(); }
bool CThing::DrawPreparePrimitivesForRendering(const CEnginePrimitivePrepareInfo & p0) { abort(); }
void CThing::SetThingStandingOn(CThing * p0) { abort(); }
const CThing * CThing::GetThingStandingOn() const { abort(); }
CThing * CThing::GetThingStandingOn() { abort(); }
long CThing::GetFloorMaterial() const { abort(); }
void CThing::OnCollision(ECollideType p0, const C3DVector & p1, const C3DVector & p2, CThing * p3) { abort(); }
void CThing::OnNonKillingLevelUnload() { abort(); }
void CThing::OnLevelUnload() { abort(); }
bool CThing::IsFrameUpdated() const { abort(); }
void CThing::SetPositionUsingBrain() { abort(); }
void CTCTavernGame::OnSerialise(CPersistContext&, bool) { abort(); }
void CTCTavernGame::OnInitialActivate() { abort(); }
bool CTCTavernGame::HasFrameUpdate() const { abort(); }
void CTCTavernGame::FrameUpdate() { abort(); }
void CTCTavernGame::DrawPreparePrimitivesForRendering(const CEnginePrimitivePrepareInfo&) { abort(); }
ETCInterfaceType CTCTavernGame::GetInterfaceType() const { abort(); }
void CTCTavernGame::ProcessInput(const CInputEvent&, CProcessedInput&, CInputProcessTavernGame*) { abort(); }
void CTCTavernGame::EnterGame() { abort(); }
void CTCTavernGame::LeaveGame() { abort(); }
void CTCTavernGame::NewGame() { abort(); }
void CTCTavernGame::NewRound() { abort(); }
void CTCTavernGame::EndRound() { abort(); }
void CTCTavernGame::QuitMidGame() { abort(); }
void CTCTavernGame::Greeting() { abort(); }
void CTCTavernGame::Options() { abort(); }
void CTCTavernGame::Instructions() { abort(); }
void CTCTavernGame::Betting() { abort(); }
void CTCTavernGame::ReactionAndPayout() { abort(); }
bool CTCTavernGame::UpdateValues() { abort(); }
void CTCTavernGame::Farewell() { abort(); }
CWideString CTCTavernGame::GetWinPhrase() { abort(); }
CWideString CTCTavernGame::GetAdditionalInfo() { abort(); }
CWideString CTCTavernGame::GetBet() { abort(); }
CWideString CTCTavernGame::GetPlayersMoney() { abort(); }
CWideString CTCTavernGame::GetTotalWinnings() { abort(); }
void CTCTavernGame::DrawBetAndMoney(const CEnginePrimitivePrepareInfo&, float, float, float, float, float, float) { abort(); }
void CTCTavernGameCoinBase::OnInitialActivate() { abort(); }
ETCInterfaceType CTCTavernGameCoinBase::GetInterfaceType() const { abort(); }
void CTCTavernGameCoinBase::EnterGame() { abort(); }
void CTCTavernGameCoinBase::LeaveGame() { abort(); }
void CTCTavernGameCoinBase::NewGame() { abort(); }
void CTCTavernGameCoinBase::NewRound() { abort(); }
void CTCTavernGameCoinBase::EndRound() { abort(); }