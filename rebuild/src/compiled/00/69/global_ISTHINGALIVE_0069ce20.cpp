// IS_THING_ALIVE<CIntelligentPointer<const CThing> >(const CIntelligentPointer<const CThing>& thing) -- true when the pointer holds a thing that is not dead (GetPItem @ 0x00A01B50 twice: null, then the dead bit +0x91 & 1)
// Leaf: ??$IS_THING_ALIVE@V?$CIntelligentPointer@$$CBVCThing@@@@@@YI_NABV?$CIntelligentPointer@$$CBVCThing@@@@@Z
// Flags: none
// Retail: Fable.exe (TLC, VC7.1) function @ 0x0069CE20, 36 bytes, `__fastcall` (the reference in ecx).
// Retail bytes:  push esi ; mov esi,ecx ; call GetPItem ; test eax,eax ; je F ; mov ecx,esi ; call GetPItem ; test byte ptr [eax+91h],1 ; jne F ; mov al,1 ; pop esi ; ret ; F: xor al,al ; pop esi ; ret
// Name-fix 2026-11-12 (was the stand-in free function IsThingAlive_0069ce20 over CIntelligentPointer<CThing>&): FableWin.pdb
//   ??$IS_THING_ALIVE@V?$CIntelligentPointer@$$CBVCThing@@@@@@YI_NABV?$CIntelligentPointer@$$CBVCThing@@@@@Z (_fablewin_api.tsv
//   0x0184443C / 0x01F2F960); the landed callers CThingSoundEmitter::RenderUpdate 0x00836AD0 and
//   CAIStateGroup_CreatureRangedCombatFireAtSpecificTargets::Update 0x0095E0B0 (PTarget, CIntelligentPointer<const CThing>) bind this
//   address under that symbol. The body is the shared engine/ThingAlive.h template, explicitly instantiated.
#define FABLE_RETAIL_CINTELLIGENTPOINTER
#define FABLE_RETAIL_CINTELLIGENTPOINTER_COMPARE_NULL_INT
#define FABLE_RETAIL_IS_THING_ALIVE
#define FABLE_RETAIL_CTHING
#include "engine/CThing.h"
#include "engine/CIntelligentPointer.h"
#include "engine/ThingAlive.h"

template bool FABLE_FASTCALL IS_THING_ALIVE<CIntelligentPointer<const CThing> >(const CIntelligentPointer<const CThing>&);
