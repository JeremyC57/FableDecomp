// NCreatureMode::GetStrafingAnimationName(const CThing& thing, EMovementType movementType) -- strafe animation name for the thing's physics movement and facing ("STRAFE_IDLE" for a class-type-2 thing whose player is in mode 0x19)
// Grade: DIFFER(135v135) -- scheduling only: retail loads the physics vtable (mov eax,[ecx]) between the first fld and fsub of the movement delta; ours loads it right before the virtual call (lea edx takes that slot)
// Leaf: ?GetStrafingAnimationName@NCreatureMode@@YI?AVCCharString@@ABVCThing@@W4EMovementType@@@Z
// Retail: Fable.exe (TLC, VC7.1) function @ 0x008E8FF0, 135 bytes, __fastcall (hidden CCharString result in ecx, thing in
//   edx, movement type on the stack, ret 4). Ego_r/FableWin ?GetStrafingAnimationName@NCreatureMode@@YI?AVCCharString@@
//   ABVCThing@@W4EMovementType@@@Z (donor 0x0090F4A0) -- a NCreatureMode namespace function, not a global.
// Callees: CThing::PeekPlayer() const 0x004C7A10 (thing is const&: Ego_r has GetPlayer QAE and PeekPlayer QBE), CPlayer::
//   GetCurrentMode() const 0x00633BE0 (Ego_r ?GetCurrentMode@CPlayer@@QBE?AW4EPlayerMode@@XZ), CCharString(const char*, long)
//   0x0099EBF0 ("STRAFE_IDLE" 0x01287C64), CTCPhysicsBase::GetFacingVector() const (slot 70, +0x118, Ego_r UBE),
//   NCreatureMode::GetStrafingAnimationName(const C3DVector&, const C3DVector&, EMovementType) 0x008E8E50 (Ego_r donor
//   0x0090F2E0; reloc_bindings has the wrong ?GetStrafeAnimationName@@YI...).
// Tried (this session): in-call C3DVector(x,y,z) temporary + in-call facing (131, type pushed before the facing call),
//   facing declared before the delta (155), implicit C3DVector copy ctor (same), PDB accessors GetPhysicsTC()/PeekPlayer
//   (this file, same). Earlier: four operator- forms, all-in-call, movement local + facing in call, physics reference.
// Layout: CClass::Type +0x10 (2), CThing PhysicsTC +0x60 (PDB +0x70); CTCPhysicsBase Position +0x0C, OldPosition +0x18.
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

};

enum EMovementType { MOVEMENT_TYPE_0 };
enum EPlayerMode { PLAYER_MODE_0x19 = 0x19 };

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
    EPlayerMode GetCurrentMode() const;         // 0x00633BE0
};

class CThing {
public:
    unsigned char   _pad00[0x10];
    unsigned char   ThingType;                  // +0x10 (2 = a creature that can be player-controlled)
    unsigned char   _pad11[0x60 - 0x11];
    CTCPhysicsBase* PhysicsTC;                  // +0x60
    const CPlayer& PeekPlayer() const;          // 0x004C7A10
    const CTCPhysicsBase* GetPhysicsTC() const { return PhysicsTC; }
};

namespace NCreatureMode {
CCharString __fastcall GetStrafingAnimationName(const C3DVector& movement, const C3DVector& facing,
                                                EMovementType movementType);   // 0x008E8E50

CCharString __fastcall GetStrafingAnimationName(const CThing& thing, EMovementType movementType)
{
    if (thing.ThingType == 2 && thing.PeekPlayer().GetCurrentMode() == PLAYER_MODE_0x19)
        return CCharString("STRAFE_IDLE");
    const CTCPhysicsBase* physics = thing.GetPhysicsTC();
    C3DVector movement;
    movement.X = physics->Position.X - physics->OldPosition.X;
    movement.Y = physics->Position.Y - physics->OldPosition.Y;
    movement.Z = physics->Position.Z - physics->OldPosition.Z;
    C3DVector facing = thing.GetPhysicsTC()->GetFacingVector();
    return GetStrafingAnimationName(movement, facing, movementType);
}
}
