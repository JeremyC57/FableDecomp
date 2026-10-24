// Behaviour test for vector<CPhysicsMeshInfo>::push_back (0x00A2D190): an in-place copy while capacity remains (the
// copy keeps Bank/Index, is a CPhysicsMeshInfo and addrefs the shared mesh data), and the overflow path when full.
#include <cstdio>
#include <cstdlib>
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
    ~CIVCountedPointer() { if (Object) --Object->IVRefCount; }
};

class CPhysicsMeshInfo : public CMeshDataBankEntry {
public:
    class CMeshData;
    virtual int Kind() const { return 1; }
    CIVCountedPointer<CMeshData> MeshData;
};

static int g_overflows = 0;

namespace _STL {
struct __false_type { __false_type() {} };
template <class _T1, class _T2>
inline void _Construct(_T1* __p, const _T2& __val) { new (__p) _T1(__val); }
template <class _Tp> class allocator { public: allocator() {} };
template <class _Tp, class _Alloc> class _STLP_alloc_proxy { public: _Tp _M_data; };
template <class _Tp, class _Alloc = allocator<_Tp> > class vector {
public:
    _Tp* _M_start;
    _Tp* _M_finish;
    _STLP_alloc_proxy<_Tp*, _Alloc> _M_end_of_storage;
    void _M_insert_overflow(_Tp* __position, const _Tp& __x, const __false_type&, unsigned int __fill_len, bool)
    {
        ++g_overflows;
        unsigned int __old = (unsigned int)(_M_finish - _M_start);
        unsigned int __len = __old + (__old > __fill_len ? __old : __fill_len);
        _Tp* __new_start = (_Tp*)std::malloc(__len * sizeof(_Tp));
        _Tp* __new_finish = __new_start;
        for (_Tp* p = _M_start; p != __position; ++p, ++__new_finish) _Construct(__new_finish, *p);
        for (unsigned int i = 0; i < __fill_len; ++i, ++__new_finish) _Construct(__new_finish, __x);
        for (_Tp* p = _M_start; p != _M_finish; ++p) p->~_Tp();
        std::free(_M_start);
        _M_start = __new_start;
        _M_finish = __new_finish;
        _M_end_of_storage._M_data = __new_start + __len;
    }
    void push_back(const _Tp& __x)
    {
        if (this->_M_finish != this->_M_end_of_storage._M_data) {
            _Construct(this->_M_finish, __x);
            ++this->_M_finish;
        }
        else
            _M_insert_overflow(this->_M_finish, __x, __false_type(), 1UL, true);
    }
};
}

int main()
{
    int ok = 1;
    CMeshDataBank bank; bank.Id = 3;
    CIVCountedPointeeBase data; data.IVRefCount = 1;
    CPhysicsMeshInfo info;
    info.Bank = &bank; info.Index = 42; info.MeshData.Object = &data;

    _STL::vector<CPhysicsMeshInfo> v;
    v._M_start = (CPhysicsMeshInfo*)std::malloc(2 * sizeof(CPhysicsMeshInfo));
    v._M_finish = v._M_start; v._M_end_of_storage._M_data = v._M_start + 2;

    v.push_back(info);                                    // room: in place
    if (g_overflows != 0 || v._M_finish - v._M_start != 1) ok = 0;
    if (v._M_start[0].Kind() != 1 || v._M_start[0].Bank != &bank || v._M_start[0].Index != 42) ok = 0;
    if (v._M_start[0].MeshData.Object != &data || data.IVRefCount != 2) ok = 0;
    info.Index = 43;
    v.push_back(info);
    if (g_overflows != 0 || v._M_finish - v._M_start != 2 || v._M_start[1].Index != 43 || data.IVRefCount != 3) ok = 0;
    info.Index = 44;
    v.push_back(info);                                    // full: overflow path
    if (g_overflows != 1 || v._M_finish - v._M_start != 3) ok = 0;
    if (v._M_start[0].Index != 42 || v._M_start[1].Index != 43 || v._M_start[2].Index != 44) ok = 0;
    if (v._M_start[2].Kind() != 1 || data.IVRefCount != 4) ok = 0;

    for (CPhysicsMeshInfo* p = v._M_start; p != v._M_finish; ++p) p->~CPhysicsMeshInfo();
    std::free(v._M_start);
    if (data.IVRefCount != 1) ok = 0;
    info.MeshData.Object = 0;

    if (!ok) { std::printf("00a2d190_TEST FAIL\n"); return 1; }
    std::printf("00a2d190_TEST PASS\n");
    return 0;
}