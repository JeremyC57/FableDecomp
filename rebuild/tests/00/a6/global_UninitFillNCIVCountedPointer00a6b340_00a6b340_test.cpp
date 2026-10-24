#include <cstdio>
#include <new>

// Model of 0x00A6B340: placement-copy `count` intrusive CIVCountedPointer handles (no allocator argument, `ret 4`).
struct CIVCountedPointeeBase {
    virtual ~CIVCountedPointeeBase() { }
    unsigned long IVRefCount;
    CIVCountedPointeeBase() : IVRefCount(1) { }
};
struct CIVCountedPointer {
    CIVCountedPointeeBase* Object;
    explicit CIVCountedPointer(CIVCountedPointeeBase* o) : Object(o) { }
    CIVCountedPointer(const CIVCountedPointer& other) : Object(other.Object)
    {
        if (Object)
            ++Object->IVRefCount;
    }
};

static CIVCountedPointer* UninitFillN(CIVCountedPointer* first, unsigned long count, const CIVCountedPointer& value)
{
    for (; 0 < count; --count, ++first)
        new (first) CIVCountedPointer(value);
    return first;
}

static double g_storage[64];

static int Fail()
{
    std::printf("00a6b340_TEST FAIL\n");
    return 1;
}

int main()
{
    CIVCountedPointeeBase object;
    CIVCountedPointer value(&object);
    CIVCountedPointer* buf = (CIVCountedPointer*)g_storage;
    if (UninitFillN(buf, 5, value) != buf + 5 || object.IVRefCount != 6)
        return Fail();
    for (int i = 0; i < 5; ++i)
        if (buf[i].Object != &object)
            return Fail();
    if (UninitFillN(buf, 0, value) != buf || object.IVRefCount != 6)
        return Fail();
    CIVCountedPointer null(0);
    if (UninitFillN(buf + 8, 2, null) != buf + 10 || buf[8].Object != 0 || buf[9].Object != 0)
        return Fail();
    std::printf("00a6b340_TEST PASS\n");
    return 0;
}