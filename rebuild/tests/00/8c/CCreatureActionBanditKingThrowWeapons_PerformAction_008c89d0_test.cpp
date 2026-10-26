// Behaviour test for 0x008C89D0 CCreatureAction_BanditKingThrowWeapons::PerformAction: runs the reconstructed body over
// stub things, carry slots, defs and shots; checks the dead/absent creature early outs, the right- then left-hand slot
// lookups by name, the throw of each living removed thing (CreateShot arguments: thing, shooter, origin = thing, target,
// accuracy as float, 1, 1, ability out, -1), the right-hand weapon def index stored into the boss component (only for
// slot 0, only when the def and the component exist), the def and ability releases, and the shot's physics state 2 and
// navigator enable.
#include <cstdio>
#include <cstring>

#ifndef _MSC_VER
#define __forceinline inline
#define __fastcall
#endif

static int g_failures = 0;
#define CHECK(c) do { if (!(c)) { std::printf("FAIL line %d: %s\n", __LINE__, #c); ++g_failures; } } while (0)

class CTCBase { public: };
class CCharString { public:
    char Text[64];
    CCharString(const char* text, long length = -1) { (void)length; std::strncpy(Text, text, 63); Text[63] = 0; }
};
class CDefinitionManager { public:
    long GetDefGlobalIndexFromName(const CCharString& name)
    {
        if (std::strcmp(name.Text, "CARRY_SLOT_RIGHT_HAND") == 0) return 11;
        if (std::strcmp(name.Text, "CARRY_SLOT_LEFT_HAND") == 0) return 22;
        return -5;
    }
};
struct CInterfaceMap {
    struct SEntry { long type; CTCBase* component; };
    SEntry  entries[8];
    SEntry* first;
    SEntry* last;
    SEntry* LowerBound(const long* type)
    {
        SEntry* it = first;
        while (it != last && it->type < *type)
            ++it;
        return it;
    }
};
struct CDefBase {
    int  Destroyed;
    long RefCount;
    CDefBase() : Destroyed(0), RefCount(1) { }
    virtual ~CDefBase() { }
    virtual void Destroy() { ++Destroyed; }
};
class CWeaponDef : public CDefBase { public:
    long GlobalIndex;
};
template<class T> class CDefPointer { public:
    T* Object;
    CDefPointer() : Object(0) { }
    ~CDefPointer() { if (Object) { if (--Object->RefCount == 0) Object->Destroy(); } }
    T* operator->() const { return Object; }
};
class CDefClassBase : public CDefBase { public:
    CWeaponDef* Weapon;
    CDefClassBase() : Weapon(0) { }
    bool GetSubDef(CDefPointer<CWeaponDef>* out)
    {
        if (!Weapon) return false;
        ++Weapon->RefCount;
        out->Object = Weapon;
        return true;
    }
};
struct CRefCounted {
    long m_count;
    void DecRef() { --m_count; }
    long GetRefCount() const { return m_count; }
};
struct CDefVirtual {
    int Destroyed;
    CDefVirtual() : Destroyed(0) { }
    virtual ~CDefVirtual() { }
    virtual void Destroy() { ++Destroyed; }
};
class CMeleeCombatAbilityDef : public CDefVirtual, public CRefCounted { public:
    void ReleaseTemporary()
    {
        DecRef();
        if (GetRefCount() == 0)
            Destroy();
    }
};
template <class T> struct CAbilityPointer {
    T* m_p;
    CAbilityPointer() : m_p(0) { }
    ~CAbilityPointer()
    {
        if (m_p) {
            m_p->ReleaseTemporary();
            m_p = 0;
        }
    }
};
class CTCPhysicsBase { public:
    long State;
    int  Calls;
    CTCPhysicsBase() : State(0), Calls(0) { }
    virtual ~CTCPhysicsBase() { }
    virtual void SetPhysicsState(long state) { ++Calls; State = state; }
};
struct CTCPhysicsNavigator : CTCBase {
    int  Calls;
    bool Enabled;
    CTCPhysicsNavigator() : Calls(0), Enabled(false) { }
    void EnableNavigator(bool enable) { ++Calls; Enabled = enable; }
};
struct CTCBanditKingWeapons : CTCBase {
    long m_weaponDef;
    CTCBanditKingWeapons() : m_weaponDef(-99) { }
};
class CThing { public:
    unsigned long   m_presentBits[9];
    CInterfaceMap   m_interfaces;
    CTCPhysicsBase* m_pPhysics;
    CDefClassBase*  m_pDef;
    unsigned char   m_flags91;
    CThing() : m_pPhysics(0), m_pDef(0), m_flags91(0)
    {
        std::memset(m_presentBits, 0, sizeof(m_presentBits));
        m_interfaces.first = m_interfaces.last = m_interfaces.entries;
    }
    void AddInterface(long type, CTCBase* component)
    {
        m_presentBits[type >> 5] |= 1UL << (type & 31);
        CInterfaceMap::SEntry* it = m_interfaces.last++;
        while (it != m_interfaces.first && (it - 1)->type > type) { *it = *(it - 1); --it; }
        it->type = type;
        it->component = component;
    }
    CTCPhysicsBase* GetPhysics() const { return m_pPhysics; }
    template<class T> __forceinline bool GetInterface(T** ppOut, long interfaceType)
    {
        if (m_presentBits[interfaceType >> 5] & (1UL << (interfaceType & 31))) {
            long type = interfaceType;
            CInterfaceMap* map = &m_interfaces;
            CInterfaceMap::SEntry* it = map->LowerBound(&type);
            if (it == map->last || interfaceType < it->type)
                it = map->last;
            *ppOut = (T*)it->component;
            return true;
        }
        return false;
    }
};
class CThingCreatureBase : public CThing { public:
    long Accuracy;
    CThingCreatureBase() : Accuracy(0) { }
    long GetShotAccuracyPercentage() { return Accuracy; }
};
class CTCCarrying : public CTCBase { public:
    CThing* Slot11;
    CThing* Slot22;
    int     Calls;
    long    Order[4];
    CTCCarrying() : Slot11(0), Slot22(0), Calls(0) { }
    CThing* RemoveThingInCarrySlot(long slot, bool drop)
    {
        CHECK(!drop);
        Order[Calls++ & 3] = slot;
        CThing* t = 0;
        if (slot == 11) { t = Slot11; Slot11 = 0; }
        if (slot == 22) { t = Slot22; Slot22 = 0; }
        return t;
    }
};

struct SShotCall {
    CThing* Shot; CThing* Shooter; CThing* Origin; CThing* Target;
    float Accuracy, A, B; long D;
};
static SShotCall g_shots[4];
static int       g_shotCalls = 0;
static CThing*   g_shotResult[4];
static CMeleeCombatAbilityDef* g_ability = 0;   // when set, CreateShot hands out one reference to it
CThing* __fastcall CreateShot(CThing* shot, CThing* shooter, CThing* origin, CThing* target, float accuracy, float a, float b,
                              CAbilityPointer<CMeleeCombatAbilityDef>* ability, long d)
{
    SShotCall c = { shot, shooter, origin, target, accuracy, a, b, d };
    int n = g_shotCalls++;
    g_shots[n & 3] = c;
    if (g_ability) {
        ++g_ability->m_count;
        ability->m_p = g_ability;
    }
    return g_shotResult[n & 3];
}

struct CBaseIntelligentPointer {
    CThingCreatureBase* m_pItem;
    CThingCreatureBase* GetPItem() const { return m_pItem; }
};
__forceinline bool IS_THING_ALIVE(const CBaseIntelligentPointer& p)
{
    return p.GetPItem() && !(p.GetPItem()->m_flags91 & 1);
}

static CDefinitionManager g_manager;
class CCreatureActionBase {
public:
    CBaseIntelligentPointer PCreature;
    CBaseIntelligentPointer PTarget;
    virtual ~CCreatureActionBase() { }
    virtual void PerformAction() { }
    CDefinitionManager* GetPDefinitionManager() const { return &g_manager; }
};
class CCreatureAction_BanditKingThrowWeapons : public CCreatureActionBase {
public:
    virtual void PerformAction();
};

// ---- reconstructed body (src/008c89d0.cpp) ----
void CCreatureAction_BanditKingThrowWeapons::PerformAction()
{
    if (!IS_THING_ALIVE(PCreature))
        return;
    CTCCarrying* carrying;
    PCreature.GetPItem()->GetInterface(&carrying, 0x46);
    long slots[2];
    {
        CCharString name("CARRY_SLOT_RIGHT_HAND");
        slots[0] = GetPDefinitionManager()->GetDefGlobalIndexFromName(name);
    }
    {
        CCharString name("CARRY_SLOT_LEFT_HAND");
        slots[1] = GetPDefinitionManager()->GetDefGlobalIndexFromName(name);
    }
    for (long i = 0; i < 2; ++i) {
        CThing* weapon = carrying->RemoveThingInCarrySlot(slots[i], false);
        if (weapon && !(weapon->m_flags91 & 1)) {
            if (i == 0) {
                CDefPointer<CWeaponDef> def;
                CDefClassBase* defClass = weapon->m_pDef;
                if (defClass && defClass->GetSubDef(&def)) {
                    CTCBanditKingWeapons* boss;
                    if (PCreature.GetPItem()->GetInterface(&boss, 0xad))
                        boss->m_weaponDef = def->GlobalIndex;
                }
            }
            CThing* shot;
            {
                CAbilityPointer<CMeleeCombatAbilityDef> ability;
                CThing* target = PTarget.GetPItem();
                CThing* creature = PCreature.GetPItem();
                shot = CreateShot(weapon, creature, weapon, target, (float)PCreature.GetPItem()->GetShotAccuracyPercentage(),
                                  1.0f, 1.0f, &ability, -1);
            }
            if (shot) {
                shot->GetPhysics()->SetPhysicsState(2);
                CTCPhysicsNavigator* navigator;
                if (shot->GetInterface(&navigator, 0x77))
                    navigator->EnableNavigator(true);
            }
        }
    }
}
// ---- end body ----

static void Reset()
{
    g_shotCalls = 0;
    g_ability = 0;
    for (int i = 0; i < 4; ++i) g_shotResult[i] = 0;
}

int main()
{
    // 1: dead creature / no creature: nothing happens.
    {
        Reset();
        CThingCreatureBase king;
        king.m_flags91 = 1;
        CTCCarrying carrying;
        king.AddInterface(0x46, &carrying);
        CThing sword;
        carrying.Slot11 = &sword;
        CCreatureAction_BanditKingThrowWeapons action;
        action.PCreature.m_pItem = &king;
        action.PTarget.m_pItem = 0;
        action.PerformAction();
        CHECK(carrying.Calls == 0 && g_shotCalls == 0 && carrying.Slot11 == &sword);
        action.PCreature.m_pItem = 0;
        action.PerformAction();
        CHECK(carrying.Calls == 0 && g_shotCalls == 0);
    }
    // 2: both hands hold living weapons; right-hand def recorded; both shots get physics + navigator.
    {
        Reset();
        CThingCreatureBase king, hero;
        king.Accuracy = 75;
        CTCCarrying carrying;
        CTCBanditKingWeapons boss;
        king.AddInterface(0xad, &boss);
        king.AddInterface(0x46, &carrying);
        CThing right, left;
        CDefClassBase rightClass;
        CWeaponDef rightDef; rightDef.GlobalIndex = 1234; rightDef.RefCount = 1;
        rightClass.Weapon = &rightDef;
        right.m_pDef = &rightClass;
        CDefClassBase leftClass;
        CWeaponDef leftDef; leftDef.GlobalIndex = 999;
        leftClass.Weapon = &leftDef;
        left.m_pDef = &leftClass;
        carrying.Slot11 = &right;
        carrying.Slot22 = &left;
        CThing shot0, shot1;
        CTCPhysicsBase phys0, phys1;
        shot0.m_pPhysics = &phys0; shot1.m_pPhysics = &phys1;
        CTCPhysicsNavigator nav0;
        shot0.AddInterface(0x77, &nav0);
        g_shotResult[0] = &shot0; g_shotResult[1] = &shot1;
        CMeleeCombatAbilityDef ability; ability.m_count = 0;
        g_ability = &ability;
        CCreatureAction_BanditKingThrowWeapons action;
        action.PCreature.m_pItem = &king;
        action.PTarget.m_pItem = &hero;
        action.PerformAction();
        CHECK(carrying.Calls == 2 && carrying.Order[0] == 11 && carrying.Order[1] == 22);
        CHECK(boss.m_weaponDef == 1234);
        CHECK(rightDef.RefCount == 1 && rightDef.Destroyed == 0);
        CHECK(leftDef.RefCount == 1);                  // slot 1's def is never looked up
        CHECK(g_shotCalls == 2);
        CHECK(g_shots[0].Shot == &right && g_shots[0].Shooter == &king && g_shots[0].Origin == &right && g_shots[0].Target == &hero);
        CHECK(g_shots[0].Accuracy == 75.0f && g_shots[0].A == 1.0f && g_shots[0].B == 1.0f && g_shots[0].D == -1);
        CHECK(g_shots[1].Shot == &left && g_shots[1].Origin == &left && g_shots[1].Target == &hero);
        CHECK(ability.m_count == 0 && ability.Destroyed == 2);   // each throw's reference released to zero
        CHECK(phys0.Calls == 1 && phys0.State == 2 && phys1.Calls == 1 && phys1.State == 2);
        CHECK(nav0.Calls == 1 && nav0.Enabled);
    }
    // 3: right hand empty, left hand dead weapon, no boss component; then right weapon whose def has no weapon sub-def.
    {
        Reset();
        CThingCreatureBase king;
        CTCCarrying carrying;
        king.AddInterface(0x46, &carrying);
        CThing deadLeft; deadLeft.m_flags91 = 1;
        carrying.Slot22 = &deadLeft;
        CCreatureAction_BanditKingThrowWeapons action;
        action.PCreature.m_pItem = &king;
        action.PTarget.m_pItem = 0;
        action.PerformAction();
        CHECK(carrying.Calls == 2 && g_shotCalls == 0);

        Reset();
        CThing right;
        CDefClassBase noWeapon;
        right.m_pDef = &noWeapon;
        carrying.Slot11 = &right;
        CTCBanditKingWeapons boss;
        king.AddInterface(0xad, &boss);
        action.PerformAction();
        CHECK(boss.m_weaponDef == -99);
        CHECK(g_shotCalls == 1 && g_shots[0].Shot == &right && g_shots[0].Target == 0 && g_shots[0].Accuracy == 0.0f);

        Reset();
        CThing bare;                                    // no def class at all; def released to zero on the way out
        carrying.Slot11 = &bare;
        action.PerformAction();
        CHECK(g_shotCalls == 1 && boss.m_weaponDef == -99);

        Reset();
        CThing right2;
        CDefClassBase cls2;
        CWeaponDef wdef; wdef.GlobalIndex = 7; wdef.RefCount = 0;
        cls2.Weapon = &wdef;
        right2.m_pDef = &cls2;
        carrying.Slot11 = &right2;
        action.PerformAction();
        CHECK(boss.m_weaponDef == 7 && wdef.RefCount == 0 && wdef.Destroyed == 1);
    }
    if (g_failures == 0)
        std::printf("008c89d0_TEST PASS\n");
    return g_failures ? 1 : 0;
}