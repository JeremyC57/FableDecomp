// CThingAICreature::OnHit(CHitParameters& hit) -- a Briar Rose concentrating creature swaps place and orientation with a random live imposter and passes the hit on; otherwise, for a hit from a live game-object attacker post the hit message, take the attacker as the last attacker (unless the hit is a fatal ability hit or the current attacker keeps priority) and post the assault deeds to the attacker's deed log; then the base creature OnHit and the attacker's hit-thing event
// Leaf: ?OnHit@CThingAICreature
// Grade: DIFFER(1206v1187) -- 141 difflines (base 1208v1187/192); a __forceinline memberwise C3DVector copy ctor (the 007F9960 lever) fixes the imposter position copy. Left: frame 0x3c vs 0x38 (retail homes the deed block's interface keys, the opinion pointer and the tolerance bool in one word), the hoisted PostDeed tail pushes, and the ability-def release form (retail test/dec [ecx+4] RMW, ours load/dec/store). Next: the OnDie 0x00832E60 deed-log GetInterface out-parameter model.
//   the opinion pointer and the tolerance bool in one word [esp+18h] and the FindDeed handle at [esp+20h]); the imposter sits
//   in edi (retail ebp, the mode in edi); the imposter position copy goes through `lea eax,[ebp+0ch]`; the deed block's
//   common PostDeed tail pushes (extra, b3, faction, b2) are hoisted above `cmp eax,-1` in retail; the ability-def release is
//   the register read-modify-write form (retail `dec dword ptr [ecx+4] ; jne`). Tried: CDefPointeeBase/ReleaseDef dtor and
//   an explicit release through a local, the CDefBase layout. Next: the CThingAICreature::OnDie 0x00832E60 deed-log model
//   (GetInterface out-parameter lookups) and a C3DVector with a user copy ctor.
// Retail: Fable.exe (TLC, VC7.1) @ 0x008329B0, 1187 bytes, `ret 4`; CThingAICreature vtable 0x0127293C slot 65.
// Callees: LowerBound 0x0040F020 / 0x004365B0 (const), CTCCreatureModeManager::HasMode 0x0073A8A0 / GetCurrentMode
//   0x0073A6E0, the Briar Rose concentrate mode's 0x008E7E10 (landed CIconText::HasEnoughIcons) / GetRandomImposter
//   0x008E7F80, CRightHandedSet copy ctor 0x0049CF80, CTCPhysicsBase slots 31 SetPosition / 32 ResetRHSet / 72 GetLastRHSet,
//   CThing slot 65 OnHit, CBaseIntelligentPointer::GetPItem 0x00A01B50, CThing::PeekWorld 0x004C7AA0 / GetPosition 0x004C73D0,
//   CMessageEventManager::AddMessage 0x00687540, AreThingsEnemies 0x0076D2F0, GetSubDef<CAbilityDef> 0x00485815, CThing slot 74
//   (0x00830110 for an AI creature: returns true), SetAttackedByCreature 0x006624F0, CTCCreatureOpinionOfHero::
//   UpdateHitTolerance 0x006819D0, the deed log's 0x006C5800 (descriptive FindDeed) / PostDeed 0x006C6990 / GetDeedFromHandle
//   0x006C58F0, CThingCreatureBase::GetPosition 0x006A4D00 / OnHit 0x00667D30, IsThingCarryingMeleeWeapon 0x006A15A0 /
//   IsThingCarryingRangedWeapon 0x006A1620, operator new 0x00BFEA1A, CEventHitThing ctor 0x008110A0, CTCEntityEvents::AddEvent
//   0x008113C0. Layout: retail CHitParameters Decapitate +0x1b, Flourish +0x1d, PThingHitting +0x50, PWeapon +0x58; CThing
//   ThingTypeFlags +0x6c; CThingCreatureBase Health +0xb4, PLastAttackedByCreature +0xf4; CAbilityDef flags +0xac.
#define VF(n) virtual void v##n();
struct C3DVector {
    float X, Y, Z;
    C3DVector() { }
    __forceinline C3DVector(const C3DVector& v) : X(v.X), Y(v.Y), Z(v.Z) { }
};
struct CTCBase;
class CThing;

struct CRightHandedSet {
    unsigned char _data[0x18];
    CRightHandedSet(const CRightHandedSet& other);     // 0x0049CF80
};
struct CInterfaceMap {
    struct SEntry { long type; CTCBase* component; };
    SEntry* first;
    SEntry* last;
    SEntry* LowerBound(const long* type);               // 0x0040F020
    const SEntry* LowerBound(const long* type) const;   // 0x004365B0
};
struct CTCPhysicsBase {
    VF(00) VF(01) VF(02) VF(03) VF(04) VF(05) VF(06) VF(07) VF(08) VF(09)
    VF(10) VF(11) VF(12) VF(13) VF(14) VF(15) VF(16) VF(17) VF(18) VF(19)
    VF(20) VF(21) VF(22) VF(23) VF(24) VF(25) VF(26) VF(27) VF(28) VF(29)
    VF(30)
    virtual void SetPosition(const C3DVector& position);         // slot 31 (+0x7c)
    virtual void ResetRHSet(const CRightHandedSet& set);         // slot 32 (+0x80)
    VF(33) VF(34) VF(35) VF(36) VF(37) VF(38) VF(39)
    VF(40) VF(41) VF(42) VF(43) VF(44) VF(45) VF(46) VF(47) VF(48) VF(49)
    VF(50) VF(51) VF(52) VF(53) VF(54) VF(55) VF(56) VF(57) VF(58) VF(59)
    VF(60) VF(61) VF(62) VF(63) VF(64) VF(65) VF(66) VF(67) VF(68) VF(69)
    VF(70) VF(71)
    virtual const CRightHandedSet& GetLastRHSet() const;         // slot 72 (+0x120)
    unsigned char _pad04[0x0c - 0x04];
    C3DVector Position;                                 // +0x0c
};
struct CMessageEvent;
struct CMessageEventManager {
    CMessageEvent* AddMessage(long type, unsigned long delay, const C3DVector& position, CThing* from, CThing* to, float value1, float value2, long data);   // 0x00687540
};
struct CWorld {
    unsigned char _pad[0x60];
    CMessageEventManager* MessageEventManager;          // +0x60
};
extern long g_framesPerSecond;                          // 0x01375550

struct CDefVirtual {
    virtual ~CDefVirtual();                             // slot 0
    virtual void Destroy();                             // slot 1 (+4)
};
struct CDefBase : CDefVirtual {
    long m_refCount;                                    // +0x04
};
template<class T> struct CDefPointer {
    T* m_p;
    CDefPointer() : m_p(0) { }
};
struct CAbilityDef : CDefBase {
    unsigned char _pad08[0xac - 0x08];
    unsigned long Flags;                                // +0xac
};
struct CDefClassBase {
    bool GetSubDef(CDefPointer<CAbilityDef>& out);      // 0x00485815 (GetSubDef<CAbilityDef>)
};

#define VF64 VF(00) VF(01) VF(02) VF(03) VF(04) VF(05) VF(06) VF(07) VF(08) VF(09) \
    VF(10) VF(11) VF(12) VF(13) VF(14) VF(15) VF(16) VF(17) VF(18) VF(19) \
    VF(20) VF(21) VF(22) VF(23) VF(24) VF(25) VF(26) VF(27) VF(28) VF(29) \
    VF(30) VF(31) VF(32) VF(33) VF(34) VF(35) VF(36) VF(37) VF(38) VF(39) \
    VF(40) VF(41) VF(42) VF(43) VF(44) VF(45) VF(46) VF(47) VF(48) VF(49) \
    VF(50) VF(51) VF(52) VF(53) VF(54) VF(55) VF(56) VF(57) VF(58) VF(59) \
    VF(60) VF(61) VF(62) VF(63) VF(64)
struct CHitParameters;
class CThing {
public:
    VF64
    virtual void OnHit(CHitParameters& hit);            // slot 65 (+0x104)
    VF(66) VF(67) VF(68) VF(69) VF(70) VF(71) VF(72) VF(73)
    virtual bool IsNewAttackerMoreImportant(CThing* currentAttacker, CThing* newAttacker);   // slot 74 (+0x128, descriptive)
    unsigned char _pad04[0x20 - 0x04];
    unsigned long m_presentBits[9];                     // +0x20
    CInterfaceMap m_interfaces;                         // +0x44
    unsigned char _pad4c[0x60 - 0x4c];
    CTCPhysicsBase* PhysicsTC;                          // +0x60
    unsigned char _pad64[0x6c - 0x64];
    unsigned long ThingTypeFlags;                       // +0x6c
    CDefClassBase* PBaseDef;                            // +0x70
    unsigned char _pad74[0x91 - 0x74];
    unsigned char Dead : 1;                             // +0x91 bit 0
    const C3DVector& GetPosition() const;               // 0x004C73D0
    CWorld* PeekWorld();                                // 0x004C7AA0
    CTCPhysicsBase* GetPhysicsTC() const { return PhysicsTC; }
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
    __forceinline const CTCBase* GetConstTC(long type) const
    {
        long key = type;
        const CInterfaceMap* map = &m_interfaces;
        const CInterfaceMap::SEntry* it = map->LowerBound(&key);
        const CInterfaceMap::SEntry* end = map->last;
        return (it == end || type < it->type ? end : it)->component;
    }
};
struct CBaseIntelligentPointer {
    void* __vftable;
    CThing* m_pItem;
    CThing* GetPItem() const;                           // 0x00A01B50
};
struct CHitParameters {
    unsigned char _pad00[0x1b];
    bool Decapitate;                                    // +0x1b
    bool Blockable;                                     // +0x1c
    bool Flourish;                                      // +0x1d
    unsigned char _pad1e[0x50 - 0x1e];
    CBaseIntelligentPointer PThingHitting;              // +0x50
    CBaseIntelligentPointer PWeapon;                    // +0x58
};
struct CCreatureModeBase {
    unsigned char _pad00[0x8c];
    bool Finished;                                      // +0x8c (descriptive)
};
struct CModeBriarRoseConcentrate : CCreatureModeBase {
    long HasEnoughImposters();                          // 0x008E7E10 (landed as CIconText::HasEnoughIcons)
    CThing* GetRandomImposter();                        // 0x008E7F80
};
struct CTCCreatureModeManager {
    bool HasMode(unsigned long mode);                   // 0x0073A8A0
    CCreatureModeBase* GetCurrentMode();                // 0x0073A6E0
};
struct CFaction;
struct CTCFaction {
    unsigned char _pad00[0x18];
    CFaction* Faction;                                  // +0x18
};
class CThingAICreature;
struct COpinionDeed;
struct CTCHeroOpinionDeedLog {
    long FindDeed(long deedType, CThing* victim);       // 0x006C5800 (descriptive)
    long PostDeed(long deedType, const C3DVector& position, const CThing* target, CThingAICreature* victim,
                  long postType, bool b1, bool b2, const CFaction* faction, bool b3, long extra);   // 0x006C6990
    COpinionDeed* GetDeedFromHandle(long handle);       // 0x006C58F0
};
struct CTCCreatureOpinionOfHero {
    bool UpdateHitTolerance(bool armed);                // 0x006819D0
};
struct CEventBase;
struct CTCEntityEvents {
    void AddEvent(CEventBase* event);                   // 0x008113C0
};
struct CEventHitThing {
    unsigned char _body[0x18];
    CEventHitThing(CThing* hitter, CThing* target, bool enemies, bool melee, bool ranged, CThing* weapon, bool flourish, bool decapitate);   // 0x008110A0
};
bool __fastcall AreThingsEnemies(const CThing* a, const CThing* b);   // 0x0076D2F0
bool __fastcall IsThingCarryingMeleeWeapon(const CThing* thing);      // 0x006A15A0
bool __fastcall IsThingCarryingRangedWeapon(const CThing* thing);     // 0x006A1620

class CThingCreatureBase : public CThing {
public:
    unsigned char _pad94[0xb4 - 0x94];
    float Health;                                       // +0xb4
    unsigned char _padb8[0xf4 - 0xb8];
    CBaseIntelligentPointer PLastAttackedByCreature;    // +0xf4
    const C3DVector& GetCreaturePosition() const;       // 0x006A4D00
    void SetAttackedByCreature(CThing* attacker);       // 0x006624F0
    void OnHit(CHitParameters& hit);                    // 0x00667D30
};
class CThingAICreature : public CThingCreatureBase {
public:
    void OnHit(CHitParameters& hit);
};

void CThingAICreature::OnHit(CHitParameters& hit)
{
    if (HasTC(0x31)) {
        CTCCreatureModeManager* modes = (CTCCreatureModeManager*)GetTC(0x31);
        if (modes->HasMode(0x3f)) {
            CModeBriarRoseConcentrate* mode = (CModeBriarRoseConcentrate*)modes->GetCurrentMode();
            if (!mode->Finished && !mode->HasEnoughImposters()) {
                CThing* imposter = mode->GetRandomImposter();
                if (imposter && !imposter->Dead) {
                    CRightHandedSet imposterSet(imposter->PhysicsTC->GetLastRHSet());
                    C3DVector imposterPosition = imposter->GetPhysicsTC()->Position;
                    imposter->GetPhysicsTC()->ResetRHSet(PhysicsTC->GetLastRHSet());
                    imposter->PhysicsTC->SetPosition(PhysicsTC->Position);
                    PhysicsTC->ResetRHSet(imposterSet);
                    PhysicsTC->SetPosition(imposterPosition);
                    if (imposter->ThingTypeFlags & 4) {
                        imposter->OnHit(hit);
                        return;
                    }
                }
            }
        }
        if (modes->HasMode(0x51))
            return;
    }
    bool enemies = false;
    CThing* attacker = hit.PThingHitting.GetPItem();
    if (attacker && !attacker->Dead && (attacker->ThingTypeFlags & 8)) {
        long delay = g_framesPerSecond * 5;
        CMessageEventManager* manager = PeekWorld()->MessageEventManager;
        manager->AddMessage(0xc, delay, GetPosition(), this, attacker, 0.0f, 5.0f, 0);
        enemies = AreThingsEnemies(this, attacker);
        bool fatal = false;
        CDefPointer<CAbilityDef> ability;
        if (PBaseDef->GetSubDef(ability))
            fatal = Health <= 0.0f && (ability.m_p->Flags & 0x2018000) != 0;
        CThing* lastAttacker = PLastAttackedByCreature.GetPItem();
        if ((!lastAttacker || lastAttacker->Dead || IsNewAttackerMoreImportant(lastAttacker, attacker)) && !fatal) {
            SetAttackedByCreature(attacker);
            if (attacker->HasTC(0x51)) {
                CTCHeroOpinionDeedLog* deeds = (CTCHeroOpinionDeedLog*)attacker->GetTC(0x51);
                if (HasTC(0x49)) {
                    const CTCFaction* faction = (const CTCFaction*)GetConstTC(0x49);
                    if (faction->Faction && HasTC(0x4f)) {
                        CTCCreatureOpinionOfHero* opinion = (CTCCreatureOpinionOfHero*)GetTC(0x4f);
                        long deedType = hit.PWeapon.GetPItem() != attacker ? 8 : 7;
                        bool tolerated = opinion->UpdateHitTolerance(deedType == 8);
                        long previous = deeds->FindDeed(deedType, this);
                        if (previous == -1) {
                            long handle = deeds->PostDeed(deedType, GetCreaturePosition(), this, this, 1, tolerated, false, faction->Faction, false, 0);
                            if (deeds->GetDeedFromHandle(handle)) {
                                if (!tolerated)
                                    handle = -1;
                                deeds->PostDeed(0x35, GetCreaturePosition(), this, this, 2, false, false, faction->Faction, false, handle);
                            }
                        } else {
                            long handle = deeds->PostDeed(deedType, GetCreaturePosition(), this, this, 1, false, false, faction->Faction, false, 0);
                            if (deeds->GetDeedFromHandle(handle))
                                deeds->PostDeed(0x35, GetCreaturePosition(), this, this, 2, false, false, faction->Faction, false, previous);
                        }
                    }
                }
            }
        }
        CAbilityDef* def = ability.m_p;
        if (def && --def->m_refCount == 0)
            def->Destroy();
    }
    CThingCreatureBase::OnHit(hit);
    if (attacker && !attacker->Dead && (attacker->ThingTypeFlags & 8) && attacker->HasTC(0x42)) {
        CTCEntityEvents* events = (CTCEntityEvents*)attacker->GetTC(0x42);
        bool decapitate = hit.Decapitate;
        bool melee = IsThingCarryingMeleeWeapon(attacker);
        bool ranged = IsThingCarryingRangedWeapon(attacker);
        events->AddEvent((CEventBase*)new CEventHitThing(attacker, this, enemies, melee, ranged, hit.PWeapon.GetPItem(), hit.Flourish, decapitate));
    }
}
