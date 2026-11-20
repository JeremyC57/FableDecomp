// IS_THING_DEAD<CIntelligentPointer<const CThing> >(const CIntelligentPointer<const CThing>& thing) -- GetPItem @ 0x00A01B50 twice: null, then the dead bit (+0x91 & 1)
// Leaf: ??$IS_THING_DEAD@V?$CIntelligentPointer@$$CBVCThing@@@@@@YI_NABV?$CIntelligentPointer@$$CBVCThing@@@@@Z
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00836BE0, oracle length 36 bytes.
// Retail bytes:  push esi ; mov esi,ecx ; call GetPItem ; test eax,eax ; je F ; mov ecx,esi ; call GetPItem ; test byte ptr [eax+91h],1 ; jne F ; xor al,al ; pop esi ; ret ; F: mov al,1 ; pop esi ; ret
// Name (NF196, 2026-11-20, q6446): Ego_r.pdb 0x0084D490 ??$IS_THING_DEAD@V?$CIntelligentPointer@$$CBVCThing@@@@@@YI_NABV?$CIntelligentPointer@$$CBVCThing@@@@@Z
//   (also FableWin.pdb 0x028996A0); its only retail caller, CAIStateGroup_DeedReactionRoot::Update 0x00960120 (`lea ebx,[ebp+14h] ;
//   mov ecx,ebx ; call 0x00836BE0`, 0x0096061E), passes ThingReactingTo, the Ego_r.pdb CIntelligentPointer<const CThing> member at +0x14.
//   Was the address-suffixed stand-in ?IsThingDead_00836be0@@YI_NAAV?$CIntelligentPointer@VCThing@@@@@Z (non-template, non-const T).
// Shape: the IS_THING_DEAD template in the nested-if form (the shared engine/ThingAlive.h `thing==0||IsDead()` spelling is the
//   catalog's other shape); instantiated explicitly; fastcall, the reference in ecx. Pointer recipe of the landed IS_THING_ALIVE
//   instance 0x004CC340.
#define FABLE_RETAIL_CTHING
#define FABLE_RETAIL_CINTELLIGENTPOINTER
#define FABLE_RETAIL_CINTELLIGENTPOINTER_DEREFERENCE
#define FABLE_RETAIL_CINTELLIGENTPOINTER_GET_ORIGINAL_CHAIN
#define FABLE_RETAIL_CINTELLIGENTPOINTER_COMPARE_NULL_INT
#include "engine/CThing.h"
#include "engine/CIntelligentPointer.h"
template<class T> bool FABLE_FASTCALL IS_THING_DEAD(const T& thing)
{
    if (thing != 0) {
        if (!thing->IsDead())
            return false;
    }
    return true;
}
template bool FABLE_FASTCALL IS_THING_DEAD<CIntelligentPointer<const CThing> >(const CIntelligentPointer<const CThing>& thing);
