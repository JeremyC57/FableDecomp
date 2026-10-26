// CTCLook::SetBodyMovingViaAnimation() -- while the creature plays a turning animation set (type 7) whose first anim is loaded, turn the body by the current turning anim's rotation over this frame's slice of the animation
// Leaf: SetBodyMovingViaAnimation
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00753930, 401 bytes (+int3), `ret`, esp frame 0x3c (speed TU).
// Callees: CVectorMap::LowerBound @ 0x0040F020 (inline GetInterface 0x5A, unchecked: the miss path reads the dead `turn` slot),
//   CTCAnimationComplex::GetAnimation @ 0x0070B460 (CAnimateThing&, called twice), CAnimateThing::GetMeshBank @ 0x0070C0D0,
//   CMeshDataBank::PeekAnimation @ 0x00A242C0, C3DAnimationInfo::IsLoaded @ 0x00A23F50, GetAnimDuration @ 0x0070B4A0,
//   the turning anim entry @ 0x005DC000 (manifest "_Cons_val"; takes (from, to, float* turn)), physics slots 72 / 75 (+0x120 /
//   +0x12c, get / set the right-handed set), C3DVector::Rotate @ 0x00A13C60, C3DVector::AccurateNormalise @ 0x00A14440.
// Shape: forward is copied before up (two named vectors from the const reference), forward.Z is cleared before up is set to
//   (0,0,1) through a temporary, and SetRHSet gets a CRightHandedSet(up, forward) temporary (the second 24-byte copy).
//   GetTC stores the component as ONE ternary (`(it == last || type < it->type) ? last : it`), and the duration's anim
//   index is read into a named long first (this in ebx, the component in edi, the movsx through edx/eax as retail).
// Layout (retail, local model): CAnimSet in CAnimateThing at +0 (Anims vector<short> +0x04, CurrentPercentageComplete +0x1c,
//   LastPercentageComplete +0x20, Type +0x50; PDB +0x58 with 16-byte vectors); CTCLook FractionOfTurningToAcceptEachFrame +0x44,
//   PCurrentTurningAnim +0x9c (PDB +0xa0).
class C3DVector { public:
    float X, Y, Z;
    C3DVector() { }
    C3DVector(float x, float y, float z) : X(x), Y(y), Z(z) { }
    C3DVector(const C3DVector& o) : X(o.X), Y(o.Y), Z(o.Z) { }
    void Rotate(const C3DVector& axis, float angle);   // 0x00A13C60
    C3DVector& AccurateNormalise();                                  // 0x00A14440 (Ego_r ?AccurateNormalise@C3DVector@@QAEAAV1@XZ)
};
class CRightHandedSet { public:
    C3DVector Up;                                      // +0x00
    C3DVector Forward;                                 // +0x0c
    CRightHandedSet(const C3DVector& up, const C3DVector& forward) : Up(up), Forward(forward) { }
};
class C3DAnimationInfo { public:
    bool IsLoaded() const;                             // 0x00A23F50
};
class CMeshDataBank { public:
    C3DAnimationInfo* PeekAnimation(long index);       // 0x00A242C0
};
struct CShortVector {                                  // STLport vector<short>
    short* _M_start;
    short* _M_finish;
    short* _M_end_of_storage;
    short* begin() { return _M_start; }
    short& operator[](unsigned int n) { return *(begin() + n); }
};
class CAnimateThing { public:                                 // CurrentAnim.MainAnimSet at +0 (retail layout)
    long           UpdateMode;                         // +0x00
    CShortVector   Anims;                              // +0x04 vector<short>
    unsigned char  _pad10[0x1c - 0x10];
    float          CurrentPercentageComplete;          // +0x1c
    float          LastPercentageComplete;             // +0x20
    unsigned char  _pad24[0x50 - 0x24];
    unsigned char  Type;                               // +0x50
    CMeshDataBank* GetMeshBank();                      // 0x0070C0D0
};
class CTCAnimationComplex { public:                           // interface 0x5A
    CAnimateThing& GetAnimation();                     // 0x0070B460
    float GetAnimDuration(long index);                 // 0x0070B4A0
};
class CAnimationEntry { public:
    void GetTurnBetween(float from, float to, float* turn) const;   // 0x005DC000 (name descriptive)
};
class CTCPhysicsBase { public:
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
    virtual const CRightHandedSet& GetRHSet();         // slot 72 (+0x120) (name descriptive)
    virtual void v73(); virtual void v74();
    virtual void SetRHSet(const CRightHandedSet& rh);  // slot 75 (+0x12c) (name descriptive)
};
struct CInterfaceMap {
    struct SEntry { long type; void* component; };
    SEntry* first;
    SEntry* last;
    SEntry* LowerBound(const long* type);              // 0x0040F020
};
#pragma auto_inline(off)
// Same-TU body of the CVectorMap<ETCInterfaceType, CTCBase*>::LowerBound instantiation (a template member, compiled in the
// caller's TU): binary search for the first entry whose type is not below the key read through the pointer.
__declspec(noinline) CInterfaceMap::SEntry* CInterfaceMap::LowerBound(const long* type)
{
    SEntry* it = first;
    long count = last - first;
    while (count > 0) {
        long half = count / 2;
        if (it[half].type < *type) {
            it += half + 1;
            count -= half + 1;
        } else
            count = half;
    }
    return it;
}
#pragma auto_inline(on)

class CThing { public:
    unsigned char  _pad00[0x20];
    unsigned long  m_presentBits[9];                   // +0x20
    CInterfaceMap  m_interfaces;                       // +0x44
    unsigned char  _pad4c[0x60 - 0x4c];
    CTCPhysicsBase* PhysicsTC;                         // +0x60
    template <class T> __forceinline bool GetInterface(T** ppOut, long interfaceType)
    {
        if (m_presentBits[interfaceType >> 5] & (1UL << (interfaceType & 31))) {
            long type = interfaceType;
            CInterfaceMap* map = &m_interfaces;
            CInterfaceMap::SEntry* it = map->LowerBound(&type);
            *ppOut = (T*)((it == map->last || interfaceType < it->type) ? map->last : it)->component;
            return true;
        }
        return false;
    }
    template <class T> __forceinline T* GetTC(long interfaceType)
    {
        T* component;
        GetInterface(&component, interfaceType);
        return component;
    }
};
class CTCLook { public:
    void*            __vftable;                        // +0x00
    CThing*          ParentThing;                      // +0x04
    unsigned char    _pad08[0x44 - 0x08];
    float            FractionOfTurningToAcceptEachFrame;   // +0x44
    unsigned char    _pad48[0x9c - 0x48];
    CAnimationEntry* PCurrentTurningAnim;              // +0x9c (PDB +0xa0)
    void SetBodyMovingViaAnimation();
};

void CTCLook::SetBodyMovingViaAnimation()
{
    CTCAnimationComplex* component = ParentThing->GetTC<CTCAnimationComplex>(0x5a);
    CAnimateThing& animation = component->GetAnimation();
    if (animation.Type == 7 &&
        component->GetAnimation().GetMeshBank()->PeekAnimation(animation.Anims[0])->IsLoaded()) {
        long anim = animation.Anims[0];
        float duration = component->GetAnimDuration(anim);
        float currentTime = duration * animation.CurrentPercentageComplete;
        float lastTime = duration * animation.LastPercentageComplete;
        CTCPhysicsBase* physics = ParentThing->PhysicsTC;
        const CRightHandedSet& current = physics->GetRHSet();
        C3DVector forward = current.Forward;
        C3DVector up = current.Up;
        float turn;
        PCurrentTurningAnim->GetTurnBetween(lastTime, currentTime, &turn);
        float angle = turn * FractionOfTurningToAcceptEachFrame;
        forward.Z = 0.0f;
        up = C3DVector(0.0f, 0.0f, 1.0f);
        forward.Rotate(up, angle);
        forward.AccurateNormalise();
        physics->SetRHSet(CRightHandedSet(up, forward));
    }
}