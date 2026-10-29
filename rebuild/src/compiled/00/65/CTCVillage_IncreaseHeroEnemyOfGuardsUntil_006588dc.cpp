#pragma optimize("s",on)
// CTCVillage::IncreaseHeroEnemyOfGuardsUntil(long)
// Retail: Fable.exe (TLC, VC7.1) function @ 0x006588DC, oracle length 47 bytes.
// Grade: landed -- byte-exact (MATCH/RELOCATION_MATCH) and behaviour PASS under the Linux lane
//        (Wine + DDK 13.10.4035 cl.exe, 2026-10-13); the lander's winning pragma, if any, heads the file.
//
// Retail bytes:  push ebp ; mov ebp,esp ; push ecx ; push esi ; mov esi,ecx ; call <world> ; call <current frame> ; lea ecx,[esi+0D4h] ; cmp eax,[ecx] ; mov [ebp-4],eax ; pop esi ; mov eax,ecx ; jl M ; lea eax,[ebp-4] ; M: mov eax,[eax] ; add eax,[ebp+8] ; mov [ecx],eax ; leave ; ret 4
// Extends the frame until which the guards treat the hero as an enemy (PDB +0x160, retail +0xD4):
// the deadline becomes max(now, deadline) + frames. The max is a reference-returning helper (the
// retail code selects a pointer to either the member or the stack copy of `now`), which is why
// the frame pointer and the stack local appear. The frame count is read through the world
// object (0x00686A70 fetches it, 0x0049D870 is the static frame accessor).
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
class CWorld;
class CTCBase
{
protected:
    CWorld& GetWorld() const;   // 0x00686A70
};

struct CTCVillage_Retail : public CTCBase {
    unsigned char          _pad_0x00[0x60];
    CVillageTaskListNode*  CrateTasks_Head;              // +0x60 list<CCountedPointer<CVillageTaskCarryCrate>> (PDB +0xD0)
    unsigned char          _pad_0x64[0x3C];
    long                   AmountOfFineAlreadyPaid;      // +0xA0 (PDB +0x12C)
    unsigned char          _pad_0xA4[0x30];
    long                   HeroEnemyOfGuardsUntil;       // +0xD4 (PDB +0x160)
    unsigned char          _pad_0xD8[0x0C];
    bool                   CurrentIsHeroCriminal;        // +0xE4 (PDB +0x170)
    void IncreaseHeroEnemyOfGuardsUntil(long frames);
};

class CWorld { public:
    static long __fastcall GetFrame();            // 0x0049D870
};

template<class T> inline const T& MaxOf(const T& a, const T& b) { return (a < b) ? b : a; }

void CTCVillage_Retail::IncreaseHeroEnemyOfGuardsUntil(long frames)
{
    long now = GetWorld().GetFrame();
    this->HeroEnemyOfGuardsUntil = MaxOf(now, this->HeroEnemyOfGuardsUntil) + frames;
}