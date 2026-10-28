// std::vector<std::pair<CCharString, C3DHelperPoint> >::_M_insert_overflow(std::pair<CCharString, C3DHelperPoint>* position, const std::pair<CCharString, C3DHelperPoint>& x, const __false_type&, unsigned int fill_len, bool atend) -- STLport vector growth for the helper-point map entries (20 bytes): move into a size + max(size, fill_len) block with x inserted fill_len times at position
// Leaf: ?_M_insert_overflow@?$vector@U?$pair@VCCharString@@VC3DHelperPoint@@@std@@V?$allocator@U?$pair@VCCharString@@VC3DHelperPoint@@@std@@@2@@std@@QAEXPAU?$pair@VCCharString@@VC3DHelperPoint@@@2@ABU32@ABU__false_type@2@I_N@Z
// Retail: Fable.exe (TLC, VC7.1) @ 0x00A71FC0, 283 bytes, thiscall `ret 14h` (a /Gr TU: the helpers are __fastcall).
// Identity: its callers are the landed push_back 0x00A72690 (reached from CVectorMap<CCharString, C3DHelperPoint>::Add
//   0x00A72B90, which sets the map's Dirty byte +0x0D) and CPhysicsMesh::GetHelperDataFromFileData (0x00A6D1E3); the
//   20-byte element (`0x66666667 ; sar 3`, `lea [n+n*4] ; shl 2`) is the map's pair<CCharString, C3DHelperPoint>
//   (Ego_r.pdb CPhysicsMesh::HelperPoints; C3DHelperPoint = C3DVector Position + long BoneIndex). The current binding
//   spelling (CSkeletalMorphEntryVector / SSkeletalMorphEntry) is a stand-in named after a wrong caller label.
//   Catalog label "std_Vector_Reallocate_With_Copy" is descriptive.
// Callees: malloc 0x00BFEA0E / free 0x00BFEA14 (allocator), __uninitialized_copy 0x00A6ED80 (x2), __uninitialized_fill_n 0x00A6EDE0,
//   CCharString copy ctor 0x0099EC30 (STLport pair's user copy ctor in the fill_len == 1 construct), ~CCharString
//   0x0099EAE0 (the inlined _M_clear: the pair's implicit destructor).
// Shape: the landed 0x00624B90 / this chunk's 0x00A4A710 STLport `_M_insert_overflow` model verbatim.
extern "C" void* __cdecl malloc(unsigned int size);                // 0x00BFEA0E
extern "C" void __cdecl free(void* p);                             // 0x00BFEA14

inline void* operator new(unsigned int, void* p) { return p; }

class CCharString {
public:
    void* PStringData;
    CCharString(const CCharString& other);                         // 0x0099EC30
    ~CCharString();                                                // 0x0099EAE0
};

class C3DVector {                                                  // 12 bytes (Ego_r.pdb)
public:
    float X, Y, Z;
    C3DVector(const C3DVector& v) : X(v.X), Y(v.Y), Z(v.Z) { }    // FableWin.pdb ??0C3DVector@@QAE@ABV0@@Z
};

class C3DHelperPoint {                                             // 16 bytes (Ego_r.pdb)
public:
    C3DVector Position;                                            // +0x00
    long      BoneIndex;                                           // +0x0c
    C3DHelperPoint(const C3DHelperPoint& o) : Position(o.Position), BoneIndex(o.BoneIndex) { }   // FableWin.pdb
};

namespace std {

struct __false_type { __false_type() { } };

template <class _T1, class _T2> struct pair {
    _T1 first;
    _T2 second;
    pair(const pair<_T1, _T2>& __o) : first(__o.first), second(__o.second) { }
};

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
{                                                                  // 0x00A6ED80
    _ForwardIter __cur = __result;
    for (; __first != __last; ++__first, ++__cur)
        _Construct(&*__cur, *__first);
    return __cur;
}

template <class _ForwardIter, class _Size, class _Tp>
__declspec(noinline) _ForwardIter __fastcall __uninitialized_fill_n(_ForwardIter __first, _Size __n, const _Tp& __x,
                                                                    const __false_type&)
{                                                                  // 0x00A6EDE0
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

template void vector<std::pair<CCharString, C3DHelperPoint> >::_M_insert_overflow(std::pair<CCharString, C3DHelperPoint>*, const std::pair<CCharString, C3DHelperPoint>&,
                                                                 const __false_type&, unsigned int, bool);

}