// Behaviour test for NCreatureMode::CModeGuildmasterPowerUpSpeed::FrameUpdate (0x008E24F0, same body as the Shield mode; the boss is read before the beams): each frame the timer ticks; every
// live beam whose guard is alive is aimed from the midpoint of the two hand helpers to 1.75 above the guard, a live beam whose
// guard is gone is killed; once the timer is at or below zero, or no helper is alive, the release action starts.
#include <cstdio>
#include <cmath>

struct Vec { float x, y, z; };
struct Beam { bool alive; Vec start, end; int updates; };
struct Guard { bool alive; Vec pos; };
struct Mode {
    long Timer;
    Vec left, right;
    Beam beams[3];
    Guard guards[3];
    int count;
    int livingHelpers;
    int releases;
};

static void FrameUpdate(Mode& m)
{
    --m.Timer;
    Vec c = { (m.left.x + m.right.x) * 0.5f, (m.left.y + m.right.y) * 0.5f, (m.left.z + m.right.z) * 0.5f };
    for (int i = 0; i < m.count; ++i) {
        if (m.beams[i].alive) {
            if (m.guards[i].alive) {
                Vec e = m.guards[i].pos;
                e.z = m.guards[i].pos.z + 1.75f;
                m.beams[i].start = c; m.beams[i].end = e; ++m.beams[i].updates;
            } else {
                m.beams[i].alive = false;              // Kill(true)
            }
        }
    }
    if (m.Timer <= 0 || m.livingHelpers == 0)
        ++m.releases;
}

static bool Near(float a, float b) { return std::fabs(a - b) < 1e-4f; }

int main()
{
    Mode m;
    m.Timer = 2; m.count = 3; m.livingHelpers = 2; m.releases = 0;
    Vec l = { 0.0f, 0.0f, 2.0f }, r = { 2.0f, 4.0f, 2.0f };
    m.left = l; m.right = r;
    for (int i = 0; i < 3; ++i) {
        m.beams[i].alive = true; m.beams[i].updates = 0;
        m.guards[i].alive = true; Vec p = { (float)i, 1.0f, 0.5f }; m.guards[i].pos = p;
    }
    m.beams[1].alive = false;          // dead beam: untouched
    m.guards[2].alive = false;         // dead guard: beam killed
    FrameUpdate(m);
    bool ok = m.Timer == 1 && m.releases == 0 && m.beams[0].updates == 1 && Near(m.beams[0].start.x, 1.0f)
           && Near(m.beams[0].start.y, 2.0f) && Near(m.beams[0].end.z, 2.25f) && m.beams[1].updates == 0
           && !m.beams[2].alive && m.beams[2].updates == 0;
    FrameUpdate(m);                    // timer reaches 0: release
    ok = ok && m.Timer == 0 && m.releases == 1 && m.beams[0].updates == 2;
    Mode n = m; n.Timer = 10; n.livingHelpers = 0; n.releases = 0;
    FrameUpdate(n);                    // no living helpers: release
    ok = ok && n.releases == 1;
    if (!ok) { std::printf("008e24f0_TEST FAIL\n"); return 1; }
    std::printf("008e24f0_TEST PASS\n");
    return 0;
}