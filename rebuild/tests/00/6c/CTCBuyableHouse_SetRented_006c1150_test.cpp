// Behaviour test for CTCBuyableHouse::SetRented @ 0x006c1150: the hero's rented-building count follows rent transitions, an owned house is evicted when no longer rented, the sign post is killed and rebuilt, the mini-map refreshes.
#include <stdio.h>
struct Thing { bool Dead; bool Killed; Thing() : Dead(false), Killed(false) {} void Kill(bool) { Killed = true; } };
struct Stats { long NumberOfBuildingsRented; };
struct Hero { bool Dead; bool HasStats; Stats S; };
static Hero* g_hero = 0;
static int g_minimap = 0;
static bool GetTC(Hero* h, Stats** out) { if (h->HasStats) { *out = &h->S; return true; } return false; }
struct House {
    bool OwnedByPlayer, Rented; Thing* MainSignPost; bool WantsSignPost;
    int Evicted, Created;
    House() : OwnedByPlayer(false), Rented(false), MainSignPost(0), WantsSignPost(false), Evicted(0), Created(0) {}
    void Evict() { Evicted++; }
    bool NeedsMainSignPost() { return WantsSignPost; }
    void CreateMainSignPost() { Created++; }
    void SetRented(bool rented)
    {
        Hero* hero = g_hero;
        if (hero && !hero->Dead) {
            Stats* stats = 0;
            GetTC(hero, &stats);
            if (rented == true) { if (!Rented) stats->NumberOfBuildingsRented++; }
            else if (rented == false) { if (Rented == true) stats->NumberOfBuildingsRented--; }
        }
        Rented = rented;
        if (!rented && OwnedByPlayer) Evict();
        if (MainSignPost && !MainSignPost->Dead) MainSignPost->Kill(true);
        if (NeedsMainSignPost()) CreateMainSignPost();
        g_minimap++;
    }
};
#define CHECK(c) do { if (!(c)) { printf("006c1150_TEST FAIL line %d\n", __LINE__); return 1; } } while (0)
int main()
{
    Hero hero; hero.Dead = false; hero.HasStats = true; hero.S.NumberOfBuildingsRented = 2; g_hero = &hero;
    House h; Thing post; h.MainSignPost = &post; h.WantsSignPost = true;
    h.SetRented(true);  CHECK(h.Rented && hero.S.NumberOfBuildingsRented == 3 && post.Killed && h.Created == 1 && g_minimap == 1);
    h.SetRented(true);  CHECK(hero.S.NumberOfBuildingsRented == 3);           // already rented: no double count
    h.SetRented(false); CHECK(!h.Rented && hero.S.NumberOfBuildingsRented == 2 && h.Evicted == 0);
    h.SetRented(false); CHECK(hero.S.NumberOfBuildingsRented == 2);
    h.OwnedByPlayer = true; h.SetRented(false); CHECK(h.Evicted == 1);        // owned house no longer rented: evict
    post.Killed = false; post.Dead = true; h.WantsSignPost = false; h.Created = 0;
    h.SetRented(true); CHECK(!post.Killed && h.Created == 0 && hero.S.NumberOfBuildingsRented == 3);
    hero.Dead = true; h.SetRented(false); CHECK(hero.S.NumberOfBuildingsRented == 3 && !h.Rented && h.Evicted == 2);
    g_hero = 0; h.SetRented(true); CHECK(h.Rented && g_minimap == 8);
    printf("006c1150_TEST PASS\n");
    return 0;
}