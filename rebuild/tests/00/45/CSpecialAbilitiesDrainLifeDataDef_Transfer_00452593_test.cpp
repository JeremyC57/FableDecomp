#include <cstdio>
// Behaviour test for 0x00452593: the reconstructed body (pragmas stripped) driven through recording stubs;
// checks the transfer order, the helper chosen per member (kind) and the member address.
static const void* g_p[32]; static int g_k[32]; static int g_n = 0;
static void Rec(int k, const void* p) { if (g_n < 32) { g_k[g_n] = k; g_p[g_n] = p; } ++g_n; }
class CPersistContext { public:
    template <class T> void Transfer(T& value);       // one-argument transfer helpers (namepass TR2)
};
class CSpecialAbilitiesDrainLifeDataDef { public:
    virtual void Transfer(CPersistContext& context);
    unsigned char _base04[0x24];       // CSubDefClassBase body
    float Damage;           // +0x28
};
template <> void CPersistContext::Transfer<float>(float& value);

void CSpecialAbilitiesDrainLifeDataDef::Transfer(CPersistContext& context)
{
    context.Transfer(Damage);
}
template <> void CPersistContext::Transfer<float>(float& v) { Rec(2, &v); }
static CSpecialAbilitiesDrainLifeDataDef d;
int main()
{
    CPersistContext c;
    d.Transfer(c);
    bool ok = g_n == 1;
    ok = ok && g_k[0] == 2 && g_p[0] == (const void*)&d.Damage;
    if (ok) { std::printf("CSpecialAbilitiesDrainLifeDataDef_00452593_TEST PASS\n"); return 0; }
    std::printf("FAIL %d\n", g_n);
    return 1;
}
