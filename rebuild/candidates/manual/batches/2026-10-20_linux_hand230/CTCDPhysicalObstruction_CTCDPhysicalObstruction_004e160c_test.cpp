// Behaviour test for CTCDPhysicalObstruction::~CTCDPhysicalObstruction (0x004E160C): each primitive handle tells its
// primitive when it holds the last reference, then drops the reference; handles die line3, line2, line1.
#include <cstdio>

struct Prim {
    long RefCount;
    int notified;
    int destroyed;
    int order;
};
static int g_seq = 0;

struct Handle {
    Prim* Data;
    void Clear()
    {
        long count = Data ? Data->RefCount : 0;
        if (count == 1)
            Data->notified = ++g_seq;
        if (Data) {
            if (--Data->RefCount == 0)
                Data->destroyed = 1;
            Data = 0;
        }
        Data = 0;
    }
    ~Handle() { Clear(); }
};

struct Obstruction {
    Handle line1, line2, line3;
};

int main()
{
    Prim a = { 1, 0, 0, 0 };   // unique -> notified + destroyed
    Prim b = { 2, 0, 0, 0 };   // shared -> just decremented
    Prim c = { 1, 0, 0, 0 };
    {
        Obstruction* o = new Obstruction;
        o->line1.Data = &a;
        o->line2.Data = &b;
        o->line3.Data = &c;
        delete o;
    }
    if (a.notified != 2 || a.destroyed != 1 || a.RefCount != 0) { std::printf("004e160c_TEST FAIL\n"); return 1; }
    if (b.notified != 0 || b.destroyed != 0 || b.RefCount != 1) { std::printf("004e160c_TEST FAIL\n"); return 1; }
    if (c.notified != 1 || c.destroyed != 1) { std::printf("004e160c_TEST FAIL\n"); return 1; }
    {
        Handle h;
        h.Data = 0;           // null handle: nothing happens
    }
    std::printf("004e160c_TEST PASS\n");
    return 0;
}
