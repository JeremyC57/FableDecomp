// Behaviour test for CAnimationEntry::operator= @ 0x00471a09: dwords copied, old map released (Destroy at zero), new map addref'd; self-same map untouched.
#include <cstdio>
static int g_destroyed = 0;
struct CIVCountedPointeeBase { virtual void v0() { } virtual void Destroy() { ++g_destroyed; } long m_refCount; };
template<class T> struct CIVCountedPointer {
    T* m_pObject;
    void Release() { T* p = m_pObject; if (p) { if (--p->m_refCount == 0) p->Destroy(); m_pObject = 0; } }
    CIVCountedPointer& operator=(const CIVCountedPointer& other)
    { if (m_pObject != other.m_pObject) { Release(); T* p = other.m_pObject; m_pObject = p; if (p) ++p->m_refCount; } return *this; }
};
struct CDefString { long TablePos; };
struct CAnimComponentMap : CIVCountedPointeeBase { };
struct CAnimationEntry {
    long BankIndex; CDefString AnimName; CDefString GroupName; CIVCountedPointer<CAnimComponentMap> Components;
    CAnimationEntry& operator=(const CAnimationEntry& o) { BankIndex = o.BankIndex; AnimName = o.AnimName; GroupName = o.GroupName; Components = o.Components; return *this; }
};
int main()
{
    CAnimComponentMap a, b; a.m_refCount = 1; b.m_refCount = 1;
    CAnimationEntry x, y;
    x.BankIndex = 1; x.AnimName.TablePos = 2; x.GroupName.TablePos = 3; x.Components.m_pObject = &a;
    y.BankIndex = 10; y.AnimName.TablePos = 20; y.GroupName.TablePos = 30; y.Components.m_pObject = &b;
    x = y;
    bool ok = x.BankIndex == 10 && x.AnimName.TablePos == 20 && x.GroupName.TablePos == 30 && x.Components.m_pObject == &b
        && b.m_refCount == 2 && a.m_refCount == 0 && g_destroyed == 1;
    x = y;                                                                   // same map: no count change
    if (!(b.m_refCount == 2 && g_destroyed == 1)) ok = false;
    y.Components.m_pObject = 0; x = y;                                       // to null: release, no addref
    if (!(x.Components.m_pObject == 0 && b.m_refCount == 1)) ok = false;
    if (!ok) { std::printf("00471a09_TEST FAIL\n"); return 1; }
    std::printf("00471a09_TEST PASS\n");
    return 0;
}
