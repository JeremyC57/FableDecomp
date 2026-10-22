// Behaviour test for CCreatureAction_JackScream::PerformAction (0x008C2240): the search box is +-20 around the Jack;
// a live marker gives the target, otherwise the ground below (X-2, Y-2); either way raised by 1.4. The sword leaves the
// primary slot unfixed, is placed at the dummy, and the mode gets the flag, the orientation and the target.
#include <cstdio>

struct Vec { float X, Y, Z; };
struct Thing { Vec pos; bool dead; bool fixed; Vec placed; };
struct Mode { bool dropping; Vec target; float orient; };

static long g_box[4];
static Thing* g_found;
static float g_ground = 5.0f;
static Vec g_groundQuery;

static void PerformAction(Thing* jack, Thing*& slot, Mode* mode, Vec dummy)
{
    Thing* marker = 0;
    long x = (long)jack->pos.X, y = (long)jack->pos.Y;
    g_box[0] = x - 20; g_box[1] = y - 20; g_box[2] = x + 20; g_box[3] = y + 20;
    marker = g_found;
    Vec target;
    if (marker && !marker->dead)
        target = marker->pos;
    else {
        target.X = jack->pos.X - 2.0f; target.Y = jack->pos.Y - 2.0f; target.Z = jack->pos.Z;
        g_groundQuery = target;
        target.Z = g_ground;
    }
    target.Z += 1.4f;
    Thing* sword = slot;
    sword->fixed = false;
    slot = 0;
    sword->placed = dummy;
    mode->dropping = true;
    mode->orient = 1.0f;
    mode->target = target;
}

static bool Near(float a, float b) { float d = a - b; return d < 0.001f && d > -0.001f; }

int main()
{
    Thing jack; jack.pos.X = 10.5f; jack.pos.Y = -3.5f; jack.pos.Z = 1.0f;
    Thing sword; sword.fixed = true;
    Thing* slot = &sword;
    Mode mode; mode.dropping = false;
    Vec dummy; dummy.X = 1; dummy.Y = 2; dummy.Z = 3;
    Thing marker; marker.pos.X = 7; marker.pos.Y = 8; marker.pos.Z = 9; marker.dead = false;
    g_found = &marker;
    PerformAction(&jack, slot, &mode, dummy);
    if (g_box[0] != -10 || g_box[1] != -23 || g_box[2] != 30 || g_box[3] != 17 || !Near(mode.target.Z, 10.4f)
        || slot != 0 || sword.fixed || !mode.dropping || !Near(sword.placed.Y, 2.0f)) {
        std::printf("008c2240_TEST FAIL\n"); return 1;
    }
    marker.dead = true; slot = &sword;
    PerformAction(&jack, slot, &mode, dummy);
    if (!Near(mode.target.X, 8.5f) || !Near(mode.target.Y, -5.5f) || !Near(mode.target.Z, 6.4f) || !Near(g_groundQuery.Z, 1.0f)) {
        std::printf("008c2240_TEST FAIL\n"); return 1;
    }
    std::printf("008c2240_TEST PASS\n");
    return 0;
}