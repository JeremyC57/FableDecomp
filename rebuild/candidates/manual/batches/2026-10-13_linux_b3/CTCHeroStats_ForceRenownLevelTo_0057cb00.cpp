// CTCHeroStats::ForceRenownLevelTo(long)
// Retail: Fable.exe (TLC, VC7.1) function @ 0x0057CB00, oracle length 42 bytes.
// Grade: landed -- byte-exact (MATCH/RELOCATION_MATCH) and behaviour PASS under the Linux lane
//        (Wine + DDK 13.10.4035 cl.exe, 2026-10-13); the lander's winning pragma, if any, heads the file.
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
//
// Lane feedback: byte-exact as an inlined `Clamp(long&, lo, hi)` helper under #pragma optimize("s",on)
// (`and dword ptr [..],0` zero stores, shared tail); the lander sweep applies the pragma. RELOCATION_MATCH["s"].
// Retail CTCHeroStats view (PDB offsets +8; see the batch README container-size rule).
struct CTCHeroStats_Retail {
    unsigned char _pad_0x00[0x70];
    long          RenownLevel;            // +0x70 (PDB +0x78)
    long          RenownMaxLevel;         // +0x74 (PDB +0x7C)
    long          RenownPointsInLevel;    // +0x78 (PDB +0x80)
    void CheckForNewExpressions();        // 0x0057BE71
    void ForceRenownLevelTo(long level);
};

// Shared inline clamp (reference parameter: the compiler re-reads the member after the store,
// exactly as the retail bytes do).
static inline void Clamp(long& value, long minimum, long maximum)
{
    if (value < minimum)
        value = minimum;
    else if (value > maximum)
        value = maximum;
}

void CTCHeroStats_Retail::ForceRenownLevelTo(long level)
{
    // Force the level, keep it inside [0, RenownMaxLevel], restart the points earned in the
    // level and re-check which expressions the new renown unlocks.
    this->RenownLevel = level;
    Clamp(this->RenownLevel, 0, this->RenownMaxLevel);
    this->RenownPointsInLevel = 0;
    CheckForNewExpressions();
}
