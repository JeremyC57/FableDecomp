// Behaviour test for 0x00AD4C40 QuadricOptimiserInternals::CUniqueVertex::CUniqueVertex (two inlined STLport set<unsigned long> constructors).
#include <cstdio>
#include <cstdlib>

struct SNode { bool Color; SNode* Parent; SNode* Left; SNode* Right; long Value; };
struct SLongSet {
    SNode* Header; unsigned int Count; char Compare;
    SLongSet() : Header(0)
    {
        Header = (SNode*)std::malloc(sizeof(SNode));
        Count = 0;
        Header->Color = false;
        Header->Parent = 0;
        Header->Left = Header;
        Header->Right = Header;
    }
};
struct CUniqueVertex { bool Exists; SLongSet VertexIndices; SLongSet FaceList; CUniqueVertex() { } };

static bool EmptyOk(const SLongSet& s)
{
    return s.Header != 0 && s.Count == 0 && s.Header->Color == false && s.Header->Parent == 0
        && s.Header->Left == s.Header && s.Header->Right == s.Header;
}

int main()
{
    CUniqueVertex r;
    r.Exists = true;
    if (!EmptyOk(r.VertexIndices) || !EmptyOk(r.FaceList) || r.VertexIndices.Header == r.FaceList.Header || r.Exists != true) {
        std::printf("00ad4c40_TEST FAIL\n"); return 1;
    }
    std::free(r.VertexIndices.Header); std::free(r.FaceList.Header);
    std::printf("00ad4c40_TEST PASS\n");
    return 0;
}