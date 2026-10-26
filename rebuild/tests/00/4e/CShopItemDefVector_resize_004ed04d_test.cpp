#include <cstdio>
// CShopItemDefVector::resize(n): resize(n, CShopItemDef()) -- the default temporary is built, passed, then destroyed
static long g_ctor, g_dtor, g_root, g_calls, g_n; static bool g_valueOk;
struct CBase_009fc550 { virtual void v00() { } ~CBase_009fc550() { ++g_root; } };
class CDefPointeeBase : public CBase_009fc550 { public: virtual void v00() { } ~CDefPointeeBase() { } };
class CShopItemDef : public CDefPointeeBase { public:
    long Type; float Quantity;
    virtual void v00() { }
    CShopItemDef() : Type(0), Quantity(0.0f) { ++g_ctor; }
    ~CShopItemDef() { ++g_dtor; }
};
struct CShopItemDefVector {
    CShopItemDef* m_pFirst; CShopItemDef* m_pLast; CShopItemDef* m_pEnd;
    void resize(unsigned long newSize, const CShopItemDef& value) { ++g_calls; g_n = (long)newSize; g_valueOk = value.Type == 0 && g_ctor == 1 && g_dtor == 0; }
    void resize(unsigned long newSize) { resize(newSize, CShopItemDef()); }
};
int main()
{
    CShopItemDefVector v; v.m_pFirst = v.m_pLast = v.m_pEnd = 0;
    v.resize(6);
    if (g_calls != 1 || g_n != 6 || !g_valueOk || g_ctor != 1 || g_dtor != 1 || g_root != 1) { std::printf("004ed04d_TEST FAIL\n"); return 1; }
    std::printf("004ed04d_TEST PASS\n");
    return 0;
}