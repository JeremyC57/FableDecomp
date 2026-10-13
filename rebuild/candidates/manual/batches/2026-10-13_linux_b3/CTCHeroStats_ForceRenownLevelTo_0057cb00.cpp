// CTCHeroStats::ForceRenownLevelTo(long)
// Retail: Fable.exe (TLC, VC7.1) function @ 0x0057CB00, oracle length 42 bytes.
// Grade: candidate (authored from the retail bytes; byte parity NOT yet proven -- run
//        tools/decomp_pipeline/verify_and_land.py on this batch's payload to grade it).
//
// Retail bytes:  mov eax,[esp+4] ; mov [ecx+70h],eax ; mov edx,[ecx+70h] ; test edx,edx ; mov eax,[ecx+74h] ; jge A ; and dword ptr [ecx+70h],0 ; jmp B ; A: cmp edx,eax ; jle B ; mov [ecx+70h],eax ; B: and dword ptr [ecx+78h],0 ; call CTCHeroStats::CheckForNewExpressions ; ret 4
// Retail CTCHeroStats is 8 bytes smaller than Ego_r.pdb from the money block on (two 4-byte
// shorter containers before it), so RenownLevel/RenownMaxLevel/RenownPointsInLevel are at
// +0x70/+0x74/+0x78. Clamps the forced level into [0, RenownMaxLevel], resets the points earned
// inside the level and re-checks which expressions the new renown unlocks (0x0057BE71).
// Layout note: in the shipped binary a std::vector / CArray / map / set / CVectorMap is 4 bytes
// smaller than in Ego_r.pdb (the editor build carries an extra word per container) and a
// std::list is 8 bytes smaller (retail keeps only the head pointer; the PDB list is 0xC bytes).
// A retail std::vector is {first, last, end} with `first` at +0. Members after such containers
// therefore sit earlier than the generated engine header says: 4 bytes per vector/map/set/CArray
// and 8 bytes per list that precede them.

struct CTCHeroStats_Retail {
    unsigned char _pad_0x00[0x3C];
    long          Money;                  // +0x3C (PDB +0x44)
    unsigned char _pad_0x40[4];
    long          ConfiscatedMoney;       // +0x44 (PDB +0x4C)
    unsigned char _pad_0x48[0x28];
    long          RenownLevel;            // +0x70 (PDB +0x78)
    long          RenownMaxLevel;         // +0x74 (PDB +0x7C)
    long          RenownPointsInLevel;    // +0x78 (PDB +0x80)
    void CheckForNewExpressions();        // 0x0057BE71
    void ForceRenownLevelTo(long level);
};

void CTCHeroStats_Retail::ForceRenownLevelTo(long level)
{
    this->RenownLevel = level;
    if (this->RenownLevel < 0)
        this->RenownLevel = 0;
    else if (this->RenownLevel > this->RenownMaxLevel)
        this->RenownLevel = this->RenownMaxLevel;
    this->RenownPointsInLevel = 0;
    CheckForNewExpressions();
}
