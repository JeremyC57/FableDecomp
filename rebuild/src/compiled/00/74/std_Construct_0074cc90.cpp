// std::_Construct(CIntelligentPointer<CThingCreatureBase>* __p, const CIntelligentPointer<CThingCreatureBase>& __val) -- STLport placement copy (nothing for a null __p): the inline copy constructor (CBaseIntelligentPointer() 0x00A01B10, the CIntelligentPointer<CThingCreatureBase> vtable, SetPItem(__val.GetPItem()))
// Flags: /Gr
// Leaf: ??$_Construct@V?$CIntelligentPointer@VCThingCreatureBase@@@@V1@@std@@YIXPAV?$CIntelligentPointer@VCThingCreatureBase@@@@ABV1@@Z
// Retail: Fable.exe @ 0x0074CC90, 39 bytes, __fastcall (/Gr STL TU: __p in ecx, __val in edx).
// Identity: STLport 4.6 _construct.h `_Construct(_T1* __p, const _T2& __val) { new(__p) _T1(__val); }`: the placement new's
//   `test esi,esi ; je` null test, then the inline copy constructor storing vtable 0x0123E7D8 (RTTI .?AV?$CIntelligentPointer@VCThingCreatureBase@@@@).
//   The catalog names (`_global::_Cons_val`, Dinkumware; `CIntelligentPointer<CThingCreatureBase>::CIntelligentPointer`) are propagated: a constructor has no
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
class CThingCreatureBase;

template void std::_Construct<CIntelligentPointer<CThingCreatureBase>, CIntelligentPointer<CThingCreatureBase> >(CIntelligentPointer<CThingCreatureBase>*, const CIntelligentPointer<CThingCreatureBase>&);
