// Behaviour test for 0x00EF49A0 CVillager::Init: the crate handle takes the looked-up block (old one released,
// re-assigning the same block keeps the count).
#include <cstdio>
#include <cstring>
class CCharString { public: char text[32]; CCharString(const char* t, long length = -1) { std::strcpy(text, t); (void)length; } };
class CCPPointerInfo { public: long RefCount; void (*DeleteFunc)(void* data); void* Data; };
static int g_deleted = 0;
static void DeleteThing(void*) { ++g_deleted; }
template <class T> class CCountedPointer { public:
    T* Data; CCPPointerInfo* Info;
    CCountedPointer() : Data(0), Info(0) { }
    CCountedPointer(const CCountedPointer& o) : Data(o.Data), Info(o.Info) { if (Info) ++Info->RefCount; }
    ~CCountedPointer()
    {
        if (Info) { --Info->RefCount; if (Info->RefCount == 0) { Info->DeleteFunc(Info->Data); delete Info; } }
        Data = 0; Info = 0;
    }
    CCountedPointer& operator=(const CCountedPointer& other)
    {
        CCPPointerInfo* info = other.Info; T* data = other.Data;
        if (Info != info) {
            if (Info) { --Info->RefCount; if (Info->RefCount == 0) { Info->DeleteFunc(Info->Data); delete Info; } }
            Data = data; Info = info;
            if (info) ++info->RefCount;
        }
        return *this;
    }
};
class CBaseClass { public: virtual ~CBaseClass() { } };
class CScriptThing : public CBaseClass { public: CCountedPointer<CScriptThing> PImp; ~CScriptThing() { } };
static CCPPointerInfo* g_newBlock = 0;
static char g_lastName[32];
class CGameScriptInterface { public:
    virtual CScriptThing GetThingWithScriptName(const CCharString& name)
    {
        std::strcpy(g_lastName, name.text);
        CScriptThing t; t.PImp.Data = (CScriptThing*)g_newBlock; t.PImp.Info = g_newBlock; ++g_newBlock->RefCount;
        return t;
    }
};
struct CVillager {
    CGameScriptInterface* Game; CScriptThing Crate;
    void Init() { Crate = Game->GetThingWithScriptName("ScrumpyCrate"); }
};
int main()
{
    CGameScriptInterface g;
    CCPPointerInfo* oldBlock = new CCPPointerInfo; oldBlock->RefCount = 2; oldBlock->DeleteFunc = DeleteThing; oldBlock->Data = 0;
    g_newBlock = new CCPPointerInfo; g_newBlock->RefCount = 1; g_newBlock->DeleteFunc = DeleteThing; g_newBlock->Data = 0;
    CVillager v; v.Game = &g; v.Crate.PImp.Info = oldBlock;
    v.Init();
    bool ok = g_deleted == 0 && oldBlock->RefCount == 1 && v.Crate.PImp.Info == g_newBlock && g_newBlock->RefCount == 2
        && std::strcmp(g_lastName, "ScrumpyCrate") == 0;
    v.Init();                                         // same block again: no release, no extra reference kept
    ok = ok && g_newBlock->RefCount == 2 && g_deleted == 0;
    if (!ok) { std::printf("00ef49a0_TEST FAIL\n"); return 1; }
    std::printf("00ef49a0_TEST PASS\n");
    return 0;
}