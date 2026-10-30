// C3DOrientationInterpolator::C3DOrientationInterpolator() -- base CBase() then this class's vtable
// Retail: Fable.exe (TLC, VC7.1) function @ 0x004C2110, oracle length 18 bytes.
// Grade: landed -- byte-exact (RELOCATION_MATCH) and behaviour PASS under the Linux lane (Wine + DDK
//        13.10.4035 cl.exe); generated for a repeated body shape, see the batch README.
//
// Retail bytes:  push esi ; mov esi,ecx ; call CBaseClass::CBaseClass ; mov dword ptr [esi],offset C3DOrientationInterpolator::`vftable' ; mov eax,esi ; pop esi ; ret
// Default constructor of a polymorphic class with no members of its own to initialise: the base
// constructor @ 0x0099A2D0 runs, then the compiler stores this class's vtable (0x01239824
// -- named through rebuild/manifest/rtti-vtables.tsv) and returns `this`. One of 57
// bodies of this shape.
#include "engine/CBaseClass.h"                         // CBaseClass() 0x0099A2D0, ~CBaseClass 0x0099A2E0 (slot 0), implicit copy

class C3DOrientationInterpolator : public CBaseClass { public:
    C3DOrientationInterpolator();
};

C3DOrientationInterpolator::C3DOrientationInterpolator()
    : CBaseClass()
{
}