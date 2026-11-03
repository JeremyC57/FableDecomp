// Behaviour test for the CScrollingViewport vector-deleting-destructor adjustor thunk 0x0053FC10: deleting the object
// through its secondary base (Observer at +0x4 in retail) must run the full object's destructor once and free the
// full object's address (the thunk subtracts the subobject offset before jumping to ??_G 0x0053FC30).
#include <cstdio>
static int g_dtors = 0;
struct UIInterface { virtual ~UIInterface() {} };
struct Observer { virtual ~Observer() {} int events; };
struct ActionParam { virtual ~ActionParam() {} };
struct Viewport : UIInterface, Observer, ActionParam { char members[0x40]; ~Viewport() { ++g_dtors; } };
int main()
{
    Viewport* v = new Viewport;
    Observer* sub = v;
    if ((void*)sub == (void*)v) { std::printf("0053fc10_TEST FAIL\n"); return 1; }
    delete sub;
    if (g_dtors != 1) { std::printf("0053fc10_TEST FAIL\n"); return 1; }
    std::printf("0053fc10_TEST PASS\n");
    return 0;
}