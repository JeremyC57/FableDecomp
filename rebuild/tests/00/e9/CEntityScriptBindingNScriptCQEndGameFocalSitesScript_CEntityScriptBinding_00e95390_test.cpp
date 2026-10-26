// Behaviour test for 00e95390 CEntityScriptBinding<CQ_EndGameFocalSitesScript> class-allocator constructor
// (copy of the source model; CCharString copy is a spy that records its source).
#include <cstdio>

static const void* g_copiedFrom = 0;
class CCharString { public:
    const char* PStringData;
    CCharString(const char* s) : PStringData(s) {}
    CCharString(const CCharString& other) : PStringData(other.PStringData) { g_copiedFrom = &other; }
};

class CScriptDataBase;
class CScriptThing;
class CEntityScriptBase;
template<class T> class CCountedPointer { public: T* Data; void* Info; };

namespace NScript { class CQ_EndGameFocalSitesScript { public: int Dummy; void Run(CScriptThing*) {} }; }

class CEntityScriptBindingBase { public:
    virtual ~CEntityScriptBindingBase() {}
};

template<class ParentScript>
struct CEntityScriptBinding : CEntityScriptBindingBase {
    typedef CCountedPointer<CEntityScriptBase> (*TClassAllocFunc)(ParentScript*, CScriptDataBase*, CScriptThing*);
    typedef void (ParentScript::*TFunc)(CScriptThing*);
    CCharString     EntityScriptName;
    ParentScript*   PParentScript;
    TFunc           Func;
    TClassAllocFunc ClassAllocFunc;
    bool            ClassAllocValid;
    unsigned long   Flags;
    CEntityScriptBinding(const CCharString& name, ParentScript* parentScript, TClassAllocFunc allocFunc, unsigned long flags);
    virtual ~CEntityScriptBinding() {}
};

template<class ParentScript>
CEntityScriptBinding<ParentScript>::CEntityScriptBinding(const CCharString& name, ParentScript* parentScript, TClassAllocFunc allocFunc, unsigned long flags)
    : EntityScriptName(name), PParentScript(parentScript), ClassAllocFunc(allocFunc), ClassAllocValid(true), Flags(flags)
{
}

typedef NScript::CQ_EndGameFocalSitesScript TScript;
static CCountedPointer<CEntityScriptBase> Alloc(TScript*, CScriptDataBase*, CScriptThing*)
{
    CCountedPointer<CEntityScriptBase> r; r.Data = 0; r.Info = 0; return r;
}

int main()
{
    TScript script;
    CCharString name("FocalSite");
    CEntityScriptBinding<TScript>* b = new CEntityScriptBinding<TScript>(name, &script, &Alloc, 0x25u);
    int ok = 1;
    if (g_copiedFrom != &name || b->EntityScriptName.PStringData != name.PStringData) ok = 0;
    if (b->PParentScript != &script || b->ClassAllocFunc != &Alloc || !b->ClassAllocValid || b->Flags != 0x25u) ok = 0;
    CEntityScriptBindingBase* base = b;
    delete base;
    if (!ok) { std::printf("00e95390_TEST FAIL\n"); return 1; }
    std::printf("00e95390_TEST PASS\n");
    return 0;
}