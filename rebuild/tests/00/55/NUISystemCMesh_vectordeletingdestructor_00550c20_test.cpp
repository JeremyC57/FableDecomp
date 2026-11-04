// Behaviour test for the NUISystem::CMesh vector-deleting-destructor adjustor thunk 0x00550C20: deleting the object through its
// ActionParam subobject (at +0x18 in retail) must run the full object's destructor once and free the full object's address
// (the thunk subtracts the subobject offset before jumping to ??_G).
#include <cstdio>
static int g_dtors = 0;
struct UIInterface { virtual ~UIInterface() {} };
struct Observer { virtual ~Observer() {} int events[4]; };
struct ActionParam { virtual ~ActionParam() {} int type; };
struct Component : UIInterface, Observer, ActionParam { char members[0x40]; };
struct CMesh : Component { char own[0x20]; ~CMesh() { ++g_dtors; } };
int main()
{
    CMesh* object = new CMesh;
    ActionParam* sub = object;
    if ((void*)sub == (void*)object) { std::printf("00550c20_TEST FAIL\n"); return 1; }
    delete sub;
    if (g_dtors != 1) { std::printf("00550c20_TEST FAIL\n"); return 1; }
    std::printf("00550c20_TEST PASS\n");
    return 0;
}