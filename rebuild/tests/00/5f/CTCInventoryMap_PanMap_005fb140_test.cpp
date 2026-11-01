// Behaviour test for CTCInventoryMap::PanMap (0x005FB140): a copy of the region pick over a std::map<long, world-map offset>, checking
// the no-move early out, the 128-unit radius, the nearest pick (first region wins until a nearer one) and the reselect.
#include <cstdio>
#include <cmath>
#include <map>

struct V { float X, Y; };
struct State { int panned; long selected; int selects; };

static void PanMap(State& st, const std::map<long, V>& regions, V delta, V mouse, V centre, V zoom)
{
    ++st.panned;
    if (std::sqrt(delta.X * delta.X + delta.Y * delta.Y) < 0.01)
        return;
    V pos; pos.X = (mouse.X - centre.X) * -1.0f; pos.Y = (mouse.Y - centre.Y) * -1.0f;
    pos.X = pos.X / zoom.X; pos.Y = pos.Y / zoom.Y;
    if (regions.size() == 0)
        return;
    bool first = true;
    float best = 0.0f;
    std::map<long, V>::const_iterator bestIt;
    for (std::map<long, V>::const_iterator it = regions.begin(); it != regions.end(); ++it) {
        float dx = it->second.X - pos.X, dy = it->second.Y - pos.Y;
        float dist = (float)std::sqrt(dx * dx + dy * dy);
        if ((dist < best || first) && dist < 128.0f) {
            best = dist;
            first = false;
            bestIt = it;
        }
    }
    if (first == true)
        return;
    long key = bestIt->first;
    if (key == st.selected)
        return;
    st.selected = key;
    ++st.selects;
}

int main()
{
    int fails = 0;
    std::map<long, V> regions;
    V a = { 0.0f, 0.0f }; regions[0] = a;
    V b = { 100.0f, 0.0f }; regions[1] = b;
    V c = { 500.0f, 500.0f }; regions[2] = c;
    V move = { 5.0f, 0.0f }, still = { 0.0f, 0.0f }, zoom = { 1.0f, 1.0f }, centre = { 0.0f, 0.0f };

    State s0 = { 0, 0, 0 };
    V m0 = { -90.0f, 0.0f };   // pos = (90, 0): region 1 nearest
    PanMap(s0, regions, move, m0, centre, zoom);
    if (s0.selected != 1 || s0.selects != 1) ++fails;

    State s1 = { 0, 0, 0 };
    PanMap(s1, regions, still, m0, centre, zoom);
    if (s1.panned != 1 || s1.selects != 0) ++fails;

    State s2 = { 0, 2, 0 };
    V m2 = { -300.0f, -300.0f };   // pos = (300, 300): nothing within 128
    PanMap(s2, regions, move, m2, centre, zoom);
    if (s2.selected != 2 || s2.selects != 0) ++fails;

    State s3 = { 0, 0, 0 };
    V m3 = { -20.0f, 0.0f };   // pos = (20, 0): region 0, already selected
    PanMap(s3, regions, move, m3, centre, zoom);
    if (s3.selects != 0) ++fails;

    State s4 = { 0, 0, 0 };
    V z4 = { 2.0f, 2.0f };
    V m4 = { -180.0f, 0.0f };  // pos = (90, 0) after the zoom divide
    PanMap(s4, regions, move, m4, centre, z4);
    if (s4.selected != 1) ++fails;

    if (fails == 0) std::printf("005fb140_TEST PASS\n");
    else std::printf("005fb140_TEST FAIL %d\n", fails);
    return fails;
}