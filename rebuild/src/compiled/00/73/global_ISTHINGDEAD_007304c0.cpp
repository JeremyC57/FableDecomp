// IS_THING_DEAD<CIntelligentPointer<CThingAICreature> >(const CIntelligentPointer<CThingAICreature>& thing) -- GetPItem @ 0x00A01B50 twice: null, then the dead bit (+0x91 & 1)
// Leaf: ??$IS_THING_DEAD@V?$CIntelligentPointer@VCThingAICreature@@@@@@YI_NABV?$CIntelligentPointer@VCThingAICreature@@@@@Z
// Retail: Fable.exe (TLC, VC7.1) function @ 0x007304C0, oracle length 36 bytes.
// Grade: landed -- byte-exact (RELOCATION_MATCH) and behaviour PASS under the Linux lane (Wine + DDK
//        13.10.4035 cl.exe); generated for a repeated body shape, see the batch README.
//
// Retail bytes:  push esi ; mov esi,ecx ; call GetPItem ; test eax,eax ; je F ; mov ecx,esi ; call GetPItem ; test byte ptr [eax+91h],1 ; jne F ; xor al,al ; pop esi ; ret ; F: mov al,1 ; pop esi ; ret
// Script-side liveness predicate over an intelligent pointer: the item is
// fetched twice through CBaseIntelligentPointer::GetPItem @ 0x00A01B50 (null means dead),
// and its dead bit (bit 0 of the byte at +0x91) decides; the nested-if form keeps both tests as
// branches with the shared true tail at the end. Fastcall: the reference in ecx. The manifest
// label is propagated; the thing class is opaque. One of 6 bodies of this shape.
// Name (round 4, NF40): the function template instance IS_THING_DEAD<CIntelligentPointer<CThingAICreature> > (FableWin.pdb
//   0x0257C7B0; Codex pass 22 NAME_FIXES row 0090e5d0: the landed caller binds this spelling here). Was the stand-in
//   IsThingDead_007304c0 over CIntelligentPointer<CThing>&; now the template (the shape of the landed 0x00936180 / 0x005CC1B0
//   family) over engine/CThingAICreature.h and engine/CIntelligentPointer.h.
#define FABLE_RETAIL_CCHARSTRING
#define FABLE_RETAIL_CCLASS
#define FABLE_RETAIL_DEF_CLASS_BASE
#define FABLE_RETAIL_CTCBASE
#define FABLE_RETAIL_CTHING
#define FABLE_RETAIL_CINTELLIGENTPOINTER
#define FABLE_RETAIL_CTHINGPHYSICAL
#define FABLE_RETAIL_CTHINGGAMEOBJECT
#define FABLE_RETAIL_CTHINGCREATUREBASE
#define FABLE_RETAIL_CTHINGAICREATURE
#include "fable_string.h"
#include "engine/CThing.h"
#include "engine/CDefPointer.h"
#include "stlport/fable_containers.h"
#include "engine/CThingAICreature.h"
#include "engine/CIntelligentPointer.h"

template <class T> bool __fastcall IS_THING_DEAD(const T& thing)
{
    if (thing.Get() != 0) {
        if (!thing.Get()->IsDead())
            return false;
    }
    return true;
}

template bool __fastcall IS_THING_DEAD<CIntelligentPointer<CThingAICreature> >(const CIntelligentPointer<CThingAICreature>& thing);
