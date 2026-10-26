// CAIStateGroup_GoToPub::InitStateGroup(const CWorld&, CThingAICreature&) -- reset the pub visit state: frames, seat/bar indices, flags and the tankard pointer
// Leaf: ?InitStateGroup@CAIStateGroup_GoToPub@@MAEXABVCWorld@@AAVCThingAICreature@@@Z
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00947DB0, length 110 bytes (+ int3 padding). Vtable slot 13
// of CAIStateGroup_GoToPub (0x0128BEC4).
// Callees: CBaseIntelligentPointer::CBaseIntelligentPointer @ 0x00A01B10, ::SetPItem @ 0x00A01B90,
// ::GetPItem @ 0x00A01B50, ::~CBaseIntelligentPointer @ 0x00A01C10. Vtable 0x01238C6C =
// CIntelligentPointer<CThing> (rtti-vtables.tsv).
//
// Shape: body assignments in source order with ebx as the shared zero register (NextFramePub +0x68,
// Sitting +0x70, ActionPointIndex +0x64 = -1), then `PTankard = CIntelligentPointer<CThing>(0)` (stack
// temporary: base ctor, push 0, vtable, SetPItem; copied with SetPItem(GetPItem()); base destructor), then
// HandsOffMode +0xa0, TimeBeforeAskingForBeer +0xa4, BarActionPointIndex +0x74 and SeatIndex +0x3c.
// Members are the PDB names (struct_layouts_egor.tsv, sizeof 0xA8); no engine header exists for the class.
class CThing;
class CThingObject;
class CThingBuilding;
class CThingAICreature;
class CWorld;

struct CBaseIntelligentPointer {
    virtual void v00();
    void* m_pItem;                              // +0x04
    CBaseIntelligentPointer();                  // 0x00A01B10
    ~CBaseIntelligentPointer();                 // 0x00A01C10
    void* GetPItem() const;                     // 0x00A01B50
    void SetPItem(void* item);                  // 0x00A01B90
};

template<class T> class CIntelligentPointer : public CBaseIntelligentPointer { public:
    virtual void v00();                         // one vtable per instantiation
    CIntelligentPointer(T* item) : CBaseIntelligentPointer() { SetPItem(item); }
    CIntelligentPointer& operator=(const CIntelligentPointer& other)
    {
        SetPItem(other.GetPItem());
        return *this;
    }
};

class C3DVector { public: float X, Y, Z; };
class CRightHandedSet { public: C3DVector Up; C3DVector Forward; };

class CAIStateGroupBase { public:
    virtual ~CAIStateGroupBase();
    void* PBrain;                               // +0x04
    long ScriptAIPriority;                      // +0x08
    void* Name_PStringData;                     // +0x0c
    unsigned char Terminated;                   // +0x10
    unsigned char Priority;                     // +0x11
    unsigned char Enabled;                      // +0x12
    unsigned char _pad_0x13;
protected:
    virtual void InitStateGroup(const CWorld& world, CThingAICreature& creature);
};

struct CAIStateGroup_GoToPub : CAIStateGroupBase {
    CIntelligentPointer<CThingBuilding> PPub;       // +0x14
    CIntelligentPointer<CThingObject> PBar;         // +0x1c
    CIntelligentPointer<CThingObject> PTable;       // +0x24
    CIntelligentPointer<CThingAICreature> PBarman;  // +0x2c
    CIntelligentPointer<CThing> PTankard;           // +0x34
    long SeatIndex;                             // +0x3c
    C3DVector Pos;                              // +0x40
    CRightHandedSet RHSet;                      // +0x4c
    long ActionPointIndex;                      // +0x64
    long NextFramePub;                          // +0x68
    long FrameLeavePub;                         // +0x6c
    bool Sitting;                               // +0x70
    long BarActionPointIndex;                   // +0x74
    C3DVector BarPos;                           // +0x78
    CRightHandedSet BarRHSet;                   // +0x84
    long BarIndex;                              // +0x9c
    bool HandsOffMode;                          // +0xa0
    bool UnderScriptControl;                    // +0xa1
    long TimeBeforeAskingForBeer;               // +0xa4
protected:
    virtual void InitStateGroup(const CWorld& world, CThingAICreature& creature);
};

void CAIStateGroup_GoToPub::InitStateGroup(const CWorld& world, CThingAICreature& creature)
{
    NextFramePub = 0;
    Sitting = false;
    ActionPointIndex = -1;
    PTankard = CIntelligentPointer<CThing>(0);
    HandsOffMode = false;
    TimeBeforeAskingForBeer = 0;
    BarActionPointIndex = -1;
    SeatIndex = -1;
}