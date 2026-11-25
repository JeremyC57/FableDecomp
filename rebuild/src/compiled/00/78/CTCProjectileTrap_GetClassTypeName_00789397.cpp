// CTCProjectileTrap::GetClassTypeName() const -- the class-type name "CTCProjectileTrap" as a CCharString (size-optimised TU)
// Leaf: ?GetClassTypeName@CTCProjectileTrap@@UBE?AVCCharString@@XZ
// Retail: Fable.exe (TLC, VC7.1) @ 0x00789397, 23 bytes: mov ecx,[esp+4]; push -1; push 0x0123C174 "CTCProjectileTrap";
//   call CCharString::CCharString(const char*, long) 0x0099EBF0; mov eax,[esp+4]; ret 4 -- a const virtual __thiscall returning
//   CCharString by value: `this` (ecx) is unused, the hidden result pointer is the one stack argument (re-read, not kept in esi:
//   the /Os shape; the /O2 siblings of this slot are 25 bytes).
// Identity (NF421, 2026-11-25): Ego_r.pdb and FableWin.pdb name sets both spell
//   ?GetClassTypeName@CTCProjectileTrap@@UBE?AVCCharString@@XZ; retail vftable 0x0126B90C (RTTI .?AVCTCProjectileTrap@@) slot 22
//   (0x0126B964 -> 0x00789397) is CTCBase's pure GetClassTypeName slot (engine/CTCBase.h); the literal 0x0123C174 = "CTCProjectileTrap"
//   = the class name (read from the exe).
// Form (NF421): the literal is spelt in the virtual itself under `#pragma optimize("s",on)` (kept from HEAD: a size-optimised TU,
//   consistent with retail's re-read of [esp+4] instead of keeping the pointer in esi). FableWin.pdb also spells the class's static
//   ?StatGetClassTypeName@CTCProjectileTrap@@SI?AVCCharString@@XZ (retail's out-of-line copy is 0x004D4974 (bound by 004ee23f)), but a
//   virtual that calls that in-class inline static does not give these bytes: measured with stock cl 13.10.3077 (NF421 hand-back,
//   batches/t1/evidence/os_inline_measure.txt): under optimize("s") and under /O1, /O1 /Ob2 and /Og /Os /Ob1 the size-mode inliner
//   keeps `call StatGetClassTypeName` (14 bytes), and only the direct return gives retail's 23 bytes. So the row does not name the
//   static and emits no copy of it.
// Respell (NF421): was the stand-in self ?GetTypeAsString@CTCProjectileTrap_Methods@@QAE?AVCCharString@@XZ (a non-virtual member of a
//   made-up `_Methods` class).
// Header: no engine header defines CTCProjectileTrap; a local class over its RTTI direct base engine/CTCTrapBase.h (rtti_bases.tsv:
//   .?AVCTCProjectileTrap@@ : CTCTrapBase); prelude FABLE_RETAIL_CTCTRAPBASE, FABLE_RETAIL_DEF_CLASS_BASE, "engine/ETCInterfaceType.h"
//   (the smallest prelude psearch found for the sibling 0x00789341 CTCMeleeTrap over the same CTCTrapBase, minimised line by line:
//   NF421 tools/psearch.py).
#pragma optimize("s",on)
#define FABLE_RETAIL_CCHARSTRING                     // fable_string.h: CCharString(const char*, long length = -1) 0x0099EBF0
#include "fable_string.h"
#define FABLE_RETAIL_CTCTRAPBASE
#define FABLE_RETAIL_DEF_CLASS_BASE
#include "engine/ETCInterfaceType.h"
#define FABLE_RETAIL_CTCBASE                         // the polymorphic CTCBase (slot 22 = the pure GetClassTypeName this overrides)
#include "engine/CTCTrapBase.h"

class CTCProjectileTrap : public CTCTrapBase
{
public:
    virtual CCharString GetClassTypeName() const;
};

CCharString CTCProjectileTrap::GetClassTypeName() const
{
    return CCharString("CTCProjectileTrap", -1);
}
