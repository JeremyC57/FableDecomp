#include <cstdio>
// Behaviour of CTCInventoryQuests::vfunc_0 (0x0061A220): the def's title index goes through GetInventoryText; the def
// pointer copy adds a reference and drops it again (Destroy on the last reference).
struct CWideString { long m_value; CWideString(long v) : m_value(v) {} CWideString(const CWideString& o) : m_value(o.m_value) {} ~CWideString() {} };
struct CInventoryDef {
    long m_destroyed; long m_refCount; long m_titleTextIndex;
    virtual void Destroy() { ++m_destroyed; }
    void AddRef() { ++m_refCount; }
    void Release() { if (--m_refCount == 0) Destroy(); }
};
struct CDefPointer {
    CInventoryDef* m_pDef;
    CDefPointer(const CDefPointer& other) : m_pDef(other.m_pDef) { if (m_pDef) m_pDef->AddRef(); }
    ~CDefPointer() { m_pDef->Release(); }
    CInventoryDef* operator->() const { return m_pDef; }
};
static long g_peakRef = 0;
struct CTCInventoryBase_Retail {
    CDefPointer PDef;
    CTCInventoryBase_Retail(CInventoryDef* d) : PDef(*(CDefPointer*)&d) {}
    CWideString GetInventoryText(long index) { g_peakRef = PDef.m_pDef->m_refCount; return CWideString(index * 3); }
};
struct CBase { virtual CWideString vfunc_0() { return CWideString(-1); } };
struct CTCInventoryQuests : CTCInventoryBase_Retail, CBase {
    CTCInventoryQuests(CInventoryDef* d) : CTCInventoryBase_Retail(d) {}
    virtual CWideString vfunc_0()
    {
        CDefPointer def = PDef;
        return GetInventoryText(def->m_titleTextIndex);
    }
};
int main()
{
    CInventoryDef def; def.m_destroyed = 0; def.m_refCount = 0; def.m_titleTextIndex = 7;
    {
        CTCInventoryQuests screen(&def);                          // PDef holds one reference
        CBase* base = &screen;
        CWideString text = base->vfunc_0();
        if (text.m_value != 21 || g_peakRef != 2 || def.m_refCount != 1 || def.m_destroyed != 0) { std::printf("0061a220_TEST FAIL\n"); return 1; }
        screen.PDef.m_pDef->m_refCount = 0;       // a transient-only reference: the copy's release destroys the def
        CWideString again = base->vfunc_0();
        if (again.m_value != 21 || def.m_destroyed != 1) { std::printf("0061a220_TEST FAIL\n"); return 1; }
        def.m_refCount = 1;
    }
    std::printf("0061a220_TEST PASS\n");
    return 0;
}
