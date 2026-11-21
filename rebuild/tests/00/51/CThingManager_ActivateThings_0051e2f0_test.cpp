// Behaviour test for 0x0051E2F0 CThingManager::ActivateThings (links the REAL row object). The things are raw retail-layout objects:
// a fake vtable whose slots 8 / 9 / 10 (OnCreate / InitialActivate / OnInitialActivate) log the call, the type byte +0x10, the Dead bit
// 0 of +0x91 and the InitialisationStage bits 0..2 of +0x94 (higher bits of that word set to check the mask). The manager is raw
// storage whose World reference +0x24 points at a CWorld buffer with SerialiseAboutToLoadHeroState at +0x101. CCharString and
// NProgressDisplay::DisplayProgress are stubs under the row's mangled names. The expected call log comes from an independent model:
// three passes in thing order (a progress line per thing in each), the held-back player creature, and the closing progress line.
#include <stdio.h>
#include <string.h>
namespace std {
template<class T> class allocator {};
template<class T, class A = allocator<T> > class vector { public: T* s; T* f; T* e; };   // STLport: _M_start, _M_finish, _M_end_of_storage
}
class CThing;
class CCharString { public:
    const char* Text;
    CCharString(const char* text, long length);
    ~CCharString();
};
class CThingManager { public:
    void ActivateThings(const std::vector<CThing*>& activate_these_things);
};
static int g_fail = 0, g_live = 0;
static char g_log[4096]; static int g_len = 0;
static void Log(const char* s) { int n = (int)strlen(s); if (g_len + n + 2 < (int)sizeof(g_log)) { memcpy(g_log + g_len, s, n); g_len += n; g_log[g_len++] = ' '; g_log[g_len] = 0; } }
static void Check(bool ok, int line) { if (!ok) { g_fail = 1; printf("check failed at line %d\n", line); } }
CCharString::CCharString(const char* text, long length) { Text = text; ++g_live; if (length != -1) { g_fail = 1; printf("length %ld\n", length); } }
CCharString::~CCharString() { --g_live; }
namespace NProgressDisplay {
void __fastcall DisplayProgress(const CCharString& text, float progress, bool a, bool b)
{
    if (progress != -1.0f || a || b) { g_fail = 1; printf("bad progress args\n"); }
    if (!strcmp(text.Text, "Activate Things")) Log("P1");
    else if (!strcmp(text.Text, "Activate Things: OnCreate")) Log("P2");
    else if (!strcmp(text.Text, "Activate Things: Initial Activate")) Log("P3");
    else if (!strcmp(text.Text, "Activate Things End")) Log("P4");
    else { g_fail = 1; printf("unexpected text %s\n", text.Text); }
}
}
class FakeThing { public:
    virtual void v0() {} virtual void v1() {} virtual void v2() {} virtual void v3() {}
    virtual void v4() {} virtual void v5() {} virtual void v6() {} virtual void v7() {}
    virtual void OnCreate() { Mark('C'); }              // slot 8
    virtual void InitialActivate() { Mark('I'); }       // slot 9
    virtual void OnInitialActivate() { Mark('A'); }     // slot 10
    unsigned char Raw[0xA0];                            // +0x04..
    int Id;
    void Mark(char c) { char b[16]; sprintf(b, "%c%d", c, Id); Log(b); }
    void Set(int id, int type, int stage, bool dead)
    {
        memset(Raw, 0xA5, sizeof(Raw)); Id = id;
        Raw[0x10 - 4] = (unsigned char)type;
        Raw[0x91 - 4] = (unsigned char)(dead ? 0xFF : 0xFE);           // only bit 0 is Dead
        unsigned long w = 0xFFFFFFF8ul | (unsigned long)stage;          // only bits 0..2 are the stage
        memcpy(&Raw[0x94 - 4], &w, 4);
    }
};
struct Spec { int type, stage; bool dead; };
static void Model(const Spec* t, int n, bool flag, char* out)
{
    char b[16]; out[0] = 0;
#define ADD(s) strcat(out, s), strcat(out, " ")
    for (int i = 0; i != n; ++i) { ADD("P1"); if (t[i].stage == 0 && !(t[i].type == 2 && flag)) { sprintf(b, "C%d", i); ADD(b); } }
    for (int i = 0; i != n; ++i) { ADD("P2"); if (t[i].stage == 1 && !(t[i].type == 2 && flag)) { sprintf(b, "I%d", i); ADD(b); } }
    int hero = -1;
    for (int i = 0; i != n; ++i) { ADD("P3"); if (t[i].stage == 2) { if (t[i].type == 2) hero = i; else { sprintf(b, "A%d", i); ADD(b); } } }
    if (hero >= 0 && !t[hero].dead && !flag) { sprintf(b, "A%d", hero); ADD(b); }
    ADD("P4");
#undef ADD
}
static void Run(const Spec* t, int n, bool flag, int line)
{
    static FakeThing things[16]; CThing* ptrs[16];
    for (int i = 0; i != n; ++i) { things[i].Set(i, t[i].type, t[i].stage, t[i].dead); ptrs[i] = (CThing*)&things[i]; }
    unsigned char world[0x200]; memset(world, 0x5A, sizeof(world)); world[0x101] = flag ? 1 : 0;
    unsigned char mgr[0x100]; memset(mgr, 0, sizeof(mgr)); void* pw = world; memcpy(mgr + 0x24, &pw, 4);
    std::vector<CThing*> v; v.s = ptrs; v.f = ptrs + n; v.e = ptrs + n;
    g_len = 0; g_log[0] = 0;
    ((CThingManager*)mgr)->ActivateThings(v);
    char want[4096]; Model(t, n, flag, want);
    Check(!strcmp(g_log, want), line);
    if (strcmp(g_log, want)) printf("  got  %s\n  want %s\n", g_log, want);
    Check(g_live == 0, line);
}
int main()
{
    // every type / stage / dead mix, a player creature at each stage, two player creatures at stage 2 (the last one is held back)
    Spec a[] = { { 1, 0, false }, { 2, 0, false }, { 3, 1, false }, { 2, 1, true }, { 1, 2, false }, { 2, 2, false }, { 4, 3, false },
                 { 2, 2, false }, { 0, 2, true }, { 15, 1, false }, { 2, 3, false }, { 1, 0, true } };
    Run(a, 12, false, __LINE__);
    Run(a, 12, true, __LINE__);                 // the world is about to load hero state: player creatures skipped, held hero not activated
    Spec b[] = { { 1, 2, false }, { 2, 2, true } };
    Run(b, 2, false, __LINE__);                 // the held hero is dead: not activated
    Spec c[] = { { 2, 2, false }, { 3, 2, false } };
    Run(c, 2, false, __LINE__);                 // the held hero is activated after the others
    Run(c, 2, true, __LINE__);
    Spec d[] = { { 3, 0, false }, { 3, 1, false }, { 3, 2, false } };
    Run(d, 3, true, __LINE__);                  // no player creature: the flag changes nothing
    Run(d, 0, false, __LINE__);                 // empty: only the closing progress line
    if (!g_fail) printf("0051e2f0_TEST PASS\n");
    return g_fail;
}
