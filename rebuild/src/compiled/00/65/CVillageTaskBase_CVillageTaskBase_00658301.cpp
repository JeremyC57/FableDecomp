// CVillageTaskBase::CVillageTaskBase() -- base CBaseIntelligentPointer() then this class's vtable
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00658301, oracle length 18 bytes.
// Grade: landed -- byte-exact (RELOCATION_MATCH) and behaviour PASS under the Linux lane (Wine + DDK
//        13.10.4035 cl.exe); generated for a repeated body shape, see the batch README.
//
// Retail bytes:  push esi ; mov esi,ecx ; call CBaseIntelligentPointer::CBaseIntelligentPointer ; mov dword ptr [esi],offset CVillageTaskBase::`vftable' ; mov eax,esi ; pop esi ; ret
// Default constructor of a polymorphic class with no members of its own to initialise: the base
// constructor @ 0x0099A380 runs, then the compiler stores this class's vtable (0x0125A830
// -- named through rebuild/manifest/rtti-vtables.tsv) and returns `this`. One of 265
// bodies of this shape.
#define FABLE_RETAIL_BASE_OBJECT
#include "engine/CBaseObject.h"                       // : CBaseClass, vtable 0x0129A7D8 (~ slot 0, Validate slot 1)

class CVillageTaskBase : public CBaseObject { public:
    virtual void v00();                    // this class has its own vtable
    CVillageTaskBase();
};

CVillageTaskBase::CVillageTaskBase()
    : CBaseObject()
{
}