// Grade: DIFFER(384v384) -- only the copy of the source position into the frame is scheduled differently: retail loads x, y, stores x, loads z, stores y, z; ours load/store pairs (same registers and slots); everything else matches
// CTCLook::IsPosInViewRange(const C3DVector& position, float viewAngle, bool fromHead, bool withEyes) const -- false while the parent creature is alive and its animation is in a blocking state (state 7 with queued entries and the flag clear); otherwise the angle from the creature (or its head) to the position must lie within half the view angle of the body facing (plus the head and optionally eye angles), wrapped to [0, 1)
// Leaf: IsPosInViewRange
// Retail: Fable.exe (TLC, VC7.1) function @ 0x007534B0, 384 bytes (+ int3 padding), `ret 10h`. Speed TU (esp frame).
//   Ego_r.pdb: `?IsPosInViewRange@CTCLook@@QBE_NABVC3DVector@@M_N1@Z`.
// Callees: the const CVectorMap LowerBound @ 0x004365B0 (inlined const GetInterface, animation 0x5A; the bitset test
//   folds to `test [thing+28h],4000000h`), CTCAnimation::GetAnimateThing() const @ 0x0070B470 (label
//   "CWorld::DrawGetEnvironment" is propagated: `mov eax,[ecx+0Ch]`), the thing's +0x60 object's virtual slot 63
//   (+0xFC, facing), CTCLook::GetHeadPos() @ 0x00751B30 (landed; hidden-pointer C3DVector), CThing::GetPosition
//   @ 0x004C73D0, AngleBetweenPositions(const C3DVector&, const C3DVector&) @ 0x00661540 (`__fastcall`, float;
//   forge-autoname), _CIfmod @ 0x00BFEB28 (fmod intrinsic), AngleDifference(float, float) @ 0x00A441E0 (`__stdcall`).
// Constants: 0.0f 0x0122DEDC, 1.0f 0x0122DED8, 1.0 (double) 0x01237F00.
// Shape: the ternary of GetHeadPos() and the creature position is copied once after the join (memberwise user copy
//   ctor); `return x * 2.0f <= viewAngle;` gives the `mov eax,1 / xor eax,eax` tail separate from the early `xor al,al`.
// Layout: retail CTCLook ParentThing +0x04, PParentCreature +0x10, HeadAngleRelToBodyXY +0x2C, EyeAnglesRelToHeadXY
//   +0x34 (Ego_r.pdb, matches retail here); CThing bitset +0x20, interface map +0x44, +0x60 object, dead flag bit 0 of
//   +0x91; the animate thing's state byte +0x50, flags +0x51 and the entry vector +0x04/+0x08.
#include <math.h>
#pragma intrinsic(fmod)
float __stdcall AngleDifference(float a, float b);   // 0x00A441E0

struct C3DVector {
    float X, Y, Z;
    C3DVector() { }
    C3DVector(const C3DVector& o) : X(o.X), Y(o.Y), Z(o.Z) { }
};
float __fastcall AngleBetweenPositions(const C3DVector& from, const C3DVector& to);   // 0x00661540

inline float WrapAngle(float angle)
{
    if (angle < 0.0f || angle >= 1.0f) {
        angle = (float)fmod((double)angle, 1.0);
        if (angle < 0.0f)
            angle += 1.0f;
    }
    return angle;
}

struct CPresenceBits {
    unsigned long words[9];
    bool test(unsigned long position) const { return (words[position >> 5] & (1UL << (position & 31))) != 0; }
};
struct CInterfaceMap {
    struct SEntry { long type; void* component; };
    SEntry* first;
    SEntry* last;
    const SEntry* LowerBound(const long* type) const;       // 0x004365B0
};
struct CThingPlacement {
    virtual void v00(); virtual void v01(); virtual void v02(); virtual void v03(); virtual void v04(); virtual void v05(); virtual void v06(); virtual void v07(); 
    virtual void v08(); virtual void v09(); virtual void v10(); virtual void v11(); virtual void v12(); virtual void v13(); virtual void v14(); virtual void v15(); 
    virtual void v16(); virtual void v17(); virtual void v18(); virtual void v19(); virtual void v20(); virtual void v21(); virtual void v22(); virtual void v23(); 
    virtual void v24(); virtual void v25(); virtual void v26(); virtual void v27(); virtual void v28(); virtual void v29(); virtual void v30(); virtual void v31(); 
    virtual void v32(); virtual void v33(); virtual void v34(); virtual void v35(); virtual void v36(); virtual void v37(); virtual void v38(); virtual void v39(); 
    virtual void v40(); virtual void v41(); virtual void v42(); virtual void v43(); virtual void v44(); virtual void v45(); virtual void v46(); virtual void v47(); 
    virtual void v48(); virtual void v49(); virtual void v50(); virtual void v51(); virtual void v52(); virtual void v53(); virtual void v54(); virtual void v55(); 
    virtual void v56(); virtual void v57(); virtual void v58(); virtual void v59(); virtual void v60(); virtual void v61(); virtual void v62(); 
    virtual float GetAngleXY();                             // slot 63 (+0xFC)
};
struct CThing {
    unsigned char _pad00[0x20];
    CPresenceBits UsingComponents;                          // +0x20
    CInterfaceMap m_interfaces;                             // +0x44
    unsigned char _pad4c[0x60 - 0x4c];
    CThingPlacement* PPlacement;                            // +0x60
    unsigned char _pad64[0x91 - 0x64];
    unsigned char Flags91;                                  // +0x91 (bit 0: dead)
    template<class T> __forceinline bool GetInterface(T** out, long wanted) const
    {
        if (UsingComponents.test(wanted)) {
            long type = wanted;
            const CInterfaceMap* map = &m_interfaces;
            const CInterfaceMap::SEntry* it = map->LowerBound(&type);
            *out = (T*)((it == map->last || wanted < it->type) ? map->last : it)->component;
            return true;
        }
        return false;
    }
    const C3DVector& GetPosition() const;                   // 0x004C73D0
};
struct CAnimateThing {
    unsigned char _pad00[0x04];
    void*         EntriesStart;                             // +0x04
    void*         EntriesFinish;                            // +0x08
    unsigned char _pad0c[0x50 - 0x0c];
    unsigned char State;                                    // +0x50
    unsigned char Flags;                                    // +0x51
};
struct CTCAnimation {                                       // interface type 0x5A
    const CAnimateThing* GetAnimateThing() const;           // 0x0070B470
};

struct CTCLook {
    void*         __vftable;                                // +0x00
    CThing*       ParentThing;                              // +0x04
    unsigned char _pad08[0x10 - 0x08];
    CThing*       PParentCreature;                          // +0x10
    unsigned char _pad14[0x2c - 0x14];
    float         HeadAngleRelToBodyXY;                     // +0x2C
    float         HeadAngleRelToBodyYZ;                     // +0x30
    float         EyeAnglesRelToHeadXY;                     // +0x34
    C3DVector GetHeadPos() const;                           // 0x00751B30
    bool IsPosInViewRange(const C3DVector& position, float viewAngle, bool fromHead, bool withEyes) const;
};

bool CTCLook::IsPosInViewRange(const C3DVector& position, float viewAngle, bool fromHead, bool withEyes) const
{
    CTCAnimation* animation;
    ParentThing->GetInterface(&animation, 0x5a);
    if (PParentCreature && !(PParentCreature->Flags91 & 1)) {
        const CAnimateThing* animate = animation->GetAnimateThing();
        if (animate->State == 7 && animate->EntriesStart != animate->EntriesFinish && !(animate->Flags & 2))
            return false;
    }
    float bodyAngle = ParentThing->PPlacement->GetAngleXY();
    C3DVector from = fromHead ? GetHeadPos() : PParentCreature->GetPosition();
    float angleToPosition = AngleBetweenPositions(from, position);
    float lookAngle = bodyAngle;
    if (fromHead)
        lookAngle += HeadAngleRelToBodyXY;
    if (withEyes)
        lookAngle += EyeAnglesRelToHeadXY;
    return fabs(AngleDifference(WrapAngle(lookAngle), angleToPosition)) * 2.0f <= viewAngle;
}
