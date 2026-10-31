// CCreatureActionBase::UpdateActionMovement() -- when the creature uses movement in actions (or the action always uses animation movement) and the action uses animation or forced movement, mark the creature's physics as moving, refresh the next movement position (UpdateActionMoveToPos) and hand it to the physics component with IsMovementUsingPhysics()
// Leaf: ?UpdateActionMovement@CCreatureActionBase@@IAEXXZ
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00692AA0, 132 bytes (+ int3 padding), __fastcall, plain ret.
// Identity: Ego_r.pdb ?UpdateActionMovement@CCreatureActionBase@@IAEXXZ (protected; donor 0x00519420).
// Virtual slots (retail vtable 0x01259D24 read from the image, names from the Ego_r donor order 0x00518FE0..0x00519000 and
//   0x005190C0): 34 IsAlwaysUseAnimationMovement() const (UBE), 57 IsUsingAnimationMovement() const, 58 IsUsingForcedMovement()
//   const, 59 IsMovementUsingPhysics() const (all MBE, base bodies `xor al,al ; ret`), 60 UpdateActionMoveToPos() (MAE, the
//   landed 0x00692B30). CTCPhysicsBase slot 34 (+0x88) is SetForcedToMovePos(const C3DVector&, bool) (Ego_r overriders
//   CTCPhysicsControlled / CTCPhysicsNavigator 0x0073BE30 / 0x0073E180).
// Callees: CBaseIntelligentPointer::GetPItem() const 0x00A01B50 (x3), CTCPhysicsBase::SetAsMoving(bool) 0x006B0130 (Ego_r).
// Shape: the second argument is the BOOL IsMovementUsingPhysics(): with the bool parameter VC7.1 forms `add esi,68h ; mov
//   ecx,edi` before both pushes (a long parameter pushes eax straight after the slot-59 call -- the old 132v132 residue).
//   The physics component comes through the inline CThing::GetPhysicsTC() (FableWin.pdb QAE) after PCreature's inline
//   operator->: the object expression is then complete before the `push 1` of SetAsMoving (a direct ->PhysicsTC pushes 1
//   before the GetPItem call). The creature flag is the PDB bitfield UseMovementInActions (bit 0 of +0x15d; PDB +0x185).
// Layout: engine/CCreatureActionBase.h (PDB) agrees on PCreature +8 and NextMovementPos +0x68; a local model carries the
//   virtual slots. Retail CThing::PhysicsTC +0x60 (PDB +0x70).
class C3DVector { public: float X, Y, Z; };
class CTCPhysicsBase {
public:
    virtual void v00(); virtual void v01(); virtual void v02(); virtual void v03(); virtual void v04(); virtual void v05(); virtual void v06(); virtual void v07(); virtual void v08(); virtual void v09(); virtual void v10(); virtual void v11(); virtual void v12(); virtual void v13(); virtual void v14(); virtual void v15(); virtual void v16(); virtual void v17(); virtual void v18(); virtual void v19(); virtual void v20(); virtual void v21(); virtual void v22(); virtual void v23(); virtual void v24(); virtual void v25(); virtual void v26(); virtual void v27(); virtual void v28(); virtual void v29(); virtual void v30(); virtual void v31(); virtual void v32(); virtual void v33();
    virtual void SetForcedToMovePos(const C3DVector& pos, bool forced);   // slot 34 (+0x88)
    void SetAsMoving(bool moving);                     // 0x006B0130
};
class CThing {
public:
    unsigned char   _pad0[0x60];
    CTCPhysicsBase* PhysicsTC;                         // +0x60 (PDB +0x70)
    CTCPhysicsBase* GetPhysicsTC() { return PhysicsTC; }
};
class CThingCreatureBase : public CThing {
public:
    unsigned char _pad64[0x15d - 0x64];
    bool          UseMovementInActions : 1;            // +0x15d bit 0 (PDB +0x185)
};
class CBaseObject;
class CBaseIntelligentPointer {
public:
    virtual ~CBaseIntelligentPointer();                // 0x00A01C10
    CBaseObject* GetPItem() const;                     // 0x00A01B50
private:
    void* PData;                                       // +0x04
};
template <class T> class CIntelligentPointer : public CBaseIntelligentPointer {
public:
    T* operator->() const { return (T*)GetPItem(); }
};
class CCreatureActionBase {
public:
    virtual void v00(); virtual void v01(); virtual void v02(); virtual void v03(); virtual void v04(); virtual void v05(); virtual void v06(); virtual void v07(); virtual void v08(); virtual void v09(); virtual void v10(); virtual void v11(); virtual void v12(); virtual void v13(); virtual void v14(); virtual void v15(); virtual void v16(); virtual void v17(); virtual void v18(); virtual void v19(); virtual void v20(); virtual void v21(); virtual void v22(); virtual void v23(); virtual void v24(); virtual void v25(); virtual void v26(); virtual void v27(); virtual void v28(); virtual void v29(); virtual void v30(); virtual void v31(); virtual void v32(); virtual void v33();
    virtual bool IsAlwaysUseAnimationMovement() const;   // slot 34 (+0x88)
    virtual void v35(); virtual void v36(); virtual void v37(); virtual void v38(); virtual void v39(); virtual void v40(); virtual void v41(); virtual void v42(); virtual void v43(); virtual void v44(); virtual void v45(); virtual void v46(); virtual void v47(); virtual void v48(); virtual void v49(); virtual void v50(); virtual void v51(); virtual void v52(); virtual void v53(); virtual void v54(); virtual void v55(); virtual void v56();
protected:
    virtual bool IsUsingAnimationMovement() const;     // slot 57 (+0xe4)
    virtual bool IsUsingForcedMovement() const;        // slot 58 (+0xe8)
    virtual bool IsMovementUsingPhysics() const;       // slot 59 (+0xec)
    virtual void UpdateActionMoveToPos();              // slot 60 (+0xf0)
public:
    unsigned char _pad04[4];                           // +0x04
    CIntelligentPointer<CThingCreatureBase> PCreature; // +0x08
    unsigned char _pad10[0x68 - 0x10];                 // +0x10
    C3DVector     NextMovementPos;                     // +0x68
    unsigned char _pad74[0xa8 - 0x74];                 // +0x74
protected:
    void UpdateActionMovement();
};

void CCreatureActionBase::UpdateActionMovement()
{
    if ((PCreature->UseMovementInActions || IsAlwaysUseAnimationMovement())
        && (IsUsingAnimationMovement() || IsUsingForcedMovement())) {
        PCreature->GetPhysicsTC()->SetAsMoving(true);
        UpdateActionMoveToPos();
        PCreature->GetPhysicsTC()->SetForcedToMovePos(NextMovementPos, IsMovementUsingPhysics());
    }
}