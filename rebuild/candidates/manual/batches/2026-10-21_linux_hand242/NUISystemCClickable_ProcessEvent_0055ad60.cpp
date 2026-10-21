// NUISystem::CClickable::ProcessEvent(EEvent) -- mouse-button events of a clickable component: left/right press (when hovered) fire OnLeftClicked/OnRightClicked and latch the clicked flag, release fires the unclick handler, the held events re-run the held/right-click actions at most once per InputDelay; every handled event stamps LastInputTime, then the CHoverable handler runs
// Retail: Fable.exe (TLC, VC7.1) @ 0x0055AD60, 294 bytes + a 7-entry jump table (336 with padding).
// Vtables: NUISystem::CClickable 0x0124BD08 slot 1 and NUISystem::CNavButton 0x0124B024 slot 1 -- the event
// receiver base sits at +4 (RTTI locator offset 4), so `this` is the full object + 4 and the primary virtuals
// are called through `lea ecx,[esi-4]`.
// Callees: CHoverable::ProcessEvent 0x0055B9D0; primary vtable slots 131 (+0x20c, ExecuteActions 0x005309B0),
//          146 (+0x248, OnLeftClicked), 147 (+0x24c, OnLeftUnclicked), 148 (+0x250, OnRightClicked) and
//          149 (+0x254, OnRightUnclicked) (CNavButton vtable 0x0124B04C).
//
// Shape: a dense switch on events 26..32 (`lea eax,[edi-1Ah] ; cmp eax,6 ; ja`, jump table) with
// `LastInputTime = Time; break;` closing every handled case and one CHoverable::ProcessEvent call after the
// switch. The cases are written in retail's block order (left clicked, held, unclicked, then right clicked,
// held, unclicked): VC7.1 lays case blocks out in source order, and that order also decides which stamp tails
// are merged (ecx group / eax group) and that only the left-click tail is duplicated. The held test is
// `if (Time - LastInputTime >= InputDelay) { ... }` (`test ah,1 ; jne` skips both the actions and the stamp).
// Layout: local model. NUISystem::CClickable is 0x1c4 bytes in the PDB (fable_types.h); retail sits 0x30
// lower from CHoverable on (CChangingStateComponent is 0x15c in retail): WasHovered +0x160, WasLeftClicked
// +0x170, PActionsOnLeftHeld +0x178, WasRightClicked +0x184, PActionsOnRightClicked +0x188, InputDelay +0x18c,
// LastInputTime +0x190; CComponent::Time is +0x30 (as the landed 0x005339B0).
namespace NUISystem {

enum EEvent {
    EVENT_LEFT_CLICKED = 26,
    EVENT_RIGHT_CLICKED = 27,
    EVENT_LEFT_UNCLICKED = 28,
    EVENT_RIGHT_UNCLICKED = 29,
    EVENT_30 = 30,
    EVENT_LEFT_HELD = 31,
    EVENT_RIGHT_HELD = 32
};
struct CActionList;                         // list<CCountedPointer<NUISystem::CAction>>
struct CComponentVirtuals {                 // the primary vtable (+0)
    virtual void VSlot0();
    virtual void VSlot1();
    virtual void VSlot2();
    virtual void VSlot3();
    virtual void VSlot4();
    virtual void VSlot5();
    virtual void VSlot6();
    virtual void VSlot7();
    virtual void VSlot8();
    virtual void VSlot9();
    virtual void VSlot10();
    virtual void VSlot11();
    virtual void VSlot12();
    virtual void VSlot13();
    virtual void VSlot14();
    virtual void VSlot15();
    virtual void VSlot16();
    virtual void VSlot17();
    virtual void VSlot18();
    virtual void VSlot19();
    virtual void VSlot20();
    virtual void VSlot21();
    virtual void VSlot22();
    virtual void VSlot23();
    virtual void VSlot24();
    virtual void VSlot25();
    virtual void VSlot26();
    virtual void VSlot27();
    virtual void VSlot28();
    virtual void VSlot29();
    virtual void VSlot30();
    virtual void VSlot31();
    virtual void VSlot32();
    virtual void VSlot33();
    virtual void VSlot34();
    virtual void VSlot35();
    virtual void VSlot36();
    virtual void VSlot37();
    virtual void VSlot38();
    virtual void VSlot39();
    virtual void VSlot40();
    virtual void VSlot41();
    virtual void VSlot42();
    virtual void VSlot43();
    virtual void VSlot44();
    virtual void VSlot45();
    virtual void VSlot46();
    virtual void VSlot47();
    virtual void VSlot48();
    virtual void VSlot49();
    virtual void VSlot50();
    virtual void VSlot51();
    virtual void VSlot52();
    virtual void VSlot53();
    virtual void VSlot54();
    virtual void VSlot55();
    virtual void VSlot56();
    virtual void VSlot57();
    virtual void VSlot58();
    virtual void VSlot59();
    virtual void VSlot60();
    virtual void VSlot61();
    virtual void VSlot62();
    virtual void VSlot63();
    virtual void VSlot64();
    virtual void VSlot65();
    virtual void VSlot66();
    virtual void VSlot67();
    virtual void VSlot68();
    virtual void VSlot69();
    virtual void VSlot70();
    virtual void VSlot71();
    virtual void VSlot72();
    virtual void VSlot73();
    virtual void VSlot74();
    virtual void VSlot75();
    virtual void VSlot76();
    virtual void VSlot77();
    virtual void VSlot78();
    virtual void VSlot79();
    virtual void VSlot80();
    virtual void VSlot81();
    virtual void VSlot82();
    virtual void VSlot83();
    virtual void VSlot84();
    virtual void VSlot85();
    virtual void VSlot86();
    virtual void VSlot87();
    virtual void VSlot88();
    virtual void VSlot89();
    virtual void VSlot90();
    virtual void VSlot91();
    virtual void VSlot92();
    virtual void VSlot93();
    virtual void VSlot94();
    virtual void VSlot95();
    virtual void VSlot96();
    virtual void VSlot97();
    virtual void VSlot98();
    virtual void VSlot99();
    virtual void VSlot100();
    virtual void VSlot101();
    virtual void VSlot102();
    virtual void VSlot103();
    virtual void VSlot104();
    virtual void VSlot105();
    virtual void VSlot106();
    virtual void VSlot107();
    virtual void VSlot108();
    virtual void VSlot109();
    virtual void VSlot110();
    virtual void VSlot111();
    virtual void VSlot112();
    virtual void VSlot113();
    virtual void VSlot114();
    virtual void VSlot115();
    virtual void VSlot116();
    virtual void VSlot117();
    virtual void VSlot118();
    virtual void VSlot119();
    virtual void VSlot120();
    virtual void VSlot121();
    virtual void VSlot122();
    virtual void VSlot123();
    virtual void VSlot124();
    virtual void VSlot125();
    virtual void VSlot126();
    virtual void VSlot127();
    virtual void VSlot128();
    virtual void VSlot129();
    virtual void VSlot130();
    virtual void ExecuteActions(CActionList* actions);              // slot 131 (+0x20c), 0x005309B0
    virtual void VSlot132();
    virtual void VSlot133();
    virtual void VSlot134();
    virtual void VSlot135();
    virtual void VSlot136();
    virtual void VSlot137();
    virtual void VSlot138();
    virtual void VSlot139();
    virtual void VSlot140();
    virtual void VSlot141();
    virtual void VSlot142();
    virtual void VSlot143();
    virtual void VSlot144();
    virtual void VSlot145();
    virtual void OnLeftClicked();                                   // slot 146 (+0x248)
    virtual void OnLeftUnclicked();                                 // slot 147 (+0x24c)
    virtual void OnRightClicked();                                  // slot 148 (+0x250)
    virtual void OnRightUnclicked();                                // slot 149 (+0x254)
};
struct CEventReceiver {                     // secondary base at +4 (vtable slot 1 ProcessEvent)
    virtual void VSlot0();
    virtual void ProcessEvent(EEvent event);
    virtual void VSlot2();
    unsigned char _data[0x10];              // +0x08 .. +0x18
};
class CComponent : public CComponentVirtuals, public CEventReceiver {
public:
    unsigned char _pad18[0x18];             // +0x18 (a third base with its vtable, Name, States)
    float         Time;                     // +0x30
    unsigned char _pad34[0x100];            // +0x34 .. +0x134
};
class CChangingStateComponent : public CComponent {
public:
    unsigned char _pad134[0x28];            // +0x134 .. +0x15c (state machine)
};
class CHoverable : public CChangingStateComponent {
public:
    long          StateBeforeHovered;       // +0x15c
    bool          WasHovered;               // +0x160
    CActionList*  PActionsOnHovered;        // +0x164
    CActionList*  PActionsOnUnhovered;      // +0x168
    virtual void ProcessEvent(EEvent event);    // 0x0055B9D0
};
class CClickable : public CHoverable {
public:
    long          StateBeforeLeftClicked;   // +0x16c
    bool          WasLeftClicked;           // +0x170
    CActionList*  PActionsOnLeftClicked;    // +0x174
    CActionList*  PActionsOnLeftHeld;       // +0x178
    CActionList*  PActionsOnLeftUnclicked;  // +0x17c
    long          StateBeforeRightClicked;  // +0x180
    bool          WasRightClicked;          // +0x184
    CActionList*  PActionsOnRightClicked;   // +0x188
    float         InputDelay;               // +0x18c
    float         LastInputTime;            // +0x190
    virtual void ProcessEvent(EEvent event);
};

void CClickable::ProcessEvent(EEvent event)
{
    switch (event) {
    case EVENT_LEFT_CLICKED:
        if (WasHovered) {
            OnLeftClicked();
            WasLeftClicked = true;
        }
        LastInputTime = Time;
        break;
    case EVENT_LEFT_HELD:
        if (Time - LastInputTime >= InputDelay) {
            if (WasLeftClicked)
                ExecuteActions(PActionsOnLeftHeld);
            LastInputTime = Time;
        }
        break;
    case EVENT_LEFT_UNCLICKED:
        if (WasLeftClicked) {
            OnLeftUnclicked();
            WasLeftClicked = false;
        }
        LastInputTime = Time;
        break;
    case EVENT_RIGHT_CLICKED:
        if (WasHovered) {
            OnRightClicked();
            WasRightClicked = true;
        }
        LastInputTime = Time;
        break;
    case EVENT_RIGHT_HELD:
        if (WasRightClicked)
            ExecuteActions(PActionsOnRightClicked);
        LastInputTime = Time;
        break;
    case EVENT_RIGHT_UNCLICKED:
        if (WasRightClicked) {
            OnRightUnclicked();
            WasRightClicked = false;
        }
        LastInputTime = Time;
        break;
    }
    CHoverable::ProcessEvent(event);
}

}
