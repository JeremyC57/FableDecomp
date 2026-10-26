#include <cstdio>

static int g_destroyed = 0;
class CAnimComponentMap { public:
    virtual void v00() { }
    virtual void Destroy() { ++g_destroyed; m_dead = true; }
    long m_refCount;
    bool m_dead;
    CAnimComponentMap() : m_refCount(0), m_dead(false) { }
};
template <class T>
class CIVCountedPointer { public:
    T* Object;
    void Release()
    {
        T* object = Object;
        if (object) {
            if (--object->m_refCount == 0)
                object->Destroy();
            Object = 0;
        }
    }
    CIVCountedPointer& operator=(const CIVCountedPointer& other)
    {
        if (Object != other.Object) {
            Release();
            Object = other.Object;
            if (Object)
                ++Object->m_refCount;
        }
        return *this;
    }
};
class CDefString { public: long TablePos; };
class CAnimationEntry { public: long BankIndex; CDefString AnimName; CDefString GroupName; CIVCountedPointer<CAnimComponentMap> Components; };
struct SElem { unsigned long first; CAnimationEntry second; };
struct STag { STag() { } };

static SElem* CopyBackward(SElem* first, SElem* last, SElem* result, STag, long*)
{
    for (long n = last - first; n > 0; --n)
        *--result = *--last;
    return result;
}

int main()
{
    STag tag;
    CAnimComponentMap maps[4];
    SElem buf[4];
    for (int i = 0; i < 4; ++i) {
        buf[i].first = 100 + i; buf[i].second.BankIndex = i; buf[i].second.AnimName.TablePos = 10 + i; buf[i].second.GroupName.TablePos = 20 + i;
        buf[i].second.Components.Object = &maps[i]; maps[i].m_refCount = 1;
    }
    buf[2].second.Components.Object = &maps[1]; maps[1].m_refCount = 2; maps[2].m_refCount = 0;   // [1] and [2] share map 1
    // shift [0,3) up by one: buf[3] <- buf[2] (map1), buf[2] <- buf[1] (same map1: untouched), buf[1] <- buf[0] (map0)
    SElem* r = CopyBackward(buf, buf + 3, buf + 4, tag, 0);
    bool ok = r == buf + 1;
    if (buf[3].first != 102 || buf[3].second.BankIndex != 2 || buf[3].second.AnimName.TablePos != 12 || buf[3].second.GroupName.TablePos != 22) ok = false;
    if (buf[3].second.Components.Object != &maps[1] || buf[2].second.Components.Object != &maps[1] || buf[1].second.Components.Object != &maps[0]) ok = false;
    if (!maps[3].m_dead || g_destroyed != 1) ok = false;                 // map 3 lost its only reference
    if (maps[1].m_refCount != 2 || maps[0].m_refCount != 2) ok = false;  // map1: -1 (old buf[1]) +1 (buf[3]); map0: +1
    if (CopyBackward(buf, buf, buf + 2, tag, 0) != buf + 2) ok = false;
    if (!ok) { std::printf("005e0e70_TEST FAIL\n"); return 1; }
    std::printf("005e0e70_TEST PASS\n");
    return 0;
}