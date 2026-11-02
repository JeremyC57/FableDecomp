// CAIStateGroup_CreatureRangedCombatFireAtSpecificTargets::CAIStateGroup_CreatureRangedCombatFireAtSpecificTargets() -- base CAIStateGroupBase(), own vtable, then the PTarget member (+0x14)
// Leaf: ??0CAIStateGroup_CreatureRangedCombatFireAtSpecificTargets@@QAE@XZ
// Retail: Fable.exe (TLC, VC7.1) @ 0x00900F5E, 36 bytes, thiscall, `ret`.
// Identity: stores own vtable 0x0128C894 (RTTI .?AVCAIStateGroup_CreatureRangedCombatFireAtSpecificTargets@@, base CAIStateGroupBase : CBaseClass); member vtable 0x01237F1C = RTTI .?AV?$CIntelligentPointer@$$CBVCThing@@@@; Ego_r.pdb / FableWin.pdb
//   ??0CAIStateGroup_CreatureRangedCombatFireAtSpecificTargets@@QAE@XZ.
// Callees: CAIStateGroupBase::CAIStateGroupBase 0x008FD255, CBaseIntelligentPointer::CBaseIntelligentPointer 0x00A01B10 (??0CBaseIntelligentPointer@@QAE@XZ, Ego_r.pdb / FableWin.pdb).
// Layout: engine/CAIStateGroupBase.h's retail branch (CAIStateGroupBase : CBaseClass, 0x14); Ego_r.pdb CAIStateGroup_CreatureRangedCombatFireAtSpecificTargets::PTarget +0x14; the members before it
//   are left unmodelled. CBaseIntelligentPointer / CIntelligentPointer<T>: the chunk 703 model (0x00900B41).
// Shape: user default ctor, empty; CIntelligentPointer's ctor inlines to the base call + vtable store after this class's own vtable store.
// History: generated in a chunk batch for a repeated body shape over the CBase_008fd255 / CMemberBase_00a01b10 stand-ins; re-authored on the
//   PDB classes in naming wave 32 (family aisg).
#define FABLE_RETAIL_CAISTATEGROUPBASE
#include "engine/CAIStateGroupBase.h"                  // CAIStateGroupBase : CBaseClass (0x14), ctor 0x008FD255

class CBaseObjectPointer;
#pragma pack(push, 1)
class CBaseIntelligentPointer {                         // Ego_r.pdb: vptr + PData (8 bytes), 1-aligned (chunk 703 0x00900B41)
public:
    CBaseIntelligentPointer();                          // 0x00A01B10
    virtual ~CBaseIntelligentPointer();                 // 0x00A01C10
    CBaseObjectPointer* PData;                          // +0x04
};
template <class T> class CIntelligentPointer : public CBaseIntelligentPointer {
public:
    CIntelligentPointer() : CBaseIntelligentPointer() { }
    virtual ~CIntelligentPointer() { }
};
#pragma pack(pop)
class CThing;

class CAIStateGroup_CreatureRangedCombatFireAtSpecificTargets : public CAIStateGroupBase {      // vtable 0x0128C894
public:
    CIntelligentPointer<const CThing> PTarget;                     // +0x14
    CAIStateGroup_CreatureRangedCombatFireAtSpecificTargets();
};

CAIStateGroup_CreatureRangedCombatFireAtSpecificTargets::CAIStateGroup_CreatureRangedCombatFireAtSpecificTargets()
{
}
