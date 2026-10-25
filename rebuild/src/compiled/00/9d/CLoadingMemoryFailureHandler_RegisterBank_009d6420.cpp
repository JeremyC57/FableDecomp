// CLoadingMemoryFailureHandler::RegisterBank(CBankFileAsync* bank) -- under the handler's critical section, add an async bank to the list of banks asked to free memory when an allocation fails
// Leaf: ?RegisterBank@CLoadingMemoryFailureHandler@@QAEXPAVCBankFileAsync@@@Z
// Retail: Fable.exe (TLC, VC7.1) function @ 0x009D6420, 97 bytes (+ int3 padding), thiscall `ret 4`.
// Identity: Ego_r.pdb name set `?RegisterBank@CLoadingMemoryFailureHandler@@QAEXPAVCBankFileAsync@@@Z`; the only caller
//   is CBankFileAsync::SetLoadingMemoryFailureHandler 0x009D66A0 (landed: "add bank", `push esi ; mov ecx,ebx`), whose
//   other callee 0x009D5A30 is the matching UnregisterBank. The catalog label "vector_push_back_4byte_list" is
//   descriptive.
// Callees: CCriticalSectionLock(RTL_CRITICAL_SECTION*, bool) @ 0x00A62920, ~CCriticalSectionLock @ 0x00A629A0,
//   vector<CBankFileAsync*>::_M_insert_overflow(pos, x, __true_type, 1, true) @ 0x009D7FC0.
// Layout: the landed siblings' retail model (HandleFailedAllocation 0x009D5F10, dtor 0x009D5EE0): Banks (CArray, the
//   12-byte STLport vector) +0x04, CriticalSection +0x10 (Ego_r.pdb +0x14: its CArray is 16 bytes). The lock is the
//   8-byte local of those siblings (the generated engine/CCriticalSectionLock.h is a bare POD without the ctor/dtor).
// Shape (fix): the real STLport `_M_insert_overflow` body is in the TU, noinline (see below). The lock is built first in the `sub esp,8` frame, then STLport's inline push_back: placement copy under the
//   null test, else _M_insert_overflow with `x` = the by-value parameter's own slot, which also hosts the __true_type
//   tag temporary; the lock is destroyed on both arms.
inline void* operator new(unsigned int, void* p) { return p; }   // placement new (the null test on the destination)
extern "C" void* __cdecl malloc(unsigned int size);         // 0x00BFEA0E
extern "C" void __cdecl free(void* block);                  // 0x00BFEA14
extern "C" void* __cdecl memmove(void* dest, const void* src, unsigned int count);   // 0x00BFEAE6

struct SCriticalSection { unsigned char _body[0x18]; };   // RTL_CRITICAL_SECTION
struct CCriticalSectionLock {                      // 8 bytes on the stack
    SCriticalSection* m_pSection;
    bool              m_locked;
    CCriticalSectionLock(SCriticalSection* section, bool tryOnly);   // 0x00A62920
    ~CCriticalSectionLock();                       // 0x00A629A0
};
class CBankFileAsync;

namespace _STL {
struct __true_type { __true_type() { } };          // user constructor: no zero-initialised tag temporary

template <class _T1, class _T2> inline void _Construct(_T1* __p, const _T2& __val) { new (__p) _T1(__val); }

inline void* __copy_trivial(const void* __first, const void* __last, void* __result)
{
    return (__last == __first) ? __result
        : ((char*)memmove(__result, __first, ((const char*)__last - (const char*)__first))) + ((const char*)__last - (const char*)__first);
}
template <class _OutputIter, class _Size, class _Tp>
inline _OutputIter fill_n(_OutputIter __first, _Size __n, const _Tp& __val)
{
    for ( ; __n > 0; --__n, ++__first)
        *__first = __val;
    return __first;
}
template <class _Tp> inline const _Tp& (max)(const _Tp& __a, const _Tp& __b) { return __a < __b ? __b : __a; }

template <class _Tp> class allocator {
public:
    allocator() { }
    _Tp* allocate(unsigned int __n) { return __n != 0 ? (_Tp*)malloc(__n * sizeof(_Tp)) : 0; }
    void deallocate(_Tp* __p, unsigned int) { if (__p != 0) free(__p); }
};

template <class _Value, class _Alloc> class _STLP_alloc_proxy : public _Alloc {
public:
    _Value _M_data;
};

template <class _Tp, class _Alloc = allocator<_Tp> > class vector {
public:
    typedef __true_type _IsPODType;
    void push_back(const _Tp& __x)
    {
        if (this->_M_finish != this->_M_end_of_storage._M_data) {
            _Construct(this->_M_finish, __x);
            ++this->_M_finish;
        }
        else
            _M_insert_overflow(this->_M_finish, __x, _IsPODType(), 1UL, true);
    }
    void _M_insert_overflow(_Tp* __position, const _Tp& __x, const __true_type&, unsigned int __fill_len,
                            bool __atend = false);   // vector<CBankFileAsync*>: 0x009D7FC0 (same-TU body below)
    unsigned int size() const { return unsigned int(this->_M_finish - this->_M_start); }
    void _M_clear() { this->_M_end_of_storage.deallocate(this->_M_start, this->_M_end_of_storage._M_data - this->_M_start); }
    void _M_set(_Tp* __s, _Tp* __f, _Tp* __e)
    {
        this->_M_start = __s;
        this->_M_finish = __f;
        this->_M_end_of_storage._M_data = __e;
    }
private:
    _Tp* _M_start;                                      // +0x00
    _Tp* _M_finish;                                     // +0x04
    _STLP_alloc_proxy<_Tp*, _Alloc> _M_end_of_storage;  // +0x08
};
}

// STLport's POD _M_insert_overflow (0x009D7FC0, a COMDAT of this TU) as a same-TU noinline body: VC7.1 then sees that
// it never touches the __true_type tag and homes the tag temporary in the dead `bank` argument slot, as retail does.
#pragma auto_inline(off)
namespace _STL {
template <class _Tp, class _Alloc>
__declspec(noinline) void vector<_Tp, _Alloc>::_M_insert_overflow(_Tp* __position, const _Tp& __x, const __true_type&,
                                                                  unsigned int __fill_len, bool __atend)
{
    const unsigned int __old_size = size();
    const unsigned int __len = __old_size + (max)(__old_size, __fill_len);
    _Tp* __new_start = this->_M_end_of_storage.allocate(__len);
    _Tp* __new_finish = (_Tp*)__copy_trivial(this->_M_start, __position, __new_start);
    __new_finish = fill_n(__new_finish, __fill_len, __x);
    if (!__atend)
        __new_finish = (_Tp*)__copy_trivial(__position, this->_M_finish, __new_finish);
    _M_clear();
    _M_set(__new_start, __new_finish, __new_start + __len);
}
}
#pragma auto_inline(on)

template <class T> class CArray : public _STL::vector<T> {      // retail CArray: the 12-byte STLport vector
};

class CFailedAllocationHandler {                   // vtable 0x0129B874
public:
    virtual bool HandleFailedAllocation(unsigned long size);
};

class CLoadingMemoryFailureHandler : public CFailedAllocationHandler {   // vtable 0x0129B8C4
public:
    CArray<CBankFileAsync*> Banks;                 // +0x04
    SCriticalSection        CriticalSection;       // +0x10
    virtual bool HandleFailedAllocation(unsigned long size);
    void RegisterBank(CBankFileAsync* bank);
    void UnregisterBank(CBankFileAsync* bank);     // 0x009D5A30
};

void CLoadingMemoryFailureHandler::RegisterBank(CBankFileAsync* bank)
{
    CCriticalSectionLock lock(&CriticalSection, false);
    Banks.push_back(bank);
}