// behaviour test for CAIStateGroup_TavernServer::PutDownJug (0x00967A20): logic copy over stubs
#include <cstdio>
#include <cstring>
struct Thing { unsigned char flags91; bool hasHolder; void* holder; };
static int g_strings = 0;
struct Str { const char* s; Str(const char* t) : s(t) { g_strings++; } ~Str() { g_strings--; } };
struct Action { const char* anim; long mode; Action(const Str& a, long m) : anim(a.s), mode(m) { } };
struct Server {
    Thing* jug; long servingPoint; long actionResult; int barFreed; const char* lastAnim; long lastMode;
    long PerformActionAndWait(Action* a) { lastAnim = a->anim; lastMode = a->mode; return actionResult; }
    bool PutDownJug()
    {
        bool failed;
        {
            Action action(Str("PUT_EMPTY_JUG_ON_BAR"), 0x29);
            failed = PerformActionAndWait(&action) != 2;
        }
        barFreed++;
        if (failed) return false;
        if (jug && !(jug->flags91 & 1) && jug->hasHolder) jug->holder = 0;
        jug = 0;
        servingPoint = -1;
        return true;
    }
};
int main()
{
    int dummy;
    Thing j = { 0, true, &dummy };
    Server s = { &j, 5, 2, 0, 0, 0 };
    bool ok = s.PutDownJug() && j.holder == 0 && s.jug == 0 && s.servingPoint == -1 && s.barFreed == 1
        && std::strcmp(s.lastAnim, "PUT_EMPTY_JUG_ON_BAR") == 0 && s.lastMode == 0x29 && g_strings == 0;
    Thing k = { 0, true, &dummy };
    Server t = { &k, 5, 4, 0, 0, 0 };
    if (t.PutDownJug() || t.jug != &k || t.servingPoint != 5 || t.barFreed != 1 || k.holder != &dummy) ok = false;
    if (!ok) { std::printf("00967a20_TEST FAIL\n"); return 1; }
    std::printf("00967a20_TEST PASS\n");
    return 0;
}
