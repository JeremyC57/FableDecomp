#include <cstdio>
class CEnginePrimitiveHandle;
static int g_last = 0, g_destroy = 0;
class CPrim {
public:
    long RefCount;
    CPrim() : RefCount(0) { }
    virtual ~CPrim() { }
    virtual void Destroy() { ++g_destroy; }
    virtual void OnLastHandleReleased(CEnginePrimitiveHandle*) { ++g_last; }
    void Release() { if (--RefCount == 0) Destroy(); }
};
class CIVCountedPointer {
public:
    CPrim* Object;
    CIVCountedPointer() : Object(0) { }
    CPrim* operator->() const { return Object; }
    bool IsUnique() const { return Object != 0 && Object->RefCount == 1; }
    CIVCountedPointer& operator=(int) { if (Object) { Object->Release(); Object = 0; } Object = 0; return *this; }
    CIVCountedPointer& operator=(const CIVCountedPointer& other)
    {
        CPrim* old = Object;
        if (old != other.Object) {
            if (old) { old->Release(); Object = 0; }
            Object = other.Object;
            if (Object) ++Object->RefCount;
        }
        return *this;
    }
};
class CEnginePrimitiveHandle {
public:
    CIVCountedPointer Data;
    void Clear() { if (Data.IsUnique()) Data->OnLastHandleReleased(this); Data = 0; }
    CEnginePrimitiveHandle& operator=(const CEnginePrimitiveHandle& other) { Clear(); Data = other.Data; return *this; }
};
int main()
{
    CPrim a, b; a.RefCount = 1; b.RefCount = 1;
    CEnginePrimitiveHandle h1, h2; h1.Data.Object = &a; h2.Data.Object = &b;
    CEnginePrimitiveHandle& r = (h1 = h2);       // a unique: notified + destroyed; b shared
    if (&r != &h1 || h1.Data.Object != &b || b.RefCount != 2 || g_last != 1 || g_destroy != 1) { std::printf("0043f9b0_TEST FAIL\n"); return 1; }
    h2 = h1;                                     // same primitive: count 2 -> 1 (not unique before) -> re-adopted
    if (h2.Data.Object != &b || b.RefCount != 2 || g_last != 1) { std::printf("0043f9b0_TEST FAIL\n"); return 1; }
    std::printf("0043f9b0_TEST PASS\n");
    return 0;
}
