// CTCStealth::SetAsInDanger(const CThing* thing, long status) -- record (or refresh) the danger status of a thing; for the main player's creature entering danger start the danger music and stop the rain; restart the in-danger cool-off timer
// Leaf: SetAsInDanger
// Grade: DIFFER(242v240) -- retail sinks `push ebp` into the miss branch to hold the status (and zeroes through registers), the lane re-reads the status slot and stores immediates; the iterator-tag temporary lands in the status slot in retail, in the dead thing slot in the lane
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00747B80, 238 bytes (`ret 8`).
// Callees: SGI __find_if over the danger list @ 0x007485F0 (__fastcall: first, last, the by-value thing predicate and the
// iterator tag by reference -- its temporary homed in the dead `thing` argument slot), CBaseIntelligentPointer ctor
// @ 0x00A01B10 / SetPItem @ 0x00A01B90 / dtor @ 0x00A01C10 (the pair temporary's CIntelligentPointer<const CThing>, vtable
// 0x01237F1C), vector<pair<CIntelligentPointer<const CThing>, CDangerStatus> >::push_back @ 0x00749620, CThing::GetPlayer const
// @ 0x004C7A30, CPlayer::IsMainPlayer @ 0x00487520, CTCBase::GetWorld @ 0x00686A70 (twice), CMusicManager::EnableDangerMusic(bool)
// @ 0x006BDE50, CEnvironment::SetRainEnabled(bool) @ 0x006B32B0, __ftol2 @ 0x00BFEA70; g_framesPerSecond @ 0x01375550.
// Layout: as the landed ClearDangerStatus 0x00747090 / SetAsRunningAwayFromCombat 0x007470E0 (PDef +0x1c, InDangerCoolOffTimer
// +0x34, DangerThings +0x38/+0x3c of 16-byte {CIntelligentPointer, Status, Timer}); CStealthDef DangerFeedbackHangtimeSeconds +0x40.
// Shape: a miss pushes a pair temporary (pointer built in place, then the status and a zero timer), a hit refreshes the
// entry in place; the cool-off timer is `(long)(PDef ? seconds * fps : 1.0f)` (its __ftol2 tail duplicated per arm).
class CThing;
class CBaseObject;
extern long g_framesPerSecond;                          // 0x01375550
struct CPlayer {
    bool IsMainPlayer() const;                          // 0x00487520
};
struct CMusicManager {
    void EnableDangerMusic(bool enable);                // 0x006BDE50
};
struct CEnvironment {
    void SetRainEnabled(bool enabled);                  // 0x006B32B0
};
struct CWorld {
    unsigned char  _pad00[0x18];
    CEnvironment*  PEnvironment;                        // +0x18 scoped_ptr<CEnvironment>
    unsigned char  _pad1c[0x6c - 0x1c];
    CMusicManager* PMusicManager;                       // +0x6c CCountedPointer<CMusicManager> (object word)
};
class CThing {
public:
    unsigned char _pad[0x10];
    unsigned char m_thingType;                          // +0x10 (2 = creature)
    CPlayer* GetPlayer() const;                         // 0x004C7A30
};
struct CBaseIntelligentPointer {
    virtual void v00();
    CThing* m_pItem;                                    // +0x04
    CBaseIntelligentPointer();                          // 0x00A01B10
    ~CBaseIntelligentPointer();                         // 0x00A01C10
    CThing* GetPItem() const;                           // 0x00A01B50
    void SetPItem(CBaseObject* item);                   // 0x00A01B90
};
template <class T> struct CIntelligentPointer : CBaseIntelligentPointer {   // vtable 0x01237F1C (const CThing)
    virtual void v00();
    CIntelligentPointer(T* item) : CBaseIntelligentPointer() { SetPItem((CBaseObject*)item); }
};
struct CDangerThing {                                   // pair<CIntelligentPointer<const CThing>, NStealth::CDangerStatus>, 16 bytes
    CIntelligentPointer<const CThing> first;            // +0x00
    long Status;                                        // +0x08
    long Timer;                                         // +0x0c
    CDangerThing(const CThing* thing, long status) : first(thing) { Status = status; Timer = 0; }
};
struct CDangerThingIs {                                 // the find predicate: the entry's thing is the given one
    const CThing* Thing;
    CDangerThingIs(const CThing* thing) : Thing(thing) { }
};
struct random_access_iterator_tag { random_access_iterator_tag() { } };
CDangerThing* __fastcall __find_if(CDangerThing* first, CDangerThing* last, CDangerThingIs pred,
                                   const random_access_iterator_tag& tag);   // 0x007485F0
struct CDangerThingVector {                             // STLport vector (12 bytes)
    CDangerThing* _M_start;
    CDangerThing* _M_finish;
    CDangerThing* _M_end_of_storage;
    CDangerThing* begin() { return _M_start; }
    CDangerThing* end() { return _M_finish; }
    void push_back(const CDangerThing& value);          // 0x00749620
};
inline CDangerThing* find_if(CDangerThing* first, CDangerThing* last, CDangerThingIs pred)
{
    return __find_if(first, last, pred, random_access_iterator_tag());
}
struct CStealthDef {
    unsigned char _pad[0x40];
    float DangerFeedbackHangtimeSeconds;                // +0x40
};
struct CTCStealth {
    void*         __vftable;                            // +0x00
    CThing*       ParentThing;                          // +0x04
    bool          ToBeRemoved;                          // +0x08
    bool          IsInGlobalUpdateTCs;                  // +0x09
    unsigned char _pad0a[2];
    bool          Viewable;                             // +0x0c
    bool          Smellable;                            // +0x0d
    bool          Hearable;                             // +0x0e
    bool          Sneaking;                             // +0x0f
    bool          SneakingToggleSet;                    // +0x10
    float         DistanceCanBeHeardFrom;               // +0x14
    float         MaxSneakingSpeed;                     // +0x18
    CStealthDef*  PDef;                                 // +0x1c CDefPointer<CStealthDef const>
    long          FrameStartedWalking;                  // +0x20
    long          FrameStartedSneakingAndMoving;        // +0x24
    long          SneakingAndMovingDeedHandle;          // +0x28
    long          ActionPermittedToSneakID;             // +0x2c
    long          RunningAwayFromCombatTimer;           // +0x30
    long          InDangerCoolOffTimer;                 // +0x34
    CDangerThingVector DangerThings;                    // +0x38
    CWorld* GetWorld();                                 // 0x00686A70 (CTCBase)
    void SetAsInDanger(const CThing* thing, long status);
};

void CTCStealth::SetAsInDanger(const CThing* thing, long status)
{
    CDangerThing* entry = find_if(DangerThings.begin(), DangerThings.end(), CDangerThingIs(thing));
    if (entry == DangerThings.end())
        DangerThings.push_back(CDangerThing(thing, status));
    else {
        entry->Status = status;
        entry->Timer = 0;
    }
    if (ParentThing->GetPlayer()->IsMainPlayer() && ParentThing->m_thingType == 2 && InDangerCoolOffTimer < 0) {
        CWorld* world = GetWorld();
        world->PMusicManager->EnableDangerMusic(true);
        CWorld* world2 = GetWorld();
        world2->PEnvironment->SetRainEnabled(false);
    }
    InDangerCoolOffTimer = (long)(PDef ? PDef->DangerFeedbackHangtimeSeconds * g_framesPerSecond : 1.0f);
}
