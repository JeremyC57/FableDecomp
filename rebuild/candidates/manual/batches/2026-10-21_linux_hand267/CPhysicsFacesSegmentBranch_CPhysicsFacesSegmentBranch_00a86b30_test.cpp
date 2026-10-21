// Behaviour test for 00a86b30 (CPhysicsFacesSegmentBranch ctor): the plane is stored, a face set that splits again
// becomes a nested branch (depth + 1), one that does not becomes a leaf holding a copy of the faces.
#include <cstdio>
#include <vector>

struct Plane { float a, b, c, d; };
struct Node { virtual ~Node() { } virtual int kind() const = 0; };
struct Leaf : Node { std::vector<unsigned short> faces; Leaf(const std::vector<unsigned short>& f) : faces(f) { } int kind() const { return 0; } };
static int g_calls = 0;
// splits while the set has more than two faces and the depth is below 4: halves the set
static bool Calculate(const std::vector<unsigned short>& faces, int depth, Plane* plane, std::vector<unsigned short>* pos, std::vector<unsigned short>* neg)
{
    ++g_calls;
    if (faces.size() <= 2 || depth >= 4) return false;
    plane->a = 1.0f; plane->b = 0.0f; plane->c = 0.0f; plane->d = (float)depth;
    for (size_t i = 0; i < faces.size(); ++i) (i < faces.size() / 2 ? pos : neg)->push_back(faces[i]);
    return true;
}
struct Branch : Node {
    Plane plane; Node* positive; Node* negative; int depth;
    Branch(const Plane& p, const std::vector<unsigned short>& posFaces, const std::vector<unsigned short>& negFaces, int d)
        : plane(p), depth(d)
    {
        std::vector<unsigned short> cp, cn; Plane child;
        if (Calculate(posFaces, d + 1, &child, &cp, &cn)) positive = new Branch(child, cp, cn, d + 1);
        else positive = new Leaf(posFaces);
        cp.clear(); cn.clear();
        if (Calculate(negFaces, d + 1, &child, &cp, &cn)) negative = new Branch(child, cp, cn, d + 1);
        else negative = new Leaf(negFaces);
    }
    ~Branch() { delete positive; delete negative; }
    int kind() const { return 1; }
};
int main()
{
    Plane p = { 0.0f, 1.0f, 0.0f, 5.0f };
    std::vector<unsigned short> a, b;
    a.push_back(1); a.push_back(2); a.push_back(3); a.push_back(4);
    b.push_back(9);
    Branch root(p, a, b, 0);
    int ok = root.plane.d == 5.0f && root.positive->kind() == 1 && root.negative->kind() == 0;
    Branch* inner = (Branch*)root.positive;
    ok = ok && inner->depth == 1 && inner->plane.d == 1.0f && inner->positive->kind() == 0 && ((Leaf*)inner->positive)->faces.size() == 2;
    ok = ok && ((Leaf*)root.negative)->faces.size() == 1 && ((Leaf*)root.negative)->faces[0] == 9 && g_calls == 4;
    if (!ok) { std::printf("00a86b30_TEST FAIL\n"); return 1; }
    std::printf("00a86b30_TEST PASS\n");
    return 0;
}
