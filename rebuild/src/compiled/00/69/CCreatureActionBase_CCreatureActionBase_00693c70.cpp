// CCreatureActionBase::CCreatureActionBase(CThingCreatureBase*, const CSoundPair&, long, long) -- the base creature-action constructor: attach the creature, one-frame total/action time, the interruption group and priority, the sound pair, no pause (-1 frame/timer), empty containers, a fresh default animation entry, and the next unique action id
// Leaf: ??0CCreatureActionBase
// Retail: Fable.exe (TLC, VC7.1) @ 0x00693C70, 321 bytes (336 with padding). `ret 10h`; vtable 0x01259D24.
// Callees: CBaseObject::CBaseObject 0x0099A380; CBaseIntelligentPointer ctor 0x00A01B10 and SetPItem 0x00A01B90
//          (CIntelligentPointer<CThingCreatureBase> vtable 0x0123E7D8, CIntelligentPointer<CThing> 0x01238C6C);
//          malloc 0x00BFEA0E (the map header node); operator new 0x00BFEA1A; CAnimationEntry(long, long, long, long)
//          0x005DED00; CCountedPointer<CAnimationEntry>::Reset 0x00696830 (manifest label names another instantiation).
//          Global: the action-id counter 0x013BA8B4.
//
// Shape: init list in declaration order (AnimMovementStep / NextMovementPos are left unconstructed by an empty
// C3DVector ctor); the STLport map ctor is inline (header proxy zeroed, header node malloc'd, red with null
// parent and self links); the body resets the animation entry to a new CAnimationEntry(0, 0, 0, 2) and takes the
// next id with `ActionID = NextActionID; ++NextActionID;` (the store may alias the global, so it is re-read).
// The two vectors are STLport `_Vector_base` + `_STLP_alloc_proxy` models: with plain zeroing ctors the operator
// new size push is scheduled above DependentThings' first two zero stores.
// Layout: local model (as the copy constructor 0x0062EBC0): engine/CCreatureActionBase.h agrees up to +0x80, the
// retail containers are 12 bytes (ActionEvents +0x84, CutSceneEvents +0x90, DependentThings +0x9c).
struct CThing;
struct CThingCreatureBase;
struct CAnimationEntry {
    unsigned char _body[0x10];
    CAnimationEntry(long a, long b, long c, long type);    // 0x005DED00
};
extern "C" void* __cdecl malloc(unsigned int size);       // 0x00BFEA0E

struct CBaseObjectPointer;
struct CBaseObject {                        // vtable 0x0129A7D8
    virtual void v00();
    CBaseObjectPointer* IntelligentPointer; // +0x04
    CBaseObject();                          // 0x0099A380 (manifest label CBaseIntelligentPointer is propagated)
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
    CCreaturePointer(CThingCreatureBase* item) : CBaseIntelligentPointer() { SetPItem((CThing*)item); }
};
struct CThingPointer : CBaseIntelligentPointer {        // CIntelligentPointer<CThing>, vtable 0x01238C6C
    virtual void v00();
    CThingPointer() : CBaseIntelligentPointer() {}
};
struct CCPPointerInfo {
    long Count;
};
struct CAnimationEntryPointer {             // CCountedPointer<CAnimationEntry>
    CAnimationEntry* Data;
    CCPPointerInfo*  Info;
    CAnimationEntryPointer() : Data(0), Info(0) {}
    void Reset(CAnimationEntry* entry);     // 0x00696830 (release the old entry, take the new one)
};
struct CSoundPair {
    long FirstSound;
    long LastSound;
};
struct C3DVector {
    float X, Y, Z;
    C3DVector() {}
};
template <class T> struct CAllocator {     // STLport allocator<T>: empty
    CAllocator() { }
};
template <class T> struct CAllocProxy : CAllocator<T> {    // STLport _STLP_alloc_proxy<T*, T, allocator<T> >
    T* m_data;
    CAllocProxy(const CAllocator<T>& alloc, T* p) : CAllocator<T>(alloc), m_data(p) { }
};
template <class T> struct CVectorBase {    // STLport _Vector_base<T, allocator<T> >
    T*             _M_start;
    T*             _M_finish;
    CAllocProxy<T> _M_end_of_storage;
    CVectorBase(const CAllocator<T>& alloc) : _M_start(0), _M_finish(0), _M_end_of_storage(alloc, 0) { }
};
template <class T> struct CVector : CVectorBase<T> {   // STLport vector<T> (12 bytes)
    CVector() : CVectorBase<T>(CAllocator<T>()) { }
};
struct CActionEventPointer { void* Data; void* Info; };
struct _Rb_tree_node_base {
    char                _M_color;           // +0
    _Rb_tree_node_base* _M_parent;          // +4
    _Rb_tree_node_base* _M_left;            // +8
    _Rb_tree_node_base* _M_right;           // +0xc
};
struct SCutSceneEventNode : _Rb_tree_node_base {    // 0x18 bytes
    long  first;
    void* second;                           // CCharString
};
struct CCutSceneEventMap {                  // map<long, CCharString> (STLport, 12 bytes)
    SCutSceneEventNode* _M_header;
    unsigned long       _M_node_count;
    unsigned char       _M_compare;
    CCutSceneEventMap()
        : _M_header(0)
    {
        _M_header = (SCutSceneEventNode*)malloc(sizeof(SCutSceneEventNode));
        _M_node_count = 0;
        _M_header->_M_color = 0;            // red
        _M_header->_M_parent = 0;
        _M_header->_M_left = _M_header;
        _M_header->_M_right = _M_header;
    }
};

struct CCreatureActionBase : CBaseObject {  // vtable 0x01259D24
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
    CVector<CActionEventPointer> ActionEvents;                // +0x84
    CCutSceneEventMap      CutSceneEvents;              // +0x90
    CVector<CThingPointer> DependentThings;             // +0x9c
    virtual void v00();
    static long NextActionID;               // 0x013BA8B4
    CCreatureActionBase(CThingCreatureBase* creature, const CSoundPair& sounds, long interruptionGroupID, long interruptionPriority);
};

CCreatureActionBase::CCreatureActionBase(CThingCreatureBase* creature, const CSoundPair& sounds, long interruptionGroupID, long interruptionPriority)
    : PCreature(creature)
    , TotalTime(1)
    , ActionTime(1)
    , AnimDelayTime(0)
    , FramesRunning(0)
    , InterruptionGroupID(interruptionGroupID)
    , InterruptionPriority(interruptionPriority)
    , ActionID(0)
    , Sounds(sounds)
    , PlayingSoundID(0)
    , RunningTimeSeconds(0.0f)
    , LastRunningTimeSeconds(0.0f)
    , StartFrame(0)
    , PauseFrame(-1)
    , PauseTimer(-1)
    , AddedActionAnim(false)
    , Finished(false)
    , PerformedAction(false)
    , Initialised(false)
    , NextMovementPosWorldFrame(0)
{
    PAnimEntry.Reset(new CAnimationEntry(0, 0, 0, 2));
    ActionID = NextActionID;
    ++NextActionID;
}