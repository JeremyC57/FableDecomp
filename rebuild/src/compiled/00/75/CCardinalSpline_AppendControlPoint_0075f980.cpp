// CCardinalSpline::AppendControlPoint(const C3DVector& point) -- push a control point onto the spline's point array and return its index
// Leaf: ?AppendControlPoint@
// Retail: Fable.exe (TLC, VC7.1) function @ 0x0075F980, 92 bytes, `ret 4`. No direct caller or reference (an
//   unreferenced out-of-line copy kept without /OPT:REF).
// Callee: vector<C3DVector>::_M_insert_overflow(pos, x, tag, 1, true) 0x0066E370 (the shared C3DVector growth
//   path, 65 call sites).
// Identity: Ego_r.pdb names `long CCardinalSpline::AppendControlPoint(const C3DVector&)` (?AppendControlPoint@
//   CCardinalSpline@@QAEJABVC3DVector@@@Z), the only PDB method returning long from a const C3DVector& on a class
//   whose first member is a C3DVector array (CCardinalSpline::ControlPoints, CArray<C3DVector> +0; the retail
//   STLport vector is 12 bytes). The catalog's "Vector_PushBackOrInsert" is a descriptive auto label.
// Shape: STLport `push_back` inlined (`_Construct` = placement new with its null test, three dword moves, `add
//   [esi+4],0Ch`; else `_M_insert_overflow(_M_finish, x, tag, 1, true)` with the empty tag homed in the dead `point`
//   slot), then `size() - 1` from the signed 0x2AAAAAAB divide by 12.
inline void* operator new(unsigned int, void* where) { return where; }

class C3DVector {
public:
    float X, Y, Z;
};

struct __true_type { __true_type() { } };

template <class _T1, class _T2>
inline void _Construct(_T1* __p, const _T2& __val) { new (__p) _T1(__val); }

template <class _Tp> struct SAllocProxy {                   // STLport _STLP_alloc_proxy: the end-of-storage pointer
    _Tp* _M_data;
};

template <class _Tp> class CArray {                         // retail STLport vector (12 bytes)
public:
    _Tp* _M_start;                                          // +0x00
    _Tp* _M_finish;                                         // +0x04
    SAllocProxy<_Tp> _M_end_of_storage;                     // +0x08
    void _M_insert_overflow(_Tp* __position, const _Tp& __x, const __true_type&, unsigned int __fill_len, bool __atend);   // 0x0066E370
    unsigned int size() const { return _M_finish - _M_start; }
    void push_back(const _Tp& __x)
    {
        if (this->_M_finish != this->_M_end_of_storage._M_data) {
            _Construct(this->_M_finish, __x);
            ++this->_M_finish;
        }
        else
            _M_insert_overflow(this->_M_finish, __x, __true_type(), 1UL, true);
    }
};

class CCardinalSpline {
public:
    CArray<C3DVector> ControlPoints;                        // +0x00
    long AppendControlPoint(const C3DVector& point);
};

long CCardinalSpline::AppendControlPoint(const C3DVector& point)
{
    ControlPoints.push_back(point);
    return ControlPoints.size() - 1;
}