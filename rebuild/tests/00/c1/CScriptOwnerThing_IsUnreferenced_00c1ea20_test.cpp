#include <cstdio>

// Behaviour model of CScriptOwnerThing::IsUnreferenced (0x00C1EA20): the +0x1E word is zero.
struct CScriptOwnerThing {
    unsigned char  _pad00[0x1e];
    unsigned short m_refCount;
    bool IsUnreferenced() const { return m_refCount == 0; }
};

int main()
{

    CScriptOwnerThing o; o.m_refCount = 0;
    if (!o.IsUnreferenced()) { std::printf("00c1ea20_TEST FAIL\n"); return 1; }
    o.m_refCount = 3;
    if (o.IsUnreferenced()) { std::printf("00c1ea20_TEST FAIL\n"); return 1; }
    o.m_refCount = 0x8000;
    if (o.IsUnreferenced()) { std::printf("00c1ea20_TEST FAIL\n"); return 1; }
    std::printf("00c1ea20_TEST PASS\n");
    return 0;
}