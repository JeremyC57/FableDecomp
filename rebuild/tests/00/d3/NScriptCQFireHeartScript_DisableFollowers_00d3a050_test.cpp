// Behaviour test for NScript::CQ_FireHeartScript::DisableFollowers 0x00D3A050 (model copy with a fake game interface).
#include <cstdio>
struct Thing { int id; float alpha; bool emote; int scripted; };
struct Handle { int valid; int released; Thing* target; bool IsValid() const { return valid != 0; } void Release() { ++released; valid = 0; } };
struct Game {
    Thing hero; Thing* followers; int followerCount; int frames; int failFirst;
    Thing& GetHero() { return hero; }
    bool GetFollowingEntityList(const Thing&, Thing* out, int& n) { for (int i = 0; i < followerCount; ++i) out[i] = followers[i]; n = followerCount; return true; }
    bool StartScriptingEntity(Thing& t, Handle& h, int priority) { if (failFirst > 0) { --failFirst; return false; } h.valid = 1; h.target = &t; t.scripted = priority; return true; }
    void NewScriptFrame() { ++frames; }
    void EntitySetAlpha(Thing& t, float a, bool) { t.alpha = a; }
    void EntitySetAsDisplayingEmoteIcon(Thing& t, bool d) { t.emote = d; }
};
struct Script {
    Game* G; int terminateAfter; int checks;
    Thing Followers[8]; int FollowerCount; Handle FollowerHandles[8];
    bool IsActiveThreadTerminating() { ++checks; return terminateAfter >= 0 && checks > terminateAfter; }
    void DisableFollowers()
    {
        G->GetFollowingEntityList(G->GetHero(), Followers, FollowerCount);
        long count = FollowerCount;
        for (long i = 0; i < count; ++i) {
            if (IsActiveThreadTerminating()) return;
            Handle* handles = FollowerHandles;
            if (handles[i].IsValid()) handles[i].Release();
            while (!G->StartScriptingEntity(Followers[i], FollowerHandles[i], 4)) {
                G->NewScriptFrame();
                if (IsActiveThreadTerminating()) return;
            }
            if (IsActiveThreadTerminating()) return;
            G->EntitySetAlpha(Followers[i], 0.0f, true);
            G->EntitySetAsDisplayingEmoteIcon(Followers[i], false);
        }
        if (IsActiveThreadTerminating()) return;
    }
};
int main()
{
    Thing src[3]; for (int i = 0; i < 3; ++i) { src[i].id = i; src[i].alpha = 1.0f; src[i].emote = true; src[i].scripted = 0; }
    Game g; g.followers = src; g.followerCount = 3; g.frames = 0; g.failFirst = 2;
    Script s; s.G = &g; s.terminateAfter = -1; s.checks = 0;
    for (int i = 0; i < 8; ++i) { s.FollowerHandles[i].valid = 0; s.FollowerHandles[i].released = 0; }
    s.FollowerHandles[1].valid = 1;
    s.DisableFollowers();
    bool ok = g.frames == 2 && s.FollowerHandles[1].released == 1 && s.FollowerHandles[0].released == 0;
    for (int i = 0; i < 3; ++i) if (s.Followers[i].alpha != 0.0f || s.Followers[i].emote || s.Followers[i].scripted != 4 || !s.FollowerHandles[i].valid) ok = false;
    Script t; t.G = &g; t.terminateAfter = 2; t.checks = 0; g.failFirst = 0;
    for (int i = 0; i < 8; ++i) { t.FollowerHandles[i].valid = 0; t.FollowerHandles[i].released = 0; }
    t.DisableFollowers();
    if (t.Followers[0].alpha != 0.0f || t.Followers[1].alpha != 1.0f) ok = false;
    if (!ok) { std::printf("00d3a050_TEST FAIL\n"); return 1; }
    std::printf("00d3a050_TEST PASS\n");
    return 0;
}