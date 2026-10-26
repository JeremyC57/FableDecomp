// behaviour test for 0x00E95690 NScript::CV_HauntedHouseScript::CM_UndeadHere::Alloc: one new entity handed to a fresh counted pointer (count 1, the CEntityScriptBase deleter, Data == the object), Game taken from the parent script, Me copy-constructed (the shared script-thing count bumped), ParentClass = &parent, PMasterData = data, the members laid out as Ego_r, the stored deleter destroys the entity through its virtual destructor and releases Me
#include <cstdio>
#ifndef _MSC_VER
#define __fastcall
#endif
class CGameScriptInterfaceBase { public: int Tag; };
class CScriptDataBase { public: int Tag; };
static int gDestroyed = 0;
void __fastcall Destroy_00cdee00(void* object);
class CCPPointerInfo {
public:
    unsigned long RefCount;
    void (__fastcall* DeleteFunc)(void*);
    void* Data;
    CCPPointerInfo(void* data, void (__fastcall* deleteFunc)(void*)) : RefCount(1), DeleteFunc(deleteFunc), Data(data) { }
};
template <class T> class CCountedPointer {
public:
    T* Data;
    CCPPointerInfo* Info;
    CCountedPointer(T* data) : Data(data)
    {
        if (data)
            Info = new CCPPointerInfo(Data, Destroy_00cdee00);
        else
            Info = 0;
    }
};
static int gScriptThingDefaultCtors = 0;
class CScriptThing {
public:
    virtual ~CScriptThing() { if (PImp.Info) --PImp.Info->RefCount; }
    CCountedPointer<CScriptThing> PImp;
    CScriptThing() : PImp(0) { ++gScriptThingDefaultCtors; }
    CScriptThing(const CScriptThing& other) : PImp(0)
    {
        PImp.Data = other.PImp.Data;
        PImp.Info = other.PImp.Info;
        if (PImp.Info) ++PImp.Info->RefCount;
    }
};
static int gMemberCtors = 0;

class CEntityScriptBase {
public:
    CEntityScriptBase(CGameScriptInterfaceBase& game, const CScriptThing& me) : Game(&game), Me(me) { }
    virtual ~CEntityScriptBase() { }
    CGameScriptInterfaceBase* Game;
    CScriptThing Me;
};
void __fastcall Destroy_00cdee00(void* object)
{
    if (object)
        delete (CEntityScriptBase*)object;
}
class CScriptBase {
public:
    virtual ~CScriptBase() { }
    unsigned char _body[0x3c];
};
template <class TData> class CLevelScriptBase : public CScriptBase {
public:
    CGameScriptInterfaceBase* Game;
    TData* MasterScriptData;
};
namespace NScript {
class CQ_SunnyvaleMasterData;
class CV_HauntedHouseScript : public CLevelScriptBase<CQ_SunnyvaleMasterData> {
public:
    class CM_UndeadHere : public CEntityScriptBase {
    public:
        CV_HauntedHouseScript* ParentClass;
        CQ_SunnyvaleMasterData* PMasterData;
        long Counter;
        CM_UndeadHere(CV_HauntedHouseScript& parent, CScriptDataBase* data, const CScriptThing& me)
            : CEntityScriptBase(*parent.Game, me), ParentClass(&parent)
        {
            PMasterData = (CQ_SunnyvaleMasterData*)data;
        }
        virtual ~CM_UndeadHere() { ++gDestroyed; }
        static CCountedPointer<CEntityScriptBase> __fastcall Alloc(CV_HauntedHouseScript& parent, CScriptDataBase* data, const CScriptThing& me);
    };
};
CCountedPointer<CEntityScriptBase> __fastcall CV_HauntedHouseScript::CM_UndeadHere::Alloc(CV_HauntedHouseScript& parent, CScriptDataBase* data, const CScriptThing& me)
{
    return CCountedPointer<CEntityScriptBase>(new CM_UndeadHere(parent, data, me));
}
}
typedef NScript::CV_HauntedHouseScript TScript;
typedef NScript::CV_HauntedHouseScript::CM_UndeadHere TEntity;
static int Check()
{
    static CGameScriptInterfaceBase game;
    static CScriptDataBase data;
    static CCPPointerInfo meInfo(0, 0);
    TScript* parent = new TScript;
    parent->Game = &game;
    CScriptThing me;
    me.PImp.Data = &me;
    me.PImp.Info = &meInfo;
    meInfo.RefCount = 1;
    CCountedPointer<CEntityScriptBase> p = TScript::CM_UndeadHere::Alloc(*parent, &data, me);
    if (p.Data == 0 || p.Info == 0) return 1;
    if (p.Info->RefCount != 1 || p.Info->DeleteFunc != Destroy_00cdee00 || p.Info->Data != (void*)p.Data) return 2;
    TEntity* e = (TEntity*)p.Data;
    if (e->Game != &game || e->Me.PImp.Data != &me || e->Me.PImp.Info != &meInfo || meInfo.RefCount != 2) return 3;
    if (e->ParentClass != parent || (void*)e->PMasterData != (void*)&data) return 4;
    if (gMemberCtors != 0) return 5;

    p.Info->DeleteFunc(p.Info->Data);                      // the deleter: virtual destruction, Me released
    if (gDestroyed != 1 || meInfo.RefCount != 1) return 6;
    Destroy_00cdee00(0);                                   // null is ignored
    if (gDestroyed != 1) return 7;
    delete p.Info;
    me.PImp.Info = 0;
    delete parent;
    return 0;
}
int main()
{
    int r = Check();
    if (r != 0) { std::printf("00e95690_TEST FAIL %d\n", r); return 1; }
    std::printf("00e95690_TEST PASS\n");
    return 0;
}
