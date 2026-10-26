// Behaviour test for 009901d0: STLport _Rb_tree::_M_lower_bound -- the first node whose key does not sort before k, under CThreadedFileManager's pending-object order (physical sort key, then file position, then object address)
#include <cstdio>

class CThreadedFile { public: unsigned int PhysicalSortKey; };
struct CFileObject {
    unsigned int   FilePos;
    CThreadedFile* PFile;
};
class CPendingObjectSortKey {
public:
    CFileObject* Object;
    bool operator<(const CPendingObjectSortKey& o) const
    {
        if (Object->PFile->PhysicalSortKey < o.Object->PFile->PhysicalSortKey)
            return true;
        if (Object->PFile->PhysicalSortKey > o.Object->PFile->PhysicalSortKey)
            return false;
        if (Object->FilePos < o.Object->FilePos)
            return true;
        if (Object->FilePos > o.Object->FilePos)
            return false;
        return Object < o.Object;
    }
};
struct NodeBase {
    bool      Color;
    NodeBase* Parent;
    NodeBase* Left;
    NodeBase* Right;
};
template <class V> struct Node : NodeBase { V Value; };
template <class T1, class T2> struct Pair { T1 first; T2 second; };
typedef Pair<CPendingObjectSortKey, CFileObject*> ValueType;
typedef Node<ValueType> TreeNode;

struct CPendingObjectTree {
    TreeNode* Header;
    TreeNode* Bound(const CPendingObjectSortKey& k) const
    {
        TreeNode* y = Header;
        TreeNode* x = (TreeNode*)Header->Parent;
        while (x != 0)
            if (!(x->Value.first < k))
                y = x, x = (TreeNode*)x->Left;
            else
                x = (TreeNode*)x->Right;
        return y;
    }
};

static void BstInsert(NodeBase* header, TreeNode* n)
{
    n->Left = n->Right = 0;
    NodeBase* y = header;
    NodeBase* x = header->Parent;
    bool goLeft = true;
    while (x) { y = x; goLeft = n->Value.first < ((TreeNode*)x)->Value.first; x = goLeft ? x->Left : x->Right; }
    n->Parent = y;
    if (y == header) header->Parent = n;
    else if (goLeft) y->Left = n;
    else y->Right = n;
}

int main()
{
    CThreadedFile files[2] = { { 5 }, { 9 } };
    CFileObject objs[6] = { { 100, &files[0] }, { 200, &files[0] }, { 200, &files[0] }, { 50, &files[1] }, { 300, &files[1] }, { 400, &files[1] } };
    CPendingObjectSortKey keys[6];
    for (int i = 0; i < 6; ++i) keys[i].Object = &objs[i];
    TreeNode header;
    header.Parent = 0; header.Left = header.Right = &header;
    TreeNode nodes[6];
    int order[6] = { 3, 0, 5, 1, 4, 2 };
    for (int i = 0; i < 6; ++i) { nodes[order[i]].Value.first = keys[order[i]]; nodes[order[i]].Value.second = &objs[order[i]]; BstInsert(&header, &nodes[order[i]]); }
    CPendingObjectTree tree;
    tree.Header = &header;
    bool ok = true;
    CFileObject probes[4] = { { 150, &files[0] }, { 0, &files[1] }, { 999, &files[1] }, { 0, &files[0] } };
    for (int p = 0; p < 10; ++p) {
        CPendingObjectSortKey k;
        k.Object = p < 6 ? &objs[p] : &probes[p - 6];
        // reference: the smallest key satisfying the bound predicate (keys are in increasing order by construction)
        TreeNode* expect = &header;
        for (int i = 0; i < 6; ++i) {
            bool hit = !(keys[i] < k);
            if (hit && (expect == &header || keys[i] < expect->Value.first)) expect = &nodes[i];
        }
        ok = ok && tree.Bound(k) == expect;
    }
    if (!ok) { std::printf("009901d0_TEST FAIL\n"); return 1; }
    std::printf("009901d0_TEST PASS\n");
    return 0;
}