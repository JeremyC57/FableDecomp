// 00ac8540 behaviour test: COptimisedPrimitive::PaintSegments (final.cpp body over standard containers)
#include <stdio.h>
#include <vector>
#include <list>
static int g_fail = 0;
#define CHECK(c) do { if (!(c)) { printf("FAIL line %d: %s\n", __LINE__, #c); g_fail = 1; } } while (0)
struct COptimisedFace { bool Exists; long Segment; int Neighbour; };
static int g_lists;
struct CIndexWorkList { std::list<unsigned long> L; CIndexWorkList() { ++g_lists; } };
class COptimisedPrimitive {
public:
    std::vector<COptimisedFace> Faces;
    long SegmentCount;
    void PaintSegment(unsigned long face, long segment, CIndexWorkList& work)
    {   // flood fill along the Neighbour chain
        work.L.push_back(face);
        while (!work.L.empty()) {
            unsigned long f = work.L.front(); work.L.pop_front();
            if (!Faces[f].Exists || Faces[f].Segment != -1) continue;
            Faces[f].Segment = segment;
            if (Faces[f].Neighbour >= 0) work.L.push_back(Faces[f].Neighbour);
        }
    }
    void PaintSegments();
};
void COptimisedPrimitive::PaintSegments()
{
    long faceCount = Faces.size();
    SegmentCount = 0;
    for (long i = 0; i < faceCount; ++i)
    {
        COptimisedFace& face = Faces[i];
        if (face.Exists && face.Segment == -1)
        {
            CIndexWorkList work;
            PaintSegment(i, SegmentCount, work);
            SegmentCount++;
        }
    }
}
int main()
{
    COptimisedPrimitive p;
    COptimisedFace f[6] = { { true, -1, 2 }, { true, -1, -1 }, { true, -1, -1 }, { false, -1, -1 }, { true, -1, 1 }, { true, 7, -1 } };
    for (int i = 0; i < 6; ++i) p.Faces.push_back(f[i]);
    p.PaintSegments();
    CHECK(p.SegmentCount == 3 && g_lists == 3);
    CHECK(p.Faces[0].Segment == 0 && p.Faces[2].Segment == 0 && p.Faces[1].Segment == 1);
    CHECK(p.Faces[3].Segment == -1 && p.Faces[4].Segment == 2 && p.Faces[5].Segment == 7);
    if (!g_fail) printf("00ac8540_TEST PASS\n");
    return g_fail;
}