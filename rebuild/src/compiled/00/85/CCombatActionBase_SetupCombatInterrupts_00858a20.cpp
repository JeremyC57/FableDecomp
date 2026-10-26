// CCombatActionBase::SetupCombatInterrupts() -- read the strike-interrupt, reset-combo, movement-interrupt and strike-speed-variation events of the action's animation into the interrupt times/frame and their Has* flags
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00858A20, length 665 bytes (+ int3 padding), esp frame, plain `ret`.
// Callees: the anim-manager getter @ 0x006F90B0 (`__fastcall`, no arguments), NAnimationEvents::CManager::GetEvents (virtual
//   slot 0, Ego_r.pdb `UBEX...vector<pair<CCharString,float> >`), CManager::AnimHasEvent @ 0x006F9720 and
//   GetEventTimeInSeconds @ 0x006F9790, virtual GetAnimDelayTime (slot 6, +0x18), CBaseIntelligentPointer::GetPItem
//   @ 0x00A01B50, CDefClassBase::GetSubDef<CCreatureStatsDef> @ 0x00696130 (pushes "CCreatureStatsDef"; same-TU out-only
//   stand-in here), CCharString::CCharString(const char*, long) @ 0x0099EBF0 / ~CCharString @ 0x0099EAE0, _ftol @ 0x00BFEA70,
//   free @ 0x00BFEA14 (the event vector's storage).
//   Data: g_FramesPerSecond @ 0x01375550 (long), the event-name CCharStrings @ 0x013BAC88 ("COMBT_ACTION_FINISHED"),
//   0x013BAC74 ("STRIKE_EVENT_RESET_COMBO"), 0x013BACE0 ("ALLOW_MOVEMENT_INTERRUPT"), 1.0f @ 0x0122DED8, 0.5f @ 0x0122F59C,
//   the literals "STRIKE_SPEED_VARIATION_START" @ 0x012764D0 and "STRIKE_SPEED_VARIATION_END" @ 0x012764B4.
// Shape: the frame rate is converted to a float local first. The fetched event list is never read (only destroyed: the
//   element loop over (first, last) copies, then free). The creature-stats out word goes to a same-TU out-only stand-in, so
//   VC7.1 knows the def's `dec [ecx+4]` cannot alias it (no reload, flags from the dec). The speed defaults to a 1.0f frame
//   word that the if/else merge loads onto the x87 stack. The speed-variation names are block-scoped named strings (built,
//   then re-addressed with `lea`). A 1.0f time scale folds the four `*=` into plain load/store self-copies and leaves the
//   movement frame's fild/_ftol round trip, as retail has them.
// Layout: engine/CCombatActionBase.h is quarantined (PDB build differs); retail offsets follow the landed InitialiseActionTimes
//   0x00856A20 / FrameUpdate 0x00859D80 models: TotalTime +0x10, PAnimEntry +0x74, the interrupt/reset-combo times +0xc4/+0xc8,
//   movement-interrupt frame +0xcc, speed-variation start/end +0xd0/+0xd4, Has* flags +0xd8..+0xda. CThing +0x70 = PDef,
//   CCreatureStatsDef +0x3c = Speed (Ego_r.pdb).
extern "C" void __cdecl free(void* block);            // 0x00BFEA14
extern long g_FramesPerSecond;                         // 0x01375550

class CCharString { public:
    void* m_pData;
    CCharString(const char* text, long length = -1);   // 0x0099EBF0
    ~CCharString();                                    // 0x0099EAE0
};
extern CCharString g_combtActionFinishedEventName;         // 0x013BAC88 ("COMBT_ACTION_FINISHED")
extern CCharString g_strikeEventResetComboEventName;        // 0x013BAC74 ("STRIKE_EVENT_RESET_COMBO")
extern CCharString g_allowMovementInterruptEventName;       // 0x013BACE0 ("ALLOW_MOVEMENT_INTERRUPT")

struct CAnimEvent {                                    // std::pair<CCharString, float>
    CCharString Name;
    float       Time;
};
struct CAnimEventVector {                              // std::vector<std::pair<CCharString, float> >
    CAnimEvent* First;
    CAnimEvent* Last;
    CAnimEvent* End;
    CAnimEventVector() : First(0), Last(0), End(0) { }
    static void Destroy(CAnimEvent* first, CAnimEvent* last)
    {
        for (; first != last; ++first)
            first->~CAnimEvent();
    }
    ~CAnimEventVector()
    {
        Destroy(First, Last);
        if (First)
            free(First);
    }
};

namespace NAnimationEvents {
class CManager { public:
    virtual void GetEvents(unsigned long animID, CAnimEventVector& events) const;          // slot 0
    bool  AnimHasEvent(unsigned long animID, const CCharString& eventName) const;          // 0x006F9720
    float GetEventTimeInSeconds(unsigned long animID, const CCharString& eventName) const; // 0x006F9790
};
}
NAnimationEvents::CManager* __fastcall GetAnimManager();   // 0x006F90B0

struct CDefBase {
    virtual void v00();
    virtual void Destroy();                            // slot 1
    long RefCount;                                     // +0x04
    void Release() { if (--RefCount == 0) Destroy(); }
};
class CCreatureStatsDef : public CDefBase { public:
    unsigned char _pad08[0x3c - 0x08];
    float Speed;                                       // +0x3c
};
template <class T> class CDefPointer { public:
    T* m_p;
    CDefPointer() : m_p(0) { }
    ~CDefPointer() { if (m_p) m_p->Release(); }
    T* operator->() const { return m_p; }
};
class CDefClassBase { public:
    bool GetSubDef(CDefPointer<CCreatureStatsDef>& out);   // 0x00696130
};
void* FindSubDef_CCreatureStatsDef(CDefClassBase* def);
#pragma auto_inline(off)
__declspec(noinline) bool CDefClassBase::GetSubDef(CDefPointer<CCreatureStatsDef>& out)
{
    void* found = FindSubDef_CCreatureStatsDef(this);
    if (!found)
        return false;
    out.m_p = (CCreatureStatsDef*)found;
    return true;
}
#pragma auto_inline(on)
class CThing { public:
    unsigned char  _pad0[0x70];
    CDefClassBase* PDef;                               // +0x70
};
struct CBaseIntelligentPointer {
    virtual void v00();
    void* m_pData;
    CThing* GetPItem() const;                          // 0x00A01B50
};
class CAnimationEntry { public:
    unsigned long AnimID;                              // +0x00
};

class CCombatActionBase { public:
    virtual void v00(); virtual void v01(); virtual void v02(); virtual void v03(); virtual void v04(); virtual void v05();
    virtual long GetAnimDelayTime() const;             // slot 6 (+0x18)
    unsigned char           _pad04[4];
    CBaseIntelligentPointer PCreature;                 // +0x08
    long                    TotalTime;                 // +0x10
    unsigned char           _pad14[0x74 - 0x14];
    CAnimationEntry*        PAnimEntry;                // +0x74 (CCountedPointer<CAnimationEntry>)
    void*                   PAnimEntryInfo;            // +0x78
    unsigned char           _pad7c[0xc4 - 0x7c];
    float StrikeEventInterruptTime;                    // +0xc4
    float StrikeEventResetComboTime;                   // +0xc8
    long  MovementEventInterruptFrame;                 // +0xcc
    float StrikeSpeedVariationStartTime;               // +0xd0
    float StrikeSpeedVariationEndTime;                 // +0xd4
    bool  HasStrikeEventInterrupt;                     // +0xd8
    bool  HasStrikeEventResetCombo;                    // +0xd9
    bool  HasMovementEventInterrupt;                   // +0xda
    unsigned char _paddb[0x114 - 0xdb];
    void SetupCombatInterrupts();
};

void CCombatActionBase::SetupCombatInterrupts()
{
    float framesPerSecond = (float)g_FramesPerSecond;
    float timeScale = 1.0f;
    unsigned long animID = PAnimEntry->AnimID;
    NAnimationEvents::CManager* manager = GetAnimManager();
    CAnimEventVector events;
    manager->GetEvents(animID, events);
    if (manager->AnimHasEvent(animID, g_combtActionFinishedEventName)) {
        HasStrikeEventInterrupt = true;
        float eventTime = manager->GetEventTimeInSeconds(animID, g_combtActionFinishedEventName);
        float totalTime = (TotalTime - GetAnimDelayTime()) / framesPerSecond;
        float speed = 1.0f;
        CDefPointer<CCreatureStatsDef> stats;
        CDefClassBase* creatureDef = PCreature.GetPItem()->PDef;
        if (creatureDef->GetSubDef(stats))
            speed = stats->Speed;
        StrikeEventInterruptTime = eventTime + (totalTime - eventTime) * (1.0f - speed) * 0.5f;
    }
    if (manager->AnimHasEvent(animID, g_strikeEventResetComboEventName)) {
        HasStrikeEventResetCombo = true;
        StrikeEventResetComboTime = manager->GetEventTimeInSeconds(animID, g_strikeEventResetComboEventName);
    }
    if (manager->AnimHasEvent(animID, g_allowMovementInterruptEventName)) {
        HasMovementEventInterrupt = true;
        MovementEventInterruptFrame = (long)(manager->GetEventTimeInSeconds(animID, g_allowMovementInterruptEventName) * framesPerSecond);
    }
    bool hasStart;
    {
        CCharString name("STRIKE_SPEED_VARIATION_START");
        hasStart = manager->AnimHasEvent(animID, name);
    }
    if (hasStart) {
        {
            CCharString name("STRIKE_SPEED_VARIATION_START");
            StrikeSpeedVariationStartTime = manager->GetEventTimeInSeconds(animID, name);
        }
        bool hasEnd;
        {
            CCharString name("STRIKE_SPEED_VARIATION_END");
            hasEnd = manager->AnimHasEvent(animID, name);
        }
        if (hasEnd) {
            CCharString name("STRIKE_SPEED_VARIATION_END");
            StrikeSpeedVariationEndTime = manager->GetEventTimeInSeconds(animID, name);
        } else {
            StrikeSpeedVariationStartTime = -1.0f;
            StrikeSpeedVariationEndTime = -1.0f;
        }
    }
    StrikeEventInterruptTime *= timeScale;
    StrikeEventResetComboTime *= timeScale;
    StrikeSpeedVariationStartTime *= timeScale;
    StrikeSpeedVariationEndTime *= timeScale;
    if (HasMovementEventInterrupt) {
        MovementEventInterruptFrame = (long)(MovementEventInterruptFrame * timeScale);
        MovementEventInterruptFrame += GetAnimDelayTime();
    }
}