// Behaviour test for std::pair<unsigned long, CAnimationEntry>::operator= @ 0x005e0e20: key and entry copied with counted-pointer semantics.
#include <cstdio>
#include <cstddef>
static int g_destroyed = 0;
class CIVCountedPointeeBase { public: virtual void v0() { } virtual void Destroy() { ++g_destroyed; } long m_refCount; };
template<class T> class CIVCountedPointer { public:
    T* m_pObject;
    void Release() { T* p = m_pObject; if (p) { if (--p->m_refCount == 0) p->Destroy(); m_pObject = 0; } }
    CIVCountedPointer& operator=(const CIVCountedPointer& other)
    { if (m_pObject != other.m_pObject) { Release(); T* p = other.m_pObject; m_pObject = p; if (p) ++p->m_refCount; } return *this; }
};
class CDefString { public: long TablePos; };
class CAnimComponentMap : public CIVCountedPointeeBase { public: };
class CAnimationEntry { public:
    long BankIndex; CDefString AnimName; CDefString GroupName; CIVCountedPointer<CAnimComponentMap> Components;
    CAnimationEntry& operator=(const CAnimationEntry& o) { BankIndex = o.BankIndex; AnimName = o.AnimName; GroupName = o.GroupName; Components = o.Components; return *this; }
};
namespace std {
template<class A, class B> struct pair { A first; B second; pair& operator=(const pair& o) { first = o.first; second = o.second; return *this; } };
}
int main()
{
    CAnimComponentMap a, b; a.m_refCount = 1; b.m_refCount = 1;
    std::pair<unsigned long, CAnimationEntry> x, y;
    x.first = 5; x.second.BankIndex = 1; x.second.AnimName.TablePos = 2; x.second.GroupName.TablePos = 3; x.second.Components.m_pObject = &a;
    y.first = 50; y.second.BankIndex = 10; y.second.AnimName.TablePos = 20; y.second.GroupName.TablePos = 30; y.second.Components.m_pObject = &b;
    x = y;
    bool ok = x.first == 50 && x.second.BankIndex == 10 && x.second.AnimName.TablePos == 20 && x.second.GroupName.TablePos == 30
        && x.second.Components.m_pObject == &b && b.m_refCount == 2 && a.m_refCount == 0 && g_destroyed == 1
        && sizeof(x) == 0x14 && offsetof(CAnimationEntry, Components) == 0x0c;
    if (!ok) { std::printf("005e0e20_TEST FAIL\n"); return 1; }
    std::printf("005e0e20_TEST PASS\n");
    return 0;
}