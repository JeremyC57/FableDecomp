// CSimVoice::operator=(const CSimVoice& other) -- member-wise assignment: the CInitBaseClass Valid byte, then the Voices map (its sorted vector assigned out of line, the empty comparator, the Dirty flag)
// Leaf: ??4CSimVoice@@QAEAAV0@ABV0@@Z
// Flags: none
// Retail: Fable.exe (TLC, VC7.1) function @ 0x0046313E, 43 bytes, thiscall `ret 4` (Ego_r.pdb ??4CSimVoice@@QAEAAV0@ABV0@@Z).
//   Called through the landed CSimVoicesDef::Copy 0x0046311F on its +0x3c member.
// Callees: std::vector<std::pair<EReactionSpeechType, long> >::operator= 0x00463169 (STLport 4.6, out of line in this TU).
// Layout: Ego_r.pdb CSimVoice : CInitBaseClass (vptr +0, Valid +4) { CVectorMap<EReactionSpeechType, long> Voices +8 }.
// Shape: a size-optimised TU (unaligned entry): optimize("s"); CInitBaseClass's implicit assignment copies Valid through cl;
//   the CVectorMap assignment is expanded (FABLE_RETAIL_CVECTORMAP_ASSIGN, as 0x0045CFAD CSoundMap::operator=), the Dirty byte
//   copied through al before the pops.
#pragma optimize("s", on)
#define FABLE_RETAIL_STLP_VECTOR_ASSIGN
#define FABLE_RETAIL_CVECTORMAP_ASSIGN
#include "engine/CInitBaseClass.h"
#include "engine/CVectorMap.h"
enum EReactionSpeechType { };
class CSimVoice : public CInitBaseClass {
public:
    CVectorMap<EReactionSpeechType, long> Voices;          // +0x08
    CSimVoice& operator=(const CSimVoice& other);
};

CSimVoice& CSimVoice::operator=(const CSimVoice& other)
{
    CInitBaseClass::operator=(other);
    Voices = other.Voices;
    return *this;
}