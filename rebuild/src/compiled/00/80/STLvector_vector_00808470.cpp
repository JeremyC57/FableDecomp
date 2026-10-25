// std::vector::vector(const vector& x) -- STLport vector<NPlayerTargeting::CTargetPoint> copy constructor: allocate x.size() target points and copy-construct x's elements into them
// Leaf: ??0?$vector@
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00808470, 104 bytes, __fastcall, `ret 4`, returns this.
// Name: Ego_r.pdb `std::vector<NPlayerTargeting::CTargetPoint>::vector(const vector&)` (donor 0x008039F0, Dinkumware);
//   retail is the STLport 4.x copy constructor. The catalog's "NPlayerTargeting_AllocateCopyTargetPoints" is
//   descriptive.
// Callees: MSVCR71 malloc 0x00BFEA0E (the retail STLport allocator), STLport __uninitialized_copy(first, last, result,
//   const __false_type&) 0x00807980 (landed, __fastcall).
// Layout: 12-byte STLport vector (_M_start, _M_finish, _M_end_of_storage) of 0x1c-byte NPlayerTargeting::CTargetPoint.
// Shape: STLport `vector(const vector& x) : _Vector_base(x.size(), x.get_allocator()) { _M_finish =
//   __uninitialized_copy(x._M_start, x._M_finish, _M_start, _IsPODType()); }` with `_Vector_base(n, a) : _M_start(0),
//   _M_finish(0), _M_end_of_storage(a, 0) { _M_start = allocate(n); _M_finish = _M_start; _M_end_of_storage._M_data =
//   _M_start + n; }` -- the three zero stores share `mov eax,0` (the size's flags still live for the `n != 0` test);
//   the empty tag is homed in the `x` argument slot (x itself lives in ebx).
extern "C" void* __cdecl malloc(unsigned int size);   // 0x00BFEA0E

struct CThing;
struct C3DVector { float X, Y, Z; };

class CBaseIntelligentPointer {
public:
    virtual ~CBaseIntelligentPointer();
    void* PData;                               // +0x4
};
template <class T> class CIntelligentPointer : public CBaseIntelligentPointer { };

namespace NPlayerTargeting {
class CTargetPoint {                           // 0x1c bytes
public:
    CIntelligentPointer<CThing> PThing;        // +0x00
    long Index;                                // +0x08
    C3DVector TargetingPosition;               // +0x0c
    long FrameRejected;                        // +0x18
};
}

namespace std {
struct __false_type { __false_type() { } };

template <class _InputIter, class _ForwardIter>
_ForwardIter __fastcall __uninitialized_copy(_InputIter __first, _InputIter __last, _ForwardIter __result,
                                             const __false_type&);   // 0x00807980

template <class _Tp> class allocator {
public:
    _Tp* allocate(unsigned int __n) { return __n != 0 ? (_Tp*)malloc(__n * sizeof(_Tp)) : 0; }
};

template <class _Value, class _Tp> class _STLP_alloc_proxy : public _Tp {
public:
    _Value _M_data;
    _STLP_alloc_proxy(const _Tp& __a, _Value __p) : _Tp(__a), _M_data(__p) { }
};

template <class _Tp> class _Vector_base {
public:
    _Tp* _M_start;
    _Tp* _M_finish;
    _STLP_alloc_proxy<_Tp*, allocator<_Tp> > _M_end_of_storage;
    _Vector_base(unsigned int __n, const allocator<_Tp>& __a) : _M_start(0), _M_finish(0), _M_end_of_storage(__a, 0)
    {
        _M_start = _M_end_of_storage.allocate(__n);
        _M_finish = _M_start;
        _M_end_of_storage._M_data = _M_start + __n;
    }
};

template <class _Tp> class vector : public _Vector_base<_Tp> {
public:
    unsigned int size() const { return (unsigned int)(this->_M_finish - this->_M_start); }
    allocator<_Tp> get_allocator() const { return this->_M_end_of_storage; }
    vector(const vector& __x) : _Vector_base<_Tp>(__x.size(), __x.get_allocator())
    {
        this->_M_finish = __uninitialized_copy((const _Tp*)__x._M_start, (const _Tp*)__x._M_finish, this->_M_start,
                                               __false_type());
    }
};

template class vector<NPlayerTargeting::CTargetPoint>;
}