// CTCPhysicsStandard::ReturnToDefaultInteractionFlags() -- reset the interaction flags to the physics def's default
// Leaf: ?ReturnToDefaultInteractionFlags@CTCPhysicsStandard@@UAEXXZ
// Retail: Fable.exe @ 0x00723A50, 16 bytes: `mov edx,[ecx+38h] ; mov edx,[edx+3Ch] ; mov eax,[ecx] ; push edx ;
// call [eax+0ACh] ; ret` (CTCPhysicsStandard vtable 0x01265F84 slot 44, also shared by two derived vtables).
// Identity (proven): Ego_r.pdb names ?ReturnToDefaultInteractionFlags@CTCPhysicsStandard@@UAEXXZ next to
// ?SetInteractionFlags@CTCPhysicsStandard@@UAEXJ@Z; SetInteractionFlags is slot 43 (+0xAC, GOTCHAS wave 25 chunk 168) and the
// body passes it the def's InteractionFlags, so slot 44 is the reset.
// Layout: Ego_r.pdb CTCPhysicsBase::PhysicsDef (CDefPointer<const CPhysicsDef>) +0x38, CPhysicsDef::InteractionFlags +0x3C
// (both at the PDB offsets in retail).
// Header debt: engine/CTCPhysicsStandard.h / CTCPhysicsBase.h flatten the vptr into a data member and leave +0x38 unnamed;
// the virtual override needs this local polymorphic model.
// Shape: `call ; ret` -- the callee pops its argument, this function pops none, so no tail jump.
class CPhysicsDef { public:
    unsigned char _pad00[0x28];
    float Diameter;                                 // +0x28
    float Friction;                                 // +0x2c
    float Elasticity;                               // +0x30
    float AirResistance;                            // +0x34
    float Mass;                                     // +0x38
    long  InteractionFlags;                         // +0x3c
    bool  PushableByHero;                           // +0x40
};
template<class T> class CDefPointer { public:
    T* Object;
    T* operator->() const { return Object; }
};
class C3DVector { public: float X, Y, Z; };
class CThing;
class CTCPhysicsBase { public:
    virtual void v00(); virtual void v01(); virtual void v02(); virtual void v03(); virtual void v04(); virtual void v05();
    virtual void v06(); virtual void v07(); virtual void v08(); virtual void v09(); virtual void v10(); virtual void v11();
    virtual void v12(); virtual void v13(); virtual void v14(); virtual void v15(); virtual void v16(); virtual void v17();
    virtual void v18(); virtual void v19(); virtual void v20(); virtual void v21(); virtual void v22(); virtual void v23();
    virtual void v24(); virtual void v25(); virtual void v26(); virtual void v27(); virtual void v28(); virtual void v29();
    virtual void v30(); virtual void v31(); virtual void v32(); virtual void v33(); virtual void v34(); virtual void v35();
    virtual void v36(); virtual void v37(); virtual void v38(); virtual void v39(); virtual void v40(); virtual void v41();
    virtual void v42();
    virtual void SetInteractionFlags(long flags);   // slot 43
    virtual void ReturnToDefaultInteractionFlags(); // slot 44
    CThing*   ParentThing;                          // +0x04 (CTCBase)
    bool      ToBeRemoved;                          // +0x08
    bool      IsInGlobalUpdateTCs;                  // +0x09
    C3DVector Position;                             // +0x0c
    C3DVector OldPosition;                          // +0x18
    long      OldPositionLastFrameSet;              // +0x24
    C3DVector Velocity;                             // +0x28
    float     Radius;                               // +0x34
    CDefPointer<const CPhysicsDef> PhysicsDef;      // +0x38
};
class CTCPhysicsStandard : public CTCPhysicsBase { public:
    void SetInteractionFlags(long flags);
    void ReturnToDefaultInteractionFlags();
};

void CTCPhysicsStandard::ReturnToDefaultInteractionFlags()
{
    SetInteractionFlags(PhysicsDef->InteractionFlags);
}