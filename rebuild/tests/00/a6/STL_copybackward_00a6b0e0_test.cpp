// Behaviour test for 0x00A6B0E0: __copy_backward over CIVCountedPointer<CLocalDetailPlacementGrid>
#include <cstdio>
#include <cstdlib>
#include <new>

static int gDeletes = 0;

class CIVCountedPointeeBase {
public:
    virtual ~CIVCountedPointeeBase() { }
    virtual void Delete() { ++gDeletes; }
    unsigned int IVRefCount;
    CIVCountedPointeeBase() : IVRefCount(0) { }
    void RemoveRef()
    {
        if (--IVRefCount == 0)
            Delete();
    }
};

class CLocalDetailPlacementGrid : public CIVCountedPointeeBase {
};

template <class T> class CIVCountedPointer {
public:
    T* Object;
    CIVCountedPointer() : Object(0) { }
    CIVCountedPointer& operator=(const CIVCountedPointer& other)
    {
        if (Object != other.Object) {
            if (Object) {
                Object->RemoveRef();
                Object = 0;
            }
            Object = other.Object;
            if (Object)
                ++Object->IVRefCount;
        }
        return *this;
    }
};

namespace std {
class random_access_iterator_tag { public: random_access_iterator_tag() { } };

template <class _RandomAccessIter, class _BidirectionalIter, class _Distance>
_BidirectionalIter __copy_backward(_RandomAccessIter __first, _RandomAccessIter __last, _BidirectionalIter __result,
                                   const random_access_iterator_tag&, _Distance*)
{
    for (_Distance __n = __last - __first; __n > 0; --__n)
        *--__result = *--__last;
    return __result;
}
}

typedef CIVCountedPointer<CLocalDetailPlacementGrid> TGridPtr;

int main()
{
    int ok = 1;
    CLocalDetailPlacementGrid g[4];
    TGridPtr a[4];
    for (int i = 0; i < 4; ++i) {
        TGridPtr p;
        p.Object = &g[i];
        a[i] = p;                        // count 1 each (the temporary's own reference is not counted)
    }
    TGridPtr* r = std::__copy_backward(a, a + 3, a + 4, std::random_access_iterator_tag(), (int*)0);
    if (r != a + 1 || a[0].Object != &g[0] || a[1].Object != &g[0] || a[2].Object != &g[1] || a[3].Object != &g[2])
        ok = 0;
    if (g[0].IVRefCount != 2 || g[1].IVRefCount != 1 || g[2].IVRefCount != 1 || g[3].IVRefCount != 0 || gDeletes != 1)
        ok = 0;

    if (!ok) {
        std::printf("00a6b0e0_TEST FAIL\n");
        return 1;
    }
    std::printf("00a6b0e0_TEST PASS\n");
    return 0;
}