#ifndef FABLE_ENGINE_CSCORPIONKINGBATTLEDEF_H
#define FABLE_ENGINE_CSCORPIONKINGBATTLEDEF_H
// New header (chunk 2034, row 0x0097BDA0 CAIStateGroup_ScorpionKing::UpdateCombat). RTTI .?AVCScorpionKingBattleDef@@ :
// CParentDefClassBase (ghidra_out/rtti_bases.tsv; vtable 0x01242EA4). Members: Ego_r.pdb TPI names and order (struct_layouts_egor.tsv,
// first member +0x40 there), at the retail offsets: CParentDefClassBase is 0x39 bytes (packed; its vector is 12 bytes in retail), so
// NumChargesBeforeTailStrike sits at +0x3C (UpdateCombat compares ChargeCount with +0x3C, 0x0097C22B; reads DamageToCauseTailStrike
// +0x54, 0x0097C164, and ClawObstructionRadius +0x5C, 0x0097C472 / 0x0097C4E0). The def is reached through the out-of-line
// GetSubDef<CScorpionKingBattleDef> 0x008C5A50 (Ego_r ??$GetSubDef@VCScorpionKingBattleDef@@@CDefClassBase@@QBE_NPAV?$CDefPointer@$$CBVCScorpionKingBattleDef@@@@@Z).
// The inline ?StatGetTypeName@CScorpionKingBattleDef@@SI?AVCCharString@@XZ (Ego_r.pdb / FableWin.pdb) builds the 'CScorpionKingBattleDef'
// literal the landed GetSubDef<CScorpionKingBattleDef> 0x008C5A50 passes to GetPSubDefInfo; with it a row can use the shared GetSubDef
// body (FABLE_RETAIL_CDEFCLASSBASE_GETSUBDEF_BODY). Needs FABLE_RETAIL_DEF_CLASS_BASE (engine/CParentDefClassBase.h) and fable_string.h.
#include "fable_string.h"
#include "engine/CParentDefClassBase.h"
class CScorpionKingBattleDef : public CParentDefClassBase {
public:
    static CCharString FABLE_FASTCALL StatGetTypeName() { return CCharString("CScorpionKingBattleDef"); }
    long  NumChargesBeforeTailStrike;                                    // +0x3c
    float UnderGroundTime;                                               // +0x40
    float TailPlungeTime;                                                // +0x44
    float SpikeTime;                                                     // +0x48
    float SpikeDecisionTime;                                             // +0x4c
    float TailStrikePrepareTime;                                         // +0x50
    float DamageToCauseTailStrike;                                       // +0x54
    float ChargeDamage;                                                  // +0x58
    float ClawObstructionRadius;                                         // +0x5c
};
#endif // FABLE_ENGINE_CSCORPIONKINGBATTLEDEF_H
