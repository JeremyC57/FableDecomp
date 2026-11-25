// CTCSearchableContainer::Search(CThing&) -- counts NumberOfSearchesRemaining (+0x1c, PDB +0x20; persisted by OnSerialise 0x00812370) down, clamped at 0, and hands out the container's rewards when the count reaches 0
// Retry 10b: the clamp is a void `ClampRef(long&)` helper on the member followed by a fresh `NumberOfSearchesRemaining == 0` read: retail's decrement through edx, the `mov eax,edx` test copy and the post-clamp RE-READ of the member (`mov eax,[ecx+1Ch] ; test eax,eax`) into one shared `jne` + call (a reference-returning ClampToZero tested directly forwards the 0 and calls from the clamp arm).
// Leaf: ?Search@CTCSearchableContainer@@QAEXAAVCThing@@@Z
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00812340, length 35 bytes (+13 int3 padding); `ret 4` with the argument unused.
// Name: Ego_r.pdb 0x0080B650 + FableWin.pdb 0x025A7410 ?Search@CTCSearchableContainer@@QAEXAAVCThing@@@Z (the searcher by reference;
// NF33 2026-11-16, was Search(long)); bound so by 0x007EB6A0.
// Callee: CTCContainerRewardHero::RewardHeroWithItems @ 0x0074EE90 (manifest label; called on `this`). Callers 0x007EB7EF,
// 0x00844AA7. Label kept from the manifest (unverified).
class CThing;
class CTCSearchableContainer { public:
    unsigned char _pad[0x1c];
    long NumberOfSearchesRemaining;                 // +0x1c
    static void ClampRef(long& v) { if (v < 0) v = 0; }
    void Search(CThing& searcher);
};
// Naming (NF443 gen, 2026-11-25): callee-only rebind of this row's calls to their PDB spellings (T1 tracker; the target
//   self rows' Leaf / PDB name sets), through a bodiless declaration or the row's own model (no layout; rulings r2/r3, NF438 q1 friend):
//   0x0074EE90 ?RewardHeroWithItems@CTCContainerRewardHero@@QAE_NXZ (was ?FinishSearch@CTCSearchableContainer@@QAEXXZ)
class CTCContainerRewardHero { public: bool RewardHeroWithItems(); };
void CTCSearchableContainer::Search(CThing&)
{
    --NumberOfSearchesRemaining;
    ClampRef(NumberOfSearchesRemaining);
    if (NumberOfSearchesRemaining == 0)
        reinterpret_cast<CTCContainerRewardHero*>(this)->RewardHeroWithItems();
}