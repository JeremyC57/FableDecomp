// CAIStateGroup_JackOfBladesStage1::GetRushToPos(C3DVector&) -- when the live target is more than 16 units away (XY), the point 8 units from Jack along the target bearing turned half a unit left or right by the zig-zag counter's parity; false when the target is dead or too close
// Grade: DIFFER(506v510) -- two gaps: retail loads Jack into ebp before the target's GetPItem and keeps the target in ebx (push ebx/ebp shrink-wrapped around the two GetPosition calls; ours reuses ebp for both and pushes only ebp), and squares dy before dx for the length (`fld st0` vs our `fld st1`); the arena-centre ternary, fraction wrap, offset select and the sum line up
// Shape: the unused centre position is a ternary of the two GetPosition calls (tail-merged); the offset is chosen by an if/else copy
// from two named constant vectors (both stored first); `*= 8` member-wise; pos = offset + Jack's position.
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00971CD0, length 508 bytes (+ int3 padding), `ret 4`.
// Callees: CBaseIntelligentPointer::GetPItem @ 0x00A01B50 (PArenaCentre / PTargetThing), CThing::GetPosition @ 0x004C73D0 (the
// arena centre's or Jack's, unused; Jack's and the target's; Jack's again), _CIfmod @ 0x00BFEB28 (the fmod intrinsic),
// C3DVector::RotateAboutZ(float) @ 0x00A14040.
// Globals: 16.0f 0x01238164, 1/(2pi) 0x01237F08, 0.0f 0x0122DEDC, 1.0f 0x0122DED8, the double 1.0 0x01237F00, 8.0f 0x012975FC.
// Prototype: FableWin.pdb ?GetRushToPos@CAIStateGroup_JackOfBladesStage1@@IAE_NAAVC3DVector@@@Z.
// Layout: Ego_r.pdb CAIStateGroup_JackOfBladesBase (PArenaCentre +0x1c, PTargetThing +0x24; the two STLport vectors are 12
// bytes, so the base ends at +0x4c) and CAIStateGroup_JackOfBladesStage1 (ZigZagCounter +0x50, PDB +0x58).
extern "C" double __cdecl atan2(double y, double x);
extern "C" double __cdecl fmod(double x, double y);
extern "C" double __cdecl sqrt(double x);
#pragma intrinsic(atan2, fmod, sqrt)

struct C3DVector {
    float X, Y, Z;
    C3DVector() { }
    C3DVector(float x, float y, float z) : X(x), Y(y), Z(z) { }
    C3DVector operator+(const C3DVector& o) const { return C3DVector(X + o.X, Y + o.Y, Z + o.Z); }
    C3DVector& operator*=(float f) { X *= f; Y *= f; Z *= f; return *this; }
    void RotateAboutZ(float angle);             // 0x00A14040
};
struct C2DVector {
    float X, Y;
    C2DVector(float x, float y) : X(x), Y(y) { }
    float GetLength() const { return (float)sqrt(Y * Y + X * X); }
};
struct CThing {
    unsigned char _pad0[0x91];
    unsigned char m_flags91;                    // +0x91 bit 0: Dead
    const C3DVector& GetPosition();             // 0x004C73D0
};
struct CBaseIntelligentPointer {
    virtual void v00();
    CThing* m_pItem;                            // +0x04
    CThing* GetPItem() const;                   // 0x00A01B50
};
inline bool IsThingAlive(const CBaseIntelligentPointer& thing) { return thing.GetPItem() != 0 && !(thing.GetPItem()->m_flags91 & 1); }
struct CAIBrain {
    unsigned char _pad[0x18];
    CThing* PCreature;                          // +0x18
};
struct CAIStateGroupBase {
    virtual ~CAIStateGroupBase();
    CAIBrain* PBrain;                           // +0x04
    unsigned char _pad08[0x0c];                 // +0x08..+0x13
    CThing* GetCreature() const { return PBrain->PCreature; }
};
struct CAIStateGroup_JackOfBladesBase : CAIStateGroupBase {
    CBaseIntelligentPointer PSwordEmitter;      // +0x14
    CBaseIntelligentPointer PArenaCentre;       // +0x1c
    CBaseIntelligentPointer PTargetThing;       // +0x24
    long EnemyKnockDownFrame;                   // +0x2c
    long SpeechCounter;                         // +0x30
    unsigned char PillarMarkers[12];            // +0x34 STLport vector
    unsigned char Pillars[12];                  // +0x40 STLport vector
};
struct CAIStateGroup_JackOfBladesStage1 : CAIStateGroup_JackOfBladesBase {
    bool HasAttacked;                           // +0x4c
    long ZigZagCounter;                         // +0x50
protected:
    bool GetRushToPos(C3DVector& pos);
};

bool CAIStateGroup_JackOfBladesStage1::GetRushToPos(C3DVector& pos)
{
    const C3DVector& centre = IsThingAlive(PArenaCentre) ? PArenaCentre.GetPItem()->GetPosition() : GetCreature()->GetPosition();
    if (!IsThingAlive(PTargetThing))
        return false;
    CThing* target = PTargetThing.GetPItem();
    CThing* me = GetCreature();
    const C3DVector& myPos = me->GetPosition();
    const C3DVector& targetPos = target->GetPosition();
    C2DVector toTarget(targetPos.X - myPos.X, targetPos.Y - myPos.Y);
    if (toTarget.GetLength() > 16.0f) {
        float fraction = (float)atan2(toTarget.X, toTarget.Y) * 0.15915493667125702f;
        if (fraction < 0.0f || fraction >= 1.0f) {
            fraction = (float)fmod(fraction, 1.0);
            if (fraction < 0.0f)
                fraction += 1.0f;
        }
        C3DVector right(0.5f, 1.0f, 0.0f);
        C3DVector left(-0.5f, 1.0f, 0.0f);
        C3DVector offset;
        if (ZigZagCounter % 2)
            offset = right;
        else
            offset = left;
        offset.RotateAboutZ(fraction);
        offset *= 8.0f;
        pos = offset + GetCreature()->GetPosition();
        return true;
    }
    return false;
}
