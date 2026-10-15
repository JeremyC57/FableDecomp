// Behaviour test for 0x00BF6B70 (CLandscapeBackgroundTreeNode::OnPostRelocation): owner re-registration of sub node, task and the task's request object.
#include <cstdio>
struct Res { void* parent; Res() : parent(0) {} virtual ~Res() {} virtual void OnPostParentRelocation(void* p) { parent = p; } };
struct Mem { void* blocks[4]; void* owners[4]; int n; void Rebind(void* b, void* o) { blocks[n] = b; owners[n] = o; ++n; } };
static Mem g_mem;
struct Task { Res* owner; };
struct Node {
    Res* SubNode; Task* AsyncLoadTask;
    void OnPostRelocation()
    {
        if (SubNode) { g_mem.Rebind(SubNode, this); SubNode->OnPostParentRelocation(this); }
        if (AsyncLoadTask) {
            g_mem.Rebind(AsyncLoadTask, this);
            if (AsyncLoadTask->owner) { g_mem.Rebind(AsyncLoadTask->owner, this); AsyncLoadTask->owner->OnPostParentRelocation(this); }
        }
    }
};
int main()
{
    Node n; Res sub, req; Task t; t.owner = 0; n.SubNode = 0; n.AsyncLoadTask = 0; g_mem.n = 0;
    n.OnPostRelocation();
    int ok = g_mem.n == 0;
    n.SubNode = &sub; n.AsyncLoadTask = &t; n.OnPostRelocation();
    ok &= g_mem.n == 2 && g_mem.blocks[0] == &sub && g_mem.blocks[1] == &t && sub.parent == &n;
    g_mem.n = 0; t.owner = &req; n.OnPostRelocation();
    ok &= g_mem.n == 3 && g_mem.blocks[2] == &req && g_mem.owners[2] == &n && req.parent == &n;
    if (!ok) { std::printf("00bf6b70_TEST FAIL\n"); return 1; }
    std::printf("00bf6b70_TEST PASS\n");
    return 0;
}
