// Behaviour test for the NUISystem::CIconText vector-deleting-destructor adjustor thunk 0x00543B30: deleting the icon text through its
// CObserver subobject (+4 in retail) must run the full object's destructor once and free the full object's address
// (the thunk subtracts the subobject offset before jumping to the deleting destructor 0x00543C70).
#include <cstdio>
static int g_dtors = 0;
static void* g_freed = 0;
struct UIInterface { virtual ~UIInterface() {} };
struct Observer { virtual ~Observer() {} int events[4]; };
struct ActionParam { virtual ~ActionParam() {} int type; };
struct Component : UIInterface, Observer, ActionParam { char members[0x40]; };
struct CIconText : Component {
    char own[0x40];
    ~CIconText() { ++g_dtors; }
    static void operator delete(void* p) { g_freed = p; ::operator delete(p); }
};
int main()
{
    CIconText* object = new CIconText;
    Observer* sub = object;
    if ((void*)sub == (void*)object) { std::printf("00543b30_TEST FAIL\n"); return 1; }
    delete sub;
    if (g_dtors != 1 || g_freed != (void*)object) { std::printf("00543b30_TEST FAIL\n"); return 1; }
    std::printf("00543b30_TEST PASS\n");
    return 0;
}