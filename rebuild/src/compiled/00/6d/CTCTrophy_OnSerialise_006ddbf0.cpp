// CTCTrophy::OnSerialise(CPersistContext&, bool) -- persists BestWitnessesAheadToDate (default 0) and Mountable (default false)
// Leaf: OnSerialise
// Retail: Fable.exe (TLC, VC7.1) function @ 0x006DDBF0, length 70 bytes; the catalog's "vfunc_1" is the TC persist hook
// slot (as the landed CTCChest::Persist 0x00750010); the name and signature are the PDB's
// `?OnSerialise@CTCTrophy@@UAEXAAVCPersistContext@@_N@Z`.
// Callees: CPersistContext::Transfer<long>(name, value, default) @ 0x00410BE0, CPersistContext::Transfer<bool>
// @ 0x004045C0. The two keys are string literals (0x0125FCD0, 0x0125FCC4) named after the members.
// Shape: the long default is a named local in the `push ecx` frame word; the bool default is the literal `false`
// bound to the const reference, homed in the dead context-argument slot (`mov byte ptr [esp+1Ch],0`); the second
// member address is formed by advancing `this` (`add esi,48h`). The bool parameter is unused.
// Layout: Ego_r.pdb CTCTrophy with the retail one-pointer lists: BestWitnessesAheadToDate +0x18, Mountable +0x48.
struct CPersistContext {
    void Transfer(const char* name, long& value, const long& defaultValue);   // 0x00410BE0
    void Transfer(const char* name, bool& value, const bool& defaultValue);   // 0x004045C0
};

struct CTCTrophy {
    unsigned char _pad_0x04[0x14];          // +0x04 CTCBase, PDef, NonMinigameDeedHandle, MinigameInProgress
    long          BestWitnessesAheadToDate; // +0x18
    unsigned char _pad_0x1c[0x2C];          // +0x1C minigame counters, witness lists, flags, page
    bool          Mountable;                // +0x48
    virtual void v00();
    virtual void OnSerialise(CPersistContext& context, bool flag);
};

void CTCTrophy::OnSerialise(CPersistContext& context, bool flag)
{
    long defaultBest = 0;
    context.Transfer("BestWitnessesAheadToDate", BestWitnessesAheadToDate, defaultBest);
    context.Transfer("Mountable", Mountable, false);
}