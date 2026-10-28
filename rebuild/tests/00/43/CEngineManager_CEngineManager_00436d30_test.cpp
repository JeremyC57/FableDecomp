// Behaviour test for 0x00436D30 CEngineManager::CEngineManager: the engine list gets a freshly allocated sentinel node linked to itself, and the changed-area vector starts empty
extern "C" int printf(const char*, ...);
static int fails = 0;
#define CHECK(c) do { if (!(c)) { ++fails; printf("FAIL line %d\n", __LINE__); } } while (0)
#ifdef _MSC_VER
typedef unsigned int tsize;
#else
typedef __SIZE_TYPE__ tsize;
#endif
static unsigned char g_heap[256]; static int g_mallocs = 0; static unsigned g_lastSize = 0;
static void* tmalloc(unsigned n) { ++g_mallocs; g_lastSize = n; return g_heap; }
struct Node { Node* next; Node* prev; void* data; };
struct List { Node* node; List() : node(0) { Node* n = (Node*)tmalloc(sizeof(Node)); n->next = n; n->prev = n; node = n; } };
struct Vec { void* start; void* finish; void* eos; Vec() : start(0), finish(0), eos(0) { } };
class CEngineManager { public: List Engines; Vec ChangedAreas; CEngineManager() { } };
int main()
{
    CEngineManager m;
    CHECK(g_mallocs == 1 && g_lastSize == sizeof(Node));
    CHECK(m.Engines.node == (Node*)g_heap);
    CHECK(m.Engines.node->next == m.Engines.node && m.Engines.node->prev == m.Engines.node);
    CHECK(m.ChangedAreas.start == 0 && m.ChangedAreas.finish == 0 && m.ChangedAreas.eos == 0);
    if (fails == 0) printf("00436d30_TEST PASS\n"); else printf("00436d30_TEST FAIL\n");
    return fails;
}