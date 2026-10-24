// Behaviour test for vector<C3DAnimationInfo>::reserve (0x00A2C360): reserving on an empty vector allocates exactly n
// elements and keeps size 0 (reserve(0) on an empty vector allocates nothing); reserving above capacity copies the existing elements into a new n-element block and frees
// the old one; reserving at or below capacity changes nothing. The animation infos keep bank/index, timings, movement and their animation data reference count.
#include <cstdio>
#include <cstdlib>
#include <new>

static int g_allocs = 0, g_frees = 0;
static unsigned int g_lastBytes = 0;
static int nearf(float a, float b) { float d = a - b; return d < 0.0001f && d > -0.0001f; }

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
    CIVCountedPointer(const CIVCountedPointer& other) : Object(other.Object) { if (Object) ++Object->IVRefCount; }
    ~CIVCountedPointer() { if (Object) --Object->IVRefCount; }
};
class C3DVector { public: float X, Y, Z; };
class C3DAnimationInfo : public CMeshDataBankEntry {
public:
    class CAnimationData;
    virtual int Kind() const { return 2; }
    CIVCountedPointer<CAnimationData> Data;
    float     Duration;
    float     NonLoopingDuration;
    C3DVector MovementVector;
    float     Rotation;
};
static CMeshDataBank g_bank;
static CIVCountedPointeeBase g_data;

namespace _STL {
struct __false_type { __false_type() {} };
template <class _T1, class _T2>
inline void _Construct(_T1* __p, const _T2& __val) { new (__p) _T1(__val); }
template <class _InputIter, class _ForwardIter>
_ForwardIter __uninitialized_copy(_InputIter __first, _InputIter __last, _ForwardIter __result, const __false_type&)
{
    _ForwardIter __cur = __result;
    for ( ; __first != __last; ++__first, ++__cur)
        _Construct(&*__cur, *__first);
    return __cur;
}
template <class _Tp> class allocator {
public:
    allocator() {}
    _Tp* allocate(unsigned int __n) { ++g_allocs; g_lastBytes = __n * sizeof(_Tp); return __n != 0 ? (_Tp*)std::malloc(__n * sizeof(_Tp)) : 0; }
    void deallocate(_Tp* __p, unsigned int) { if (__p) { ++g_frees; std::free(__p); } }
};
template <class _Tp, class _Alloc> class _STLP_alloc_proxy : public _Alloc { public: _Tp _M_data; };
template <class _Tp, class _Alloc = allocator<_Tp> > class vector {
public:
    typedef _Tp* pointer;
    typedef const _Tp* const_pointer;
    typedef unsigned int size_type;
    _Tp* _M_start;
    _Tp* _M_finish;
    _STLP_alloc_proxy<_Tp*, _Alloc> _M_end_of_storage;
    vector() : _M_start(0), _M_finish(0) { _M_end_of_storage._M_data = 0; }
    size_type size() const { return size_type(this->_M_finish - this->_M_start); }
    size_type capacity() const { return size_type(this->_M_end_of_storage._M_data - this->_M_start); }
    pointer _M_allocate_and_copy(size_type __n, const_pointer __first, const_pointer __last)
    {
        pointer __result = this->_M_end_of_storage.allocate(__n);
        __uninitialized_copy(__first, __last, __result, __false_type());
        return __result;
    }
    void _M_clear()
    {
        for (pointer p = this->_M_start; p != this->_M_finish; ++p) p->~_Tp();
        this->_M_end_of_storage.deallocate(this->_M_start, this->_M_end_of_storage._M_data - this->_M_start);
    }
    void _M_set(pointer __s, pointer __f, pointer __e)
    {
        this->_M_start = __s;
        this->_M_finish = __f;
        this->_M_end_of_storage._M_data = __e;
    }
    void push_back(const _Tp& x)
    {
        if (_M_finish == _M_end_of_storage._M_data) reserve(size() ? size() * 2 : 1);
        _Construct(_M_finish, x); ++_M_finish;
    }
    void reserve(size_type __n);
};
template <class _Tp, class _Alloc>
void vector<_Tp, _Alloc>::reserve(size_type __n)
{
    if (capacity() < __n) {
        const size_type __old_size = size();
        pointer __tmp;
        if (this->_M_start) {
            __tmp = _M_allocate_and_copy(__n, this->_M_start, this->_M_finish);
            _M_clear();
        } else {
            __tmp = this->_M_end_of_storage.allocate(__n);
        }
        _M_set(__tmp, __tmp + __old_size, __tmp + __n);
    }
}
}

int main()
{
    int ok = 1;
    if (sizeof(void*) == 4u && sizeof(C3DAnimationInfo) != 40u) ok = 0;
    _STL::vector<C3DAnimationInfo> v;
    v.reserve(0);
    if (v._M_start != 0 || v.capacity() != 0 || g_allocs != 0) ok = 0;
    v.reserve(2);
    if (v.size() != 0 || v.capacity() != 2 || g_allocs != 1 || g_lastBytes != 2u * sizeof(C3DAnimationInfo) || g_frees != 0) ok = 0;
    for (int i = 0; i < 2; ++i) { C3DAnimationInfo e; e.Bank = &g_bank; e.Index = 30 + i; e.Data.Object = &g_data; ++g_data.IVRefCount; e.Duration = 1.5f * (i + 1); e.MovementVector.Z = -4.0f; e.Rotation = 0.25f; v.push_back(e); }
    C3DAnimationInfo* oldStart = v._M_start;
    int allocs = g_allocs;
    v.reserve(2);
    v.reserve(1);
    if (g_allocs != allocs || v._M_start != oldStart || v.capacity() != 2) ok = 0;
    v.reserve(5);
    if (g_allocs != allocs + 1 || g_frees != 1 || v.capacity() != 5 || v.size() != 2 || g_lastBytes != 5u * sizeof(C3DAnimationInfo)) ok = 0;
    for (int i = 0; i < 2; ++i) { const C3DAnimationInfo& e = v._M_start[i]; if (!(e.Kind() == 2 && e.Bank == &g_bank && e.Index == 30u + i && e.Data.Object == &g_data && nearf(e.Duration, 1.5f * (i + 1)) && nearf(e.MovementVector.Z, -4.0f) && nearf(e.Rotation, 0.25f))) ok = 0; }
    if (!(g_data.IVRefCount == 2)) ok = 0;

    if (!ok) { std::printf("00a2c360_TEST FAIL\n"); return 1; }
    std::printf("00a2c360_TEST PASS\n");
    return 0;
}