// CCreatureAction_ScorpionChargeStrike::CCreatureAction_ScorpionChargeStrike(const CCreatureAction_ScorpionChargeStrike& other) -- copy constructor: the CCombatActionBase copy, this class's vtable, then ChargeDistance, StartPos memberwise
// Leaf: ??0CCreatureAction_ScorpionChargeStrike@@QAE@ABV0@@Z
// Plan phase 3 (2026-11-04), C3DVector wave: the local C3DVector stand-in replaced by the shared engine/C3DVector.h (PDB C3DVector TPI 0x2783: public X/Y/Z at 0/4/8, size 12).
// Retail: Fable.exe (TLC, VC7.1) @ 0x008C5E40, 75 bytes, thiscall, `ret 4`, returns this.
// Identity (proven): it stores vtable 0x01281184 whose RTTI is .?AVCCreatureAction_ScorpionChargeStrike@@; FableWin.pdb ??0CCreatureAction_ScorpionChargeStrike@@QAE@ABV0@@Z (donor 0x02A23E90);
//   only caller the landed CCreatureAction_ScorpionChargeStrike::Clone 0x008C5E10 (which names this callee); members are Ego_r.pdb's at retail offsets
//   (PDB - 0x18: the CCombatActionBase shrink).
// Callees: CCombatActionBase::CCombatActionBase(const CCombatActionBase&) 0x0062F3B0.
// Shape (family K038, representative 0x008B8A10): the compiler's memberwise copy (written out as the init list it
//   generates): base copy constructor out of line, the vtable store, each member in declaration order (a C3DVector as
//   its three words through C3DVector's inline user copy constructor -- a trivial C3DVector copy re-bases both
//   pointers instead of using direct offsets).
class CCreatureActionBase {
public:
    virtual ~CCreatureActionBase();
};
class CCombatActionBase : public CCreatureActionBase {   // retail sizeof 0x114 (Ego_r.pdb 0x12C)
public:
    CCombatActionBase(const CCombatActionBase& other);   // 0x0062F3B0
    unsigned char _body[0x114 - 4];
};
#pragma pack(push, 1)
#include "engine/C3DVector.h"
#pragma pack(pop)
class CCreatureAction_ScorpionChargeStrike : public CCombatActionBase {
public:
    CCreatureAction_ScorpionChargeStrike(const CCreatureAction_ScorpionChargeStrike& other);
    float ChargeDistance;                     // +0x114 (PDB +300)
    C3DVector StartPos;                       // +0x118
};
CCreatureAction_ScorpionChargeStrike::CCreatureAction_ScorpionChargeStrike(const CCreatureAction_ScorpionChargeStrike& other)
    : CCombatActionBase(other)
    , ChargeDistance(other.ChargeDistance)
    , StartPos(other.StartPos)
{
}