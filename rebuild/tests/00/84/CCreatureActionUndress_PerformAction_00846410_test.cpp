// Behaviour test for CCreatureAction_Undress::PerformAction (0x00846410): of the worn type-2 modifiers, those whose
// appearance-modifier sub def is not removable are skipped; the rest are taken off last first, each dropped as a
// thing at the head position offset by (0.2 - r, 0.2 - r) in X/Y with velocity (dx, dy, 0.15).
#include <cstdio>

struct Vec { float X, Y, Z; };
struct Node { Node* next; Node* prev; long data; };
static Node g_nodes[8];

static bool g_removable[8];
static bool g_hasDef[8];
static long g_removed[8]; static int g_removedCount;
static long g_created[8]; static Vec g_pos[8]; static Vec g_vel[8]; static int g_createdCount;
static float g_rand = 0.1f;

static void PerformAction(const long* worn, int count, Vec head)
{
    Node sentinel; sentinel.next = &sentinel; sentinel.prev = &sentinel;
    for (int i = 0; i < count; ++i) {
        Node* n = &g_nodes[i]; n->data = worn[i];
        n->prev = sentinel.prev; n->next = &sentinel; sentinel.prev->next = n; sentinel.prev = n;
    }
    for (Node* it = sentinel.next; it != &sentinel; ) {
        long id = it->data;
        if (g_hasDef[id] && !g_removable[id]) {
            Node* next = it->next; it->prev->next = next; next->prev = it->prev; it = next;
        } else
            it = it->next;
    }
    while (sentinel.next != &sentinel) {
        Node* last = sentinel.prev;
        long item = last->data;
        g_removed[g_removedCount++] = item;
        Vec v; v.X = 0.2f - g_rand; v.Y = 0.2f - g_rand; v.Z = 0.15f;
        Vec p; p.X = v.X + head.X; p.Y = v.Y + head.Y; p.Z = head.Z;
        g_created[g_createdCount] = item; g_pos[g_createdCount] = p; g_vel[g_createdCount] = v; ++g_createdCount;
        last->prev->next = last->next; last->next->prev = last->prev;
    }
}

static bool Near(float a, float b) { float d = a - b; return d < 0.0001f && d > -0.0001f; }

int main()
{
    long worn[4] = { 1, 2, 3, 4 };
    g_hasDef[1] = g_hasDef[2] = g_hasDef[3] = true;
    g_removable[1] = true; g_removable[2] = false; g_removable[3] = true;   // 2 stays on; 4 has no def -> taken off
    Vec head; head.X = 1.0f; head.Y = 2.0f; head.Z = 3.0f;
    PerformAction(worn, 4, head);
    if (g_removedCount != 3 || g_removed[0] != 4 || g_removed[1] != 3 || g_removed[2] != 1) {
        std::printf("00846410_TEST FAIL\n"); return 1;
    }
    if (!Near(g_pos[0].X, 1.1f) || !Near(g_pos[0].Y, 2.1f) || !Near(g_pos[0].Z, 3.0f) || !Near(g_vel[0].Z, 0.15f)) {
        std::printf("00846410_TEST FAIL\n"); return 1;
    }
    std::printf("00846410_TEST PASS\n");
    return 0;
}