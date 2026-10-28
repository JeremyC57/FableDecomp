// std::vector::_M_insert_overflow(CDebugScriptElement* position, const CDebugScriptElement& x, const __false_type&, unsigned int fill_len, bool atend) -- STLport non-POD growth of the camera's debug-script element vector: allocate old + max(old, n), copy the head, place the n copies, copy the tail, free the old block
// Leaf: ?_M_insert_overflow@?$vector@UCDebugScriptElement@CEngineCamera@@
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00B31B10, length 233 bytes, thiscall `ret 14h`.
// Callees: malloc @ 0x00BFEA0E (the inline allocate, `len ? malloc(len * 32) : 0`), __uninitialized_copy @ 0x00B31A30
//   (twice; catalog "_Move<CTCCameraPointScriptedSpline::CKeyCamera*,...>" is a propagated label), __uninitialized_fill_n
//   @ 0x00B31A90 (this chunk), free @ 0x00BFEA14 (the inline deallocate).
// Caller: vector<CDebugScriptElement>::push_back @ 0x00B31C10 (fill_len 1, atend true) <- CEngineCamera::
//   ConsoleAddDebugScript @ 0x00B317A0 (`DebugScript` at retail CEngineCamera +0x208).
// Shape: STLport 4.x verbatim, as the landed CQuake twin 0x0069D300: `old = size(); len = old + (max)(old, fill_len)`
//   (max by const reference), the tag temporaries homed in the atend argument slot (`lea ecx,[esp+28h]`), the
//   fill_len == 1 arm an inline _Construct (placement null test, eight dword moves), _M_clear = the trivial destroy loop
//   (folds away) + `if (_M_start) free(_M_start)`, then _M_set.  The two placement helpers are same-TU
//   __declspec(noinline) bodies.
// Element (Ego_r.pdb): CEngineCamera::CDebugScriptElement, 0x20 bytes (WorldPos, LookVector, FOV, TimeToNextElement).
// Label: catalog "Vector_InsertResizeAnimation" is behavioural.
extern "C" void* __cdecl malloc(unsigned int size);         // 0x00BFEA0E
extern "C" void __cdecl free(void* p);                      // 0x00BFEA14
inline void* __cdecl operator new(unsigned int, void* where) throw() { return where; }

class C3DVector {
public:
    float X, Y, Z;
    C3DVector(const C3DVector& o) : X(o.X), Y(o.Y), Z(o.Z) { }
};

class CEngineCamera {
public:
    struct CDebugScriptElement {                           // PDB sizeof 0x20
        C3DVector WorldPos;                                 // +0x00
        C3DVector LookVector;                               // +0x0C
        float     FOV;                                      // +0x18
        float     TimeToNextElement;                        // +0x1C
    };
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
                                                                  const __false_type&)   // 0x00B31A30 (same-TU body)
{
    _ForwardIter cur = result;
    for (; first != last; ++first, ++cur)
        _Construct(&*cur, *first);
    return cur;
}
template <class _ForwardIter, class _Size, class _Tp>
__declspec(noinline) _ForwardIter __fastcall __uninitialized_fill_n(_ForwardIter first, _Size n, const _Tp& x,
                                                                    const __false_type&)   // 0x00B31A90 (same-TU body)
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
    protected:
    void _M_clear()
    {
        _Destroy(this->_M_start, this->_M_finish);
        this->_M_end_of_storage.deallocate(this->_M_start, this->_M_end_of_storage._M_data - this->_M_start);
    }
    public:
    protected:
    void _M_set(T* s, T* f, T* e)
    {
        this->_M_start = s;
        this->_M_finish = f;
        this->_M_end_of_storage._M_data = e;
    }
    public:
    protected:
    void _M_insert_overflow(T* position, const T& x, const __false_type&, unsigned int fill_len, bool atend);
    public:
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

template void std::vector<CEngineCamera::CDebugScriptElement>::_M_insert_overflow(CEngineCamera::CDebugScriptElement*,
    const CEngineCamera::CDebugScriptElement&, const std::__false_type&, unsigned int, bool);