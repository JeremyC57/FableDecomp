// Behaviour test for 0x00528930 _Rb_tree<unsigned __int64, pair<const unsigned __int64, CPersistedDeadEntity> >::_M_clone_node: a new node with a copy of the value, the source colour, and no children
extern "C" int printf(const char*, ...);
static int fails = 0;
#define CHECK(c) do { if (!(c)) { ++fails; printf("FAIL line %d\n", __LINE__); } } while (0)
#ifdef _MSC_VER
typedef unsigned int tsize;
#else
typedef __SIZE_TYPE__ tsize;
#endif
struct CPersistedDeadEntity { int a, b, c; };
struct Pair { unsigned long long first; CPersistedDeadEntity second; };
struct Node { bool color; Node* parent; Node* left; Node* right; Pair value; };
static Node g_pool[2]; static int g_used = 0;
static Node* create_node(const Pair& x) { Node* t = &g_pool[g_used++]; t->left = t->right = t->parent = (Node*)1; t->value = x; return t; }
static Node* clone_node(Node* x) { Node* t = create_node(x->value); t->color = x->color; t->left = 0; t->right = 0; return t; }
int main()
{
    Node src; src.color = true; src.left = &src; src.right = &src; src.parent = 0;
    src.value.first = 0x123456789ULL; src.value.second.a = 7; src.value.second.b = 8; src.value.second.c = 9;
    Node* n = clone_node(&src);
    CHECK(n == &g_pool[0] && n->color == true && n->left == 0 && n->right == 0);
    CHECK(n->value.first == 0x123456789ULL && n->value.second.a == 7 && n->value.second.c == 9);
    src.color = false; CHECK(clone_node(&src)->color == false);
    if (fails == 0) printf("00528930_TEST PASS\n"); else printf("00528930_TEST FAIL\n");
    return fails;
}