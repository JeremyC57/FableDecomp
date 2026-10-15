// CTCHeroStats::PeekMarriageInfo(unsigned __int64, CHeroMarriageInfo*) const -- copies out the marriage record for this wife, if there is one
// Retail: Fable.exe (TLC, VC7.1) function @ 0x0057AC89, 59 bytes.
//
// Shape: HasMarriageInState's (0x0057AC29, landed) list walk -- `jmp` into the bottom test, a 64-bit WifeUID compare
// against the by-value key's two argument words -- with the hit block placed after the `xor al,al` exit: the 24-byte
// POD record is copied with `push 6 ; pop ecx ; rep movsd` (size TU) and the block jumps back to the shared `ret 0Ch`.
// Layout: Marriages +0x1D8 in retail (PDB +0x1F0); CHeroMarriageInfo from the PDB (0x18 bytes: WifeUID, MaritalHomeUID,
// State, GayMarriage), the node's value at +0x08.
#pragma optimize("s", on)

struct CHeroMarriageInfo {
    unsigned __int64 WifeUID;                  // +0x00
    unsigned __int64 MaritalHomeUID;           // +0x08
    long             State;                    // +0x10 EMarriageState
    bool             GayMarriage;              // +0x14
};

struct CHeroMarriageInfoNode {                 // std::list<CHeroMarriageInfo> node
    CHeroMarriageInfoNode* Next;               // +0x00
    CHeroMarriageInfoNode* Prev;               // +0x04
    CHeroMarriageInfo      Info;               // +0x08
};

struct CTCHeroStats_Retail {
    unsigned char          _pad_0x00[0x1D8];
    CHeroMarriageInfoNode* Marriages_Head;     // +0x1D8 (PDB +0x1F0)

    bool PeekMarriageInfo(unsigned __int64 wifeUID, CHeroMarriageInfo* info) const;
};

bool CTCHeroStats_Retail::PeekMarriageInfo(unsigned __int64 wifeUID, CHeroMarriageInfo* info) const
{
    CHeroMarriageInfoNode* head = Marriages_Head;
    for (CHeroMarriageInfoNode* node = head->Next; node != head; node = node->Next) {
        if (node->Info.WifeUID == wifeUID) {
            *info = node->Info;
            return true;
        }
    }
    return false;
}
