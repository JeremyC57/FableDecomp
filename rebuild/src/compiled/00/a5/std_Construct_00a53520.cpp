// std::_Construct(CIntelligentPointer<C3DAnimation2>* __p, const CIntelligentPointer<C3DAnimation2>& __val) -- STLport placement copy (nothing for a null __p): the inline copy constructor (CBaseIntelligentPointer() 0x00A01B10, the CIntelligentPointer<C3DAnimation2> vtable, SetPItem(__val.GetPItem()))
// Flags: /Gr
// Leaf: ??$_Construct@V?$CIntelligentPointer@VC3DAnimation2@@@@V1@@std@@YIXPAV?$CIntelligentPointer@VC3DAnimation2@@@@ABV1@@Z
// Retail: Fable.exe @ 0x00A53520, 39 bytes, __fastcall (/Gr STL TU: __p in ecx, __val in edx).
// Identity: STLport 4.6 _construct.h `_Construct(_T1* __p, const _T2& __val) { new(__p) _T1(__val); }`: the placement new's
//   `test esi,esi ; je` null test, then the inline copy constructor storing vtable 0x0129D50C (RTTI .?AV?$CIntelligentPointer@VC3DAnimation2@@@@).
//   The catalog names (`_global::_Cons_val`, Dinkumware; `CIntelligentPointer<C3DAnimation2>::CIntelligentPointer`) are propagated: a constructor has no
//   null test. (Both PDBs spell Dinkumware `_Construct<T, const T&>`; retail's STL is STLport, whose template arguments are <T, T>.)
// Callees: CBaseIntelligentPointer::CBaseIntelligentPointer 0x00A01B10 (Ego_r.pdb / FableWin.pdb ??0CBaseIntelligentPointer@@QAE@XZ),
//   GetPItem 0x00A01B50, SetPItem 0x00A01B90.
// Model: the shared engine/CIntelligentPointer.h (FABLE_RETAIL_CINTELLIGENTPOINTER_COPY: the inline copy constructor) and the shared
//   STLport _Construct (stlport/fable_containers.h).
#define FABLE_RETAIL_BASE_OBJECT
#define FABLE_RETAIL_CINTELLIGENTPOINTER
#define FABLE_RETAIL_CINTELLIGENTPOINTER_COPY
#include "engine/CBaseObject.h"
#include "engine/CIntelligentPointer.h"
#include "stlport/fable_containers.h"
class C3DAnimation2;

template void std::_Construct<CIntelligentPointer<C3DAnimation2>, CIntelligentPointer<C3DAnimation2> >(CIntelligentPointer<C3DAnimation2>*, const CIntelligentPointer<C3DAnimation2>&);
