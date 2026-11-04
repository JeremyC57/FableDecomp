// Behaviour test for the NUISystem::CViewport vector-deleting-destructor adjustor thunk 0x0054EC60: deleting the object through its
// ActionParam subobject (+0x18 in retail) must run the full object's destructor once and free the full object's address
// (the thunk subtracts the subobject offset before jumping to the deleting destructor).
#include <cstdio>
static int g_dtors = 0;
struct UIInterface { virtual ~UIInterface() {} };
struct Observer { virtual ~Observer() {} int events[4]; };
struct ActionParam { virtual ~ActionParam() {} int type; };
struct Component : UIInterface, Observer, ActionParam { char members[0x40]; };
struct CViewport : Component { char own[0x20]; ~CViewport() { ++g_dtors; } };
int main()
{
    CViewport* object = new CViewport;
    ActionParam* sub = object;
    if ((void*)sub == (void*)object) { std::printf("0054ec60_TEST FAIL\n"); return 1; }
    delete sub;
    if (g_dtors != 1) { std::printf("0054ec60_TEST FAIL\n"); return 1; }
    std::printf("0054ec60_TEST PASS\n");
    return 0;
}