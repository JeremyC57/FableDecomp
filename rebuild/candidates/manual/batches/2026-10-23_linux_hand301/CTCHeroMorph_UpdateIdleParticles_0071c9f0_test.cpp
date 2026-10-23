// Behaviour test for CTCHeroMorph::UpdateIdleParticles (0071c9f0): pruning dead emitters, idle-frame latch, delay gate, spawn-once and kill-on-move
#include <cstdio>

struct Morph { int emitters[8]; int count; long frameIdle; bool added; int spawns; int kills; };

static const long kDelay = 30;

static void Update(Morph& m, long mode, float speedSq, long frame, int toSpawn)
{
    int w = 0;
    for (int i = 0; i < m.count; ++i)
        if (m.emitters[i]) m.emitters[w++] = m.emitters[i];
    m.count = w;
    if (mode == 1 && !(speedSq > 0.0001f * 0.0001f)) {
        if (!m.frameIdle) {
            m.frameIdle = frame;
            m.added = false;
            return;
        }
        if (frame - m.frameIdle >= kDelay && !m.added) {
            for (int i = 0; i < toSpawn; ++i)
                m.emitters[m.count++] = 100 + i;
            ++m.spawns;
            m.added = true;
        }
    } else if (m.added) {
        m.frameIdle = 0;
        m.added = false;
        ++m.kills;
        m.count = 0;
    }
}

int main()
{
    int fails = 0;
    Morph m = { { 5, 0, 7 }, 3, 0, false, 0, 0 };
    Update(m, 1, 0.0f, 10, 2);
    if (m.count != 2 || m.frameIdle != 10 || m.spawns) ++fails;
    Update(m, 1, 0.0f, 39, 2);
    if (m.spawns) ++fails;
    Update(m, 1, 0.0f, 40, 2);
    if (m.spawns != 1 || !m.added || m.count != 4) ++fails;
    Update(m, 1, 0.0f, 80, 2);
    if (m.spawns != 1) ++fails;
    Update(m, 1, 1.0f, 81, 2);
    if (m.kills != 1 || m.added || m.frameIdle) ++fails;
    Update(m, 2, 0.0f, 82, 2);
    if (m.kills != 1 || m.frameIdle) ++fails;
    if (fails) {
        printf("0071c9f0_TEST FAIL\n");
        return 1;
    }
    printf("0071c9f0_TEST PASS\n");
    return 0;
}
