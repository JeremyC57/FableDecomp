// Grade: RELOCATION_MATCH (Fable landing 2026-10-19 after the Codex review hold: test uses a copy_model stand-in)
// CCreatureAction_DragonTakeOffIntoFlying::CCreatureAction_DragonTakeOffIntoFlying(const CCreatureAction_DragonTakeOffIntoFlying&)
//   -- member-wise copy: combat-action base, the three points, the movement-point vector, the flag and the counted end action
// Retail: Fable.exe (TLC, VC7.1) @ 0x00953BC0, 0xC1 bytes. Callees: CCombatActionBase::CCombatActionBase(const CCombatActionBase&)
//   0x0062F3B0 (manifest), the STLport vector<C3DVector> copy ctor 0x006F0810 (label "Vector_CopyFrom"); vtable
//   0x01293A7C = CCreatureAction_DragonTakeOffIntoFlying.
// Layout: PDB (Ego_r) names/types; the TLC CCombatActionBase is 0x114 bytes (PDB 0x12c), so retail offset = PDB - 0x18:
//   Dest +0x114, StartPos +0x120, PreviousPoint +0x12c, MovementPoints +0x138 (12-byte STLport vector), UsePreviousPoint
//   +0x144, PEndAction +0x148 (CCountedPointer: Data, Info; the shared CCPPointerInfo keeps RefCount at +0).
//   Local model: no engine/ header exists for these classes.
struct CCreatureActionBase;
struct CCombatActionBase {
    virtual ~CCombatActionBase();
    unsigned char _body[0x110];
    CCombatActionBase(const CCombatActionBase& other);  // 0x0062F3B0
};
struct C3DVector {                                      // the engine vector has a user-written member-wise copy
    float X, Y, Z;
    C3DVector(const C3DVector& v) : X(v.X), Y(v.Y), Z(v.Z) {}
};
template<class T> struct CRetailVector {                // STLport vector: begin/end/capacity
    T* m_first; T* m_last; T* m_end;
    CRetailVector(const CRetailVector& other);          // C3DVector: 0x006F0810
};
struct CCPPointerInfo { unsigned long RefCount; void (__cdecl* DeleteFunc)(void*); void* Data; };
template<class T> struct CCountedPointer {
    T* Data;
    CCPPointerInfo* Info;
    CCountedPointer(const CCountedPointer& o) : Data(o.Data), Info(o.Info) { if (Info) ++Info->RefCount; }
};
struct CCreatureAction_DragonTakeOffIntoFlying : CCombatActionBase {
    C3DVector Dest;                                     // +0x114
    C3DVector StartPos;                                 // +0x120
    C3DVector PreviousPoint;                            // +0x12c
    CRetailVector<C3DVector> MovementPoints;            // +0x138
    bool UsePreviousPoint;                              // +0x144
    CCountedPointer<CCreatureActionBase> PEndAction;    // +0x148
    virtual ~CCreatureAction_DragonTakeOffIntoFlying();
    CCreatureAction_DragonTakeOffIntoFlying(const CCreatureAction_DragonTakeOffIntoFlying& other);
};
CCreatureAction_DragonTakeOffIntoFlying::CCreatureAction_DragonTakeOffIntoFlying(const CCreatureAction_DragonTakeOffIntoFlying& o)
    : CCombatActionBase(o), Dest(o.Dest), StartPos(o.StartPos), PreviousPoint(o.PreviousPoint),
      MovementPoints(o.MovementPoints), UsePreviousPoint(o.UsePreviousPoint), PEndAction(o.PEndAction)
{
}