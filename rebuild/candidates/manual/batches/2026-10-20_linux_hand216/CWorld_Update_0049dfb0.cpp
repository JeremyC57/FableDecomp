// CWorld::Update(const CGameEventPackage& events, bool processType1Events) -- times the world update: publishes the thing manager, dispatches every event except type 1 through its action handler, then (when asked) the type-1 events, and records the update duration
// Leaf: ?Update@CWorld@@
// Retail: Fable.exe (TLC, VC7.1) @ 0x0049DFB0, 198 bytes (padded to 208), `ret 8`.
//   PDB name ?Update@CWorld@@QAEXABVCGameEventPackage@@_N@Z.
// Callees: GFGetTime 0x009E1BC0 (`__fastcall`, double), CWorld virtual slot 3 GetThingManager 0x0049E1B0 (returns a
//   reference), SetCurrentThingManager 0x004C74F0 (`__fastcall`), CGameEventPackage::GetNoEvents 0x009F16E0 / GetEvent
//   0x009F16D0, the event's ActionFunction pointer to member from the 64-byte dispatch table 0x013B9288 (named as in the
//   landed CDisplayEngine::ConfirmFeedbackGameEventPackage 0x00434A60).
// Shape: the inline ProcessEvent (`GetEventDispatchTableEntry(type)->ActionFunction`, an 8-byte {function, this-adjust}
//   pointer to member called when set) is expanded in both loops; in the second loop the type is known to be 1, so the
//   entry address folds to 0x013B92C8; the start time is a double local kept in the frame (`fst`), LastUpdateTime stored
//   from the same value.
// Layout (engine/CWorld.h, offsets below +0xac match retail): two polymorphic bases (vtables 0x012390F0 / 0x012390DC),
//   LastUpdateTimeLength +0x90, LastUpdateTime +0x98.
struct __multiple_inheritance CWorld;
struct CGameEvent {
    long Type;                                     // +0 the dispatch-table index
};
struct CGameEventPackage {
    unsigned long GetNoEvents() const;             // 0x009F16E0
    const CGameEvent* GetEvent(unsigned long index) const;   // 0x009F16D0
};
struct SGameEventHandler {                         // CGameEventDispatch, 64 bytes (four 16-byte member-pointer slots)
    void (CWorld::*ActionFunction)(const CGameEvent* event);   // +0x00 {function, this-adjust}
    unsigned char _pad08[0x40 - 0x08];             // LocalAction +0x10, Feedback +0x20, Confirm +0x30
};
extern SGameEventHandler g_GameEventHandlers[];    // 0x013B9288
struct CThingManager;
double __fastcall GFGetTime();                     // 0x009E1BC0
void __fastcall SetCurrentThingManager(CThingManager* manager);   // 0x004C74F0
struct CWorldPrimaryBase {                         // vtable 0x012390F0
    virtual ~CWorldPrimaryBase();
    virtual void v01();
    virtual void v02();
    virtual CThingManager& GetThingManager();      // slot 3 (+0xc) 0x0049E1B0
};
struct CWorldSecondaryBase {                       // vtable 0x012390DC at +4
    virtual ~CWorldSecondaryBase();
};
struct CWorld : CWorldPrimaryBase, CWorldSecondaryBase {
    unsigned char _pad08[0x90 - 0x08];
    double        LastUpdateTimeLength;            // +0x90
    double        LastUpdateTime;                  // +0x98
    const SGameEventHandler* GetEventDispatchTableEntry(long type) const { return &g_GameEventHandlers[type]; }
    void ProcessEvent(const CGameEvent* event)
    {
        const SGameEventHandler* entry = GetEventDispatchTableEntry(event->Type);
        if (entry->ActionFunction)
            (this->*entry->ActionFunction)(event);
    }
    void Update(const CGameEventPackage& events, bool processType1Events);
};
void CWorld::Update(const CGameEventPackage& events, bool processType1Events)
{
    double start = GFGetTime();
    LastUpdateTime = start;
    SetCurrentThingManager(&GetThingManager());
    for (unsigned long i = 0; i < events.GetNoEvents(); ++i) {
        const CGameEvent* event = events.GetEvent(i);
        if (event->Type != 1)
            ProcessEvent(event);
    }
    if (processType1Events) {
        for (unsigned long i = 0; i < events.GetNoEvents(); ++i) {
            const CGameEvent* event = events.GetEvent(i);
            if (event->Type == 1)
                ProcessEvent(event);
        }
    }
    LastUpdateTimeLength = GFGetTime() - start;
}
