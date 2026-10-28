// CAIStateGroup_CreatureUseCoverBase::CanSeeTarget() -- with a live target and cover thing: true when the cover has no physics mesh, else when the eye-height line from the creature to the target does not hit the cover's mesh
// Retail: Fable.exe (TLC, VC7.1) @ 0x00970790, 264 bytes (`ret`). Ego_r.pdb: ?CanSeeTarget@CAIStateGroup_CreatureUseCoverBase@@IAE_NXZ.
// Callees: CBaseIntelligentPointer::GetPItem 0x00A01B50 (IS_THING_ALIVE twice, then the cover/target things),
// CTCPhysicsBase virtual slot 93 (+0x174, GetPhysicsMeshInfo), CThing::GetPos 0x004C73D0,
// CTCPhysicsBase::GetPhysicsMeshObject 0x006B0850 (CPhysicsMeshObject by hidden pointer) / ~CPhysicsMeshObject 0x00A127D0,
// CPhysicsMeshObject::GetPointIntersectionFromTo(const C3DVector*, const C3DVector*, CMeshIntersectionData*) 0x00A12450;
// 1.2f pool 0x0129749C (eye height).
// Shape: IS_THING_ALIVE on PTarget (+0x14) and PCoverThing (+0x1f, a pack(1) intelligent pointer); the cover's physics
// component reached through an inline accessor (`GetPhysics()`) so the slot-93 call's object lands straight in ecx (a
// direct member read goes through eax, and recolours this/&PTarget); the two line ends
// built as `C3DVector(p.X, p.Y, p.Z + 1.2f)` (Z first: value-ctor arguments right to left); the intersection call's
// arguments are pushed before the by-value mesh-object temporary is made; the bool is kept across its dtor.
// Layout: engine/CAIStateGroup_CreatureUseCoverBase.h (PCoverThing +0x1f); PTarget +0x14 (8-byte pointer before it).
// Leaf: CanSeeTarget@CAIStateGroup_CreatureUseCoverBase
class CThing;

class C3DVector { public:
    float X, Y, Z;
    C3DVector(float x, float y, float z) : X(x), Y(y), Z(z) {}
};

#pragma pack(push, 1)
class CBaseObject;
class CBaseObjectPointer;
class CBaseIntelligentPointer {                       // vtable 0x0129C95C (Ego_r: vptr + PData, 8 bytes)
public:
    CBaseIntelligentPointer();                           // 0x00A01B10
    virtual ~CBaseIntelligentPointer();                  // 0x00A01C10
    CBaseObject* GetPItem() const;                       // 0x00A01B50
    void SetPItem(CBaseObject* item);                    // 0x00A01B90
private:
    CBaseObjectPointer* PData;                           // +0x04
};
template <class T> class CIntelligentPointer : public CBaseIntelligentPointer {
public:
    T* GetPItem() const { return reinterpret_cast<T*>(CBaseIntelligentPointer::GetPItem()); }
    void SetPItem(T* item) { CBaseIntelligentPointer::SetPItem(reinterpret_cast<CBaseObject*>(item)); }
};
#pragma pack(pop)

class CMeshIntersectionData { public: unsigned char _body[0x1c]; };
class CPhysicsMeshObject { public:
    unsigned char _body[0x40 - 4];
    virtual ~CPhysicsMeshObject();                         // 0x00A127D0
    bool GetPointIntersectionFromTo(const C3DVector* from, const C3DVector* to, CMeshIntersectionData* data);   // 0x00A12450
};
struct CTCPhysicsBaseV {                           // vtable view: slot 93 (+0x174)
    virtual void v00(); virtual void v01(); virtual void v02(); virtual void v03(); virtual void v04(); virtual void v05();
    virtual void v06(); virtual void v07(); virtual void v08(); virtual void v09(); virtual void v10(); virtual void v11();
    virtual void v12(); virtual void v13(); virtual void v14(); virtual void v15(); virtual void v16(); virtual void v17();
    virtual void v18(); virtual void v19(); virtual void v20(); virtual void v21(); virtual void v22(); virtual void v23();
    virtual void v24(); virtual void v25(); virtual void v26(); virtual void v27(); virtual void v28(); virtual void v29();
    virtual void v30(); virtual void v31(); virtual void v32(); virtual void v33(); virtual void v34(); virtual void v35();
    virtual void v36(); virtual void v37(); virtual void v38(); virtual void v39(); virtual void v40(); virtual void v41();
    virtual void v42(); virtual void v43(); virtual void v44(); virtual void v45(); virtual void v46(); virtual void v47();
    virtual void v48(); virtual void v49(); virtual void v50(); virtual void v51(); virtual void v52(); virtual void v53();
    virtual void v54(); virtual void v55(); virtual void v56(); virtual void v57(); virtual void v58(); virtual void v59();
    virtual void v60(); virtual void v61(); virtual void v62(); virtual void v63(); virtual void v64(); virtual void v65();
    virtual void v66(); virtual void v67(); virtual void v68(); virtual void v69(); virtual void v70(); virtual void v71();
    virtual void v72(); virtual void v73(); virtual void v74(); virtual void v75(); virtual void v76(); virtual void v77();
    virtual void v78(); virtual void v79(); virtual void v80(); virtual void v81(); virtual void v82(); virtual void v83();
    virtual void v84(); virtual void v85(); virtual void v86(); virtual void v87(); virtual void v88(); virtual void v89();
    virtual void v90(); virtual void v91(); virtual void v92();
    virtual void* GetPhysicsMeshInfo();            // slot 93 (+0x174)
    CPhysicsMeshObject GetPhysicsMeshObject();     // 0x006B0850
};

class CThing { public:
    unsigned char _pad[0x60];
    CTCPhysicsBaseV* Physics;                      // +0x60
    CTCPhysicsBaseV* GetPhysics() const { return Physics; }
    unsigned char _pad64[0x2d];
    unsigned char m_flags91;                       // +0x91 bit 0: dead
    const C3DVector& GetPos() const;          // 0x004C73D0
    bool IsDead() const { return (m_flags91 & 1) != 0; }
};

inline bool IS_THING_ALIVE(const CIntelligentPointer<CThing>& p)
{
    if (p.GetPItem() != 0) {
        if (!p.GetPItem()->IsDead())
            return true;
    }
    return false;
}

class CAIBrain { public:
    unsigned char _pad[0x18];
    CThing* PCreature;                             // +0x18
};

#pragma pack(push, 1)
struct CAIStateGroup_CreatureUseCoverBase {
    virtual ~CAIStateGroup_CreatureUseCoverBase();
    CAIBrain* PBrain;                              // +0x04
    unsigned char _pad08[0x0c];                    // +0x08
    CIntelligentPointer<CThing> PTarget;               // +0x14
    bool HasDiveAnim;                              // +0x1c
    bool HasJumpOutAnims;                          // +0x1d
    bool TimingCritical;                           // +0x1e
    CIntelligentPointer<CThing> PCoverThing;           // +0x1f
    bool CanSeeTarget();
};
#pragma pack(pop)

bool CAIStateGroup_CreatureUseCoverBase::CanSeeTarget()
{
    if (IS_THING_ALIVE(PTarget) && IS_THING_ALIVE(PCoverThing)) {
        if (PCoverThing.GetPItem()->GetPhysics()->GetPhysicsMeshInfo()) {
            const C3DVector& eye = PBrain->PCreature->GetPos();
            C3DVector from(eye.X, eye.Y, eye.Z + 1.2f);
            const C3DVector& target = PTarget.GetPItem()->GetPos();
            C3DVector to(target.X, target.Y, target.Z + 1.2f);
            CTCPhysicsBaseV* physics = PCoverThing.GetPItem()->Physics;
            CMeshIntersectionData data;
            bool blocked = physics->GetPhysicsMeshObject().GetPointIntersectionFromTo(&from, &to, &data);
            if (blocked)
                return false;
        }
        return true;
    }
    return false;
}