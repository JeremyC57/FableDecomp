// CCombatSequenceGeneric::GetFramesToPredictTargetPosition()
// Retail: Fable.exe (TLC, VC7.1) function @ 0x008B06E0, oracle length 17 bytes.
// Grade: landed -- byte-exact (MATCH/RELOCATION_MATCH) and behaviour PASS under the Linux lane
//        (Wine + DDK 13.10.4035 cl.exe, 2026-10-13); the lander's winning pragma, if any, heads the file.
//
// Retail bytes:  fild dword ptr [01375550h] ; mov eax,[ecx+28h] ; fmul dword ptr [eax+60h] ; jmp _ftol
// engine/CCombatSequenceGeneric.h: +0x28 is the def pointer in retail (PDB +0x2C). Seconds from the
// def (+0x60) times the global frame rate, truncated to frames through the CRT _ftol (tail jump).
extern long g_FramesPerSecond;              // 0x01375550 (.data)

struct CCombatSequenceDefCalls {
    unsigned char _pad_0x00[0x60];
    float         SecondsToPredictTargetPosition;   // +0x60 (name from use)
};

struct CCombatSequenceGeneric_Retail {
    unsigned char            _pad_0x00[0x28];
    CCombatSequenceDefCalls* Def;           // +0x28 (PDB +0x2C)
    long GetFramesToPredictTargetPosition();
};

long CCombatSequenceGeneric_Retail::GetFramesToPredictTargetPosition()
{
    return (long)(g_FramesPerSecond * this->Def->SecondsToPredictTargetPosition);
}
