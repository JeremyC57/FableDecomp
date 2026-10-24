// NCreatureMode::GetStrafingAnimationName(const CThing&, EMovementType) -- strafe animation name for the thing's current movement and facing ("STRAFE_IDLE" in player mode 0x19)
// Leaf: ?GetStrafingAnimationName@NCreatureMode@@YI?AVCCharString@@ABVCThing@@
// Grade: DIFFER(135v135) -- scheduling only: retail loads the physics vtable (`mov eax,[ecx]`) between the first `fld` and
//   `fsub` of the movement delta; the lane loads it right before the virtual call. Tried: named-result operator- copied
//   into a local (145, extra frame temporary), copy + `-=` (159), `return C3DVector(...)` operator- (135, Z first),
//   explicit X/Y/Z stores (135, this file: only the vtable load placement differs). 4 grades used.
// Retail 0x008E8FF0, 0x87 bytes, __fastcall (hidden CCharString result in ecx, thing in edx, movement type on the stack,
//   `ret 4`). Manifest name matches the FableWin PDB name set
//   (`?GetStrafingAnimationName@NCreatureMode@@YI?AVCCharString@@ABVCThing@@W4EMovementType@@@Z`, the overload beside
//   the (C3DVector, C3DVector, EMovementType) one at 0x008E8E50).
// Callees: CThing::GetPlayer @ 0x004C7A10, CPlayer::GetCurrentMode @ 0x00633BE0, CCharString(const char*, long) @
//   0x0099EBF0 ("STRAFE_IDLE" @ 0x01287C64), the physics TC's virtual GetFacingVector() const (slot 70, +0x118, hidden
//   C3DVector result; CTCPhysicsBase's own body returns (0, 1, 0)), GetStrafingAnimationName(const C3DVector&,
//   const C3DVector&, EMovementType) @ 0x008E8E50.
// Layout: CThing +0x10 thing type byte (2), +0x60 the physics component; CTCPhysicsBase Position +0x0C, OldPosition +0x18
//   (Ego_r.pdb).
// Next idea: fetch the facing through an inline helper that takes the physics pointer first, or declare `facing` before
//   the delta and assign it after.
class CCharString {
public:
    void* PStringData;                          // +0x00
    CCharString(const char* text, long length = -1);    // 0x0099EBF0
    CCharString(const CCharString& other);      // 0x0099EC30
    ~CCharString();                             // 0x0099EAE0
};

class C3DVector {
public:
    float X, Y, Z;
    C3DVector() {}
    C3DVector(const C3DVector& v) : X(v.X), Y(v.Y), Z(v.Z) {}

};

enum EMovementType { MOVEMENT_TYPE_0 };

class CTCPhysicsBase {
public:
    virtual void v00();
    virtual void v01();
    virtual void v02();
    virtual void v03();
    virtual void v04();
    virtual void v05();
    virtual void v06();
    virtual void v07();
    virtual void v08();
    virtual void v09();
    virtual void v10();
    virtual void v11();
    virtual void v12();
    virtual void v13();
    virtual void v14();
    virtual void v15();
    virtual void v16();
    virtual void v17();
    virtual void v18();
    virtual void v19();
    virtual void v20();
    virtual void v21();
    virtual void v22();
    virtual void v23();
    virtual void v24();
    virtual void v25();
    virtual void v26();
    virtual void v27();
    virtual void v28();
    virtual void v29();
    virtual void v30();
    virtual void v31();
    virtual void v32();
    virtual void v33();
    virtual void v34();
    virtual void v35();
    virtual void v36();
    virtual void v37();
    virtual void v38();
    virtual void v39();
    virtual void v40();
    virtual void v41();
    virtual void v42();
    virtual void v43();
    virtual void v44();
    virtual void v45();
    virtual void v46();
    virtual void v47();
    virtual void v48();
    virtual void v49();
    virtual void v50();
    virtual void v51();
    virtual void v52();
    virtual void v53();
    virtual void v54();
    virtual void v55();
    virtual void v56();
    virtual void v57();
    virtual void v58();
    virtual void v59();
    virtual void v60();
    virtual void v61();
    virtual void v62();
    virtual void v63();
    virtual void v64();
    virtual void v65();
    virtual void v66();
    virtual void v67();
    virtual void v68();
    virtual void v69();
    virtual C3DVector GetFacingVector() const;  // slot 70 (+0x118)
    unsigned char _pad04[0x8];
    C3DVector Position;                         // +0x0c
    C3DVector OldPosition;                      // +0x18
};

class CPlayer {
public:
    long GetCurrentMode() const;                // 0x00633BE0 (EPlayerMode)
};

class CThing {
public:
    unsigned char   _pad00[0x10];
    unsigned char   ThingType;                  // +0x10 (2 = a creature that can be player-controlled)
    unsigned char   _pad11[0x60 - 0x11];
    CTCPhysicsBase* PhysicsTC;                  // +0x60
    CPlayer* GetPlayer() const;                 // 0x004C7A10
};

namespace NCreatureMode {
CCharString __fastcall GetStrafingAnimationName(const C3DVector& movement, const C3DVector& facing,
                                                EMovementType movementType);   // 0x008E8E50

CCharString __fastcall GetStrafingAnimationName(const CThing& thing, EMovementType movementType)
{
    if (thing.ThingType == 2 && thing.GetPlayer()->GetCurrentMode() == 0x19)
        return CCharString("STRAFE_IDLE");
    const CTCPhysicsBase* physics = thing.PhysicsTC;
    C3DVector movement;
    movement.X = physics->Position.X - physics->OldPosition.X;
    movement.Y = physics->Position.Y - physics->OldPosition.Y;
    movement.Z = physics->Position.Z - physics->OldPosition.Z;
    C3DVector facing = physics->GetFacingVector();
    return GetStrafingAnimationName(movement, facing, movementType);
}
}
