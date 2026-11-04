// Behaviour test for the NUISystem::CMorphingSprite vector-deleting-destructor adjustor thunk 0x00555120: deleting the object through its
// Observer subobject (at +4 in retail) must run the full object's destructor once and release the full object
// (the thunk subtracts the subobject offset before jumping to ??_G).
#include <cstdio>
#include <cstdlib>
static int g_dtors = 0;
static void* g_block = 0;
static void* g_freed = 0;
struct UIInterface { virtual ~UIInterface() {} };
struct Observer { virtual ~Observer() {} int events[4]; };
struct ActionParam { virtual ~ActionParam() {} int type; };
struct Component : UIInterface, Observer, ActionParam { char members[0x40]; };
struct CMorphingSprite : Component {
    char own[0x20];
    ~CMorphingSprite() { ++g_dtors; }
    static void* operator new(std::size_t size) { g_block = std::malloc(size); return g_block; }
    static void operator delete(void* p) { g_freed = p; std::free(p); }
};
int main()
{
    CMorphingSprite* object = new CMorphingSprite;
    Observer* sub = object;
    if ((void*)sub == (void*)object) { std::printf("00555120_TEST FAIL\n"); return 1; }
    delete sub;
    if (g_dtors != 1 || g_freed != g_block) { std::printf("00555120_TEST FAIL\n"); return 1; }
    std::printf("00555120_TEST PASS\n");
    return 0;
}