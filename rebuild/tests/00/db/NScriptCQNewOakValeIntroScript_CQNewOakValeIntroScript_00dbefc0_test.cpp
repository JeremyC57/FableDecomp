// Test for ~CQ_NewOakValeIntroScript (0x00DBEFC0): WatchTimer then TalkIntermittentTimer deregister (reverse declaration order),
// then the eight CCharString vectors are destroyed from VillagerSpeechFemaleNeither back to VillagerSpeechMaleGood, each
// destroying its elements in order and freeing its storage, then the CScriptBase base dies last.
#include <cstdio>
static int g_log[64]; static int g_n = 0;
struct Timer { int id; ~Timer() { g_log[g_n++] = id; } };
struct Str { int id; Str() : id(0) {} ~Str() { if (id) g_log[g_n++] = id; } };
struct Vec { Str* start; Str* finish; int id; ~Vec() { for (Str* p = start; p != finish; ++p) p->~Str(); g_log[g_n++] = id; } };
struct Base { ~Base() { g_log[g_n++] = 999; } };
static Str g_strs[8][2];
struct Script : Base {
    Vec v[8]; bool deeds[5]; Timer TalkIntermittentTimer; Timer WatchTimer;
};
int main()
{
    int expect[64]; int e = 0;
    {
        Script* s = (Script*)operator new(sizeof(Script));
        for (int i = 0; i < 8; ++i) { g_strs[i][0].id = 100 + i * 10; g_strs[i][1].id = 101 + i * 10; s->v[i].start = g_strs[i]; s->v[i].finish = g_strs[i] + 2; s->v[i].id = 1 + i; }
        s->TalkIntermittentTimer.id = 50; s->WatchTimer.id = 51;
        s->~Script(); operator delete(s);
    }
    expect[e++] = 51; expect[e++] = 50;
    for (int i = 7; i >= 0; --i) { expect[e++] = 100 + i * 10; expect[e++] = 101 + i * 10; expect[e++] = 1 + i; }
    expect[e++] = 999;
    bool ok = (g_n == e);
    for (int i = 0; ok && i < e; ++i) if (g_log[i] != expect[i]) ok = false;
    std::printf(ok ? "00dbefc0_TEST PASS\n" : "00dbefc0_TEST FAIL\n");
    return ok ? 0 : 1;
}