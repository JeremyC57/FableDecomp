// Behaviour test for 0x00AD7A00 COptimisedVertex::COptimisedVertex (three inlined STLport tree constructors + Exists = true).
#include <cstdio>
#include <cstdlib>

struct SNode { bool Color; SNode* Parent; SNode* Left; SNode* Right; };
template <int NodeSize> struct STree {
    SNode* Header; unsigned int Count; char Compare;
    STree() : Header(0)
    {
        Header = (SNode*)std::malloc(sizeof(SNode) + NodeSize);   // 0x10-byte link block + value (host pointers may be wider)
        Count = 0;
        Header->Color = false; Header->Parent = 0; Header->Left = Header; Header->Right = Header;
    }
};
struct COptimisedVertex {
    unsigned char Position_to_Cost[0x84];
    STree<0x14> FaceList; bool Exists; unsigned int UniqueIndex; unsigned int OutputIndex;
    STree<0x18> Groups; STree<0x20> PairContractions;
    COptimisedVertex() : Exists(true) { }
};
template <int N> static bool EmptyOk(const STree<N>& t)
{
    return t.Header != 0 && t.Count == 0 && !t.Header->Color && t.Header->Parent == 0 && t.Header->Left == t.Header && t.Header->Right == t.Header;
}

int main()
{
    COptimisedVertex* v = new COptimisedVertex;
    if (!v->Exists || !EmptyOk(v->FaceList) || !EmptyOk(v->Groups) || !EmptyOk(v->PairContractions)) { std::printf("00ad7a00_TEST FAIL\n"); return 1; }
    if (v->FaceList.Header == v->Groups.Header || v->Groups.Header == v->PairContractions.Header) { std::printf("00ad7a00_TEST FAIL\n"); return 1; }
    std::free(v->FaceList.Header); std::free(v->Groups.Header); std::free(v->PairContractions.Header);
    delete v;
    std::printf("00ad7a00_TEST PASS\n");
    return 0;
}