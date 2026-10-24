// Behaviour test for 0x0098F2A0 CThreadedFileManager::ThreadMain: ProcessObjects runs until ExitThread is raised (not at
// all when already raised), then every non-null file object has its OVERLAPPED block freed (when present) and is
// deleted, and ThreadFinished is set under the lock.
#include <cstdio>
static int g_enter = 0, g_leave = 0, g_freed = 0, g_deleted = 0;
struct NodeBase { bool color; NodeBase* parent; NodeBase* left; NodeBase* right; };
struct CFileObject { void* Overlapped; };
struct Node : NodeBase { unsigned long key; CFileObject* second; };
static NodeBase* Increment(NodeBase* n)
{
    if (n->right) { n = n->right; while (n->left) n = n->left; }
    else { NodeBase* y = n->parent; while (n == y->right) { n = y; y = y->parent; } if (n->right != y) n = y; }
    return n;
}
struct Manager {
    NodeBase* header; bool ExitThread; bool ThreadFinished; int processCalls; int exitAfter; bool finishedSeenInLock;
    void ProcessObjects() { if (++processCalls >= exitAfter) ExitThread = true; }
    void ThreadMain()
    {
        while (!ExitThread)
            ProcessObjects();
        for (NodeBase* it = header->left; it != header; it = Increment(it)) {
            CFileObject* object = ((Node*)it)->second;
            if (object) {
                if (object->Overlapped)
                    ++g_freed;
                ++g_deleted;
            }
        }
        ++g_enter;
        ThreadFinished = true;
        ++g_leave;
    }
};
int main()
{
    int fails = 0, ov = 0;
    CFileObject o1 = { &ov }, o3 = { 0 };
    NodeBase h; Node n1, n2, n3;
    n2.parent = &h; n2.left = &n1; n2.right = &n3; n2.key = 2; n2.second = 0;
    n1.parent = &n2; n1.left = 0; n1.right = 0; n1.key = 1; n1.second = &o1;
    n3.parent = &n2; n3.left = 0; n3.right = 0; n3.key = 3; n3.second = &o3;
    h.parent = &n2; h.left = &n1; h.right = &n3;
    Manager m; m.header = &h; m.ExitThread = false; m.ThreadFinished = false; m.processCalls = 0; m.exitAfter = 3;
    m.ThreadMain();
    if (m.processCalls != 3 || g_freed != 1 || g_deleted != 2 || !m.ThreadFinished || g_enter != 1 || g_leave != 1) ++fails;
    Manager e; e.header = &h; h.left = &h; h.right = &h; e.ExitThread = true; e.ThreadFinished = false; e.processCalls = 0; e.exitAfter = 1;
    e.ThreadMain();
    if (e.processCalls != 0 || g_deleted != 2 || !e.ThreadFinished) ++fails;
    std::printf(fails ? "0098f2a0_TEST FAIL\n" : "0098f2a0_TEST PASS\n");
    return fails ? 1 : 0;
}