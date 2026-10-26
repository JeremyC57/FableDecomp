// Behaviour test for 005e3c60: body copied from src/005e3c60.cpp (vector<pair<unsigned long, CAnimationEntry> >::resize(n)),
// the two-argument resize stubbed to record the default value it receives and optionally hand back a component map.
#include <cstdio>
#include <cstring>
static int fails = 0;
#define CHECK(c) do { if (!(c)) { ++fails; printf("FAIL %d: %s\n", __LINE__, #c); } } while (0)
static __forceinline bool DecRefToZero(long& count) { return --count == 0; }
struct CRefCounted { long m_count; };
static int g_destroyed = 0;
struct CCountedVirtual {
    virtual void v00() {}
    virtual void Destroy() { ++g_destroyed; }
};
class CAnimComponentMap : public CCountedVirtual, public CRefCounted { public:
    void Release()
    {
        if (DecRefToZero(m_count))
            Destroy();
    }
};
class CDefString { public:
    long TablePos;
    CDefString() : TablePos(-1) { }
};
struct CIVCountedPointer_AnimComponentMap {
    CAnimComponentMap* Object;
    CIVCountedPointer_AnimComponentMap() : Object(0) { }
    ~CIVCountedPointer_AnimComponentMap()
    {
        CAnimComponentMap* object = Object;
        if (object)
            object->Release();
    }
};
class CAnimationEntry { public:
    long                               BankIndex;
    CDefString                         AnimName;
    CDefString                         GroupName;
    CIVCountedPointer_AnimComponentMap Components;
    CAnimationEntry() { }
};
struct SAnimationPair {
    unsigned long   first;
    CAnimationEntry second;
    SAnimationPair() : first(0), second() { }
};
static unsigned long g_n = 0; static unsigned long g_first = 99; static long g_anim = 0, g_group = 0; static void* g_comp = (void*)1;
static int g_calls = 0; static CAnimComponentMap* g_handBack = 0;
struct CAnimationPairVector {
    SAnimationPair* First;
    SAnimationPair* Last;
    SAnimationPair* End;
    void resize(unsigned long n, const SAnimationPair& value)
    {
        ++g_calls; g_n = n; g_first = value.first; g_anim = value.second.AnimName.TablePos;
        g_group = value.second.GroupName.TablePos; g_comp = value.second.Components.Object;
        if (g_handBack) const_cast<SAnimationPair&>(value).second.Components.Object = g_handBack;
    }
    void resize(unsigned long n);
};
void CAnimationPairVector::resize(unsigned long n)
{
    resize(n, SAnimationPair());
}
int main()
{
    CAnimationPairVector v; memset(&v, 0, sizeof(v));
    v.resize(7);
    CHECK(g_calls == 1); CHECK(g_n == 7); CHECK(g_first == 0); CHECK(g_anim == -1); CHECK(g_group == -1); CHECK(g_comp == 0);
    CHECK(g_destroyed == 0);
    CAnimComponentMap m; m.m_count = 2; g_handBack = &m;
    v.resize(3);
    CHECK(g_calls == 2); CHECK(g_n == 3); CHECK(m.m_count == 1); CHECK(g_destroyed == 0);
    v.resize(4);
    CHECK(m.m_count == 0); CHECK(g_destroyed == 1);
    if (fails == 0) printf("005e3c60_TEST PASS\n"); else printf("005e3c60_TEST FAIL\n");
    return fails;
}