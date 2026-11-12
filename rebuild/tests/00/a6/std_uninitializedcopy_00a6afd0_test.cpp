// Behaviour test for 00a6afd0: placement-copy 4-byte CIVCountedPointer handles, bumping the intrusive count of each non-null pointee
#include <cstdio>
#include <new>

class CIVCountedPointeeBase { public:
    virtual void Destroy(int flags) { (void)flags; }
    virtual void Delete() { }
    unsigned int IVRefCount;
    CIVCountedPointeeBase() : IVRefCount(0) { }
    virtual ~CIVCountedPointeeBase() { }
};

struct CIVCountedPointer {
    CIVCountedPointeeBase* Object;
    CIVCountedPointer() : Object(0) { }
    CIVCountedPointer(const CIVCountedPointer& other) : Object(other.Object)
    {
        if (Object)
            ++Object->IVRefCount;
    }
};

struct FalseType { FalseType() { } };

CIVCountedPointer* UninitCopy(const CIVCountedPointer* first, const CIVCountedPointer* last, CIVCountedPointer* result, const FalseType&)
{
    for (; first != last; ++first, ++result)
        new (result) CIVCountedPointer(*first);
    return result;
}

int main()
{
    CIVCountedPointeeBase objs[2];
    objs[0].IVRefCount = 1;
    objs[1].IVRefCount = 4;
    CIVCountedPointer src[4];
    src[0].Object = &objs[0];
    src[1].Object = 0;
    src[2].Object = &objs[1];
    src[3].Object = &objs[0];
    void* raw = ::operator new(4 * sizeof(CIVCountedPointer));
    CIVCountedPointer* dst = static_cast<CIVCountedPointer*>(raw);
    CIVCountedPointer* end = UninitCopy(src, src + 4, dst, FalseType());
    bool ok = end == dst + 4;
    for (int i = 0; i < 4; ++i)
        ok = ok && dst[i].Object == src[i].Object;
    ok = ok && objs[0].IVRefCount == 3 && objs[1].IVRefCount == 5;
    ok = ok && UninitCopy(src, src, dst, FalseType()) == dst;
    ::operator delete(raw);
    if (!ok) { std::printf("00a6afd0_TEST FAIL\n"); return 1; }
    std::printf("00a6afd0_TEST PASS\n");
    return 0;
}