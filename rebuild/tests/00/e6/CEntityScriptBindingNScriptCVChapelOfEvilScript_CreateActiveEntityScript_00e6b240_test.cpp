// behaviour test for 0x00E6B240 CEntityScriptBinding<NScript::CV_ChapelOfEvilScript>::CreateActiveEntityScript: the class-allocator branch
// allocates with (parent, parent's master data, thing), Init()s the entity script and shares it into the new active script;
// the member-function branch stores Func; both keep thing/uid/flags/parent and release every reference at the end
#include <cstdio>
#if defined(_MSC_VER)
#define FASTCALL __fastcall
typedef unsigned __int64 u64;
#else
#define FASTCALL
typedef unsigned long long u64;
#endif
static int gLiveEntityScripts = 0;
static int gInitCalls = 0;
class CCharString { public: const char* PStringData; };
class CCPPointerInfo {
public:
    unsigned long RefCount;
    void (FASTCALL* DeleteFunc)(void* data);
    void* Data;
};
template <class T> class CCountedPointer {
public:
    T* Data;
    CCPPointerInfo* Info;
    static void FASTCALL Deleter(void* p) { delete (T*)p; }
    CCountedPointer(T* data) : Data(data), Info(0)
    {
        if (data) { Info = new CCPPointerInfo; Info->RefCount = 1; Info->DeleteFunc = &Deleter; Info->Data = data; }
    }
    CCountedPointer(const CCountedPointer& other) : Data(other.Data), Info(other.Info) { if (Info) ++Info->RefCount; }
    ~CCountedPointer()
    {
        if (Info) {
            if (--Info->RefCount == 0) { Info->DeleteFunc(Info->Data); delete Info; }
        }
        Data = 0; Info = 0;
    }
    CCountedPointer& operator=(const CCountedPointer& other) { ShareData(other.Data, other.Info); return *this; }
    void ShareData(T* data, CCPPointerInfo* info)
    {
        if (Info == info) return;
        if (Info) {
            if (--Info->RefCount == 0) { Info->DeleteFunc(Info->Data); delete Info; }
        }
        Data = data; Info = info;
        if (info) ++info->RefCount;
    }
    T* operator->() const { return Data; }
};
class CScriptThing { public: int Id; };
class CEntityScriptBase {
public:
    int Inits;
    CEntityScriptBase() : Inits(0) { ++gLiveEntityScripts; }
    virtual ~CEntityScriptBase() { --gLiveEntityScripts; }
    virtual void Main() {}
    virtual void Init() { ++Inits; ++gInitCalls; }
};
class CSpawnedFuncBase {
public:
    virtual ~CSpawnedFuncBase() {}
    virtual void SuspendableProcess() = 0;
};
class CActiveEntityScriptBase : public CSpawnedFuncBase {
public:
    CCountedPointer<CEntityScriptBase> PEntityScript;
    unsigned long Flags;
    u64 EntityUID;
    CScriptThing Entity;
    CActiveEntityScriptBase(const CScriptThing& entity, u64 uid, unsigned long flags)
        : PEntityScript(0), Flags(flags), EntityUID(uid), Entity(entity) {}
};
template <class T> class CActiveEntityScript : public CActiveEntityScriptBase {
public:
    void (T::*Func)(CScriptThing);
    T* PParentScript;
    CActiveEntityScript(const CScriptThing& entity, u64 uid, T* parent, void (T::*func)(CScriptThing), unsigned long flags)
        : CActiveEntityScriptBase(entity, uid, flags), Func(func), PParentScript(parent) {}
    CActiveEntityScript(const CScriptThing& entity, u64 uid, T* parent, CCountedPointer<CEntityScriptBase>& script, unsigned long flags)
        : CActiveEntityScriptBase(entity, uid, flags), Func(0), PParentScript(parent) { PEntityScript = script; }
    virtual void SuspendableProcess() {}
};
class CScriptDataBase { public: virtual ~CScriptDataBase() {} };
class CScriptBase { public: virtual ~CScriptBase() {} };
class CLevelScriptBase : public CScriptBase {
public:
    void* Game;
    CScriptDataBase* MasterScriptData;
};
template <class T> class CEntityScriptBinding {
public:
    typedef CCountedPointer<CEntityScriptBase> (FASTCALL* TAllocFunc)(T& parent, CScriptDataBase* data, const CScriptThing& thing);
    CCharString EntityScriptName;
    T* PParentScript;
    void (T::*Func)(CScriptThing);
    TAllocFunc ClassAllocFunc;
    bool ClassAllocValid;
    unsigned long Flags;
    CCountedPointer<CActiveEntityScriptBase> CreateActiveEntityScript(const CScriptThing& thing, u64 uid);
};
template <class T>
CCountedPointer<CActiveEntityScriptBase> CEntityScriptBinding<T>::CreateActiveEntityScript(const CScriptThing& thing, u64 uid)
{
    if (ClassAllocValid) {
        CCountedPointer<CEntityScriptBase> script(ClassAllocFunc(*PParentScript, PParentScript->MasterScriptData, thing));
        script->Init();
        return CCountedPointer<CActiveEntityScriptBase>(new CActiveEntityScript<T>(thing, uid, PParentScript, script, Flags));
    }
    return CCountedPointer<CActiveEntityScriptBase>(new CActiveEntityScript<T>(thing, uid, PParentScript, Func, Flags));
}
class CV_ChapelOfEvilScript : public CLevelScriptBase {
public:
    int Seen;
    CV_ChapelOfEvilScript() : Seen(0) {}
    void OnThing(CScriptThing thing) { Seen = thing.Id; }
};
static CV_ChapelOfEvilScript* gAllocParent = 0;
static CScriptDataBase* gAllocData = 0;
static int gAllocThing = 0;
static CCountedPointer<CEntityScriptBase> FASTCALL AllocScript(CV_ChapelOfEvilScript& parent, CScriptDataBase* data, const CScriptThing& thing)
{
    gAllocParent = &parent; gAllocData = data; gAllocThing = thing.Id;
    return CCountedPointer<CEntityScriptBase>(new CEntityScriptBase);
}
static int Check()
{
    CScriptDataBase data;
    CV_ChapelOfEvilScript parent;
    parent.Game = 0;
    parent.MasterScriptData = &data;
    CEntityScriptBinding<CV_ChapelOfEvilScript> b;
    b.EntityScriptName.PStringData = "Thing";
    b.PParentScript = &parent;
    b.Func = &CV_ChapelOfEvilScript::OnThing;
    b.ClassAllocFunc = &AllocScript;
    b.ClassAllocValid = true;
    b.Flags = 7;
    CScriptThing thing;
    thing.Id = 42;
    u64 uid = (u64)0x12345678 * 0x100 + 0x9a;
    {
        CCountedPointer<CActiveEntityScriptBase> r = b.CreateActiveEntityScript(thing, uid);
        CActiveEntityScript<CV_ChapelOfEvilScript>* s = (CActiveEntityScript<CV_ChapelOfEvilScript>*)r.Data;
        if (gAllocParent != &parent || gAllocData != &data || gAllocThing != 42 || gInitCalls != 1) return 1;
        if (s == 0 || s->PEntityScript.Data == 0 || s->PEntityScript.Data->Inits != 1 || s->PEntityScript.Info->RefCount != 1) return 2;
        if (s->Flags != 7 || s->EntityUID != uid || s->Entity.Id != 42 || s->PParentScript != &parent || r.Info->RefCount != 1) return 3;
        if (gLiveEntityScripts != 1) return 4;
    }
    if (gLiveEntityScripts != 0) return 5;
    b.ClassAllocValid = false;
    b.Flags = 3;
    {
        CCountedPointer<CActiveEntityScriptBase> r = b.CreateActiveEntityScript(thing, uid + 1);
        CActiveEntityScript<CV_ChapelOfEvilScript>* s = (CActiveEntityScript<CV_ChapelOfEvilScript>*)r.Data;
        if (s == 0 || s->Func != &CV_ChapelOfEvilScript::OnThing || s->PParentScript != &parent || s->PEntityScript.Data != 0) return 6;
        if (s->Flags != 3 || s->EntityUID != uid + 1 || s->Entity.Id != 42 || gInitCalls != 1 || gLiveEntityScripts != 0) return 7;
        (s->PParentScript->*s->Func)(s->Entity);
        if (parent.Seen != 42) return 8;
    }
    return 0;
}
int main()
{
    if (Check() != 0) { std::printf("00e6b240_TEST FAIL\n"); return 1; }
    std::printf("00e6b240_TEST PASS\n");
    return 0;
}