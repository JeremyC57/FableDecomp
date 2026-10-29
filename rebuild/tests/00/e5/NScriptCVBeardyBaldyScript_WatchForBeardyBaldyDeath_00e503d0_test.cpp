// Behaviour test for 00e503d0: the model below is a copy of src/00e503d0.cpp (compiled separately), driven by test doubles.
#include <cstdio>
#include <cstring>
#ifndef _MSC_VER
#define __fastcall
#define __forceinline inline
#endif
static int g_frames = 0, g_termAt = 1000, g_killedAt = 1000, g_impDestroyed = 0, g_infoDeleted = 0, g_lookups = 0, g_killedQueries = 0;
class CCharString { public:
    const char* PStringData;
    CCharString(const char* text, long length = -1) : PStringData(text) { (void)length; }
    ~CCharString() {}
};
class CBaseClass {
public:
    virtual ~CBaseClass() {}
    CBaseClass() {}
};
class CCPPointerInfo { public:
    long RefCount;
    void (__fastcall* DeleteFunc)(void* data);
    void* Data;
};
template <class T> class CCountedPointer { public:
    T*              Data;
    CCPPointerInfo* Info;
    ~CCountedPointer()
    {
        if (Info) {
            --Info->RefCount;
            if (Info->RefCount == 0) {
                Info->DeleteFunc(Info->Data);
                delete Info;
                ++g_infoDeleted;
            }
        }
        Data = 0;
        Info = 0;
    }
};
struct CScriptThingImpl {
    virtual bool MsgIsKilledBy(const CCharString& killer) { ++g_killedQueries; if (std::strcmp(killer.PStringData, "") != 0) g_killedQueries += 100; return g_frames >= g_killedAt; }
};
class CScriptThing : public CBaseClass { public:
    CCountedPointer<CScriptThingImpl> PImp;
    CScriptThing() { PImp.Data = 0; PImp.Info = 0; }
    CScriptThing(const CScriptThing& other) : CBaseClass() { PImp.Data = other.PImp.Data; PImp.Info = other.PImp.Info; if (PImp.Info) ++PImp.Info->RefCount; }
    ~CScriptThing() {}
    bool MsgIsKilledBy(const CCharString& killer)
    {
        if (!PImp.Data)
            return false;
        return PImp.Data->MsgIsKilledBy(killer);
    }
};
static CScriptThing* g_beardy = 0;
class CGameScriptInterface { public:
    virtual void NewScriptFrame() const { ++g_frames; }
    virtual CScriptThing GetThingWithScriptName(const CCharString& name) { ++g_lookups; if (std::strcmp(name.PStringData, "BB_BeardyBaldyMan") != 0) g_lookups += 100; return *g_beardy; }
};
class CScriptBase { public:
    virtual void v00() {}
    unsigned char _body[0x3c];
    bool IsActiveThreadTerminating();
};
struct CQuestScriptBase : CScriptBase {
    CGameScriptInterface* GSI;
    void*                 Owner;
};
struct CV_BeardyBaldyScript : CQuestScriptBase {
    unsigned char _pad48[0x20];
    bool BeardyBaldyKilled;
    void WatchForBeardyBaldyDeath();
};
bool CScriptBase::IsActiveThreadTerminating() { return g_frames >= g_termAt; }

void CV_BeardyBaldyScript::WatchForBeardyBaldyDeath()
{
    CScriptThing beardy = GSI->GetThingWithScriptName("BB_BeardyBaldyMan");
    while (!beardy.MsgIsKilledBy("")) {
        GSI->NewScriptFrame();
        if (IsActiveThreadTerminating())
            return;
    }
    if (IsActiveThreadTerminating())
        return;
    BeardyBaldyKilled = true;
}

// ---- test doubles and driver ----
static void __fastcall DestroyImp(void* data) { delete (CScriptThingImpl*)data; ++g_impDestroyed; }

static bool Run(int termAt, int killedAt, bool withImp, int expectFrames, bool expectKilled)
{
    static CGameScriptInterface gsi;
    CV_BeardyBaldyScript s; std::memset(&s, 0, sizeof(s)); s.GSI = &gsi;
    g_frames = 0; g_termAt = termAt; g_killedAt = killedAt; g_impDestroyed = g_infoDeleted = g_lookups = g_killedQueries = 0;
    {
        CScriptThing holder;
        if (withImp) {
            CScriptThingImpl* imp = new CScriptThingImpl;
            CCPPointerInfo* info = new CCPPointerInfo; info->RefCount = 1; info->DeleteFunc = DestroyImp; info->Data = imp;
            holder.PImp.Data = imp; holder.PImp.Info = info;
        }
        g_beardy = &holder;
        s.WatchForBeardyBaldyDeath();
        if (g_impDestroyed != 0 || g_infoDeleted != 0) return false;   // the holder still owns one reference
    }
    if (withImp && (g_impDestroyed != 1 || g_infoDeleted != 1)) return false;
    if (!withImp && (g_impDestroyed != 0 || g_infoDeleted != 0)) return false;
    if (g_lookups != 1) return false;
    if (g_frames != expectFrames || s.BeardyBaldyKilled != expectKilled) return false;
    return true;
}

int main()
{
    bool ok = true;
    ok = ok && Run(1000, 3, true, 3, true);      // killed on frame 3: flag set
    ok = ok && Run(1000, 0, true, 0, true);      // already dead: no frame, flag set
    ok = ok && Run(2, 1000, true, 2, false);     // terminated while waiting: nothing
    ok = ok && Run(4, 4, true, 4, false);        // killed and terminating on the same frame: the second test returns
    ok = ok && Run(3, 1000, false, 3, false);    // no implementation: never killed, terminated on frame 3
    if (!ok) { std::printf("00e503d0_TEST FAIL\n"); return 1; }
    std::printf("00e503d0_TEST PASS\n");
    return 0;
}