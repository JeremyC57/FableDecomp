// Behaviour test for 00e9d800: the model below is a copy of src/00e9d800.cpp (compiled separately), driven by test doubles.
#include <cstdio>
#include <cstring>
#ifndef _MSC_VER
#define __fastcall
#define __forceinline inline
#endif
static int g_frames = 0, g_doneAt = 1000, g_aliveUntil = 1000, g_removed = 0, g_impDestroyed = 0, g_infoDeleted = 0, g_lookups = 0;
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
struct SScriptThingInfo {
    long m_references;
    void (__fastcall* m_pfnDestroy)(void*);
    void* m_pObject;
};
struct CScriptThingImpl {
    virtual bool IsAlive() { return g_frames < g_aliveUntil; }
};
class CScriptThing : public CBaseClass { public:
    CScriptThingImpl* PImp_Data;
    SScriptThingInfo* PImp_Info;
    CScriptThing() : PImp_Data(0), PImp_Info(0) {}
    CScriptThing(const CScriptThing& other) : CBaseClass(), PImp_Data(other.PImp_Data), PImp_Info(other.PImp_Info) { if (PImp_Info) ++PImp_Info->m_references; }
    ~CScriptThing()
    {
        if (PImp_Info) {
            --PImp_Info->m_references;
            if (PImp_Info->m_references == 0) {
                PImp_Info->m_pfnDestroy(PImp_Info->m_pObject);
                delete PImp_Info;
                ++g_infoDeleted;
            }
        }
        PImp_Data = 0;
        PImp_Info = 0;
    }
    bool IsAlive()
    {
        return PImp_Data && PImp_Data->IsAlive();
    }
};
static CScriptThing* g_briar = 0;
class CGameScriptInterface { public:
    virtual void Update() { ++g_frames; }
    virtual CScriptThing GetThingWithScriptName(const CCharString& name) { ++g_lookups; if (std::strcmp(name.PStringData, "BriarRose") != 0) g_lookups += 100; return *g_briar; }
    virtual void RemoveThing(const CScriptThing& thing, bool a, bool b) { ++g_removed; if (a || !b) g_removed += 100; if (thing.PImp_Data != (CScriptThingImpl*)0x1234) g_removed += 1000; }
};
class CTrophiesBriarTrophy {
public:
    void*                 __vftable;
    CGameScriptInterface* Game;
    CScriptThing          Me;
    bool IsDone();
    void Main();
};
bool CTrophiesBriarTrophy::IsDone() { return g_frames >= g_doneAt; }

void CTrophiesBriarTrophy::Main()
{
    CScriptThing briar = Game->GetThingWithScriptName("BriarRose");
    while (briar.IsAlive()) {
        Game->Update();
        if (IsDone())
            return;
    }
    if (IsDone())
        return;
    Game->RemoveThing(Me, false, true);
}

// ---- test doubles and driver ----
static void __fastcall DestroyImp(void* data) { delete (CScriptThingImpl*)data; ++g_impDestroyed; }

static bool Run(int doneAt, int aliveUntil, bool withImp, int expectFrames, int expectRemoved)
{
    static CGameScriptInterface gsi;
    CTrophiesBriarTrophy s; s.__vftable = 0; s.Game = &gsi; s.Me.PImp_Data = (CScriptThingImpl*)0x1234; s.Me.PImp_Info = 0;
    g_frames = 0; g_doneAt = doneAt; g_aliveUntil = aliveUntil; g_removed = g_impDestroyed = g_infoDeleted = g_lookups = 0;
    {
        CScriptThing holder;
        if (withImp) {
            CScriptThingImpl* imp = new CScriptThingImpl;
            SScriptThingInfo* info = new SScriptThingInfo; info->m_references = 1; info->m_pfnDestroy = DestroyImp; info->m_pObject = imp;
            holder.PImp_Data = imp; holder.PImp_Info = info;
        }
        g_briar = &holder;
        s.Main();
        if (g_impDestroyed != 0 || g_infoDeleted != 0) return false;   // the holder still owns one reference
    }
    if (withImp && (g_impDestroyed != 1 || g_infoDeleted != 1)) return false;
    if (!withImp && (g_impDestroyed != 0 || g_infoDeleted != 0)) return false;
    if (g_lookups != 1) return false;
    if (g_frames != expectFrames || g_removed != expectRemoved) return false;
    return true;
}

int main()
{
    bool ok = true;
    ok = ok && Run(1000, 3, true, 3, 1);     // dies on frame 3: trophy removed
    ok = ok && Run(1000, 0, true, 0, 1);     // already dead: no frame, removed
    ok = ok && Run(2, 1000, true, 2, 0);     // done while waiting: nothing
    ok = ok && Run(4, 4, true, 4, 0);        // dead and done on the same frame: the second test returns
    ok = ok && Run(1000, 1000, false, 0, 1); // no implementation: not alive, removed at once
    if (!ok) { std::printf("00e9d800_TEST FAIL\n"); return 1; }
    std::printf("00e9d800_TEST PASS\n");
    return 0;
}