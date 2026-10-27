// std::__uninitialized_copy(const CTCSmokeGenerator::CGenerationPos* first, const CTCSmokeGenerator::CGenerationPos* last, CTCSmokeGenerator::CGenerationPos* result, const __false_type&) -- STLport placement copy-construct each 20-byte record (the position, then the intelligent pointer's copy constructor) and return the advanced result
// Leaf: ??$__uninitialized_copy@PBVCGenerationPos@CTCSmokeGenerator@@PAV12@@std@@YIPAVCGenerationPos@CTCSmokeGenerator@@PBV12@0PAV12@ABU__false_type@0@@Z
// Retail: Fable.exe (TLC, VC7.1) @ 0x00722c30, 90 bytes, __fastcall (first in ecx, last in edx, result + tag on the stack), `ret 8`.
// Identity (proven): its callers are vector<CTCSmokeGenerator::CGenerationPos>::operator= 0x00722F30 (twice, const sources; family L032) and the landed 0x00722D30; the non-const twin is 0x00722B70.
// Callees: CBaseIntelligentPointer::CBaseIntelligentPointer 0x00A01B10, GetPItem 0x00A01B50, SetPItem 0x00A01B90.
// Shape (family L051): STLport 4.6 `__uninitialized_copy(first, last, result, const __false_type&)` verbatim; the
//   implicit copy constructor runs C3DVector's inline user copy (three dwords, addressed off the pointer member) then runs CIntelligentPointer's copy constructor on +0x0C
//   (edi = the element's pointer member, walked alongside).
class CBaseObject;
class CBaseObjectPointer;
class CBaseIntelligentPointer {                            // Ego_r.pdb: vptr + PData, 8 bytes
public:
    CBaseIntelligentPointer();                            // 0x00A01B10
    virtual ~CBaseIntelligentPointer();                   // 0x00A01C10
    CBaseObject* GetPItem() const;                        // 0x00A01B50
    void SetPItem(CBaseObject* item);                     // 0x00A01B90
    CBaseObjectPointer* PData;                            // +0x04
};
template <class T> class CIntelligentPointer : public CBaseIntelligentPointer {   // vtable 0x01238C6C for CThing
public:
    CIntelligentPointer(const CIntelligentPointer& other) : CBaseIntelligentPointer() { SetPItem(other.CBaseIntelligentPointer::GetPItem()); }
};
class CThing;
class C3DVector {                                          // user copy constructor: ??0C3DVector@@QAE@ABV0@@Z (FableWin/Ego_d symbols)
public:
    float X, Y, Z;                                        // +0x00
    C3DVector() { }
    C3DVector(const C3DVector& v) : X(v.X), Y(v.Y), Z(v.Z) { }
};
class CTCSmokeGenerator {
public:
    class CGenerationPos {                                 // Ego_r.pdb, 20 bytes
    public:
        C3DVector Pos;                                    // +0x00
        CIntelligentPointer<CThing> PEmitter;             // +0x0C
    };
};
inline void* __cdecl operator new(unsigned int, void* where) { return where; }
namespace std {
struct __false_type { };
template <class _T1, class _T2> inline void _Construct(_T1* __p, const _T2& __val) { new (__p) _T1(__val); }
template <class _InputIter, class _ForwardIter>
_ForwardIter __fastcall __uninitialized_copy(_InputIter __first, _InputIter __last, _ForwardIter __result, const __false_type&)
{
    _ForwardIter __cur = __result;
    for (; __first != __last; ++__first, ++__cur)
        _Construct(&*__cur, *__first);
    return __cur;
}
}
template CTCSmokeGenerator::CGenerationPos* __fastcall std::__uninitialized_copy(const CTCSmokeGenerator::CGenerationPos*, const CTCSmokeGenerator::CGenerationPos*, CTCSmokeGenerator::CGenerationPos*, const std::__false_type&);