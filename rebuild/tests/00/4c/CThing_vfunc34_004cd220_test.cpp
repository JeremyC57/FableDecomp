// Behaviour test for 004cd220 CThing::vfunc_34 (default: always false)
#include <cstdio>
class CThing { public:
    virtual bool vfunc_34(void* arg);
    virtual ~CThing() {}
};
bool CThing::vfunc_34(void* arg)
{
    return false;
}
int main()
{
    CThing t; int x = 0;
    if (t.vfunc_34(0) || t.vfunc_34(&x)) { std::printf("004cd220_TEST FAIL\n"); return 1; }
    std::printf("004cd220_TEST PASS\n");
    return 0;
}