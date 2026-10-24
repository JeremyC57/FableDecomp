// Behaviour test for 0x00440180 UninitializedFillN_CEnginePrimitiveHandle_00440180: three-argument uninitialized_fill_n
// copy-constructing n primitive handles (vtable + add-ref of the shared primitive).
#include <cstdio>
#include <new>

class CBaseClass {
public:
    CBaseClass() { }
    virtual ~CBaseClass() { }
};
class CEnginePersistentPrimitiveBase {
public:
    CEnginePersistentPrimitiveBase() : RefCount(1) { }
    virtual void v000() { }
    long RefCount;
};
class CIVCountedPointer {
public:
    CEnginePersistentPrimitiveBase* P;
    CIVCountedPointer() : P(0) { }
    CIVCountedPointer(const CIVCountedPointer& x) : P(x.P) { if (P) ++P->RefCount; }
};
class CEnginePrimitiveHandle : public CBaseClass {
public:
    CIVCountedPointer Data;
    CEnginePrimitiveHandle() { }
    CEnginePrimitiveHandle(const CEnginePrimitiveHandle& x) : CBaseClass(x), Data(x.Data) { }
    virtual int Kind() const { return 3; }
};
struct __false_type { __false_type() { } };

static CEnginePrimitiveHandle* __uninitialized_fill_n(CEnginePrimitiveHandle* first, unsigned int n, const CEnginePrimitiveHandle& x, const __false_type&)
{
    CEnginePrimitiveHandle* cur = first;
    for (; n > 0; --n, ++cur)
        new (cur) CEnginePrimitiveHandle(x);
    return cur;
}
static CEnginePrimitiveHandle* UninitializedFillN(CEnginePrimitiveHandle* first, unsigned int n, const CEnginePrimitiveHandle& x)
{
    return __uninitialized_fill_n(first, n, x, __false_type());
}

int main()
{
    CEnginePersistentPrimitiveBase primitive;
    CEnginePrimitiveHandle x;
    x.Data.P = &primitive;
    static double storage[5 * sizeof(CEnginePrimitiveHandle) / sizeof(double) + 1];
    CEnginePrimitiveHandle* dst = (CEnginePrimitiveHandle*)storage;
    CEnginePrimitiveHandle* r = UninitializedFillN(dst, 5, x);
    bool ok = r == dst + 5 && primitive.RefCount == 6;
    for (int k = 0; k < 5; ++k)
        if (dst[k].Data.P != &primitive || dst[k].Kind() != 3) ok = false;
    CEnginePrimitiveHandle empty;
    if (UninitializedFillN(dst, 2, empty) != dst + 2 || dst[0].Data.P != 0 || primitive.RefCount != 6) ok = false;
    if (UninitializedFillN(dst, 0, x) != dst || primitive.RefCount != 6) ok = false;
    if (!ok) { std::printf("00440180_TEST FAIL\n"); return 1; }
    std::printf("00440180_TEST PASS\n");
    return 0;
}