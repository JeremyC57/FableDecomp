// Behaviour test for __uninitialized_fill_n<CNavigationLayerUpdate> (0x0051BBA0): n deep copies of one record (two
// vectors each), returning the end -- through a copy of the model.
#include <cstdio>
#include <cstdlib>
#include <new>

struct Line { float x1, y1, x2, y2; };
struct Blockage { unsigned int lo, hi; int list; };

struct LineVector {
    Line* start; Line* finish; Line* eos;
    LineVector(const LineVector& x)
    {
        unsigned int n = (unsigned int)(x.finish - x.start);
        start = finish = eos = 0;
        start = n != 0 ? (Line*)std::malloc(n * sizeof(Line)) : 0;
        finish = start; eos = start + n;
        Line* cur = start;
        for (const Line* p = x.start; p != x.finish; ++p, ++cur) new (cur) Line(*p);
        finish = cur;
    }
    LineVector() : start(0), finish(0), eos(0) {}
};
struct BlockageVector {
    Blockage* start; Blockage* finish; Blockage* eos;
    BlockageVector(const BlockageVector& x)
    {
        unsigned int n = (unsigned int)(x.finish - x.start);
        start = finish = eos = 0;
        start = n != 0 ? (Blockage*)std::malloc(n * sizeof(Blockage)) : 0;
        finish = start; eos = start + n;
        Blockage* cur = start;
        for (const Blockage* p = x.start; p != x.finish; ++p, ++cur) new (cur) Blockage(*p);
        finish = cur;
    }
    BlockageVector() : start(0), finish(0), eos(0) {}
};
struct LayerUpdate {
    LineVector LineList;
    BlockageVector SwitchableBlockages;
};

static LayerUpdate* UninitFillN(LayerUpdate* first, unsigned int n, const LayerUpdate& x)
{
    LayerUpdate* cur = first;
    for (; n > 0; --n, ++cur) new (cur) LayerUpdate(x);
    return cur;
}

int main()
{
    LayerUpdate proto;
    proto.LineList.start = (Line*)std::malloc(2 * sizeof(Line));
    proto.LineList.start[0].x1 = 1.0f; proto.LineList.start[0].y2 = 4.0f;
    proto.LineList.start[1].x1 = 5.0f; proto.LineList.start[1].y2 = 8.0f;
    proto.LineList.finish = proto.LineList.eos = proto.LineList.start + 2;
    proto.SwitchableBlockages.start = (Blockage*)std::malloc(sizeof(Blockage));
    proto.SwitchableBlockages.start[0].lo = 0x1234; proto.SwitchableBlockages.start[0].hi = 7;
    proto.SwitchableBlockages.finish = proto.SwitchableBlockages.eos = proto.SwitchableBlockages.start + 1;

    LayerUpdate* block = (LayerUpdate*)std::malloc(3 * sizeof(LayerUpdate));
    LayerUpdate* end = UninitFillN(block, 3, proto);
    if (end != block + 3) { std::printf("0051bba0_TEST FAIL\n"); return 1; }
    for (int i = 0; i < 3; ++i) {
        const LayerUpdate& u = block[i];
        if (u.LineList.finish - u.LineList.start != 2 || u.LineList.start == proto.LineList.start) { std::printf("0051bba0_TEST FAIL\n"); return 1; }
        if (u.LineList.start[1].x1 != 5.0f || u.LineList.start[0].y2 != 4.0f) { std::printf("0051bba0_TEST FAIL\n"); return 1; }
        if (u.SwitchableBlockages.finish - u.SwitchableBlockages.start != 1 || u.SwitchableBlockages.start[0].lo != 0x1234) { std::printf("0051bba0_TEST FAIL\n"); return 1; }
    }
    if (UninitFillN(block, 0, proto) != block) { std::printf("0051bba0_TEST FAIL\n"); return 1; }
    std::printf("0051bba0_TEST PASS\n");
    return 0;
}