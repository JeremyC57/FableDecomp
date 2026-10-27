// CCreatureAction_JackFloatUp::CCreatureAction_JackFloatUp(const CCreatureAction_JackFloatUp& other) -- copy constructor: the CCombatActionBase copy, this class's vtable, then FloatHeight, FloatSpeed, Moving, StartHeight memberwise
// Leaf: ??0CCreatureAction_JackFloatUp@@QAE@ABV0@@Z
// Retail: Fable.exe (TLC, VC7.1) @ 0x00977690, 75 bytes, thiscall, `ret 4`, returns this.
// Identity (proven): it stores vtable 0x01297F54 whose RTTI is .?AVCCreatureAction_JackFloatUp@@; FableWin.pdb ??0CCreatureAction_JackFloatUp@@QAE@ABV0@@Z (donor 0x02BF1EF0);
//   only caller the landed CCreatureAction_JackFloatUp::Clone 0x00977660 (which names this callee); members are Ego_r.pdb's at retail offsets
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
class CCreatureAction_JackFloatUp : public CCombatActionBase {
public:
    CCreatureAction_JackFloatUp(const CCreatureAction_JackFloatUp& other);
    float FloatHeight;                        // +0x114 (PDB +300)
    float FloatSpeed;                         // +0x118
    bool Moving;                              // +0x11C
    float StartHeight;                        // +0x120
};
CCreatureAction_JackFloatUp::CCreatureAction_JackFloatUp(const CCreatureAction_JackFloatUp& other)
    : CCombatActionBase(other)
    , FloatHeight(other.FloatHeight)
    , FloatSpeed(other.FloatSpeed)
    , Moving(other.Moving)
    , StartHeight(other.StartHeight)
{
}