// Behaviour test for __uninitialized_fill_n<C3DAnimationInfo*> (0x00A2AFA0): n copies of the prototype are built in raw
// storage (same Bank/Index/timings/vector/rotation, each copy addrefs the shared animation data), the returned pointer
// is first + n, and n == 0 constructs nothing.
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <new>

class CMeshDataBank { public: int Id; };

class CMeshDataBankEntry {
public:
    virtual ~CMeshDataBankEntry() {}
    virtual int Kind() const { return 0; }
    CMeshDataBank* Bank;
    unsigned long  Index;
};

class CIVCountedPointeeBase {
public:
    virtual ~CIVCountedPointeeBase() {}
    unsigned long IVRefCount;
};

template <class T> class CIVCountedPointer {
public:
    CIVCountedPointeeBase* Object;
    CIVCountedPointer() : Object(0) {}
    CIVCountedPointer(const CIVCountedPointer& other) : Object(other.Object)
    {
        if (Object)
            ++Object->IVRefCount;
    }
};

class C3DVector {
public:
    float X, Y, Z;
    C3DVector() : X(0), Y(0), Z(0) {}
    C3DVector(const C3DVector& other) : X(other.X), Y(other.Y), Z(other.Z) {}
};

class C3DAnimationInfo : public CMeshDataBankEntry {
public:
    class CAnimationData;
    CIVCountedPointer<CAnimationData> Data;
    float     Duration;
    float     NonLoopingDuration;
    C3DVector MovementVector;
    float     Rotation;
    C3DAnimationInfo() : Duration(0), NonLoopingDuration(0), Rotation(0) { Bank = 0; Index = 0; }
    virtual int Kind() const { return 2; }
    C3DAnimationInfo(const C3DAnimationInfo& other)
        : CMeshDataBankEntry(other), Data(other.Data), Duration(other.Duration), NonLoopingDuration(other.NonLoopingDuration),
          MovementVector(other.MovementVector), Rotation(other.Rotation)
    {
    }
};

namespace _STL {
struct __false_type { __false_type() {} };
template <class _T1, class _T2>
inline void _Construct(_T1* __p, const _T2& __val) { new (__p) _T1(__val); }
template <class _ForwardIter, class _Size, class _Tp>
_ForwardIter __uninitialized_fill_n(_ForwardIter __first, _Size __n, const _Tp& __x, const __false_type&)
{
    _ForwardIter __cur = __first;
    for ( ; __n > 0; --__n, ++__cur)
        _Construct(&*__cur, __x);
    return __cur;
}
}

static int nearf(float a, float b) { float d = a - b; return d < 0.0001f && d > -0.0001f; }

int main()
{
    int ok = 1;
    CMeshDataBank bank; bank.Id = 7;
    CIVCountedPointeeBase data; data.IVRefCount = 1;
    C3DAnimationInfo proto;
    proto.Bank = &bank; proto.Index = 42; proto.Data.Object = &data;
    proto.Duration = 1.5f; proto.NonLoopingDuration = 0.75f; proto.Rotation = 3.0f;
    proto.MovementVector.X = 1.0f; proto.MovementVector.Y = -2.0f; proto.MovementVector.Z = 4.0f;

    void* raw = std::malloc(sizeof(C3DAnimationInfo) * 4);
    std::memset(raw, 0xCD, sizeof(C3DAnimationInfo) * 4);
    C3DAnimationInfo* first = (C3DAnimationInfo*)raw;
    C3DAnimationInfo* end = _STL::__uninitialized_fill_n(first, 3u, proto, _STL::__false_type());
    if (end != first + 3) ok = 0;
    for (int i = 0; i < 3; ++i) {
        C3DAnimationInfo& a = first[i];
        if (a.Kind() != 2 || a.Bank != &bank || a.Index != 42 || a.Data.Object != &data) ok = 0;
        if (!nearf(a.Duration, 1.5f) || !nearf(a.NonLoopingDuration, 0.75f) || !nearf(a.Rotation, 3.0f)) ok = 0;
        if (!nearf(a.MovementVector.X, 1.0f) || !nearf(a.MovementVector.Y, -2.0f) || !nearf(a.MovementVector.Z, 4.0f)) ok = 0;
    }
    if (data.IVRefCount != 4) ok = 0;
    unsigned char* tail = (unsigned char*)(first + 3);
    if (tail[0] != 0xCD || tail[sizeof(C3DAnimationInfo) - 1] != 0xCD) ok = 0;
    C3DAnimationInfo* none = _STL::__uninitialized_fill_n(first + 3, 0u, proto, _STL::__false_type());
    if (none != first + 3 || data.IVRefCount != 4 || tail[0] != 0xCD) ok = 0;
    std::free(raw);

    if (!ok) { std::printf("00a2afa0_TEST FAIL\n"); return 1; }
    std::printf("00a2afa0_TEST PASS\n");
    return 0;
}