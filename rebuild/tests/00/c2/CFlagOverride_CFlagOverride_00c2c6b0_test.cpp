#include <cstdio>
#include <cstring>
#include <new>
class CFlagOverrideBase { public: virtual long Kind() { return 1; } CFlagOverrideBase() { } };
class CFlagOverride : public CFlagOverrideBase {
public:
    CFlagOverride() : m_maskB(0), m_valueB(0), m_maskA(0), m_valueA(0) { }
    virtual long Kind() { return 2; }
    unsigned short m_maskB, m_valueB, m_maskA, m_valueA;
};
int main()
{
    char buf[sizeof(CFlagOverride)]; std::memset(buf, 0xAB, sizeof(buf));
    CFlagOverride* o = new (buf) CFlagOverride;
    bool ok = sizeof(CFlagOverride) == 12 && o->Kind() == 2 && o->m_maskB == 0 && o->m_valueB == 0 && o->m_maskA == 0 && o->m_valueA == 0
        && (char*)&o->m_maskA - (char*)o == 8;
    if (!ok) { std::printf("00c2c6b0_TEST FAIL\n"); return 1; }
    std::printf("00c2c6b0_TEST PASS\n");
    return 0;
}
