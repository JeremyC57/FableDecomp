// CTCNoise::SetCreator(const CThing& creator) -- point the noise's Creator (CIntelligentPointer<const CThing>, +0x48) at the thing
// Leaf: ?SetCreator@CTCNoise@@QAEXABVCThing@@@Z
// Flags: none
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00749880, oracle length 71 bytes.
// Grade: landed -- byte-exact (RELOCATION_MATCH) and behaviour PASS under the Linux lane (Wine + VC7.1 cl.exe).
// Name (agentNF23, name-fix queue land1011cx; was the stand-in _unattributed::AssignTarget_00749880): Ego_r.pdb 0x0075B900 and
//   FableWin.pdb 0x02599030 ?SetCreator@CTCNoise@@QAEXABVCThing@@@Z, donor-adjacent to GetCreator (Ego_r 0x0075B950) exactly as
//   retail 0x00749880 sits before the landed CTCNoise::GetCreator 0x007498D0; the sole caller CThingNoise::SetCreator 0x008386E0
//   (landed) calls it on the interface-0x37 component with its const CThing&; the body writes +0x48, Ego_r's Creator.
//
// Retail bytes:  sub esp,8 ; push esi ; mov esi,ecx ; lea ecx,[esp+4] ; call CBaseIntelligentPointer::CBaseIntelligentPointer ; mov eax,[esp+10h] ; push eax ; lea ecx,[esp+8] ; mov dword ptr [esp+8],offset CIntelligentPointer::`vftable' ; call SetPItem ; lea ecx,[esp+4] ; call GetPItem ; push eax ; lea ecx,[esi+0x48] ; call SetPItem ; lea ecx,[esp+4] ; call ~CBaseIntelligentPointer ; pop esi ; add esp,8 ; ret 4
// `Creator = &creator;`: the temporary CIntelligentPointer<const CThing>(const CThing*) (base ctor 0x00A01B10, vptr 0x01237F1C,
// SetPItem 0x00A01B90), the inline operator= (GetPItem 0x00A01B50 on the temporary, SetPItem on Creator), then the temporary's
// destructor 0x00A01C10. Headers: engine/CTCNoise.h (FABLE_RETAIL_HEARING branch, as the landed CTCNoise::FrameUpdate 0x00749860),
// engine/CIntelligentPointer.h (FABLE_RETAIL_CINTELLIGENTPOINTER + _ASSIGN).
#define FABLE_RETAIL_HEARING
#define FABLE_RETAIL_CTCBASE
#define FABLE_RETAIL_CINTELLIGENTPOINTER
#define FABLE_RETAIL_CINTELLIGENTPOINTER_ASSIGN
#define FABLE_RETAIL_CTHING
#define FABLE_RETAIL_DEF_CLASS_BASE
#include "engine/ETCInterfaceType.h"
#include "engine/CThing.h"
#include "engine/CTCNoise.h"
void CTCNoise::SetCreator(const CThing& creator)
{
    Creator = &creator;
}
