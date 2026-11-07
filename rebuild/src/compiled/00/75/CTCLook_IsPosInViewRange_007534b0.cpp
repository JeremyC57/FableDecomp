// CTCLook::IsPosInViewRange(const C3DVector& position, float viewAngle, bool fromHead, bool withEyes) const -- false while the parent creature is alive and its animation is in a blocking state (state 7 with queued entries and the flag clear); otherwise the angle from the creature (or its head) to the position must lie within half the view angle of the body facing (plus the head and optionally eye angles), wrapped to [0, 1)
// Grade: RELOCATION_MATCH (chunk 844, 13 attempts after chunks 757/840): three separate early `return false` statements (null creature, dead creature, blocking animation state) give retail's shared `xor al,al` block and jump targets; the angle wrap is written in the body on a named copy `float angle = lookAngle;` (an inline WrapAngle(float) helper is left out of line by VC7.1 once the function has the three early returns, whatever /d2inlS or /d2inlT says); `GFGetAngleXYTo(C3DVector(from), position)` with a POD C3DVector gives the hoisted from-position copy.
// Semantics: retail returns false when PParentCreature is null or its +0x91 bit 0 (dead) is set (je/jne to the xor al,al block); the chunk 757/840 parked source skipped the animation check instead (md.py hid the jump targets).
// Leaf: ?IsPosInViewRange@CTCLook@@QBE_NABVC3DVector@@M_N1@Z
// Retail symbol: ?IsPosInViewRange@CTCLook@@QBE_NABVC3DVector@@M_N1@Z (class-keyed C3DVector, as the local model since chunk 844).
// Earlier grade: DIFFER(ours v retail) -- copy scheduling of the `from` position (three dword moves ordered differently); best build DIFFER(384v384), 3 masked diff lines (chunk 757, 10 attempts)
// Flags: none
// Earlier note: DIFFER(384v384) -- only the copy of the source position into the frame is scheduled differently: retail loads x, y, stores x, loads z, stores y, z; ours load/store pairs (same registers and slots); everything else matches
// Retail: Fable.exe (TLC, VC7.1) function @ 0x007534B0, 384 bytes (+ int3 padding), `ret 10h`. Speed TU (esp frame).
//   Ego_r.pdb: `?IsPosInViewRange@CTCLook@@QBE_NABVC3DVector@@M_N1@Z`.
// Callees: the const CVectorMap LowerBound @ 0x004365B0 (inlined const GetInterface, animation 0x5A; the bitset test
//   folds to `test [thing+28h],4000000h`), CTCAnimation::GetAnimateThing() const @ 0x0070B470 (label
//   "CWorld::DrawGetEnvironment" is propagated: `mov eax,[ecx+0Ch]`), the thing's +0x60 object's virtual slot 63
//   (+0xFC, facing), CTCLook::GetHeadPos() @ 0x00751B30 (landed; hidden-pointer C3DVector), CThing::GetPosition
//   @ 0x004C73D0, AngleBetweenPositions(const C3DVector&, const C3DVector&) @ 0x00661540 (`__fastcall`, float;
//   forge-autoname), _CIfmod @ 0x00BFEB28 (fmod intrinsic), GFGetSignedAngleDifference(float, float) @ 0x00A441E0 (`__fastcall`, Ego_r.pdb ?GFGetSignedAngleDifference@@YIMMM@Z; both floats still go on the stack, `ret 8`).
// Constants: 0.0f 0x0122DEDC, 1.0f 0x0122DED8, 1.0 (double) 0x01237F00.
// Shape: the ternary of GetHeadPos() and the creature position is copied once after the join (memberwise user copy
//   ctor); `return x * 2.0f <= viewAngle;` gives the `mov eax,1 / xor eax,eax` tail separate from the early `xor al,al`.
// Layout: retail CTCLook ParentThing +0x04, PParentCreature +0x10, HeadAngleRelToBodyXY +0x2C, EyeAnglesRelToHeadXY
//   +0x34 (Ego_r.pdb, matches retail here); CThing bitset +0x20, interface map +0x44, +0x60 object, dead flag bit 0 of
//   +0x91; the animate thing's state byte +0x50, flags +0x51 and the entry vector +0x04/+0x08.
#include <math.h>
#pragma intrinsic(fmod)
float __fastcall GFGetSignedAngleDifference(float a, float b);   // 0x00A441E0

class C3DVector {
public:
    float X, Y, Z;
};
float __fastcall GFGetAngleXYTo(const C3DVector& from, const C3DVector& to);        // 0x00661540 (bound ?GFGetAngleXYTo@@YIMABVC3DVector@@0@Z, landed 0x006650E0 / 0x008BF1C0)

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
class CThing { public:
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
    const C3DVector& GetPos() const;                        // 0x004C73D0 (bound ?GetPos@CThing@@QBEABVC3DVector@@XZ)
};
struct CTurnState {                                         // the landed CTCLook::IsCurrentlyTurningViaAnimation 0x00753B40 model (descriptive)
    unsigned char _pad00[0x04];
    void*         EntriesStart;                             // +0x04
    void*         EntriesFinish;                            // +0x08
    unsigned char _pad0c[0x50 - 0x0c];
    unsigned char State;                                    // +0x50
    unsigned char Flags;                                    // +0x51
};
struct CTCTurnSource {                                      // interface type 0x5A = 90 (descriptive, as the landed 0x00753B40 / 0x006D5AF0)
    CTurnState* GetState();                                 // 0x0070B470 (bound ?GetState@CTCTurnSource@@QAEPAUCTurnState@@XZ; `mov eax,[ecx+0Ch]`)
};

class CTCLook { public:
    void*         __vftable;                                // +0x00
    CThing*       ParentThing;                              // +0x04
    unsigned char _pad08[0x10 - 0x08];
    CThing*       PParentCreature;                          // +0x10
    unsigned char _pad14[0x2c - 0x14];
    float         HeadAngleRelToBodyXY;                     // +0x2C
    float         HeadAngleRelToBodyYZ;                     // +0x30
    float         EyeAnglesRelToHeadXY;                     // +0x34
private:
    C3DVector GetHeadPos() const;                           // 0x00751B30 (bound ?GetHeadPos@CTCLook@@ABE?AVC3DVector@@XZ: private)
public:
    bool IsPosInViewRange(const C3DVector& position, float viewAngle, bool fromHead, bool withEyes) const;
};

bool CTCLook::IsPosInViewRange(const C3DVector& position, float viewAngle, bool fromHead, bool withEyes) const
{
    CTCTurnSource* source;
    ParentThing->GetInterface(&source, 0x5a);
    if (PParentCreature == 0)
        return false;
    if (PParentCreature->Flags91 & 1)
        return false;
    CTurnState* state = source->GetState();
    if (state->State == 7 && state->EntriesStart != state->EntriesFinish && !(state->Flags & 2))
        return false;
    float bodyAngle = ParentThing->PPlacement->GetAngleXY();
    C3DVector from = fromHead ? GetHeadPos() : PParentCreature->GetPos();
    float angleToPosition = GFGetAngleXYTo(C3DVector(from), position);
    float lookAngle = bodyAngle;
    if (fromHead)
        lookAngle += HeadAngleRelToBodyXY;
    if (withEyes)
        lookAngle += EyeAnglesRelToHeadXY;
    float angle = lookAngle;
    if (angle < 0.0f || angle >= 1.0f) {
        angle = (float)fmod((double)angle, 1.0);
        if (angle < 0.0f)
            angle += 1.0f;
    }
    return fabs(GFGetSignedAngleDifference(angle, angleToPosition)) * 2.0f <= viewAngle;
}