// CCombatActionBase::UpdateFacingDirection() -- turn the creature to face its target: nothing without a live
//   target distinct from the creature, when the action says not to, or while the animation is rotating; the facing
//   fraction is atan2 of the target delta over 2*pi wrapped into [0,1); inside the facing distance it snaps,
//   otherwise it turns only when the tolerance is under 0.5 and the current facing is off by more than it
// Leaf: UpdateFacingDirection
// Retail: Fable.exe (TLC, VC7.1) @ 0x00856B20, 513 bytes (528 with padding).
// Callees: CBaseIntelligentPointer::GetPItem 0x00A01B50, CAnimationEntry::GetRotationInAnimation 0x005DBFD0,
//   CThing::GetPosition 0x004C73D0, fmod (intrinsic) 0x00BFEB28, AngleDifference 0x00A441E0 (__stdcall).
// Layout: Ego_r.pdb CCreatureActionBase: PCreature +0x08, PAnimEntry +0x74; CCombatActionBase PTarget PDB +0xb4 =
//   retail +0xa8 (retail CCreatureActionBase is 0xa8: its three STLport containers are 4 bytes smaller each).
extern "C" double __cdecl atan2(double y, double x);
extern "C" double __cdecl fmod(double x, double y);
extern "C" double __cdecl fabs(double x);
#pragma intrinsic(atan2, fmod, fabs)
inline float fabs(float x) { return (float)fabs((double)x); }
class C3DVector { public: float X, Y, Z; };
float __stdcall AngleDifference(float a, float b);   // 0x00A441E0
struct CPhysicsBody {
    virtual void p000(); virtual void p001(); virtual void p002(); virtual void p003(); virtual void p004(); virtual void p005(); virtual void p006(); virtual void p007();
    virtual void p008(); virtual void p009(); virtual void p010(); virtual void p011(); virtual void p012(); virtual void p013(); virtual void p014(); virtual void p015();
    virtual void p016(); virtual void p017(); virtual void p018(); virtual void p019(); virtual void p020(); virtual void p021(); virtual void p022(); virtual void p023();
    virtual void p024(); virtual void p025(); virtual void p026(); virtual void p027(); virtual void p028(); virtual void p029(); virtual void p030(); virtual void p031();
    virtual void p032(); virtual void p033(); virtual void p034(); virtual void p035(); virtual void p036(); virtual void p037(); virtual void p038(); virtual void p039();
    virtual void p040(); virtual void p041(); virtual void p042(); virtual void p043(); virtual void p044(); virtual void p045(); virtual void p046(); virtual void p047();
    virtual void p048(); virtual void p049(); virtual void p050(); virtual void p051(); virtual void p052(); virtual void p053(); virtual void p054(); virtual void p055();
    virtual void p056(); virtual void p057(); virtual void p058(); virtual void p059(); virtual void p060(); virtual void p061(); virtual void p062(); virtual void p063();
    virtual void p064(); virtual void p065();
    virtual void SetFacingAngle(float fraction);   // slot 66 (+0x108)
    virtual void p067(); virtual void p068(); virtual void p069();
    virtual C3DVector GetFacing();   // slot 70 (+0x118)
};
class CThing { public:
    unsigned char _pad00[0x60];
    CPhysicsBody* m_pPhysics;                   // +0x60
    const C3DVector& GetPosition() const;       // 0x004C73D0
    CPhysicsBody* GetPhysics() const { return m_pPhysics; }
};
class CAnimationEntry { public:
    void GetRotationInAnimation(float* pRotation) const;   // 0x005DBFD0
};
struct CBaseIntelligentPointer {
    virtual void v00();
    CThing* m_pItem;                            // +0x04
    CThing* GetPItem() const;                   // 0x00A01B50
};
__forceinline float FacingFraction(float x, float y)
{
    float fraction = (float)atan2(x, y) * 0.15915493667125702f;
    if (fraction < 0.0f || fraction >= 1.0f) {
        fraction = (float)fmod(fraction, 1.0);
        if (fraction < 0.0f)
            fraction += 1.0f;
    }
    return fraction;
}
class CCombatActionBase { public:
    virtual void v000(); virtual void v001(); virtual void v002(); virtual void v003(); virtual void v004(); virtual void v005(); virtual void v006(); virtual void v007();
    virtual void v008(); virtual void v009(); virtual void v010(); virtual void v011(); virtual void v012(); virtual void v013(); virtual void v014(); virtual void v015();
    virtual void v016(); virtual void v017(); virtual void v018(); virtual void v019(); virtual void v020(); virtual void v021(); virtual void v022(); virtual void v023();
    virtual void v024(); virtual void v025(); virtual void v026(); virtual void v027(); virtual void v028(); virtual void v029(); virtual void v030(); virtual void v031();
    virtual void v032(); virtual void v033(); virtual void v034(); virtual void v035(); virtual void v036(); virtual void v037(); virtual void v038(); virtual void v039();
    virtual void v040(); virtual void v041(); virtual void v042(); virtual void v043(); virtual void v044(); virtual void v045(); virtual void v046(); virtual void v047();
    virtual void v048(); virtual void v049(); virtual void v050(); virtual void v051(); virtual void v052(); virtual void v053(); virtual void v054(); virtual void v055();
    virtual void v056(); virtual void v057(); virtual void v058(); virtual void v059(); virtual void v060(); virtual void v061(); virtual void v062(); virtual void v063();
    virtual void v064(); virtual void v065(); virtual void v066(); virtual void v067(); virtual void v068(); virtual void v069(); virtual void v070(); virtual void v071();
    virtual void v072();
    virtual bool IsToUpdateFacing();   // slot 73 (+0x124)
    virtual void v074(); virtual void v075(); virtual void v076(); virtual void v077();
    virtual float GetFacingTolerance();   // slot 78 (+0x138)
    virtual float GetFacingDistance();   // slot 79 (+0x13c)
    unsigned char           _pad04[4];
    CBaseIntelligentPointer PCreature;          // +0x08
    unsigned char           _pad10[0x74 - 0x10];
    CAnimationEntry*        PAnimEntry;         // +0x74
    unsigned char           _pad78[0xa8 - 0x78];
    CBaseIntelligentPointer PTarget;            // +0xa8
    void UpdateFacingDirection();
};

void CCombatActionBase::UpdateFacingDirection()
{
    if (PTarget.GetPItem() == 0)
        return;
    if (PTarget.GetPItem() == PCreature.GetPItem())
        return;
    if (!IsToUpdateFacing())
        return;
    if (PAnimEntry) {
        float rotation;
        PAnimEntry->GetRotationInAnimation(&rotation);
        if (fabs(rotation) > 0.0001f)
            return;
    }
    CThing* target = PTarget.GetPItem();
    CThing* creature = PCreature.GetPItem();
    const C3DVector& from = creature->GetPosition();
    const C3DVector& to = target->GetPosition();
    C3DVector delta;
    delta.X = to.X - from.X;
    delta.Y = to.Y - from.Y;
    float facing = FacingFraction(delta.X, delta.Y);
    float distance = GetFacingDistance();
    if (delta.Y * delta.Y + delta.X * delta.X > distance * distance) {
        float tolerance = GetFacingTolerance();
        if (tolerance < 0.5f) {
            C3DVector current = PCreature.GetPItem()->GetPhysics()->GetFacing();
            float currentFacing = FacingFraction(current.X, current.Y);
            if (fabs(AngleDifference(currentFacing, facing)) > tolerance)
                return;
        }
    }
    PCreature.GetPItem()->GetPhysics()->SetFacingAngle(facing);
}