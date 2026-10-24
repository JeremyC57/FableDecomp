// _STL::__uninitialized_fill_n(CMinimapData* first, unsigned int n, const CMinimapData& x, const __false_type&) -- copy-construct n minimap records from one prototype into raw storage (the vector<CMinimapData> fill)
// Leaf: ??$__uninitialized_fill_n@PAVCMinimapData@@
// Retail: Fable.exe (TLC, VC7.1) @ 0x0082CC40, 191 bytes (+1 int3), link-time __fastcall `ret 8` (first in ecx, n in
//   edx, the value and the tag reference on the stack); returns the advanced cursor.
// Identity: the 0x3c-byte record is Ego_r.pdb CMinimapData (see the sibling __uninitialized_copy 0x0082CB70). The
//   catalog label "CEditPrimitiveStats_CopyPrimitiveArray" is a propagated/descriptive name.
// Lever: C2DExtentsI carries a memberwise user copy constructor; the implicit (bitwise) copy addresses Y off the
//   record base instead of the strength-reduced cursors (203v203 / 191v191 register-only diffs).
// Callees: vector<MinimapTheme>(const vector&) @ 0x0082C620, CSurface(const CSurface&) @ 0x009F2D60, CTexture(const
//   CTexture&) @ 0x009F9D00, malloc @ 0x00BFEA0E, __uninitialized_copy<CRGBColour> @ 0x0082C690 (__fastcall, landed).
// Shape: STLport `for (; n > 0; --n, ++cur) _Construct(&*cur, x);` (unsigned count: `test edx,edx ; jbe`) with
//   CMinimapData's implicit copy constructor inline, as the sibling 0x0082CB70. Same-TU noinline body for the colour
//   copy; the theme list's copy constructor stays a call (explicit-specialisation declaration).
extern "C" void* __cdecl malloc(unsigned int size);   // 0x00BFEA0E
inline void* operator new(unsigned int, void* p) { return p; }

struct IDirect3DSurface9;
struct IDirect3DTexture9;

class CSurface {                                        // Ego_r.pdb CSurface (16 bytes, vtable 0x0122F84C)
public:
    virtual ~CSurface();
    IDirect3DSurface9* PD3DSurface;                     // +0x04
    unsigned long      AllocationSource;                // +0x08
    void*              PAllocatedMemory;                // +0x0c
    CSurface(const CSurface& other);                    // 0x009F2D60
};
class CTexture {                                        // Ego_r.pdb CTexture (8 bytes)
public:
    IDirect3DTexture9* PD3DTexture;                     // +0x00
    unsigned long      ByteLength : 28;                 // +0x04
    unsigned long      AllocationSource : 4;
    CTexture(const CTexture& other);                    // 0x009F9D00
};
class CRGBColour {                                      // 4 bytes, user copy constructor (as the landed 0x0082C690)
public:
    unsigned char B, G, R, A;
    void Set(unsigned char r, unsigned char g, unsigned char b, unsigned char a) { R = r; G = g; B = b; A = a; }
    CRGBColour(const CRGBColour& other) { Set(other.R, other.G, other.B, other.A); }
};
class C2DExtentsI {                                     // Ego_r.pdb C2DExtentsI (8 bytes)
public:
    long X;                                             // +0x00
    long Y;                                             // +0x04
    C2DExtentsI(const C2DExtentsI& other) : X(other.X), Y(other.Y) { }
};

namespace _STL {
struct __false_type { __false_type() { } };

template <class T> inline void _Construct(T* p, const T& value) { new (p) T(value); }

// STLport __uninitialized_copy / __uninitialized_fill_n (__false_type forms), link-time __fastcall.
template <class _InputIter, class _ForwardIter>
__declspec(noinline) _ForwardIter __fastcall __uninitialized_copy(_InputIter __first, _InputIter __last,
                                                                  _ForwardIter __result, const __false_type&)
{
    _ForwardIter __cur = __result;
    for (; __first != __last; ++__first, ++__cur)
        _Construct(&*__cur, *__first);
    return __cur;
}

template <class _ForwardIter, class _Size, class _Tp>
__declspec(noinline) _ForwardIter __fastcall __uninitialized_fill_n(_ForwardIter __first, _Size __n, const _Tp& __x,
                                                                    const __false_type&)
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
    typedef const _Tp* const_pointer;
    unsigned int size() const { return unsigned int(this->_M_finish - this->_M_start); }
    _Alloc get_allocator() const { return (const _Alloc&)this->_M_end_of_storage; }
    vector(const vector<_Tp, _Alloc>& __x)
        : _Vector_base<_Tp, _Alloc>(__x.size(), __x.get_allocator())
    {
        this->_M_finish = __uninitialized_copy((const_pointer)__x._M_start, (const_pointer)__x._M_finish,
                                               this->_M_start, __false_type());
    }
};
}

class CMinimapData {                                    // Ego_r.pdb CMinimapData (retail 0x3c: two 12-byte vectors)
public:
    enum MinimapTheme { MINIMAP_THEME_FIRST };          // enumerators not needed
    C2DExtentsI                        MinimapExtents;          // +0x00
    _STL::vector<MinimapTheme>         MinimapThemeList;        // +0x08 (copy ctor out of line, 0x0082C620)
    CSurface                           MinimapSurface;          // +0x14
    CTexture                           MinimapTexture;          // +0x24
    unsigned long                      MaxMinimapThemeColours;  // +0x2c
    _STL::vector<CRGBColour>           ColourMinimapTheme;      // +0x30
};

// The theme list's copy constructor is a separate function in retail (0x0082C620); the colour list's is inline.
template <> _STL::vector<CMinimapData::MinimapTheme, _STL::allocator<CMinimapData::MinimapTheme> >::vector(
    const _STL::vector<CMinimapData::MinimapTheme, _STL::allocator<CMinimapData::MinimapTheme> >& __x);   // 0x0082C620

template CMinimapData* __fastcall _STL::__uninitialized_fill_n(CMinimapData*, unsigned int, const CMinimapData&, const _STL::__false_type&);