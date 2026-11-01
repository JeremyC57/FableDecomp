// Test for 0x007E74D0 CScriptGameResourceObjectScriptedThingBase::~CScriptGameResourceObjectScriptedThingBase (implicit): PImp drops its reference (DeleteFunc + info freed at zero) and is cleared, then
// the inline ~CScriptGameResourceObjectBase and ~CBaseObject run.
#include <cstdio>
static int g_deleted = 0, g_groBase = 0, g_baseObject = 0;
class CCPPointerInfo { public:
    unsigned int RefCount; void (*DeleteFunc)(void*); void* Data;
};
static void DeleteIt(void*) { ++g_deleted; }
template <class T> class CCountedPointer { public:
    T* Data; CCPPointerInfo* Info;
    ~CCountedPointer()
    {
        if (Info) { --Info->RefCount; if (Info->RefCount == 0) { Info->DeleteFunc(Info->Data); delete Info; } }
        Data = 0; Info = 0;
    }
};
class CBaseClass { public: virtual ~CBaseClass() { } };
class CBaseObject : public CBaseClass { public:
    virtual ~CBaseObject() { ++g_baseObject; }
    void* IntelligentPointer;
};
class CScriptGameResourceObjectBase : public CBaseObject { public:
    virtual ~CScriptGameResourceObjectBase() { ++g_groBase; }
};
class CScriptGameResourceObjectScriptedThingBase : public CScriptGameResourceObjectBase { public:
    CCountedPointer<CScriptGameResourceObjectScriptedThingBase> PImp;
};
int main()
{
    CCPPointerInfo* info = new CCPPointerInfo; info->RefCount = 2; info->DeleteFunc = DeleteIt; info->Data = 0;
    { CScriptGameResourceObjectScriptedThingBase a; a.PImp.Data = 0; a.PImp.Info = info; }
    if (g_deleted != 0 || info->RefCount != 1 || g_groBase != 1 || g_baseObject != 1) { std::printf("007e74d0_TEST FAIL\n"); return 1; }
    { CScriptGameResourceObjectScriptedThingBase b; b.PImp.Data = 0; b.PImp.Info = info; }
    if (g_deleted != 1 || g_groBase != 2 || g_baseObject != 2) { std::printf("007e74d0_TEST FAIL\n"); return 1; }
    { CScriptGameResourceObjectScriptedThingBase c; c.PImp.Data = 0; c.PImp.Info = 0; }
    if (g_deleted != 1 || g_groBase != 3) { std::printf("007e74d0_TEST FAIL\n"); return 1; }
    std::printf("007e74d0_TEST PASS\n");
    return 0;
}