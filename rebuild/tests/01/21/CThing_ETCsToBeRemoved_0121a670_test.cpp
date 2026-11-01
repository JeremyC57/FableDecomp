// Behaviour test for 0x0121A670 (dynamic initializer of CThing::TCsToBeRemoved): the STLport _Rb_tree default constructor
// allocates a header node, colours it red (0), sets root 0 and leftmost/rightmost to the header, count 0; the atexit
// destructor (clear() of an empty tree, then free of the header) leaves nothing behind.
#include <cstdio>
#include <cstdlib>

static int g_mallocs = 0, g_frees = 0;
static void* test_malloc(unsigned int n) { ++g_mallocs; return std::malloc(n); }
static void test_free(void* p) { if (p) { ++g_frees; std::free(p); } }

struct NodeBase { bool color; NodeBase* parent; NodeBase* left; NodeBase* right; };
struct Value16 { long key; unsigned char rest[12]; };
struct Node : NodeBase { Value16 value; };

struct Tree {
    Node* header;
    unsigned int count;
    Tree() : header(0), count(0)
    {
        header = (Node*)test_malloc(sizeof(Node));
        header->color = false;
        header->parent = 0;
        header->left = header;
        header->right = header;
    }
    ~Tree()
    {
        if (count != 0) { /* _M_erase(root) -- not reached for an empty tree */ }
        test_free(header);
    }
};

int main()
{
    if (sizeof(Node) != 0x20 && sizeof(void*) == 4) { std::printf("0121a670_TEST FAIL\n"); return 1; }
    {
        Tree t;                                          // what the initializer does to the static
        if (g_mallocs != 1 || t.count != 0 || t.header->color != false || t.header->parent != 0 ||
            t.header->left != t.header || t.header->right != t.header) {
            std::printf("0121a670_TEST FAIL\n"); return 1;
        }
    }                                                    // what the atexit destructor does
    if (g_frees != 1) { std::printf("0121a670_TEST FAIL\n"); return 1; }
    std::printf("0121a670_TEST PASS\n");
    return 0;
}