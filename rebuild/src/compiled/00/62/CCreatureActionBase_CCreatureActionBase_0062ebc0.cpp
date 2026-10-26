// CCreatureActionBase::CCreatureActionBase(const CCreatureActionBase&) -- the member-wise copy constructor: CBaseObject copy (detached), own vtable, the two intelligent pointers re-attached to the source's items, the plain fields copied, the two counted animation-entry pointers add-ref'd, and the three containers copy-constructed
// Leaf: ??0CCreatureActionBase
// Retail: Fable.exe (TLC, VC7.1) @ 0x0062EBC0, 345 bytes (352 with padding). `ret 4`; vtable 0x01259D24.
// Callees: CBaseObject::CBaseObject(const CBaseObject&) 0x0099A390; CBaseIntelligentPointer ctor 0x00A01B10,
//          GetPItem 0x00A01B50, SetPItem 0x00A01B90 (CIntelligentPointer<CThingCreatureBase> vtable 0x0123E7D8,
//          CIntelligentPointer<CThing> vtable 0x01238C6C); the STLport copy constructors of ActionEvents 0x0062EDD0,
//          CutSceneEvents 0x0062EE80 and DependentThings 0x0062ED40.
//
// Shape: an explicit init list in declaration order reproduces the implicit copy: each intelligent pointer's
// inline copy constructor is `CBaseIntelligentPointer()` + vtable store + `SetPItem(other.GetPItem())`; a
// CCountedPointer copy is two dword copies with `if (Info) ++Info->Count` on the info block. C3DVector needs a
// user member-wise copy constructor: the implicit one copies through `lea` base registers.
// Layout: local model. engine/CCreatureActionBase.h (Ego_r PDB, 0xb4 bytes) agrees up to PPreceedingPAnimEntry
// (+0x80); the PDB's 16-byte containers are 12 bytes in retail (the copy constructor calls take +0x84, +0x90 and
// +0x9c), so ActionEvents +0x84, CutSceneEvents +0x90, DependentThings +0x9c and the class is 0xa8 bytes.
class CThing;
class CThingCreatureBase;
class CAnimationEntry;

struct CBaseObjectPointer;
class CBaseObject { public:                        // vtable 0x0129A7D8
    CBaseObjectPointer* IntelligentPointer; // +0x04
protected:
    CBaseObject(const CBaseObject& other);               // 0x0099A390
    virtual ~CBaseObject();                              // 0x0099A430
};
struct CBaseIntelligentPointer {
    virtual void v00();
    CThing* PItem;                          // +0x04
    CBaseIntelligentPointer();              // 0x00A01B10
    CThing* GetPItem() const;               // 0x00A01B50
    void SetPItem(CThing* item);            // 0x00A01B90
};
struct CCreaturePointer : CBaseIntelligentPointer {     // CIntelligentPointer<CThingCreatureBase>, vtable 0x0123E7D8
    virtual void v00();
    CCreaturePointer(const CCreaturePointer& other) : CBaseIntelligentPointer() { SetPItem(other.GetPItem()); }
};
struct CThingPointer : CBaseIntelligentPointer {        // CIntelligentPointer<CThing>, vtable 0x01238C6C
    virtual void v00();
    CThingPointer(const CThingPointer& other) : CBaseIntelligentPointer() { SetPItem(other.GetPItem()); }
};
class CCPPointerInfo { public:
    long Count;
};
struct CAnimationEntryPointer {             // CCountedPointer<CAnimationEntry>
    CAnimationEntry* Data;
    CCPPointerInfo*  Info;
    CAnimationEntryPointer(const CAnimationEntryPointer& other) : Data(other.Data), Info(other.Info)
    {
        if (Info)
            ++Info->Count;
    }
};
class CSoundPair { public:
    long FirstSound;
    long LastSound;
};
class C3DVector { public:
    float X, Y, Z;
    C3DVector(const C3DVector& v) : X(v.X), Y(v.Y), Z(v.Z) {}
};
struct CActionEventVector {                 // vector<CCountedPointer<NCreatureAction::CActionEventBase>> (12 bytes)
    void* _M_start;
    void* _M_finish;
    void* _M_end_of_storage;
    CActionEventVector(const CActionEventVector& other);    // 0x0062EDD0
};
struct CCutSceneEventMap {                  // map<long, CCharString> (12 bytes)
    void*         _M_header;
    unsigned long _M_node_count;
    unsigned char _M_compare;
    CCutSceneEventMap(const CCutSceneEventMap& other);      // 0x0062EE80
};
struct CDependentThingVector {              // vector<CIntelligentPointer<CThing>> (12 bytes)
    void* _M_start;
    void* _M_finish;
    void* _M_end_of_storage;
    CDependentThingVector(const CDependentThingVector& other);   // 0x0062ED40
};

class CCreatureActionBase : public CBaseObject { public:  // vtable 0x01259D24
    CCreaturePointer       PCreature;                   // +0x08
    long                   TotalTime;                   // +0x10
    long                   ActionTime;                  // +0x14
    long                   AnimDelayTime;               // +0x18
    long                   FramesRunning;               // +0x1c
    long                   InterruptionGroupID;         // +0x20
    long                   InterruptionPriority;        // +0x24
    long                   ActionID;                    // +0x28
    CThingPointer          PItemRemoved;                // +0x2c
    CSoundPair             Sounds;                      // +0x34
    unsigned long          PlayingSoundID;              // +0x3c
    float                  RunningTimeSeconds;          // +0x40
    float                  LastRunningTimeSeconds;      // +0x44
    long                   StartFrame;                  // +0x48
    long                   PauseFrame;                  // +0x4c
    long                   PauseTimer;                  // +0x50
    C3DVector              AnimMovementStep;            // +0x54
    bool                   AddedActionAnim;             // +0x60
    bool                   Finished;                    // +0x61
    bool                   PerformedAction;             // +0x62
    bool                   Initialised;                 // +0x63
    long                   NextMovementPosWorldFrame;   // +0x64
    C3DVector              NextMovementPos;             // +0x68
    CAnimationEntryPointer PAnimEntry;                  // +0x74
    CAnimationEntryPointer PPreceedingPAnimEntry;       // +0x7c
    CActionEventVector     ActionEvents;                // +0x84
    CCutSceneEventMap      CutSceneEvents;              // +0x90
    CDependentThingVector  DependentThings;             // +0x9c
    virtual void v00();
    CCreatureActionBase(const CCreatureActionBase& other);
};

CCreatureActionBase::CCreatureActionBase(const CCreatureActionBase& other)
    : CBaseObject(other)
    , PCreature(other.PCreature)
    , TotalTime(other.TotalTime)
    , ActionTime(other.ActionTime)
    , AnimDelayTime(other.AnimDelayTime)
    , FramesRunning(other.FramesRunning)
    , InterruptionGroupID(other.InterruptionGroupID)
    , InterruptionPriority(other.InterruptionPriority)
    , ActionID(other.ActionID)
    , PItemRemoved(other.PItemRemoved)
    , Sounds(other.Sounds)
    , PlayingSoundID(other.PlayingSoundID)
    , RunningTimeSeconds(other.RunningTimeSeconds)
    , LastRunningTimeSeconds(other.LastRunningTimeSeconds)
    , StartFrame(other.StartFrame)
    , PauseFrame(other.PauseFrame)
    , PauseTimer(other.PauseTimer)
    , AnimMovementStep(other.AnimMovementStep)
    , AddedActionAnim(other.AddedActionAnim)
    , Finished(other.Finished)
    , PerformedAction(other.PerformedAction)
    , Initialised(other.Initialised)
    , NextMovementPosWorldFrame(other.NextMovementPosWorldFrame)
    , NextMovementPos(other.NextMovementPos)
    , PAnimEntry(other.PAnimEntry)
    , PPreceedingPAnimEntry(other.PPreceedingPAnimEntry)
    , ActionEvents(other.ActionEvents)
    , CutSceneEvents(other.CutSceneEvents)
    , DependentThings(other.DependentThings)
{
}