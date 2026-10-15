// CTCHeroStats::GetHeroSexuality() const -- classifies the hero from scripted sex and current marriages: 0 none, 1 straight, 2 bisexual, 3 gay
// Retail: Fable.exe (TLC, VC7.1) function @ 0x0057ACC4, 89 bytes.
//
// Shape: two byte flags seeded from HadScriptedSex (+0x158, al) and HadScriptedGaySex (+0x159, bl); the marriage list
// walk (`jmp` to the bottom test) checks the state against 1, 3, 2, 4 in that order (an `||` chain, not a switch) and
// sets the gay flag for a gay marriage, the straight flag otherwise. The result is two ternaries on the straight flag:
// without the gay flag `straight ? 1 : 0` (`xor ecx,ecx ; test al,al ; setne cl`), with it `straight ? 2 : 3`
// (`neg al ; sbb eax,eax ; add eax,3`); the no-gay case is tested first (`if (!gay)`: `if (gay)` swaps the blocks).
// Layout: retail CTCHeroStats sits 0x14 below the PDB here: HadScriptedSex +0x158 (PDB +0x16C), HadScriptedGaySex +0x159
// (PDB +0x16D), Marriages +0x1D8 (PDB +0x1F0); marriage node: State +0x18, GayMarriage +0x1C.
#pragma optimize("s", on)

struct CHeroMarriageInfoNode {                 // std::list<CHeroMarriageInfo> node
    CHeroMarriageInfoNode* Next;               // +0x00
    CHeroMarriageInfoNode* Prev;               // +0x04
    unsigned __int64       WifeUID;            // +0x08
    unsigned __int64       MaritalHomeUID;     // +0x10
    long                   State;              // +0x18 EMarriageState
    bool                   GayMarriage;        // +0x1C
};

struct CTCHeroStats_Retail {
    unsigned char          _pad_0x00[0x158];
    bool                   HadScriptedSex;     // +0x158 (PDB +0x16C)
    bool                   HadScriptedGaySex;  // +0x159 (PDB +0x16D)
    unsigned char          _pad_0x15a[0x7E];
    CHeroMarriageInfoNode* Marriages_Head;     // +0x1D8 (PDB +0x1F0)

    long GetHeroSexuality() const;
};

long CTCHeroStats_Retail::GetHeroSexuality() const
{
    bool straight = HadScriptedSex;
    bool gay = HadScriptedGaySex;
    CHeroMarriageInfoNode* head = Marriages_Head;
    for (CHeroMarriageInfoNode* node = head->Next; node != head; node = node->Next) {
        long state = node->State;
        if (state == 1 || state == 3 || state == 2 || state == 4) {
            if (node->GayMarriage)
                gay = true;
            else
                straight = true;
        }
    }
    if (!gay)
        return straight ? 1 : 0;
    return straight ? 2 : 3;
}
