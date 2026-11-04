// Behaviour test for the NUISystem::CFrontEndList vector-deleting-destructor adjustor thunk 0x0054DA50: deleting the object through its
// Observer subobject (at +4 in retail) must run the full object's destructor once and free the full object's address
// (the thunk subtracts the subobject offset before jumping to ??_G).
#include <cstdio>
static int g_dtors = 0;
struct UIInterface { virtual ~UIInterface() {} };
struct Observer { virtual ~Observer() {} int events[4]; };
struct ActionParam { virtual ~ActionParam() {} int type; };
struct Component : UIInterface, Observer, ActionParam { char members[0x40]; };
struct CFrontEndList : Component { char own[0x20]; ~CFrontEndList() { ++g_dtors; } };
int main()
{
    CFrontEndList* object = new CFrontEndList;
    Observer* sub = object;
    if ((void*)sub == (void*)object) { std::printf("0054da50_TEST FAIL\n"); return 1; }
    delete sub;
    if (g_dtors != 1) { std::printf("0054da50_TEST FAIL\n"); return 1; }
    std::printf("0054da50_TEST PASS\n");
    return 0;
}