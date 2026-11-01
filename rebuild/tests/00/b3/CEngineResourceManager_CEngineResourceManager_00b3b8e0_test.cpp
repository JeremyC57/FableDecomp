// Behaviour test for 0x00B3B8E0 CEngineResourceManager::~CEngineResourceManager: counted-pointer release and map clear semantics used by the destructor.
#include <cstdio>
struct Info { unsigned long RefCount; int deleted; };
struct CP { void* Data; Info* Info_; };
static void Release(CP& p)
{
    if (p.Info_) {
        --p.Info_->RefCount;
        if (p.Info_->RefCount == 0)
            p.Info_->deleted = 1;
    }
    p.Data = 0;
    p.Info_ = 0;
}
struct Node { Node* parent; Node* left; Node* right; };
struct Tree { Node* header; unsigned long count; };
static void Clear(Tree& t)
{
    if (t.count != 0) {
        t.header->left = t.header;
        t.header->parent = 0;
        t.header->right = t.header;
        t.count = 0;
    }
}
int main()
{
    Info shared = { 2, 0 };
    CP a = { (void*)&shared, &shared }, b = { (void*)&shared, &shared };
    Release(a);
    bool ok = shared.RefCount == 1 && !shared.deleted && a.Data == 0 && a.Info_ == 0;
    Release(b);
    ok = ok && shared.deleted == 1;
    Node h = { 0, 0, 0 }, n = { 0, 0, 0 };
    h.parent = &n;
    Tree t = { &h, 1 };
    Clear(t);
    ok = ok && t.count == 0 && h.left == &h && h.right == &h && h.parent == 0;
    std::printf(ok ? "00b3b8e0_TEST PASS\n" : "00b3b8e0_TEST FAIL\n");
    return ok ? 0 : 1;
}