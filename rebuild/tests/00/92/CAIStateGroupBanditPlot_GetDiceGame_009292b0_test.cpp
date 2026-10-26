// Behaviour test for 0x009292b0: GetDiceGame walks creature -> villager -> village -> village thing -> dice game (0xf1).
#include <cstdio>
class CThing { public: long type; void* comp; void* GetIface(long t) { return t == type ? comp : 0; } };
class CTCVillage { public: CThing* Owner; };
struct CTCVillager { CTCVillage* village; };
static CTCVillage* GetVillage(CTCVillager* v) { return v->village; }
static void* GetDiceGame(CThing* creature)
{
    CTCVillager* villager = (CTCVillager*)creature->GetIface(0x23);
    if (villager) {
        if (GetVillage(villager) != 0) {
            CThing* villageThing = GetVillage(villager)->Owner;
            void* game = villageThing->GetIface(0xf1);
            if (game) return game;
        }
    }
    return 0;
}
int main()
{
    int game = 0;
    CThing vthing = { 0xf1, &game };
    CTCVillage village = { &vthing };
    CTCVillager villager = { &village };
    CThing creature = { 0x23, &villager };
    int bad = 0;
    if (GetDiceGame(&creature) != &game) bad = 1;
    vthing.type = 0x11; if (GetDiceGame(&creature) != 0) bad = 2;
    vthing.type = 0xf1; villager.village = 0; if (GetDiceGame(&creature) != 0) bad = 3;
    creature.type = 0; villager.village = &village; if (GetDiceGame(&creature) != 0) bad = 4;
    if (bad) { std::printf("009292b0_TEST FAIL\n"); return 1; }
    std::printf("009292b0_TEST PASS\n");
    return 0;
}