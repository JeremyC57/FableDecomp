#include <cstdio>
// OpinionReactionRoot::OnReplace: stop speech; log the reaction when one was in progress on a known thing
struct Talk { int stopped; void StopCurrentSpeech() { ++stopped; } };
struct Manager { long type; void* creature; bool flag; int logs; void LogReaction(long t, void* c, bool f) { type = t; creature = c; flag = f; ++logs; } };
struct Group {
    void* creature; Talk* talk; bool hasTalk; long currentReactionType; void* thingReactingTo; Manager* manager; bool lastRunWasTryAndLook;
    void OnReplace()
    {
        if (hasTalk) talk->StopCurrentSpeech();
        if (currentReactionType != 0 && thingReactingTo != 0) { manager->LogReaction(currentReactionType, creature, false); lastRunWasTryAndLook = false; }
        else lastRunWasTryAndLook = true;
    }
};
int main()
{
    int me, thing; Talk t; t.stopped = 0; Manager m; m.logs = 0; m.flag = true;
    Group g; g.creature = &me; g.talk = &t; g.hasTalk = true; g.currentReactionType = 5; g.thingReactingTo = &thing; g.manager = &m; g.lastRunWasTryAndLook = true;
    g.OnReplace(); if (t.stopped != 1 || m.logs != 1 || m.type != 5 || m.creature != &me || m.flag || g.lastRunWasTryAndLook) { std::printf("0095f660_TEST FAIL\n"); return 1; }
    g.thingReactingTo = 0; g.OnReplace(); if (m.logs != 1 || !g.lastRunWasTryAndLook) { std::printf("0095f660_TEST FAIL\n"); return 1; }
    g.thingReactingTo = &thing; g.currentReactionType = 0; g.hasTalk = false; g.OnReplace(); if (t.stopped != 2 || m.logs != 1 || !g.lastRunWasTryAndLook) { std::printf("0095f660_TEST FAIL\n"); return 1; }
    std::printf("0095f660_TEST PASS\n"); return 0;
}
