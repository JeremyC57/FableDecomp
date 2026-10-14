// CCombatActionBase::GetFrameUpdateTime() const -- the base frame time, scaled by the animation entry's speed-variation multiplier while RunningTimeSeconds is inside the strike-speed-variation window
// Retail: Fable.exe (TLC, VC7.1) function @ 0x008571C0, length 88 bytes (+ int3 padding).
// Callees: CCreatureActionBase::GetFrameUpdateTime @ 0x00692BC0 (base virtual, non-virtual call, parked in the
// frame), CAnimationEntry::GetSpeedVariation @ 0x0085AD30 (__fastcall on PAnimEntry, a 4-byte handle class
// returned through the pushed hidden pointer; the manifest's "CThingCreatureBase_GetCombatActionFrameTime" label
// is a placeholder), its multiplier at +8 of the handle's object.
// Members (retail layout, Ego_r.pdb names): RunningTimeSeconds +0x40, PAnimEntry +0x74, StrikeSpeedVariation
// StartTime +0xd0 / EndTime +0xd4 (PDB build: +0xe4/+0xe8; retail CCombatActionBase is 0x114 bytes, local model).
// Shape: `fld [40h] ; fcom [D0h] ; test ah,41h ; jne` (time <= start: skip) then `fcomp [D4h] ; test ah,5 ; jp`
// (time >= end: skip); the product is `fld t ; fmul [handle+8]`.
struct CSpeedVariation {
    unsigned char _pad[8];
    float Multiplier;                           // +0x08
};

struct CSpeedVariationHandle {                  // 4-byte class with a constructor: returned via the hidden pointer
    const CSpeedVariation* m_pVariation;
    CSpeedVariationHandle(const CSpeedVariation* pVariation) : m_pVariation(pVariation) { }
};

struct CAnimationEntry {
    CSpeedVariationHandle GetSpeedVariation();  // 0x0085AD30
};

struct CCombatActionBase {
    virtual void v00();
    unsigned char _pad04[0x40 - 4];             // +0x04
    float RunningTimeSeconds;                   // +0x40
    unsigned char _pad44[0x74 - 0x44];          // +0x44
    CAnimationEntry* PAnimEntry;                // +0x74
    unsigned char _pad78[0xd0 - 0x78];          // +0x78
    float StrikeSpeedVariationStartTime;        // +0xd0
    float StrikeSpeedVariationEndTime;          // +0xd4
    unsigned char _padd8[0x114 - 0xd8];         // +0xd8
    float GetFrameUpdateTime() const;           // 0x00692BC0 (CCreatureActionBase implementation)
    float GetFrameUpdateTimeScaled() const;
};

float CCombatActionBase::GetFrameUpdateTimeScaled() const
{
    float frameTime = CCombatActionBase::GetFrameUpdateTime();
    if (PAnimEntry != 0) {
        CSpeedVariationHandle variation = PAnimEntry->GetSpeedVariation();
        float runningTime = RunningTimeSeconds;       // one x87 load: `fcom` keeps it for the second compare
        if (runningTime > StrikeSpeedVariationStartTime && runningTime < StrikeSpeedVariationEndTime)
            return frameTime * variation.m_pVariation->Multiplier;
    }
    return frameTime;
}