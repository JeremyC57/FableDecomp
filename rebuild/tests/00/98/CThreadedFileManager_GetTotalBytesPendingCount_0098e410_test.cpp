// Behaviour test for 0x0098E410 CThreadedFileManager::GetTotalBytesPendingCount() const: walks a hand-built 4-node red-black map in key
// order (exercising both increment arms) and sums Size over the objects whose status is not 4; the lock is entered and left once.
#include <cstdio>
static int g_enter = 0, g_leave = 0;
struct CS { int x; };
static void Enter(CS*) { ++g_enter; }
static void Leave(CS*) { ++g_leave; }
struct NodeBase { bool color; NodeBase* parent; NodeBase* left; NodeBase* right; };
struct CFileObject { unsigned int Size; int LoadingStatus; };
struct Node : NodeBase { unsigned long key; CFileObject* second; };
struct Iter {
    NodeBase* n;
    void inc()
    {
        if (n->right) { n = n->right; while (n->left) n = n->left; }
        else { NodeBase* y = n->parent; while (n == y->right) { n = y; y = y->parent; } if (n->right != y) n = y; }
    }
};
struct Manager {
    NodeBase* header; mutable CS cs;
    long GetTotalBytesPendingCount() const
    {
        Enter(&cs);
        long total = 0;
        Iter it;
        it.n = header->left;
        for (; it.n != header; it.inc())
            if (((Node*)it.n)->second->LoadingStatus != 4)
                total += ((Node*)it.n)->second->Size;
        Leave(&cs);
        return total;
    }
};
int main()
{
    CFileObject o1 = { 10, 4 }, o2 = { 20, 0 }, o3 = { 40, 4 }, o4 = { 80, 3 };
    NodeBase h; Node n1, n2, n3, n4;
    // keys 1..4: root 2, left 1, right 4 with left child 3
    n2.parent = &h; n2.left = &n1; n2.right = &n4; n2.key = 2; n2.second = &o2;
    n1.parent = &n2; n1.left = 0; n1.right = 0; n1.key = 1; n1.second = &o1;
    n4.parent = &n2; n4.left = &n3; n4.right = 0; n4.key = 4; n4.second = &o4;
    n3.parent = &n4; n3.left = 0; n3.right = 0; n3.key = 3; n3.second = &o3;
    h.parent = &n2; h.left = &n1; h.right = &n4;
    Manager m; m.header = &h;
    int fails = 0;
    if (m.GetTotalBytesPendingCount() != 100) ++fails;
    if (g_enter != 1 || g_leave != 1) ++fails;
    h.left = &h; h.right = &h; h.parent = 0;
    if (m.GetTotalBytesPendingCount() != 0) ++fails;
    std::printf(fails ? "0098e410_TEST FAIL\n" : "0098e410_TEST PASS\n");
    return fails ? 1 : 0;
}