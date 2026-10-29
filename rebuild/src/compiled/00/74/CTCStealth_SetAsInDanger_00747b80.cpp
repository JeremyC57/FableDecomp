// CTCStealth::SetAsInDanger(const CThing* thing, long status) -- record (or refresh) the danger status of a thing; for the main player's creature entering danger start the danger music and stop the rain; restart the in-danger cool-off timer
// Leaf: SetAsInDanger
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00747B80, 238 bytes (`ret 8`).
// Callees: SGI __find_if over the danger list @ 0x007485F0 (__fastcall: first, last, the by-value thing predicate and the
// iterator tag by reference; compiled in this TU, see the body below), CBaseIntelligentPointer ctor
// @ 0x00A01B10 / SetPItem @ 0x00A01B90 / dtor @ 0x00A01C10 (the pair temporary's CIntelligentPointer<const CThing>, vtable
// 0x01237F1C), vector<pair<CIntelligentPointer<const CThing>, CDangerStatus> >::push_back @ 0x00749620, CThing::GetPlayer const
// @ 0x004C7A30, CPlayer::IsMainPlayer @ 0x00487520, CTCBase::GetWorld @ 0x00686A70 (twice), CMusicManager::EnableDangerMusic(bool)
// @ 0x006BDE50, CEnvironment::SetRainEnabled(bool) @ 0x006B32B0, __ftol2 @ 0x00BFEA70; g_framesPerSecond @ 0x01375550.
// Layout: as the landed ClearDangerStatus 0x00747090 / SetAsRunningAwayFromCombat 0x007470E0 (PDef +0x1c, InDangerCoolOffTimer
// +0x34, DangerThings +0x38/+0x3c of 16-byte {CIntelligentPointer, Status, Timer}); CStealthDef DangerFeedbackHangtimeSeconds +0x40.
// Shape: a miss pushes a pair temporary (pointer built in place, then the status and a zero timer), a hit refreshes the
// entry in place; the cool-off timer is `(long)(PDef ? seconds * fps : 1.0f)` (its __ftol2 tail duplicated per arm).
//        The SGI __find_if instantiation is defined in this TU (noinline, its real unrolled body): with its body visible VC7.1
//        homes the empty iterator-tag temporary in the still-live `status` argument slot as retail; an extern declaration
//        puts it in the `thing` slot instead.
class CThing;
class CBaseObject;
extern long g_framesPerSecond;                          // 0x01375550
class CPlayer { public:
    bool IsMainPlayer() const;                          // 0x00487520
};
class CMusicManager { public:
    void EnableDangerMusic(bool enable);                // 0x006BDE50
};
class CEnvironment { public:
    void SetRainEnabled(bool enabled);                  // 0x006B32B0
};
class CWorld { public:
    unsigned char  _pad00[0x18];
    CEnvironment*  PEnvironment;                        // +0x18 scoped_ptr<CEnvironment>
    unsigned char  _pad1c[0x6c - 0x1c];
    CMusicManager* PMusicManager;                       // +0x6c CCountedPointer<CMusicManager> (object word)
};
class CThing {
public:
    unsigned char _pad[0x10];
    unsigned char m_thingType;                          // +0x10 (2 = creature)
public:
    CPlayer& GetPlayer();   // 0x004C7A30
};
struct CBaseIntelligentPointer {
    virtual void v00();
    CThing* m_pItem;                                    // +0x04
    CBaseIntelligentPointer();                          // 0x00A01B10
    virtual ~CBaseIntelligentPointer();                         // 0x00A01C10
    CBaseObject* GetPItem() const;                           // 0x00A01B50
    void SetPItem(CBaseObject* item);                   // 0x00A01B90
};
template <class T> class CIntelligentPointer : public CBaseIntelligentPointer { public:   // vtable 0x01237F1C (const CThing)
    virtual void v00();
    CIntelligentPointer(T* item) : CBaseIntelligentPointer() { SetPItem((CBaseObject*)item); }
};
struct CDangerStatus {                                  // NStealth::CDangerStatus (PDB, 8 bytes)
    long DangerStatus;                                  // +0x00 EDangerStatus
    long Timer;                                         // +0x04
    CDangerStatus(long status) : DangerStatus(status), Timer(0) { }
};
struct CDangerThing {                                   // pair<CIntelligentPointer<const CThing>, NStealth::CDangerStatus>, 16 bytes
    CIntelligentPointer<const CThing> first;            // +0x00
    CDangerStatus second;                               // +0x08
    CDangerThing(const CThing* thing, CDangerStatus status) : first(thing), second(status) { }
};
struct CDangerThingIs {                                 // the find predicate: the entry's thing is the given one
    const CThing* Thing;
    CDangerThingIs(const CThing* thing) : Thing(thing) { }
    bool operator()(const CDangerThing& entry) const;
};
struct random_access_iterator_tag { random_access_iterator_tag() { } };
struct CDangerThingVector {                             // STLport vector (12 bytes)
    CDangerThing* _M_start;
    CDangerThing* _M_finish;
    CDangerThing* _M_end_of_storage;
    CDangerThing* begin() { return _M_start; }
    CDangerThing* end() { return _M_finish; }
    void push_back(const CDangerThing& value);          // 0x00749620
};
inline bool CDangerThingIs::operator()(const CDangerThing& entry) const { return ((CThing*)entry.first.GetPItem()) == Thing; }
#pragma auto_inline(off)
__declspec(noinline) CDangerThing* __fastcall __find_if(CDangerThing* first, CDangerThing* last, CDangerThingIs pred,
                                                        const random_access_iterator_tag&)   // 0x007485F0
{
    long tripCount = (last - first) >> 2;
    for (; tripCount > 0; --tripCount) {
        if (pred(*first)) return first;
        ++first;
        if (pred(*first)) return first;
        ++first;
        if (pred(*first)) return first;
        ++first;
        if (pred(*first)) return first;
        ++first;
    }
    switch (last - first) {
    case 3:
        if (pred(*first)) return first;
        ++first;
    case 2:
        if (pred(*first)) return first;
        ++first;
    case 1:
        if (pred(*first)) return first;
    case 0:
    default:
        return last;
    }
}
#pragma auto_inline(on)
inline CDangerThing* find_if(CDangerThing* first, CDangerThing* last, CDangerThingIs pred)
{
    return __find_if(first, last, pred, random_access_iterator_tag());
}
class CStealthDef { public:
    unsigned char _pad[0x40];
    float DangerFeedbackHangtimeSeconds;                // +0x40
};
class CTCBase
{
protected:
    CWorld& GetWorld() const;   // 0x00686A70
};

class CTCStealth : public CTCBase { public:
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
    void SetAsInDanger(const CThing* thing, long status);
};

void CTCStealth::SetAsInDanger(const CThing* thing, long status)
{
    CDangerThing* entry = find_if(DangerThings.begin(), DangerThings.end(), CDangerThingIs(thing));
    if (entry == DangerThings.end())
        DangerThings.push_back(CDangerThing(thing, CDangerStatus(status)));
    else {
        entry->second = CDangerStatus(status);
    }
    if (ParentThing->GetPlayer().IsMainPlayer() && ParentThing->m_thingType == 2 && InDangerCoolOffTimer < 0) {
        CWorld* world = (&GetWorld());
        world->PMusicManager->EnableDangerMusic(true);
        CWorld* world2 = (&GetWorld());
        world2->PEnvironment->SetRainEnabled(false);
    }
    InDangerCoolOffTimer = (long)(PDef ? PDef->DangerFeedbackHangtimeSeconds * g_framesPerSecond : 1.0f);
}