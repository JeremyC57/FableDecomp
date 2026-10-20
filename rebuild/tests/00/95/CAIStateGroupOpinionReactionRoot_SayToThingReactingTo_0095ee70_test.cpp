// behaviour test for CAIStateGroup_OpinionReactionRoot::SayToThingReactingTo (0x0095EE70): logic copy over stubs
#include <cstdio>
struct Thing { unsigned char flags91; unsigned char typeFlags; };
struct Group {
    Thing* reactingTo; Thing* self; bool allowNonPure;
    int calls7, calls6; Thing* lastTarget; long lastPriority;
    long Speak7(Thing*, Thing* target, const char*, bool, long priority, bool) { calls7++; lastTarget = target; lastPriority = priority; return 2; }
    long Speak6(Thing*, Thing* target, const char*, bool, long priority) { calls6++; lastTarget = target; lastPriority = priority; return 1; }
    long SayToThingReactingTo(const char* speech)
    {
        if (reactingTo && !(reactingTo->flags91 & 1) && (reactingTo->typeFlags & 8)) {
            Thing* target = reactingTo;
            if (allowNonPure) return Speak7(self, target, speech, false, 0x93, false);
            return Speak6(self, target, speech, false, 0x93);
        }
        return 3;
    }
};
int main()
{
    Thing me = { 0, 8 }, other = { 0, 8 }, rock = { 0, 0 }, dead = { 1, 8 };
    Group g = { &other, &me, false, 0, 0, 0, 0 };
    bool ok = g.SayToThingReactingTo("HELLO") == 1 && g.calls6 == 1 && g.lastTarget == &other && g.lastPriority == 0x93;
    g.allowNonPure = true;
    if (g.SayToThingReactingTo("HELLO") != 2 || g.calls7 != 1) ok = false;
    g.reactingTo = &rock;
    if (g.SayToThingReactingTo("HELLO") != 3) ok = false;
    g.reactingTo = &dead;
    if (g.SayToThingReactingTo("HELLO") != 3) ok = false;
    g.reactingTo = 0;
    if (g.SayToThingReactingTo("HELLO") != 3 || g.calls6 + g.calls7 != 2) ok = false;
    if (!ok) { std::printf("0095ee70_TEST FAIL\n"); return 1; }
    std::printf("0095ee70_TEST PASS\n");
    return 0;
}