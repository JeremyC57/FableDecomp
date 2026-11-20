// CIntelligentPointer<NParticleEngine::CParticleEmitter>::operator=(const CIntelligentPointer& other) -- smart-pointer copy assignment:
//   SetPItem(other.GetPItem()); return *this
// Leaf: ??4?$CIntelligentPointer@VCParticleEmitter@NParticleEngine@@@@QAEAAV0@ABV0@@Z
// Retail: Fable.exe (TLC, VC7.1) @ 0x0065533C, 26 bytes: `push esi ; mov esi,ecx ; mov ecx,[esp+8] ; call GetPItem 0x00A01B50 ; push eax ;
//   mov ecx,esi ; call SetPItem 0x00A01B90 ; mov eax,esi ; pop esi ; ret 4`.
// Identity: FableWin.pdb CIntelligentPointer<T>::operator=(const CIntelligentPointer&) (QAEAAV0@ABV0@@Z); T = NParticleEngine::CParticleEmitter:
//   all nine shared-header callers in REFS (0x006445BA, 0x006451DD, 0x0064546F, 0x00645CE9, 0x00645F8D, 0x00646580, 0x00652FA4, 0x006557FB,
//   0x00655855) bind this instance's spelling (RELOCATION_MATCH), and the fill 0x006556D3 over vector<CIntelligentPointer<CParticleEmitter> >
//   (_M_fill_insert 0x006568F2) calls it per element.
// Body: engine/CIntelligentPointer.h FABLE_RETAIL_CINTELLIGENTPOINTER_ASSIGN + _ASSIGN_DIRECT (SetPItem(other.GetPItem()), the retail
//   order), emitted out of line by explicit instantiation.
// NF113 (PRIORITY q5007): was the untemplated stand-in CIntelligentPointer::operator=(CIntelligentPointer*) (??4CIntelligentPointer@@QAEPAV0@PAV0@@Z)
//   over local CBaseIntelligentPointer / CIntelligentPointer models.
#define FABLE_RETAIL_CINTELLIGENTPOINTER
#define FABLE_RETAIL_CINTELLIGENTPOINTER_ASSIGN
#define FABLE_RETAIL_CINTELLIGENTPOINTER_ASSIGN_DIRECT
#include "engine/CIntelligentPointer.h"
namespace NParticleEngine { class CParticleEmitter; }

template CIntelligentPointer<NParticleEngine::CParticleEmitter>& CIntelligentPointer<NParticleEngine::CParticleEmitter>::operator=(
    const CIntelligentPointer<NParticleEngine::CParticleEmitter>& other);
