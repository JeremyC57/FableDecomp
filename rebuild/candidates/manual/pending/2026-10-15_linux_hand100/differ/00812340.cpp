// CTCSearchableContainer::Search(long) -- counts NumberOfSearchesRemaining (+0x1c, PDB +0x20; persisted by OnSerialise 0x00812370) down, clamped at 0, and hands out the container's rewards when the count reaches 0
// Grade: DIFFER(36v35) -- retail decrements through edx (`mov edx,[ecx+1Ch] ; dec edx ; mov eax,edx ; test eax,eax ; mov [ecx+1Ch],edx`) and after the clamp store RE-READS the member (`mov eax,[ecx+1Ch] ; test eax,eax`) into one shared `jne` + call; ours keeps eax and, knowing the clamped value is 0, calls directly from the clamp arm (separate `ret 4`). Tried: plain --/clamp/test (27: `dec [mem] ; jns`), `long& ClampToZero(long&)` helper (36, best), `long left` local with an assignment expression, ternary lvalue, reference local, `m = Max(m, 0L)` / `m = Max(m - 1, 0L)` (Dinkumware const-ref max), by-value ClampToZero.
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00812340, length 35 bytes (+13 int3 padding); `ret 4` with the argument unused.
// Callee: CTCContainerRewardHero::RewardHeroWithItems @ 0x0074EE90 (manifest label; called on `this`). Callers 0x007EB7EF,
// 0x00844AA7. Label kept from the manifest (unverified).
struct CTCSearchableContainer {
    unsigned char _pad[0x1c];
    long NumberOfSearchesRemaining;                 // +0x1c
    void FinishSearch();                     // 0x0074EE90
    static long& ClampToZero(long& v) { if (v < 0) v = 0; return v; }
    static const long& Max(const long& a, const long& b) { return a < b ? b : a; }
    void Search(long frames);
};
void CTCSearchableContainer::Search(long frames)
{
    if (ClampToZero(--NumberOfSearchesRemaining) == 0)
        FinishSearch();
}
