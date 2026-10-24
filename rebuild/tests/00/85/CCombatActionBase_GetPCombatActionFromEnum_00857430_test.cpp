// Behaviour test for 0x00857430 CCombatActionBase::GetPCombatActionFromEnum (chunk 402): an unregistered type, a null
// prototype or a failed clone give a null counted pointer; otherwise the clone gets its creature and target and is
// returned counted.
#include <cstdio>
#if !defined(_MSC_VER)
#define __fastcall
#endif

enum EActionRegisteredType { ACTION_REGISTERED_TYPE_0 = 0, ACTION_SWORD = 3, ACTION_AXE = 5, ACTION_BROKEN = 7, ACTION_EMPTY = 9 };
class CThing {
public:
    int Id;
};
class CThingCreatureBase : public CThing { };
class CCPPointerInfo {
public:
    long References;
};
static CCPPointerInfo gInfo;
template <class T> class CCountedPointer {
public:
    T*              Data;
    CCPPointerInfo* Info;
    CCountedPointer() : Data(0), Info(0) { }
    CCountedPointer(T* item) : Data(item), Info(item ? &gInfo : 0) { }
    T* operator->() const { return Data; }
};
// the registry: a sorted table with map-like find/end
namespace stl402 {
template <class _Key, class _Tp> class map {
public:
    struct value_type { _Key first; _Tp second; };
    class iterator {
    public:
        value_type* _M_node;
        iterator(value_type* node) : _M_node(node) { }
        value_type* operator->() const { return _M_node; }
        bool operator==(const iterator& other) const { return _M_node == other._M_node; }
    };
    value_type Entries[8];
    int        Count;
    map() : Count(0) { }
    iterator end() { return iterator(Entries + Count); }
    iterator find(const _Key& key)
    {
        for (int i = 0; i < Count; ++i)
            if (Entries[i].first == key)
                return iterator(Entries + i);
        return end();
    }
};
}
class CCreatureActionBase {
public:
    CThingCreatureBase* PCreature;
    bool                CloneFails;
    CCreatureActionBase() : PCreature(0), CloneFails(false) { }
    virtual ~CCreatureActionBase() { }
    virtual CCreatureActionBase* Clone() const = 0;
protected:
    void SetCreature(CThingCreatureBase& creature) { PCreature = &creature; }
    static stl402::map<EActionRegisteredType, CCountedPointer<CCreatureActionBase> > Registry;
public:
    static void Register(EActionRegisteredType type, CCreatureActionBase* prototype)
    {
        Registry.Entries[Registry.Count].first = type;
        Registry.Entries[Registry.Count].second = CCountedPointer<CCreatureActionBase>(prototype);
        ++Registry.Count;
    }
};
stl402::map<EActionRegisteredType, CCountedPointer<CCreatureActionBase> > CCreatureActionBase::Registry;

class CCombatActionBase : public CCreatureActionBase {
public:
    CThing* PTarget;
    int     Kind;
    CCombatActionBase(int kind) : PTarget(0), Kind(kind) { }
    virtual CCreatureActionBase* Clone() const
    {
        if (CloneFails)
            return 0;
        return new CCombatActionBase(Kind);
    }
    void SetTarget(CThing& target) { PTarget = &target; }
    static CCountedPointer<CCombatActionBase> __fastcall GetPCombatActionFromEnum(EActionRegisteredType type,
                                                                                  CThingCreatureBase& creature,
                                                                                  CThingCreatureBase& target);
};

CCountedPointer<CCombatActionBase> __fastcall CCombatActionBase::GetPCombatActionFromEnum(EActionRegisteredType type,
                                                                                          CThingCreatureBase& creature,
                                                                                          CThingCreatureBase& target)
{
    stl402::map<EActionRegisteredType, CCountedPointer<CCreatureActionBase> >::iterator it = Registry.find(type);
    if (it == Registry.end())
        return CCountedPointer<CCombatActionBase>();
    if (!it->second.Data)
        return CCountedPointer<CCombatActionBase>();
    CCombatActionBase* action = (CCombatActionBase*)it->second->Clone();
    if (action) {
        action->SetCreature(creature);
        action->SetTarget(target);
        return CCountedPointer<CCombatActionBase>(action);
    }
    return CCountedPointer<CCombatActionBase>();
}

int main()
{
    int failures = 0;
    CCombatActionBase sword(1), axe(2), broken(3);
    broken.CloneFails = true;
    CCreatureActionBase::Register(ACTION_SWORD, &sword);
    CCreatureActionBase::Register(ACTION_AXE, &axe);
    CCreatureActionBase::Register(ACTION_BROKEN, &broken);
    CCreatureActionBase::Register(ACTION_EMPTY, 0);
    CThingCreatureBase hero, enemy;
    hero.Id = 1; enemy.Id = 2;

    CCountedPointer<CCombatActionBase> p = CCombatActionBase::GetPCombatActionFromEnum(ACTION_AXE, hero, enemy);
    if (!p.Data || p.Info != &gInfo || p.Data == &axe) { printf("  no fresh clone\n"); ++failures; }
    else {
        if (p->Kind != 2) { printf("  wrong prototype cloned\n"); ++failures; }
        if (p->PCreature != &hero || p->PTarget != &enemy) { printf("  creature/target not set\n"); ++failures; }
        delete p.Data;
    }
    if (axe.PCreature != 0 || axe.PTarget != 0) { printf("  prototype modified\n"); ++failures; }
    p = CCombatActionBase::GetPCombatActionFromEnum(ACTION_REGISTERED_TYPE_0, hero, enemy);
    if (p.Data || p.Info) { printf("  unregistered type not null\n"); ++failures; }
    p = CCombatActionBase::GetPCombatActionFromEnum(ACTION_EMPTY, hero, enemy);
    if (p.Data || p.Info) { printf("  null prototype not null\n"); ++failures; }
    p = CCombatActionBase::GetPCombatActionFromEnum(ACTION_BROKEN, hero, enemy);
    if (p.Data || p.Info) { printf("  failed clone not null\n"); ++failures; }
    if (failures == 0) {
        printf("00857430_TEST PASS\n");
        return 0;
    }
    printf("00857430_TEST FAIL\n");
    return 1;
}