// std::_Construct<CBezierCurve, CBezierCurve>(CBezierCurve*, const CBezierCurve&) -- STLport placement copy of one flight-path
//   Bezier curve: null-checked placement new, then the implicit memberwise copy (four C3DVectors = twelve single moves)
// Retail: Fable.exe @ 0x00913590, 75 bytes; __fastcall (ecx = element slot, edx = source element).
// Identity: the element is CBezierCurve (Ego_r.pdb, 48 bytes = Point1..Point4), not CGPUScratchBufferTextureManager::
//   CResourceEntry (a propagated byte-twin label): the only caller is deque<CBezierCurve>::push_back 0x00913E70, whose TU
//   (0x009137F0 _M_initialize_map, 0x00913930 __destroy_aux, 0x00913A20 clear, 0x00913D60 _M_push_back_aux_v) serves the
//   CAIStateGroup_FlyingThingBase flight-path deque (Ego_r.pdb AddRandomBezierCurveToPath / CalculatePrettyPath take
//   deque<CBezierCurve>&); the neighbouring allocator 0x009135E0 allocates n * 0x30. FINDINGS chunk 406.
// Callees: none (inline memberwise copy).
// Leaf: ??$_Construct@VCBezierCurve@@V1@@std@@YIXPAVCBezierCurve@@ABV1@@Z
inline void* operator new(unsigned int, void* p) { return p; }   // placement new (STLport <new>)

class C3DVector {
public:
    float X;
    float Y;
    float Z;
    C3DVector(const C3DVector& v) : X(v.X), Y(v.Y), Z(v.Z) { }
};

class CBezierCurve {                            // Ego_r.pdb, 48 bytes
public:
    C3DVector Point1;                           // +0x00
    C3DVector Point2;                           // +0x0C
    C3DVector Point3;                           // +0x18
    C3DVector Point4;                           // +0x24
};

namespace std {
template <class _T1, class _T2>
void __fastcall _Construct(_T1* __p, const _T2& __val)
{
    new (__p) _T1(__val);
}
template void __fastcall _Construct<CBezierCurve, CBezierCurve>(CBezierCurve* __p, const CBezierCurve& __val);
}
