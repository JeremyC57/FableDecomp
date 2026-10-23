// Behaviour test for CFH_Villager::Main (0x00D37530): group-hit bookkeeping, comment flag, escape run and fade/remove.
#include <cstdio>
#include <cstring>

static char g_log[512];
static void Log(const char* s) { std::strcat(g_log, s); std::strcat(g_log, ";"); }
struct Parent {
    bool MissionSucceeded;
    bool GroupOne, GroupTwo;
    bool Speak[6];
};
struct Sim { int frame, hitAt, succeedAt; float dist; };
static Sim s;
struct Villager {
    Parent* P;
    long MyID;
    bool IsActiveThreadTerminating() { return s.frame > 40; }
    void NewScriptFrame() { ++s.frame; if (s.frame == s.succeedAt) P->MissionSucceeded = true; }
    bool HitByHero() { return s.frame == s.hitAt; }
    void Main();
};
void Villager::Main()
{
    if ((MyID < 3 && P->GroupOne) || (MyID >= 3 && P->GroupTwo)) {
        Log("remove");
        return;
    }
    while (!P->MissionSucceeded) {
        if (MyID < 3) {
            if (P->GroupOne)
                break;
        } else if (P->GroupTwo)
            break;
        NewScriptFrame();
        if (IsActiveThreadTerminating())
            return;
        if (HitByHero()) {
            if (MyID < 3) P->GroupOne = true; else P->GroupTwo = true;
        } else if (P->Speak[MyID]) {
            Log("comment");
            P->Speak[MyID] = false;
        }
    }
    if ((MyID < 3 && P->GroupOne) || (MyID >= 3 && P->GroupTwo)) {
        Log("run");
        if (s.dist <= 16.0f) {
            Log("fade");
            return;
        }
    }
    Log("remove");
}
static bool Run(Parent p, long id, int hitAt, int succeedAt, float dist, const char* expect)
{
    Villager v; v.P = &p; v.MyID = id;
    s.frame = 0; s.hitAt = hitAt; s.succeedAt = succeedAt; s.dist = dist;
    g_log[0] = 0;
    v.Main();
    return std::strcmp(g_log, expect) == 0;
}
int main()
{
    Parent p; std::memset(&p, 0, sizeof(p));
    bool ok = true;
    Parent hit1 = p; hit1.GroupOne = true;
    ok = ok && Run(hit1, 1, -1, 5, 1.0f, "remove;");
    ok = ok && Run(hit1, 4, -1, 5, 1.0f, "remove;");
    Parent talk = p; talk.Speak[2] = true;
    ok = ok && Run(talk, 2, -1, 5, 1.0f, "comment;remove;");
    ok = ok && Run(p, 4, 3, -1, 10.0f, "run;fade;");
    ok = ok && Run(p, 4, 3, -1, 20.0f, "run;remove;");
    if (!ok) {
        std::printf("00d37530_TEST FAIL\n");
        return 1;
    }
    std::printf("00d37530_TEST PASS\n");
    return 0;
}
