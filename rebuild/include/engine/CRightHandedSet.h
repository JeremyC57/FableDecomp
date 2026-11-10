#ifndef FABLE_ENGINE_CRIGHTHANDEDSET_H
#define FABLE_ENGINE_CRIGHTHANDEDSET_H
#include "engine/C3DVector.h"
// Ego_r original class0x9f08: protected Up+0,Forward+12,size24.
// Native CThingCreatureBase::OnHit gets GetRHSet then reads Forward at+12.
#ifndef FABLE_RETAIL_CRIGHTHANDEDSET
class CRightHandedSet { protected: C3DVector Up,Forward; public: const C3DVector& GetForward() const { return Forward; } };
#else
// Retail opt-in (chunk 734, row 0x004A1CB0): Ego_r.pdb / FableWin.pdb ?GetUp@CRightHandedSet@@QBEABVC3DVector@@XZ (inline) and
// ?GetRight@CRightHandedSet@@QBE?AVC3DVector@@XZ, out of line at 0x0049CE00 (landed: Forward x Up) and expanded in
// PreparePrimitivesForEngine (0x004A1D95..0x004A1E01). FableWin.pdb: GetRight (0x01B409F0) is followed by ??TC3DVector (0x01B40A20).
class CRightHandedSet { protected: C3DVector Up,Forward; public: const C3DVector& GetForward() const { return Forward; }
    const C3DVector& GetUp() const { return Up; }
    C3DVector GetRight() const { return Forward ^ Up; }
#ifdef FABLE_RETAIL_CRIGHTHANDEDSET_GETANGLEXZ
    // Retail opt-in (Codex 2026-11-09): Ego_r 00b8b690 / FableWin 031e1ca0,
    // native member callers 00723753 and 007599b3.
    float GetAngleXZ() const;
#endif
    // Chunk 739 (row 0x00B0CAD0): FableWin.pdb ?SetForward@CRightHandedSet@@QAEXABVC3DVector@@@Z / ?SetUp@...@QAEXABVC3DVector@@@Z
    // (inline) and ?MakePerpendicular@CRightHandedSet@@QAEXXZ (out of line at 0x00A22E10, landed users 0x00B9DFC0 / 0x00BD2BC0).
#ifdef FABLE_RETAIL_CRIGHTHANDEDSET_SET
    CRightHandedSet(){}
    void Set(const C3DVector& up,const C3DVector& forward){Up=up;Forward=forward;}
#endif
    void SetForward(const C3DVector& forward) { Forward = forward; }
    void SetUp(const C3DVector& up) { Up = up; }
#ifdef FABLE_RETAIL_CRIGHTHANDEDSET_ROTATE
    void Rotate(const C3DVector& rotate_axis,float angle);
#endif
    void MakePerpendicular();                                            // 0x00A22E10
#ifdef FABLE_RETAIL_CRIGHTHANDEDSET_CTOR_INLINE
    // Retail opt-in (chunk 794, row 0x0079ACC0 CTCSummonSpell::CreateMinion): Ego_r.pdb ??0CRightHandedSet@@QAE@ABVC3DVector@@0@Z with its
    // body visible (the out-of-line COMDAT is 0x00487050; CreateMinion builds the set in place: `mov [esp+1Ch],0 ; mov [esp+20h],0 ;
    // mov [esp+24h],3F800000h` for Up then the negated forward). Mutually exclusive with FABLE_RETAIL_CRIGHTHANDEDSET_CTOR.
    CRightHandedSet() {}
    CRightHandedSet(const C3DVector& up, const C3DVector& forward) : Up(up), Forward(forward) {}
#endif
#ifdef FABLE_RETAIL_CRIGHTHANDEDSET_CTOR
    // Chunk 764 (row 0x007BC004): Ego_r.pdb ??0CRightHandedSet@@QAE@XZ and ??0CRightHandedSet@@QAE@ABVC3DVector@@0@Z (retail 0x00487050,
    // out of line). Declarations only: no layout change.
    CRightHandedSet() {}
    CRightHandedSet(const C3DVector& a, const C3DVector& b);
#endif
#ifdef FABLE_RETAIL_CRIGHTHANDEDSET_DEFAULT_IDENTITY
    // Retail opt-in (chunk 1014, row 0x00703210 CThingObject::Create): FableWin.pdb ??0CRightHandedSet@@QAE@XZ (inline in retail:
    // Create builds the default set in place, `mov [esp+18h],0 ; mov [esp+1Ch],0 ; mov [esp+20h],3F800000h` Up = (0,0,1), then
    // Forward = (0,1,0)). Mutually exclusive with the other default-constructor opt-ins.
    CRightHandedSet() { Up.X = 0.0f; Up.Y = 0.0f; Up.Z = 1.0f; Forward.X = 0.0f; Forward.Y = 1.0f; Forward.Z = 0.0f; }
#endif
#ifdef FABLE_RETAIL_CRIGHTHANDEDSET_ANGLES
    // Chunk 771 (row 0x006A5D90): Ego_r.pdb ??0CRightHandedSet@@QAE@ABVC3DAngles@@@Z (out of line at 0x00A229E0; the level script's
    // AngleXY / AngleYZ / AngleXZ triple). A declaration only.
    CRightHandedSet(const class C3DAngles& angles);                      // 0x00A229E0
#endif
};
#endif
#endif
