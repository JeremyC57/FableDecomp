// Test for 0x004AA840 CScriptThing::~CScriptThing: the PImp counted pointer drops its reference (DeleteFunc + info freed at
// zero) and is cleared, then ~CBaseClass.
#include <cstdio>
static int g_deleted = 0, g_baseDtors = 0;
class CCPPointerInfo { public:
    unsigned long RefCount; void (*DeleteFunc)(void*); void* Data;
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
class CBaseClass { public: virtual ~CBaseClass() { ++g_baseDtors; } };
class CScriptThing : public CBaseClass { public:
    virtual ~CScriptThing();
    CCountedPointer<CScriptThing> PImp;
};
CScriptThing::~CScriptThing() { }
int main()
{
    CCPPointerInfo* info = new CCPPointerInfo; info->RefCount = 2; info->DeleteFunc = DeleteIt; info->Data = 0;
    { CScriptThing a; a.PImp.Data = 0; a.PImp.Info = info; }
    if (g_deleted != 0 || info->RefCount != 1 || g_baseDtors != 1) { std::printf("004aa840_TEST FAIL\n"); return 1; }
    { CScriptThing b; b.PImp.Data = 0; b.PImp.Info = info; }
    if (g_deleted != 1 || g_baseDtors != 2) { std::printf("004aa840_TEST FAIL\n"); return 1; }
    { CScriptThing c; c.PImp.Data = 0; c.PImp.Info = 0; }
    if (g_deleted != 1 || g_baseDtors != 3) { std::printf("004aa840_TEST FAIL\n"); return 1; }
    std::printf("004aa840_TEST PASS\n");
    return 0;
}