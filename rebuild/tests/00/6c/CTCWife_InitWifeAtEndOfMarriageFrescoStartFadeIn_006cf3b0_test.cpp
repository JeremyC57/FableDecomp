// behaviour test for 0x006CF3B0 CTCWife::InitWifeAtEndOfMarriageFrescoStartFadeIn(): with a live hero, the wife is removed
// from the hero's followed list (TC 0xBC) and region-follower list (TC 0x7A) when she is on them and the hero has the TC.
#include <cstdio>
#include <vector>
#include <algorithm>
struct Thing { bool dead; bool hasFollowed; bool hasRegion; std::vector<const Thing*> followers, regionFollowers; };
static void Remove(std::vector<const Thing*>& v, const Thing* t) { v.erase(std::remove(v.begin(), v.end(), t), v.end()); }
static bool On(const std::vector<const Thing*>& v, const Thing* t) { return std::find(v.begin(), v.end(), t) != v.end(); }
static int removals;
static void StartFadeIn(Thing* hero, const Thing* wife)
{
    if (!hero || hero->dead) return;
    if (hero->hasFollowed && On(hero->followers, wife)) { Remove(hero->followers, wife); ++removals; }
    if (hero->hasRegion && On(hero->regionFollowers, wife)) { Remove(hero->regionFollowers, wife); ++removals; }
}
int main()
{
    Thing wife = Thing(), other = Thing();
    Thing hero = Thing(); hero.hasFollowed = hero.hasRegion = true;
    hero.followers.push_back(&other); hero.followers.push_back(&wife); hero.regionFollowers.push_back(&wife);
    StartFadeIn(0, &wife);
    bool ok = removals == 0;
    hero.dead = true; StartFadeIn(&hero, &wife); ok = ok && removals == 0 && On(hero.followers, &wife);
    hero.dead = false; hero.hasRegion = false; StartFadeIn(&hero, &wife);
    ok = ok && removals == 1 && !On(hero.followers, &wife) && On(hero.followers, &other) && On(hero.regionFollowers, &wife);
    hero.hasRegion = true; StartFadeIn(&hero, &wife); ok = ok && removals == 2 && hero.regionFollowers.empty();
    StartFadeIn(&hero, &wife); ok = ok && removals == 2;
    if (!ok) { std::printf("006cf3b0_TEST FAIL\n"); return 1; }
    std::printf("006cf3b0_TEST PASS\n");
    return 0;
}