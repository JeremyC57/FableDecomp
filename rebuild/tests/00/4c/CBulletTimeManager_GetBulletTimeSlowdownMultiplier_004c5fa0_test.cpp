// Behaviour test for 0x004C5FA0 CBulletTimeManager::GetBulletTimeSlowdownMultiplier(): the default (global 4 as float) while
// UseDefaultMultiplier is set or the map is empty; otherwise the sum of the registered Multipliers in tree order.
#include <cstdio>

struct Node { bool color; Node* parent; Node* left; Node* right; void* key; float Multiplier; float SoundPitch; };
struct Manager { Node* header; unsigned count; unsigned cmp; bool Override; bool UseDefault; };
static long g_Default = 4;

static Node* Increment(Node* n)
{
    if (n->right) { n = n->right; while (n->left) n = n->left; }
    else { Node* y = n->parent; while (n == y->right) { n = y; y = y->parent; } if (n->right != y) n = y; }
    return n;
}

static float Get(const Manager& m)
{
    if (m.UseDefault || m.count == 0) return (float)g_Default;
    float total = 0.0f;
    for (Node* it = m.header->left; it != m.header; it = Increment(it)) total += it->Multiplier;
    return total;
}

int main()
{
    Node h = { false }, a = { false }, b = { false };
    h.parent = &a; h.left = &a; h.right = &b; a.parent = &h; a.right = &b; b.parent = &a;
    a.Multiplier = 0.25f; b.Multiplier = 0.5f;
    Manager m = { &h, 2, 0, false, false };
    if (Get(m) != 0.75f) { std::printf("004c5fa0_TEST FAIL\n"); return 1; }
    m.UseDefault = true;
    if (Get(m) != 4.0f) { std::printf("004c5fa0_TEST FAIL\n"); return 1; }
    m.UseDefault = false; m.count = 0;
    if (Get(m) != 4.0f) { std::printf("004c5fa0_TEST FAIL\n"); return 1; }
    std::printf("004c5fa0_TEST PASS\n");
    return 0;
}