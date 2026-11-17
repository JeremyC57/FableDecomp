// std::__push_heap<std::pair<CCharString,NParticleEngine::CRegisteredPSCInfo>*, CKeyPairCompareLess<CCharString,NParticleEngine::CRegisteredPSCInfo>> (STLport 4.6, element std::pair<CCharString,NParticleEngine::CRegisteredPSCInfo>) -- heap sift-up of the particle-system component registry
// Flags: /Gr
// Leaf: ??$__push_heap@PAU?$pair@VCCharString@@VCRegisteredPSCInfo@NParticleEngine@@@std@@HU12@V?$CKeyPairCompareLess@VCCharString@@VCRegisteredPSCInfo@NParticleEngine@@@@@std@@YIXPAU?$pair@VCCharString@@VCRegisteredPSCInfo@NParticleEngine@@@0@HHU10@V?$CKeyPairCompareLess@VCCharString@@VCRegisteredPSCInfo@NParticleEngine@@@@@Z
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00AF2F10, 193 bytes, `ret 0x18`.
// NF82 (q11007): moved off the row-local CBasicString (its own copy of the operator< body, or a stand-in CBasicStringChar compare) /
//   CCharString / element / comparator models onto the shared headers: stlport/fable_containers.h (the STLport 4.6 __push_heap body, explicit
//   instantiation; generator work/gen/algo.py), fable_string.h (FABLE_RETAIL_CCHARSTRING_LESS_INLINE + FABLE_RETAIL_CBASICSTRING_LESS_BODY:
//   the string compare binds Ego_r.pdb ??M?$CBasicString@D@@QBE_NABV0@@Z) and the element's own header; `// Flags: /Gr` (the algorithm
//   TU; the old row spelled __fastcall by hand). Element std::pair<CCharString,NParticleEngine::CRegisteredPSCInfo>, comparator CKeyPairCompareLess<CCharString,NParticleEngine::CRegisteredPSCInfo>.
#define FABLE_RETAIL_CCHARSTRING
#define FABLE_RETAIL_CCHARSTRING_LESS_INLINE
#define FABLE_RETAIL_CBASICSTRING_LESS_BODY
#define FABLE_RETAIL_STLP_DECAL_HEAP
#include "stlport/fable_containers.h"
#include "fable_string.h"
#include "engine/CPSCBase.h"
// NParticleEngine::CRegisteredPSCInfo: no shared header exists; Ego_r.pdb members (struct_layouts_egor.tsv, 12 bytes).
namespace NParticleEngine {
class CRegisteredPSCInfo {
public:
    CPSCBase* (__cdecl* AllocFunc)();                      // +0x00
    EPSCType ComponentType;                                // +0x04
    CCharString GUIName;                                   // +0x08
};
}
#include "engine/CVectorMap.h"
template void std::__push_heap(std::pair<CCharString,NParticleEngine::CRegisteredPSCInfo>*,int,int,std::pair<CCharString,NParticleEngine::CRegisteredPSCInfo>,CKeyPairCompareLess<CCharString,NParticleEngine::CRegisteredPSCInfo>);
