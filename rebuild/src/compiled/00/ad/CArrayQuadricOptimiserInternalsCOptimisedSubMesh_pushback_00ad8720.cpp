// CArray<QuadricOptimiserInternals::COptimisedSubMesh>::push_back(const QuadricOptimiserInternals::COptimisedSubMesh& x) -- STLport push_back: copy-construct x at finish (placement new's null test, each member's out-of-line copy constructor) when there is room, else grow through _M_insert_overflow(finish, x, __false_type, 1, true)
// Leaf: ?push_back@?$CArray@VCOptimisedSubMesh@QuadricOptimiserInternals@@@@QAEXABVCOptimisedSubMesh@QuadricOptimiserInternals@@@Z
// Retail: Fable.exe (TLC, VC7.1) @ 0x00ad8720, 78 bytes, thiscall, `ret 4`.
// Identity (probable): the element copy is the two landed vector copy constructors 0x00AD26D0 (vector<CCountedPointer<COptimisedPrimitive> >) and 0x00AD2730 (vector<COptimisedBone>) at +0 / +0xC = Ego_r.pdb QuadricOptimiserInternals::COptimisedSubMesh {Primitives; Bones}; Ego_r.pdb defines ?push_back@?$CArray@VCOptimisedSubMesh@QuadricOptimiserInternals@@@@...; no caller (unreferenced).
// Callees: std::vector<CCountedPointer<COptimisedPrimitive> >::vector(const vector&) 0x00AD26D0 and std::vector<COptimisedBone>::vector(const vector&) 0x00AD2730 (both landed), vector<COptimisedSubMesh>::_M_insert_overflow 0x00AD83D0.
// Shape (family L007, representative 0x0051C800): STLport 4.6 vector::push_back verbatim, inline in the CArray member;
//   the record's implicit copy constructor copies its two members out of line (ebx = &x, then &x + member offset); the
//   empty tag of the overflow call is homed on x's argument slot.
inline void* operator new(unsigned int, void* p) { return p; }
namespace std {
struct __false_type { __false_type() { } };
template <class _T1, class _T2> inline void _Construct(_T1* __p, const _T2& __val) { new (__p) _T1(__val); }
template <class _Tp> class allocator { public: allocator() { } };
template <class _Tp, class _Alloc = allocator<_Tp> > class vector {     // STLport 4.6, 12 bytes
public:
    _Tp* _M_start;                                         // +0x00
    _Tp* _M_finish;                                        // +0x04
    _Tp* _M_end_of_storage;                                // +0x08
    vector(const vector<_Tp, _Alloc>& __x);                // out of line per element type
    void _M_insert_overflow(_Tp* __position, const _Tp& __x, const __false_type&, unsigned int __fill_len,
                            bool __atend);                 // 0x00AD83D0
    void push_back(const _Tp& __x)
    {
        if (this->_M_finish != this->_M_end_of_storage) {
            _Construct(this->_M_finish, __x);
            ++this->_M_finish;
        }
        else
            _M_insert_overflow(this->_M_finish, __x, __false_type(), 1UL, true);
    }
};
}
template <class T> class CArray : public std::vector<T> {   // retail CArray<T> = the 12-byte STLport vector
public:
    void push_back(const T& x) { std::vector<T>::push_back(x); }
};
template <class T> class CCountedPointer;
namespace QuadricOptimiserInternals {
class COptimisedPrimitive;
class COptimisedBone;
class COptimisedSubMesh {                                   // Ego_r.pdb; retail 24 bytes
public:
    CArray<CCountedPointer<COptimisedPrimitive> > Primitives;   // +0x00
    CArray<COptimisedBone> Bones;                        // +0x0C
};
}
template void CArray<QuadricOptimiserInternals::COptimisedSubMesh>::push_back(const QuadricOptimiserInternals::COptimisedSubMesh&);