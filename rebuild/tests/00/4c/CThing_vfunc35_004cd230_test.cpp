// Behaviour test for 004cd230 CThing::vfunc_35 (default: always false)
#include <cstdio>
struct CThing {
    virtual bool vfunc_35(void* arg);
    virtual ~CThing() {}
};
bool CThing::vfunc_35(void* arg)
{
    return false;
}
int main()
{
    CThing t; int x = 0;
    if (t.vfunc_35(0) || t.vfunc_35(&x)) { std::printf("004cd230_TEST FAIL\n"); return 1; }
    std::printf("004cd230_TEST PASS\n");
    return 0;
}