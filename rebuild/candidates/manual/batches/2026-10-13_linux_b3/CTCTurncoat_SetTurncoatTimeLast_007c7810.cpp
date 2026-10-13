// CTCTurncoat::SetTurncoatTimeLast(float)
// Retail: Fable.exe (TLC, VC7.1) function @ 0x007C7810, oracle length 43 bytes.
// Grade: candidate (authored from the retail bytes; byte parity NOT yet proven -- run
//        tools/decomp_pipeline/verify_and_land.py on this batch's payload to grade it).
//
// Retail bytes:  fild [g_FramesPerSecond] ; push esi ; mov esi,ecx ; fmul [esp+8] ; call _ftol ; fld [esi+10h] ; fcomp [esi+48h] ; mov ecx,eax ; mov [esi+20h],ecx ; fnstsw ax ; test ah,5 ; jp S ; mov [esi+0Ch],ecx ; S: pop esi ; ret 4
// engine/CTCTurncoat.h (PDB): +0x0C WillTurncoatTimer, +0x10 CurrentTurncoatedness, +0x20
// TurncoatTimeLast; CreatureTurncoatResistance is +0x48 in retail (PDB +0x4C). Converts the
// duration in seconds to frames with the global frame rate (long at 0x01375550, value 15) and the
// CRT _ftol, records it, and if the creature is already past its resistance threshold also arms
// the countdown to turning coat.
// Layout note: in the shipped binary std::vector / CArray / map / CVectorMap objects are 4 bytes
// smaller than in Ego_r.pdb (the editor build carries an extra word per container), and a retail
// std::vector is {first, last, end} with `first` at +0. Members after such containers therefore
// sit 4 bytes per preceding container earlier than the generated engine header says.

extern long g_FramesPerSecond;      // 0x01375550 (.data), 15

struct CTCTurncoat_Retail {
    unsigned char _pad_0x00[0x0C];
    long          WillTurncoatTimer;            // +0x0C (PDB)
    float         CurrentTurncoatedness;        // +0x10 (PDB)
    unsigned char _pad_0x14[0x0C];
    long          TurncoatTimeLast;             // +0x20 (PDB)
    unsigned char _pad_0x24[0x24];
    float         CreatureTurncoatResistance;   // +0x48 (PDB +0x4C)
    void SetTurncoatTimeLast(float seconds);
};

void CTCTurncoat_Retail::SetTurncoatTimeLast(float seconds)
{
    long frames = (long)(g_FramesPerSecond * seconds);
    this->TurncoatTimeLast = frames;
    if (this->CurrentTurncoatedness >= this->CreatureTurncoatResistance)
        this->WillTurncoatTimer = frames;
}
