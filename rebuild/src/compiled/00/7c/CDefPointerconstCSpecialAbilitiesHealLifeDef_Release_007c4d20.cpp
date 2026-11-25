// CDefPointer::Release() -- drop the def handle's counted reference (the pointee deletes itself on the last one) and empty it
// Leaf: ?Release@?$CDefPointer@$$CBVCSpecialAbilitiesHealLifeDef@@@@QAEXXZ
// Retail: Fable.exe (TLC, VC7.1) @ 0x007C4D20, 27 bytes, no retail caller: the unreferenced out-of-line COMDAT copy of the
// inline CDefPointer<const CSpecialAbilitiesHealLifeDef>::Release() that the tc_heal_life TU emits (retail links without /OPT:REF).
// Identity (NF426, two sources): T from FableWin.pdb's debug\tc_heal_life.obj module stream, whose only CDefPointer / CIVCountedPointer
// copies are ?Release@ and ??1 of CDefPointer<const CSpecialAbilitiesHealLifeDef> (the retail TU holds exactly two such copies), and Ego_r.pdb's
// tc_heal_life.obj (CDefinitionManager::GetDef<CSpecialAbilitiesHealLifeDef>). Member: the lower of the two retail copies (0x007C4D20) is Release, the higher (0x007C4E20) the destructor: VC7.1 /O1 and
// /O2 emit an inline's out-of-line Release copy before the ~CDefPointer copy that inlines it, /Od the other way round (NF426 toy TUs;
// /Od reproduces FableWin.pdb's dtor-first module order, which all 62 single-T modules show).
// Retail check (NF426 assignscan, 56 single-T TUs): no assign body (SetDynamicCast / operator=, which expand Release) sits below the
// lower copy in any of them; 15 sit between the copies. The toys do not reproduce retail's layout, only that order.
// Emitted by explicit instantiation of the shared header's member (a select-any COMDAT, as retail's inline copy).
#pragma optimize("t", on)
#define FABLE_RETAIL_DEF_CLASS_BASE
#include "engine/CDefPointer.h"

class CSpecialAbilitiesHealLifeDef;

template void CDefPointer<const CSpecialAbilitiesHealLifeDef>::Release();
