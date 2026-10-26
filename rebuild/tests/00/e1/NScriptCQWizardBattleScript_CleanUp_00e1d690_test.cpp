// Behaviour test for CQ_WizardBattleScript::CleanUp (0x00E1D690): screamers collected and killed, the maze removed,
// override music stopped, the village looked up and released from limbo; every temporary, the village handle and the
// vector (elements + storage) are released.
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <new>

static int g_live = 0, g_thingDtors = 0, g_frees = 0, g_infoDeletes = 0;
class CCharString {
public:
    const char* PStringData;
    CCharString(const char* text, long = -1) : PStringData(text) { ++g_live; }
    ~CCharString() { --g_live; }
};
struct CCPPointerInfo { long RefCount; void (*DeleteFunc)(void*); void* Data; };
static void DeletePointee(void*) { ++g_infoDeletes; }
template <class T> struct CCountedPointer {
    T* Data; CCPPointerInfo* Info;
    ~CCountedPointer()
    {
        if (Info) { --Info->RefCount; if (Info->RefCount == 0) { Info->DeleteFunc(Info->Data); delete Info; } }
        Data = 0; Info = 0;
    }
};
struct CBaseClass { CBaseClass() {} virtual ~CBaseClass() { ++g_thingDtors; } };
struct CScriptThing : CBaseClass {
    CCountedPointer<CScriptThing> PImp;
    CScriptThing() { PImp.Data = 0; PImp.Info = 0; }
    CScriptThing(const CScriptThing& o) : CBaseClass() { PImp.Data = o.PImp.Data; PImp.Info = o.PImp.Info; if (PImp.Info) ++PImp.Info->RefCount; }
    ~CScriptThing() {}
};
template <class T> void _Destroy(T* first, T* last) { for (; first != last; ++first) first->~T(); }
template <class T> struct vector {
    T* _M_start; T* _M_finish; T* _M_end_of_storage;
    vector() : _M_start(0), _M_finish(0), _M_end_of_storage(0) { }
    ~vector() { _Destroy(_M_start, _M_finish); if (_M_start) { ++g_frees; std::free(_M_start); } }
};
static char g_log[512];
static void Log(const char* s) { std::strcat(g_log, s); std::strcat(g_log, ";"); }
struct CGameScriptInterface;
static void KillAllThingsInVector(CGameScriptInterface*, vector<CScriptThing>* v, bool b) { if (v->_M_finish - v->_M_start == 3 && !b) Log("kill3"); }
static CScriptThing g_pointee;
struct CScriptBase { virtual void v00() {} };
struct CQ_WizardBattleScript;
struct CGameScriptInterface {
    const CScriptThing* removed; const CScriptThing* village;
    virtual CScriptThing GetThingWithScriptName(const CCharString& n)
    {
        Log("find"); Log(n.PStringData);
        CScriptThing t; t.PImp.Data = &g_pointee; t.PImp.Info = new CCPPointerInfo;
        t.PImp.Info->RefCount = 1; t.PImp.Info->DeleteFunc = DeletePointee; t.PImp.Info->Data = &g_pointee;
        return t;
    }
    virtual long GetAllThingsWithScriptName(const CCharString& n, vector<CScriptThing>& v)
    {
        Log("all"); Log(n.PStringData);
        v._M_start = (CScriptThing*)std::malloc(4 * sizeof(CScriptThing));
        for (int i = 0; i < 3; ++i) new (&v._M_start[i]) CScriptThing();
        v._M_finish = v._M_start + 3; v._M_end_of_storage = v._M_start + 4;
        return 3;
    }
    virtual void RemoveThing(const CScriptThing& t, bool b1, bool b2) { Log("remove"); removed = &t; if (b1 || !b2) Log("badflags"); }
    virtual void SetVillageLimbo(const CScriptThing& v, bool limbo) { Log("limbo"); village = &v; if (limbo) Log("on"); }
    virtual void StopOverrideMusic(bool imm) { Log("music"); if (imm) Log("imm"); }
};
struct CQ_WizardBattleScript : CScriptBase {
    CGameScriptInterface* m_pInterface;
    CScriptThing Maze;
    void CleanUp();
};
void CQ_WizardBattleScript::CleanUp()
{
    vector<CScriptThing> screamers;
    m_pInterface->GetAllThingsWithScriptName("SpawnedScreamer", screamers);
    KillAllThingsInVector(m_pInterface, &screamers, false);
    m_pInterface->RemoveThing(Maze, false, true);
    m_pInterface->StopOverrideMusic(false);
    CScriptThing village = m_pInterface->GetThingWithScriptName("HookCoastVillage");
    m_pInterface->SetVillageLimbo(village, false);
}
int main()
{
    CGameScriptInterface gi; gi.removed = 0; gi.village = 0;
    CQ_WizardBattleScript s; s.m_pInterface = &gi;
    g_log[0] = 0; g_thingDtors = 0;
    s.CleanUp();
    int bad = std::strcmp(g_log, "all;SpawnedScreamer;kill3;remove;music;find;HookCoastVillage;limbo;") != 0;
    if (gi.removed != &s.Maze || g_live != 0 || g_frees != 1 || g_infoDeletes != 1 || g_thingDtors < 4) bad = 1;
    if (bad) { std::printf("00e1d690_TEST FAIL\n"); return 1; }
    std::printf("00e1d690_TEST PASS\n");
    return 0;
}