// CCombatSequence_TrollOpeningGroundPound::OnStart() -- schedule the next happening fifteen seconds of logic frames from now
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00907670, length 40 bytes (+ int3 padding).
// Callees: GFGetCurrentFrame @ 0x0049D870 (`__fastcall`, no arguments), the compiler's __ftol2 @ 0x00BFEA70.
// The frame is parked in the one-dword frame while the FPU forms `(float)g_logicFramesPerSecond * 15.0f`
// (`fild [g] ; fmul [__real]`), adds the frame as the right operand (`fiadd`: the frame is the second
// summand in the source, VC7.1 still evaluates the call first) and the sum is truncated to the long member at
// +0x28 (PDB WFNextHappen +0x2c; the base's 16-byte vector is 12 bytes in retail).
long __fastcall GFGetCurrentFrame();           // 0x0049D870
extern long g_logicFramesPerSecond;            // 0x01375550 (15)

struct CCombatSequence_TrollOpeningGroundPound_Retail {
    virtual ~CCombatSequence_TrollOpeningGroundPound_Retail();
    unsigned char _base[0x24];                 // +0x04 CCombatSequenceBase data
    long WFNextHappen;                         // +0x28 (PDB +0x2c)
    virtual void OnStart();
};

void CCombatSequence_TrollOpeningGroundPound_Retail::OnStart()
{
    long frame = GFGetCurrentFrame();
    WFNextHappen = g_logicFramesPerSecond * 15.0f + frame;
}