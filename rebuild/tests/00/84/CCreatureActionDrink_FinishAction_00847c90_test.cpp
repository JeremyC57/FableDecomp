// Behaviour test for 0x00847C90 CCreatureAction_Drink::FinishAction (see src/00847c90.cpp): with a live drink prop the prop
// is killed and the creature's primary slot is emptied; a live jug carried by another creature only reaches the base
// FinishAction, a jug carried by this creature is removed then put back into its carry slot, an uncarried jug is just put
// in the slot; the base FinishAction always runs exactly once.
#include <cstdio>
struct Thing { bool Alive; int Kills; Thing* Carrier; long Slot; };
struct Carrying { int PrimaryRemoved, Removed, Put; Thing* LastPut; long LastSlot; };
struct Action {
    Thing* Creature; Thing* Drink; Thing* Jug; Carrying* Carry; int BaseCalls;
    void FinishAction()
    {
        if (Drink && Drink->Alive) {
            ++Drink->Kills;
            Carrying* carrying = Carry;
            ++carrying->PrimaryRemoved;
            if (Jug && Jug->Alive) {
                if (Jug->Carrier) {
                    if (Jug->Carrier == Creature)
                        ++carrying->Removed;
                    else
                        goto done;
                }
                ++carrying->Put; carrying->LastPut = Jug; carrying->LastSlot = Jug->Slot;
            }
        }
    done:
        ++BaseCalls;
    }
};
int main()
{
    Thing me = { true, 0, 0, 0 }, other = { true, 0, 0, 0 };
    Thing drink = { true, 0, 0, 0 }, jug = { true, 0, &other, 3 };
    Carrying c = { 0, 0, 0, 0, 0 };
    Action a = { &me, &drink, &jug, &c, 0 };
    a.FinishAction();
    bool ok = drink.Kills == 1 && c.PrimaryRemoved == 1 && c.Put == 0 && c.Removed == 0 && a.BaseCalls == 1;
    jug.Carrier = &me;
    a.FinishAction();
    ok = ok && c.Removed == 1 && c.Put == 1 && c.LastSlot == 3 && a.BaseCalls == 2;
    jug.Carrier = 0;
    a.FinishAction();
    ok = ok && c.Removed == 1 && c.Put == 2 && a.BaseCalls == 3;
    drink.Alive = false;
    a.FinishAction();
    ok = ok && drink.Kills == 3 && a.BaseCalls == 4;
    std::printf(ok ? "00847c90_TEST PASS\n" : "00847c90_TEST FAIL\n");
    return ok ? 0 : 1;
}