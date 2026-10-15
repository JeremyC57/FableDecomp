// CAIStateGroup_DeedReactionRoot::InitStateGroup(const CWorld& world, CThingAICreature& creature) -- resets the reaction state: no reaction/speech delivered, no thing reacting to, no current or last deed
// Retail: Fable.exe (TLC, VC7.1) function @ 0x0095FE60, length 98 bytes (+ int3 padding), `ret 8` (both references unused).
// Callees: CBaseIntelligentPointer::CBaseIntelligentPointer @ 0x00A01B10, ::SetPItem @ 0x00A01B90 (twice),
// ::GetPItem @ 0x00A01B50, ::~CBaseIntelligentPointer @ 0x00A01C10.
// Shape: one zero register (ebx) for every zero store and the null pointer argument; the two delivered flags
// are cleared before `ThingReactingTo = CIntelligentPointer<const CThing>(0)` (frame temporary through the inline
// converting constructor -- vtable 0x01237F1C, SetPItem(0) -- the inline operator= and the base destructor);
// the scalar resets follow in source order with the -1 stores (`or eax,-1`) sunk to the end.
// Model: local (no engine header for this class; member names from struct_layouts_egor.tsv).
struct CThing;
class CWorld;
class CThingAICreature;

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

struct CAIStateGroupBase {
    virtual ~CAIStateGroupBase();
    void* PBrain;                                   // +0x04
    long ScriptAIPriority;                          // +0x08
    void* Name_PStringData;                         // +0x0c
    unsigned char Terminated;                       // +0x10
    unsigned char Priority;                         // +0x11
    unsigned char Enabled;                          // +0x12
    unsigned char _pad_0x13;
};

struct CAIStateGroup_DeedReactionRoot : CAIStateGroupBase {
    CIntelligentPointerThing ThingReactingTo;       // +0x14
    long CurrentDeedType;                           // +0x1c EOpinionDeedType
    long CurrentDeedHandle;                         // +0x20
    bool DeedIsVirtual;                             // +0x24
    unsigned __int64 LastThingOpinionSourceUID;     // +0x28
    long LastOpinionSourceVirtualDeedID;            // +0x30
    bool DeliveredReaction;                         // +0x34
    bool DeliveredSpeech;                           // +0x35
    virtual void InitStateGroup(const CWorld& world, CThingAICreature& creature);
};

void CAIStateGroup_DeedReactionRoot::InitStateGroup(const CWorld& world, CThingAICreature& creature)
{
    DeliveredReaction = false;
    DeliveredSpeech = false;
    ThingReactingTo = CIntelligentPointerThing(0);
    CurrentDeedType = 0;
    CurrentDeedHandle = -1;
    DeedIsVirtual = false;
    LastThingOpinionSourceUID = 0;
    LastOpinionSourceVirtualDeedID = -1;
}