// Behaviour test for 0x007691A0 STLport fill over CIVCountedPointer<T>: every slot ends up sharing
// the value's pointee, released pointees are deleted at refcount zero, refcounts stay balanced.
#include <cstdio>

static int gDeleted = 0;

struct CIVCountedPointeeBase {
    virtual void Destroy(int) { }
    virtual void Delete() { ++gDeleted; }
    unsigned long IVRefCount;
    CIVCountedPointeeBase() : IVRefCount(0) { }
    virtual ~CIVCountedPointeeBase() { }
};

template <class T> class CIVCountedPointer {
public:
    T* Object;
    void Release()
    {
        T* object = Object;
        if (object) {
            if (--object->IVRefCount == 0)
                object->Delete();
            Object = 0;
        }
    }
    CIVCountedPointer& operator=(const CIVCountedPointer& other)
    {
        if (Object != other.Object) {
            Release();
            Object = other.Object;
            if (Object)
                ++Object->IVRefCount;
        }
        return *this;
    }
};

template <class _ForwardIter, class _Tp>
void fill_model(_ForwardIter __first, _ForwardIter __last, const _Tp& __val)
{
    for ( ; __first != __last; ++__first)
        *__first = __val;
}

int main()
{
    CIVCountedPointeeBase a, b, c;
    CIVCountedPointer<CIVCountedPointeeBase> v[4];
    v[0].Object = &a; a.IVRefCount = 1;          // sole owner: deleted when overwritten
    v[1].Object = &b; b.IVRefCount = 2;          // shared with v[2]
    v[2].Object = &b;
    v[3].Object = 0;
    CIVCountedPointer<CIVCountedPointeeBase> val; val.Object = &c; c.IVRefCount = 1;
    fill_model(v, v + 4, val);
    int ok = 1;
    for (int i = 0; i < 4; ++i) if (v[i].Object != &c) ok = 0;
    if (c.IVRefCount != 5) ok = 0;
    if (a.IVRefCount != 0 || b.IVRefCount != 0) ok = 0;
    if (gDeleted != 2) ok = 0;
    // self-fill: same pointee, no refcount change
    fill_model(v, v + 4, val);
    if (c.IVRefCount != 5 || gDeleted != 2) ok = 0;
    // empty range
    fill_model(v, v, val);
    if (c.IVRefCount != 5) ok = 0;
    if (!ok) { std::printf("007691a0_TEST FAIL\n"); return 1; }
    std::printf("007691a0_TEST PASS\n");
    return 0;
}