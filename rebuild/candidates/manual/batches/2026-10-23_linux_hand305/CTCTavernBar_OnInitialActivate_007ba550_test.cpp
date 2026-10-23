// Behaviour test for CTCTavernBar::OnInitialActivate (0x007BA550): one jug per free action point, each pushed into Jugs,
// placed at the point's position/orientation, flagged 2, given two TCs owned by the bar; the inline interface lookup is the
// STLport lower_bound over {type, component} pairs.
#include <cstdio>

struct SEntry { int type; int component; };

static int LowerBoundLookup(SEntry* first, SEntry* last, int type)
{
    int count = (int)(last - first);
    while (count > 0) {
        int half = count / 2;
        if (first[half].type < type) {
            first += half + 1;
            count -= half + 1;
        }
        else
            count = half;
    }
    return ((first == last || type < first->type) ? last : first)->component;
}

struct CJug {
    float posX, rhX;
    unsigned char flags91;
    int tcCount;
    int owner1, owner2;
};

struct CBar {
    int ParentId;
    int freePoints;
    CJug jugs[8];
    int numJugs;
    bool GetFreePoint(float* pos, float* rh)
    {
        if (freePoints == 0)
            return false;
        *pos = (float)freePoints * 10.0f;
        *rh = (float)freePoints;
        --freePoints;
        return true;
    }
    void OnInitialActivate()
    {
        float pos, rh;
        for (;;) {
            if (!GetFreePoint(&pos, &rh))
                break;
            CJug* jug = &jugs[numJugs++];
            jug->flags91 = 0;
            jug->tcCount = 0;
            jug->posX = pos;
            jug->rhX = rh;
            jug->flags91 |= 2;
            ++jug->tcCount;
            jug->owner1 = ParentId;
            ++jug->tcCount;
            jug->owner2 = ParentId;
        }
    }
};

int main()
{
    SEntry map[6] = { { 0x2e, 1 }, { 0x32, 2 }, { 0x33, 3 }, { 0xbb, 4 }, { 0x10c, 5 }, { 0x7fffffff, 0 } };
    int ok = LowerBoundLookup(map, map + 5, 0x33) == 3 && LowerBoundLookup(map, map + 5, 0xbb) == 4
             && LowerBoundLookup(map, map + 5, 0x2e) == 1 && LowerBoundLookup(map, map + 4, 0x10c) == 5
             && LowerBoundLookup(map, map + 5, 0x40) == 0;
    CBar bar;
    bar.ParentId = 7;
    bar.freePoints = 3;
    bar.numJugs = 0;
    bar.OnInitialActivate();
    ok = ok && bar.numJugs == 3 && bar.freePoints == 0;
    for (int i = 0; ok && i < 3; ++i) {
        CJug& j = bar.jugs[i];
        ok = j.flags91 == 2 && j.tcCount == 2 && j.owner1 == 7 && j.owner2 == 7 && j.posX == (float)(3 - i) * 10.0f
             && j.rhX == (float)(3 - i);
    }
    if (!ok) {
        std::printf("007ba550_TEST FAIL\n");
        return 1;
    }
    std::printf("007ba550_TEST PASS\n");
    return 0;
}
