// Behaviour test for 0x007F01F0 CTCCarriedActionUseFishingRod::OnActionUse (see src/007f01f0.cpp): the base use runs first;
// a creature able to fish is given the "Expression_Fish" activate-quest action naming the rod's parent thing (resources not
// loaded); the rod's showable component (when present) is told the show ended either way.
#include <cstdio>
#include <cstring>
struct Thing { bool CanFish; const char* Script; Thing* Object; bool Load; int Actions; bool HasShowable; int ShowEnded; };
struct Rod {
    Thing* ParentThing; int BaseUses;
    void BaseOnActionUse(Thing&) { ++BaseUses; }
    void OnActionUse(Thing& user)
    {
        BaseOnActionUse(user);
        if (user.CanFish) { user.Script = "Expression_Fish"; user.Object = ParentThing; user.Load = false; ++user.Actions; }
        if (ParentThing->HasShowable) ++ParentThing->ShowEnded;
    }
};
int main()
{
    Thing rodThing = { false, 0, 0, true, 0, true, 0 };
    Thing fisher = { true, 0, 0, true, 0, false, 0 };
    Rod rod = { &rodThing, 0 };
    rod.OnActionUse(fisher);
    if (rod.BaseUses != 1 || fisher.Actions != 1 || std::strcmp(fisher.Script, "Expression_Fish") || fisher.Object != &rodThing || fisher.Load || rodThing.ShowEnded != 1) { std::printf("007f01f0_TEST FAIL\n"); return 1; }
    Thing other = { false, 0, 0, true, 0, false, 0 };
    rodThing.HasShowable = false;
    rod.OnActionUse(other);
    if (rod.BaseUses != 2 || other.Actions != 0 || rodThing.ShowEnded != 1) { std::printf("007f01f0_TEST FAIL\n"); return 1; }
    std::printf("007f01f0_TEST PASS\n");
    return 0;
}