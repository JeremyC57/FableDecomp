// CTCTavern::CTCTavern(CThing&) -- base CEngineSubPrimitive2DClockSprite(argument) then this class's vtable
// Leaf: ??0CTCTavern@@QAE@AAVCThing@@@Z
// Ported 2026-11-16 onto the shared engine/CTCBase.h: the local base model CEngineSubPrimitive2DClockSprite (which bound ??0CEngineSubPrimitive2DClockSprite@@QAE@PAVCEngineInternalPrimitive2DClockSprite@@@Z to 0x00686800) is gone;
//   the base call is ??0CTCBase@@QAE@AAVCThing@@@Z and this constructor takes CThing& as Ego_r.pdb and FableWin.pdb spell it (??0CTCTavern@@QAE@AAVCThing@@@Z).
// Retail: Fable.exe (TLC, VC7.1) function @ 0x007BABC0, oracle length 25 bytes.
// Grade: landed -- byte-exact (RELOCATION_MATCH) and behaviour PASS under the Linux lane (Wine + DDK
//        13.10.4035 cl.exe); generated for a repeated body shape, see the batch README.
//
// Retail bytes:  push esi ; push dword ptr [esp+8] ; mov esi,ecx ; call CEngineSubPrimitive2DClockSprite::CEngineSubPrimitive2DClockSprite ; mov dword ptr [esi],offset CTCTavern::`vftable' ; mov eax,esi ; pop esi ; ret 4
// One-argument constructor of a polymorphic class that only forwards the argument to its base
// constructor @ 0x00686800; the compiler then stores this class's vtable (0x0126DD24, named
// through rtti-vtables.tsv) and returns `this`. One of 12 bodies of this shape.
#define FABLE_RETAIL_CTCBASE
#include "engine/CTCBase.h"                     // CTCBase(CThing&) 0x00686800 ??0CTCBase@@QAE@AAVCThing@@@Z
class CEngineInternalPrimitive2DClockSprite;

class CTCTavern : public CTCBase { public:
    CTCTavern(CThing& argument);
};

CTCTavern::CTCTavern(CThing& argument)
    : CTCBase(argument)
{
}