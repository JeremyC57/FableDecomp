// Test for 0x00C3D9A0 CMorphAction_00c3dcd0 vector deleting destructor: element destructors run (last to first) and the
// class operator delete / delete[] free the blocks.
#include <cstdio>
#include <cstdlib>
static int g_dtors = 0, g_frees = 0, g_arrayFrees = 0;
struct CPKObject {
    virtual ~CPKObject() { }
    static void* operator new(size_t n) { return std::malloc(n); }
    static void operator delete(void* p) { ++g_frees; std::free(p); }
    static void* operator new[](size_t n) { return std::malloc(n); }
    static void operator delete[](void* p) { ++g_arrayFrees; std::free(p); }
};
struct CMorphAction : CPKObject { int m_48; CMorphAction() : m_48((int)0x80000000) { } ~CMorphAction() { ++g_dtors; } };
int main()
{
    CMorphAction* arr = new CMorphAction[3];
    delete[] arr;
    bool ok = g_dtors == 3 && g_arrayFrees == 1 && g_frees == 0;
    CPKObject* one = new CMorphAction;
    delete one;
    ok = ok && g_dtors == 4 && g_frees == 1;
    if (!ok) { std::printf("00c3d9a0_TEST FAIL\n"); return 1; }
    std::printf("00c3d9a0_TEST PASS\n");
    return 0;
}