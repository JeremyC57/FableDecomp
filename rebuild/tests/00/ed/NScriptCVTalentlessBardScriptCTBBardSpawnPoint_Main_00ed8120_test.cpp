// Behaviour test for CTB_BardSpawnPoint::Main (0x00ED8120): the created creature replaces the parent's stBard,
// the old handle's block is released (and freed) and the new block ends with one reference.
#include <cstdio>
#include <cstring>
#include <cstdlib>

static int g_deleteCount = 0;
static bool g_deleteArmed = false;
void operator delete(void* p) throw() { if (g_deleteArmed) ++g_deleteCount; (void)p; }

static char g_lastDef[64];
static char g_lastName[64];
class CCharString { public:
    void* PStringData;
    CCharString(const char* text, long = -1) { PStringData = (void*)text; }
    ~CCharString() {}
};
class C3DVector { public: float X, Y, Z; };
class CCPPointerInfo { public: long RefCount; void (*DeleteFunc)(void* data); void* Data; };
static int g_destroyed = 0;
static void DestroyData(void*) { ++g_destroyed; }

class CScriptThing;
struct CCountedScriptImp {
    CScriptThing* Data; CCPPointerInfo* Info;
    CCountedScriptImp() : Data(0), Info(0) {}
    CCountedScriptImp(const CCountedScriptImp& o) : Data(o.Data), Info(o.Info) { if (Info) ++Info->RefCount; }
    CCountedScriptImp& operator=(const CCountedScriptImp& other)
    {
        CCPPointerInfo* info = other.Info; CScriptThing* data = other.Data;
        if (Info != info) {
            if (Info) { --Info->RefCount; if (Info->RefCount == 0) { Info->DeleteFunc(Info->Data); delete Info; } }
            Data = data; Info = info; if (info) ++info->RefCount;
        }
        return *this;
    }
    ~CCountedScriptImp()
    {
        if (Info) { --Info->RefCount; if (Info->RefCount == 0) { Info->DeleteFunc(Info->Data); delete Info; } }
        Data = 0; Info = 0;
    }
};
class CBaseClass { public: virtual ~CBaseClass() {} };
static C3DVector g_pos = { 1.0f, 2.0f, 3.0f };
static const C3DVector* g_lastPos = 0;
class CScriptThing : public CBaseClass { public:
    CCountedScriptImp PImp;
    virtual ~CScriptThing() {}
    virtual void v01() const {} virtual void v02() const {} virtual void v03() const {} virtual void v04() const {}
    virtual void v05() const {}
    virtual const C3DVector& GetPos() const { return g_pos; }
};
static CCPPointerInfo* g_newInfo = 0;
static CScriptThing* g_newData = (CScriptThing*)0x1234;
class CGameScriptInterface { public:
    virtual void v000() {}
    virtual CScriptThing CreateCreature(const CCharString& defName, const C3DVector& position,
                                        const CCharString& scriptName, bool) const
    {
        std::strcpy(g_lastDef, (const char*)defName.PStringData);
        std::strcpy(g_lastName, (const char*)scriptName.PStringData);
        g_lastPos = &position;
        CScriptThing t; t.PImp.Data = g_newData; t.PImp.Info = g_newInfo; ++g_newInfo->RefCount;
        return t;
    }
};
struct CV_TalentlessBardScript { unsigned char _base[0x54]; CScriptThing stBard; };
class CEntityScriptBase { public: virtual void v00() {} CGameScriptInterface* Game; CScriptThing Me; };
struct CTB_BardSpawnPoint : CEntityScriptBase {
    CV_TalentlessBardScript* ParentClass; void* PMasterData;
    void Main()
    {
        CCharString scriptName("TB_Bard");
        CCharString defName("CREATURE_BOWERSTONE_BARD");
        ParentClass->stBard = Game->CreateCreature(defName, Me.GetPos(), scriptName, false);
    }
};

int main()
{
    CCPPointerInfo oldInfo = { 1, DestroyData, 0 };
    CCPPointerInfo newInfo = { 0, DestroyData, 0 };
    g_newInfo = &newInfo;
    CGameScriptInterface game;
    CV_TalentlessBardScript parent;
    parent.stBard.PImp.Data = (CScriptThing*)0x99; parent.stBard.PImp.Info = &oldInfo;
    CTB_BardSpawnPoint s;
    s.Game = &game; s.ParentClass = &parent; s.PMasterData = 0;
    s.Me.PImp.Data = 0; s.Me.PImp.Info = 0;
    g_deleteArmed = true;
    s.Main();
    g_deleteArmed = false;
    bool ok = std::strcmp(g_lastDef, "CREATURE_BOWERSTONE_BARD") == 0 && std::strcmp(g_lastName, "TB_Bard") == 0
        && g_lastPos == &g_pos && parent.stBard.PImp.Data == g_newData && parent.stBard.PImp.Info == &newInfo
        && newInfo.RefCount == 1 && oldInfo.RefCount == 0 && g_destroyed == 1 && g_deleteCount == 1;
    parent.stBard.PImp.Info = 0;
    if (!ok) { std::printf("00ed8120_TEST FAIL\n"); return 1; }
    std::printf("00ed8120_TEST PASS\n");
    return 0;
}