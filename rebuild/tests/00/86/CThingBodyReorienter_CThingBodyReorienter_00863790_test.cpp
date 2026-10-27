// Behaviour test for 0x00863790 CThingBodyReorienter::~CThingBodyReorienter(): calls ReorientBodyYZForSlopes first, then
// the member vector of intelligent pointers destroys each element and frees its block.
#include <cstdio>
#include <cstdlib>
#include <new>

static int g_log[8];
static int g_n = 0;

class IntelligentPointer {
public:
    int id;
    explicit IntelligentPointer(int i) : id(i) { }
    ~IntelligentPointer() { g_log[g_n++] = id; }
};

class PointerVector {
public:
    IntelligentPointer* start; IntelligentPointer* finish;
    PointerVector() : start(0), finish(0) { }
    void Fill(int n)
    {
        start = static_cast<IntelligentPointer*>(std::malloc(sizeof(IntelligentPointer) * n));
        finish = start;
        for (int i = 0; i < n; ++i) { new (static_cast<void*>(finish)) IntelligentPointer(10 + i); ++finish; }
    }
    ~PointerVector()
    {
        for (IntelligentPointer* p = start; p != finish; ++p) p->~IntelligentPointer();
        if (start) { std::free(start); g_log[g_n++] = -1; }
    }
};

class BodyReorienter {
public:
    PointerVector PossibleIntersectingThings;
    void ReorientBodyYZForSlopes() { g_log[g_n++] = 0; }
    ~BodyReorienter() { ReorientBodyYZForSlopes(); }
};

int main()
{
    {
        BodyReorienter r;
        r.PossibleIntersectingThings.Fill(2);
    }
    if (g_n != 4 || g_log[0] != 0 || g_log[1] != 10 || g_log[2] != 11 || g_log[3] != -1) { std::printf("00863790_TEST FAIL\n"); return 1; }
    g_n = 0;
    { BodyReorienter empty; }
    if (g_n != 1 || g_log[0] != 0) { std::printf("00863790_TEST FAIL\n"); return 1; }
    std::printf("00863790_TEST PASS\n");
    return 0;
}