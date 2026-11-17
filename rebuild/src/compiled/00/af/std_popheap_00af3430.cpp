// std::__pop_heap<std::pair<CCharString,NParticleEngine::CRegisteredPSCInfo>*, CKeyPairCompareLess<CCharString,NParticleEngine::CRegisteredPSCInfo>> (STLport 4.6, element std::pair<CCharString,NParticleEngine::CRegisteredPSCInfo>) -- SGI pop_heap core for the registered particle-component table: *result = *first, then sift `value` down from the root
// Flags: /Gr
// Leaf: ??$__pop_heap@PAU?$pair@VCCharString@@VCRegisteredPSCInfo@NParticleEngine@@@std@@U12@V?$CKeyPairCompareLess@VCCharString@@VCRegisteredPSCInfo@NParticleEngine@@@@H@std@@YIXPAU?$pair@VCCharString@@VCRegisteredPSCInfo@NParticleEngine@@@0@00U10@V?$CKeyPairCompareLess@VCCharString@@VCRegisteredPSCInfo@NParticleEngine@@@@PAH@Z
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00AF3430, 131 bytes.
// NF85: moved off the row-local CBasicString / CCharString / element / comparator models onto the shared headers:
//   stlport/fable_containers.h (the STLport 4.6 __pop_heap body, explicit instantiation; generator work/gen/algo.py), fable_string.h
//   (FABLE_RETAIL_CCHARSTRING_LESS_INLINE + FABLE_RETAIL_CBASICSTRING_LESS_BODY) and the element's own header; `// Flags: /Gr` (the
//   algorithm TU; the old row spelled __fastcall by hand). Element std::pair<CCharString,NParticleEngine::CRegisteredPSCInfo>, comparator CKeyPairCompareLess<CCharString,NParticleEngine::CRegisteredPSCInfo>.
#define FABLE_RETAIL_CCHARSTRING
#define FABLE_RETAIL_CCHARSTRING_LESS_INLINE
#define FABLE_RETAIL_CBASICSTRING_LESS_BODY
#define FABLE_RETAIL_STLP_HEAP
#include "stlport/fable_containers.h"
#include "fable_string.h"
#include "engine/CPSCBase.h"
// NParticleEngine::CRegisteredPSCInfo: no shared header exists; Ego_r.pdb members (struct_layouts_egor.tsv, 12 bytes), as NF82 0x00AF3290.
namespace NParticleEngine {
class CRegisteredPSCInfo {
public:
    CPSCBase* (__cdecl* AllocFunc)();                      // +0x00
    EPSCType ComponentType;                                // +0x04
    CCharString GUIName;                                   // +0x08
};
}
#include "engine/CVectorMap.h"
template void FABLE_STLP_FASTCALL std::__pop_heap(std::pair<CCharString,NParticleEngine::CRegisteredPSCInfo>*,std::pair<CCharString,NParticleEngine::CRegisteredPSCInfo>*,std::pair<CCharString,NParticleEngine::CRegisteredPSCInfo>*,std::pair<CCharString,NParticleEngine::CRegisteredPSCInfo>,CKeyPairCompareLess<CCharString,NParticleEngine::CRegisteredPSCInfo>,int*);
