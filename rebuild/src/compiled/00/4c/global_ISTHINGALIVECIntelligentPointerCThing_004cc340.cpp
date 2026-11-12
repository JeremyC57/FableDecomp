// IS_THING_ALIVE<CIntelligentPointer<CThing> >(const CIntelligentPointer<CThing>& thing) -- GetPItem @ 0x00A01B50 twice: null, then the dead bit (+0x91 & 1)
// Leaf: ??$IS_THING_ALIVE@V?$CIntelligentPointer@VCThing@@@@@@YI_NABV?$CIntelligentPointer@VCThing@@@@@Z
// Retail: Fable.exe (TLC, VC7.1) function @ 0x004CC340, oracle length 36 bytes.
// Retail bytes:  push esi ; mov esi,ecx ; call GetPItem ; test eax,eax ; je F ; mov ecx,esi ; call GetPItem ; test byte ptr [eax+91h],1 ; jne F ; mov al,1 ; pop esi ; ret ; F: xor al,al ; pop esi ; ret
// Name (agentNF22, Codex pass 21 queue): Ego_r.pdb + FableWin.pdb ??$IS_THING_ALIVE@V?$CIntelligentPointer@VCThing@@@@@@YI_NABV?$CIntelligentPointer@VCThing@@@@@Z,
//   the CIntelligentPointer<CThing> instantiation of the shared engine/ThingAlive.h template (`thing != 0`, then `thing->IsDead()`,
//   each through CBaseIntelligentPointer::GetPItem). Was the address-suffixed free stand-in IsThingAlive_004cc340(CIntelligentPointer<CThing>&).
// Shape: the template body kept out of line (FABLE_RETAIL_IS_THING_ALIVE_NOINLINE) and instantiated explicitly; fastcall, the
//   reference in ecx.
#define FABLE_RETAIL_CTHING
#define FABLE_RETAIL_CINTELLIGENTPOINTER
#define FABLE_RETAIL_CINTELLIGENTPOINTER_DEREFERENCE
#define FABLE_RETAIL_CINTELLIGENTPOINTER_GET_ORIGINAL_CHAIN
#define FABLE_RETAIL_CINTELLIGENTPOINTER_COMPARE_NULL_INT
#define FABLE_RETAIL_IS_THING_ALIVE
#define FABLE_RETAIL_IS_THING_ALIVE_NOINLINE
#include "engine/CThing.h"
#include "engine/CIntelligentPointer.h"
#include "engine/ThingAlive.h"

template bool FABLE_FASTCALL IS_THING_ALIVE<CIntelligentPointer<CThing> >(const CIntelligentPointer<CThing>& thing);
