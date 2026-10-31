// CThingHeadingGauge::GetHeadingAlignment() const -- 0..40 by how closely the thing's heading matches the world reference direction (within a quarter turn), 0 beyond, 0xff when the facing flag is off
// Grade: RELOCATION_MATCH NAME REVIEW (retry wave 2 sec2b): the world taken as a named `const CWorld& world = PeekWorld();` reference loads the +0x18 object straight into ecx like retail (was 490v488). Identity unknown (no caller/vtable/data ref): every class/member name is descriptive; 0x004C7AB0 spelled CThing::PeekWorld as in the landed rows.
// Leaf: ?GetHeadingAlignment@
// Retail: Fable.exe (TLC, VC7.1) @ 0x00702E00, 488 bytes, __thiscall, `ret`, returns a byte (0xff / 0 / 0..40).
// Identity: unknown -- no direct call and no vtable or data reference anywhere in the image (an unreferenced body the
//   linker kept); catalog "CViewBase_GetRotationDegrees" (forge autoname-t5) was descriptive and wrong (0x004C7AB0 is the
//   this-ignoring CThing::GetWorld, g_013b8a1c->+0x24). Every class/member name here is descriptive.
// Callees: the +0x60 object's slot 70 (+0x118, a C3DVector returned through a hidden pointer), CThing::GetWorld
//   0x004C7AB0, the world +0x18 object's slot 4 (+0x10, a vector by reference), _CIfmod 0x00BFEB28 (x4), __ftol2
//   0x00BFEA70.
// Shape: `if (flags(+0xD0 -> +0x90) & 0x2000) {...} return 0xff;` (the 0xff exit last); each angle is
//   atan2(X, Y) as float times 1/2pi (`fmul dword`) normalised into [0, 1) by a __forceinline helper (`< 0 || >= 1` then
//   fmod(a, 1.0) and +1 when negative); the difference normalises both again (the reference into a frame copy, the
//   heading on the x87 stack), takes fabs, wraps above 0.5, returns 0 above 0.25 else (1 - 4d) * 40.
extern "C" double __cdecl atan2(double y, double x);
extern "C" double __cdecl fmod(double x, double y);
extern "C" double __cdecl fabs(double x);
#pragma intrinsic(atan2, fmod, fabs)

class C3DVector {
public:
    float X, Y, Z;
};

class CHeadingSource {                              // +0x60 member: slot 70 returns the heading by value
public:
    virtual void v00(); virtual void v01(); virtual void v02(); virtual void v03(); virtual void v04(); virtual void v05(); virtual void v06(); virtual void v07(); virtual void v08(); virtual void v09();
    virtual void v10(); virtual void v11(); virtual void v12(); virtual void v13(); virtual void v14(); virtual void v15(); virtual void v16(); virtual void v17(); virtual void v18(); virtual void v19();
    virtual void v20(); virtual void v21(); virtual void v22(); virtual void v23(); virtual void v24(); virtual void v25(); virtual void v26(); virtual void v27(); virtual void v28(); virtual void v29();
    virtual void v30(); virtual void v31(); virtual void v32(); virtual void v33(); virtual void v34(); virtual void v35(); virtual void v36(); virtual void v37(); virtual void v38(); virtual void v39();
    virtual void v40(); virtual void v41(); virtual void v42(); virtual void v43(); virtual void v44(); virtual void v45(); virtual void v46(); virtual void v47(); virtual void v48(); virtual void v49();
    virtual void v50(); virtual void v51(); virtual void v52(); virtual void v53(); virtual void v54(); virtual void v55(); virtual void v56(); virtual void v57(); virtual void v58(); virtual void v59();
    virtual void v60(); virtual void v61(); virtual void v62(); virtual void v63(); virtual void v64(); virtual void v65(); virtual void v66(); virtual void v67(); virtual void v68(); virtual void v69();
    virtual C3DVector GetHeading() const;           // slot 70 (+0x118)
};
class CReferenceDirection {                         // world +0x18: slot 4 returns the reference direction
public:
    virtual void v00(); virtual void v01(); virtual void v02(); virtual void v03();
    virtual const C3DVector& GetDirection() const;  // slot 4 (+0x10)
};
class CWorld {
public:
    unsigned char _pad00[0x18];
    CReferenceDirection* PReferenceDirection;      // +0x18
};
class CFlagsOwner {
public:
    unsigned char _pad00[0x90];
    unsigned long Flags;                            // +0x90 (bit 13 = heading tracked)
};

__forceinline float NormaliseRevolutions(float angle)     // into [0, 1)
{
    if (angle < 0.0f || angle >= 1.0f) {
        angle = (float)fmod(angle, 1.0);
        if (angle < 0.0f)
            angle += 1.0f;
    }
    return angle;
}
__forceinline float GetHeadingRevolutions(const C3DVector& v)
{
    return NormaliseRevolutions((float)atan2(v.X, v.Y) * 0.159154943f);
}

class CThingHeadingGauge {
public:
    unsigned char _pad00[0x60];
    CHeadingSource* PHeadingSource;                 // +0x60
    unsigned char _pad64[0xd0 - 0x64];
    CFlagsOwner* PFlagsOwner;                       // +0xD0
    const CWorld& PeekWorld() const;                // 0x004C7AB0
    unsigned char GetHeadingAlignment() const;
};

unsigned char CThingHeadingGauge::GetHeadingAlignment() const
{
    if (PFlagsOwner->Flags & 0x2000) {
        float heading = GetHeadingRevolutions(PHeadingSource->GetHeading());
        const CWorld& world = PeekWorld();
        float reference = GetHeadingRevolutions(world.PReferenceDirection->GetDirection());
        float difference = (float)fabs(NormaliseRevolutions(reference) - NormaliseRevolutions(heading));
        if (difference > 0.5f)
            difference = 1.0f - difference;
        if (difference > 0.25f)
            return 0;
        return (unsigned char)((1.0f - difference * 4.0f) * 40.0f);
    }
    return 0xff;
}
