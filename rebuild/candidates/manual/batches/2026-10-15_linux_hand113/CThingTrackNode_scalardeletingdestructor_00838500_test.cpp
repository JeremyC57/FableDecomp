#include <cstdio>
#include <cstdlib>
// Behaviour test for CThingTrackNode scalar deleting destructor (0x00838500): the +0xe8 string dies before
// the game-object base, and the memory goes back through the class's own operator delete.
static int g_order[8]; static int g_n = 0; static int g_classFrees = 0;
struct CCharString { void* m_pData; ~CCharString() { g_order[g_n++] = 2; } };
struct CThingGameObject {
    virtual ~CThingGameObject() { g_order[g_n++] = 1; }
    static void operator delete(void* p) { ++g_classFrees; std::free(p); }
    static void* operator new(size_t n) { return std::malloc(n); }
};
struct CThingTrackNode : CThingGameObject {
    CCharString m_name;
    virtual ~CThingTrackNode() { }
};
int main()
{
    CThingGameObject* p = new CThingTrackNode;
    delete p;
    if (g_n != 2 || g_order[0] != 2 || g_order[1] != 1 || g_classFrees != 1) { std::printf("00838500_TEST FAIL\n"); return 1; }
    std::printf("00838500_TEST PASS\n");
    return 0;
}
