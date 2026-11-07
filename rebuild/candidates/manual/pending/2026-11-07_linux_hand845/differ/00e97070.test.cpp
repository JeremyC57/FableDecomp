// Behaviour test for NScript::CV_HeroDollsScript::CBS_Teacher::Main (0x00E97070): each frame, with all dolls the teacher offers
// the swap (yes removes the dolls, gives the Jack of Blades doll and ends; no ends the loop); before the kid intro, carrying any
// doll triggers the doll kid's intro and the quest card; otherwise a newly received doll marks the hero as carrying dolls.
#include <cstdio>

struct Teacher {
    bool Ended, KidIntroDone, HasDolls, Swapped, CardGiven; int Dolls, AllDolls;
    bool Frame(bool answer_yes, bool received)
    {
        if (Dolls == AllDolls) {
            if (answer_yes) { Dolls = 0; Swapped = true; Ended = true; return true; }
            return false;
        } else if (!KidIntroDone && HasDolls) {
            CardGiven = true; KidIntroDone = true;
        } else if (received && Dolls > 0) HasDolls = true;
        return true;
    }
};

int main()
{
    Teacher t = { false, false, false, false, false, 0, 3 };
    bool ok = true;
    ok = ok && t.Frame(false, false) && !t.HasDolls;
    t.Dolls = 1; ok = ok && t.Frame(false, true) && t.HasDolls && !t.CardGiven;
    ok = ok && t.Frame(false, false) && t.CardGiven && t.KidIntroDone;
    t.Dolls = 3; ok = ok && !t.Frame(false, false) && !t.Swapped;
    ok = ok && t.Frame(true, false) && t.Swapped && t.Ended && t.Dolls == 0;
    if (!ok) { std::printf("00e97070_TEST FAIL\n"); return 1; }
    std::printf("00e97070_TEST PASS\n");
    return 0;
}
