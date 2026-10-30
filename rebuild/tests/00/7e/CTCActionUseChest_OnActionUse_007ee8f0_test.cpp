// Behaviour test for 0x007EE8F0 CTCActionUseChest::OnActionUse (see src/007ee8f0.cpp): the base use runs; a shut, idle chest
// starts the open-chest action on the user; a theft by a user with a deed log posts a theft deed (type 6) at the user's position
// naming the chest thing, and a user with hero stats gets the theft morality change added to its morality.
#include <cstdio>
struct Thing { int id; };
struct User { bool HasLog, HasStats; int Deeds, LastDeedType; Thing* LastTarget; float Morality; int Actions; };
struct Chest { bool Opening, Closing; };
struct UseChest {
    Thing* ParentThing; Chest* TheChest; bool Theft; float TheftMorality; int BaseUses;
    void OnActionUse(User* user)
    {
        ++BaseUses;
        if (!TheChest->Opening && !TheChest->Closing) ++user->Actions;
        if (Theft && user->HasLog) {
            ++user->Deeds; user->LastDeedType = 6; user->LastTarget = ParentThing;
            if (user->HasStats) user->Morality = TheftMorality + user->Morality;
        }
    }
};
int main()
{
    Thing chestThing = { 7 }; Chest chest = { false, false };
    UseChest use = { &chestThing, &chest, true, -2.0f, 0 };
    User u = { true, true, 0, 0, 0, 10.0f, 0 };
    use.OnActionUse(&u);
    if (use.BaseUses != 1 || u.Actions != 1 || u.Deeds != 1 || u.LastDeedType != 6 || u.LastTarget != &chestThing || u.Morality != 8.0f) { std::printf("007ee8f0_TEST FAIL\n"); return 1; }
    chest.Opening = true; use.Theft = false;
    use.OnActionUse(&u);
    if (u.Actions != 1 || u.Deeds != 1) { std::printf("007ee8f0_TEST FAIL\n"); return 1; }
    std::printf("007ee8f0_TEST PASS\n");
    return 0;
}