// std::vector<CCamera>::_M_insert_overflow(CCamera* position, const CCamera& x, const __false_type&, unsigned int fill_len, bool atend) -- STLport non-POD growth of the engine's camera vectors: allocate old + max(old, n), copy the head, copy-construct the n new cameras, copy the tail, free the old block
// Leaf: ?_M_insert_overflow@?$vector@VCCamera@@
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00B29840, length 211 bytes, thiscall `ret 14h`.
// Callees: malloc @ 0x00BFEA0E (the inline allocate, `len ? malloc(len * 56) : 0`), __uninitialized_copy @ 0x00B28ED0
//   (twice; catalog "_Move<CTCCameraPointScriptedSpline::CKeyCamera*,...>" is a propagated label), CCamera::CCamera(
//   const CCamera&) @ 0x0049D380 (the fill_len == 1 arm; catalog "CCamera"), __uninitialized_fill_n @ 0x00B28F60
//   (catalog "GFExplicitNew<N3DPrimitive::CAnimatedBlock>" is a propagated label), free @ 0x00BFEA14.
// Callers: push_back sites in CEngine::LoadNewScene @ 0x00B277A0 (call 0x00B2780C), CEngine::CutsceneAddCameraAngle @
//   0x00B27D50 (call 0x00B27D85; Ego_r.pdb ?CutsceneAddCameraAngle@CEngine@@UAEXABVCCamera@@@Z, CArray<CCamera>
//   PreloadCameras) and 0x00B2AA70 -- all fill_len 1, atend true.
// Shape: STLport 4.x verbatim (the landed CQuake twin 0x0069D300): `old = size()` (the /56 signed magic divide),
//   `len = old + (max)(old, fill_len)` by const reference, the tag temporaries homed in the atend argument slot, the
//   fill_len == 1 arm an inline _Construct whose placement new calls the out-of-line copy constructor after a null
//   test, _M_clear = the trivial destroy loop (folds away) + `if (_M_start) free(_M_start)`, then _M_set.  The two
//   placement helpers are same-TU __declspec(noinline) bodies.
// Element (Ego_r.pdb): CCamera, 0x38 bytes = C3DVector WorldPosition +0, CRightHandedSet RHSet +0x0C, bool HeightLocked
//   +0x24, bool ViewVecZLocked +0x25, FOVFlags +0x28, HorizontalFOV +0x2C, VerticalFOV +0x30, Zoom +0x34.
// Label: catalog "CTCCameraPointScriptedSpline_InsertKeyCamera" is a propagated label (CKeyCamera is also 0x38 bytes).
extern "C" void* __cdecl malloc(unsigned int size);         // 0x00BFEA0E
extern "C" void __cdecl free(void* p);                      // 0x00BFEA14
inline void* __cdecl operator new(unsigned int, void* where) throw() { return where; }

class C3DVector {
public:
    float X, Y, Z;
};

class CRightHandedSet {
public:
    C3DVector Up;                                           // +0x00
    C3DVector Forward;                                      // +0x0C
};

class CCamera {                                             // PDB sizeof 0x38
public:
    C3DVector       WorldPosition;                          // +0x00
    CRightHandedSet RHSet;                                  // +0x0C
    bool            HeightLocked;                           // +0x24
    bool            ViewVecZLocked;                         // +0x25
    unsigned long   FOVFlags;                               // +0x28
    float           HorizontalFOV;                          // +0x2C
    float           VerticalFOV;                            // +0x30
    float           Zoom;                                   // +0x34
    CCamera(const CCamera& other);                          // 0x0049D380
};

namespace std {
struct __false_type { __false_type() {} };
template <class T> inline const T& (max)(const T& a, const T& b) { return a < b ? b : a; }
template <class T> inline void _Destroy(T* p) { p->~T(); }
template <class _ForwardIterator> inline void _Destroy(_ForwardIterator first, _ForwardIterator last)
{
    for (; first != last; ++first)
        _Destroy(&*first);
}
template <class _T1, class _T2> inline void _Construct(_T1* __p, const _T2& __val) { new (__p) _T1(__val); }

template <class T> class allocator {
public:
    T* allocate(unsigned int n) { return n != 0 ? (T*)malloc(n * sizeof(T)) : 0; }
    void deallocate(T* p, unsigned int) { if (p != 0) free(p); }
};
template <class T> struct _STLP_alloc_proxy : allocator<T> {
    T* _M_data;
};
template <class T> class _Vector_base {
public:
    T* _M_start;
    T* _M_finish;
    _STLP_alloc_proxy<T> _M_end_of_storage;
};

#pragma auto_inline(off)
template <class _InputIter, class _ForwardIter>
__declspec(noinline) _ForwardIter __fastcall __uninitialized_copy(_InputIter first, _InputIter last, _ForwardIter result,
                                                                  const __false_type&)   // 0x00B28ED0 (same-TU body)
{
    _ForwardIter cur = result;
    for (; first != last; ++first, ++cur)
        _Construct(&*cur, *first);
    return cur;
}
template <class _ForwardIter, class _Size, class _Tp>
__declspec(noinline) _ForwardIter __fastcall __uninitialized_fill_n(_ForwardIter first, _Size n, const _Tp& x,
                                                                    const __false_type&)   // 0x00B28F60 (same-TU body)
{
    _ForwardIter cur = first;
    for (; n > 0; --n, ++cur)
        _Construct(&*cur, x);
    return cur;
}
#pragma auto_inline(on)

template <class T> class vector : public _Vector_base<T> {
public:
    unsigned int size() const { return unsigned int(this->_M_finish - this->_M_start); }
    void _M_clear()
    {
        _Destroy(this->_M_start, this->_M_finish);
        this->_M_end_of_storage.deallocate(this->_M_start, this->_M_end_of_storage._M_data - this->_M_start);
    }
    void _M_set(T* s, T* f, T* e)
    {
        this->_M_start = s;
        this->_M_finish = f;
        this->_M_end_of_storage._M_data = e;
    }
    void _M_insert_overflow(T* position, const T& x, const __false_type&, unsigned int fill_len, bool atend);
};

template <class T>
void vector<T>::_M_insert_overflow(T* position, const T& x, const __false_type&, unsigned int fill_len, bool atend)
{
    const unsigned int old_size = size();
    const unsigned int len = old_size + (max)(old_size, fill_len);
    T* new_start = this->_M_end_of_storage.allocate(len);
    T* new_finish = new_start;
    new_finish = __uninitialized_copy(this->_M_start, position, new_start, __false_type());
    if (fill_len == 1) {
        _Construct(new_finish, x);
        ++new_finish;
    }
    else
        new_finish = __uninitialized_fill_n(new_finish, fill_len, x, __false_type());
    if (!atend)
        new_finish = __uninitialized_copy(position, this->_M_finish, new_finish, __false_type());
    _M_clear();
    _M_set(new_start, new_finish, new_start + len);
}
}

template void std::vector<CCamera>::_M_insert_overflow(CCamera*, const CCamera&, const std::__false_type&, unsigned int,
    bool);