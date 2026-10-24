// CCreatureAction_DragonSwoop::CCreatureAction_DragonSwoop(const CCreatureAction_DragonSwoop&) -- implicit copy constructor: base copy, own vtable, the three swoop points
// Leaf: ??0CCreatureAction_DragonSwoop@@QAE@ABV0@@Z
// Retail 0x008F57F0, 0x51 bytes, thiscall `ret 4`. Manifest label "CCombatActionBase_CopyConstructor" (_global) is
//   propagated: the vtable stored is 0x012887D4 (RTTI .?AVCCreatureAction_DragonSwoop@@) and the only caller is the
//   landed CCreatureAction_DragonSwoop::Clone 0x008F57C0 (`new` of 0x138 bytes). PDB: `??0CCreatureAction_DragonSwoop@@QAE@ABV0@@Z`.
// Callee: CCombatActionBase::CCombatActionBase(const CCombatActionBase&) @ 0x0062F3B0.
// Layout: retail CCombatActionBase is 0x114 bytes, SwoopPoints C3DVector[3] at +0x114 (Ego_r.pdb +0x12C and sizeof 0x150:
//   the engine header carries PDB offsets, so this source keeps a local model).
// Shape: the compiler-GENERATED copy constructor, emitted out of line because C3DVector declares a memberwise copy ctor
//   (the class's implicit copy is then non-trivial) and Clone's `new T(*this)` references it: base copy call, vtable
//   store, then the array copied by a 3-iteration loop of element copy ctors (VC's strength-reduced `[eax-8]`/`[edx+eax]`
//   addressing).
class C3DVector {
public:
    float X, Y, Z;
    C3DVector() {}
    C3DVector(const C3DVector& v) : X(v.X), Y(v.Y), Z(v.Z) {}   // ??0C3DVector@@QAE@ABV0@@Z (Ego_r.pdb)
};

class CCreatureActionBase;

class CCombatActionBase {                       // retail sizeof 0x114
public:
    virtual void v00();
    unsigned char _body[0x114 - 4];
    CCombatActionBase(const CCombatActionBase& other);   // 0x0062F3B0
};

class CCreatureAction_DragonSwoop : public CCombatActionBase {   // vtable 0x012887D4, sizeof 0x138
public:
    C3DVector SwoopPoints[3];                   // +0x114 (PDB +0x12c: retail CCombatActionBase is 0x18 smaller)
    virtual void v00();
    CCreatureActionBase* Clone() const;
};

CCreatureActionBase* CCreatureAction_DragonSwoop::Clone() const
{
    return (CCreatureActionBase*)new CCreatureAction_DragonSwoop(*this);
}