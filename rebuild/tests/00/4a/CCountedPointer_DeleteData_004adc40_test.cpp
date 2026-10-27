// Behaviour test for 004adc40 (plain C++98 model of the reconstructed logic).
#include <cstdio>
#include <new>
#include <cstdlib>
static int g_Cleared = 0, g_Freed = 0;
struct List { void* Node; ~List() { ++g_Cleared; if (Node) { std::free(Node); ++g_Freed; } } };
class CMessageEventManager { public: void* World; List Messages; };
template <class T> class CCountedPointer { public: static void DeleteData(void* data) { delete (T*)data; } };
int main() {
    CMessageEventManager* m = new CMessageEventManager; m->World = 0; m->Messages.Node = std::malloc(16);
    CCountedPointer<CMessageEventManager>::DeleteData(m);
    if (g_Cleared != 1 || g_Freed != 1) { std::printf("004adc40_TEST FAIL\n"); return 1; }
    CCountedPointer<CMessageEventManager>::DeleteData(0);
    if (g_Cleared != 1) { std::printf("004adc40_TEST FAIL\n"); return 1; }
    { std::printf("004adc40_TEST PASS\n"); return 0; }
}