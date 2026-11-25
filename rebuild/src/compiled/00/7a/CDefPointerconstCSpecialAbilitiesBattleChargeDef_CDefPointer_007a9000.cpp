// CDefPointer::~CDefPointer() -- destroy the def handle: its inline Release() expanded (count down, pointee self-delete at zero, empty)
// Leaf: ??1?$CDefPointer@$$CBVCSpecialAbilitiesBattleChargeDef@@@@QAE@XZ
// Retail: Fable.exe (TLC, VC7.1) @ 0x007A9000, 27 bytes, no retail caller: the unreferenced out-of-line COMDAT copy of the
// inline CDefPointer<const CSpecialAbilitiesBattleChargeDef>::~CDefPointer() (Release() forced inline, the header's FABLE_DEFPTR_INLINE) that the tc_battle_charge TU emits (retail links without /OPT:REF).
// Identity (NF426, two sources): T from FableWin.pdb's debug\tc_battle_charge.obj module stream, whose only CDefPointer / CIVCountedPointer
// copies are ?Release@ and ??1 of CDefPointer<const CSpecialAbilitiesBattleChargeDef> (the retail TU holds exactly two such copies), and Ego_r.pdb's
// tc_battle_charge.obj (CDefinitionManager::GetDef<CSpecialAbilitiesBattleChargeDef>). Member: the higher of the two retail copies (0x007A9000) is the destructor, the lower (0x007A8E50) Release: VC7.1 /O1 and
// /O2 emit an inline's out-of-line Release copy before the ~CDefPointer copy that inlines it, /Od the other way round (NF426 toy TUs;
// /Od reproduces FableWin.pdb's dtor-first module order, which all 62 single-T modules show).
// Retail check (NF426 assignscan, 56 single-T TUs): no assign body (SetDynamicCast / operator=, which expand Release) sits below the
// lower copy in any of them; 15 sit between the copies. The toys do not reproduce retail's layout, only that order.
// Emitted by explicit instantiation of the shared header's member (a select-any COMDAT, as retail's inline copy).
#define FABLE_RETAIL_DEF_CLASS_BASE
#include "engine/CDefPointer.h"

class CSpecialAbilitiesBattleChargeDef;

template CDefPointer<const CSpecialAbilitiesBattleChargeDef>::~CDefPointer();
