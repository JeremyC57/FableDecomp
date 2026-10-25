// vector::vector(unsigned int n) -- STLport vector<CScriptedMapBrush::CScriptedMapCell>(n): allocate n cells and fill them with copies of a value-initialised (all-zero) cell
// Leaf: ??0?$vector@VCScriptedMapCell@CScriptedMapBrush@@
// Retail: Fable.exe (TLC, VC7.1) @ 0x0082F630, 108 bytes (+4 int3), thiscall `ret 4`.
// Identity: CScriptedMapBrush::CScriptedMapBrush(const CScriptedMapBrushInit&) 0x0082E600 calls it for Cells with the
//   game area's width * height; the 20-byte element is Ego_r.pdb CScriptedMapBrush::CScriptedMapCell (Height +0,
//   Theme[3] +4, ThemeBlend[2] +0x10). The catalog label "CTextureManagerPoolInfo_Vector_Constructor" is propagated.
// Callees: malloc 0x00BFEA0E (`n ? malloc(n * 20) : 0`), __uninitialized_fill_n(first, n, const T&, const
//   __false_type&) 0x0082F240 (link-time __fastcall: first in ecx, n in edx).
// Shape: STLport 4.x `explicit vector(size_type n) : _Vector_base(n, allocator_type()) { _M_finish =
//   __uninitialized_fill_n(_M_start, n, _Tp(), __false_type()); }` -- _Vector_base zeroes the three pointers before the
//   allocation; `_Tp()` of the POD cell is five zero dwords in the frame; the tag is homed on the dead `n` slot (a
//   same-TU noinline body for the fill: only the constructor is graded); explicit instantiation of the constructor.
extern "C" void* __cdecl malloc(unsigned int size);   // 0x00BFEA0E
inline void* operator new(unsigned int, void* p) { return p; }

namespace std {
struct __false_type { __false_type() { } };

template <class T> inline void _Construct(T* p, const T& value) { new (p) T(value); }

template <class _ForwardIter, class _Size, class _Tp>
__declspec(noinline) _ForwardIter __fastcall __uninitialized_fill_n(_ForwardIter __first, _Size __n, const _Tp& __x,
                                                                    const __false_type&)   // 0x0082F240
{
    _ForwardIter __cur = __first;
    for (; __n > 0; --__n, ++__cur)
        _Construct(&*__cur, __x);
    return __cur;
}

template <class _Tp> class allocator {
public:
    allocator() { }
    allocator(const allocator&) { }
    _Tp* allocate(unsigned int __n) { return __n != 0 ? (_Tp*)malloc(__n * sizeof(_Tp)) : 0; }
};

template <class _Value, class _Tp, class _MaybeReboundAlloc>
class _STLP_alloc_proxy : public _MaybeReboundAlloc {
public:
    _Value _M_data;
    _STLP_alloc_proxy(const _MaybeReboundAlloc& __a, _Value __p) : _MaybeReboundAlloc(__a), _M_data(__p) { }
};

template <class _Tp, class _Alloc> class _Vector_base {
public:
    _Vector_base(unsigned int __n, const _Alloc& __a)
        : _M_start(0), _M_finish(0), _M_end_of_storage(__a, 0)
    {
        _M_start = _M_end_of_storage.allocate(__n);
        _M_finish = _M_start;
        _M_end_of_storage._M_data = _M_start + __n;
    }
    _Tp* _M_start;                                      // +0x00
    _Tp* _M_finish;                                     // +0x04
    _STLP_alloc_proxy<_Tp*, _Tp, _Alloc> _M_end_of_storage;   // +0x08
};

template <class _Tp, class _Alloc = allocator<_Tp> >
class vector : public _Vector_base<_Tp, _Alloc> {
public:
    typedef _Alloc allocator_type;
    explicit vector(unsigned int __n)
        : _Vector_base<_Tp, _Alloc>(__n, allocator_type())
    {
        this->_M_finish = __uninitialized_fill_n(this->_M_start, __n, _Tp(), __false_type());
    }
};
}

class CScriptedMapBrush {
public:
    class CScriptedMapCell {                            // Ego_r.pdb, 20 bytes
    public:
        float         Height;                           // +0x00
        unsigned long Theme[3];                         // +0x04
        unsigned char ThemeBlend[2];                    // +0x10
    };
};

template std::vector<CScriptedMapBrush::CScriptedMapCell>::vector(unsigned int);