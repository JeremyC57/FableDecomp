#include <cstdio>
#include <new>

struct __false_type { };
struct CMeshDataBank { int m_id; };
struct CIVCountedPointeeBase {
    unsigned long IVRefCount;
    CIVCountedPointeeBase() : IVRefCount(1) { }
    virtual void Destroy(int) { }
    virtual ~CIVCountedPointeeBase() { }
};
template <class T>
struct CIVCountedPointer {
    CIVCountedPointeeBase* Object;
    CIVCountedPointer() : Object(0) { }
    CIVCountedPointer(const CIVCountedPointer& other) : Object(other.Object) { if (Object) ++Object->IVRefCount; }
};
struct C3DVector {
    float X, Y, Z;
    C3DVector() : X(0), Y(0), Z(0) { }
    C3DVector(const C3DVector& v) : X(v.X), Y(v.Y), Z(v.Z) { }
};
struct CMeshDataBankEntry {
    CMeshDataBank* Bank;
    unsigned long  Index;
    CMeshDataBankEntry() : Bank(0), Index(0) { }
    virtual ~CMeshDataBankEntry() { }
    virtual int Kind() const { return 1; }
};
struct C3DAnimationInfo : CMeshDataBankEntry {
    struct CAnimationData;
    CIVCountedPointer<CAnimationData> Data;
    float     Duration;
    float     NonLoopingDuration;
    C3DVector MovementVector;
    float     Rotation;
    C3DAnimationInfo() : Duration(0), NonLoopingDuration(0), Rotation(0) { }
    virtual int Kind() const { return 2; }
};
static CMeshDataBank g_bank;
static CIVCountedPointeeBase g_data[3];

typedef C3DAnimationInfo T_;

static T_* UninitCopy(const T_* first, const T_* last, T_* result, const __false_type&)
{
    for (; first != last; ++first, ++result)
        new (result) T_(*first);
    return result;
}

int main()
{
    bool ok = true;
    T_* src = (T_*)new double[(3 * sizeof(T_)) / sizeof(double) + 1];
    for (int i = 0; i < 3; ++i) {
        T_* s = new (&src[i]) T_();
        s->Bank = &g_bank; s->Index = 7 + i; s->Data.Object = (i == 1) ? 0 : &g_data[i];
        s->Duration = 1.5f * i; s->NonLoopingDuration = 2.0f + i; s->MovementVector.X = 3.0f; s->MovementVector.Y = -1.0f * i;
        s->MovementVector.Z = 0.25f; s->Rotation = 9.0f - i;
    }
    static double storage[64];
    T_* dst = (T_*)storage;
    __false_type tag;
    T_* r = UninitCopy(src, src + 3, dst, tag);
    if (r != dst + 3) ok = false;
    for (int i = 0; i < 3; ++i) {
        const T_* s = &src[i];
        const T_* d = &dst[i];
        if (d->Kind() != 2 || d->Bank != &g_bank || d->Index != (unsigned long)(7 + i) || d->Data.Object != s->Data.Object) ok = false;
        if (d->Duration != s->Duration || d->NonLoopingDuration != s->NonLoopingDuration || d->Rotation != s->Rotation) ok = false;
        if (d->MovementVector.X != 3.0f || d->MovementVector.Y != -1.0f * i || d->MovementVector.Z != 0.25f) ok = false;
        if (i != 1 && g_data[i].IVRefCount != 2) ok = false;
    }
    if (UninitCopy(src, src, dst + 1, tag) != dst + 1) ok = false;
    if (!ok) { std::printf("00a2af20_TEST FAIL\n"); return 1; }
    std::printf("00a2af20_TEST PASS\n");
    return 0;
}