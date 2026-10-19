#include <cstdio>
#include <cstring>
static int g_log[64];
static int g_n = 0;
static void Log(int v) { if (g_n < 64) g_log[g_n++] = v; }
struct CThing { int id; };
struct CCharString {
    const char* p;
    CCharString() : p(0) {}
    CCharString(const char* t, long = -1) : p(t) {}
    CCharString& operator=(const CCharString& o) { p = o.p; return *this; }
    unsigned long GetLength() const { return p ? (unsigned long)std::strlen(p) : 0; }
    const char* GetPString() const { return p ? p : ""; }
};
static unsigned long Calc(unsigned long seed, const void* d, unsigned long n)
{
    const unsigned char* c = (const unsigned char*)d;
    unsigned long h = seed;
    for (unsigned long i = 0; i < n; ++i) h = h * 33 + c[i];
    return h;
}
struct CAnimFilterOfGroup {
    CCharString   Group;
    unsigned long GroupCRC;
    CAnimFilterOfGroup() : Group(), GroupCRC(0) {}
    CAnimFilterOfGroup(const CCharString& g) : Group() { Group = g; GroupCRC = Calc(0, g.GetPString(), g.GetLength()); }
    void SetGroup(const CCharString& g) { Group = g; GroupCRC = Calc(0, g.GetPString(), g.GetLength()); }
};
static bool FilterIs(const CAnimFilterOfGroup& f, const char* s)
{
    return f.Group.p != 0 && std::strcmp(f.Group.p, s) == 0 && f.GroupCRC == Calc(0, s, (unsigned long)std::strlen(s));
}
struct CIP {
    CThing* PItem;
    int     built;
    CIP() : PItem(0), built(1) {}
    CIP(CThing* t) : PItem(0), built(1) { PItem = t; }
};
template <class T> struct CVector { T* a; T* b; T* c; CVector() : a(0), b(0), c(0) {} };
template <class T> struct CDefPointer { T* Object; CDefPointer() : Object(0) {} };
struct C3DVector { float X, Y, Z; };
struct CModeStandBase {
    CThing* ParentThing;
    CModeStandBase(CThing* t) : ParentThing(t) { Log(1); }
};
// behaviour copy of NCreatureMode::CModeIntangible::CModeIntangible(CThing*, CCreatureActionBase*)
struct CCreatureActionBase { int id; };
struct SCountBlock { long m_references; void* m_pfnDestroy; void* m_pObject; SCountBlock(void* o) : m_references(1), m_pfnDestroy((void*)1), m_pObject(o) {} };
template <class T> struct CCountedPointer {
    T* m_pObject; SCountBlock* m_pCount;
    CCountedPointer(T* object) { m_pObject = object; if (object) m_pCount = new SCountBlock(m_pObject); else m_pCount = 0; }
};
struct CModeDragonBase : CModeStandBase {
    CAnimFilterOfGroup Filter; CIP PTarget;
    CModeDragonBase(CThing* thing, const CCharString& group) : CModeStandBase(thing), Filter(group) { }
};
struct CModeIntangible : CModeDragonBase {
    long Timer; long BossPhase; bool WasDamageable; CCountedPointer<CCreatureActionBase> PActionOnDeactivate;
    CModeIntangible(CThing* thing, CCreatureActionBase* action)
        : CModeDragonBase(thing, ""), Timer(-1), WasDamageable(true), PActionOnDeactivate(action) { }
};
int main()
{
    CThing t; CCreatureActionBase a; CModeIntangible m(&t, &a); CModeIntangible n(&t, 0);
    if (g_n != 2 || !FilterIs(m.Filter, "") || m.Timer != -1 || !m.WasDamageable || m.PActionOnDeactivate.m_pObject != &a
        || m.PActionOnDeactivate.m_pCount == 0 || m.PActionOnDeactivate.m_pCount->m_references != 1
        || m.PActionOnDeactivate.m_pCount->m_pObject != &a || n.PActionOnDeactivate.m_pObject != 0 || n.PActionOnDeactivate.m_pCount != 0)
    { std::printf("00741db0_TEST FAIL\n"); return 1; }
    delete m.PActionOnDeactivate.m_pCount;
    std::printf("00741db0_TEST PASS\n");
    return 0;
}