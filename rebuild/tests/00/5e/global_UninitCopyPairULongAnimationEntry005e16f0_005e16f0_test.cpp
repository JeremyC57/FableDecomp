#include <cstdio>
#include <new>

class CIVCountedPointeeBase { public:
    long m_refCount;
    CIVCountedPointeeBase() : m_refCount(0) { }
    virtual ~CIVCountedPointeeBase() { }
    virtual void Destroy() { }
};
template<class T> class CIVCountedPointer { public:
    T* m_pObject;
    CIVCountedPointer() : m_pObject(0) { }
    CIVCountedPointer(const CIVCountedPointer& other) : m_pObject(other.m_pObject)
    {
        if (m_pObject)
            ++m_pObject->m_refCount;
    }
};
class CDefString { public: long TablePos; };
class CAnimComponentMap : public CIVCountedPointeeBase { public: };
class CAnimationEntry { public:
    long BankIndex;
    CDefString AnimName;
    CDefString GroupName;
    CIVCountedPointer<CAnimComponentMap> Components;
};
struct CAnimPair {
    unsigned long first;
    CAnimationEntry second;
};
struct __false_type { __false_type() { } };

static CAnimPair* UninitCopy(const CAnimPair* first, const CAnimPair* last, CAnimPair* result, const __false_type&)
{
    CAnimPair* cur = result;
    for (; first != last; ++first, ++cur)
        new (cur) CAnimPair(*first);
    return cur;
}

int main()
{
    CAnimComponentMap maps[2];
    CAnimPair src[3];
    for (int i = 0; i < 3; ++i) {
        src[i].first = 0x100 + i;
        src[i].second.BankIndex = i;
        src[i].second.AnimName.TablePos = 10 + i;
        src[i].second.GroupName.TablePos = 20 + i;
        src[i].second.Components.m_pObject = i < 2 ? &maps[i] : 0;   // the last entry has no component map
    }
    maps[0].m_refCount = 1; maps[1].m_refCount = 1;
    static unsigned char storage[4 * sizeof(CAnimPair)];
    CAnimPair* dst = (CAnimPair*)storage;
    __false_type tag;
    CAnimPair* r = UninitCopy(src, src + 3, dst, tag);
    bool ok = r == dst + 3 && maps[0].m_refCount == 2 && maps[1].m_refCount == 2;
    for (int i = 0; i < 3; ++i)
        if (dst[i].first != 0x100u + i || dst[i].second.BankIndex != i || dst[i].second.AnimName.TablePos != 10 + i
            || dst[i].second.GroupName.TablePos != 20 + i || dst[i].second.Components.m_pObject != src[i].second.Components.m_pObject) ok = false;
    if (UninitCopy(src, src, dst + 3, tag) != dst + 3 || maps[0].m_refCount != 2) ok = false;
    if (!ok) { std::printf("005e16f0_TEST FAIL\n"); return 1; }
    std::printf("005e16f0_TEST PASS\n");
    return 0;
}