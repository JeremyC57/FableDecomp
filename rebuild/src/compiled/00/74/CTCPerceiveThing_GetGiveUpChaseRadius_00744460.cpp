// CTCPerceiveThing::GetGiveUpChaseRadius() const -- return the radius beyond which a chase is given up (GiveUpChaseRadius +0x4C)
// Leaf: ?GetGiveUpChaseRadius@CTCPerceiveThing@@QBEMXZ
// Flags: none
// Retail: Fable.exe (TLC, VC7.1) @ 0x00744460, 4 bytes: `fld dword ptr [ecx+4Ch] ; ret`.
// Name-fix 2026-11-12 (was the stand-in C00744460::M, catalog _global::GetMember_float0x4c_00744460): Ego_r.pdb and FableWin.pdb
//   ?GetGiveUpChaseRadius@CTCPerceiveThing@@QBEMXZ; Ego_r.pdb CTCPerceiveThing.GiveUpChaseRadius is at +76 (0x4C) in the 0x58-byte
//   class (engine/CTCPerceiveThing.h asserts the size); the landed caller CAIStateGroupTrollBase::Initialise 0x0094D780 calls it on
//   the TCI_PERCEIVE_THING component and binds this address under this symbol.
#define FABLE_RETAIL_CTCPERCEIVETHING_SMELL
#define FABLE_RETAIL_CTCBASE
#define FABLE_RETAIL_CTHING
#define FABLE_RETAIL_CINTELLIGENTPOINTER
#define FABLE_RETAIL_DEF_CLASS_BASE
#include "engine/ETCInterfaceType.h"
#include "engine/CTCPerceiveThing.h"

float CTCPerceiveThing::GetGiveUpChaseRadius() const
{
    return GiveUpChaseRadius;
}
