// NBBBMaths::CLinearSpline::AddControlPoint(const C3DVector& point) -- append a control point to ControlPoints
// Leaf: ?AddControlPoint@CLinearSpline@NBBBMaths@@QAEXABVC3DVector@@@Z
// Retail: Fable.exe (TLC, VC7.1) @ 0x00A580F0, 64 bytes, thiscall, `ret 4`.
// Identity (proven): Ego_r.pdb orders NBBBMaths::CLinearSpline::GetLength, GetPointWithParam, AddControlPoint(const C3DVector&), ProcessPoints; retail has GetLength 0x00A57EF0, GetPointWithParam 0x00A57F70, this function, then the spline point generator 0x00A58130; CLinearSpline::ControlPoints is the vector<C3DVector> at +0 that the body pushes onto; its only caller CTCLightningOrb::Initialise 0x007F26B0 builds the orb's CCountedPointer<NBBBMaths::CLinearSpline> PSpline point by point. It is a separate copy of the 0x0066E630 code because it is a separate symbol (no ICF).
// Callees: vector<C3DVector>::_M_insert_overflow(pointer, const value_type&, const __false_type&, size_type, bool) 0x0066E370.
// Shape (family M185, representative 0x0066E630): STLport `if (_M_finish != _M_end_of_storage._M_data) { _Construct(_M_finish,
//   __x); ++_M_finish; } else _M_insert_overflow(_M_finish, __x, __false_type(), 1UL, true);` -- the implicit 12-byte copy
//   (three dword moves through esi) under placement new's null test; the empty tag homed in the dead __x slot. Speed TU.
inline void* __cdecl operator new(unsigned int, void* p) { return p; }
class C3DVector {                                         // Ego_r.pdb, 12 bytes
public:
    float X, Y, Z;
};
namespace std {
struct __false_type { __false_type() { } };
template <class _Tp> class allocator { public: allocator() { } };
template <class _Tp, class _Alloc> class _STLP_alloc_proxy : public _Alloc { public: _Tp _M_data; };
template <class _T1, class _T2> inline void _Construct(_T1* __p, const _T2& __val) { new (__p) _T1(__val); }
template <class _Tp, class _Alloc> class _Vector_base {
public:
    _Tp* _M_start;
    _Tp* _M_finish;
    _STLP_alloc_proxy<_Tp*, _Alloc> _M_end_of_storage;
};
template <class _Tp, class _Alloc = allocator<_Tp> > class vector : public _Vector_base<_Tp, _Alloc> {
public:
    void push_back(const _Tp& __x)
    {
        if (this->_M_finish != this->_M_end_of_storage._M_data) {
            _Construct(this->_M_finish, __x);
            ++this->_M_finish;
        }
        else
            _M_insert_overflow(this->_M_finish, __x, __false_type(), 1UL, true);
    }
protected:
    void _M_insert_overflow(_Tp* __position, const _Tp& __x, const __false_type&, unsigned int __fill_len, bool __atend = false);   // 0x0066E370
};
}
namespace NBBBMaths {
class CLinearSpline {
public:
    void AddControlPoint(const C3DVector& point);
    std::vector<C3DVector> ControlPoints;                 // +0x00
    std::vector<C3DVector> ProcessedPoints;               // +0x0C
};
void CLinearSpline::AddControlPoint(const C3DVector& point)
{
    ControlPoints.push_back(point);
}
}