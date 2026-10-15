// CTCCombat::GetComboMaxStage() -- the highest stage of the current combo chain over the parent creature's combat animation set (for_each with a CGetMaxComboStage functor); 0 when no chain is active or no animation belongs to it
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00733910, length 64 bytes.
// Callees: CThingCreatureBase::GetCombatAnimationSet @ 0x00663260 (returns the CAnimationSet, whose Anims
// CVectorMap {first, last} of pair<unsigned long, CAnimationEntry> sits at +0),
// std::for_each<const pair<unsigned long, CAnimationEntry>*, CGetMaxComboStage> @ 0x00738F30 (the SGI
// algorithm built /Gr: __fastcall, the hidden functor result in ecx, `first` in edx, `last` and the
// 8-byte functor on the stack).
// Layout = engine/CTCCombat.h: ComboChainID +0x10 (-999 = no chain), ParentCreature +0x24.
// Shape: the functor is {MaxStage, ChainID} with a user constructor (MaxStage starts at the -777
// sentinel), so it is returned through the hidden pointer into the 8-byte frame and passed by value as
// two pushed dwords; the no-chain test and the sentinel result share the `xor eax,eax` exit.
#include "engine/CTCCombat.h"

struct CAnimationEntryPair {                                // pair<unsigned long, CAnimationEntry>: 20 bytes
    unsigned long Key;
    unsigned char Entry[0x10];
};

struct CAnimationSet {
    const CAnimationEntryPair* First;                       // +0x00 Anims (CVectorMap)
    const CAnimationEntryPair* Last;                        // +0x04
};

struct CGetMaxComboStage {
    long MaxStage;                                          // +0x00
    long ChainID;                                           // +0x04
    CGetMaxComboStage(long chainID) : MaxStage(-777), ChainID(chainID) { }
};

CGetMaxComboStage __fastcall ForEachAnimation(const CAnimationEntryPair* first, const CAnimationEntryPair* last,
                                              CGetMaxComboStage f);   // 0x00738F30

struct CThingCreatureBase {
    const CAnimationSet* GetCombatAnimationSet();           // 0x00663260
};

struct CTCCombat_Methods : CTCCombat {
    long GetComboMaxStage();
};

long CTCCombat_Methods::GetComboMaxStage()
{
    if (ComboChainID != -999) {
        const CAnimationSet* set = ParentCreature->GetCombatAnimationSet();
        long stage = ForEachAnimation(set->First, set->Last, CGetMaxComboStage(ComboChainID)).MaxStage;
        if (stage != -777)
            return stage;
    }
    return 0;
}
