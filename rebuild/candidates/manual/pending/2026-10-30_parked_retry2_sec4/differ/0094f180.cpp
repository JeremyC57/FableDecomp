// CAIStateGroup_BalverineCornered::IsRunnable() -- runnable when the creature has a target (or was just hit in melee by a live attacker) it can see, the jump-back point behind the creature is out of nav sight, the target is within the combat range, and a landing spot next to the target is reachable; records the landing spot, the jump time and the target
// Grade: DIFFER(736v754) nd=90 (was 737v754 nd=119) -- levers: every return false through one fail: label (retail pushes ebx/ebp/esi up front and shares the false exit), the attacker read through an inline CHitInfo accessor (lea ecx,[eax+50h]); remaining: frame 0x3c v 0x38 (every slot 4 higher: one extra 4-byte local), the jump-back vector math slots
// Retail: Fable.exe (TLC, VC7.1) @ 0x0094F180, 768 bytes. ?IsRunnable@CAIStateGroup_BalverineCornered@@MAE_NXZ (Ego_r.pdb).
// Callees: CInterfaceMap lower_bound 0x0040F020 / const 0x004365B0, interface 8 virtual slot 29 (+0x74),
//   CTCEntityEvents::GetEvent<CEventHitBy, CFilter_Bind2<HitByMelee, Blocked>> 0x0094F890, CEventHitBy hit-info accessor
//   0x00810F40, GetPItem 0x00A01B50, CanCreatureSeeCreature 0x008645E0 (__fastcall), CThing::GetPosition 0x004C73D0,
//   C3DVector::Normalise 0x00A14440, GetPWorld 0x008FCFC0 (+ its virtual slot 12, result unused),
//   CThingCreatureBase::GetCombatTypeDef 0x00666D10, CTCCreatureNavigation::IsPosNavLineOfSightTo 0x006CBA20 /
//   IsPosNavLineOfSightToPos 0x006CB610, GetLandPos 0x0094F090, CWorld::GetWorldFrame 0x0049D870,
//   CIntelligentPointer<const CThing>(CThing*) 0x00668B60, SetPItem 0x00A01B90, ~CBaseIntelligentPointer 0x00A01C10.
// Layout: engine/CAIStateGroup_BalverineCornered.h (PTarget +0x14, PLastJumpTarget +0x1c, TimeOfLastJump +0x24, LandPos +0x28)
//   modelled locally to declare members; thing def +0x60 (radius +0x34), creature-type flags +0x6c bit 3, combat def
//   +0x70 (retail operands; field names guessed).
struct CTCBase;
struct C3DVector {
    float X, Y, Z;
    C3DVector() { }
    C3DVector(float x, float y, float z) : X(x), Y(y), Z(z) { }
    C3DVector(const C3DVector& o) : X(o.X), Y(o.Y), Z(o.Z) { }
    C3DVector operator-(const C3DVector& o) const { return C3DVector(X - o.X, Y - o.Y, Z - o.Z); }
    C3DVector operator+(const C3DVector& o) const { return C3DVector(X + o.X, Y + o.Y, Z + o.Z); }
    C3DVector operator*(float s) const
    {
        C3DVector r;
        r.X = X * s;
        r.Y = Y * s;
        r.Z = Z * s;
        return r;
    }

    void Normalise();                           // 0x00A14440
};
struct CInterfaceMap {
    struct SEntry { long type; CTCBase* component; };
    SEntry* first;
    SEntry* last;
    SEntry* LowerBound(const long* type);       // 0x0040F020
    const SEntry* LowerBound(const long* type) const;   // 0x004365B0
};
#pragma auto_inline(off)
CInterfaceMap::SEntry* CInterfaceMap::LowerBound(const long* type)
{
    SEntry* it = first;
    int len = int(last - first);
    while (len > 0) {
        int half = len / 2;
        SEntry* middle = it + half;
        if (middle->type < *type) {
            it = middle + 1;
            len = len - half - 1;
        } else
            len = half;
    }
    return it;
}
const CInterfaceMap::SEntry* CInterfaceMap::LowerBound(const long* type) const
{
    const SEntry* it = first;
    int len = int(last - first);
    while (len > 0) {
        int half = len / 2;
        const SEntry* middle = it + half;
        if (middle->type < *type) {
            it = middle + 1;
            len = len - half - 1;
        } else
            len = half;
    }
    return it;
}
#pragma auto_inline(on)
struct CThing {
    unsigned char _pad0[0x20];
    unsigned long m_presentBits[9];             // +0x20
    CInterfaceMap m_interfaces;                 // +0x44
    unsigned char _pad4c[0x60 - 0x4c];
    struct CThingDef* PDef;                     // +0x60
    unsigned char _pad64[0x6c - 0x64];
    unsigned char m_flags6c;                    // +0x6c bit 3: creature
    unsigned char _pad6d[0x91 - 0x6d];
    unsigned char m_flags91;                    // +0x91 bit 0: dead
    const C3DVector& GetPosition() const;       // 0x004C73D0
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
        return (it == map->last || type < it->type ? map->last : it)->component;
    }
    __forceinline const CTCBase* GetTC(long type) const
    {
        long key = type;
        const CInterfaceMap* map = &m_interfaces;
        const CInterfaceMap::SEntry* it = map->LowerBound(&key);
        return (it == map->last || type < it->type ? map->last : it)->component;
    }
};
struct CThingDef {
    unsigned char _pad00[0x34];
    float CollisionRadius;                      // +0x34
};
struct CCombatTypeDef {
    unsigned char _pad00[0x70];
    float CorneredRange;                        // +0x70
};
struct CThingCreatureBase : CThing {
    CCombatTypeDef* GetCombatTypeDef();         // 0x00666D10
};
bool __fastcall CanCreatureSeeCreature(const CThing* viewer, const CThing* target, bool ignoreLight);   // 0x008645E0
struct CBaseIntelligentPointer {
    virtual void v00();
    CThing* m_pItem;
    ~CBaseIntelligentPointer();                 // 0x00A01C10
    CThing* GetPItem() const;                   // 0x00A01B50
    void SetPItem(CThing* item);                // 0x00A01B90
};
struct CIntelligentPointerConstThing : CBaseIntelligentPointer {
    CIntelligentPointerConstThing(const CThing* item);   // 0x00668B60
    CIntelligentPointerConstThing& operator=(const CIntelligentPointerConstThing& o) { SetPItem(o.GetPItem()); return *this; }
};
struct CTCTargeting {                           // interface 8
    virtual void v00(); virtual void v01(); virtual void v02(); virtual void v03(); virtual void v04(); virtual void v05();
    virtual void v06(); virtual void v07(); virtual void v08(); virtual void v09(); virtual void v0a(); virtual void v0b();
    virtual void v0c(); virtual void v0d(); virtual void v0e(); virtual void v0f(); virtual void v10(); virtual void v11();
    virtual void v12(); virtual void v13(); virtual void v14(); virtual void v15(); virtual void v16(); virtual void v17();
    virtual void v18(); virtual void v19(); virtual void v1a(); virtual void v1b(); virtual void v1c();
    virtual CThing* GetPTarget();               // slot 29 (+0x74)
};
struct CHitInfo {
    unsigned char _pad00[0x50];
    CBaseIntelligentPointer Attacker;           // +0x50
    CThing* GetAttacker() const { return Attacker.GetPItem(); }
};
namespace NEntityEvents {
struct CEventHitBy {
    const CHitInfo& GetHitInfo() const;         // 0x00810F40
};
struct CIsHitEventHitByMelee { CIsHitEventHitByMelee() { } };
struct CIsHitEventBlocked { CIsHitEventBlocked() { } };
}
template<class A, class B> struct CFilter_Bind2 {
    const A* First;
    const B* Second;
    CFilter_Bind2(const A& a, const B& b) : First(&a), Second(&b) { }
};
typedef CFilter_Bind2<NEntityEvents::CIsHitEventHitByMelee, NEntityEvents::CIsHitEventBlocked> CMeleeHitFilter;
struct CTCEntityEvents {                        // interface 0x42
    bool GetEvent(const NEntityEvents::CEventHitBy** out, const CMeleeHitFilter& filter);   // 0x0094F890
};
struct CTCCreatureNavigation {                  // interface 7
    bool IsPosNavLineOfSightTo(const C3DVector& pos) const;   // 0x006CBA20
    bool IsPosNavLineOfSightToPos(const C3DVector& from, const C3DVector& to) const;   // 0x006CB610
};
struct CWorld {
    static unsigned long GetWorldFrame();       // 0x0049D870
};
struct CWorldObject {
    virtual void v00(); virtual void v01(); virtual void v02(); virtual void v03(); virtual void v04(); virtual void v05();
    virtual void v06(); virtual void v07(); virtual void v08(); virtual void v09(); virtual void v0a(); virtual void v0b();
    virtual void* GetDebugGraphics();           // slot 12 (+0x30)
};
struct CAIBrain {
    unsigned char _pad00[0x18];
    CThingCreatureBase* Creature;               // +0x18
};
struct CAIStateGroupBase {
    virtual void v00();
    CAIBrain* PBrain;                           // +0x04
    unsigned char _pad08[0x0c];
    CThingCreatureBase* GetCreature() const { return PBrain->Creature; }
    CWorldObject* GetPWorld() const;            // 0x008FCFC0
};
struct CAIStateGroup_BalverineCornered : CAIStateGroupBase {
    CIntelligentPointerConstThing PTarget;      // +0x14
    CIntelligentPointerConstThing PLastJumpTarget;   // +0x1c
    unsigned long TimeOfLastJump;               // +0x24
    C3DVector LandPos;                          // +0x28
    C3DVector GetLandPos(const CThing& target) const;   // 0x0094F090
    bool IsRunnable();
};

bool CAIStateGroup_BalverineCornered::IsRunnable()
{
    CThingCreatureBase* creature = GetCreature();
    if (creature->HasTC(8)) {
    CThing* target = ((CTCTargeting*)creature->GetTC(8))->GetPTarget();
    bool hitByMelee = false;
    CThingCreatureBase* self = GetCreature();
    if (self->HasTC(0x42)) {
        CTCEntityEvents* events = (CTCEntityEvents*)self->GetTC(0x42);
        const NEntityEvents::CEventHitBy* event;
        if (events->GetEvent(&event,
                CMeleeHitFilter(NEntityEvents::CIsHitEventHitByMelee(), NEntityEvents::CIsHitEventBlocked()))) {
            CThing* attacker = event->GetHitInfo().GetAttacker();
            if (attacker && !(attacker->m_flags91 & 1) && (attacker->m_flags6c & 8)) {
                target = attacker;
                hitByMelee = true;
            }
        }
    }
    if (!target || (target->m_flags91 & 1))
        goto fail;
    if (!hitByMelee && !CanCreatureSeeCreature(GetCreature(), target, false))
        goto fail;
    CThingCreatureBase* me = GetCreature();
    C3DVector away = me->GetPosition() - target->GetPosition();
    away.Normalise();
    CThingCreatureBase* jumper = GetCreature();
    C3DVector offset = away * (jumper->PDef->CollisionRadius + jumper->PDef->CollisionRadius);
    C3DVector jumpBack = offset + jumper->GetPosition();
    GetPWorld()->GetDebugGraphics();
    const CThingCreatureBase* walker = GetCreature();
    const CTCCreatureNavigation* nav;
    if (walker->HasTC(7))
        nav = (const CTCCreatureNavigation*)walker->GetTC(7);
    const C3DVector& targetPos = target->GetPosition();
    const C3DVector& walkerPos = walker->GetPosition();
    float dx = walkerPos.X - targetPos.X;
    float dy = walkerPos.Y - targetPos.Y;
    CCombatTypeDef* combat = GetCreature()->GetCombatTypeDef();
    if (!hitByMelee) {
        float range = combat->CorneredRange;
        if (range * range <= dy * dy + dx * dx)
            goto fail;
        if (nav->IsPosNavLineOfSightTo(jumpBack))
            goto fail;
    }
    C3DVector landPos = GetLandPos(*target);
    if (!nav->IsPosNavLineOfSightToPos(target->GetPosition(), landPos))
        goto fail;
    LandPos = landPos;
    TimeOfLastJump = CWorld::GetWorldFrame();
    PLastJumpTarget = CIntelligentPointerConstThing(target);
    PTarget = CIntelligentPointerConstThing(target);
    return true;
    }
    goto fail;
fail:
    return false;
}
