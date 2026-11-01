// CCombatSequence_TrollOpeningGroundPound::OnStart() -- schedule the next happening fifteen seconds of logic frames from now
// Naming wave 18: the global stand-in g_logicFramesPerSecond (0x01375550) is the protected static long CMainGameComponent::ConstantFPS, read through the
//   inline static accessor GetConstantFPS() as retail code outside the class does (name-only: the bytes are unchanged).
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00907670, length 40 bytes (+ int3 padding).
// Callees: GFGetCurrentFrame @ 0x0049D870 (`__fastcall`, no arguments), the compiler's __ftol2 @ 0x00BFEA70.
// The frame is parked in the one-dword frame while the FPU forms `(float)CMainGameComponent::GetConstantFPS() * 15.0f`
// (`fild [g] ; fmul [__real]`), adds the frame as the right operand (`fiadd`: the frame is the second
// summand in the source, VC7.1 still evaluates the call first) and the sum is truncated to the long member at
// +0x28 (PDB WFNextHappen +0x2c; the base's 16-byte vector is 12 bytes in retail).
class CWorld {
public:
    static long __fastcall GetFrame();   // 0x0049D870
};           // 0x0049D870
class CMainGameComponent {
protected:
    static long ConstantFPS;                             // 0x01375550 ?ConstantFPS@CMainGameComponent@@1JA (protected static)
public:
    static long __fastcall GetConstantFPS() { return ConstantFPS; }   // Ego_r / FableWin ?GetConstantFPS@CMainGameComponent@@SIJXZ (inline)
};

struct CCombatSequence_TrollOpeningGroundPound_Retail {
    virtual ~CCombatSequence_TrollOpeningGroundPound_Retail();
    unsigned char _base[0x24];                 // +0x04 CCombatSequenceBase data
    long WFNextHappen;                         // +0x28 (PDB +0x2c)
    virtual void OnStart();
};

void CCombatSequence_TrollOpeningGroundPound_Retail::OnStart()
{
    long frame = CWorld::GetFrame();
    WFNextHappen = CMainGameComponent::GetConstantFPS() * 15.0f + frame;
}