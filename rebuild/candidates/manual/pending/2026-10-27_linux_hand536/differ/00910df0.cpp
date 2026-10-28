// CAIStateGroup_MinionGuardHeardYellForHelp::Update() -- find the latest yell-for-help message (type 0xb) from a living originator the guard can see or hear, consume it, look at the yelling creature (FOV/360 turn speed), run to within 3 units of it and, when the hero is alive, declare him a criminal; terminate when done or when the yeller dies
// Grade: DIFFER(500v500) -- residue: retail's inlined CMessageEvent::GetPOriginator<CThingCreatureBase> type test is `xor eax,0xf ; test al,0xf` (its own out-of-line copy 0x00910CC0 and every source form give `not eax` or `and/cmp`; retail inlines use xor at all 7 sites: 0x0090D030 x2, 0x0090E5D0, 0x00940BF0 x3) and the brain load before GetFOV goes through eax (ours ecx,ecx) | tried: 2026-10-21 (f&0xf)==0xf, ~f&0xf, (f^0xf)&0xf, 4-bit bitfield, IsThingOfType<T> mask template, four bit tests; 2026-10-28 real-name rewrite (PDB filters, CIntelligentPointer<const CThingCreatureBase> per RTTI 0x0125AAEC, GetCreature() by reference, GetFOV slot 92) with `!(~f & mask)`, IsThingOfType mask local, `((f ^ m) & m) == 0`, an originator local; FOV via *(1/360), PBrain->Creature, PBrain->GetCreature() | next: lane gap (compiler choice of xor vs not at inline sites); compare with a landed inline-site match if one appears
// Leaf: ?Update@CAIStateGroup_MinionGuardHeardYellForHelp@@UAEXXZ
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00910DF0, length 500 bytes (+ int3 padding), plain `ret` (virtual), `and esp,-8` frame 0xd0.
// Callees (Ego_r.pdb / FableWin.pdb spellings): CAIStateGroupBase::PeekWorld 0x008FCFA0 / GetWorld 0x008FCFC0 / GetPlayerManager
//   0x008FCF90 (donor order: Terminate 0x008FCF40 then ClearTerminateFlag, GetThingManager, PeekThingManager, PeekPlayerManager,
//   GetPlayerManager, PeekWorld at 0x10 steps) / Terminate 0x008FCF40 / AIAction_LookAtThing 0x0097D6F0 / AIAction_MoveToThingProximity
//   0x0097E380 / AIAction_PerformActionAndWait 0x0097DB70; CMessageEventManager::FindMostRecentMessage<CFilter_Bind3<IsOfType,
//   IsOriginatorAlive, CanBeSeenOrHeard>> 0x00910CF0 / RemoveMessage 0x00493B5B; CBaseIntelligentPointer ctor 0x00A01B10 / GetPItem
//   0x00A01B50 / SetPItem 0x00A01B90 / dtor 0x00A01C10; CPlayerManager::GetMainPlayer 0x00449970; CPlayer::GetPControlledCreature
//   0x00487DC0; CCreatureAction_DeclarePlayerACriminal ctor 0x00910C30 / dtor 0x00910B20; CThingAICreature::GetFOV (slot 92, 0x008302F0:
//   `[[this+0xE0]+0xBC]` = CThingCreatureBase::PDef (Ego_r +0xFC less 0x1C) -> CThingCreatureDef::FOV (Ego_r +0xC0 less 4)).
// Vtables: CIntelligentPointer<const CThingCreatureBase> 0x0125AAEC, CIntelligentPointer<const CThing> 0x01237F1C (RTTI type names).
// Data: 1/360 @ 0x01238E00 (from `/ 360.0f`); 3.0f immediate.
// Layout: CMessageEvent Type +8, POriginator +0x48 (Ego_r); CWorld PMessageEventManager +0x60; CAIBrain Creature +0x18; CThing
//   ThingTypeFlags +0x6C, Dead +0x91 bit 0 (Ego_r less 0x10); CCreatureActionBase retail 0xA8 bytes (opaque), PCriminal after it.
class CBaseObject { public: virtual ~CBaseObject(); };
class CBaseObjectPointer;
class CBaseIntelligentPointer {
public:
    CBaseIntelligentPointer();                              // 0x00A01B10
    virtual ~CBaseIntelligentPointer();                     // 0x00A01C10
    CBaseObject* GetPItem() const;                          // 0x00A01B50
    void SetPItem(CBaseObject* item);                       // 0x00A01B90
private:
    CBaseObjectPointer* PData;                              // +0x04
};
template <class T> class CIntelligentPointer : public CBaseIntelligentPointer {
public:
    CIntelligentPointer(T* item) { SetPItem((CBaseObject*)item); }
    template <class U> CIntelligentPointer(CIntelligentPointer<U>& other) { SetPItem((CBaseObject*)other.Get()); }
    T* Get() const { return (T*)GetPItem(); }
    T* operator->() const { return Get(); }
};
template <class T> inline bool operator!=(const CIntelligentPointer<T>& p, int) { return p.GetPItem() != 0; }

#define VS(n) virtual void _vslot##n();
#define VS10(p) VS(p##0) VS(p##1) VS(p##2) VS(p##3) VS(p##4) VS(p##5) VS(p##6) VS(p##7) VS(p##8) VS(p##9)
class CThing : public CBaseObject {                         // rtti_bases: CThing : CClass, CBaseObject, CBaseClass (all @0)
public:
    VS10(0) VS(10) VS(11) VS(12) VS(13) VS(14) VS(15) VS(16) VS(17) VS(18) VS(19)
    VS10(2) VS10(3) VS10(4) VS10(5) VS10(6) VS10(7) VS10(8) VS(90)
    enum { ThingTypeMask = 0 };
    unsigned char _pad04[0x6c - 0x04];
    unsigned long ThingTypeFlags;                           // +0x6c
    unsigned char _pad70[0x91 - 0x70];
    unsigned char Dead : 1;                                 // +0x91 bit 0
    bool IsDead() const { return Dead != 0; }
};
class CThingCreatureBase : public CThing { public: enum { ThingTypeMask = 0xf }; };
class CThingPlayerCreature : public CThingCreatureBase { };
class CThingAICreature : public CThingCreatureBase {
public:
    enum { ThingTypeMask = 0x1f };
    virtual float GetFOV() const;                           // slot 92 (+0x170), 0x008302F0
};

enum EMessageEventType { };
class CMessageEvent {
public:
    unsigned char               _pad00[0x08];
    EMessageEventType           Type;                       // +0x08
    unsigned char               _pad0c[0x48 - 0x0c];
    CIntelligentPointer<CThing> POriginator;                // +0x48
    template <class T> T* GetPOriginator() const
    {
        T* result = 0;
        if (POriginator.Get()) {
            if (!(~POriginator.Get()->ThingTypeFlags & T::ThingTypeMask))
                result = (T*)POriginator.Get();
        }
        return result;
    }
};

class CMessageEventFilter_IsOfType { public:
    EMessageEventType Type;
    CMessageEventFilter_IsOfType(EMessageEventType type) : Type(type) { }
};
class CMessageEventFilter_IsOriginatorAlive { public:
    CMessageEventFilter_IsOriginatorAlive() { }
};
class CMessageEventFilter_CanBeSeenOrHeard { public:
    const CThingAICreature* PCreature;
    CMessageEventFilter_CanBeSeenOrHeard(const CThingAICreature& creature) : PCreature(&creature) { }
};
template <class A, class B> class CFilter_Bind2 { public:
    const A* PFirst;
    const B* PSecond;
    CFilter_Bind2(const A& a, const B& b) : PFirst(&a), PSecond(&b) { }
};
template <class A, class B, class C> class CFilter_Bind3 { public:
    const A* PFirst;
    const B* PSecond;
    const C* PThird;
    CFilter_Bind3(const CFilter_Bind2<A, B>& ab, const C& c) : PFirst(ab.PFirst), PSecond(ab.PSecond), PThird(&c) { }
};
template <class A, class B> inline CFilter_Bind2<A, B> operator&&(const A& a, const B& b) { return CFilter_Bind2<A, B>(a, b); }
template <class A, class B, class C> inline CFilter_Bind3<A, B, C> operator&&(const CFilter_Bind2<A, B>& ab, const C& c) { return CFilter_Bind3<A, B, C>(ab, c); }

class CMessageEventManager {
public:
    template <class F> const CMessageEvent* FindMostRecentMessage(const F& filter) const;
    void RemoveMessage(const CMessageEvent* message);        // 0x00493B5B
};
template <class T> class CCountedPointer { public: T* Data; void* Info; };
class CWorld {
public:
    unsigned char                           _pad00[0x60];
    CCountedPointer<CMessageEventManager>   PMessageEventManager;   // +0x60
    CMessageEventManager& GetMessageEventManager() { return *PMessageEventManager.Data; }
    const CMessageEventManager& GetMessageEventManager() const { return *PMessageEventManager.Data; }
};
class CPlayer { public: CThingPlayerCreature* GetPControlledCreature() const; };   // 0x00487DC0
class CPlayerManager { public: CPlayer& GetMainPlayer(); };                          // 0x00449970

class CCreatureActionBase {
public:
    virtual ~CCreatureActionBase();
    unsigned char _pad04[0xa8 - 0x04];
};
class CCreatureAction_DeclarePlayerACriminal : public CCreatureActionBase {
public:
    CCreatureAction_DeclarePlayerACriminal(CThingCreatureBase& creature, CThingPlayerCreature& player);   // 0x00910C30
    ~CCreatureAction_DeclarePlayerACriminal();                                                            // 0x00910B20
    CIntelligentPointer<CThingPlayerCreature> PCriminal;
};
class CAIBrain {
public:
    unsigned char     _pad00[0x18];
    CThingAICreature* Creature;                             // +0x18
    CThingAICreature& GetCreature() const { return *Creature; }
};
enum EAIActionReturn { };
enum EAIActionRepeat { };
enum EMovementType { };
class CAIStateGroupBase {
public:
    virtual ~CAIStateGroupBase();
    CAIBrain* PBrain;                                       // +0x04
    unsigned char _pad08[0x14 - 0x08];
protected:
    CThingAICreature& GetCreature() const { return PBrain->GetCreature(); }
    CPlayerManager& GetPlayerManager() const;               // 0x008FCF90
    const CWorld& PeekWorld() const;                        // 0x008FCFA0
    CWorld& GetWorld() const;                               // 0x008FCFC0
public:
    void Terminate();                                       // 0x008FCF40
protected:
    EAIActionReturn AIAction_LookAtThing(EAIActionRepeat repeat, CIntelligentPointer<const CThing> thing, long frames, float speed);   // 0x0097D6F0
    EAIActionReturn AIAction_MoveToThingProximity(EAIActionRepeat repeat, CIntelligentPointer<const CThing> thing, float proximity,
        bool& arrived, EMovementType movement, bool a, bool b, bool c, bool d, float e);   // 0x0097E380
    EAIActionReturn AIAction_PerformActionAndWait(const CCreatureActionBase& action, bool wait);   // 0x0097DB70
};

class CAIStateGroup_MinionGuardHeardYellForHelp : public CAIStateGroupBase {
public:
    virtual void Update();
};

void CAIStateGroup_MinionGuardHeardYellForHelp::Update()
{
    const CMessageEvent* message = PeekWorld().GetMessageEventManager().FindMostRecentMessage(
        CMessageEventFilter_IsOfType((EMessageEventType)0xb) && CMessageEventFilter_IsOriginatorAlive() &&
        CMessageEventFilter_CanBeSeenOrHeard(GetCreature()));
    if (message) {
        CIntelligentPointer<const CThingCreatureBase> yeller(message->GetPOriginator<CThingCreatureBase>());
        GetWorld().GetMessageEventManager().RemoveMessage(message);
        if (AIAction_LookAtThing((EAIActionRepeat)1, yeller, 0x23, GetCreature().GetFOV() / 360.0f) != 2) {
            Terminate();
            return;
        }
        bool arrived = false;
        for (;;) {
            if (!(yeller != 0 && !yeller->IsDead())) {
                Terminate();
                return;
            }
            EAIActionReturn result = AIAction_MoveToThingProximity((EAIActionRepeat)2, yeller, 3.0f, arrived,
                (EMovementType)5, true, false, true, true, 0.0f);
            if (result == 4)
                continue;
            if (result != 2) {
                Terminate();
                return;
            }
            break;
        }
        CThingPlayerCreature* hero = GetPlayerManager().GetMainPlayer().GetPControlledCreature();
        if (hero != 0 && !hero->IsDead())
            AIAction_PerformActionAndWait(CCreatureAction_DeclarePlayerACriminal(GetCreature(), *hero), false);
    }
    Terminate();
}
