#pragma optimize("s",on)
// CTCHeroStats::GetNumberOfCurrentSpouses()
// Retail: Fable.exe (TLC, VC7.1) function @ 0x0057AD1D, oracle length 47 bytes.
// Grade: landed -- byte-exact (MATCH/RELOCATION_MATCH) and behaviour PASS under the Linux lane
//        (Wine + DDK 13.10.4035 cl.exe, 2026-10-13); the lander's winning pragma, if any, heads the file.
//
// Retail bytes:  mov edx,[ecx+1D8h] ; mov ecx,[edx] ; xor eax,eax ; cmp ecx,edx ; je R ; push esi ; L: mov esi,[ecx+18h] ; cmp esi,1 ; je I ; cmp esi,3 ; je I ; cmp esi,2 ; je I ; cmp esi,4 ; jne N ; I: inc eax ; N: mov ecx,[ecx] ; cmp ecx,edx ; jne L ; pop esi ; R: ret
// engine/CTCHeroStats.h (PDB): +0x1F0 std::list<CHeroMarriageInfo> Marriages; six std containers
// precede it, each 4 bytes smaller in retail, so the list head is at +0x1D8. Counts the marriages
// whose EMarriageState is one of the "current" states 1..4 (tested in the order 1, 3, 2, 4).
// Layout note: in the shipped binary a std::vector / CArray / map / set / CVectorMap is 4 bytes
// smaller than in Ego_r.pdb (the editor build carries an extra word per container) and a
// std::list is 8 bytes smaller (retail keeps only the head pointer; the PDB list is 0xC bytes).
// A retail std::vector is {first, last, end} with `first` at +0. Members after such containers
// therefore sit earlier than the generated engine header says: 4 bytes per vector/map/set/CArray
// and 8 bytes per list that precede them.

// PDB CHeroMarriageInfo: +0x00 u64 WifeUID, +0x08 u64 MaritalHomeUID, +0x10 EMarriageState State,
// +0x14 bool GayMarriage. std::list node = {next, prev, value}, so State is at node+0x18.
struct CHeroMarriageInfoNode {
    CHeroMarriageInfoNode* Next;            // +0x00
    CHeroMarriageInfoNode* Prev;            // +0x04
    unsigned __int64       WifeUID;         // +0x08
    unsigned __int64       MaritalHomeUID;  // +0x10
    long                   State;           // +0x18 EMarriageState
    bool                   GayMarriage;     // +0x1C
};

struct CTCHeroStats_Retail {
    unsigned char          _pad_0x000[0x1D8];
    CHeroMarriageInfoNode* Marriages_Head;  // +0x1D8 list sentinel (PDB list at +0x1F0)
    long GetNumberOfCurrentSpouses();
};

long CTCHeroStats_Retail::GetNumberOfCurrentSpouses()
{
    CHeroMarriageInfoNode* head = this->Marriages_Head;
    long count = 0;
    for (CHeroMarriageInfoNode* node = head->Next; node != head; node = node->Next) {
        long state = node->State;
        if (state == 1 || state == 3 || state == 2 || state == 4)
            ++count;
    }
    return count;
}