// C3DFrustrum::AddPoint(const C3DVector&, float) -- appends a (point, weight) pair to the frustum's point vector (STLport push_back: copy-construct in place, or reallocate through the out-of-line insert-overflow)
// Leaf: ?AddPoint@C3DFrustrum@
// Grade: RELOCATION_MATCH (near-miss retry agent901) -- a same-TU noinline _M_insert_overflow body (0x0084F5E0 stand-in with the STLport reallocation shape, tag unnamed) homes the __true_type in the dead weight slot and copy-propagates the weight
// Retail: Fable.exe (TLC, VC7.1) @ 0x0084E390, 111 bytes, `ret 8`.
// Identity: Ego_r.pdb ?AddPoint@C3DFrustrum@?A0x49b2e1a3@@QAEXABVC3DVector@@M@Z (a class in an anonymous namespace):
//   C3DFrustrum = Apex +0, Up +0xc, Points vector<pair<C3DVector, float> > +0x18, FOV, Focus. Its constructor
//   C3DFrustrum(const C3DVector&, const C3DVector&) is the landed 0x0084F4D0 (catalogued as "CAxisPointList_0084f4d0");
//   the caller 0x0084E590 builds a frustrum on the stack and adds points through here. The manifest label
//   "StdVector_PushBack" is descriptive.
// Callee: vector<pair<C3DVector, float> >::_M_insert_overflow(pointer pos, const value_type& x, const __true_type&,
//   size_type fill_len, bool atend) 0x0084F5E0 (thiscall, the tag homed in the dead weight argument slot).
// Layout: retail C3DFrustrum (STLport 12-byte vector): Apex +0, Up +0xc, Points +0x18 (start / finish / end of storage),
//   FOV +0x24, Focus +0x28 (the landed constructor zeroes +0x18..+0x24).
// Shape: the pair temporary is built in the frame from the point and the weight; push_back compares finish against the
//   end of storage, placement-constructs (null-checked) and bumps finish by 0x10; the in-place copy re-reads the weight
//   from its argument slot (the temporary's second is known to equal it).
#include <new>
extern "C" void* __cdecl malloc(unsigned int size);   // 0x00BFEA0E
extern "C" void __cdecl free(void* p);                // 0x00BFEA14

class C3DVector { public:
    float X, Y, Z;
    C3DVector(const C3DVector& other) : X(other.X), Y(other.Y), Z(other.Z) { }   // memberwise (landed 0x0084F4D0 model)
};

struct __true_type { __true_type() { } };

namespace std {
template <class T1, class T2> struct pair {
    T1 first;
    T2 second;
    pair(const T1& a, const T2& b) : first(a), second(b) { }
};

template <class T> struct _STLP_alloc_proxy { T _M_data; };

template <class T> class vector {                        // STLport vector: start / finish / end of storage
public:
    void push_back(const T& x)
    {
        if (_M_finish != _M_end_of_storage._M_data) {
            new (_M_finish) T(x);
            ++_M_finish;
        } else
            _M_insert_overflow(_M_finish, x, __true_type(), 1, true);
    }
    void _M_insert_overflow(T* pos, const T& x, const __true_type&, unsigned int fill_len, bool atend);   // 0x0084F5E0
    T* _M_start;
    T* _M_finish;
    _STLP_alloc_proxy<T*> _M_end_of_storage;
};

// _M_insert_overflow @ 0x0084F5E0: a same-TU stand-in with the STLport reallocation shape (GOTCHAS empty-tag rule: the
// callee that receives the empty __true_type gets a body, never inlined, the tag unnamed) -- VC7.1 then homes the tag
// in the dead weight slot and copies the weight from its argument slot as retail.
#pragma auto_inline(off)
template <class T>
__declspec(noinline) void vector<T>::_M_insert_overflow(T* pos, const T& x, const __true_type&, unsigned int fill_len, bool atend)
{
    unsigned int old_size = _M_finish - _M_start;
    unsigned int len = old_size + (old_size > fill_len ? old_size : fill_len);
    T* new_start = (T*)malloc(len * sizeof(T));
    T* new_finish = new_start;
    for (T* p = _M_start; p != pos; ++p, ++new_finish)
        new (new_finish) T(*p);
    for (unsigned int i = 0; i < fill_len; ++i, ++new_finish)
        new (new_finish) T(x);
    if (!atend)
        for (T* p = pos; p != _M_finish; ++p, ++new_finish)
            new (new_finish) T(*p);
    if (_M_start)
        free(_M_start);
    _M_start = new_start;
    _M_finish = new_finish;
    _M_end_of_storage._M_data = new_start + len;
}
#pragma auto_inline(on)
}

namespace {
class C3DFrustrum {
public:
    void AddPoint(const C3DVector& point, float weight);
    C3DVector Apex;                                      // +0x00
    C3DVector Up;                                        // +0x0c
    std::vector<std::pair<C3DVector, float> > Points;    // +0x18
    float     FOV;                                       // +0x24
    C3DVector Focus;                                     // +0x28
};

void C3DFrustrum::AddPoint(const C3DVector& point, float weight)
{
    Points.push_back(std::pair<C3DVector, float>(point, weight));
}
}

void (C3DFrustrum::*g_keepAddPoint)(const C3DVector&, float) = &C3DFrustrum::AddPoint;   // keeps the internal-linkage body emitted; not part of the retail function