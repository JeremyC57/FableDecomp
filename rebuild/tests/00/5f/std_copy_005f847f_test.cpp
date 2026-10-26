// Behaviour test for 0x005F847F std::__copy(CDefPointer<const CHeroAbilityDef>*, ..., random_access_iterator_tag, int*):
// counted def-pointer assignment per element (release the old def, add-ref the new one, self-assignment skipped).
#include <cstdio>

static int g_destroyed = 0;

class CDefPointeeBase { public:
    virtual void v00() { }
    virtual void Destroy() const { ++g_destroyed; }
    mutable long m_refCount;
    CDefPointeeBase() : m_refCount(0) { }
    virtual ~CDefPointeeBase() { }
    void Release() const { if (--m_refCount == 0) Destroy(); }
};

class CHeroAbilityDef : public CDefPointeeBase { public: };

template <class T> class CDefPointer { public:
    T* Object;
    CDefPointer& operator=(const CDefPointer& other)
    {
        if (Object != other.Object) {
            if (Object) {
                Object->Release();
                Object = 0;
            }
            Object = other.Object;
            if (Object)
                ++Object->m_refCount;
        }
        return *this;
    }
};

namespace stl {
struct random_access_iterator_tag { random_access_iterator_tag() { } };

template <class RandomAccessIter, class OutputIter, class Distance>
OutputIter __copy(RandomAccessIter first, RandomAccessIter last, OutputIter result,
                  const random_access_iterator_tag&, Distance*)
{
    for (Distance n = last - first; n > 0; --n) {
        *result = *first;
        ++first;
        ++result;
    }
    return result;
}
}

typedef CDefPointer<const CHeroAbilityDef> P;

int main()
{
    CHeroAbilityDef a, b, c, old;
    a.m_refCount = 1; b.m_refCount = 1; c.m_refCount = 1; old.m_refCount = 1;
    P src[3];
    src[0].Object = &a; src[1].Object = &b; src[2].Object = 0;
    P dst[3];
    dst[0].Object = &old;   // released to zero -> Destroy
    dst[1].Object = &b;     // self-assignment: untouched
    dst[2].Object = &c;     // replaced by null: c released
    P* end = stl::__copy(src, src + 3, dst, stl::random_access_iterator_tag(), (int*)0);
    int ok = end == dst + 3 && dst[0].Object == &a && dst[1].Object == &b && dst[2].Object == 0 &&
             a.m_refCount == 2 && b.m_refCount == 1 && c.m_refCount == 0 && old.m_refCount == 0 && g_destroyed == 2;
    if (!ok) { std::printf("005f847f_TEST FAIL\n"); return 1; }
    std::printf("005f847f_TEST PASS\n");
    return 0;
}