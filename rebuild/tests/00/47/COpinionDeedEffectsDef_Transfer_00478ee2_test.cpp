// Behaviour test for COpinionDeedEffectsDef::Transfer (0x00478EE2): a recording persist context checks the transfer
// order and that every flag bit is read out and written back (saving keeps the bits, loading overwrites them).
#include <cstdio>
#include <cstring>
struct Ctx {
    int n; char log[64][40]; bool loading; int loadIndex;
    Ctx() : n(0), loading(false), loadIndex(0) {}
    void rec(const char* s) { strcpy(log[n++], s); }
    void Transfer(int& v) { (void)v; rec("long"); }
    void TransferF(float& v) { (void)v; rec("float"); }
    void Transfer(const char* name, bool& value, const bool& def)
    {
        rec(name);
        if (def) rec("BADDEFAULT");
        if (loading) { value = (loadIndex % 2) == 0; loadIndex++; }
    }
};
struct Def {
    int Knowledge, Postings, Effects2; float Delay; int EffectsList, Priority, Reaction, Layer, Warning, Severity, Fine, Tutorial;
    bool CanBeHeard : 1; bool ReactionRestrictToTarget : 1; bool AllowSimultaneousReactionDialogue : 1; bool CanApologise : 1;
    bool MustNotRunWhenWarned : 1; bool OnlyGetFinedForOneInstance : 1; bool IsFlirting : 1; bool OnReactSetGreeted : 1;
    bool AllowSpeechOnNonPureAISpeaker : 1; bool AllowIndirectReactWhileCarrying : 1; bool BlockWhileFollowingPlayer : 1;
    void Transfer(Ctx& c)
    {
        c.Transfer(Knowledge); c.Transfer(Postings); c.Transfer(Effects2); c.TransferF(Delay); c.Transfer(EffectsList);
        c.Transfer(Priority); c.Transfer(Reaction); c.Transfer(Layer); c.Transfer(Warning); c.Transfer(Severity);
        c.Transfer(Fine); c.Transfer(Tutorial);
        { bool flag = CanBeHeard; c.Transfer("CanBeHeard", flag, false); CanBeHeard = flag; }
        { bool flag = ReactionRestrictToTarget; c.Transfer("ReactionRestrictToTarget", flag, false); ReactionRestrictToTarget = flag; }
        { bool flag = AllowSimultaneousReactionDialogue; c.Transfer("AllowSimultaneousReactionDialogue", flag, false); AllowSimultaneousReactionDialogue = flag; }
        { bool flag = CanApologise; c.Transfer("CanApologise", flag, false); CanApologise = flag; }
        { bool flag = MustNotRunWhenWarned; c.Transfer("MustNotRunWhenWarned", flag, false); MustNotRunWhenWarned = flag; }
        { bool flag = OnlyGetFinedForOneInstance; c.Transfer("OnlyGetFinedForOneInstance", flag, false); OnlyGetFinedForOneInstance = flag; }
        { bool flag = IsFlirting; c.Transfer("IsFlirting", flag, false); IsFlirting = flag; }
        { bool flag = OnReactSetGreeted; c.Transfer("OnReactSetGreeted", flag, false); OnReactSetGreeted = flag; }
        { bool flag = AllowSpeechOnNonPureAISpeaker; c.Transfer("AllowSpeechOnNonPureAISpeaker", flag, false); AllowSpeechOnNonPureAISpeaker = flag; }
        { bool flag = AllowIndirectReactWhileCarrying; c.Transfer("AllowIndirectReactWhileCarrying", flag, false); AllowIndirectReactWhileCarrying = flag; }
        { bool flag = BlockWhileFollowingPlayer; c.Transfer("BlockWhileFollowingPlayer", flag, false); BlockWhileFollowingPlayer = flag; }
    }
};
int main()
{
    int fails = 0;
    Def d;
    memset(&d, 0, sizeof(d));
    d.CanBeHeard = true; d.IsFlirting = true; d.OnReactSetGreeted = true; d.BlockWhileFollowingPlayer = true;
    Ctx save;
    d.Transfer(save);
    if (save.n != 23) fails++;
    if (strcmp(save.log[3], "float") != 0) fails++;
    if (strcmp(save.log[12], "CanBeHeard") != 0) fails++;
    if (strcmp(save.log[19], "OnReactSetGreeted") != 0) fails++;
    if (strcmp(save.log[22], "BlockWhileFollowingPlayer") != 0) fails++;
    if (!d.CanBeHeard || d.ReactionRestrictToTarget || !d.IsFlirting || !d.OnReactSetGreeted || !d.BlockWhileFollowingPlayer) fails++;
    Ctx load; load.loading = true;
    d.Transfer(load);
    // loaded pattern: true, false, true, ... over the eleven flags
    if (!d.CanBeHeard || d.ReactionRestrictToTarget || !d.AllowSimultaneousReactionDialogue || d.CanApologise) fails++;
    if (!d.MustNotRunWhenWarned || d.OnlyGetFinedForOneInstance || !d.IsFlirting || d.OnReactSetGreeted) fails++;
    if (!d.AllowSpeechOnNonPureAISpeaker || d.AllowIndirectReactWhileCarrying || !d.BlockWhileFollowingPlayer) fails++;
    for (int i = 0; i < load.n; i++) if (strcmp(load.log[i], "BADDEFAULT") == 0) fails++;
    if (fails == 0) printf("00478ee2_TEST PASS\n"); else printf("00478ee2_TEST FAIL %d\n", fails);
    return fails;
}