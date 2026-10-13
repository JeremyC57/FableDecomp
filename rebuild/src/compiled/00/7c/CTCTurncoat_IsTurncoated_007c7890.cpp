// CTCTurncoat::IsTurncoated()
// Retail: Fable.exe (TLC, VC7.1) function @ 0x007C7890, oracle length 39 bytes.
// Grade: landed -- byte-exact (MATCH/RELOCATION_MATCH) and behaviour PASS under the Linux lane
//        (Wine + DDK 13.10.4035 cl.exe, 2026-10-13); the lander's winning pragma, if any, heads the file.
//
// Retail bytes:  mov al,[ecx+2Ch] ; test al,al ; je F ; mov eax,[ecx+14h] ; mov dl,[eax+25h] ; test dl,dl ; je F ; fld [ecx+10h] ; fcomp [ecx+48h] ; fnstsw ax ; test ah,1 ; jne F ; mov eax,1 ; ret ; F: xor eax,eax ; ret
// engine/CTCTurncoat.h (PDB): +0x10 CurrentTurncoatedness, +0x14 PDef_Object, +0x2C
// TurncoatOverride; CreatureTurncoatResistance is at +0x48 in retail (PDB +0x4C: the
// TurncoatStageEffects vector before it is 4 bytes smaller in the shipped build). A creature is
// turncoated when the override flag is on, its def allows it (+0x25) and its turncoatedness has
// reached its resistance. The result is a 32-bit 1/0.
// Layout note: in the shipped binary a std::vector / CArray / map / set / CVectorMap is 4 bytes
// smaller than in Ego_r.pdb (the editor build carries an extra word per container) and a
// std::list is 8 bytes smaller (retail keeps only the head pointer; the PDB list is 0xC bytes).
// A retail std::vector is {first, last, end} with `first` at +0. Members after such containers
// therefore sit earlier than the generated engine header says: 4 bytes per vector/map/set/CArray
// and 8 bytes per list that precede them.

struct CTurncoatDefCalls {
    unsigned char _pad_0x00[0x25];
    bool          CanBeTurncoated;      // +0x25 (name inferred)
};

struct CTCTurncoat_Retail {
    unsigned char     _pad_0x00[0x10];
    float             CurrentTurncoatedness;        // +0x10 (PDB)
    CTurncoatDefCalls* PDef_Object;                 // +0x14 (PDB)
    unsigned char     _pad_0x18[0x14];
    bool              TurncoatOverride;             // +0x2C (PDB)
    unsigned char     _pad_0x2D[0x1B];
    float             CreatureTurncoatResistance;   // +0x48 (PDB +0x4C)
    long IsTurncoated();
};

long CTCTurncoat_Retail::IsTurncoated()
{
    if (this->TurncoatOverride && this->PDef_Object->CanBeTurncoated &&
        this->CurrentTurncoatedness >= this->CreatureTurncoatResistance)
        return 1;
    return 0;
}