// Behaviour test for 00525cd0: body copied from src/00525cd0.cpp (CThingManager::OnLevelsUnload), callees stubbed.
#include <cstdio>
#include <cstring>
#include <cstdlib>
static int fails = 0;
#define CHECK(c) do { if (!(c)) { ++fails; printf("FAIL %d: %s\n", __LINE__, #c); } } while (0)
static int g_frees = 0;
static void myfree(void* p) { ++g_frees; free(p); }
static const char* g_log[64]; static int g_logN = 0;
class CThing {
public:
    long Level; bool Ready; int Prepared; int Killed; bool KillArg;
    bool PrepareForLevelUnload_005271b0() { ++Prepared; g_log[g_logN++] = "prepare"; return true; }
    void Kill(bool immediately) { ++Killed; KillArg = immediately; g_log[g_logN++] = "kill"; }
};
struct CLongVector {
    long* _M_start;
    long* _M_finish;
    long* _M_end_of_storage;
    unsigned long size() const { return _M_finish - _M_start; }
    const long& operator[](unsigned long n) const { return *(_M_start + n); }
};
struct CThingVector {
    CThing** _M_start;
    CThing** _M_finish;
    CThing** _M_end_of_storage;
    CThingVector() : _M_start(0), _M_finish(0), _M_end_of_storage(0) { }
    ~CThingVector() { if (_M_start) myfree(_M_start); }
    CThing** begin() { return _M_start; }
    CThing** end() { return _M_finish; }
};
template<class R, class T> struct mem_fun_t {
    R (T::*m_f)();
    explicit mem_fun_t(R (T::*f)()) : m_f(f) { }
    R operator()(T* p) const { return (p->*m_f)(); }
};
template<class R, class T, class A> struct mem_fun1_t {
    R (T::*m_f)(A);
    explicit mem_fun1_t(R (T::*f)(A)) : m_f(f) { }
    R operator()(T* p, A a) const { return (p->*m_f)(a); }
};
template<class R, class T> inline mem_fun_t<R, T> mem_fun(R (T::*f)()) { return mem_fun_t<R, T>(f); }
template<class R, class T, class A> inline mem_fun1_t<R, T, A> mem_fun(R (T::*f)(A)) { return mem_fun1_t<R, T, A>(f); }
template<class R, class T, class A> struct binder2nd {
    mem_fun1_t<R, T, A> op;
    A value;
    binder2nd(const mem_fun1_t<R, T, A>& o, A v) : op(o), value(v) { }
    R operator()(T* p) const { return op(p, value); }
};
template<class R, class T, class A> inline binder2nd<R, T, A> bind2nd(const mem_fun1_t<R, T, A>& o, A v) { return binder2nd<R, T, A>(o, v); }
template<class I, class F> inline F for_each(I first, I last, F f)
{
    for (; first != last; ++first)
        f(*first);
    return f;
}
static CThing* g_things[8]; static int g_thingCount = 0; static int g_saveCalls = 0; static long g_saveLevels[8]; static bool g_saveFlag = false; static int g_updateDeaths = 0;
class CThingManager {
public:
    CThing* FirstThingOnLevel(long level) { for (int i = 0; i < g_thingCount; ++i) if (g_things[i]->Level == level) return g_things[i]; return 0; }
    CThing* NextThingOnLevel(long level, CThing* previous)
    {
        bool seen = false;
        for (int i = 0; i < g_thingCount; ++i) { if (seen && g_things[i]->Level == level) return g_things[i]; if (g_things[i] == previous) seen = true; }
        return 0;
    }
    bool ReadyForUnloading(CThing* thing, bool flag) { g_saveFlag = flag; return thing->Ready; }
    void SaveThingsOnLevelReadyForUnloading(long level, bool flag, CThingVector* things);
    void UpdateDeaths() { ++g_updateDeaths; g_log[g_logN++] = "deaths"; }
    void OnLevelsUnload(const CLongVector& levels, bool flag);
};
void CThingManager::SaveThingsOnLevelReadyForUnloading(long level, bool flag, CThingVector* things)
{
    g_saveLevels[g_saveCalls++] = level;
    for (CThing* thing = FirstThingOnLevel(level); thing; thing = NextThingOnLevel(level, thing)) {
        if (!ReadyForUnloading(thing, flag))
            continue;
        if (things->_M_finish == things->_M_end_of_storage) {
            unsigned long size = things->_M_finish - things->_M_start;
            unsigned long len = size ? 2 * size : 8;
            CThing** start = (CThing**)malloc(len * sizeof(CThing*));
            for (unsigned long i = 0; i < size; ++i)
                start[i] = things->_M_start[i];
            if (things->_M_start)
                myfree(things->_M_start);
            things->_M_start = start;
            things->_M_finish = start + size;
            things->_M_end_of_storage = start + len;
        }
        *things->_M_finish++ = thing;
    }
}
void CThingManager::OnLevelsUnload(const CLongVector& levels, bool flag)
{
    CThingVector things;
    for (unsigned long i = 0; i < levels.size(); ++i)
        SaveThingsOnLevelReadyForUnloading(levels[i], flag, &things);
    for_each(things.begin(), things.end(), mem_fun(&CThing::PrepareForLevelUnload_005271b0));
    for_each(things.begin(), things.end(), bind2nd(mem_fun(&CThing::Kill), false));
    UpdateDeaths();
}
int main()
{
    static CThing t[4]; memset(t, 0, sizeof(t));
    t[0].Level = 1; t[0].Ready = true; t[1].Level = 2; t[1].Ready = true; t[2].Level = 1; t[2].Ready = false; t[3].Level = 3; t[3].Ready = true;
    for (int i = 0; i < 4; ++i) g_things[i] = &t[i]; g_thingCount = 4;
    long levels[2] = { 1, 2 }; CLongVector lv; lv._M_start = levels; lv._M_finish = levels + 2; lv._M_end_of_storage = levels + 2;
    CThingManager mgr;
    g_logN = 0; g_frees = 0; g_saveCalls = 0;
    mgr.OnLevelsUnload(lv, true);
    CHECK(g_saveCalls == 2 && g_saveLevels[0] == 1 && g_saveLevels[1] == 2); CHECK(g_saveFlag);
    CHECK(t[0].Prepared == 1 && t[1].Prepared == 1 && t[2].Prepared == 0 && t[3].Prepared == 0);
    CHECK(t[0].Killed == 1 && t[1].Killed == 1 && t[2].Killed == 0 && t[3].Killed == 0); CHECK(!t[0].KillArg && !t[1].KillArg);
    CHECK(g_updateDeaths == 1); CHECK(g_frees == 1);
    CHECK(g_logN == 5 && strcmp(g_log[0], "prepare") == 0 && strcmp(g_log[1], "prepare") == 0 && strcmp(g_log[2], "kill") == 0 && strcmp(g_log[3], "kill") == 0 && strcmp(g_log[4], "deaths") == 0);
    // no levels: nothing collected, nothing freed, deaths still updated
    g_logN = 0; g_frees = 0; g_saveCalls = 0; lv._M_finish = levels;
    mgr.OnLevelsUnload(lv, false);
    CHECK(g_saveCalls == 0 && g_frees == 0 && g_updateDeaths == 2 && g_logN == 1);
    if (fails == 0) printf("00525cd0_TEST PASS\n"); else printf("00525cd0_TEST FAIL\n");
    return fails;
}