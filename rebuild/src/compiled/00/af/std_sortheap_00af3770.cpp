// std::sort_heap(pair<CCharString, NParticleEngine::CRegisteredPSCInfo>* first, last, CKeyPairCompareLess<CCharString, CRegisteredPSCInfo> comp) -- STLport sort_heap over the registered particle-system-component table
// Leaf: ??$sort_heap@PAU?$pair@VCCharString@@VCRegisteredPSCInfo@NParticleEngine@@@std@@V?$CKeyPairCompareLess@VCCharString@@VCRegisteredPSCInfo@NParticleEngine@@@@@std@@YIXPAU?$pair@VCCharString@@VCRegisteredPSCInfo@NParticleEngine@@@0@0V?$CKeyPairCompareLess@VCCharString@@VCRegisteredPSCInfo@NParticleEngine@@@@@Z
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00AF3770, length 107 bytes, `ret 4`.
// Identity: the catalog label `_Sort_heap<pair<CCharString,CCharString>*,...>` is propagated over the byte-identical
// skeleton. The callee is the landed SGI `__pop_heap` @ 0x00AF3430 over pair<CCharString,
// NParticleEngine::CRegisteredPSCInfo> (its caller list names 0x00AF37C1, inside this body); element model reused.
// Callees: __pop_heap @ 0x00AF3430 (out of line, __fastcall, `ret 1Ch`), CCharString copy constructor @ 0x0099EC30 (x2).
// Shape: /Gr SGI algorithm TU -- `while (last - first > 1) pop_heap(first, last--, comp)` with pop_heap and
// __pop_heap_aux inline (16-byte stride: `and -16 ; cmp 16`). The value copy in the argument area is the implicit
// pair copy: the key string, AllocFunc and ComponentType dwords, then the GUIName string (addressed off
// `lea eax,[esi+0Ch]`).
// NF87 (2026-11-17): the STLport sort_heap template (FABLE_RETAIL_STLP_HEAP + STLP_SORT_HEAP_BODY: the out-of-line __fastcall COMDAT)
//   by explicit instantiation, on the CRegisteredPSCInfo model of NF85's landed __pop_heap 0x00AF3430 (this instance; its own spelling
//   is the ??$ template one, which this row now binds). Its only retail E8 caller is 0x00AF39A1.
#define FABLE_RETAIL_CCHARSTRING
#define FABLE_RETAIL_CCHARSTRING_LESS_INLINE
#define FABLE_RETAIL_CBASICSTRING_LESS_BODY
#define FABLE_RETAIL_STLP_HEAP
#define FABLE_RETAIL_STLP_SORT_HEAP_BODY
#include "stlport/fable_containers.h"
#include "fable_string.h"
#include "engine/CPSCBase.h"
namespace NParticleEngine {
class CRegisteredPSCInfo {
public:
    CPSCBase* (__cdecl* AllocFunc)();                      // +0x00
    EPSCType ComponentType;                                // +0x04
    CCharString GUIName;                                   // +0x08
};
}
#include "engine/CVectorMap.h"
template void __fastcall std::sort_heap(std::pair<CCharString,NParticleEngine::CRegisteredPSCInfo>*,std::pair<CCharString,NParticleEngine::CRegisteredPSCInfo>*,CKeyPairCompareLess<CCharString,NParticleEngine::CRegisteredPSCInfo>);
