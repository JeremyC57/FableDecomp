// _STL::vector<CInputEventPackage>::_M_insert_overflow(CInputEventPackage*, const CInputEventPackage&, const __false_type&, unsigned int, bool) -- STLport reallocating insert of 48-byte input-event packages: allocate old_size + max(old_size, n), copy the head, construct the n new packages, copy the tail unless appending, destroy and free the old block
// Leaf: _M_insert_overflow
// Retail: Fable.exe (TLC, VC7.1) function @ 0x009F6ED0, 228 bytes (240 with padding), thiscall, `ret 14h`.
// Callees: 0x00BFEA0E malloc / 0x00BFEA14 free (STLport's malloc allocator: `n != 0 ? malloc(n * 48) : 0`, `if (p) free(p)`),
//   0x009F6C00 __uninitialized_copy<CInputEventPackage*> (__fastcall first/last, result + tag on the stack; landed as
//   global_UninitCopyRecord48), 0x009F69B0 the CInputEventPackage copy constructor (the n == 1 placement construct, null-tested;
//   landed under the propagated name CDefWithRecords_009f69b0::operator=), 0x009F6C90 __uninitialized_fill_n<CInputEventPackage*>
//   (landed std_uninitializedfilln), 0x009F6E00 __destroy_aux<CInputEventPackage*> (landed Destroy_009f6e00, the _M_clear destroy).
// Caller: vector<CInputEventPackage>::push_back @ 0x009F6FE0 (landed; the LoadedEventPackages vector of CInputManager).
//   The chunk label "Vector_InsertElementRange" is descriptive.
// Layout: Ego_r.pdb CInputEventPackage (0x30 bytes): Events vector<CInputEvent> +0x00 (retail 12-byte vector), TimeStamp +0x0C,
//   Checksum +0x10, MousePos +0x20, JoystickPos +0x28.
// Shape: STLport 4.x `_M_insert_overflow` (non-POD path) verbatim -- the landed vector<CTestQuest> twin 0x004ADB50 with a
//   48-byte element (`/ 48` = 0x2AAAAAAB multiply + `sar edx,3`, `lea [x+x*2] ; shl 4` for `* 48`). Every `__false_type()`
//   tag temporary is homed on the bool `__atend` argument slot (`lea ecx,[esp+28h]`), which VC7.1 does only when the
//   tag-taking callees are visible and never touch the tag: same-TU `auto_inline(off)` stand-in bodies (the retail callees
//   are separate COMDATs at the addresses above). `max` by reference with old_size in the `push ecx` frame word.
#include <stdlib.h>                                 // malloc @ 0x00BFEA0E, free @ 0x00BFEA14
#include <new>

struct __false_type { __false_type() { } };          // user constructor: no zero-initialised tag temporary

struct CInputEventPackage {                          // 0x30 bytes
    long m_words[12];
    CInputEventPackage(const CInputEventPackage& other);   // 0x009F69B0
    ~CInputEventPackage();
};

#pragma auto_inline(off)
// Same-TU stand-ins for the STLport COMDATs 0x009F6C00 / 0x009F6C90 / 0x009F6E00 (they never touch the tag).
CInputEventPackage* __fastcall __uninitialized_copy(CInputEventPackage* __first, CInputEventPackage* __last,
                                                    CInputEventPackage* __result, const __false_type&)
{
    for ( ; __first != __last; ++__first, ++__result)
        new (__result) CInputEventPackage(*__first);
    return __result;
}
CInputEventPackage* __fastcall __uninitialized_fill_n(CInputEventPackage* __first, unsigned int __n,
                                                      const CInputEventPackage& __x, const __false_type&)
{
    CInputEventPackage* __cur = __first;
    for ( ; __n > 0; --__n, ++__cur)
        new (__cur) CInputEventPackage(__x);
    return __cur;
}
void __fastcall __destroy_aux(CInputEventPackage* __first, CInputEventPackage* __last, const __false_type&)
{
    for ( ; __first != __last; ++__first)
        __first->~CInputEventPackage();
}
#pragma auto_inline(on)

template <class _Tp>
inline const _Tp& (max)(const _Tp& __a, const _Tp& __b) { return __a < __b ? __b : __a; }

template <class _Tp>
struct allocator {
    allocator() { }
    _Tp* allocate(unsigned int __n) { return __n != 0 ? (_Tp*)malloc(__n * sizeof(_Tp)) : 0; }
    void deallocate(_Tp* __p, unsigned int) { if (__p != 0) free(__p); }
};

template <class _Value, class _Tp, class _MaybeReboundAlloc>
struct _STLP_alloc_proxy : public _MaybeReboundAlloc {
    _Value _M_data;
};

inline void _Destroy(CInputEventPackage* __first, CInputEventPackage* __last) { __destroy_aux(__first, __last, __false_type()); }

struct CInputEventPackageVector {
    CInputEventPackage* _M_start;                    // +0x00
    CInputEventPackage* _M_finish;                   // +0x04
    _STLP_alloc_proxy<CInputEventPackage*, CInputEventPackage, allocator<CInputEventPackage> > _M_end_of_storage;   // +0x08

    unsigned int size() const { return unsigned int(_M_finish - _M_start); }
    void _M_clear()
    {
        _Destroy(_M_start, _M_finish);
        _M_end_of_storage.deallocate(_M_start, _M_end_of_storage._M_data - _M_start);
    }
    void _M_set(CInputEventPackage* __s, CInputEventPackage* __f, CInputEventPackage* __e)
    {
        _M_start = __s;
        _M_finish = __f;
        _M_end_of_storage._M_data = __e;
    }
    void _M_insert_overflow(CInputEventPackage* __position, const CInputEventPackage& __x, const __false_type&,
                            unsigned int __fill_len, bool __atend);
};

void CInputEventPackageVector::_M_insert_overflow(CInputEventPackage* __position, const CInputEventPackage& __x,
                                                  const __false_type&, unsigned int __fill_len, bool __atend)
{
    const unsigned int __old_size = size();
    const unsigned int __len = __old_size + (max)(__old_size, __fill_len);

    CInputEventPackage* __new_start = _M_end_of_storage.allocate(__len);
    CInputEventPackage* __new_finish = __new_start;
    __new_finish = __uninitialized_copy(_M_start, __position, __new_start, __false_type());
    if (__fill_len == 1) {
        new (__new_finish) CInputEventPackage(__x);
        ++__new_finish;
    }
    else
        __new_finish = __uninitialized_fill_n(__new_finish, __fill_len, __x, __false_type());
    if (!__atend)
        __new_finish = __uninitialized_copy(__position, _M_finish, __new_finish, __false_type());
    _M_clear();
    _M_set(__new_start, __new_finish, __new_start + __len);
}