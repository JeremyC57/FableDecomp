#include <cstdio>
#include <cstring>
#include <new>
class CCharString { public:
    void* m_pStringData;
    CCharString() : m_pStringData((void*)1) { }
};
struct SNamedZeroRecord {
    long        Header[4];
    CCharString Name;
    long        Value0, Value1, Value2, Value3, Value4, Value5;
    SNamedZeroRecord()
        : Value0(0), Value1(0), Value2(0), Value3(0), Value4(0), Value5(0)
    {
        std::memset(this, 0, sizeof(*this));
    }
};
int main()
{
    unsigned char raw[sizeof(SNamedZeroRecord)];
    std::memset(raw, 0xCD, sizeof(raw));
    SNamedZeroRecord* r = new (raw) SNamedZeroRecord;
    unsigned int k = 0;
    int bad = 0;
    for (k = 0; k < sizeof(raw); ++k) if (raw[k] != 0) bad = 1;
    if (r->Name.m_pStringData != 0 || r->Header[0] != 0) bad = 1;
    if (bad) { std::printf("006f2bd0_TEST FAIL\n"); return 1; }
    std::printf("006f2bd0_TEST PASS\n");
    return 0;
}