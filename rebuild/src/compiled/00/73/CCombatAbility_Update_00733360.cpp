// CCombatAbility::Update(const CCombatAbilityData&) -- count the repeat timer down while non-negative; count the active timer down, and when it just expired reload the repeat timer from SecondsToWaitBeforeRepeat * frames-per-second
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00733360, length 52 bytes.
// Callees: __ftol2 @ 0x00BFEA70 (float -> long). Globals: g_framesPerSecond @ 0x01375550 (a long,
// `fild`; named as in the landed CTimedState00878b00_ResetDuration_00878b00.cpp).
// Layout = engine/CCombatAbility.h (RepeatTimer +0, ActiveTimer +4; flat POD -> `_Retail` model);
// SecondsToWaitBeforeRepeat is at +0xc of the data (engine/CCombatAbilityData.h). The second
// decrement is written as a pre-decrement test (`dec ; cmp eax,-1 ; mov [esi+4],eax ; jne`), and
// the product is `data.SecondsToWaitBeforeRepeat * g_framesPerSecond` (the integer is loaded first).
struct CCombatAbilityData_Calls {
    void* __vftable;
    long  PercentageChance;                                 // +0x4
    float SecondsDuration;                                  // +0x8
    float SecondsToWaitBeforeRepeat;                        // +0xc
};

extern long g_framesPerSecond;                              // 0x01375550

struct CCombatAbility_Retail {
    long RepeatTimer;                                       // +0
    long ActiveTimer;                                       // +4
    void Update(const CCombatAbilityData_Calls& data);
};

void CCombatAbility_Retail::Update(const CCombatAbilityData_Calls& data)
{
    if (RepeatTimer >= 0)
        --RepeatTimer;
    if (ActiveTimer >= 0) {
        if (--ActiveTimer == -1)
            RepeatTimer = (long)(data.SecondsToWaitBeforeRepeat * g_framesPerSecond);
    }
}