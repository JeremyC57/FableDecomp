// Behaviour test for NScript::CQS_MeetSisterScript::Main() @ 0x00E28DE0: the bindings, the post-add, the quest card objective
#include <cstdio>
#include <cstring>
static char g_log[2048];
static void Log(const char* s) { std::strcat(g_log, s); std::strcat(g_log, "|"); }
class CCharString {
public:
    const char* Text;
    CCharString(const char* text, long = -1) : Text(text) { }
    CCharString(const CCharString& o) : Text(o.Text) { }
    ~CCharString() { }
};
class CScriptThing;
class CScriptDataBase;
class CEntityScriptBase;
template <class T> class CCountedPointer { public: T* Data; void* Info; };
template class CCountedPointer<CEntityScriptBase>;     // complete before the alloc typedef (VC7.1 C2664 on a UDT return otherwise)
class CEntityScriptBindingBase {
public:
    virtual const CCharString& GetEntityScriptName() const = 0;
    virtual ~CEntityScriptBindingBase() { }
};
template <class T> class CEntityScriptBinding : public CEntityScriptBindingBase {
public:
    typedef CCountedPointer<CEntityScriptBase> (*TAllocFunc)(T& parent, CScriptDataBase* data, const CScriptThing& thing);
    CCharString     EntityScriptName;
    T*              PParentScript;
    TAllocFunc      ClassAllocFunc;
    bool            ClassAllocValid;
    unsigned long   Flags;
    CEntityScriptBinding(const CCharString& name, T* parent, TAllocFunc alloc, unsigned long flags)
        : EntityScriptName(name), PParentScript(parent), ClassAllocFunc(alloc), ClassAllocValid(true), Flags(flags) { }
    virtual const CCharString& GetEntityScriptName() const { return EntityScriptName; }
};
class CSpawnedFuncBase {
public:
    CCharString Name;
    CSpawnedFuncBase(const CCharString& name, unsigned long) : Name(name) { }
    virtual ~CSpawnedFuncBase() { }
    virtual void SuspendableProcess() = 0;
};
template <class T> class CSpawnedFunc : public CSpawnedFuncBase {
public:
    void (T::*Func)();
    T* PClass;
    CSpawnedFunc(const CCharString& name, unsigned long id, T* pclass, void (T::*func)())
        : CSpawnedFuncBase(name, id), Func(func), PClass(pclass) { }
    virtual void SuspendableProcess() { (PClass->*Func)(); }
};
class CGameScriptInterface {
public:
    void PostAddScriptedEntities() const { Log("GamePostAdd"); }
    CCharString GetActiveQuestName() const { return CCharString("QS_MeetSister"); }
    void SetQuestCardObjective(const CCharString& q, const CCharString& o, const CCharString& r1, const CCharString& r2) const
    {
        char b[512]; std::sprintf(b, "Objective:%s,%s,%s,%s", q.Text, o.Text, r1.Text, r2.Text); Log(b);
    }
};
class CScriptBase {
public:
    CEntityScriptBindingBase* Bindings[4];
    CSpawnedFuncBase* Spawned[4];
    int NumBindings, NumSpawned;
    CScriptBase() : NumBindings(0), NumSpawned(0) { }
    virtual ~CScriptBase() { }
    void AddEntityScriptBinding(CEntityScriptBindingBase* binding)
    {
        Bindings[NumBindings++] = binding; char b[128]; std::sprintf(b, "Bind:%s", binding->GetEntityScriptName().Text); Log(b);
    }
    void PostAddScriptedEntities() { Log("PostAdd"); }
    void AddSpawnedFunction(CSpawnedFuncBase* func, const CCharString& region)
    {
        Spawned[NumSpawned++] = func; char b[128]; std::sprintf(b, "Spawn:%s@%s", func->Name.Text, region.Text); Log(b);
    }
    void SpawnFunction(CSpawnedFuncBase* func, const char* regionName = "") { AddSpawnedFunction(func, CCharString(regionName)); }
};
static int g_allocs = 0;
class CQS_MeetSisterScript : public CScriptBase {
public:
    class CMeetSisterMessenger { public: static CCountedPointer<CEntityScriptBase> Alloc(CQS_MeetSisterScript& parent, CScriptDataBase* data, const CScriptThing& thing); };
    class CMeetSisterSister { public: static CCountedPointer<CEntityScriptBase> Alloc(CQS_MeetSisterScript& parent, CScriptDataBase* data, const CScriptThing& thing); };

    CGameScriptInterface* Game;
    int ThreadRuns;
    CQS_MeetSisterScript() : ThreadRuns(0) { }
    void Main();
};
CCountedPointer<CEntityScriptBase> CQS_MeetSisterScript::CMeetSisterMessenger::Alloc(CQS_MeetSisterScript&, CScriptDataBase*, const CScriptThing&) { ++g_allocs; CCountedPointer<CEntityScriptBase> p; p.Data = 0; p.Info = 0; return p; }
CCountedPointer<CEntityScriptBase> CQS_MeetSisterScript::CMeetSisterSister::Alloc(CQS_MeetSisterScript&, CScriptDataBase*, const CScriptThing&) { ++g_allocs; CCountedPointer<CEntityScriptBase> p; p.Data = 0; p.Info = 0; return p; }

void CQS_MeetSisterScript::Main()
{
    AddEntityScriptBinding(new CEntityScriptBinding<CQS_MeetSisterScript>("MeetSisterMessenger", this, &CMeetSisterMessenger::Alloc, 1));
    AddEntityScriptBinding(new CEntityScriptBinding<CQS_MeetSisterScript>("MeetSisterSister", this, &CMeetSisterSister::Alloc, 0));
    PostAddScriptedEntities();
    Game->PostAddScriptedEntities();
    Game->SetQuestCardObjective(Game->GetActiveQuestName(), "TEXT_QUEST_THERESA_MOTHER_INFO_2_SUMMARY", "HauntedHouse", "BarrowFields");
}
int main()
{
    CGameScriptInterface game;
    CQS_MeetSisterScript s;
    s.Game = &game;
    s.Main();
    const char* want = "Bind:MeetSisterMessenger|Bind:MeetSisterSister|PostAdd|GamePostAdd|Objective:QS_MeetSister,TEXT_QUEST_THERESA_MOTHER_INFO_2_SUMMARY,HauntedHouse,BarrowFields|";
    bool ok = std::strcmp(g_log, want) == 0 && s.NumBindings == 2 && s.NumSpawned == 0;
    CEntityScriptBinding<CQS_MeetSisterScript>* b0 = (CEntityScriptBinding<CQS_MeetSisterScript>*)s.Bindings[0];
    CEntityScriptBinding<CQS_MeetSisterScript>* b1 = (CEntityScriptBinding<CQS_MeetSisterScript>*)s.Bindings[1];
    if (ok) ok = b0->Flags == 1 && b1->Flags == 0 && b0->ClassAllocValid && b1->ClassAllocValid && b0->PParentScript == &s
        && b0->ClassAllocFunc == &CQS_MeetSisterScript::CMeetSisterMessenger::Alloc && b1->ClassAllocFunc == &CQS_MeetSisterScript::CMeetSisterSister::Alloc;
    if (!ok) { std::printf("00e28de0_TEST FAIL\n%s\n", g_log); return 1; }
    std::printf("00e28de0_TEST PASS\n");
    return 0;
}