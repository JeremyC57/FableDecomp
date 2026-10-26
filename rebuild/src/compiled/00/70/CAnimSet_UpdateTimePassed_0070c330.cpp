// CAnimSet::UpdateTimePassed(float time) -- advance the set's completion fraction by time * Speed over its first animation's duration (looping sets wrap both fractions past 1 or below 0, others clamp to [0,1] and finish at 1), recording the signed change; a zero-length animation finishes at once
// Retail: Fable.exe (TLC, VC7.1) function @ 0x0070C330, length 333 bytes, ret 4.
// Callees: CMeshDataBank::PeekAnimation(unsigned long) const @ 0x00A242C0 (catalog "CTCInventoryAbilities::GetAbilityLevel",
// propagated), C3DAnimationInfo::GetNonLoopingDuration() const @ 0x00A23CE0 (catalog "CMap::DrawGetSeaLevel", propagated).
// Globals: g_pMainGameComponent @ 0x013B86A0 (+0x24 World -> +0x3c PMainMeshBank), EPSILON 0.0001f @ 0x0129BA3C,
// 1.0f @ 0x0122DED8, 0.0f @ 0x0122DEDC.
// Layout: retail CAnimSet = PDB - 8 (12-byte vectors): Anims +4, CurrentPercentageComplete +0x1c, LastPercentageComplete
// +0x20, Speed +0x24, LastUpdateChange +0x34, flag bits +0x51 (Looping 1, Finished 2, Forwards 4).
// Shape: `fabsf(time) < EPSILON` early out (Last = Current as an integer copy); `time *= Speed` is written back into the
// argument slot; the first animation's info comes through the landed inline PeekAnimInfo (bank in a named local: split
// `mov ax` / `movsx` index load); `Looping ? info.Duration : info.GetNonLoopingDuration()`; the main body sits inside
// `if (duration > EPSILON)` with the zero-length finish (`Current = 1.0f; Finished = true;`, the store scheduled between
// the flag load and store) as the fallthrough tail; the Forwards/Looping tests share one flag-byte load (al), and the
// non-looping clamp goes through `float& current = CurrentPercentageComplete;` -- a store through the reference stops
// VC7.1 reusing that byte for the final `Finished = true`, which retail does as `or byte ptr [esi+51h],2`.
// Leaf: UpdateTimePassed
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
extern const float EPSILON;                        // 0x0129BA3C
#include <math.h>

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
    float Speed;                                   // +0x24
    unsigned long FrameInternalAlphasUpdated;      // +0x28
    float CurrentAlpha;                            // +0x2c
    float LastAlpha;                               // +0x30
    float LastUpdateChange;                        // +0x34
    float ChangeAlphaBy;                           // +0x38
    void* PAnimator;                               // +0x3c
    unsigned long Flags;                           // +0x40
    unsigned long TypeFlags;                       // +0x44
    long AnimDelay;                                // +0x48
    long Priority;                                 // +0x4c
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
    void UpdateTimePassed(float time);
};

void CAnimSet::UpdateTimePassed(float time)
{
    if (fabsf(time) < EPSILON) {
        LastPercentageComplete = CurrentPercentageComplete;
        return;
    }
    LastPercentageComplete = CurrentPercentageComplete;
    time *= Speed;
    const C3DAnimationInfo& info = PeekAnimInfo(Anims[0]);
    float duration = Looping ? info.Duration : info.GetNonLoopingDuration();
    if (duration > EPSILON) {
        float delta = time / duration;
        if (Forwards)
            CurrentPercentageComplete += delta;
        else {
            CurrentPercentageComplete -= delta;
            delta = -delta;
        }
        LastUpdateChange = delta;
        if (Looping) {
            if (LastPercentageComplete > 1.0f) {
                LastPercentageComplete -= 1.0f;
                CurrentPercentageComplete -= 1.0f;
            } else if (LastPercentageComplete < 0.0f) {
                LastPercentageComplete += 1.0f;
                CurrentPercentageComplete += 1.0f;
            }
        } else {
            float& current = CurrentPercentageComplete;
            if (current < 0.0f)
                current = 0.0f;
            else if (current > 1.0f)
                current = 1.0f;
            if (1.0f - EPSILON <= current)
                Finished = true;
        }
        return;
    }
    CurrentPercentageComplete = 1.0f;
    Finished = true;
}