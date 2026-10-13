// CTCHeroStats::SetMoneyAsConfiscated(bool)
// Retail: Fable.exe (TLC, VC7.1) function @ 0x0057C999, oracle length 43 bytes.
// Grade: landed -- byte-exact (MATCH/RELOCATION_MATCH) and behaviour PASS under the Linux lane
//        (Wine + DDK 13.10.4035 cl.exe, 2026-10-13); the lander's winning pragma, if any, heads the file.
//
// Retail bytes:  push ebx ; xor ebx,ebx ; cmp [esp+8],bl ; push esi ; mov esi,ecx ; push ebx ; je G ; mov eax,[esi+3Ch] ; add [esi+44h],eax ; push ebx ; call CTCHeroStats::SetMoney ; jmp R ; G: push [esi+44h] ; call CTCHeroStats::AddMoney ; mov [esi+44h],ebx ; R: pop esi ; pop ebx ; ret 4
// Money +0x3C and ConfiscatedMoney +0x44 (PDB +0x44/+0x4C; see ForceRenownLevelTo for the shift).
// Confiscating moves the purse into ConfiscatedMoney and zeroes it via SetMoney(0, false)
// @ 0x0057A774; releasing gives it back via AddMoney(ConfiscatedMoney, false) @ 0x0057B327 and
// clears the confiscated amount. The second argument of both helpers is a "silent" flag.
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
    void SetMoney(long amount, bool silent);     // 0x0057A774
    void AddMoney(long amount, bool silent);     // 0x0057B327
    void SetMoneyAsConfiscated(bool confiscated);
};

void CTCHeroStats_Retail::SetMoneyAsConfiscated(bool confiscated)
{
    if (confiscated) {
        this->ConfiscatedMoney += this->Money;
        SetMoney(0, false);
    } else {
        AddMoney(this->ConfiscatedMoney, false);
        this->ConfiscatedMoney = 0;
    }
}
