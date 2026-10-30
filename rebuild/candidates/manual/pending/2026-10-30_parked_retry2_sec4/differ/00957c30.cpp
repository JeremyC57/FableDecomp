// CAIStateGroup_MinionBehaviourCrowdBase::IsRunnable() -- runnable when the creature perceives a thing with a non-zero perception value, a hero-podium "crowd" message (type 0x23) exists and the group is ready: latch the thing, and unless already in the crowd, take a crowd position from the message originator's podium
// Grade: DIFFER(ours v retail) -- 373v384 (retry wave 2 sec4): unchanged bytes; the out-of-line callee is now declared
//   as the template IS_THING_ALIVE<CThing*>(CThing* const&) so it mangles as the landed
//   ??$IS_THING_ALIVE@PAVCThing@@@@YI_NABQAVCThing@@@Z (0x0040DCE0). Left: this in edi (retail ebp), the message bool
//   folded (retail setne bl beside the message), thing slot apart from the key slot; tried ?true:false on the message
//   test
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00957C30, length 384 bytes (vtable 0x0128C494 slot 11).
// Callees: CVectorMap::LowerBound @ 0x0040F020 (inlined GetInterface 0x34 = CTCPerceiveThing, bit 20 of +0x24, and 0x52
//          = CTCHeroPodium, bit 18 of +0x28), CTCPerceiveThing::GetPerceivedThing @ 0x007446A0 / IsPerceivedThingValid
//          @ 0x00744430 (manifest "GetCanItemBeQuickAssigned" is propagated), CAIStateGroupBase::GetPWorld @ 0x008FCFC0,
//          CMessageEventManager::FindMostRecentMessage<CMessageEventFilter_IsOfType> @ 0x004AB8B0 (world +0x60),
//          CIntelligentPointer<const CThing>(CThing*) @ 0x0056C860, CBaseIntelligentPointer GetPItem/SetPItem/dtor,
//          CMessageEvent::GetPOriginator<CThing> @ 0x006AF4E0, IS_THING_ALIVE<T*>(T* const&) @ 0x0040DCE0 (__fastcall),
//          CThing::GetPosition @ 0x004C73D0, CTCHeroPodium::GetCrowdPos @ 0x00771D50, the pure virtual slot 14 (+0x38).
// Shape: both lookups are the out-parameter form (the perception miss path reads the never-written slot; the podium
//          pointer is preset to null); the perceived thing and the originator are named locals whose homes are frame
//          slots (the originator's address goes to the out-of-line IS_THING_ALIVE); the message test is a setne bool.
// Layout: no engine header for this class; modelled locally on the PDB CAIStateGroupBase (PBrain +0x04), the latched
//          thing at +0x14, the crowd position C3DVector at +0x2c and the in-crowd flag at +0x38 (descriptive, from the
//          landed Cleanup 0x00957BE0).
struct CTCBase;
struct CInterfaceMap {
    struct SEntry { long type; CTCBase* component; };
    SEntry* first;
    SEntry* last;
    SEntry* LowerBound(const long* type);       // 0x0040F020
};
struct C3DVector {
    float X, Y, Z;
};
struct CThing {
    unsigned char _pad0[0x20];
    unsigned long m_presentBits[9];             // +0x20
    CInterfaceMap m_interfaces;                 // +0x44
    const C3DVector& GetPosition();             // 0x004C73D0
    template<class T> __forceinline bool GetInterface(T** ppOut, long interfaceType)
    {
        if (m_presentBits[interfaceType >> 5] & (1UL << (interfaceType & 31))) {
            long type = interfaceType;
            CInterfaceMap* map = &m_interfaces;
            CInterfaceMap::SEntry* it = map->LowerBound(&type);
            if (it == map->last || interfaceType < it->type)
                it = map->last;
            *ppOut = (T*)it->component;
            return true;
        }
        return false;
    }
};
template<class T> bool __fastcall IS_THING_ALIVE(const T& thing);   // 0x0040DCE0 (IS_THING_ALIVE<CThing*>, out of line)

struct CTCPerceiveThing {                       // interface 0x34
    unsigned char _pad[0x18];
    float PerceptionValue;                      // +0x18 (descriptive)
    unsigned char _pad1c[0x2d - 0x1c];
    bool PerceivedThingIsVisible;               // +0x2d (descriptive)
    CThing* GetPerceivedThing();                // 0x007446A0
    bool IsPerceivedThingValid();               // 0x00744430
};
struct CTCHeroPodium {                          // interface 0x52
    bool GetCrowdPos(const C3DVector* from, C3DVector* result);   // 0x00771D50
};
struct CMessageEvent {
    CThing* GetPOriginator();                   // 0x006AF4E0
};
struct CMessageEventFilter_IsOfType {
    long m_type;
    CMessageEventFilter_IsOfType(long type) : m_type(type) { }
};
struct CMessageEventManager {
    CMessageEvent* FindMostRecentMessage(const CMessageEventFilter_IsOfType& filter);   // 0x004AB8B0
};
struct CWorld {
    unsigned char _pad[0x60];
    CMessageEventManager* PMessageManager;      // +0x60
};

struct CBaseIntelligentPointer {
    virtual void v00();
    CThing* m_pItem;                            // +0x04
    ~CBaseIntelligentPointer();                 // 0x00A01C10
    CThing* GetPItem() const;                   // 0x00A01B50
    void SetPItem(CThing* item);                // 0x00A01B90
};
struct CIntelligentPointerConstThing : CBaseIntelligentPointer {   // CIntelligentPointer<const CThing>
    CIntelligentPointerConstThing(CThing* item);   // 0x0056C860
    __forceinline CIntelligentPointerConstThing& operator=(const CIntelligentPointerConstThing& other) { SetPItem(other.GetPItem()); return *this; }
};
struct CAIBrain {
    unsigned char _pad[0x18];
    CThing* PCreature;                          // +0x18
};

struct CAIStateGroupBase {
    virtual ~CAIStateGroupBase();               // slot 0
    virtual void v01(); virtual void v02(); virtual void v03(); virtual void v04(); virtual void v05();
    virtual void v06(); virtual void v07(); virtual void v08(); virtual void v09(); virtual void v10();
    virtual bool IsRunnable();                  // slot 11
    virtual void Update();                      // slot 12
    virtual void v13();
    virtual bool IsReadyForCrowd() = 0;         // slot 14 (+0x38, pure here)
    CAIBrain* PBrain;                           // +0x04
    long ScriptAIPriority;                      // +0x08
    void* Name_PStringData;                     // +0x0c
    unsigned char Terminated;                   // +0x10
    unsigned char Priority;                     // +0x11
    unsigned char Enabled;                      // +0x12
    unsigned char _pad_0x13;
    CThing* GetCreature() const { return PBrain->PCreature; }
    CWorld* GetPWorld();                        // 0x008FCFC0
};

struct CAIStateGroup_MinionBehaviourCrowdBase : CAIStateGroupBase {
    CIntelligentPointerConstThing PThing;       // +0x14
    unsigned char _pad1c[0x2c - 0x1c];
    C3DVector CrowdPos;                         // +0x2c (descriptive)
    bool InCrowd;                               // +0x38 (descriptive)
    virtual bool IsRunnable();
};

bool CAIStateGroup_MinionBehaviourCrowdBase::IsRunnable()
{
    CTCPerceiveThing* perceive;
    GetCreature()->GetInterface(&perceive, 0x34);
    CThing* thing = perceive->GetPerceivedThing();
    if (thing) {
        CMessageEvent* message = GetPWorld()->PMessageManager->FindMostRecentMessage(CMessageEventFilter_IsOfType(0x23));
        bool hasMessage = message != 0;
        if (perceive->PerceptionValue != 0.0f && perceive->IsPerceivedThingValid() && perceive->PerceivedThingIsVisible
            && IsReadyForCrowd() && hasMessage) {
            PThing = CIntelligentPointerConstThing(thing);
            if (InCrowd)
                return true;
            thing = message->GetPOriginator();
            if (IS_THING_ALIVE(thing)) {
                CTCHeroPodium* podium = 0;
                thing->GetInterface(&podium, 0x52);
                if (podium->GetCrowdPos(&GetCreature()->GetPosition(), &CrowdPos)) {
                    InCrowd = true;
                    return true;
                }
            }
        }
    }
    return false;
}
