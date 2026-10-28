// std::vector<CEngineTexturePalette::CPaletteEntry>::_M_insert_overflow(CPaletteEntry* pos, const CPaletteEntry& x, const __false_type&, size_type fill_len, bool atend) -- STLport vector growth: move into a size + max(size, fill_len) block with x inserted fill_len times at pos
// Leaf: ?_M_insert_overflow@?$vector@UCPaletteEntry@CEngineTexturePalette@@
// Flags: /Gr
// Retail: Fable.exe (TLC, VC7.1) @ 0x00BF94D0, 249 bytes, thiscall `ret 14h` (a /Gr TU: the helpers are __fastcall).
// Identity (proven): two of its four callers are inline push_backs inside CEngineTexturePalette's constructor 0x00BF90A0
//   (call 0x00BF9133) and GetTexturePaletteIndex 0x00BF9150 (call 0x00BF91FB), both on this+4 = Ego_r.pdb
//   CEngineTexturePalette::Palette (CArray<CPaletteEntry>; RetailVersion at +0x14 once the STLport vector is 12 bytes);
//   the other two are push_back 0x00BF96F0. FableWin.pdb spells the element UCPaletteEntry@CEngineTexturePalette (struct):
//   {ulong Index +0, CCharString Name +4}. Byte twin of landed vector<CQuestCardItem>::_M_insert_overflow 0x00624B90.
//   The catalog's "Vector_ReallocateForPairs" is descriptive; "CParticleMorphs::Initialize" on 0x00BF90A0 is wrong.
// Callees: malloc 0x00BFEA0E, __uninitialized_copy 0x00BF8F30 (x2), __uninitialized_fill_n 0x00BF8F70, CCharString copy
//   ctor 0x0099EC30 (the fill_len == 1 copy), ~CCharString 0x0099EAE0 and free 0x00BFEA14 (the inlined _M_clear).
// Shape: STLport 4.x `_M_insert_overflow(.., const __false_type&, ..)`; the tag-taking callees carry same-TU
//   `__declspec(noinline)` bodies so VC7.1 homes the `__false_type()` temporaries on the `atend` slot (GOTCHAS 0x4ADB50);
//   only _M_insert_overflow is graded.
extern "C" void* __cdecl malloc(unsigned int size);                // 0x00BFEA0E
extern "C" void __cdecl free(void* p);                             // 0x00BFEA14

inline void* operator new(unsigned int, void* p) { return p; }

class CCharString {
public:
    void* PStringData;
    CCharString(const CCharString& other);                         // 0x0099EC30
    ~CCharString();                                                // 0x0099EAE0
};

class CEngineTexturePalette {
public:
    struct CPaletteEntry {                                         // 8 bytes (Ego_r.pdb)
        unsigned long Index;                                       // +0x00
        CCharString   Name;                                        // +0x04
    };
};
namespace std {

struct __false_type { __false_type() { } };

template <class _Tp> class allocator {
public:
    allocator() { }
    _Tp* allocate(unsigned int __n, const void* = 0) { return __n != 0 ? (_Tp*)malloc(__n * sizeof(_Tp)) : 0; }
    void deallocate(_Tp* __p, unsigned int) { if (__p) free(__p); }
};

template <class _Value, class _Tp, class _MaybeReboundAlloc>
class _STLP_alloc_proxy : public _MaybeReboundAlloc {
public:
    _Value _M_data;
};

template <class _Tp> inline const _Tp& (max)(const _Tp& __a, const _Tp& __b) { return __a < __b ? __b : __a; }

template <class _T1, class _T2> inline void _Construct(_T1* __p, const _T2& __val) { new (__p) _T1(__val); }
template <class _T1, class _T2> inline void _Copy_Construct(_T1* __p, const _T2& __val) { new (__p) _T1(__val); }
template <class _Tp> inline void _Destroy(_Tp* __pointer) { __pointer->~_Tp(); }
template <class _ForwardIter> inline void _Destroy(_ForwardIter __first, _ForwardIter __last)
{
    for (; __first != __last; ++__first)
        _Destroy(&*__first);
}

template <class _InputIter, class _ForwardIter>
__declspec(noinline) _ForwardIter __fastcall __uninitialized_copy(_InputIter __first, _InputIter __last,
                                                                  _ForwardIter __result, const __false_type&)
{                                                                  // 0x00BF8F30
    _ForwardIter __cur = __result;
    for (; __first != __last; ++__first, ++__cur)
        _Construct(&*__cur, *__first);
    return __cur;
}

template <class _ForwardIter, class _Size, class _Tp>
__declspec(noinline) _ForwardIter __fastcall __uninitialized_fill_n(_ForwardIter __first, _Size __n, const _Tp& __x,
                                                                    const __false_type&)
{                                                                  // 0x00BF8F70
    _ForwardIter __cur = __first;
    for (; __n > 0; --__n, ++__cur)
        _Construct(&*__cur, __x);
    return __cur;
}

template <class _Tp, class _Alloc> class _Vector_base {
public:
    _Tp* _M_start;
    _Tp* _M_finish;
    _STLP_alloc_proxy<_Tp*, _Tp, _Alloc> _M_end_of_storage;
};

template <class _Tp, class _Alloc = allocator<_Tp> >
class vector : protected _Vector_base<_Tp, _Alloc> {
public:
    typedef unsigned int size_type;
    typedef _Tp* pointer;
    size_type size() const { return size_type(this->_M_finish - this->_M_start); }
    protected:
    void _M_clear()
    {
        _Destroy(this->_M_start, this->_M_finish);
        this->_M_end_of_storage.deallocate(this->_M_start, this->_M_end_of_storage._M_data - this->_M_start);
    }
    public:
    protected:
    void _M_set(pointer __s, pointer __f, pointer __e)
    {
        this->_M_start = __s;
        this->_M_finish = __f;
        this->_M_end_of_storage._M_data = __e;
    }
    public:
    protected:
    void _M_insert_overflow(pointer __position, const _Tp& __x, const __false_type&, size_type __fill_len,
                            bool __atend = false);
    public:
};

template <class _Tp, class _Alloc>
void vector<_Tp, _Alloc>::_M_insert_overflow(pointer __position, const _Tp& __x, const __false_type&,
                                             size_type __fill_len, bool __atend)
{
    const size_type __old_size = size();
    const size_type __len = __old_size + (max)(__old_size, __fill_len);

    pointer __new_start = this->_M_end_of_storage.allocate(__len);
    pointer __new_finish = __new_start;
    __new_finish = __uninitialized_copy(this->_M_start, __position, __new_start, __false_type());
    // handle insertion
    if (__fill_len == 1) {
        _Copy_Construct(__new_finish, __x);
        ++__new_finish;
    } else
        __new_finish = __uninitialized_fill_n(__new_finish, __fill_len, __x, __false_type());
    if (!__atend)
        // copy remainder
        __new_finish = __uninitialized_copy(__position, this->_M_finish, __new_finish, __false_type());
    _M_clear();
    _M_set(__new_start, __new_finish, __new_start + __len);
}

template void vector<CEngineTexturePalette::CPaletteEntry>::_M_insert_overflow(CEngineTexturePalette::CPaletteEntry*,
                                                                        const CEngineTexturePalette::CPaletteEntry&, const __false_type&,
                                                         unsigned int, bool);

}