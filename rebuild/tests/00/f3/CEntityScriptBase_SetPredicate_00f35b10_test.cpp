// Behaviour test (0x00f35b10): CEntityScriptBase::SetPredicate forwards the predicate to the parent script (slot 3), which clones it onto its active thread
#include <cstdio>
namespace NScript {
class CPredicateBase {
public:
    virtual bool operator()() const = 0;
    virtual CPredicateBase* Clone() const = 0;
    virtual ~CPredicateBase() { }
};
class CIsTrue : public CPredicateBase {
public:
    int Id;
    CIsTrue(int id) : Id(id) { }
    virtual bool operator()() const { return true; }
    virtual CPredicateBase* Clone() const { return new CIsTrue(Id); }
};
}
class CSpawnedFuncBase {
public:
    const NScript::CPredicateBase* PPredicate;
    CSpawnedFuncBase() : PPredicate(0) { }
    ~CSpawnedFuncBase() { delete PPredicate; }
    void SetPredicate(const NScript::CPredicateBase& predicate) { delete PPredicate; PPredicate = predicate.Clone(); }
};
class CScriptBase {
public:
    CSpawnedFuncBase* PActiveThread;
    void SetPredicate(const NScript::CPredicateBase& predicate) { PActiveThread->SetPredicate(predicate); }
};
class CEntityScriptBase {
public:
    virtual ~CEntityScriptBase() { }
    virtual CScriptBase* GetParentScript() const = 0;
    void SetPredicate(const NScript::CPredicateBase& predicate) { GetParentScript()->SetPredicate(predicate); }
};
class CTestEntityScript : public CEntityScriptBase {
public:
    CScriptBase* ParentClass;
    int          ParentCalls;
    virtual CScriptBase* GetParentScript() const { ++const_cast<CTestEntityScript*>(this)->ParentCalls; return ParentClass; }
};
int main()
{
    CSpawnedFuncBase thread;
    CScriptBase script; script.PActiveThread = &thread;
    CTestEntityScript entity; entity.ParentClass = &script; entity.ParentCalls = 0;
    entity.SetPredicate(NScript::CIsTrue(42));
    const NScript::CIsTrue* stored = static_cast<const NScript::CIsTrue*>(thread.PPredicate);
    if (entity.ParentCalls != 1 || !stored || stored->Id != 42 || !(*stored)()) { std::printf("00f35b10_TEST FAIL\n"); return 1; }
    entity.SetPredicate(NScript::CIsTrue(7));
    stored = static_cast<const NScript::CIsTrue*>(thread.PPredicate);
    if (entity.ParentCalls != 2 || stored->Id != 7) { std::printf("00f35b10_TEST FAIL\n"); return 1; }
    std::printf("00f35b10_TEST PASS\n");
    return 0;
}