// Behaviour test for 0x00913D60 std::deque<CBezierCurve>::_M_push_back_aux_v (STLport slow push_back: new 2-element buffer).
#include <cstdio>
#include <cstdlib>
#include <new>

struct C3DVector { float X, Y, Z; };
struct CBezierCurve { C3DVector Point1, Point2, Point3, Point4; };

static int g_reallocs = 0;
struct Iter { CBezierCurve* cur; CBezierCurve* first; CBezierCurve* last; CBezierCurve** node; };
struct Deque {
    Iter start, finish; CBezierCurve** map; unsigned int mapSize;
    void ReallocateMap(unsigned int, bool) { ++g_reallocs; }   // stand-in: the test map is big enough
    void PushBackAux(const CBezierCurve& t)
    {
        CBezierCurve copy = t;
        if (1 + 1 > mapSize - (unsigned int)(finish.node - map)) ReallocateMap(1, false);
        *(finish.node + 1) = (CBezierCurve*)std::malloc(2 * sizeof(CBezierCurve));
        new (finish.cur) CBezierCurve(copy);
        finish.node = finish.node + 1; finish.first = *finish.node; finish.last = finish.first + 2;
        finish.cur = finish.first;
    }
};

static CBezierCurve Curve(float f) { CBezierCurve c; c.Point1.X = f; c.Point4.Z = f * 2; return c; }

int main()
{
    CBezierCurve* nodes[8] = { 0 };
    Deque d; d.map = nodes; d.mapSize = 8;
    nodes[3] = (CBezierCurve*)std::malloc(2 * sizeof(CBezierCurve));
    d.start.node = d.finish.node = &nodes[3];
    d.start.first = d.finish.first = nodes[3];
    d.start.last = d.finish.last = nodes[3] + 2;
    d.start.cur = nodes[3];
    // one element already in the buffer, finish.cur at the buffer's last slot
    nodes[3][0] = Curve(1.0f);
    d.finish.cur = nodes[3] + 1;
    d.PushBackAux(Curve(2.0f));
    if (nodes[3][1].Point1.X != 2.0f || nodes[3][1].Point4.Z != 4.0f) { std::printf("00913d60_TEST FAIL\n"); return 1; }
    if (d.finish.node != &nodes[4] || d.finish.cur != nodes[4] || d.finish.first != nodes[4] || d.finish.last != nodes[4] + 2) { std::printf("00913d60_TEST FAIL\n"); return 1; }
    if (g_reallocs != 0) { std::printf("00913d60_TEST FAIL\n"); return 1; }
    // at the end of the map: the map is reallocated first
    CBezierCurve spare[2];
    CBezierCurve* bigmap[10] = { 0 };            // the stand-in realloc leaves the map as is: keep node + 1 in bounds
    for (int i = 0; i < 8; ++i) bigmap[i] = nodes[i];
    bigmap[7] = spare;
    d.map = bigmap; d.mapSize = 8; d.finish.node = &bigmap[7]; d.finish.cur = spare + 1;   // 2 > 8 - 7
    d.PushBackAux(Curve(3.0f));
    if (g_reallocs != 1 || spare[1].Point1.X != 3.0f || d.finish.node != &bigmap[8]) { std::printf("00913d60_TEST FAIL\n"); return 1; }
    std::free(nodes[3]); std::free(nodes[4]); std::free(bigmap[8]);
    std::printf("00913d60_TEST PASS\n");
    return 0;
}