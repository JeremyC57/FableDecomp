// Behaviour test for the NUISystem::CTextSlider vector-deleting-destructor adjustor thunk 0x0054A390: deleting the object through its
// Observer subobject (at +4 in retail) must run the full object's destructor once and free the full object's address
// (the thunk subtracts the subobject offset before jumping to ??_G).
#include <cstdio>
static int g_dtors = 0;
struct UIInterface { virtual ~UIInterface() {} };
struct Observer { virtual ~Observer() {} int events[4]; };
struct ActionParam { virtual ~ActionParam() {} int type; };
struct Component : UIInterface, Observer, ActionParam { char members[0x40]; };
struct CTextSlider : Component { char own[0x20]; ~CTextSlider() { ++g_dtors; } };
int main()
{
    CTextSlider* object = new CTextSlider;
    Observer* sub = object;
    if ((void*)sub == (void*)object) { std::printf("0054a390_TEST FAIL\n"); return 1; }
    delete sub;
    if (g_dtors != 1) { std::printf("0054a390_TEST FAIL\n"); return 1; }
    std::printf("0054a390_TEST PASS\n");
    return 0;
}