// CAnimSet::GetInterpolatedTime(float alpha, long animIndex) const -- the time into one of the set's animations between the last and current completion fractions: looping sets wrap through the interpolation helper, others lerp linearly, both scaled by the animation's duration
// Retail: Fable.exe (TLC, VC7.1) function @ 0x0070C800, length 168 bytes, ret 8, returns float
// (?GetInterpolatedTime@CAnimSet@@ABEMMJ@Z: private const).
// Callees: GFGetInterpolatedLoopingValue(float last, float current, float alpha, float range, bool forwards) @ 0x0070BD80
//   (Ego_r.pdb ?GFGetInterpolatedLoopingValue@@YIMMMMM_N@Z, landed under that name; `__fastcall`: the bool in cl, the four
//   floats on the stack -- the manifest label AnimationInterpolation_CalcValue is descriptive)
// (static __fastcall: the bool in cl, four floats on the stack, ret 0x10); CMeshDataBank::PeekAnimation(unsigned long) const
// @ 0x00A242C0 (catalog "CTCInventoryAbilities::GetAbilityLevel", propagated; body `return Animations[index]` at +0x380, Ego_r
// 0x00B60570); C3DAnimationInfo::GetNonLoopingDuration() const @ 0x00A23CE0 (catalog "CMap::DrawGetSeaLevel", propagated;
// body `fld [ecx+14h]`). Global: g_pMainGameComponent @ 0x013B86A0 (+0x24 World -> +0x3c PMainMeshBank).
// Shape: Ego_r has the private inline `CAnimSet::PeekAnimInfo(short) const` returning `const C3DAnimationInfo&`; with the bank
// in a named local it keeps retail's split index load (`mov ax,[..]` early, `movsx eax,ax` after the World/bank chain).
// Last/Current are copied into two float locals first (Last loaded first); the Looping bit (bit 0 of +0x51) picks the
// branch; the looping branch reads the Forwards bit (bit 2) into a bool local after PeekAnimInfo (that schedules the
// `mov cl,[esi+51h]` before the argument pushes) and multiplies the helper's result by info.Duration (+0x10) after the
// call; the linear branch multiplies GetNonLoopingDuration() by `(current - last) * alpha + last`.
// Layout: retail CAnimSet (see the copy ctor 0x00710B80): Anims +4 (vector<short>), CurrentPercentageComplete +0x1c,
// LastPercentageComplete +0x20, flag byte +0x51.
#define C3DAnimationInfo C3DAnimationInfo_PDB
#include "engine/C3DAnimationInfo.h"
#undef C3DAnimationInfo
class C3DAnimationInfo : public C3DAnimationInfo_PDB { public:
    float GetNonLoopingDuration() const;           // 0x00A23CE0
};
class CMeshDataBank { public:
    const C3DAnimationInfo* PeekAnimation(unsigned long index) const;   // 0x00A242C0
};
class CWorld { public:
    unsigned char _pad[0x3c];
    CMeshDataBank* PMainMeshBank;                  // +0x3c
};
class CMainGameComponent { public:
    unsigned char _pad[0x24];
    CWorld* World;                                 // +0x24
};
extern CMainGameComponent* g_pMainGameComponent;   // 0x013B86A0

float __fastcall GFGetInterpolatedLoopingValue(float last, float current, float alpha, float range, bool forwards);   // 0x0070BD80

struct CVectorShort {
    short* _M_start; short* _M_finish; short* _M_end_of_storage;
    const short& operator[](long i) const { return _M_start[i]; }
};

class CAnimSet {
public:
    long UpdateMode;                               // +0x00
    CVectorShort Anims;                            // +0x04
    unsigned char InternalAlphas[0xc];             // +0x10 vector<CTransitionBlendAlpha>
    float CurrentPercentageComplete;               // +0x1c
    float LastPercentageComplete;                  // +0x20
    unsigned char _pad24[0x2c];                    // +0x24 .. +0x4f
    unsigned char Type;                            // +0x50
    bool Looping : 1;                              // +0x51 bit 0
    bool Finished : 1;                             // bit 1
    bool Forwards : 1;                             // bit 2
    bool HadUpdate : 1;                            // bit 3
    bool OwnedByMainAnim : 1;                      // bit 4
private:
    const C3DAnimationInfo& PeekAnimInfo(short anim) const
    {
        CMeshDataBank* bank = g_pMainGameComponent->World->PMainMeshBank;
        return *bank->PeekAnimation(anim);
    }
public:
    float GetInterpolatedTime(float alpha, long animIndex) const;
};

float CAnimSet::GetInterpolatedTime(float alpha, long animIndex) const
{
    float last = LastPercentageComplete;
    float current = CurrentPercentageComplete;
    if (Looping) {
        const C3DAnimationInfo& info = PeekAnimInfo(Anims[animIndex]);
        bool forwards = Forwards;
        return GFGetInterpolatedLoopingValue(last, current, alpha, 1.0f, forwards) * info.Duration;
    }
    return PeekAnimInfo(Anims[animIndex]).GetNonLoopingDuration() * ((current - last) * alpha + last);
}