// Behaviour test for 005d93b0 CTCInventoryItem::GetTattooCardDef (the thing def's CTattooDef sub-def as a counted pointer)
#include <cstdio>
static int g_destroyed = 0;
struct CDefBase {
    long m_count;
    CDefBase() : m_count(1) {}
    virtual void v00() {}
    virtual void Destroy() { ++g_destroyed; }
    virtual ~CDefBase() {}
    void Release() { if (--m_count == 0) Destroy(); }
};
struct CDefPointer {
    CDefBase* m_p;
    CDefPointer() : m_p(0) { }
    CDefPointer(CDefBase* p) : m_p(p) { if (m_p) ++m_p->m_count; }
    CDefPointer(const CDefPointer& other) : m_p(other.m_p) { if (m_p) ++m_p->m_count; }
    ~CDefPointer() { if (m_p) m_p->Release(); }
    operator bool() const { return m_p != 0; }
};
struct CDefClassBase : CDefBase {
    CDefBase* sub;                                   // the sub-def this def resolves (0 = none)
    CDefClassBase() : sub(0) {}
    bool GetSubDefTattoo(CDefPointer& out)
    {
        if (!sub) return false;
        out.m_p = sub; ++sub->m_count;
        return true;
    }
};
struct CThing { CDefClassBase* m_pDef; };
struct CTCInventoryItem {
    CThing* m_pThing;
    CDefPointer GetTattooCardDef();
};
CDefPointer CTCInventoryItem::GetTattooCardDef()
{
    CDefPointer def(m_pThing->m_pDef);
    if (def) {
        CDefPointer out;
        if (((CDefClassBase*)def.m_p)->GetSubDefTattoo(out))
            return out;
    }
    return CDefPointer();
}
int main()
{
    CDefBase sub; CDefClassBase def; CThing thing; CTCInventoryItem item;
    item.m_pThing = &thing;
    thing.m_pDef = 0;
    { CDefPointer r = item.GetTattooCardDef(); if (r) { std::printf("005d93b0_TEST FAIL\n"); return 1; } }
    thing.m_pDef = &def;
    { CDefPointer r = item.GetTattooCardDef(); if (r || def.m_count != 1) { std::printf("005d93b0_TEST FAIL\n"); return 1; } }
    def.sub = &sub;
    {
        CDefPointer r = item.GetTattooCardDef();
        if (r.m_p != &sub || sub.m_count != 2 || def.m_count != 1) { std::printf("005d93b0_TEST FAIL\n"); return 1; }
    }
    if (sub.m_count != 1 || def.m_count != 1 || g_destroyed != 0) { std::printf("005d93b0_TEST FAIL\n"); return 1; }
    std::printf("005d93b0_TEST PASS\n");
    return 0;
}