// CScorpionKingBattleDef::CScorpionKingBattleDef() -- base CParentDefClassBase() then this class's vtable
// Retail: Fable.exe (TLC, VC7.1) function @ 0x004E3ACA, oracle length 18 bytes.
// Grade: landed -- byte-exact (RELOCATION_MATCH) and behaviour PASS under the Linux lane (Wine + DDK
//        13.10.4035 cl.exe); generated for a repeated body shape, see the batch README.
//
// Retail bytes:  push esi ; mov esi,ecx ; call CParentDefClassBase::CParentDefClassBase ; mov dword ptr [esi],offset CScorpionKingBattleDef::`vftable' ; mov eax,esi ; pop esi ; ret
// Default constructor of a polymorphic class with no members of its own to initialise: the base
// constructor @ 0x00430370 runs, then the compiler stores this class's vtable (0x01242EA4
// -- named through rebuild/manifest/rtti-vtables.tsv) and returns `this`. One of 265
// bodies of this shape.
// Layout: CParentDefClassBase is the retail def chain from the shared headers (FABLE_RETAIL_DEF_CLASS_BASE opt-in:
//   vtable 0x01230CBC, 0x39 bytes packed; ctor 0x00430370, ~CParentDefClassBase 0x00430300, copy 0x0046EE98,
//   operator= 0x00431ED0).
#define FABLE_RETAIL_DEF_CLASS_BASE
#include "engine/CParentDefClassBase.h"

class CScorpionKingBattleDef : public CParentDefClassBase { public:
    virtual void v00();                    // this class has its own vtable
    CScorpionKingBattleDef();
};

CScorpionKingBattleDef::CScorpionKingBattleDef()
    : CParentDefClassBase()
{
}