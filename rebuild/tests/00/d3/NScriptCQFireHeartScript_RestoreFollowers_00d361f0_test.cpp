// Behaviour test for NScript::CQ_FireHeartScript::RestoreFollowers (0x00D361F0), g++ model of the retail logic.
#include <cstdio>
#include <vector>
struct Handle { bool valid; int releases; };
struct Follower { float alpha; bool emote; };
struct Script { int terminateAfter; int checks; std::vector<Follower> followers; std::vector<Handle> handles; };
static bool IsActiveThreadTerminating(Script& s) { return ++s.checks > s.terminateAfter; }
static void RestoreFollowers(Script& s)
{
    long count = (long)s.followers.size();
    for (long i = 0; i < count; ++i) {
        if (IsActiveThreadTerminating(s)) return;
        if (s.handles[i].valid) { s.handles[i].valid = false; ++s.handles[i].releases; }
        s.followers[i].alpha = 1.0f;
        s.followers[i].emote = true;
    }
    IsActiveThreadTerminating(s);
}
int main()
{
    Script s; s.terminateAfter = 100; s.checks = 0;
    Follower f = { 0.0f, false }; Handle h = { true, 0 }, g = { false, 0 };
    s.followers.push_back(f); s.followers.push_back(f); s.handles.push_back(h); s.handles.push_back(g);
    RestoreFollowers(s);
    int ok = s.followers[0].alpha == 1.0f && s.followers[1].emote && s.handles[0].releases == 1 && s.handles[1].releases == 0 && s.checks == 3;
    Script t; t.terminateAfter = 1; t.checks = 0; t.followers.push_back(f); t.followers.push_back(f); t.handles.push_back(h); t.handles.push_back(h);
    RestoreFollowers(t);
    ok &= t.followers[0].alpha == 1.0f && t.followers[1].alpha == 0.0f && t.handles[1].releases == 0;
    std::printf(ok ? "00d361f0_TEST PASS\n" : "00d361f0_TEST FAIL\n");
    return ok ? 0 : 1;
}