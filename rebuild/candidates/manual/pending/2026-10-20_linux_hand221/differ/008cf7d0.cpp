// CCreatureAction_TentacleStrikeLoop::FinishAction() -- base FinishAction; keep looping (a new TentacleStrikeLoop with the same start tick) until interrupted or the tentacle def's strike time has elapsed, then switch to TentacleStrikeOutOf
// Leaf: FinishAction
// Grade: DIFFER(265v287) -- retail reads g_framesPerSecond and the tentacle def before the frame-counter call (global parked in the frame), compares Interrupt against a shared ebx zero (later the CSoundPair zeros) and keeps the tentacle in eax; ours calls first, filds the global directly and holds the tentacle in ebx
// Retail: Fable.exe (TLC, VC7.1) @ 0x008CF7D0, 287 bytes (288 with padding), virtual FinishAction.
// Callees: CCombatActionBase::FinishAction 0x00859A20 (direct, first), CBaseIntelligentPointer::GetPItem 0x00A01B50,
//   CVectorMap::LowerBound 0x0040F020 (inlined unchecked lookup of 0xE3 = CTCKrakenTentacle, bit 3 of +0x3c),
//   GFGetCurrentFrame 0x0049D870 (__fastcall), _ftol2 0x00BFEA70, CCreatureAction_TentacleStrikeLoop(CThingCreatureBase*,
//   long) 0x008CF990 (landed), CCombatActionBase(CThingCreatureBase*, long, const CSoundPair&) 0x00858030 (the inlined
//   TentacleStrikeOutOf ctor, action 250, vtable 0x0128503C), CThingCreatureBase::SetCurrentAction 0x006644F0,
//   ~CCombatActionBase 0x0062E900. Global g_framesPerSecond (long) 0x01375550.
// Layout: the landed TentacleStrikeInto/StrikeLoop sources: StartTick +0x114, Interrupt +0x118 (retail offsets); the
//   tentacle component's def at +0x0c, whose +0x28 is the strike duration in seconds.
// Shape: `frame - StartTick <= (long)(g_framesPerSecond * secs)` (the right operand -- the global -- is read first and
//   parked in the frame across the frame-counter call); each branch passes a by-value temporary action to
//   SetCurrentAction (VC duplicates the destructor/epilogue into both).
struct CTCBase;
struct CInterfaceMap {
    struct SEntry { long type; CTCBase* component; };
    SEntry* first;
    SEntry* last;
    SEntry* LowerBound(const long* type);             // 0x0040F020
};
struct CThing {
    unsigned char _pad0[0x20];
    unsigned long m_presentBits[9];                   // +0x20
    CInterfaceMap m_interfaces;                       // +0x44
    template<class T> __forceinline bool GetInterface(T** ppOut, long interfaceType)
    {
        if (m_presentBits[interfaceType >> 5] & (1UL << (interfaceType & 31))) {
            long type = interfaceType;
            CInterfaceMap* map = &m_interfaces;
            CInterfaceMap::SEntry* it = map->LowerBound(&type);
            if (it == map->last || interfaceType < it->type)
                it = map->last;
            *ppOut = (T*)it->component;
            return true;
        }
        return false;
    }
};
struct CSoundPair {
    long m_first;
    long m_second;
    CSoundPair() : m_first(0), m_second(0) { }
};
struct CCombatActionBase;
struct CThingCreatureBase : CThing {
    bool SetCurrentAction(const CCombatActionBase& action);   // 0x006644F0
};
struct CBaseIntelligentPointer {
    virtual void v00();
    void* m_pItem;                                    // +0x04
    CThingCreatureBase* GetPItem() const;             // 0x00A01B50
};
struct CKrakenTentacleDef {
    unsigned char _pad00[0x28];
    float StrikeTimeSecs;                             // +0x28
};
struct CTCKrakenTentacle {                            // interface type 0xE3
    unsigned char _pad00[0x0c];
    const CKrakenTentacleDef* PDef;                   // +0x0c
};
extern long g_framesPerSecond;                        // 0x01375550
long __fastcall GFGetCurrentFrame();                  // 0x0049D870

struct CCombatActionBase {
    virtual void v00();
    unsigned char _pad04[4];
    CBaseIntelligentPointer PCreature;                // +0x08
    unsigned char _pad10[0x114 - 0x10];
    CCombatActionBase(CThingCreatureBase* owner, long actionCode, const CSoundPair& sound);   // 0x00858030
    ~CCombatActionBase();                             // 0x0062E900
    void FinishAction();                              // 0x00859A20
};
struct CCreatureAction_TentacleStrikeOutOf : CCombatActionBase {   // vtable 0x0128503C
    virtual void v00();
    CCreatureAction_TentacleStrikeOutOf(CThingCreatureBase* owner) : CCombatActionBase(owner, 250, CSoundPair()) { }
};
struct CCreatureAction_TentacleStrikeLoop : CCombatActionBase {   // vtable 0x01284EF4
    long  StartTick;                                  // +0x114
    bool  Interrupt;                                  // +0x118
    float StartHealth;                                // +0x11c
    virtual void v00();
    CCreatureAction_TentacleStrikeLoop(CThingCreatureBase* owner, long startTick);   // 0x008CF990
    void FinishAction();
};

void CCreatureAction_TentacleStrikeLoop::FinishAction()
{
    CCombatActionBase::FinishAction();
    CTCKrakenTentacle* tentacle;
    PCreature.GetPItem()->GetInterface(&tentacle, 0xE3);
    if (!Interrupt && (long)(g_framesPerSecond * tentacle->PDef->StrikeTimeSecs) >= GFGetCurrentFrame() - StartTick)
        PCreature.GetPItem()->SetCurrentAction(CCreatureAction_TentacleStrikeLoop(PCreature.GetPItem(), StartTick));
    else
        PCreature.GetPItem()->SetCurrentAction(CCreatureAction_TentacleStrikeOutOf(PCreature.GetPItem()));
}
