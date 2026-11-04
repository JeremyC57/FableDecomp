// Behaviour test for the NUISystem::CRedefinerList vector-deleting-destructor adjustor thunk 0x00557340: deleting the object through its
// Observer subobject (+4 in retail) must run the full object's destructor once and free the full object's address
// (the thunk subtracts the subobject offset before jumping to the deleting destructor).
#include <cstdio>
static int g_dtors = 0;
struct UIInterface { virtual ~UIInterface() {} };
struct Observer { virtual ~Observer() {} int events[4]; };
struct ActionParam { virtual ~ActionParam() {} int type; };
struct Component : UIInterface, Observer, ActionParam { char members[0x40]; };
struct CRedefinerList : Component { char own[0x20]; ~CRedefinerList() { ++g_dtors; } };
int main()
{
    CRedefinerList* object = new CRedefinerList;
    Observer* sub = object;
    if ((void*)sub == (void*)object) { std::printf("00557340_TEST FAIL\n"); return 1; }
    delete sub;
    if (g_dtors != 1) { std::printf("00557340_TEST FAIL\n"); return 1; }
    std::printf("00557340_TEST PASS\n");
    return 0;
}