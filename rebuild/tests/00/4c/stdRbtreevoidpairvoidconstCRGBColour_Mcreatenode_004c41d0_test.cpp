// Behaviour test for 0x004C41D0 _Rb_tree<void*, pair<void* const, CRGBColour> >::_M_create_node: one node allocated (sizeof node) and the value pair placement-copied into it, colour channel by channel
extern "C" int printf(const char*, ...);
static int fails = 0;
#define CHECK(c) do { if (!(c)) { ++fails; printf("FAIL line %d\n", __LINE__); } } while (0)
#ifdef _MSC_VER
typedef unsigned int tsize;
#else
typedef __SIZE_TYPE__ tsize;
#endif
class CRGBColour { public: unsigned char B, G, R, A;
    void Set(unsigned char r, unsigned char g, unsigned char b, unsigned char a) { R = r; G = g; B = b; A = a; }
    CRGBColour() { } CRGBColour(const CRGBColour& o) { Set(o.R, o.G, o.B, o.A); } };
struct Pair { void* first; CRGBColour second; Pair(void* f, const CRGBColour& c) : first(f), second(c) { } Pair(const Pair& p) : first(p.first), second(p.second) { } };
struct Node { bool color; Node* parent; Node* left; Node* right; Pair value; };
static unsigned char g_heap[64]; static unsigned g_size = 0; static int g_calls = 0;
static void* tmalloc(unsigned n) { ++g_calls; g_size = n; for (unsigned i = 0; i < sizeof(g_heap); ++i) g_heap[i] = 0xEE; return g_heap; }
inline void* operator new(tsize, void* p) { return p; }
static Node* create_node(const Pair& x) { Node* t = (Node*)tmalloc(sizeof(Node)); new (&t->value) Pair(x); return t; }
int main()
{
    int key = 0; CRGBColour c; c.Set(1, 2, 3, 4);
    Pair p(&key, c);
    Node* n = create_node(p);
    CHECK(g_calls == 1 && g_size == sizeof(Node) && n == (Node*)g_heap);
    CHECK(n->value.first == &key && n->value.second.R == 1 && n->value.second.G == 2 && n->value.second.B == 3 && n->value.second.A == 4);
    if (fails == 0) printf("004c41d0_TEST PASS\n"); else printf("004c41d0_TEST FAIL\n");
    return fails;
}