// Grade: DIFFER(162v168) -- retail loads the short anim index with `mov ax` and sign-extends after the engine chain (`movsx eax,ax`), ours `movsx` from memory; the helper argument loads are scheduled differently | tried: plain index, short local + operator[], GetAnimation(short) | next: a by-value inline short getter on CAnimSet (GetAnimIndex) or the index as a ushort/short parameter of an inline engine accessor
// CAnimSet::GetInterpolatedTime(float alpha, long animIndex) -- the time into one of the set's animations between the last and current completion fractions: looping sets wrap through the interpolation helper, others lerp linearly, both scaled by the animation's length
// Retail: Fable.exe (TLC, VC7.1) function @ 0x0070C800, length 168 bytes, ret 8, returns float.
// Callees: AnimationInterpolation_CalcValue(bool forwards, float last, float current, float alpha, float range) @ 0x0070BD80
// (static __fastcall: the bool in cl, four floats on the stack, ret 0x10), the animation bank's GetAnimation(long) @ 0x00A242C0
// (catalog "CTCInventoryAbilities::GetAbilityLevel", propagated), CAnimation::GetDuration() @ 0x00A23CE0 (catalog
// "CMap::DrawGetSeaLevel", propagated). Global: the engine singleton @ 0x013B86A0 (+0x24 -> +0x3c = the animation bank).
// Shape: Last/Current are copied into two float locals first (Last loaded first); the Looping bit (bit 0 of +0x51) picks the
// branch, the Forwards bit (bit 2) is the helper's first argument; the looping product reads the animation's length member
// (+0x10) after the call, the linear branch calls GetDuration() and multiplies it by `(current - last) * alpha + last`.
// Layout: retail CAnimSet (see the copy ctor 0x00710B80): Anims +4 (vector<short>), CurrentPercentageComplete +0x1c,
// LastPercentageComplete +0x20, flag byte +0x51.
struct CAnimation {
    unsigned char _pad[0x10];
    float Length;                                  // +0x10
    float GetDuration();                           // 0x00A23CE0
};
struct CAnimationBank {
    CAnimation* GetAnimation(short index);         // 0x00A242C0
};
struct CVectorShort {
    short* _M_start; short* _M_finish; short* _M_end_of_storage;
    short operator[](long i) const { return _M_start[i]; }
};
struct CEngineAnimationSystem {
    unsigned char _pad[0x3c];
    CAnimationBank* AnimationBank;                 // +0x3c
};
struct CEngine {
    unsigned char _pad[0x24];
    CEngineAnimationSystem* AnimationSystem;       // +0x24
};
extern CEngine* g_pEngine;                         // 0x013B86A0

float __fastcall AnimationInterpolation_CalcValue(bool forwards, float last, float current, float alpha, float range);   // 0x0070BD80


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
    float GetInterpolatedTime(float alpha, long animIndex);
};

float CAnimSet::GetInterpolatedTime(float alpha, long animIndex)
{
    float last = LastPercentageComplete;
    float current = CurrentPercentageComplete;
    if (Looping) {
        short index = Anims[animIndex];
        CAnimation* anim = g_pEngine->AnimationSystem->AnimationBank->GetAnimation(index);
        return AnimationInterpolation_CalcValue(Forwards, last, current, alpha, 1.0f) * anim->Length;
    }
    short index = Anims[animIndex];
    return g_pEngine->AnimationSystem->AnimationBank->GetAnimation(index)->GetDuration() *
           ((current - last) * alpha + last);
}
