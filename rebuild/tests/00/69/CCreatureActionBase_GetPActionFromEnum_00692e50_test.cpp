// Behaviour test for 0x00692E50 CCreatureActionBase::GetPActionFromEnum: a registered type yields a fresh clone of
// its prototype bound to the creature and wrapped in a counted pointer (count 1); an unregistered type yields the
// empty counted pointer and clones nothing.
#include <cstdio>
#include <map>
#ifndef _MSC_VER
#define __fastcall
#endif

enum EActionRegisteredType { ACTION_A = 3, ACTION_B = 7, ACTION_C = 12 };
class CThingCreatureBase { public: int Id; };
class CCPPointerInfo { public: int m_count; };

template <class T> class CCountedPointer {
public:
    T*              Data;
    CCPPointerInfo* Info;
    CCountedPointer() : Data(0), Info(0) { }
    CCountedPointer(T* item) : Data(item), Info(0)
    {
        if (Data) { Info = new CCPPointerInfo; Info->m_count = 1; }
    }
    T* operator->() const { return Data; }
};

static int g_clones = 0;
class CCreatureActionBase {
public:
    int Kind;
    CThingCreatureBase* PCreature;
    explicit CCreatureActionBase(int kind) : Kind(kind), PCreature(0) { }
    virtual ~CCreatureActionBase() { }
    virtual CCreatureActionBase* Clone() const { ++g_clones; return new CCreatureActionBase(Kind); }
    static CCountedPointer<CCreatureActionBase> __fastcall GetPActionFromEnum(EActionRegisteredType type,
                                                                              CThingCreatureBase& creature);
    void SetCreature(CThingCreatureBase& creature) { PCreature = &creature; }
    static std::map<EActionRegisteredType, CCountedPointer<CCreatureActionBase> > Registry;
};
std::map<EActionRegisteredType, CCountedPointer<CCreatureActionBase> > CCreatureActionBase::Registry;

CCountedPointer<CCreatureActionBase> __fastcall CCreatureActionBase::GetPActionFromEnum(EActionRegisteredType type,
                                                                                        CThingCreatureBase& creature)
{
    std::map<EActionRegisteredType, CCountedPointer<CCreatureActionBase> >::iterator it = Registry.find(type);
    if (it == Registry.end())
        return CCountedPointer<CCreatureActionBase>();
    CCreatureActionBase* action = it->second->Clone();
    action->SetCreature(creature);
    return CCountedPointer<CCreatureActionBase>(action);
}

int main()
{
    CCreatureActionBase protoA(1), protoC(3);
    CCreatureActionBase::Registry[ACTION_A] = CCountedPointer<CCreatureActionBase>(&protoA);
    CCreatureActionBase::Registry[ACTION_C] = CCountedPointer<CCreatureActionBase>(&protoC);
    CThingCreatureBase hero;
    hero.Id = 42;

    CCountedPointer<CCreatureActionBase> a = CCreatureActionBase::GetPActionFromEnum(ACTION_A, hero);
    if (!a.Data || a.Data == &protoA || a->Kind != 1 || a->PCreature != &hero || !a.Info || a.Info->m_count != 1 ||
        g_clones != 1 || protoA.PCreature != 0) {
        std::printf("00692e50_TEST FAIL\n"); return 1;
    }
    CCountedPointer<CCreatureActionBase> c = CCreatureActionBase::GetPActionFromEnum(ACTION_C, hero);
    if (!c.Data || c->Kind != 3 || c->PCreature != &hero || g_clones != 2) { std::printf("00692e50_TEST FAIL\n"); return 1; }
    CCountedPointer<CCreatureActionBase> b = CCreatureActionBase::GetPActionFromEnum(ACTION_B, hero);
    if (b.Data != 0 || b.Info != 0 || g_clones != 2) { std::printf("00692e50_TEST FAIL\n"); return 1; }
    delete a.Data; delete a.Info; delete c.Data; delete c.Info;
    for (std::map<EActionRegisteredType, CCountedPointer<CCreatureActionBase> >::iterator it =
             CCreatureActionBase::Registry.begin(); it != CCreatureActionBase::Registry.end(); ++it)
        delete it->second.Info;
    std::printf("00692e50_TEST PASS\n");
    return 0;
}