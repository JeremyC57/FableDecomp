// CAIStateGroup_CreatureSeekEnemyUnderground::OnActivate() -- forgets the enemy, remembers where the creature went underground, then runs the idle-underground activation
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00920990, length 103 bytes (+ int3 padding).
// Callees: CBaseIntelligentPointer::CBaseIntelligentPointer @ 0x00A01B10, ::SetPItem @ 0x00A01B90 (twice),
// ::GetPItem @ 0x00A01B50, ::~CBaseIntelligentPointer @ 0x00A01C10, CThing::GetPosition @ 0x004C73D0,
// CAIStateGroup_CreatureIdleUnderground::OnActivate @ 0x009208E0 (the base implementation, called non-virtually).
// Shape: `PEnemy = CIntelligentPointer<const CThing>(0)` -- a frame temporary built by the inline converting
// constructor (base ctor, vtable 0x01237F1C, SetPItem(0)), assigned through the inline operator=
// (SetPItem(other.GetPItem())), destroyed through the base destructor; RisePos is a plain POD C3DVector assigned
// with the IMPLICIT assignment (`lea ecx,[esi+1Ch]` then three dword moves through ecx, edx reused for X and Y);
// a user memberwise operator= (inline or __forceinline) or a pointer local folds the stores into [esi+1Ch..24h].
// Model: local (no engine header for this class; it derives from CAIStateGroup_CreatureIdleUnderground).
struct CThing;

struct C3DVector {
    float X, Y, Z;
};

struct CThingCreature {
    const C3DVector* GetPosition() const;           // 0x004C73D0 (CThing)
};

struct CBaseIntelligentPointer {
    virtual void v00();
    void* m_pItem;                                  // +0x04
    CBaseIntelligentPointer();                      // 0x00A01B10
    ~CBaseIntelligentPointer();                     // 0x00A01C10
    CThing* GetPItem() const;                       // 0x00A01B50
    void SetPItem(CThing* item);                    // 0x00A01B90
};

struct CIntelligentPointerThing : CBaseIntelligentPointer {      // CIntelligentPointer<const CThing>, vtable 0x01237F1C
    virtual void v00();
    CIntelligentPointerThing(CThing* item) : CBaseIntelligentPointer() { SetPItem(item); }
    CIntelligentPointerThing& operator=(const CIntelligentPointerThing& other) { SetPItem(other.GetPItem()); return *this; }
};

struct CAIBrain {
    unsigned char _pad[0x18];
    CThingCreature* Creature;                       // +0x18
};

struct CAIStateGroupBase {
    virtual ~CAIStateGroupBase();
    CAIBrain* PBrain;                               // +0x04
    long ScriptAIPriority;                          // +0x08
    void* Name_PStringData;                         // +0x0c
    unsigned char Terminated;                       // +0x10
    unsigned char Priority;                         // +0x11
    unsigned char Enabled;                          // +0x12
    unsigned char _pad_0x13;
};

struct CAIStateGroup_CreatureIdleUnderground : CAIStateGroupBase {
    virtual void OnActivate();                      // 0x009208E0
};

struct CAIStateGroup_CreatureSeekEnemyUnderground : CAIStateGroup_CreatureIdleUnderground {
    CIntelligentPointerThing PEnemy;               // +0x14
    C3DVector RisePos;                   // +0x1c
    virtual void OnActivate();
};

void CAIStateGroup_CreatureSeekEnemyUnderground::OnActivate()
{
    PEnemy = CIntelligentPointerThing(0);
    RisePos = *PBrain->Creature->GetPosition();
    CAIStateGroup_CreatureIdleUnderground::OnActivate();
}