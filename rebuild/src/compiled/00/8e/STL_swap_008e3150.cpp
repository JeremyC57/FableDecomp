// std::swap(C3DVector& a, C3DVector& b) -- exchange two 12-byte vectors through a frame temporary
// Leaf: ??$swap@
// Retail: Fable.exe (TLC, VC7.1) function @ 0x008E3150, length 66 bytes (`ret`, no stack arguments).
// Identity: no direct callers; the body sits in the /Gr STLport algorithm block of the ice-troll icicle TU next to
//   CIsUsableIciclePoint::operator() 0x008E2E40 and the C3DVector __partition 0x008E3430 (whose inlined iter_swap is
//   this same swap). The catalog label "GFToScreen" is propagated from a byte twin; 0x008E2DB0 is the other copy (a second 12-byte instantiation of the same template).
// Shape: STLport 4.x `swap(T& a, T& b) { T tmp = a; a = b; b = tmp; }` from a /Gr TU (`__fastcall`: a in ecx, b in
//   edx). The temporary is copied word by word through eax into the 12-byte frame, `a = b` through esi from the
//   copied `b` pointer (`mov eax,edx`), and the temporary is written back through ecx/eax/ecx. C3DVector needs its
//   memberwise user copy constructor: with the implicit one VC7.1 keeps the temporary in ebx/esi/edi (no frame).
class C3DVector { public:
    float X, Y, Z;
    C3DVector(const C3DVector& o) : X(o.X), Y(o.Y), Z(o.Z) { }
};

namespace std {
template <class _Tp>
inline void __fastcall swap(_Tp& __a, _Tp& __b)
{
    _Tp __tmp = __a;
    __a = __b;
    __b = __tmp;
}
template void __fastcall swap<C3DVector>(C3DVector&, C3DVector&);
}