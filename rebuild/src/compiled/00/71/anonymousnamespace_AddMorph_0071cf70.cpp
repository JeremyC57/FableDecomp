// `anonymous namespace'::AddMorph(const CSkeletalMorphs& morphs, EHeroMorphType type, float weight, vector<pair<CSkeletalMorphs::CEntry,float>>& out) -- append every skeletal morph of the given hero-morph type, paired with the weight, to out
// Leaf: ?AddMorph@
// Retail: Fable.exe (TLC, VC7.1) function @ 0x0071CF70, length 174 bytes (+ int3 padding). `__fastcall` (morphs in ecx, type in
//         edx), `ret 8`. Ego_r.pdb: `?AddMorph@?A0x990af2a9@@YIXABVCSkeletalMorphs@@W4EHeroMorphType@@MAAV?$vector@U?$pair@VCEntry@CSkeletalMorphs@@M@std@@...@Z`.
// Callees: CSkeletalMorphs::GetAllMorphsOfType(unsigned long, vector<CEntry>&) const @ 0x0081AE50,
//          vector<pair<CEntry,float> >::_M_insert_overflow(pos, x, __false_type, 1, true) @ 0x00720F40, free (CRT import
//          thunk) @ 0x00BFEA14 (the local vector).
// Identity: the manifest label matches the PDB name. Single caller 0x0071DF0C (the hero skeletal-morph update).
// Shape: a local STLport vector<CEntry> (three zero stores), `if (GetAllMorphsOfType(type, entries))` then an iterator
//   loop pushing `pair<CEntry, float>(*it, weight)` (STLport pair with its explicit copy constructor); the inline
//   push_back placement-copies the pair (null test, `add [edi+4],0Ch`), the overflow takes the empty tag in the dead
//   `weight` slot; then ~vector's `if (p != 0) free(p)`. The overflow is a same-TU stand-in body under auto_inline(off):
//   with a declaration only VC7.1 rewrites the temporary's weight every pass and copies through the temporary
//   (179 v 174 bytes); with the body it hoists the weight store and copies the words straight from the source.
// Emission: the `_Use` caller sits before the definition (VC7.1 would discard the unreferenced internal function).
extern "C" void* __cdecl malloc(unsigned int size);     // 0x00BFEA0E (import thunk)
extern "C" void __cdecl free(void* block);              // 0x00BFEA14 (import thunk)
inline void* operator new(unsigned int, void* where) { return where; }
struct __false_type { __false_type() {} };

enum EHeroMorphType { HERO_MORPH_TYPE_DUMMY };

struct CDefString {
    long TablePos;                                      // +0x00
};

namespace std {
template <class _T1, class _T2> struct pair {
    _T1 first;
    _T2 second;
    pair(const _T1& __a, const _T2& __b) : first(__a), second(__b) {}
    template <class _U1, class _U2> pair(const pair<_U1, _U2>& __p) : first(__p.first), second(__p.second) {}
    pair(const pair<_T1, _T2>& __o) : first(__o.first), second(__o.second) {}
};

template <class _T1, class _T2>
inline void _Construct(_T1* __p, const _T2& __val) { new (__p) _T1(__val); }

template <class _Tp> class allocator {                  // STLport allocator over malloc / free (empty)
public:
    allocator() {}
    _Tp* allocate(unsigned int __n) { return __n != 0 ? (_Tp*)malloc(__n * sizeof(_Tp)) : 0; }
    void deallocate(_Tp* __p, unsigned int) { if (__p != 0) free(__p); }
};
template <class _Value, class _Alloc> class _STLP_alloc_proxy : public _Alloc {   // the end-of-storage pointer
public:
    _Value _M_data;
    _STLP_alloc_proxy(const _Alloc& __a, _Value __p) : _Alloc(__a), _M_data(__p) {}
};

template <class _Tp> class _Vector_base {
public:
    _Tp* _M_start;
    _Tp* _M_finish;
    _STLP_alloc_proxy<_Tp*, allocator<_Tp> > _M_end_of_storage;
    _Vector_base(const allocator<_Tp>& __a) : _M_start(0), _M_finish(0), _M_end_of_storage(__a, 0) {}
    ~_Vector_base()
    {
        if (_M_start != 0)
            _M_end_of_storage.deallocate(_M_start, _M_end_of_storage._M_data - _M_start);
    }
};

template <class _Tp> class vector : public _Vector_base<_Tp> {
public:
    typedef _Tp* iterator;
    explicit vector(const allocator<_Tp>& __a = allocator<_Tp>()) : _Vector_base<_Tp>(__a) {}
    ~vector() {}
    iterator begin() { return this->_M_start; }
    iterator end() { return this->_M_finish; }
    void _M_insert_overflow(_Tp* __position, const _Tp& __x, const __false_type&, unsigned int __fill_len, bool __atend);
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

template <class _Tp>
inline const _Tp& (max)(const _Tp& __a, const _Tp& __b) { return __a < __b ? __b : __a; }
template <class _InputIter, class _ForwardIter>
inline _ForwardIter __uninitialized_copy(_InputIter __first, _InputIter __last, _ForwardIter __result, const __false_type&)
{
    _ForwardIter __cur = __result;
    for ( ; __first != __last; ++__first, ++__cur)
        _Construct(&*__cur, *__first);
    return __cur;
}
template <class _ForwardIter, class _Size, class _Tp>
inline _ForwardIter __uninitialized_fill_n(_ForwardIter __first, _Size __n, const _Tp& __x, const __false_type&)
{
    _ForwardIter __cur = __first;
    for ( ; __n > 0; --__n, ++__cur)
        _Construct(&*__cur, __x);
    return __cur;
}

// Same-TU stand-in body (the real overflow, 0x00720F40, sits later in this TU): VC7.1 uses what it learns from it.
#pragma auto_inline(off)
template <class _Tp>
__declspec(noinline) void vector<_Tp>::_M_insert_overflow(_Tp* __position, const _Tp& __x, const __false_type&, unsigned int __fill_len, bool __atend)
{
    const unsigned int __old_size = (unsigned int)(this->_M_finish - this->_M_start);
    const unsigned int __len = __old_size + (max)(__old_size, __fill_len);
    _Tp* __new_start = this->_M_end_of_storage.allocate(__len);
    _Tp* __new_finish = __uninitialized_copy(this->_M_start, __position, __new_start, __false_type());
    if (__fill_len == 1) {
        _Construct(__new_finish, __x);
        ++__new_finish;
    } else
        __new_finish = __uninitialized_fill_n(__new_finish, __fill_len, __x, __false_type());
    if (!__atend)
        __new_finish = __uninitialized_copy(__position, this->_M_finish, __new_finish, __false_type());
    this->_M_end_of_storage.deallocate(this->_M_start, this->_M_end_of_storage._M_data - this->_M_start);
    this->_M_start = __new_start;
    this->_M_finish = __new_finish;
    this->_M_end_of_storage._M_data = __new_start + __len;
}
#pragma auto_inline(on)
}

class CSkeletalMorphs {
public:
    class CEntry {                                      // Ego_r.pdb, 8 bytes
    public:
        CDefString MorphName;                           // +0x00
        short Priority;                                 // +0x04
        bool SupressesLowerMorphs;                      // +0x06
    };
    unsigned char Morphs[0x10];                         // +0x00 CVectorMap<unsigned long, CEntry, ...> (retail 16 bytes)
    bool GetAllMorphsOfType(unsigned long type, std::vector<CEntry>& out) const;   // 0x0081AE50
};

namespace {
void __fastcall AddMorph(const CSkeletalMorphs& morphs, EHeroMorphType type, float weight,
                         std::vector<std::pair<CSkeletalMorphs::CEntry, float> >& out);
}

void __fastcall AddMorph_Use(const CSkeletalMorphs& morphs, EHeroMorphType type, float weight,
                             std::vector<std::pair<CSkeletalMorphs::CEntry, float> >& out)
{
    AddMorph(morphs, type, weight, out);
}

namespace {
void __fastcall AddMorph(const CSkeletalMorphs& morphs, EHeroMorphType type, float weight,
                         std::vector<std::pair<CSkeletalMorphs::CEntry, float> >& out)
{
    std::vector<CSkeletalMorphs::CEntry> entries;
    if (morphs.GetAllMorphsOfType(type, entries))
        for (std::vector<CSkeletalMorphs::CEntry>::iterator it = entries.begin(); it != entries.end(); ++it)
            out.push_back(std::pair<CSkeletalMorphs::CEntry, float>(*it, weight));
}
}