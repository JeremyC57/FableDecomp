// CAIStateGroup_CreatureRangedCombat2::Update() -- ranged combat against a live target: read the creature's extended sight radius, drop the strafe mode; without a ranged weapon unsheathe one and wait; if the target is reachable close in (within 95% of the sight radius and in nav line of sight) skip the approach, else walk towards it; then add the strafe mode, ready a carried projectile weapon and pump combat actions while the target stays the best one, firing is possible and it stays out of reach but in line of sight
// Grade: DIFFER(1059v1047) -- first pass: flow, calls and const/non-const LowerBound routing follow retail; retail keeps the reachable flag in the TestTC flag byte (0x12) and the arrival flag at 0x13 (here swapped, so the in-range loop spills the target pointer), plus a hoisted brain load before the melee-weapon branch
// Retail: Fable.exe (TLC, VC7.1) @ 0x0091EC30, 1047 bytes, plain ret.
// Callees: CBaseIntelligentPointer::GetPItem 0x00A01B50 / ctor 0x00A01B10 / SetPItem 0x00A01B90 (CIntelligentPointer<const
//   CThing> vtable 0x01237F1C built in the argument slot), interface LowerBound 0x0040F020 / const 0x004365B0 (types 0x34
//   perceive, 0x31 mode manager, 7 navigation, 0x46 carrying, 0x48 weapon), CTCPerceiveThing::GetExtendedSightRadius 0x00744950,
//   CTCCreatureModeManager::RemoveMode 0x0073ABA0 / AddMode 0x0073AE10, IsThingCarryingMeleeWeapon 0x006A1620 (__fastcall),
//   CThingCreatureBase::GetCombatManager 0x00661ED0, CCombatManager::CreatureUnsheathWeapon 0x006EBDD0, CThing::GetPosition
//   0x004C73D0, CTCCreatureNavigation::IsAbleToNavigateFromPosToPos 0x006CB620 / IsPosNavLineOfSightTo 0x006CBA20,
//   IsBestTargetForAttack 0x00864840 (__fastcall), IsAbleToFire 0x0091EA70, AIAction_MoveToThingProximity 0x0097E380,
//   CTCCarrying::GetThingInPrimarySlot 0x006A1440, CTCWeapon::IsProjectileWeapon 0x005DAA40 / ReadyProjectile 0x005DA4E0,
//   AIAction_PumpCombatActions 0x0097D980, Terminate 0x008FCF40. Constant 0.95f @ 0x0125C0B8.
#include "engine/CAIStateGroup_CreatureRangedCombat2.h"

struct CTCBase;
struct C3DVector { float X, Y, Z; };
struct CInterfaceMap {
    struct SEntry { long type; CTCBase* component; };
    SEntry* first;
    SEntry* last;
    SEntry* LowerBound(const long* type);               // 0x0040F020
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
    unsigned long m_presentBits[9];             // +0x20 std::bitset of interface types
    CInterfaceMap m_interfaces;                 // +0x44
    unsigned char _pad4c[0x91 - 0x4c];
    unsigned char m_flags91;                    // +0x91 bit 0: dead
    const C3DVector& GetPosition() const;       // 0x004C73D0
    __forceinline bool HasTC(long type) const
    {
        if (m_presentBits[type >> 5] & (1UL << (type & 31)))
            return true;
        return false;
    }
    __forceinline bool TestTC(long type) const { return ((m_presentBits[type >> 5] >> (type & 31)) & 1) != 0; }
    __forceinline CTCBase* GetTC(long type)
    {
        long key = type;
        CInterfaceMap* map = &m_interfaces;
        CInterfaceMap::SEntry* it = map->LowerBound(&key);
        return (it == map->last || type < it->type ? map->last : it)->component;
    }
    __forceinline CTCBase* GetTCConst(long type) const
    {
        long key = type;
        const CInterfaceMap* map = &m_interfaces;
        const CInterfaceMap::SEntry* it = map->LowerBound(&key);
        return (it == map->last || type < it->type ? map->last : it)->component;
    }
};
struct CBaseIntelligentPointer {
    virtual void v00();
    CThing* m_pItem;
    CBaseIntelligentPointer();                  // 0x00A01B10
    ~CBaseIntelligentPointer();                 // 0x00A01C10
    CThing* GetPItem() const;                   // 0x00A01B50
    void SetPItem(const CThing* item);          // 0x00A01B90
};
struct CIntelligentPointerConstThing : CBaseIntelligentPointer {   // CIntelligentPointer<const CThing>, vtable 0x01237F1C
    virtual void v00();
    __forceinline CIntelligentPointerConstThing(const CBaseIntelligentPointer& other) { SetPItem(other.GetPItem()); }
};
__forceinline bool IsAlive(const CBaseIntelligentPointer& p)
{
    return p.GetPItem() != 0 && !(p.GetPItem()->m_flags91 & 1);
}
struct CTCPerceiveThing {
    float GetExtendedSightRadius();             // 0x00744950
};
struct CTCCreatureModeManager {
    void RemoveMode(unsigned long mode);        // 0x0073ABA0
    bool AddMode(unsigned long mode);           // 0x0073AE10
};
struct CTCCreatureNavigation {
    bool IsAbleToNavigateFromPosToPos(const C3DVector& from, const C3DVector& to, float tolerance);   // 0x006CB620
    bool IsPosNavLineOfSightTo(const C3DVector& pos);   // 0x006CBA20
};
struct CTCCarrying {
    CThing* GetThingInPrimarySlot();            // 0x006A1440
};
struct CTCWeapon {
    bool IsProjectileWeapon();                  // 0x005DAA40
    void ReadyProjectile();                     // 0x005DA4E0
};
struct CThingCreatureBase;
struct CCombatManager {
    void CreatureUnsheathWeapon(CThingCreatureBase* creature, long type);   // 0x006EBDD0
};
struct CThingCreatureBase : CThing {
    CCombatManager* GetCombatManager();         // 0x00661ED0
};
bool __fastcall IsThingCarryingMeleeWeapon(const CThing* thing);   // 0x006A1620
bool __fastcall IsBestTargetForAttack(CThing* creature, CThing* target);   // 0x00864840
struct CAIBrain {
    unsigned char _pad[0x18];
    CThingCreatureBase* PCreature;              // +0x18
};
extern float g_RangedApproachFraction;          // 0x0125C0B8 (0.95f)

struct CAIStateGroupCreatureRangedCombat2Methods : CAIStateGroup_CreatureRangedCombat2 {
    CAIBrain* Brain() const { return *(CAIBrain* const*)_pad_0x04; }
    CThingCreatureBase* GetCreature() const { return Brain()->PCreature; }
    CBaseIntelligentPointer& Target() { return *(CBaseIntelligentPointer*)PTargetThing; }
    void Terminate();                           // 0x008FCF40
    bool IsAbleToFire();                        // 0x0091EA70
    long AIAction_PumpCombatActions();          // 0x0097D980
    long AIAction_MoveToThingProximity(long repeat, CIntelligentPointerConstThing thing, float proximity, bool* arrived,
        long movement, bool a, bool b, bool c, bool d, float e);   // 0x0097E380
    void Update();
};

void CAIStateGroupCreatureRangedCombat2Methods::Update()
{
    if (!IsAlive(Target())) {
        Terminate();
        return;
    }
    bool reachable;
    bool flag;
    CThingCreatureBase* creature = GetCreature();
    CTCPerceiveThing* perceive;
    flag = creature->TestTC(0x34);
    if (flag)
        perceive = (CTCPerceiveThing*)creature->GetTCConst(0x34);
    float sightRadius = perceive->GetExtendedSightRadius();
    CThingCreatureBase* moder = GetCreature();
    flag = moder->TestTC(0x31);
    if (flag) {
        ((CTCCreatureModeManager*)moder->GetTC(0x31))->RemoveMode(0x19);
        AddedStrafe = false;
    }
    if (!IsThingCarryingMeleeWeapon(GetCreature())) {
        GetCreature()->GetCombatManager()->CreatureUnsheathWeapon(GetCreature(), 5);
        return;
    }
    CThingCreatureBase* walker = GetCreature();
    CTCCreatureNavigation* navigation;
    flag = walker->TestTC(7);
    if (flag)
        navigation = (CTCCreatureNavigation*)walker->GetTCConst(7);
    reachable = navigation->IsAbleToNavigateFromPosToPos(walker->GetPosition(), Target().GetPItem()->GetPosition(), 2.0f);
    if (reachable) {
        long result;
        do {
            if (!IsAlive(Target()) || !IsBestTargetForAttack(GetCreature(), Target().GetPItem()) || !IsAbleToFire()) {
                Terminate();
                return;
            }
            CThingCreatureBase* me = GetCreature();
            CThing* target = Target().GetPItem();
            const C3DVector& from = me->GetPosition();
            const C3DVector& to = target->GetPosition();
            float dy = to.Y - from.Y;
            float dx = to.X - from.X;
            float near = g_RangedApproachFraction * sightRadius;
            if (dx * dx + dy * dy < near * near && navigation->IsPosNavLineOfSightTo(Target().GetPItem()->GetPosition()))
                break;
            result = AIAction_MoveToThingProximity(2, Target(), 2.0f, &flag, 5, true, false, true, true, 0.0f);
        } while (result == 4);
        if (result == 1) {
            Terminate();
            return;
        }
    }
    CThingCreatureBase* strafer = GetCreature();
    if (strafer->HasTC(0x31)) {
        ((CTCCreatureModeManager*)strafer->GetTC(0x31))->AddMode(0x19);
        AddedStrafe = true;
    }
    CThingCreatureBase* carrier = GetCreature();
    if (carrier->HasTC(0x46)) {
        CThing* item = ((CTCCarrying*)carrier->GetTCConst(0x46))->GetThingInPrimarySlot();
        if (item && !(item->m_flags91 & 1) && item->HasTC(0x48)) {
            CTCWeapon* weapon = (CTCWeapon*)item->GetTC(0x48);
            if (weapon->IsProjectileWeapon())
                weapon->ReadyProjectile();
        }
    }
    for (;;) {
        if (!IsAlive(Target()) || !IsBestTargetForAttack(GetCreature(), Target().GetPItem()) || !IsAbleToFire())
            break;
        CThingCreatureBase* me = GetCreature();
        CThing* target = Target().GetPItem();
        const C3DVector& to = target->GetPosition();
        const C3DVector& from = me->GetPosition();
        float dx = from.X - to.X;
        float dy = from.Y - to.Y;
        if (reachable && dx * dx + dy * dy <= sightRadius * sightRadius)
            return;
        if (!navigation->IsPosNavLineOfSightTo(Target().GetPItem()->GetPosition()))
            return;
        if (AIAction_PumpCombatActions() == 1)
            break;
    }
    Terminate();
}
