#include <cstdio>
struct MicroThreadInfo {
    bool Dead;
};
MicroThreadInfo* g_pActiveMicroThread = 0;
struct MicroThreadDetails : MicroThreadInfo {
    unsigned char* _M_start;
    unsigned char* _M_finish;
    unsigned char* _M_end_of_storage;
    unsigned int MicroThreadStackSize() const
    {
        if (g_pActiveMicroThread == this)
            return 0;
        return _M_finish - _M_start;
    }
};
int main()
{
    unsigned char buf[64];
    MicroThreadDetails d;
    d.Dead = false;
    d._M_start = buf;
    d._M_finish = buf + 40;
    d._M_end_of_storage = buf + 64;
    if (d.MicroThreadStackSize() != 40) { std::printf("009d8890_TEST FAIL\n"); return 1; }
    g_pActiveMicroThread = &d;
    if (d.MicroThreadStackSize() != 0) { std::printf("009d8890_TEST FAIL\n"); return 1; }
    std::printf("009d8890_TEST PASS\n");
    return 0;
}