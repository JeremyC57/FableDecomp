// Behaviour test for CThingObject::IsCrop (0x00702CB0): bit 1 of the virtual GetProperties() word.
#include <cstdio>
class CThingObject { public:
    unsigned int Props;
    virtual ~CThingObject() {}
    virtual unsigned int GetProperties() const { return Props; }
    virtual bool IsCrop() const { return (GetProperties() >> 1) & 1; }
};
int main()
{
    CThingObject o;
    unsigned int props[] = { 0u, 1u, 2u, 3u, 0xFFFFFFFDu, 0x80000002u };
    bool want[] = { false, false, true, true, false, true };
    for (int i = 0; i < 6; ++i) { o.Props = props[i]; if (o.IsCrop() != want[i]) { std::printf("00702cb0_TEST FAIL\n"); return 1; } }
    std::printf("00702cb0_TEST PASS\n");
    return 0;
}