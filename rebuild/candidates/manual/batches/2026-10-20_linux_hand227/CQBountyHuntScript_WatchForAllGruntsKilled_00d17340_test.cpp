// Behaviour test for CQ_BountyHuntScript::WatchForAllGruntsKilled (0x00D17340): the grunts are collected by name, the
// thread waits frame by frame until all are dead and sets AllGruntsKilled; a terminate request at any test ends it;
// the local vector destroys every element and frees its storage on every exit.
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <new>

static int g_live = 0, g_thingDtors = 0, g_frees = 0;
class CCharString {
public:
    const char* PStringData;
    CCharString(const char* text, long = -1) : PStringData(text) { ++g_live; }
    ~CCharString() { --g_live; }
};
struct CBaseClass { CBaseClass() {} virtual ~CBaseClass() { ++g_thingDtors; } };
struct CScriptThing : CBaseClass { void* PImp_Data; void* PImp_Info; };
template <class T> void _Destroy(T* first, T* last) { for (; first != last; ++first) first->~T(); }
template <class T> struct vector {
    T* _M_start; T* _M_finish; T* _M_end_of_storage;
    vector() : _M_start(0), _M_finish(0), _M_end_of_storage(0) { }
    ~vector() { _Destroy(_M_start, _M_finish); if (_M_start) { ++g_frees; std::free(_M_start); } }
};
static const char* g_dead;     // per AreAllThingsInVectorDead call
static const char* g_term;     // per IsActiveThreadTerminating call
static int g_frames = 0;
static bool AreAllThingsInVectorDead(vector<CScriptThing>* v) { char c = *g_dead; if (c) ++g_dead; return v->_M_finish - v->_M_start == 2 && c == '1'; }
struct CGameScriptInterface {
    const char* lastName;
    virtual void NewScriptFrame() { ++g_frames; }
    virtual long GetAllThingsWithScriptName(const CCharString& n, vector<CScriptThing>& v)
    {
        lastName = n.PStringData;
        v._M_start = (CScriptThing*)std::malloc(3 * sizeof(CScriptThing));
        new (&v._M_start[0]) CScriptThing(); new (&v._M_start[1]) CScriptThing();
        v._M_finish = v._M_start + 2; v._M_end_of_storage = v._M_start + 3;
        return 2;
    }
};
struct CScriptBase { bool IsActiveThreadTerminating() { char c = *g_term; if (c) ++g_term; return c != '0'; } };
struct CQ_BountyHuntScript : CScriptBase {
    CGameScriptInterface* m_pInterface;
    bool AllGruntsKilled;
    void WatchForAllGruntsKilled();
};
void CQ_BountyHuntScript::WatchForAllGruntsKilled()
{
    vector<CScriptThing> grunts;
    m_pInterface->GetAllThingsWithScriptName("BH_GangGruntSpawn", grunts);
    for (;;) {
        if (IsActiveThreadTerminating())
            return;
        if (AreAllThingsInVectorDead(&grunts))
            break;
        m_pInterface->NewScriptFrame();
    }
    if (IsActiveThreadTerminating())
        return;
    if (IsActiveThreadTerminating())
        return;
    AllGruntsKilled = true;
}
static int Run(const char* dead, const char* term, bool killed, int frames)
{
    CGameScriptInterface gi; gi.lastName = 0;
    CQ_BountyHuntScript s; s.m_pInterface = &gi; s.AllGruntsKilled = false;
    g_dead = dead; g_term = term; g_frames = 0; g_thingDtors = 0; g_frees = 0;
    s.WatchForAllGruntsKilled();
    if (*g_dead || *g_term || g_frames != frames || s.AllGruntsKilled != killed) return 1;
    if (g_thingDtors != 2 || g_frees != 1 || g_live != 0 || std::strcmp(gi.lastName, "BH_GangGruntSpawn") != 0) return 1;
    return 0;
}
int main()
{
    int bad = 0;
    bad |= Run("001", "00000", true, 2);
    bad |= Run("0", "01", false, 1);
    bad |= Run("1", "01", false, 0);
    bad |= Run("1", "001", false, 0);
    bad |= Run("", "1", false, 0);
    if (bad) { std::printf("00d17340_TEST FAIL\n"); return 1; }
    std::printf("00d17340_TEST PASS\n");
    return 0;
}
