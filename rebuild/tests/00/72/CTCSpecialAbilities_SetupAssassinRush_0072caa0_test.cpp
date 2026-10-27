// 0072caa0 behaviour test for CTCSpecialAbilities::SetupAssassinRush
#include <cstdio>
#include <cstdlib>
#include <cstring>
#ifndef _MSC_VER
#define __forceinline inline
#endif
namespace std {
template <unsigned int _Nb>
class bitset {
public:
    enum { _Words = (_Nb + 31) / 32 };
    unsigned long _M_w[_Words];
    static unsigned int _S_whichword(unsigned int __pos) { return __pos / 32; }
    static unsigned int _S_whichbit(unsigned int __pos) { return __pos % 32; }
    static unsigned long _S_maskbit(unsigned int __pos) { return 1UL << _S_whichbit(__pos); }
    unsigned long _M_getword(unsigned int __pos) const { return _M_w[_S_whichword(__pos)]; }
    bool _Unchecked_test(unsigned int __pos) const { return (_M_getword(__pos) & _S_maskbit(__pos)) != 0; }
    bool test(unsigned int __pos) const
    {
        if (__pos >= _Nb) { puts("bitset"); abort(); }
        return _Unchecked_test(__pos);
    }
    void set(unsigned int __pos) { _M_w[_S_whichword(__pos)] |= _S_maskbit(__pos); }
};
}
class CTCBase { public: int Tag; };
enum ETCInterfaceType { TC_NONE = 0 };
class CInterfaceMap {
public:
    struct SEntry { ETCInterfaceType type; CTCBase* component; };
    SEntry* first;
    SEntry* last;
    const SEntry* LowerBound(const ETCInterfaceType* type) const
    {
        const SEntry* it = first;
        while (it != last && it->type < *type) ++it;
        return it;
    }
};
class CPlayer {
public:
    unsigned char _pad00[0x20a];
    bool          ZTargeting;
};
class CThing;
static CThing* g_target = 0;
class CTCTargeter : public CTCBase {
public:
    virtual void v00() {} virtual void v01() {} virtual void v02() {} virtual void v03() {} virtual void v04() {} virtual void v05() {}
    virtual void v06() {} virtual void v07() {} virtual void v08() {} virtual void v09() {} virtual void v10() {} virtual void v11() {}
    virtual void v12() {} virtual void v13() {} virtual void v14() {} virtual void v15() {} virtual void v16() {} virtual void v17() {}
    virtual void v18() {} virtual void v19() {} virtual void v20() {} virtual void v21() {} virtual void v22() {} virtual void v23() {}
    virtual void v24() {} virtual void v25() {} virtual void v26() {} virtual void v27() {} virtual void v28() {} virtual void v29() {}
    virtual void v30() {} virtual void v31() {} virtual void v32() {}
    virtual CThing* GetPTarget() { return g_target; }
};
static CPlayer g_player;
class CThing {
public:
    unsigned char _pad00[0x20];
    std::bitset<274> UsingComponents;
    CInterfaceMap Components;
    unsigned char _pad4c[0x6c - 0x4c];
    unsigned long ThingTypeFlags;
    unsigned char _pad70[0x91 - 0x70];
    unsigned char Dead : 1;
    CPlayer& GetPlayer() { return g_player; }
    bool HasTC(ETCInterfaceType type) const { return UsingComponents.test(type); }
    template <class T> __forceinline T* GetTC(ETCInterfaceType type) const
    {
        ETCInterfaceType key = type;
        const CInterfaceMap* map = &Components;
        const CInterfaceMap::SEntry* it = map->LowerBound(&key);
        return (T*)((it == map->last || type < it->type) ? map->last : it)->component;
    }
};
class CCreatureActionBase { public: int Kind; };
class CCPPointerInfo;
template <class T> class CCountedPointer {
public:
    T*              Data;
    CCPPointerInfo* Info;
    CCountedPointer() : Data(0), Info(0) {}
    CCountedPointer(const CCountedPointer& other) : Data(other.Data), Info(other.Info) {}
};
static int g_setCalls = 0, g_setKind = -1, g_dtors = 0, g_ctorTargeted = 0, g_ctorUntargeted = 0;
static bool g_ctorFlag = false; static CThing* g_ctorTarget = 0; static bool g_setResult = true;
class CThingCreatureBase : public CThing {
public:
    bool SetCurrentAction(const CCreatureActionBase& action) { ++g_setCalls; g_setKind = action.Kind; return g_setResult; }
};
class CCreatureAction_CastAssassinRush : public CCreatureActionBase {
public:
    unsigned char _body[0x134];
    CCreatureAction_CastAssassinRush(CThingCreatureBase& creature, CThing& target, bool flag, long priority,
                                     CCountedPointer<CCreatureActionBase> next)
    { Kind = 1; ++g_ctorTargeted; g_ctorTarget = &target; g_ctorFlag = flag; (void)creature; (void)priority; (void)next; }
    CCreatureAction_CastAssassinRush(CThingCreatureBase& creature, long priority, CCountedPointer<CCreatureActionBase> next)
    { Kind = 2; ++g_ctorUntargeted; (void)creature; (void)priority; (void)next; }
    ~CCreatureAction_CastAssassinRush() { ++g_dtors; }
    operator const CCreatureActionBase&() const { return *(const CCreatureActionBase*)this; }
};
class CTCSpecialAbilities {
public:
    virtual void v00() {}
    CThing* ParentThing;
    unsigned char _base08[4];
    bool AbilityMutexLock;
    bool UsingBed;
    long CurrentAbility;
    void* UndeadFollowers;
    CThingCreatureBase* Creature;
    bool SetupAssassinRush();
};

bool CTCSpecialAbilities::SetupAssassinRush()
{
    if (!Creature->HasTC((ETCInterfaceType)0xa9) && !Creature->HasTC((ETCInterfaceType)0xa8)) {
        if (Creature->HasTC((ETCInterfaceType)8)) {
            CThing* target = Creature->GetTC<CTCTargeter>((ETCInterfaceType)8)->GetPTarget();
            if (target && !target->Dead && (target->ThingTypeFlags & 8)) {
                bool flag = false;
                if (Creature->ThingTypeFlags & 0x20)
                    flag = Creature->GetPlayer().ZTargeting;
                return Creature->SetCurrentAction(CCreatureAction_CastAssassinRush(*Creature, *target, flag, 0, CCountedPointer<CCreatureActionBase>()));
            }
        }
        return Creature->SetCurrentAction(CCreatureAction_CastAssassinRush(*Creature, 0, CCountedPointer<CCreatureActionBase>()));
    }
    return false;
}

static CThingCreatureBase g_creature; static CThing g_victim; static CTCTargeter g_targeter;
static CInterfaceMap::SEntry g_entries[2];
static void reset(bool hasTargeter, bool rushing)
{
    memset(g_creature.UsingComponents._M_w, 0, sizeof(g_creature.UsingComponents._M_w));
    memset(g_victim.UsingComponents._M_w, 0, sizeof(g_victim.UsingComponents._M_w));
    g_entries[0].type = (ETCInterfaceType)3; g_entries[0].component = 0;
    g_entries[1].type = (ETCInterfaceType)8; g_entries[1].component = &g_targeter;
    g_creature.Components.first = g_entries; g_creature.Components.last = g_entries + (hasTargeter ? 2 : 1);
    if (hasTargeter) g_creature.UsingComponents.set(8);
    if (rushing) g_creature.UsingComponents.set(0xa9);
    g_creature.ThingTypeFlags = 8; g_creature.Dead = 0;
    g_victim.ThingTypeFlags = 8; g_victim.Dead = 0;
    g_target = &g_victim; g_player.ZTargeting = true;
    g_setCalls = g_dtors = g_ctorTargeted = g_ctorUntargeted = 0; g_setKind = -1; g_ctorFlag = false; g_ctorTarget = 0; g_setResult = true;
}
int main()
{
    bool ok = true;
    CTCSpecialAbilities sa; sa.Creature = &g_creature; sa.ParentThing = &g_creature;
    // 1. targeted, not player controlled: flag stays false
    reset(true, false);
    ok = ok && sa.SetupAssassinRush() && g_ctorTargeted == 1 && g_ctorUntargeted == 0 && g_setKind == 1 && g_ctorTarget == &g_victim && !g_ctorFlag && g_dtors == 1;
    // 2. targeted, player controlled: flag from the player's Z-targeting
    reset(true, false); g_creature.ThingTypeFlags = 8 | 0x20;
    ok = ok && sa.SetupAssassinRush() && g_ctorTargeted == 1 && g_ctorFlag && g_dtors == 1;
    // 3. target dead -> untargeted
    reset(true, false); g_victim.Dead = 1;
    ok = ok && sa.SetupAssassinRush() && g_ctorUntargeted == 1 && g_ctorTargeted == 0 && g_setKind == 2 && g_dtors == 1;
    // 4. target not a creature -> untargeted
    reset(true, false); g_victim.ThingTypeFlags = 0;
    ok = ok && sa.SetupAssassinRush() && g_ctorUntargeted == 1 && g_ctorTargeted == 0;
    // 5. no targeter interface -> untargeted (the map is never searched)
    reset(false, false);
    ok = ok && sa.SetupAssassinRush() && g_ctorUntargeted == 1 && g_ctorTargeted == 0;
    // 6. null target -> untargeted; SetCurrentAction result is returned
    reset(true, false); g_target = 0; g_setResult = false;
    ok = ok && !sa.SetupAssassinRush() && g_ctorUntargeted == 1 && g_setCalls == 1;
    // 7. already rushing -> false, nothing built
    reset(true, true);
    ok = ok && !sa.SetupAssassinRush() && g_setCalls == 0 && g_ctorTargeted == 0 && g_ctorUntargeted == 0;
    printf(ok ? "0072caa0_TEST PASS\n" : "0072caa0_TEST FAIL\n");
    return ok ? 0 : 1;
}