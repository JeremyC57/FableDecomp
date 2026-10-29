// CTCRotate::FrameUpdate() -- while rotating: flag the thing's draw state, rotate its orientation about RotationAxis by Timer*RotationSpeed, write it back, and remove this component (type 0x2A) once the timer passes its limit
// Leaf: FrameUpdate
// Retail: Fable.exe 0x0072A570, 155 bytes (plain `ret`).
// Callees: the thing's +0x60 physics/placement object virtual slot 72 (+0x120, GetOrientation, returns const
// CRightHandedSet&) and slot 75 (+0x12C, SetOrientation(const CRightHandedSet&)), CRightHandedSet rotate-about-axis
// @ 0x00A22F80 (manifest "CMatrix3x3::MultiplyByQuaternionRotation", unverified; thiscall (const C3DVector&, float)),
// CThing::RemoveTC(long) @ 0x004C9840 (manifest "CBaseObject::OnPropertyChanged" is propagated).
// Layout = Ego_r.pdb CTCRotate (0x38): RotationAxis +0x0C, RotationSpeed +0x18, NumRotations +0x1C, Timer +0x20,
// TimerLimit +0x24, Rotating +0x28; CThing: placement object +0x60, draw-state object +0x64 (flag byte +0x59 bit 1).
// Shape: the orientation is copied into a 24-byte local by the implicit memberwise copy (dword moves interleaved
// with the fild/fmul of the angle).
class C3DVector { public: float X, Y, Z; };
class CRightHandedSet { public:
    C3DVector Up;
    C3DVector Forward;
    void Rotate(const C3DVector& axis, float angle);   // 0x00A22F80
};
struct CThingPlacement {
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
    virtual void v70();
    virtual void v71();
    virtual const CRightHandedSet& GetOrientation() const;      // slot 72 (+0x120)
    virtual void v73();
    virtual void v74();
    virtual void SetOrientation(const CRightHandedSet& rhs);    // slot 75 (+0x12c)
};
struct CThingDrawState {
    unsigned char _pad00[0x59];
    unsigned char m_flags59;                                    // +0x59 (bit 1: orientation changed)
};
class CThing { public:
    unsigned char    _pad00[0x60];
    CThingPlacement* m_pPlacement;                              // +0x60
    CThingDrawState* m_pDrawState;                              // +0x64
    void RemoveTC(long type);                                   // 0x004C9840
};
class CTCRotate { public:
    virtual void v00();
    CThing*   ParentThing;                                      // +0x04 (CTCBase)
    bool      ToBeRemoved;                                      // +0x08
    bool      IsInGlobalUpdateTCs;                              // +0x09
    C3DVector RotationAxis;                                     // +0x0c
    float     RotationSpeed;                                    // +0x18
    float     NumRotations;                                     // +0x1c
    long      Timer;                                            // +0x20
    long      TimerLimit;                                       // +0x24
    bool      Rotating;                                         // +0x28
    void FrameUpdate();
};

void CTCRotate::FrameUpdate()
{
    if (Rotating) {
        ParentThing->m_pDrawState->m_flags59 |= 2;
        CRightHandedSet rhs = ParentThing->m_pPlacement->GetOrientation();
        rhs.Rotate(RotationAxis, RotationSpeed * Timer);
        ParentThing->m_pPlacement->SetOrientation(rhs);
        if (++Timer > TimerLimit)
            ParentThing->RemoveTC(0x2a);
    }
}