// Behaviour test for 0x0047AE50 CHeroLogBook::Clear(): every given quest is destroyed and finish reset to start, a non-empty log-entry tree is erased and its header reset (leftmost = rightmost = header, root = 0, count = 0), QuestBeingViewed = 0.
#include <cstdio>
static int g_destroyed = 0, g_tree_erase = 0;
struct Quest { unsigned int id; ~Quest() { ++g_destroyed; } };
struct Node { bool color; Node* parent; Node* left; Node* right; };
struct Tree {
    Node* header; unsigned int count;
    void erase(Node* root) { if (root) ++g_tree_erase; }
    void clear()
    {
        if (count != 0) {
            erase(header->parent);
            header->left = header; header->parent = 0; header->right = header; count = 0;
        }
    }
};
struct LogBook {
    Quest* start; Quest* finish; Quest* eos;
    long QuestBeingViewed;
    Tree LogEntries;
    void Clear()
    {
        Quest* i = start;                              // __copy(finish, finish, start) copies nothing
        for (Quest* p = i; p != finish; ++p) p->~Quest();
        finish = i;
        LogEntries.clear();
        QuestBeingViewed = 0;
    }
};
static int fail() { std::printf("0047ae50_TEST FAIL\n"); return 1; }
int main()
{
    static unsigned char raw[3 * sizeof(Quest)];
    Quest* q = (Quest*)raw;
    Node header, root;
    header.parent = &root; header.left = &root; header.right = &root;
    LogBook b; b.start = q; b.finish = q + 3; b.eos = q + 3; b.QuestBeingViewed = 2;
    b.LogEntries.header = &header; b.LogEntries.count = 1;
    b.Clear();
    if (b.finish != q || g_destroyed != 3 || b.QuestBeingViewed != 0 || g_tree_erase != 1 || b.LogEntries.count != 0 ||
        header.parent != 0 || header.left != &header || header.right != &header) return fail();
    b.QuestBeingViewed = 5;
    b.Clear();                                         // empty: nothing destroyed, the tree untouched
    if (g_destroyed != 3 || g_tree_erase != 1 || b.QuestBeingViewed != 0) return fail();
    std::printf("0047ae50_TEST PASS\n");
    return 0;
}