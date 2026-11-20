#pragma optimize("s",on)
// std::fill<CIntelligentPointer<NParticleEngine::CParticleEmitter>*, CIntelligentPointer<NParticleEngine::CParticleEmitter> >(first, last, value)
//   -- STLport 4.6 fill: assign value to every element of [first, last) (8-byte CIntelligentPointer elements)
// Flags: /Gr
// Flags evidence: retail passes first / last in ecx / edx (`__fastcall`, `ret 4`); the repo spelling of STLport fill is the /Gr instance (0x00BBD380
//   carries the same `// Flags: /Gr`), the YI leaf its only caller 0x006568F2 binds.
// Leaf: ??$fill@PAV?$CIntelligentPointer@VCParticleEmitter@NParticleEngine@@@@V1@@std@@YIXPAV?$CIntelligentPointer@VCParticleEmitter@NParticleEngine@@@@0ABV1@@Z
// Retail: Fable.exe (TLC, VC7.1) @ 0x006556D3, 31 bytes, `__fastcall` (first in ecx, last in edx, the value on the stack, `ret 4`): per element
//   `push [esp+0Ch] ; mov ecx,esi ; call 0x0065533C ; add esi,8`.
// Identity (descriptive-by-library: STLport 4.6 fill, the repo's fastcall spelling in stlport/fable_containers.h, as the landed 0x00BBD380):
//   the only caller, vector<CIntelligentPointer<CParticleEmitter> >::_M_fill_insert 0x006568F2, binds exactly this spelling; the per-element
//   call is CIntelligentPointer<CParticleEmitter>::operator= 0x0065533C (out of line: FABLE_RETAIL_CINTELLIGENTPOINTER_ASSIGN_OUTOFLINE).
// NF113 (PRIORITY q5007, binder of 0x0065533C): was the stand-in ?Destroy_range@@YIXPAUPair@@0PAUAllocT@@@Z ("_Destroy_range<allocator<pair<
//   unsigned long, CAnimationEntry>>>", catalog ForEachElem0x8_operator__006556d3) whose element call bound ?destroy_with@Pair@@; the body is an
//   assignment loop, not a destroy loop. Now the shared STLport fill on engine/CIntelligentPointer.h, emitted by explicit instantiation.
#define FABLE_RETAIL_CINTELLIGENTPOINTER
#define FABLE_RETAIL_CINTELLIGENTPOINTER_ASSIGN_OUTOFLINE
#include "engine/CIntelligentPointer.h"
#include "stlport/fable_containers.h"
namespace NParticleEngine { class CParticleEmitter; }

template void std::fill<CIntelligentPointer<NParticleEngine::CParticleEmitter>*, CIntelligentPointer<NParticleEngine::CParticleEmitter> >(
    CIntelligentPointer<NParticleEngine::CParticleEmitter>* first, CIntelligentPointer<NParticleEngine::CParticleEmitter>* last,
    const CIntelligentPointer<NParticleEngine::CParticleEmitter>& value);
