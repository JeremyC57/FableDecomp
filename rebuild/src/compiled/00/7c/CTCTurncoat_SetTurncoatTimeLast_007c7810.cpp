// CTCTurncoat::SetTurncoatTimeLast(float)
// Retail: Fable.exe (TLC, VC7.1) function @ 0x007C7810, oracle length 43 bytes.
// Grade: landed -- byte-exact (MATCH/RELOCATION_MATCH) and behaviour PASS under the Linux lane
//        (Wine + DDK 13.10.4035 cl.exe, 2026-10-13); the lander's winning pragma, if any, heads the file.
//
// Retail bytes:  fild [g_FramesPerSecond] ; push esi ; mov esi,ecx ; fmul [esp+8] ; call _ftol ; fld [esi+10h] ; fcomp [esi+48h] ; mov ecx,eax ; mov [esi+20h],ecx ; fnstsw ax ; test ah,5 ; jp S ; mov [esi+0Ch],ecx ; S: pop esi ; ret 4
// engine/CTCTurncoat.h (PDB): +0x0C WillTurncoatTimer, +0x10 CurrentTurncoatedness, +0x20
// TurncoatTimeLast; CreatureTurncoatResistance is +0x48 in retail (PDB +0x4C). Converts the
// duration in seconds to frames with the global frame rate (long at 0x01375550, value 15) and the
// CRT _ftol, records it, and if the creature is already past its resistance threshold also arms
// the countdown to turning coat.
// Layout note: in the shipped binary a std::vector / CArray / map / set / CVectorMap is 4 bytes
// smaller than in Ego_r.pdb (the editor build carries an extra word per container) and a
// std::list is 8 bytes smaller (retail keeps only the head pointer; the PDB list is 0xC bytes).
// A retail std::vector is {first, last, end} with `first` at +0. Members after such containers
// therefore sit earlier than the generated engine header says: 4 bytes per vector/map/set/CArray
// and 8 bytes per list that precede them.
//
// Lane feedback: the comparison is `<` (retail `test ah,5 ; jp`), not `>=`; the first draft had the sense
// inverted. RELOCATION_MATCH.
extern long g_FramesPerSecond;      // 0x01375550 (.data), 15

// Retail CTCTurncoat view (CreatureTurncoatResistance is PDB +0x4C; see the README container rule).
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
    // Seconds -> frames via the global frame rate and the CRT _ftol; remember it, and while the
    // creature is still BELOW its resistance threshold also arm the countdown to turning coat
    // (`test ah,5 ; jp` is the compiler's ordered `<`; `>=` emits `test ah,1 ; jne`).
    long frames = (long)(g_FramesPerSecond * seconds);
    this->TurncoatTimeLast = frames;
    if (this->CurrentTurncoatedness < this->CreatureTurncoatResistance)
        this->WillTurncoatTimer = frames;
}