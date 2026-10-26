// Behaviour test for 0x00903810 CScriptGameResourceObjectScriptedThing::CreateNullHandle: the returned handle owns
// one new implementation bound to the script interface, with a reference count of 1 after the local is destroyed.
#include <cstdio>

static int g_deleted = 0;

class CCPPointerInfo { public:
    unsigned int RefCount;
    void (*DeleteFunc)(void* data);
    void* Data;
};
template <class T> class CCountedPointer {
public:
    T* Data;
    CCPPointerInfo* Info;
    static void DeleteData(void* data) { delete (T*)data; ++g_deleted; }
    CCountedPointer() : Data(0), Info(0) {}
    CCountedPointer(const CCountedPointer& other) : Data(other.Data), Info(other.Info) { if (Info) ++Info->RefCount; }
    ~CCountedPointer() { Release(); }
    void Release()
    {
        if (Info) {
            --Info->RefCount;
            if (Info->RefCount == 0) { Info->DeleteFunc(Info->Data); delete Info; }
        }
        Data = 0;
        Info = 0;
    }
    void Reset(T* p)
    {
        Release();
        Data = p;
        if (p) { Info = new CCPPointerInfo; Info->RefCount = 1; Info->DeleteFunc = &DeleteData; Info->Data = p; }
    }
};
class CGameScriptInterface { public: int Id; };
class CBaseObject { public: void* IntelligentPointer; CBaseObject() : IntelligentPointer(0) {} virtual ~CBaseObject() {} };
class CScriptGameResourceObjectBase : public CBaseObject { public: virtual ~CScriptGameResourceObjectBase() {} };
class CScriptGameResourceObjectScriptedThingBase : public CScriptGameResourceObjectBase {
public:
    CCountedPointer<CScriptGameResourceObjectScriptedThingBase> PImp;
    void SetImplementation(CScriptGameResourceObjectScriptedThingBase* imp) { PImp.Reset(imp); }
};
class CScriptGameResourceObjectScriptedThing : public CScriptGameResourceObjectScriptedThingBase {
public:
    CGameScriptInterface* ScriptInterface;
    CScriptGameResourceObjectScriptedThing(CGameScriptInterface& scriptInterface) : ScriptInterface(&scriptInterface) {}
    static CScriptGameResourceObjectScriptedThingBase CreateNullHandle(CGameScriptInterface& scriptInterface)
    {
        CScriptGameResourceObjectScriptedThingBase handle;
        handle.SetImplementation(new CScriptGameResourceObjectScriptedThing(scriptInterface));
        return handle;
    }
};

int main()
{
    CGameScriptInterface iface;
    iface.Id = 5;
    int fail = 0;
    {
        CScriptGameResourceObjectScriptedThingBase h = CScriptGameResourceObjectScriptedThing::CreateNullHandle(iface);
        if (!h.PImp.Data || !h.PImp.Info) fail = 1;
        else {
            if (h.PImp.Info->RefCount != 1) fail = 1;
            if (((CScriptGameResourceObjectScriptedThing*)h.PImp.Data)->ScriptInterface != &iface) fail = 1;
            if (h.PImp.Info->Data != (void*)h.PImp.Data) fail = 1;
        }
        if (g_deleted != 0) fail = 1;
    }
    if (g_deleted != 1) fail = 1;
    if (fail) { std::printf("00903810_TEST FAIL\n"); return 1; }
    std::printf("00903810_TEST PASS\n");
    return 0;
}