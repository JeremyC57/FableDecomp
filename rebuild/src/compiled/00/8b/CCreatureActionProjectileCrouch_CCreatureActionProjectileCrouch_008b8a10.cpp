// CCreatureAction_ProjectileCrouch::CCreatureAction_ProjectileCrouch(const CCreatureAction_ProjectileCrouch& other) -- copy constructor: the CCombatActionBase copy, this class's vtable, then TurnToFaceTarget, FacePos memberwise
// Leaf: ??0CCreatureAction_ProjectileCrouch@@QAE@ABV0@@Z
// Retail: Fable.exe (TLC, VC7.1) @ 0x008B8A10, 75 bytes, thiscall, `ret 4`, returns this.
// Identity (proven): it stores vtable 0x0127AC94 whose RTTI is .?AVCCreatureAction_ProjectileCrouch@@; FableWin.pdb ??0CCreatureAction_ProjectileCrouch@@QAE@ABV0@@Z (donor 0x029FE890);
//   only caller the landed CCreatureAction_ProjectileCrouch::Clone 0x008B89E0 (which names this callee); members are Ego_r.pdb's at retail offsets
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
class C3DVector {                                        // 1-aligned in retail; FableWin.pdb ??0C3DVector@@QAE@ABV0@@Z
public:
    float X, Y, Z;
    C3DVector(const C3DVector& v) : X(v.X), Y(v.Y), Z(v.Z) { }
};
#pragma pack(pop)
class CCreatureAction_ProjectileCrouch : public CCombatActionBase {
public:
    CCreatureAction_ProjectileCrouch(const CCreatureAction_ProjectileCrouch& other);
    bool TurnToFaceTarget;                    // +0x114 (PDB +300)
    C3DVector FacePos;                        // +0x115 (PDB +301: the 1-aligned C3DVector)
};
CCreatureAction_ProjectileCrouch::CCreatureAction_ProjectileCrouch(const CCreatureAction_ProjectileCrouch& other)
    : CCombatActionBase(other)
    , TurnToFaceTarget(other.TurnToFaceTarget)
    , FacePos(other.FacePos)
{
}