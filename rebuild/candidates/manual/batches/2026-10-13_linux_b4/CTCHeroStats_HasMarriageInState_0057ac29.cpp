// CTCHeroStats::HasMarriageInState(EMarriageState)
// Retail: Fable.exe (TLC, VC7.1) function @ 0x0057AC29, oracle length 34 bytes.
// Grade: landed -- byte-exact (MATCH/RELOCATION_MATCH) and behaviour PASS under the Linux lane
//        (Wine + DDK 13.10.4035 cl.exe, 2026-10-13); the lander's winning pragma, if any, heads the file.
//
// Retail bytes:  mov ecx,[ecx+1D8h] ; mov eax,[ecx] ; jmp C ; L: mov edx,[eax+18h] ; cmp edx,[esp+4] ; je T ; mov eax,[eax] ; C: cmp eax,ecx ; jne L ; xor al,al ; ret 4 ; T: mov al,1 ; jmp <ret 4>
// Same retail list as GetNumberOfCurrentSpouses (+0x1D8, PDB +0x1F0). True when any marriage
// record is in the requested EMarriageState.
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
    bool HasMarriageInState(long state);
};

bool CTCHeroStats_Retail::HasMarriageInState(long state)
{
    CHeroMarriageInfoNode* head = this->Marriages_Head;
    for (CHeroMarriageInfoNode* node = head->Next; node != head; node = node->Next) {
        if (node->State == state)
            return true;
    }
    return false;
}
