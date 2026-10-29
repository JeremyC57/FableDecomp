#pragma optimize("s",on)
// CTCVillage::IsHeroCriminal()
// Retail: Fable.exe (TLC, VC7.1) function @ 0x0065BFCE, oracle length 44 bytes.
// Grade: landed -- byte-exact (MATCH/RELOCATION_MATCH) and behaviour PASS under the Linux lane
//        (Wine + DDK 13.10.4035 cl.exe, 2026-10-13); the lander's winning pragma, if any, heads the file.
//
// Retail bytes:  push ebx ; push esi ; mov esi,ecx ; call CTCVillage::GetFineAmountForCurrentCrimes ; test eax,eax ; setg bl ; add esi,0E4h ; cmp [esi],bl ; je S ; mov ecx,[g_pPlayerGui] ; push 0 ; call <notify> ; mov [esi],bl ; S: mov al,[esi] ; pop esi ; pop ebx ; ret
// The hero is a criminal while a fine is outstanding. CurrentIsHeroCriminal (PDB +0x170, retail
// +0xE4) caches the answer; when it changes, the global observer object at 0x013B8790 is told
// (0x004378C0, __fastcall, one argument, called with 0; its manifest label is a propagated name)
// before the cache is updated. Returns the cached flag.
// Layout note: in the shipped binary a std::vector / CArray / map / set / CVectorMap is 4 bytes
// smaller than in Ego_r.pdb (the editor build carries an extra word per container) and a
// std::list is 8 bytes smaller (retail keeps only the head pointer; the PDB list is 0xC bytes).
// A retail std::vector is {first, last, end} with `first` at +0. Members after such containers
// therefore sit earlier than the generated engine header says: 4 bytes per vector/map/set/CArray
// and 8 bytes per list that precede them.

// Retail CTCVillage: the 15 task/thing lists from +0x28 are 4 bytes each (PDB 0xC), the PlayersCrimes
// set is 0xC (PDB 0x10) and the two lists after it are 4 bytes each, so everything from
// Initialised (PDB +0x104) on is 0x8C bytes earlier than engine/CTCVillage.h says.
struct CVillageTaskListNode;
class CTCVillage { public:
    unsigned char          _pad_0x00[0x60];
    CVillageTaskListNode*  CrateTasks_Head;              // +0x60 list<CCountedPointer<CVillageTaskCarryCrate>> (PDB +0xD0)
    unsigned char          _pad_0x64[0x3C];
    long                   AmountOfFineAlreadyPaid;      // +0xA0 (PDB +0x12C)
    unsigned char          _pad_0xA4[0x30];
    long                   HeroEnemyOfGuardsUntil;       // +0xD4 (PDB +0x160)
    unsigned char          _pad_0xD8[0x0C];
    bool                   CurrentIsHeroCriminal;        // +0xE4 (PDB +0x170)
    long GetFineAmountForCurrentCrimes();     // 0x0065A41C
    bool IsHeroCriminal();
};

class CPlayerGui { public:
    void UpdateMiniMapMarkers(bool reason);           // 0x004378C0
};

extern CPlayerGui* g_pPlayerGui;                                  // 0x013B8790

bool CTCVillage::IsHeroCriminal()
{
    bool criminal = GetFineAmountForCurrentCrimes() > 0;
    if (this->CurrentIsHeroCriminal != criminal) {
        g_pPlayerGui->UpdateMiniMapMarkers(false);
        this->CurrentIsHeroCriminal = criminal;
    }
    return this->CurrentIsHeroCriminal;
}