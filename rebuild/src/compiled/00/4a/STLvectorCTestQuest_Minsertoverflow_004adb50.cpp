// std::vector<CTestQuest>::_M_insert_overflow(CTestQuest*, const CTestQuest&, const __false_type&, unsigned int, bool) -- STLport reallocating insert of 28-byte test-quest records: allocate old_size + max(old_size, n), copy the head, construct the n new records, copy the tail unless appending, destroy and free the old block
// Leaf: _M_insert_overflow
// Retail: Fable.exe (TLC, VC7.1) function @ 0x004ADB50, 226 bytes (240 with padding), `ret 14h` (__fastcall).
// Callees: 0x00BFEA0E malloc / 0x00BFEA14 free (STLport's malloc allocator: `n != 0 ? malloc(n * 28) : 0`,
//   `if (p) free(p)`), 0x004AA060 __uninitialized_copy<CTestQuest*> (__fastcall first/last, result + tag on the stack),
//   0x004A89D0 CTestQuest copy constructor (the n == 1 placement construct, null-tested), 0x004AA0D0
//   __uninitialized_fill_n<CTestQuest*> (this chunk), 0x004ABD90 __destroy_aux<CTestQuest*> (the _M_clear destroy).
// Caller: vector<CTestQuest>::push_back / insert (the ActiveTestQuests vector of CWorld, cleared by 0x004A08D0).
// Layout: Ego_r.pdb CTestQuest (28 bytes, seven words); STLport vector {_M_start, _M_finish, _M_end_of_storage}.
// Shape: STLport 4.x `_M_insert_overflow` (non-POD path) verbatim. Every `__false_type()` tag temporary is homed on the
//   bool `__atend` argument slot (`lea ecx,[esp+28h]`), which VC7.1 does only when the tag-taking callees are visible
//   and never touch the tag: they get same-TU `auto_inline(off)` stand-in bodies (the retail callees are separate
//   COMDATs at the addresses above). `max` by reference (`lea ecx,[esp+24h] ; jb ; lea ecx,[esp+10h]`) with old_size
//   in the `push ecx` frame word.
// Label correction: catalogued as `Vector_Reserve_And_Assign`; the body is STLport `vector<CTestQuest>::_M_insert_overflow`.
#include <stdlib.h>                                 // malloc @ 0x00BFEA0E, free @ 0x00BFEA14
#include <new>

struct __false_type { __false_type() { } };          // user constructor: no zero-initialised tag temporary

class CCharString { public:
    void* PStringData;
    CCharString(const CCharString& other);           // 0x0099EC30
    ~CCharString();                                  // 0x0099EAE0
};

class CTestQuest { public:
    CCharString QuestName;                           // +0x00
    CCharString HSPName;                             // +0x04
    long        ColourIndex;                         // +0x08
    CCharString DisplayName;                         // +0x0C
    CCharString IniFile;                             // +0x10
    CCharString OutiFile;                            // +0x14
    CCharString QuestCardDefName;                    // +0x18
    CTestQuest(const CTestQuest& other);             // 0x004A89D0
};

#pragma auto_inline(off)
// Same-TU stand-ins for the STLport COMDATs 0x004AA060 / 0x004AA0D0 / 0x004ABD90 (they never touch the tag).
CTestQuest* __fastcall __uninitialized_copy(CTestQuest* __first, CTestQuest* __last, CTestQuest* __result, const __false_type&)
{
    for ( ; __first != __last; ++__first, ++__result)
        new (__result) CTestQuest(*__first);
    return __result;
}
CTestQuest* __fastcall __uninitialized_fill_n(CTestQuest* __first, unsigned int __n, const CTestQuest& __x, const __false_type&)
{
    CTestQuest* __cur = __first;
    for ( ; __n > 0; --__n, ++__cur)
        new (__cur) CTestQuest(__x);
    return __cur;
}
void __fastcall __destroy_aux(CTestQuest* __first, CTestQuest* __last, const __false_type&)
{
    for ( ; __first != __last; ++__first)
        __first->~CTestQuest();
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

inline void _Destroy(CTestQuest* __first, CTestQuest* __last) { __destroy_aux(__first, __last, __false_type()); }

struct CTestQuestVector {
    CTestQuest* _M_start;                            // +0x00
    CTestQuest* _M_finish;                           // +0x04
    _STLP_alloc_proxy<CTestQuest*, CTestQuest, allocator<CTestQuest> > _M_end_of_storage;   // +0x08

    unsigned int size() const { return unsigned int(_M_finish - _M_start); }
    void _M_clear()
    {
        _Destroy(_M_start, _M_finish);
        _M_end_of_storage.deallocate(_M_start, _M_end_of_storage._M_data - _M_start);
    }
    void _M_set(CTestQuest* __s, CTestQuest* __f, CTestQuest* __e)
    {
        _M_start = __s;
        _M_finish = __f;
        _M_end_of_storage._M_data = __e;
    }
    void _M_insert_overflow(CTestQuest* __position, const CTestQuest& __x, const __false_type&,
                            unsigned int __fill_len, bool __atend);
};

void CTestQuestVector::_M_insert_overflow(CTestQuest* __position, const CTestQuest& __x, const __false_type&,
                                          unsigned int __fill_len, bool __atend)
{
    const unsigned int __old_size = size();
    const unsigned int __len = __old_size + (max)(__old_size, __fill_len);

    CTestQuest* __new_start = _M_end_of_storage.allocate(__len);
    CTestQuest* __new_finish = __new_start;
    __new_finish = __uninitialized_copy(_M_start, __position, __new_start, __false_type());
    if (__fill_len == 1) {
        new (__new_finish) CTestQuest(__x);
        ++__new_finish;
    }
    else
        __new_finish = __uninitialized_fill_n(__new_finish, __fill_len, __x, __false_type());
    if (!__atend)
        __new_finish = __uninitialized_copy(__position, _M_finish, __new_finish, __false_type());
    _M_clear();
    _M_set(__new_start, __new_finish, __new_start + __len);
}