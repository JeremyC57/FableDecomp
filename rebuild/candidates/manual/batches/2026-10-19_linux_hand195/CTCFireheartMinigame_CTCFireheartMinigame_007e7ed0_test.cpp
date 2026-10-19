#include <cstdio>
#include <stdlib.h>
static int g_defDeleted = 0, g_handleDtors = 0, g_impDestroyed = 0, g_blockDeleted = 0, g_baseObj = 0, g_tcBase = 0, g_freed = 0;
struct CFireheartMinigameDef {
    virtual void Delete() const { ++g_defDeleted; }
    long RefCount;
};
template<class T> struct CDefPointer {
    T* Object;
    ~CDefPointer()
    {
        if (Object) {
            T* object = Object;
            if (--const_cast<CFireheartMinigameDef*>(object)->RefCount == 0)
                object->Delete();
            Object = 0;
        }
    }
};
struct CBaseObject { virtual ~CBaseObject() { ++g_baseObj; } void* IntelligentPointer; };
struct CScriptThingImp { void DestroySelf() { ++g_impDestroyed; } };
struct SCountBlock { long Count; void (CScriptThingImp::*Destroy)(); CScriptThingImp* Object; };
struct CCountedPointer {
    CScriptThingImp* Data; SCountBlock* Info;
    ~CCountedPointer()
    {
        if (Info) {
            --Info->Count;
            if (Info->Count == 0) { (Info->Object->*Info->Destroy)(); ++g_blockDeleted; delete Info; }
        }
        Data = 0; Info = 0;
    }
};
struct CScriptGameResourceObjectBase : CBaseObject { ~CScriptGameResourceObjectBase() { } };
struct CScriptGameResourceObjectScriptedThingBase : CScriptGameResourceObjectBase { CCountedPointer PImp; };
struct CEnginePrimitiveHandle { virtual ~CEnginePrimitiveHandle() { ++g_handleDtors; } void* Data; };
template <class T> void _Destroy(T* first, T* last) { for (; first != last; ++first) first->~T(); }
template <class T> struct CRetailVector {
    T* _M_start; T* _M_finish; T* _M_end_of_storage;
    ~CRetailVector() { _Destroy(_M_start, _M_finish); if (_M_start) { ++g_freed; free(_M_start); } }
};
struct CTCBase { virtual void v00() { } ~CTCBase() { ++g_tcBase; } };
struct CTCFireheartMinigame : CTCBase {
    CScriptGameResourceObjectScriptedThingBase ScriptData;
    bool QuitWithDelay;
    CRetailVector<CEnginePrimitiveHandle> Handles;
    CDefPointer<const CFireheartMinigameDef> PDef;
    ~CTCFireheartMinigame();
};
CTCFireheartMinigame::~CTCFireheartMinigame()
{
}
#include <new>
int main()
{
    CFireheartMinigameDef def; def.RefCount = 1;
    CScriptThingImp imp;
    SCountBlock* block = new SCountBlock; block->Count = 1; block->Destroy = &CScriptThingImp::DestroySelf; block->Object = &imp;
    {
        CTCFireheartMinigame* m = (CTCFireheartMinigame*)malloc(sizeof(CTCFireheartMinigame));
        new (m) CTCFireheartMinigame;
        CEnginePrimitiveHandle* hs = (CEnginePrimitiveHandle*)malloc(sizeof(CEnginePrimitiveHandle) * 3);
        for (int i = 0; i < 3; ++i) new (&hs[i]) CEnginePrimitiveHandle;
        m->Handles._M_start = hs; m->Handles._M_finish = hs + 3; m->Handles._M_end_of_storage = hs + 3;
        m->PDef.Object = &def; m->ScriptData.PImp.Data = &imp; m->ScriptData.PImp.Info = block;
        m->~CTCFireheartMinigame();
        if (g_defDeleted != 1 || g_handleDtors != 3 || g_freed != 1 || g_impDestroyed != 1 || g_blockDeleted != 1 || g_baseObj != 1 || g_tcBase != 1 || m->PDef.Object != 0)
        { std::printf("007e7ed0_TEST FAIL\n"); return 1; }
        free(m);
    }
    {
        def.RefCount = 2;
        CTCFireheartMinigame* m = (CTCFireheartMinigame*)malloc(sizeof(CTCFireheartMinigame));
        new (m) CTCFireheartMinigame;
        m->Handles._M_start = m->Handles._M_finish = 0; m->PDef.Object = &def; m->ScriptData.PImp.Info = 0;
        m->~CTCFireheartMinigame();
        if (g_defDeleted != 1 || def.RefCount != 1 || g_freed != 1 || g_blockDeleted != 1 || g_tcBase != 2) { std::printf("007e7ed0_TEST FAIL\n"); return 1; }
        free(m);
    }
    std::printf("007e7ed0_TEST PASS\n");
    return 0;
}
