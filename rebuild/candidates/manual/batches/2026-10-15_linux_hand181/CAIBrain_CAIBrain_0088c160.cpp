// CAIBrain::CAIBrain(CMainGameComponent*, CWorld*, CThingManager*, CPlayerManager*, CThingAICreature*, CDefPointer<const CBrainDef>) -- construct the creature brain: base (0, 32000, 0.1f), cached managers, empty state-group map, output and update zones; then init the state groups from the def and copy its update zones
// Leaf: ??0CAIBrain
// Retail: Fable.exe (TLC, VC7.1) function @ 0x0088C160, length 218 bytes (`ret 18h`). Vtable 0x012780C4.
// Callees: the brain base ctor @ 0x00A44740 (int, int, float; manifest "CCreatureAction_CastBattleCharge" ctor label is
// propagated), CBrainOutput::CBrainOutput @ 0x00835620 (Output +0x44), CAIBrain::InitStateGroups(CDefPointer<const
// CBrainDef>) @ 0x0088BF30 (by-value def copy built in the argument area), vector<CBrainUpdateZone>::operator= @ 0x00462CB0
// (manifest "CActiveFile_CopyAssign" is propagated) from the def's zones (+0x54).
// Shape: the two vectors use the STLport `_Vector_base` + `_STLP_alloc_proxy` model (plain zeroing members reorder the
// argument loads and the Output `lea`); one `xor ebx,ebx` zero for every zero store; the parameter def is released inline at the end (`dec [ebp+4] ;
// jne ; call [vtbl+4]`). Retail layout is the PDB's minus 4 bytes from +0x34 (12-byte vector + compare byte + flag byte
// in the state-group map): PActiveStateGroup +0x38, Output +0x44, OutputValid +0x8c, PBrainDef +0x90, UpdateZones +0x94,
// PCurrentUpdateZone +0xa0, OnKillCalled +0xa4.
struct CMainGameComponent;
struct CWorld;
struct CThingManager;
struct CPlayerManager;
struct CThingAICreature;
struct CAIStateGroupBase;
struct CBrainUpdateZone;

template <class T> struct _STLP_alloc_proxy        // STLport allocator proxy holding end_of_storage
{
    T* _M_data;
    _STLP_alloc_proxy(T* p) : _M_data(p) { }
};

template <class T> struct _Vector_base
{
    T* _M_start;
    T* _M_finish;
    _STLP_alloc_proxy<T> _M_end_of_storage;
    _Vector_base() : _M_start(0), _M_finish(0), _M_end_of_storage(0) { }
};

struct CBrainUpdateZoneVector : _Vector_base<CBrainUpdateZone>   // vector<CBrainUpdateZone>
{
    CBrainUpdateZoneVector& operator=(const CBrainUpdateZoneVector& other);   // 0x00462CB0
};

struct CBrainDef
{
    virtual void v00();
    virtual void Destroy() const;                   // slot 1 (+0x04)
    mutable long RefCount;                          // +0x04
    unsigned char _pad08[0x54 - 0x08];
    CBrainUpdateZoneVector UpdateZones;             // +0x54
};

template <class T>
struct CDefPointer
{
    T* Object;
    CDefPointer(const CDefPointer& other) : Object(other.Object) { if (Object) ++Object->RefCount; }
    ~CDefPointer() { if (Object && --Object->RefCount == 0) Object->Destroy(); }
    T* operator->() const { return Object; }
};

struct CStateGroupMap                               // CVectorMap<long, CAIStateGroupBase*, CKeyPairCompareGreater<..> >
{
    _Vector_base<CAIStateGroupBase*> Entries;       // +0x00 vector of pairs
    unsigned char Compare;                          // +0x0c
    bool Sorted;                                    // +0x0d
    CStateGroupMap() : Sorted(false) { }
};

struct CBrainOutput                                 // 0x48 bytes
{
    unsigned char _data[0x48];
    CBrainOutput();                                 // 0x00835620
};

struct CAIBrainBase                                 // the brain's base class (0x14 bytes)
{
    virtual void v00();
    unsigned char _base[0x10];
    CAIBrainBase(long a, long b, float c);          // 0x00A44740
};

struct CAIBrain : CAIBrainBase
{
    CMainGameComponent* Component;                  // +0x14
    CThingAICreature* Creature;                     // +0x18
    CThingManager* ThingManager;                    // +0x1c
    CPlayerManager* PlayerManager;                  // +0x20
    CWorld* World;                                  // +0x24
    CStateGroupMap StateGroupList;                  // +0x28
    CAIStateGroupBase* PActiveStateGroup;           // +0x38
    CAIStateGroupBase* PLastStateGroup;             // +0x3c
    CAIStateGroupBase* PPendingStateGroup;          // +0x40
    CBrainOutput Output;                            // +0x44
    bool OutputValid;                               // +0x8c
    bool UpdatingStateGroup;                        // +0x8d
    bool StateGroupUpdated;                         // +0x8e
    bool InitialisedCreaturePosition;               // +0x8f
    CDefPointer<const CBrainDef> PBrainDef;         // +0x90
    CBrainUpdateZoneVector UpdateZones;             // +0x94
    CBrainUpdateZone* PCurrentUpdateZone;           // +0xa0
    bool OnKillCalled;                              // +0xa4

    CAIBrain(CMainGameComponent* component, CWorld* world, CThingManager* thingManager,
             CPlayerManager* playerManager, CThingAICreature* creature, CDefPointer<const CBrainDef> def);
    virtual void v00();
    void InitStateGroups(CDefPointer<const CBrainDef> def);   // 0x0088BF30
};

CAIBrain::CAIBrain(CMainGameComponent* component, CWorld* world, CThingManager* thingManager,
                   CPlayerManager* playerManager, CThingAICreature* creature, CDefPointer<const CBrainDef> def)
    : CAIBrainBase(0, 32000, 0.1f),
      Component(component), Creature(creature), ThingManager(thingManager), PlayerManager(playerManager),
      World(world), PBrainDef(def), OnKillCalled(false)
{
    InitStateGroups(PBrainDef);
    UpdateZones = def->UpdateZones;
    PActiveStateGroup = 0;
    PLastStateGroup = 0;
    PPendingStateGroup = 0;
    PCurrentUpdateZone = 0;
    OutputValid = false;
    UpdatingStateGroup = false;
    InitialisedCreaturePosition = true;
}
