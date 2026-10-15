// Behaviour test for 0x00C597E0 (catalog CThing::AddToRenderList): valid -> forward (a, b, c, handle) to the service; else -9.
#include <cstdio>
struct CService {
    long a, b, c, h;
    long Forward(long a_, long b_, long c_, long handle) { a = a_; b = b_; c = c_; h = handle; return a_ + b_ + c_ + handle; }
};
static CService g_service;
static CService* g_pService = &g_service;
struct CThing {
    bool valid; long m_handle;
    virtual bool IsValid() { return valid; }
    long AddToRenderList(long context, long a, long b, long c)
    {
        if (IsValid())
            return g_pService->Forward(a, b, c, m_handle);
        return -9;
    }
};
int main()
{
    CThing t; t.valid = false; t.m_handle = 100;
    if (t.AddToRenderList(1, 2, 3, 4) != -9) { std::printf("00c597e0_TEST FAIL\n"); return 1; }
    t.valid = true;
    if (t.AddToRenderList(1, 2, 3, 4) != 109 || g_service.a != 2 || g_service.b != 3 || g_service.c != 4 || g_service.h != 100) {
        std::printf("00c597e0_TEST FAIL\n"); return 1;
    }
    std::printf("00c597e0_TEST PASS\n");
    return 0;
}
