#include <cstdio>
struct CCPPointerInfo {
    long  RefCount;
    void  (__fastcall* DeleteFunc)(void* data);
    void* Data;
};
template <class T> class CCountedPointer {
public:
    T*              Data;
    CCPPointerInfo* Info;
    ~CCountedPointer()
    {
        if (Info)
        {
            --Info->RefCount;
            if (Info->RefCount == 0)
            {
                Info->DeleteFunc(Info->Data);
                delete Info;
            }
        }
        Data = 0;
        Info = 0;
    }
};
class CBaseClass {
public:
    virtual ~CBaseClass();                               // 0x0099A2E0
};
class CScriptThing : public CBaseClass {
public:
    CCountedPointer<CScriptThing> PImp;                  // +0x04
    virtual ~CScriptThing() {}
};
class CGameScriptInterfaceBase;
class CEntityScriptBase {
public:
    CGameScriptInterfaceBase* Game;                      // +0x04
    CScriptThing              Me;                        // +0x08
    virtual ~CEntityScriptBase();
};
CEntityScriptBase::~CEntityScriptBase()
{
}


static int g_base, g_deleted; static void* g_deleted_data;
CBaseClass::~CBaseClass() { ++g_base; }
static void __fastcall DeleteIt(void* data) { ++g_deleted; g_deleted_data = data; }
int main()
{
    static int payload;
    bool ok = true;
    {
        CEntityScriptBase* s = new CEntityScriptBase;
        CCPPointerInfo* info = new CCPPointerInfo; info->RefCount = 1; info->DeleteFunc = DeleteIt; info->Data = &payload;
        s->Me.PImp.Data = 0; s->Me.PImp.Info = info;
        delete s;
        ok = ok && g_deleted == 1 && g_deleted_data == &payload && g_base == 1;
    }
    {
        CEntityScriptBase* s = new CEntityScriptBase;
        CCPPointerInfo info; info.RefCount = 2; info.DeleteFunc = DeleteIt; info.Data = &payload;
        s->Me.PImp.Info = &info;
        delete s;
        ok = ok && g_deleted == 1 && info.RefCount == 1 && g_base == 2;
    }
    if (ok) std::printf("00f35b40_TEST PASS\n");
    return 0;
}