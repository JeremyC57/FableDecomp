// CAIStateGroup_WhisperRetreat::CAIStateGroup_WhisperRetreat() -- base CAIStateGroup_EnemyCreatureBase(), own vtable, then the PArenaCentre member (+0x1c)
// Leaf: ??0CAIStateGroup_WhisperRetreat@@QAE@XZ
// Plan phase 5 (2026-11-05), CIntelligentPointer wave: the local CBaseIntelligentPointer / CIntelligentPointer stand-ins replaced by the shared engine/CIntelligentPointer.h, retail (byte-packed) branch FABLE_RETAIL_CINTELLIGENTPOINTER (Ego_r.pdb: vptr +0x0, PData +0x4; ctor 0x00A01B10, dtor 0x00A01C10, GetPItem 0x00A01B50, SetPItem 0x00A01B90).
// Retail: Fable.exe (TLC, VC7.1) @ 0x00901867, 36 bytes, thiscall, `ret`.
// Identity: stores own vtable 0x0128CDCC (RTTI .?AVCAIStateGroup_WhisperRetreat@@, base CAIStateGroup_EnemyCreatureBase : CAIStateGroupBase : CBaseClass); member vtable 0x01238C6C = RTTI .?AV?$CIntelligentPointer@VCThing@@@@; Ego_r.pdb / FableWin.pdb
//   ??0CAIStateGroup_WhisperRetreat@@QAE@XZ.
// Callees: CAIStateGroup_EnemyCreatureBase::CAIStateGroup_EnemyCreatureBase 0x008FF720 (??0CAIStateGroup_EnemyCreatureBase@@QAE@XZ), CBaseIntelligentPointer::CBaseIntelligentPointer 0x00A01B10 (??0CBaseIntelligentPointer@@QAE@XZ, Ego_r.pdb / FableWin.pdb).
// Layout: engine/CAIStateGroupBase.h's retail branch (CAIStateGroupBase : CBaseClass, 0x14); Ego_r.pdb CAIStateGroup_WhisperRetreat::PArenaCentre +0x1c; the members before it
//   are left unmodelled. CBaseIntelligentPointer / CIntelligentPointer<T>: the chunk 703 model (0x00900B41).
// Shape: user default ctor, empty; CIntelligentPointer's ctor inlines to the base call + vtable store after this class's own vtable store.
// History: generated in a chunk batch for a repeated body shape over the CBase_008ff720 / CMemberBase_00a01b10 stand-ins; re-authored on the
//   PDB classes in naming wave 32 (family aisg).
#define FABLE_RETAIL_CAISTATEGROUPBASE
#include "engine/CAIStateGroupBase.h"                  // CAIStateGroupBase : CBaseClass (0x14), ctor 0x008FD255

class CBaseObjectPointer;
#pragma pack(push, 1)
#define FABLE_RETAIL_CINTELLIGENTPOINTER
#include "engine/CIntelligentPointer.h"
#pragma pack(pop)
class CThing;

class CAIStateGroup_EnemyCreatureBase : public CAIStateGroupBase {                    // RTTI CAIStateGroup_EnemyCreatureBase : CAIStateGroupBase
public:
    CAIStateGroup_EnemyCreatureBase();                         // ??0CAIStateGroup_EnemyCreatureBase@@QAE@XZ (Ego_r.pdb / FableWin.pdb)
    unsigned char _unmodeled14[0x1c - 0x14];       // Ego_r.pdb: PTargetThing +0x14
};

class CAIStateGroup_WhisperRetreat : public CAIStateGroup_EnemyCreatureBase {      // vtable 0x0128CDCC
public:
    CIntelligentPointer<CThing> PArenaCentre;                     // +0x1c
    CAIStateGroup_WhisperRetreat();
};

CAIStateGroup_WhisperRetreat::CAIStateGroup_WhisperRetreat()
{
}
