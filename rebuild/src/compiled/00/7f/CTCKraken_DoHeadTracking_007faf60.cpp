// CTCKraken::DoHeadTracking() -- turn the kraken's body toward the live target, capped per frame by the beam / targeting track speed
// Retail: Fable.exe (TLC, VC7.1) function @ 0x007FAF60, length 444 bytes (+ int3 padding), esp frame, plain `ret`.
// Callees: CBaseIntelligentPointer::GetPItem @ 0x00A01B50, CThing::GetPos @ 0x004C73D0, _CIfmod @ 0x00BFEB28 (the
// intrinsic fmod of WrapAngle), AngleDifference @ 0x00A441E0 (`__stdcall`), the physics TC's GetFacingAngleXY (slot 63,
// +0xFC) and SetFacingAngleXY (slot 66, +0x108). Constants: 1/(2pi) 0x01237F08, 0.0f 0x0122DEDC, 1.0f 0x0122DED8,
// double 1.0 0x01237F00, 1/360 0x01238E00.
// Shape: HeadTrackTarget && IS_THING_ALIVE(PTarget) (the IsDead() member form: the `& 1` stays an immediate; the raw
// `m_flags91 & 1` form CSEs the 1 into bl and costs a fourth register); the track speed is an if/else on the beam effect;
// the target angle is WrapAngle(atan2(dX, dY) * 1/(2pi)) of the two positions (own GetPos called first). The step is
// `Clamp(AngleDifference(facing, angle), -(speed/360), speed/360)` with the limit written twice in place (CSE'd: fst limit,
// fchs, fstp -limit -- a NAMED limit local swaps the two frame slots); Clamp returns a const reference (the `lea eax`
// three-way pick) and the wrapped sum goes to SetFacingAngleXY.
// Layout (local model): retail STLport lists are 4 bytes (PDB 12), so from ExplosionList (+0x30) on, retail = PDB - 8 per
// list: PBeamWeaponEffect +0x34 (PDB +0x3C), HeadTrackTarget +0x51 (PDB +0x61). CKrakenDef BeamTrackSpeed +0x58,
// TargetingTrackSpeed +0x5C (PDB). CThing PhysicsTC +0x60, dead bit +0x91.
#include <math.h>
#pragma intrinsic(atan2, fmod)

class C3DVector { public: float X, Y, Z; };

class CTCPhysicsBase { public:
    virtual void v00(); virtual void v01(); virtual void v02(); virtual void v03(); virtual void v04();
    virtual void v05(); virtual void v06(); virtual void v07(); virtual void v08(); virtual void v09();
    virtual void v10(); virtual void v11(); virtual void v12(); virtual void v13(); virtual void v14();
    virtual void v15(); virtual void v16(); virtual void v17(); virtual void v18(); virtual void v19();
    virtual void v20(); virtual void v21(); virtual void v22(); virtual void v23(); virtual void v24();
    virtual void v25(); virtual void v26(); virtual void v27(); virtual void v28(); virtual void v29();
    virtual void v30(); virtual void v31(); virtual void v32(); virtual void v33(); virtual void v34();
    virtual void v35(); virtual void v36(); virtual void v37(); virtual void v38(); virtual void v39();
    virtual void v40(); virtual void v41(); virtual void v42(); virtual void v43(); virtual void v44();
    virtual void v45(); virtual void v46(); virtual void v47(); virtual void v48(); virtual void v49();
    virtual void v50(); virtual void v51(); virtual void v52(); virtual void v53(); virtual void v54();
    virtual void v55(); virtual void v56(); virtual void v57(); virtual void v58(); virtual void v59();
    virtual void v60(); virtual void v61(); virtual void v62();
    virtual float GetFacingAngleXY();                   // slot 63 (+0xFC)
    virtual void v64(); virtual void v65();
    virtual void SetFacingAngleXY(float angle);         // slot 66 (+0x108)
};

class CThing { public:
    unsigned char   _pad00[0x60];
    CTCPhysicsBase* PhysicsTC;                          // +0x60
    unsigned char   _pad64[0x91 - 0x64];
    unsigned char   m_flags91;                          // +0x91 bit 0: dead
    const C3DVector& GetPos() const;               // 0x004C73D0
    bool IsDead() const { return (m_flags91 & 1) != 0; }
};

struct CBaseIntelligentPointer {
    void* __vftable;
    void* m_pItem;
    CThing* GetPItem() const;                           // 0x00A01B50
};

inline bool IS_THING_ALIVE(const CBaseIntelligentPointer& p)
{
    if (p.GetPItem() != 0) {
        if (!p.GetPItem()->IsDead())
            return true;
    }
    return false;
}

class CKrakenDef { public:
    unsigned char _pad00[0x58];
    float BeamTrackSpeed;                               // +0x58
    float TargetingTrackSpeed;                          // +0x5c
};

float __stdcall AngleDifference(float a, float b);      // 0x00A441E0

__forceinline float WrapAngle(float angle)
{
    if (angle < 0.0f || angle >= 1.0f) {
        angle = (float)fmod((double)angle, 1.0);
        if (angle < 0.0f)
            angle += 1.0f;
    }
    return angle;
}

template <class T> inline const T& Clamp(const T& v, const T& lo, const T& hi)
{
    return v < lo ? lo : (v > hi ? hi : v);
}

class CTCKraken { public:
    void*                   __vftable;                  // +0x00
    CThing*                 ParentThing;                // +0x04
    unsigned char           _pad08[0x0c - 0x08];
    const CKrakenDef*       PKrakenDef;                 // +0x0c
    CBaseIntelligentPointer PChamber;                   // +0x10
    CBaseIntelligentPointer PCentreMarker;              // +0x18
    CBaseIntelligentPointer PTarget;                    // +0x20
    CBaseIntelligentPointer PHeadEffect;                // +0x28
    void*                   ExplosionList;              // +0x30
    CBaseIntelligentPointer PBeamWeaponEffect;          // +0x34
    long                    DelayBeforeBeam;            // +0x3c
    void*                   PTCBoss;                    // +0x40
    void*                   Tentacles;                  // +0x44
    void*                   AttackingTentacleIterator;  // +0x48
    long                    TentacleStrikeCounter;      // +0x4c
    bool                    TentacleReadyToStrike;      // +0x50
    bool                    HeadTrackTarget;            // +0x51
    void DoHeadTracking();
};

void CTCKraken::DoHeadTracking()
{
    if (HeadTrackTarget && IS_THING_ALIVE(PTarget)) {
        float speed;
        if (IS_THING_ALIVE(PBeamWeaponEffect))
            speed = PKrakenDef->BeamTrackSpeed;
        else
            speed = PKrakenDef->TargetingTrackSpeed;
        CThing* parent = ParentThing;
        CThing* target = PTarget.GetPItem();
        const C3DVector& ownPos = parent->GetPos();
        const C3DVector& targetPos = target->GetPos();
        float angle = WrapAngle((float)atan2(targetPos.X - ownPos.X, targetPos.Y - ownPos.Y) * 0.15915494f);
        float facing = ParentThing->PhysicsTC->GetFacingAngleXY();
        ParentThing->PhysicsTC->SetFacingAngleXY(WrapAngle(facing + Clamp(AngleDifference(facing, angle),
                                                                          -(speed * (1.0f / 360.0f)),
                                                                          speed * (1.0f / 360.0f))));
    }
}