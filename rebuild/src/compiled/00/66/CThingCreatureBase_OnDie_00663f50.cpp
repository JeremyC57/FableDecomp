// CThingCreatureBase::OnDie() -- base physical death; spawn the EXPLODE_ON_KILLING_BLOW effect when the def names one; leave a dead-creature body (copying orientation, the carried components and the lighting channel) when the creature and its def allow it; post the death message, tell the killer it killed and the creature's own entity events it died, and drop experience orbs for the hero
// Leaf: ?OnDie@CThingCreatureBase
// Retail: Fable.exe (TLC, VC7.1) @ 0x00663F50, 1052 bytes (+ int3 padding), plain `ret`; virtual OnDie of CThingCreatureBase
//   (the landed CThingAICreature::OnDie 0x00832E60 tail-calls it).
// Callees: CThingPhysical::OnDie 0x006A4CF0, CDefClassBase::GetSubDef<CSpecialEffectsDef> 0x00669580,
//   CSpecialEffectsDef::GetEffectFromString 0x00669D30, CThing::GetPos 0x004C73D0, CreateEffectThing 0x006E0880,
//   LowerBound 0x004365B0 (const) / 0x0040F020, CTCCreatureModeManager::GetCurrentMode 0x0073A6E0 (the mode's slot 4),
//   operator+(const CCharString&, const char*) 0x0099F600 / ~CCharString 0x0099EAE0, CThingPhysical::PeekPos
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
#define VF(n) virtual void v##n();
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

enum ETCInterfaceType { };                                   // CTCBase interface type (values unused here)
namespace std { template <class T1, class T2> struct pair { T1 first; T2 second; }; }   // STLport 4.6 <utility>
template <class K, class V> class CKeyPairCompareLess;
template <class K, class V, class TCompare> class CVectorMap;
template <> class CVectorMap<ETCInterfaceType, CTCBase*, CKeyPairCompareLess<ETCInterfaceType, CTCBase*> > { public:
    typedef std::pair<ETCInterfaceType, CTCBase*> SEntry;
    SEntry* first;
    SEntry* last;
    SEntry* LowerBound(const ETCInterfaceType& type);               // 0x0040F020
    const SEntry* LowerBound(const ETCInterfaceType& type) const;   // 0x004365B0
};
typedef CVectorMap<ETCInterfaceType, CTCBase*, CKeyPairCompareLess<ETCInterfaceType, CTCBase*> > CInterfaceMap;

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
    const C3DVector& GetPos() const;    // 0x004C73D0
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
        ETCInterfaceType key = (ETCInterfaceType)(type);
        CInterfaceMap* map = &m_interfaces;
        CInterfaceMap::SEntry* it = map->LowerBound(key);
        if (it == map->last || type < it->first)
            it = map->last;
        return it->second;
    }
    template <class T> __forceinline bool GetConstInterface(const T** ppOut, long type) const
    {
        if (m_presentBits[type >> 5] & (1UL << (type & 31))) {
            ETCInterfaceType key = (ETCInterfaceType)(type);
            const CInterfaceMap* map = &m_interfaces;
            const CInterfaceMap::SEntry* it = map->LowerBound(key);
            const CInterfaceMap::SEntry* end = map->last;
            *ppOut = (const T*)(it == end || type < it->first ? end : it)->second;
            return true;
        }
        return false;
    }
    __forceinline const CTCBase* GetConstTC(long type) const
    {
        ETCInterfaceType key = (ETCInterfaceType)(type);
        const CInterfaceMap* map = &m_interfaces;
        const CInterfaceMap::SEntry* it = map->LowerBound(key);
        const CInterfaceMap::SEntry* end = map->last;
        return (it == end || type < it->first ? end : it)->second;
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
    const C3DVector& PeekPos() const;    // 0x006A4D00 (?PeekPos@CThingPhysical@@QBEABVC3DVector@@XZ)
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
            CreateEffectThing(effectType, &GetPos(), false);
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
        body = CThingDeadCreature::Create(definition, &PeekPos(), playerNumber,
                                          &(ScriptName + "_DEAD_CREATURE"), mode->GetDeathAnimFilter());
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