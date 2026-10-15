// CCombatActionBase::InitialiseActionTimes() -- sets the action's total time (frames) from its animation's duration, the action time from the anim's PERFORM_ACTION event, both shifted by the anim delay
// Leaf: InitialiseActionTimes
// Retail: Fable.exe (TLC, VC7.1) @ 0x00856A20, 154 bytes of code.
// Callees: the anim-manager getter @ 0x006F90B0 (__fastcall, no arguments; label "GFGetPreMainMemoryUsed" propagated),
//          CManager::AnimHasEvent(ulong, const CCharString&) @ 0x006F9720, CManager::GetEventTimeInSeconds @ 0x006F9790,
//          CBaseIntelligentPointer::GetPItem @ 0x00A01B50 (PCreature at +8), CTCAnimationComplex::GetAnimDuration(long)
//          @ 0x006627F0 (reached on the creature, float seconds), __ftol2 @ 0x00BFEA70, virtual GetAnimDelayTime (slot 6,
//          +0x18, twice).
// Globals: the game frames-per-second long @ 0x01375550 (copied into the `push ecx` frame word for both `fimul`s), the
//          event-name CCharString "PERFORM_ACTION" @ 0x013BACD0.
// Shape: TotalTime is cleared first; the anim id (PAnimEntry->[0]) and the manager are named locals (edi/ebx); the
//          creature call pushes the anim id BEFORE fetching the creature (an inline object expression); the action time
//          is only shifted by the delay when an event set it (>= 0).
// Members (retail layout, Ego_r.pdb names): TotalTime +0x10, ActionTime +0x14, PAnimEntry +0x74, HasPerformAction +0xde
// (PDB +0xf2: retail CCreatureActionBase is 0xa8 bytes), so `this` is modelled locally.
class CCharString;
extern CCharString g_performActionEventName;    // 0x013BACD0 ("PERFORM_ACTION")
extern long g_framesPerSecond;                  // 0x01375550

struct CAnimManager_Calls {                     // CManager (anim manager)
    bool  AnimHasEvent(unsigned long animID, const CCharString& eventName);            // 0x006F9720
    float GetEventTimeInSeconds(unsigned long animID, const CCharString& eventName);   // 0x006F9790
};
CAnimManager_Calls* __fastcall GetAnimManager();  // 0x006F90B0

struct CThingCreatureBase_Calls {
    float GetAnimDuration(long animID);         // 0x006627F0 (CTCAnimationComplex::GetAnimDuration)
};

struct CBaseIntelligentPointer {
    virtual void v00();
    void* m_pItem;                              // +0x04
    CThingCreatureBase_Calls* GetPItem() const; // 0x00A01B50
};

struct CAnimationEntry {
    long AnimID;                                // +0x00
};

struct CCombatActionBase {
    virtual void v00(); virtual void v01(); virtual void v02(); virtual void v03(); virtual void v04(); virtual void v05();
    virtual long GetAnimDelayTime();            // slot 6 (+0x18): 0x0062EA00
    unsigned char _pad04[4];                    // +0x04
    CBaseIntelligentPointer PCreature;          // +0x08
    long TotalTime;                             // +0x10
    long ActionTime;                            // +0x14
    unsigned char _pad18[0x74 - 0x18];
    CAnimationEntry* PAnimEntry;                // +0x74 CCountedPointer<CAnimationEntry> object
    void* PAnimEntryCount;                      // +0x78
    unsigned char _pad7c[0xde - 0x7c];
    bool HasPerformAction;                      // +0xde
    void InitialiseActionTimes();
};

void CCombatActionBase::InitialiseActionTimes()
{
    long framesPerSecond = g_framesPerSecond;
    TotalTime = 0;
    long animID = PAnimEntry->AnimID;
    CAnimManager_Calls* manager = GetAnimManager();
    if (manager->AnimHasEvent(animID, g_performActionEventName)) {
        ActionTime = (long)(manager->GetEventTimeInSeconds(animID, g_performActionEventName) * framesPerSecond) + TotalTime;
        HasPerformAction = true;
    }
    TotalTime = (long)(PCreature.GetPItem()->GetAnimDuration(animID) * framesPerSecond + TotalTime);
    if (ActionTime >= 0)
        ActionTime += GetAnimDelayTime();
    TotalTime += GetAnimDelayTime();
}
