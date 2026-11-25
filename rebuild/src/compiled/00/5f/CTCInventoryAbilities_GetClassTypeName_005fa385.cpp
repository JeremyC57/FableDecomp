// CTCInventoryAbilities::GetClassTypeName() const -- the class-type name "CTCInventoryAbilities" as a CCharString (size-optimised TU)
// Leaf: ?GetClassTypeName@CTCInventoryAbilities@@UBE?AVCCharString@@XZ
// Retail: Fable.exe (TLC, VC7.1) @ 0x005FA385, 23 bytes: mov ecx,[esp+4]; push -1; push 0x0123A498 "CTCInventoryAbilities";
//   call CCharString::CCharString(const char*, long) 0x0099EBF0; mov eax,[esp+4]; ret 4 -- a const virtual __thiscall returning
//   CCharString by value: `this` (ecx) is unused, the hidden result pointer is the one stack argument (re-read, not kept in esi:
//   the /Os shape; the /O2 siblings of this slot are 25 bytes).
// Identity (NF421, 2026-11-25): Ego_r.pdb and FableWin.pdb name sets both spell
//   ?GetClassTypeName@CTCInventoryAbilities@@UBE?AVCCharString@@XZ; retail vftable 0x01256AD4 (RTTI .?AVCTCInventoryAbilities@@) slot
//   22 (0x01256B2C -> 0x005FA385) is CTCBase's pure GetClassTypeName slot (engine/CTCBase.h); the literal 0x0123A498 =
//   "CTCInventoryAbilities" = the class name (read from the exe).
// Form (NF421): the literal is spelt in the virtual itself under `#pragma optimize("s",on)` (kept from HEAD: a size-optimised TU,
//   consistent with retail's re-read of [esp+4] instead of keeping the pointer in esi). FableWin.pdb also spells the class's static
//   ?StatGetClassTypeName@CTCInventoryAbilities@@SI?AVCCharString@@XZ (retail has no out-of-line copy bound in reloc_bindings.tsv
//   (every use expands it)), but a virtual that calls that in-class inline static does not give these bytes: measured with stock cl
//   13.10.3077 (NF421 hand-back, batches/t1/evidence/os_inline_measure.txt): under optimize("s") and under /O1, /O1 /Ob2 and /Og /Os
//   /Ob1 the size-mode inliner keeps `call StatGetClassTypeName` (14 bytes), and only the direct return gives retail's 23 bytes. So the
//   row does not name the static and emits no copy of it.
// Respell (NF421): was the stand-in self ?GetTypeAsString@CTCInventoryAbilities_Methods@@QAE?AVCCharString@@XZ (a non-virtual member of
//   a made-up `_Methods` class).
// Header: engine/CTCInventoryAbilities.h under the layout-rename pattern (the 0x004D4A86 CTCBerserk form); prelude
//   FABLE_RETAIL_CTCINVENTORYABILITIES (the smallest prelude under which the polymorphic check TU compiles, from the header's landed
//   includers, minimised line by line: NF421 tools/psearch.py).
#pragma optimize("s",on)
#define FABLE_RETAIL_CCHARSTRING                     // fable_string.h: CCharString(const char*, long length = -1) 0x0099EBF0
#include "fable_string.h"
#define FABLE_RETAIL_CTCINVENTORYABILITIES
#define FABLE_RETAIL_CTCBASE                         // the polymorphic CTCBase (slot 22 = the pure GetClassTypeName this overrides)
#define CTCInventoryAbilities CTCInventoryAbilitiesLayout
#include "engine/CTCInventoryAbilities.h"
#undef CTCInventoryAbilities

class CTCInventoryAbilities : public CTCInventoryAbilitiesLayout
{
public:
    virtual CCharString GetClassTypeName() const;
};

CCharString CTCInventoryAbilities::GetClassTypeName() const
{
    return CCharString("CTCInventoryAbilities", -1);
}
