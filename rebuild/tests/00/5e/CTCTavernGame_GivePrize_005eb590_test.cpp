// Test for 0x005EB590 CTCTavernGame::GivePrize: self-contained behaviour model -- when the hero has a receive-items component, a
// prize object of the def's Prize type is created at the table's position for the main player (Up (0,0,1), Forward (0,1,0), no
// script name) and given to the hero, the hero gains the def's PrizeRenown; when the hero also keeps an opinion deed log, every AI
// creature overlapping the table owner's bounding circle gets a TAVERN_GAME_WIN deed (0x4C) posted at the hero's position. The
// prize is marked given on every path, even when the hero cannot receive items.
#include <cstdio>
#include <vector>

struct Vec { float x, y, z; };
struct Def { long Prize; long PrizeRenown; };
struct Creature { int id; Vec pos; };
struct Hero { bool has_receive_items; bool has_deed_log; Vec pos; std::vector<int> items; long renown; };
struct Deed { int type; Vec pos; int creature; int post_type; };

static std::vector<Deed> g_deeds;
static int g_created_type, g_created_player;
static Vec g_created_pos, g_created_up, g_created_forward;

static int CreateObject(long type, const Vec& pos, long player, const Vec& up, const Vec& forward)
{
    g_created_type = (int)type; g_created_pos = pos; g_created_player = (int)player; g_created_up = up; g_created_forward = forward;
    return 1000 + (int)type;
}

struct TavernGame {
    Def def; Hero* hero; long main_player; Vec table_pos; Vec owner_centre; float owner_radius;
    std::vector<Creature> creatures; bool HasPrizeBeenGiven;

    void GivePrize()
    {
        if (hero->has_receive_items) {
            long player = main_player;
            Vec position = table_pos;
            Vec up = { 0.0f, 0.0f, 1.0f }, forward = { 0.0f, 1.0f, 0.0f };
            int prize = CreateObject(def.Prize, position, player, up, forward);
            hero->items.push_back(prize);
            hero->renown += def.PrizeRenown;
            if (hero->has_deed_log) {
                for (unsigned int i = 0; i < creatures.size(); ++i) {
                    float dx = creatures[i].pos.x - owner_centre.x, dy = creatures[i].pos.y - owner_centre.y;
                    if (dx * dx + dy * dy <= owner_radius * owner_radius) {
                        Deed d = { 0x4C, hero->pos, creatures[i].id, 1 };
                        g_deeds.push_back(d);
                    }
                }
            }
        }
        HasPrizeBeenGiven = true;
    }
};

static int fail(int n) { std::printf("005eb590_TEST FAIL %d\n", n); return 1; }

int main()
{
    Hero hero = { true, true, { 5.0f, 6.0f, 0.0f } };
    hero.renown = 10;
    TavernGame game;
    game.def.Prize = 42; game.def.PrizeRenown = 25;
    game.hero = &hero; game.main_player = 3;
    Vec table = { 1.0f, 2.0f, 3.0f }; game.table_pos = table;
    Vec centre = { 0.0f, 0.0f, 0.0f }; game.owner_centre = centre; game.owner_radius = 10.0f;
    Creature near1 = { 7, { 3.0f, 4.0f, 0.0f } }, far1 = { 8, { 30.0f, 0.0f, 0.0f } }, near2 = { 9, { -1.0f, 1.0f, 0.0f } };
    game.creatures.push_back(near1); game.creatures.push_back(far1); game.creatures.push_back(near2);
    game.HasPrizeBeenGiven = false;

    game.GivePrize();
    if (!game.HasPrizeBeenGiven) return fail(1);
    if (g_created_type != 42 || g_created_player != 3 || g_created_pos.x != 1.0f || g_created_pos.z != 3.0f) return fail(2);
    if (g_created_up.z != 1.0f || g_created_forward.y != 1.0f || g_created_up.y != 0.0f) return fail(3);
    if (hero.items.size() != 1 || hero.items[0] != 1042 || hero.renown != 35) return fail(4);
    if (g_deeds.size() != 2 || g_deeds[0].creature != 7 || g_deeds[1].creature != 9) return fail(5);
    if (g_deeds[0].type != 0x4C || g_deeds[0].post_type != 1 || g_deeds[0].pos.x != 5.0f || g_deeds[0].pos.y != 6.0f) return fail(6);

    // no deed log: the prize and renown are still given, no deed is posted
    g_deeds.clear(); hero.has_deed_log = false; game.HasPrizeBeenGiven = false;
    game.GivePrize();
    if (!game.HasPrizeBeenGiven || hero.items.size() != 2 || hero.renown != 60 || !g_deeds.empty()) return fail(7);

    // no receive-items component: nothing is created, but the prize still counts as given
    hero.has_receive_items = false; hero.has_deed_log = true; game.HasPrizeBeenGiven = false; g_created_type = 0;
    game.GivePrize();
    if (!game.HasPrizeBeenGiven || g_created_type != 0 || hero.items.size() != 2 || hero.renown != 60 || !g_deeds.empty())
        return fail(8);
    std::printf("005eb590_TEST PASS\n");
    return 0;
}