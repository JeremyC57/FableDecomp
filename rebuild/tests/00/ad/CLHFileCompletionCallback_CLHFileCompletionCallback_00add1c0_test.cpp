// behaviour test for 0x00ADD1C0 CLHFileCompletionCallback::CLHFileCompletionCallback(void*, void*): the base part is constructed first, then the two arguments land in their own members (Callback from the first argument, UserData from the second argument)
#include <cstdio>
static int gOrder = 0, gBaseOrder = 0;
struct Base { int state; Base() : state(7) { gBaseOrder = ++gOrder; } virtual ~Base() { } };
struct Object : Base {
    void* Callback; void* UserData;
    Object(void* a1, void* a2) : Callback(a1), UserData(a2) { }      // the function under test
};
int main()
{
    int fail = 0;
    int x = 1, y = 2;
    gOrder = 0;
    Object o(&x, &y);
    if (gBaseOrder != 1 || o.state != 7) fail |= 1;
    if (o.Callback != (void*)&x || o.UserData != (void*)&y) fail |= 2;
    if (fail) { std::printf("00add1c0_TEST FAIL %d\n", fail); return 1; }
    std::printf("00add1c0_TEST PASS\n");
    return 0;
}