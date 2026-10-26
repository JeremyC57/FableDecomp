// CTCSearchableContainer::Search(long) -- counts NumberOfSearchesRemaining (+0x1c, PDB +0x20; persisted by OnSerialise 0x00812370) down, clamped at 0, and hands out the container's rewards when the count reaches 0
// Retry 10b: the clamp is a void `ClampRef(long&)` helper on the member followed by a fresh `NumberOfSearchesRemaining == 0` read: retail's decrement through edx, the `mov eax,edx` test copy and the post-clamp RE-READ of the member (`mov eax,[ecx+1Ch] ; test eax,eax`) into one shared `jne` + call (a reference-returning ClampToZero tested directly forwards the 0 and calls from the clamp arm).
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00812340, length 35 bytes (+13 int3 padding); `ret 4` with the argument unused.
// Callee: CTCContainerRewardHero::RewardHeroWithItems @ 0x0074EE90 (manifest label; called on `this`). Callers 0x007EB7EF,
// 0x00844AA7. Label kept from the manifest (unverified).
class CTCSearchableContainer { public:
    unsigned char _pad[0x1c];
    long NumberOfSearchesRemaining;                 // +0x1c
    void FinishSearch();                     // 0x0074EE90
    static void ClampRef(long& v) { if (v < 0) v = 0; }
    void Search(long frames);
};
void CTCSearchableContainer::Search(long frames)
{
    --NumberOfSearchesRemaining;
    ClampRef(NumberOfSearchesRemaining);
    if (NumberOfSearchesRemaining == 0)
        FinishSearch();
}