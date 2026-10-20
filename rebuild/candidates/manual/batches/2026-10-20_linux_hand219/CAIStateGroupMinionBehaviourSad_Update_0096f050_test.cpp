// behaviour test for CAIStateGroup_MinionBehaviourSad::Update (0x0096F050): a copy of the logic over plain stubs
#include <cstdio>
struct Thing { unsigned char flags91; };
struct Group {
    Thing* sad; bool knelt; long moveResult; long lookResult;
    int moves, looks, terminates, anims, emotes;
    float lastDistance, lastLookSpeed;
    long MoveToThingProximity(Thing*, float d, bool* reached) { moves++; lastDistance = d; *reached = true; return moveResult; }
    long LookAtThing(Thing*, long, float s) { looks++; lastLookSpeed = s; return lookResult; }
    void Terminate() { terminates++; }
    void PlayKneel() { anims++; }
    void UpdateEmote() { emotes++; }
    void Update()
    {
        if (sad && !(sad->flags91 & 1)) {
            bool reached;
            if (MoveToThingProximity(sad, 1.5f, &reached) != 2) { Terminate(); return; }
            if (LookAtThing(sad, 0x11, 0.0027777f) != 2) { Terminate(); return; }
            if (!knelt) { knelt = true; PlayKneel(); }
            UpdateEmote();
        }
    }
};
int main()
{
    Thing alive = { 0 }, dead = { 1 };
    Group g = { &alive, false, 2, 2, 0, 0, 0, 0, 0, 0.0f, 0.0f };
    bool ok = true;
    g.Update(); g.Update();
    if (!g.knelt || g.anims != 1 || g.emotes != 2 || g.terminates != 0 || g.lastDistance != 1.5f) ok = false;
    g.moveResult = 4; g.Update();
    if (g.terminates != 1 || g.looks != 2) ok = false;                  // approach not done: terminate, no look
    g.moveResult = 2; g.lookResult = 1; g.Update();
    if (g.terminates != 2 || g.emotes != 2) ok = false;                 // look failed: terminate
    g.sad = &dead; g.Update();
    if (g.moves != 4 || g.terminates != 2) ok = false;                  // dead target: nothing
    if (!ok) { std::printf("0096f050_TEST FAIL\n"); return 1; }
    std::printf("0096f050_TEST PASS\n");
    return 0;
}
