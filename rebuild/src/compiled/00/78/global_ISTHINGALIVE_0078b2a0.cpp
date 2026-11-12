// IS_THING_ALIVE<CIntelligentPointer<CThingObject> >(pointer) -- GetPItem @ 0x00A01B50 twice: null, then the dead bit (+0x91 & 1)
// Leaf: ??$IS_THING_ALIVE@V?$CIntelligentPointer@VCThingObject@@@@@@YI_NABV?$CIntelligentPointer@VCThingObject@@@@@Z
// Retail: Fable.exe (TLC, VC7.1) function @ 0x0078B2A0, oracle length 36 bytes.
// Grade: landed -- byte-exact (RELOCATION_MATCH) and behaviour PASS under the Linux lane.
//
// Retail bytes:  push esi ; mov esi,ecx ; call GetPItem ; test eax,eax ; je F ; mov ecx,esi ; call GetPItem ; test byte ptr [eax+91h],1 ; jne F ; mov al,1 ; pop esi ; ret ; F: xor al,al ; pop esi ; ret
// Name: FableWin.pdb has this exact instantiation (FableWin 0x01DE6D80); the callers' pointers
// (Bar, food, PTable, PAmmo) are CIntelligentPointer<CThingObject>. The original template
// IS_THING_ALIVE<T>(const T&) lives in engine/ThingAlive.h; this row is its explicit instantiation.
#define FABLE_RETAIL_CCHARSTRING
#define FABLE_RETAIL_DEF_CLASS_BASE
#define FABLE_RETAIL_CTCBASE
#define FABLE_RETAIL_CTHING
#define FABLE_RETAIL_CTHINGPHYSICAL
#define FABLE_RETAIL_CTHINGGAMEOBJECT
#define FABLE_RETAIL_CINTELLIGENTPOINTER
#define FABLE_RETAIL_CINTELLIGENTPOINTER_COMPARE_NULL_INT
#define FABLE_RETAIL_IS_THING_ALIVE
#include "fable_string.h"
#include "engine/CThingObject.h"
#include "engine/CIntelligentPointer.h"
#include "engine/ThingAlive.h"

template bool FABLE_FASTCALL IS_THING_ALIVE<CIntelligentPointer<CThingObject> >(const CIntelligentPointer<CThingObject>&);
