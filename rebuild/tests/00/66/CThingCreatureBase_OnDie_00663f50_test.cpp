// Behaviour test for 0x00663F50 CThingCreatureBase::OnDie: the source model is copied verbatim (filler virtual slots given
// empty bodies) and every callee is a recording stub; two deaths are run (with and without a dead body / live killer).
#include <cstdio>
#include <cstring>
#include <cstdlib>
#include <new>
#ifndef _MSC_VER
#define __forceinline inline
#define __fastcall
#define __cdecl
template <class T> const T* TempAddress(const T& value) { return &value; }   // g++ has no address-of-temporary extension
#endif
// CThingCreatureBase::OnDie() -- base physical death; spawn the EXPLODE_ON_KILLING_BLOW effect when the def names one; leave a dead-creature body (copying orientation, the carried components and the lighting channel) when the creature and its def allow it; post the death message, tell the killer it killed and the creature's own entity events it died, and drop experience orbs for the hero
// Leaf: ?OnDie@CThingCreatureBase
// Retail: Fable.exe (TLC, VC7.1) @ 0x00663F50, 1052 bytes (+ int3 padding), plain `ret`; virtual OnDie of CThingCreatureBase
//   (the landed CThingAICreature::OnDie 0x00832E60 tail-calls it).
// Callees: CThingPhysical::OnDie 0x006A4CF0, CDefClassBase::GetSubDef<CSpecialEffectsDef> 0x00669580,
//   CSpecialEffectsDef::GetEffectFromString 0x00669D30, CThing::GetPosition 0x004C73D0, CreateEffectThing 0x006E0880,
//   LowerBound 0x004365B0 (const) / 0x0040F020, CTCCreatureModeManager::GetCurrentMode 0x0073A6E0 (the mode's slot 4),
//   operator+(const CCharString&, const char*) 0x0099F600 / ~CCharString 0x0099EAE0, CThingCreatureBase::GetPosition
//   0x006A4D00, CThingDeadCreature::Create 0x00835C80 (engine_api: __fastcall long, C3DVector*, long, CCharString*,
//   CAnimFilterBase*), CTCPhysicsBase slots 72 GetLastRHSet / 32 ResetRHSet / 41 / 40, CThing::AddAndCopyTC 0x004C9F60,
//   CTCGraphicAppearance::SetLightingChannel 0x004BFB40 / SetAsChanged 0x004BF9E0, the dead body's killer setter 0x00669230
//   (stores into CThingDeadCreature::PKiller +0xd8), CBaseIntelligentPointer::GetPItem 0x00A01B50, CThing::PeekWorld
//   0x004C7AA0, CMessageEventManager::AddMessage 0x00687540, operator new 0x00BFEA1A, CEventKilledCreature ctor 0x00811190,
//   CEventDied ctor 0x00669300, CTCEntityEvents::AddEvent 0x008113C0, the experience-orb drop 0x0077A890 (__fastcall).
// Strings: "EXPLODE_ON_KILLING_BLOW" 0x0125AB88, "_DEAD_CREATURE" 0x0125AB78; frame rate 0x01375550.
// Layout (retail; Ego_r.pdb less 0x10 in CThing, 0x1c/0x28 in CThingCreatureBase): CThing m_type +0x10, interface bitset
//   +0x20, map +0x44, PhysicsTC +0x60, PTCGraphicAppearance +0x64, PBaseDef +0x70, ScriptName +0x74, DefGlobalIndex +0x8c,
//   PlayerNumber +0x90, Dead bit 0 of +0x91; CThingCreatureBase PDef +0xe0, PLastAttackedByCreature +0xf4,
//   LeaveDeadBodyOverride / LeaveExperienceOrbsOverride bits 4/5 of +0x15c; CThingCreatureDef LeaveDeadCreature bit 0 of
//   +0x148; CTCPhysicsBase Position +0xc. Interfaces: 0x31 creature mode manager, 0x42 entity events, 0x75 lighting
//   channel (its channel description at +0x14), 0xc9 tested on the killer before the orb drop.
// Shape: the copied components and the lighting channel go through a const GetInterface(&out, type) out-parameter form
//   (a plain `if (HasTC) ...GetConstTC` sinks the component load and rotates its register); the mode, player number and
//   def index are read into locals before Create (retail reads them ahead of the mode's virtual call); the body's physics
//   TC goes through an inline accessor (keeps the object in edi across the inner call); the physics TC and the world are
//   locals ahead of AddMessage; each entity-events component is fetched before its `new` event.
#define VF(n) virtual void v##n() {}
class C3DVector { public: float X, Y, Z; };
class CRightHandedSet;
class CTCBase;
class CThing;

class CCharString { public:
    void* PStringData;
    CCharString(const char* text, long length = -1);    // 0x0099EBF0
    ~CCharString();                                     // 0x0099EAE0
};
CCharString __fastcall operator+(const CCharString& left, const char* right);   // 0x0099F600

struct CDefVirtual {
    virtual ~CDefVirtual();                             // slot 0
    virtual void Destroy();                             // slot 1
};
struct CDefBase : CDefVirtual {
    long m_refCount;                                    // +0x04
};
class CSpecialEffectsDef : public CDefBase { public:
    bool GetEffectFromString(const CCharString& name, long& type) const;   // 0x00669D30
};
template <class T> class CDefPointer { public:
    T* m_p;
    CDefPointer() : m_p(0) { }
};
class CDefClassBase : public CDefBase { public:
    template <class T> bool GetSubDef(CDefPointer<T>& out);   // 0x00669580 (T = CSpecialEffectsDef)
};

struct CInterfaceMap {
    struct SEntry { long type; CTCBase* component; };
    SEntry* first;
    SEntry* last;
    SEntry* LowerBound(const long* type);               // 0x0040F020
    const SEntry* LowerBound(const long* type) const;   // 0x004365B0
};

class CTCPhysicsBase { public:
    VF(00) VF(01) VF(02) VF(03) VF(04) VF(05) VF(06) VF(07) VF(08) VF(09)
    VF(10) VF(11) VF(12) VF(13) VF(14) VF(15) VF(16) VF(17) VF(18) VF(19)
    VF(20) VF(21) VF(22) VF(23) VF(24) VF(25) VF(26) VF(27) VF(28) VF(29)
    VF(30) VF(31)
    virtual void ResetRHSet(const CRightHandedSet& set);        // slot 32 (+0x80)
    VF(33) VF(34) VF(35) VF(36) VF(37) VF(38) VF(39)
    virtual void SetGravityEnabled(bool enabled);               // slot 40 (+0xa0, descriptive)
    virtual bool IsGravityEnabled() const;                      // slot 41 (+0xa4)
    VF(42) VF(43) VF(44) VF(45) VF(46) VF(47) VF(48) VF(49)
    VF(50) VF(51) VF(52) VF(53) VF(54) VF(55) VF(56) VF(57) VF(58) VF(59)
    VF(60) VF(61) VF(62) VF(63) VF(64) VF(65) VF(66) VF(67) VF(68) VF(69)
    VF(70) VF(71)
    virtual const CRightHandedSet& GetLastRHSet() const;        // slot 72 (+0x120)
    unsigned char _pad04[0x0c - 0x04];
    C3DVector Position;                                 // +0x0c
};
class CEnginePrimitiveThemeChannelDesc;
struct CTCGraphicAppearance {
    void SetLightingChannel(CEnginePrimitiveThemeChannelDesc* channel, bool a, bool b);   // 0x004BFB40
    void SetAsChanged(unsigned long reasons);           // 0x004BF9E0
};
struct CTCLightingChannel {                             // interface 0x75: the channel description at +0x14
    unsigned char _pad00[0x14];
    unsigned char Channel[4];                           // +0x14 CEnginePrimitiveThemeChannelDesc
};
class CAnimFilterBase;
struct CCreatureModeBase {
    VF(00) VF(01) VF(02) VF(03)
    virtual CAnimFilterBase* GetDeathAnimFilter();      // slot 4 (+0x10, descriptive)
};
class CTCCreatureModeManager { public:
    CCreatureModeBase* GetCurrentMode() const;          // 0x0073A6E0
};
struct CEventBase;
class CTCEntityEvents { public:
    void AddEvent(CEventBase* event);                   // 0x008113C0
};
struct CEventKilledCreature {
    unsigned char _body[0x0c];
    CEventKilledCreature(CThing* killer, CThing* victim);   // 0x00811190
};
struct CEventDied {
    unsigned char _body[0x18];
    CEventDied(CThing* killer, CThing* body);           // 0x00669300
};
class CMessageEvent;
class CMessageEventManager { public:
    CMessageEvent* AddMessage(long type, unsigned long delay, const C3DVector& position, CThing* from, CThing* to, float value1, float value2, long data);   // 0x00687540
};
class CWorld { public:
    unsigned char _pad[0x60];
    CMessageEventManager* MessageEventManager;          // +0x60
};
extern long g_framesPerSecond;                          // 0x01375550

class CThing {
public:
    void* __vftable;
    unsigned char _pad04[0x10 - 0x04];
    unsigned char m_type;                               // +0x10 (1 = the player creature)
    unsigned char _pad11[0x20 - 0x11];
    unsigned long m_presentBits[9];                     // +0x20
    CInterfaceMap m_interfaces;                         // +0x44
    unsigned char _pad4c[0x60 - 0x4c];
    CTCPhysicsBase* PhysicsTC;                          // +0x60
    CTCPhysicsBase* GetPhysicsTC() const { return PhysicsTC; }
    CTCGraphicAppearance* PTCGraphicAppearance;         // +0x64
    unsigned char _pad68[0x70 - 0x68];
    CDefClassBase* PBaseDef;                            // +0x70
    CCharString   ScriptName;                           // +0x74
    unsigned char _pad78[0x8c - 0x78];
    unsigned short DefGlobalIndex;                      // +0x8c
    unsigned char _pad8e[0x90 - 0x8e];
    signed char   PlayerNumber;                         // +0x90
    unsigned char Dead : 1;                             // +0x91 bit 0
    const C3DVector& GetPosition() const;               // 0x004C73D0
    CWorld* PeekWorld();                                // 0x004C7AA0
    void AddAndCopyTC(const CTCBase* tc, bool activate);   // 0x004C9F60
    __forceinline bool HasTC(long type) const
    {
        if (m_presentBits[type >> 5] & (1UL << (type & 31)))
            return true;
        return false;
    }
    __forceinline CTCBase* GetTC(long type)
    {
        long key = type;
        CInterfaceMap* map = &m_interfaces;
        CInterfaceMap::SEntry* it = map->LowerBound(&key);
        if (it == map->last || type < it->type)
            it = map->last;
        return it->component;
    }
    template <class T> __forceinline bool GetConstInterface(const T** ppOut, long type) const
    {
        if (m_presentBits[type >> 5] & (1UL << (type & 31))) {
            long key = type;
            const CInterfaceMap* map = &m_interfaces;
            const CInterfaceMap::SEntry* it = map->LowerBound(&key);
            const CInterfaceMap::SEntry* end = map->last;
            *ppOut = (const T*)(it == end || type < it->type ? end : it)->component;
            return true;
        }
        return false;
    }
    __forceinline const CTCBase* GetConstTC(long type) const
    {
        long key = type;
        const CInterfaceMap* map = &m_interfaces;
        const CInterfaceMap::SEntry* it = map->LowerBound(&key);
        const CInterfaceMap::SEntry* end = map->last;
        return (it == end || type < it->type ? end : it)->component;
    }
};
CThing* __fastcall CreateEffectThing(long type, const C3DVector* position, bool flag);   // 0x006E0880

struct CBaseIntelligentPointer {
    void* __vftable;
    CThing* m_pItem;
    CThing* GetPItem() const;                           // 0x00A01B50
};

class CThingCreatureDef { public:
    unsigned char _pad00[0x148];
    bool LeaveDeadCreature : 1;                         // +0x148 bit 0
};

class CThingPhysical : public CThing {
public:
    void OnDie();                                       // 0x006A4CF0
};
class CThingDeadCreature;
class CThingCreatureBase : public CThingPhysical {
public:
    unsigned char _pad94[0xe0 - 0x94];
    const CThingCreatureDef* PDef;                      // +0xe0
    unsigned char _pade4[0xf4 - 0xe4];
    CBaseIntelligentPointer PLastAttackedByCreature;    // +0xf4
    unsigned char _padfc[0x15c - 0xfc];
    bool CurrentlyFrameUpdating : 1;                    // +0x15c
    bool Blinking : 1;
    bool MeleeAttacker : 1;
    bool MeleeAttacked : 1;
    bool LeaveDeadBodyOverride : 1;                     // bit 4
    bool LeaveExperienceOrbsOverride : 1;               // bit 5
    const C3DVector& GetCreaturePosition() const;       // 0x006A4D00 (CThingCreatureBase::GetPosition)
    void OnDie();
};
class CThingDeadCreature : public CThing {
public:
    static CThingDeadCreature* __fastcall Create(long def, const C3DVector* position, long playerNumber, const CCharString* scriptName, CAnimFilterBase* filter);   // 0x00835C80
    void SetKiller(CThing* killer);                     // 0x00669230
};
void __fastcall CreateExperienceOrbs(CThing* victim);   // 0x0077A890

void CThingCreatureBase::OnDie()
{
    CThingPhysical::OnDie();
    {
        CDefPointer<CSpecialEffectsDef> effectsSlot;
        long effectType = 0;
        bool found = PBaseDef->GetSubDef(effectsSlot);
        CSpecialEffectsDef* effects = effectsSlot.m_p;
        if (found && effects->GetEffectFromString("EXPLODE_ON_KILLING_BLOW", effectType))
            CreateEffectThing(effectType, &GetPosition(), false);
        if (effects && --effects->m_refCount == 0)
            effects->Destroy();
    }
    CThingDeadCreature* body = 0;
    if (LeaveDeadBodyOverride && PDef->LeaveDeadCreature) {
        const CTCCreatureModeManager* modes = 0;
        if (HasTC(0x31))
            modes = (const CTCCreatureModeManager*)GetConstTC(0x31);
        CCreatureModeBase* mode = modes->GetCurrentMode();
        long playerNumber = PlayerNumber;
        long definition = DefGlobalIndex;
        body = CThingDeadCreature::Create(definition, &GetCreaturePosition(), playerNumber,
                                          
#ifdef _MSC_VER
                                          &(ScriptName + "_DEAD_CREATURE"),
#else
                                          TempAddress(ScriptName + "_DEAD_CREATURE"),
#endif
                                         mode->GetDeathAnimFilter());
        if (body) {
            const CTCBase* component;
            body->GetPhysicsTC()->ResetRHSet(GetPhysicsTC()->GetLastRHSet());
            if (body->PhysicsTC->IsGravityEnabled())
                body->PhysicsTC->SetGravityEnabled(false);
            if (GetConstInterface(&component, 0xd1))
                body->AddAndCopyTC(component, true);
            if (GetConstInterface(&component, 3))
                body->AddAndCopyTC(component, true);
            if (GetConstInterface(&component, 0x5e))
                body->AddAndCopyTC(component, true);
            if (GetConstInterface(&component, 0x5c))
                body->AddAndCopyTC(component, true);
            if (GetConstInterface(&component, 0x57))
                body->AddAndCopyTC(component, true);
            const CTCLightingChannel* lighting;
            if (GetConstInterface(&lighting, 0x75)) {
                if (body->PTCGraphicAppearance) {
                    body->PTCGraphicAppearance->SetLightingChannel((CEnginePrimitiveThemeChannelDesc*)lighting->Channel, true, true);
                    body->PTCGraphicAppearance->SetAsChanged(4);
                }
            }
            body->SetKiller(PLastAttackedByCreature.GetPItem());
        }
    }
    CThing* killer = PLastAttackedByCreature.GetPItem();
    CTCPhysicsBase* physics = PhysicsTC;
    CWorld* world = PeekWorld();
    world->MessageEventManager->AddMessage(9, g_framesPerSecond, physics->Position, killer, this, 0.0f, 0.0f, 0);
    if (killer && !killer->Dead) {
        if (killer->HasTC(0x42)) {
            CTCEntityEvents* events = (CTCEntityEvents*)killer->GetTC(0x42);
            events->AddEvent((CEventBase*)new CEventKilledCreature(killer, this));
        }
        if (LeaveExperienceOrbsOverride && killer->HasTC(0xc9) && m_type == 1)
            CreateExperienceOrbs(this);
    }
    if (HasTC(0x42)) {
        CTCEntityEvents* events = (CTCEntityEvents*)GetTC(0x42);
        events->AddEvent((CEventBase*)new CEventDied(killer, body));
    }
}


static int g_fail = 0;
#define CHECK(c) do { if (!(c)) { std::printf("check failed line %d: %s\n", __LINE__, #c); g_fail = 1; } } while (0)
static int g_step = 0;
static int g_baseDied = 0, g_effectType = -1, g_effectFlag = -1, g_effectsDestroyed = 0, g_effectsRefs = 0;
static const C3DVector* g_effectPos = 0;
static C3DVector g_thingPos, g_creaturePos;
static char g_concat[64];

CCharString::CCharString(const char* text, long length) { PStringData = (void*)text; (void)length; }
CCharString::~CCharString() { }
CCharString __fastcall operator+(const CCharString& left, const char* right)
{
    std::strcpy(g_concat, (const char*)left.PStringData);
    std::strcat(g_concat, right);
    return CCharString(g_concat);
}
CDefVirtual::~CDefVirtual() { }
void CDefVirtual::Destroy() { }
class CRightHandedSet { public: int tag; };
static CRightHandedSet g_rhSet;
void CTCPhysicsBase::ResetRHSet(const CRightHandedSet&) { }
void CTCPhysicsBase::SetGravityEnabled(bool) { }
bool CTCPhysicsBase::IsGravityEnabled() const { return false; }
const CRightHandedSet& CTCPhysicsBase::GetLastRHSet() const { return g_rhSet; }
CAnimFilterBase* CCreatureModeBase::GetDeathAnimFilter() { return 0; }
struct CTestEffectsDef : CSpecialEffectsDef { void Destroy() { ++g_effectsDestroyed; } };
static CTestEffectsDef g_effects;
static bool g_haveEffects = true;
template <class T> bool CDefClassBase::GetSubDef(CDefPointer<T>& out)
{
    if (!g_haveEffects)
        return false;
    g_effects.m_refCount = g_effectsRefs;
    out.m_p = (T*)&g_effects;
    return true;
}
bool CSpecialEffectsDef::GetEffectFromString(const CCharString& name, long& type) const
{
    if (std::strcmp((const char*)name.PStringData, "EXPLODE_ON_KILLING_BLOW") != 0)
        return false;
    type = 7;
    return true;
}
struct CTestDefClass : CDefClassBase { void Destroy() { } };
static CTestDefClass g_baseDef;
CThing* __fastcall CreateEffectThing(long type, const C3DVector* position, bool flag)
{
    g_effectType = type; g_effectPos = position; g_effectFlag = flag;
    return 0;
}
static const CInterfaceMap::SEntry* Find(const CInterfaceMap* map, long type)
{
    const CInterfaceMap::SEntry* it = map->first;
    while (it != map->last && it->type < type)
        ++it;
    return it;
}
CInterfaceMap::SEntry* CInterfaceMap::LowerBound(const long* type) { return (SEntry*)Find(this, *type); }
const CInterfaceMap::SEntry* CInterfaceMap::LowerBound(const long* type) const { return Find(this, *type); }
static void Present(CThing* thing, long type) { thing->m_presentBits[type >> 5] |= 1UL << (type & 31); }

struct CTestAnimFilter { int id; };
static CTestAnimFilter g_filter;
struct CTestMode : CCreatureModeBase { CAnimFilterBase* GetDeathAnimFilter() { return (CAnimFilterBase*)&g_filter; } };
static CTestMode g_mode;
CCreatureModeBase* CTCCreatureModeManager::GetCurrentMode() const { return &g_mode; }

static const CRightHandedSet* g_resetSet = 0;
static int g_gravityAsked = 0, g_gravitySet = -1;
struct CTestPhysics : CTCPhysicsBase {
    bool Gravity;
    void ResetRHSet(const CRightHandedSet& set) { g_resetSet = &set; }
    void SetGravityEnabled(bool enabled) { g_gravitySet = enabled; }
    bool IsGravityEnabled() const { ++g_gravityAsked; return Gravity; }
    const CRightHandedSet& GetLastRHSet() const { return g_rhSet; }
};
static CTestPhysics g_myPhysics, g_bodyPhysics;
static const void* g_lightingChannel = 0;
static int g_lightA = -1, g_lightB = -1, g_changed = -1;
void CTCGraphicAppearance::SetLightingChannel(CEnginePrimitiveThemeChannelDesc* channel, bool a, bool b) { g_lightingChannel = channel; g_lightA = a; g_lightB = b; }
void CTCGraphicAppearance::SetAsChanged(unsigned long reasons) { g_changed = (int)reasons; }
static CTCGraphicAppearance g_bodyAppearance;

static const CTCBase* g_copied[8];
static int g_copiedCount = 0, g_copyActivateBad = 0;
void CThing::AddAndCopyTC(const CTCBase* tc, bool activate) { g_copied[g_copiedCount++] = tc; if (!activate) g_copyActivateBad = 1; }
const C3DVector& CThing::GetPosition() const { return g_thingPos; }
static CWorld g_world;
CWorld* CThing::PeekWorld() { return &g_world; }
static long g_msgType = -1; static unsigned long g_msgDelay = 0; static const C3DVector* g_msgPos = 0;
static CThing* g_msgFrom = 0; static CThing* g_msgTo = 0;
CMessageEvent* CMessageEventManager::AddMessage(long type, unsigned long delay, const C3DVector& position, CThing* from, CThing* to, float value1, float value2, long data)
{
    g_msgType = type; g_msgDelay = delay; g_msgPos = &position; g_msgFrom = from; g_msgTo = to;
    if (value1 != 0.0f || value2 != 0.0f || data != 0) g_msgType = -2;
    return 0;
}
static CMessageEventManager g_manager;
long g_framesPerSecond = 30;
static CThing* g_lastAttacker = 0;
CThing* CBaseIntelligentPointer::GetPItem() const { return g_lastAttacker; }
void CThingPhysical::OnDie() { g_baseDied = ++g_step; }
const C3DVector& CThingCreatureBase::GetCreaturePosition() const { return g_creaturePos; }

static unsigned char g_bodyStorage[sizeof(CThingDeadCreature) + 16];   // zeroed static storage (CThing has no default ctor)
#define g_body (*(CThingDeadCreature*)g_bodyStorage)
static int g_created = 0, g_createBad = 0;
static CThing* g_bodyKiller = (CThing*)1;
CThingDeadCreature* __fastcall CThingDeadCreature::Create(long def, const C3DVector* position, long playerNumber, const CCharString* scriptName, CAnimFilterBase* filter)
{
    ++g_created;
    if (def != 0x1234 || position != &g_creaturePos || playerNumber != -2 || filter != (CAnimFilterBase*)&g_filter
        || std::strcmp((const char*)scriptName->PStringData, "Bandit_DEAD_CREATURE") != 0)
        g_createBad = 1;
    return &g_body;
}
void CThingDeadCreature::SetKiller(CThing* killer) { g_bodyKiller = killer; }

static CThing* g_eventsOwner[4]; static CEventBase* g_events[4]; static int g_eventCount = 0;
struct CTestEvents { CThing* Owner; };
void CTCEntityEvents::AddEvent(CEventBase* event) { g_eventsOwner[g_eventCount] = ((CTestEvents*)this)->Owner; g_events[g_eventCount++] = event; }
static CThing* g_killedKiller = 0; static CThing* g_killedVictim = 0;
CEventKilledCreature::CEventKilledCreature(CThing* killer, CThing* victim) { g_killedKiller = killer; g_killedVictim = victim; }
static CThing* g_diedKiller = (CThing*)1; static CThing* g_diedBody = (CThing*)1;
CEventDied::CEventDied(CThing* killer, CThing* body) { g_diedKiller = killer; g_diedBody = body; }
static int g_orbs = 0; static CThing* g_orbVictim = 0;
void __fastcall CreateExperienceOrbs(CThing* victim) { ++g_orbs; g_orbVictim = victim; }

struct CTestComponent { int id; };
static CTestComponent g_comp[10];
static CTestEvents g_myEvents, g_killerEvents;
static CInterfaceMap::SEntry g_myEntries[8], g_killerEntries[2];
static unsigned char g_creatureStorage[sizeof(CThingCreatureBase) + 16];
static unsigned char g_killerStorage[sizeof(CThing) + 16];
static CThingCreatureDef g_def;
static unsigned char g_lightingStorage[sizeof(CTCLightingChannel)];

static void Reset()
{
    g_step = g_baseDied = 0; g_effectType = g_effectFlag = -1; g_effectPos = 0; g_effectsDestroyed = 0;
    g_resetSet = 0; g_gravityAsked = 0; g_gravitySet = -1; g_lightingChannel = 0; g_lightA = g_lightB = g_changed = -1;
    g_copiedCount = 0; g_copyActivateBad = 0; g_msgType = -1; g_msgPos = 0; g_msgFrom = g_msgTo = 0;
    g_created = g_createBad = 0; g_bodyKiller = (CThing*)1; g_eventCount = 0; g_killedKiller = g_killedVictim = 0;
    g_diedKiller = g_diedBody = (CThing*)1; g_orbs = 0; g_orbVictim = 0;
}

int main()
{
    std::memset(g_creatureStorage, 0, sizeof g_creatureStorage);
    std::memset(g_killerStorage, 0, sizeof g_killerStorage);
    CThingCreatureBase* me = (CThingCreatureBase*)g_creatureStorage;
    CThing* killer = (CThing*)g_killerStorage;
    me->PBaseDef = &g_baseDef;
    me->PhysicsTC = &g_myPhysics;
    me->ScriptName.PStringData = (void*)"Bandit";
    me->DefGlobalIndex = 0x1234;
    me->PlayerNumber = -2;
    me->m_type = 1;
    me->PDef = &g_def;
    g_def.LeaveDeadCreature = 1;
    me->LeaveDeadBodyOverride = 1;
    me->LeaveExperienceOrbsOverride = 1;
    g_world.MessageEventManager = &g_manager;
    long myTypes[] = { 3, 0x31, 0x42, 0x57, 0x75, 0xd1 };
    for (int i = 0; i < 6; ++i) {
        g_myEntries[i].type = myTypes[i];
        g_myEntries[i].component = (CTCBase*)&g_comp[i];
        Present(me, myTypes[i]);
    }
    g_myEntries[2].component = (CTCBase*)&g_myEvents; g_myEvents.Owner = me;
    g_myEntries[4].component = (CTCBase*)g_lightingStorage;
    me->m_interfaces.first = g_myEntries; me->m_interfaces.last = g_myEntries + 6;
    g_killerEntries[0].type = 0x42; g_killerEntries[0].component = (CTCBase*)&g_killerEvents; g_killerEvents.Owner = killer;
    killer->m_interfaces.first = g_killerEntries; killer->m_interfaces.last = g_killerEntries + 1;
    Present(killer, 0x42); Present(killer, 0xc9);
    g_body.PhysicsTC = &g_bodyPhysics;
    g_body.PTCGraphicAppearance = &g_bodyAppearance;
    g_bodyPhysics.Gravity = true;

    // death 1: a live killer, a def with effects (one reference left), a dead body
    Reset();
    g_effectsRefs = 1;
    g_lastAttacker = killer;
    me->OnDie();
    CHECK(g_baseDied == 1);
    CHECK(g_effectType == 7 && g_effectPos == &g_thingPos && g_effectFlag == 0);
    CHECK(g_effectsDestroyed == 1);
    CHECK(g_created == 1 && g_createBad == 0);
    CHECK(g_resetSet == &g_rhSet && g_gravityAsked == 1 && g_gravitySet == 0);
    CHECK(g_copiedCount == 3 && g_copied[0] == (CTCBase*)&g_comp[5] && g_copied[1] == (CTCBase*)&g_comp[0]
          && g_copied[2] == (CTCBase*)&g_comp[3] && g_copyActivateBad == 0);
    CHECK(g_lightingChannel == ((CTCLightingChannel*)g_lightingStorage)->Channel && g_lightA == 1 && g_lightB == 1 && g_changed == 4);
    CHECK(g_bodyKiller == killer);
    CHECK(g_msgType == 9 && g_msgDelay == 30 && g_msgPos == &g_myPhysics.Position && g_msgFrom == killer && g_msgTo == me);
    CHECK(g_killedKiller == killer && g_killedVictim == me);
    CHECK(g_orbs == 1 && g_orbVictim == me);
    CHECK(g_eventCount == 2 && g_eventsOwner[0] == killer && g_eventsOwner[1] == me);
    CHECK(g_diedKiller == killer && g_diedBody == &g_body);

    // death 2: no effects def, no dead body (override clear), a dead killer, gravity already off
    Reset();
    g_haveEffects = false;
    me->LeaveDeadBodyOverride = 0;
    killer->Dead = 1;
    me->OnDie();
    CHECK(g_baseDied == 1 && g_effectType == -1 && g_effectsDestroyed == 0);
    CHECK(g_created == 0 && g_copiedCount == 0 && g_bodyKiller == (CThing*)1);
    CHECK(g_msgType == 9 && g_msgFrom == killer);
    CHECK(g_killedKiller == 0 && g_orbs == 0);
    CHECK(g_eventCount == 1 && g_eventsOwner[0] == me && g_diedKiller == killer && g_diedBody == 0);

    // death 3: body kept, effects def still referenced, no killer, body physics without gravity, no appearance
    Reset();
    g_haveEffects = true; g_effectsRefs = 2;
    me->LeaveDeadBodyOverride = 1;
    g_bodyPhysics.Gravity = false;
    g_body.PTCGraphicAppearance = 0;
    g_lastAttacker = 0;
    me->OnDie();
    CHECK(g_effectType == 7 && g_effectsDestroyed == 0 && g_effects.m_refCount == 1);
    CHECK(g_created == 1 && g_gravityAsked == 1 && g_gravitySet == -1 && g_lightingChannel == 0 && g_changed == -1);
    CHECK(g_bodyKiller == 0 && g_msgFrom == 0 && g_orbs == 0);
    CHECK(g_eventCount == 1 && g_diedKiller == 0 && g_diedBody == &g_body);

    if (g_fail) { std::printf("00663f50_TEST FAIL\n"); return 1; }
    std::printf("00663f50_TEST PASS\n");
    return 0;
}