// COpinionDeedEffectsDef::Transfer(CPersistContext& context) -- persist the deed-effects definition: knowledge life, posting and effect intervals, pre-reaction delay, effects list, reaction priority, reactions, dialogue layer, warning texts, crime severity and fine, tutorial category, then the eleven named flag bits
// Leaf: ?Transfer@COpinionDeedEffectsDef
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00478EE2, length 677 bytes (virtual, `ret 4`; unaligned entry, size-optimised
// def module with a frame pointer).
// Callees: CPersistContext typed transfers Transfer(long&) 0x00431102, Transfer(float&) 0x00431061,
//   Transfer(list<COpinionTransientOffset>&) 0x004792CF, Transfer(vector<COpinionDeedReactionDef>&) 0x00479466,
//   Transfer(EDialogueLayer&) 0x00479187, Transfer(vector<CCharString>&) 0x004331F9, Transfer(ECrimeSeverity&) 0x0047922B,
//   Transfer(ETutorialCategory&) 0x00452045, Transfer<bool>(name, value, default) 0x004045C0.
// Layout: Ego_r.pdb COpinionDeedEffectsDef (0x94) minus 4 up to the effects list (retail def base is 4 bytes smaller),
//   then the retail STLport containers: the list is one node pointer (4 bytes, PDB 12) and the vectors are 12 bytes
//   (PDB 16): KnowledgeLifeInSeconds +0x3C, MinimumSecondsBetweenPostings +0x40,
//   MinimumSecondsBetweenEffectsForIndividual +0x44, PreReactionDelayIgnoreOnDelay +0x48, Effects +0x4C,
//   ReactionPriority +0x50, Reaction +0x54, ReactionDialogueLayer +0x60, WarningText +0x64, CrimeSeverity +0x70,
//   CrimeFineAmount +0x74, Tutorial +0x78, flag bits +0x7C/+0x7D. No engine header.
// Shape: the landed CThingObjectDef::Transfer 0x0045912F model, but each flag in its OWN `{ bool flag = X; ... }` block
//   (one function-wide bool gives `and al,7Fh` first for the top bit 7 merge; retail does `shl al,7` first); the bool
//   still homes in the dead argument slot, with a one-byte
//   `false` temporary, bits merged back with xor/and/xor, constant 1 in ebx).
#pragma optimize("y", off)
#pragma optimize("s", on)

struct COpinionTransientOffsetList { void* _M_node; };
struct COpinionDeedReactionDefVector { void* _M_start; void* _M_finish; void* _M_end_of_storage; };
struct CCharStringVector { void* _M_start; void* _M_finish; void* _M_end_of_storage; };
enum EDialogueLayer { DIALOGUE_LAYER_0 };
enum ECrimeSeverity { CRIME_SEVERITY_0 };
enum ETutorialCategory { TUTORIAL_CATEGORY_0 };

struct CPersistContext {
    void Transfer(long& value);                                              // 0x00431102
    void Transfer(float& value);                                             // 0x00431061
    void Transfer(COpinionTransientOffsetList& value);                       // 0x004792CF
    void Transfer(COpinionDeedReactionDefVector& value);                     // 0x00479466
    void Transfer(EDialogueLayer& value);                                    // 0x00479187
    void Transfer(CCharStringVector& value);                                 // 0x004331F9
    void Transfer(ECrimeSeverity& value);                                    // 0x0047922B
    void Transfer(ETutorialCategory& value);                                 // 0x00452045
    void Transfer(const char* name, bool& value, const bool& defaultValue);  // 0x004045C0
};

struct CDefBase {
    virtual void Transfer(CPersistContext& context);
    unsigned char _pad04[0x3c - 4];
};

struct COpinionDeedEffectsDef : CDefBase {
    long                          KnowledgeLifeInSeconds;                    // +0x3c
    long                          MinimumSecondsBetweenPostings;             // +0x40
    long                          MinimumSecondsBetweenEffectsForIndividual; // +0x44
    float                         PreReactionDelayIgnoreOnDelay;             // +0x48
    COpinionTransientOffsetList   Effects;                                   // +0x4c
    long                          ReactionPriority;                          // +0x50
    COpinionDeedReactionDefVector Reaction;                                  // +0x54
    EDialogueLayer                ReactionDialogueLayer;                     // +0x60
    CCharStringVector             WarningText;                               // +0x64
    ECrimeSeverity                CrimeSeverity;                             // +0x70
    long                          CrimeFineAmount;                           // +0x74
    ETutorialCategory             Tutorial;                                  // +0x78
    bool CanBeHeard : 1;                                                     // +0x7c bit 0
    bool ReactionRestrictToTarget : 1;
    bool AllowSimultaneousReactionDialogue : 1;
    bool CanApologise : 1;
    bool MustNotRunWhenWarned : 1;
    bool OnlyGetFinedForOneInstance : 1;
    bool IsFlirting : 1;
    bool OnReactSetGreeted : 1;
    bool AllowSpeechOnNonPureAISpeaker : 1;                                  // +0x7d bit 0
    bool AllowIndirectReactWhileCarrying : 1;
    bool BlockWhileFollowingPlayer : 1;
    virtual void Transfer(CPersistContext& context);
};

void COpinionDeedEffectsDef::Transfer(CPersistContext& context)
{
    context.Transfer(KnowledgeLifeInSeconds);
    context.Transfer(MinimumSecondsBetweenPostings);
    context.Transfer(MinimumSecondsBetweenEffectsForIndividual);
    context.Transfer(PreReactionDelayIgnoreOnDelay);
    context.Transfer(Effects);
    context.Transfer(ReactionPriority);
    context.Transfer(Reaction);
    context.Transfer(ReactionDialogueLayer);
    context.Transfer(WarningText);
    context.Transfer(CrimeSeverity);
    context.Transfer(CrimeFineAmount);
    context.Transfer(Tutorial);
    {
        bool flag = CanBeHeard;
        context.Transfer("CanBeHeard", flag, false);
        CanBeHeard = flag;
    }
    {
        bool flag = ReactionRestrictToTarget;
        context.Transfer("ReactionRestrictToTarget", flag, false);
        ReactionRestrictToTarget = flag;
    }
    {
        bool flag = AllowSimultaneousReactionDialogue;
        context.Transfer("AllowSimultaneousReactionDialogue", flag, false);
        AllowSimultaneousReactionDialogue = flag;
    }
    {
        bool flag = CanApologise;
        context.Transfer("CanApologise", flag, false);
        CanApologise = flag;
    }
    {
        bool flag = MustNotRunWhenWarned;
        context.Transfer("MustNotRunWhenWarned", flag, false);
        MustNotRunWhenWarned = flag;
    }
    {
        bool flag = OnlyGetFinedForOneInstance;
        context.Transfer("OnlyGetFinedForOneInstance", flag, false);
        OnlyGetFinedForOneInstance = flag;
    }
    {
        bool flag = IsFlirting;
        context.Transfer("IsFlirting", flag, false);
        IsFlirting = flag;
    }
    {
        bool flag = OnReactSetGreeted;
        context.Transfer("OnReactSetGreeted", flag, false);
        OnReactSetGreeted = flag;
    }
    {
        bool flag = AllowSpeechOnNonPureAISpeaker;
        context.Transfer("AllowSpeechOnNonPureAISpeaker", flag, false);
        AllowSpeechOnNonPureAISpeaker = flag;
    }
    {
        bool flag = AllowIndirectReactWhileCarrying;
        context.Transfer("AllowIndirectReactWhileCarrying", flag, false);
        AllowIndirectReactWhileCarrying = flag;
    }
    {
        bool flag = BlockWhileFollowingPlayer;
        context.Transfer("BlockWhileFollowingPlayer", flag, false);
        BlockWhileFollowingPlayer = flag;
    }
}
