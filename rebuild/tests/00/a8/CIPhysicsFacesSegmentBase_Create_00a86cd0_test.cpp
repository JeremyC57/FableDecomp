// Behaviour test for 0x00A86CD0 CIPhysicsFacesSegmentBase::Create: index every triangle, then a branch when Calculate
// finds a dividing plane, else a leaf holding all the indices
#include <cstdio>
#include <vector>

struct Segment { virtual ~Segment() { } virtual int Kind() const = 0; };
struct Leaf : Segment {
    std::vector<unsigned short> Faces;
    Leaf(const std::vector<unsigned short>& f) : Faces(f) { }
    int Kind() const { return 1; }
};
struct Branch : Segment {
    std::vector<unsigned short> Positive, Negative; long Depth;
    Branch(const std::vector<unsigned short>& p, const std::vector<unsigned short>& n, long d) : Positive(p), Negative(n), Depth(d) { }
    int Kind() const { return 2; }
};
static bool g_split = false;
static long g_seenDepth = -1;
static unsigned int g_seenFaces = 0;
static bool Calculate(const std::vector<unsigned short>& faces, long depth, std::vector<unsigned short>& positive,
                      std::vector<unsigned short>& negative)
{
    g_seenDepth = depth; g_seenFaces = (unsigned int)faces.size();
    if (!g_split) return false;
    for (unsigned int i = 0; i < faces.size(); ++i) (i & 1 ? negative : positive).push_back(faces[i]);
    return true;
}
static const Segment* Create(unsigned int triangleCount)                // the row's body
{
    std::vector<unsigned short> faces;
    for (unsigned long i = 0; i < triangleCount; ++i) faces.push_back((unsigned short)i);
    std::vector<unsigned short> positive, negative;
    const Segment* result;
    if (Calculate(faces, 0, positive, negative)) result = new Branch(positive, negative, 0);
    else result = new Leaf(faces);
    return result;
}

int main()
{
    int ok = 1;
    const Segment* s = Create(7);
    ok &= s->Kind() == 1 && ((const Leaf*)s)->Faces.size() == 7 && ((const Leaf*)s)->Faces[6] == 6;
    ok &= g_seenDepth == 0 && g_seenFaces == 7;
    delete s;
    g_split = true;
    s = Create(5);
    ok &= s->Kind() == 2 && ((const Branch*)s)->Positive.size() == 3 && ((const Branch*)s)->Negative.size() == 2;
    ok &= ((const Branch*)s)->Depth == 0;
    delete s;
    s = Create(0);
    ok &= s->Kind() == 2 && ((const Branch*)s)->Positive.empty();
    delete s;
    if (ok) std::printf("00a86cd0_TEST PASS\n"); else std::printf("00a86cd0_TEST FAIL\n");
    return ok ? 0 : 1;
}