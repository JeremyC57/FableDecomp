// CCombatAbility::Update(const CCombatAbilityData&) -- count the repeat timer down while non-negative; count the active timer down, and when it just expired reload the repeat timer from SecondsToWaitBeforeRepeat * frames-per-second
// Naming wave 18: the global stand-in g_framesPerSecond (0x01375550) is the protected static long CMainGameComponent::ConstantFPS, read through the
//   inline static accessor GetConstantFPS() as retail code outside the class does (name-only: the bytes are unchanged).
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00733360, length 52 bytes.
// Callees: __ftol2 @ 0x00BFEA70 (float -> long). Globals: CMainGameComponent::GetConstantFPS() @ 0x01375550 (a long,
// `fild`; named as in the landed CTimedState00878b00_ResetDuration_00878b00.cpp).
// Layout = engine/CCombatAbility.h (RepeatTimer +0, ActiveTimer +4; flat POD -> `_Retail` model);
// SecondsToWaitBeforeRepeat is at +0xc of the data (engine/CCombatAbilityData.h). The second
// decrement is written as a pre-decrement test (`dec ; cmp eax,-1 ; mov [esi+4],eax ; jne`), and
// the product is `data.SecondsToWaitBeforeRepeat * CMainGameComponent::GetConstantFPS()` (the integer is loaded first).
struct CCombatAbilityData_Calls {
    void* __vftable;
    long  PercentageChance;                                 // +0x4
    float SecondsDuration;                                  // +0x8
    float SecondsToWaitBeforeRepeat;                        // +0xc
};

class CMainGameComponent {
protected:
    static long ConstantFPS;                             // 0x01375550 ?ConstantFPS@CMainGameComponent@@1JA (protected static)
public:
    static long __fastcall GetConstantFPS() { return ConstantFPS; }   // Ego_r / FableWin ?GetConstantFPS@CMainGameComponent@@SIJXZ (inline)
};

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
            RepeatTimer = (long)(data.SecondsToWaitBeforeRepeat * CMainGameComponent::GetConstantFPS());
    }
}