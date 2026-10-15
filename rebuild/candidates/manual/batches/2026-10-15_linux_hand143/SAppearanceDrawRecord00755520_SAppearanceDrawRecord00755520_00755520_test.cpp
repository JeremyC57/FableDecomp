// Behaviour test for 0x00755520 (CTCLook::OnAppearanceDraw record destructor): the tree is emptied and freed, the member destroyed, the pointee released.
#include <cstdio>
#include <cstdlib>
static int g_log[8]; static int g_n = 0; static int g_erased = 0;
struct Pointee { int refs; bool deleted; void Delete() { deleted = true; } };
struct CountedPtr { Pointee* Object; ~CountedPtr() { Pointee* old = Object; if (old) { if (--old->refs == 0) old->Delete(); Object = 0; } g_log[g_n++] = 3; } };
struct Member { ~Member() { g_log[g_n++] = 2; } };
struct Tree {
    struct node { unsigned char colour; node* parent; node* left; node* right; };
    node* head; unsigned int size;
    void Erase(node* root) { if (root) ++g_erased; }
    ~Tree() { if (size) { Erase(head->parent); head->left = head; head->parent = 0; head->right = head; size = 0; } if (head) std::free(head); g_log[g_n++] = 1; }
};
struct Record { CountedPtr m_pointee; Member m_member; Tree m_tree; };
int main()
{
    Pointee p = { 1, false };
    {
        Record r; r.m_pointee.Object = &p; r.m_tree.head = (Tree::node*)std::malloc(sizeof(Tree::node)); r.m_tree.head->parent = r.m_tree.head; r.m_tree.size = 2;
    }
    int ok = g_n == 3 && g_log[0] == 1 && g_log[1] == 2 && g_log[2] == 3 && p.deleted && p.refs == 0 && g_erased == 1;
    if (!ok) { std::printf("00755520_TEST FAIL\n"); return 1; }
    std::printf("00755520_TEST PASS\n");
    return 0;
}
