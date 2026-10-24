// Behaviour test for 0x004C5F10 CBulletTimeManager::RecalculateSoundPitch(): no call without a sound system; otherwise the
// sound system gets (0, min(1.0, every entry's SoundPitch)) walking the map in order.
#include <cstdio>

struct Node { bool color; Node* parent; Node* left; Node* right; void* key; float Multiplier; float SoundPitch; };
static int g_calls = 0; static long g_cat = -1; static float g_pitch = -1.0f;
struct SoundSystem { void SetPitch(long c, float p) { ++g_calls; g_cat = c; g_pitch = p; } };
static SoundSystem* g_pSoundSystem = 0;

static Node* Increment(Node* n)
{
    if (n->right) { n = n->right; while (n->left) n = n->left; }
    else { Node* y = n->parent; while (n == y->right) { n = y; y = y->parent; } if (n->right != y) n = y; }
    return n;
}

static void Recalculate(Node* header)
{
    if (g_pSoundSystem) {
        float pitch = 1.0f;
        for (Node* it = header->left; it != header; it = Increment(it))
            if (it->SoundPitch < pitch) pitch = it->SoundPitch;
        g_pSoundSystem->SetPitch(0, pitch);
    }
}

int main()
{
    // three-node tree: root b, left a, right c; header links root / leftmost / rightmost
    Node h = { false }, a = { false }, b = { false }, c = { false };
    h.parent = &b; h.left = &a; h.right = &c;
    b.parent = &h; b.left = &a; b.right = &c; a.parent = &b; c.parent = &b;
    a.SoundPitch = 0.9f; b.SoundPitch = 0.5f; c.SoundPitch = 1.5f;
    Recalculate(&h);
    if (g_calls != 0) { std::printf("004c5f10_TEST FAIL\n"); return 1; }
    SoundSystem s; g_pSoundSystem = &s;
    Recalculate(&h);
    if (g_calls != 1 || g_cat != 0 || g_pitch != 0.5f) { std::printf("004c5f10_TEST FAIL\n"); return 1; }
    Node e = { false }; e.parent = 0; e.left = &e; e.right = &e;     // empty map: 1.0
    Recalculate(&e);
    if (g_calls != 2 || g_pitch != 1.0f) { std::printf("004c5f10_TEST FAIL\n"); return 1; }
    std::printf("004c5f10_TEST PASS\n");
    return 0;
}