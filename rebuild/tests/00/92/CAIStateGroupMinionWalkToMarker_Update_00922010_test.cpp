// Behaviour test for CAIStateGroup_MinionWalkToMarker::Update (0x00922010): a copy of its control flow over a small model --
// nearest marker first, then random (group 8) / nearest-not-last-two (group 9) picks, walking while the move is in progress,
// stopping when the group changes or the state group is stopping.
#include <cstdio>
#include <vector>

static unsigned long GFROR13(unsigned long v) { return (v >> 13) | (v << 19); }
static unsigned long GFRandom(unsigned long range, unsigned long* seed)
{
    *seed = *seed * 0x24a1 + 0x24df;
    *seed = GFROR13(*seed);
    return range ? *seed % range : 0;
}

struct Marker { int id; float x; };
struct Model {
    std::vector<Marker> markers;
    float pos;
    int group;
    unsigned long seed;
    int moveCalls;
    int stopAfterWalks;
    int walks;
    std::vector<int> visited;
    bool terminated;
    const Marker* Nearest(const Marker* notA, const Marker* notB) const
    {
        const Marker* best = 0;
        for (size_t i = 0; i < markers.size(); ++i) {
            const Marker* m = &markers[i];
            if (m == notA || m == notB) continue;
            float d = m->x - pos; if (d < 0) d = -d;
            if (!best) { best = m; continue; }
            float bd = best->x - pos; if (bd < 0) bd = -bd;
            if (d < bd) best = m;
        }
        return best;
    }
    int Move(const Marker* t)                    // two "in progress" steps then finished
    {
        ++moveCalls;
        if (moveCalls % 3 != 0) return 4;
        pos = t->x; visited.push_back(t->id); return 0;
    }
    bool IsStopping() { return ++walks >= stopAfterWalks; }
    void Update()
    {
        const Marker* target = Nearest(0, 0);
        const Marker* previous = 0;
        const Marker* beforePrevious = 0;
        while (group == 8 || group == 9) {
            int g = group;
            if (g == 8) {
                std::vector<const Marker*> things;
                for (size_t i = 0; i < markers.size(); ++i) things.push_back(&markers[i]);
                target = things[GFRandom((unsigned long)things.size(), &seed)];
            } else {
                target = Nearest(previous, beforePrevious);
            }
            while ((group == 8 || group == 9) && Move(target) == 4)
                ;
            if (IsStopping()) break;
            beforePrevious = previous;
            previous = target;
        }
        terminated = true;
    }
};

int main()
{
    Model m;
    Marker ms[4] = { {0, 0.0f}, {1, 10.0f}, {2, 20.0f}, {3, 30.0f} };
    m.markers.assign(ms, ms + 4);
    m.pos = 1.0f; m.group = 9; m.seed = 12345; m.moveCalls = 0; m.stopAfterWalks = 3; m.walks = 0; m.terminated = false;
    m.Update();
    // group 9 from pos 1: nearest is 0 (visit 0), then not {0}: 1, then not {1,0}: 2
    bool ok = m.terminated && m.visited.size() == 3 && m.visited[0] == 0 && m.visited[1] == 1 && m.visited[2] == 2 && m.moveCalls == 9;
    Model r = m;
    r.visited.clear(); r.group = 8; r.seed = 7; r.moveCalls = 0; r.walks = 0; r.stopAfterWalks = 2; r.terminated = false;
    unsigned long s = 7;
    int e1 = (int)GFRandom(4, &s); int e2 = (int)GFRandom(4, &s);
    r.Update();
    ok = ok && r.terminated && r.visited.size() == 2 && r.visited[0] == e1 && r.visited[1] == e2;
    Model z = m;
    z.group = 0; z.terminated = false; z.visited.clear(); z.moveCalls = 0;
    z.Update();
    ok = ok && z.terminated && z.moveCalls == 0;
    if (!ok) { std::printf("00922010_TEST FAIL\n"); return 1; }
    std::printf("00922010_TEST PASS\n");
    return 0;
}