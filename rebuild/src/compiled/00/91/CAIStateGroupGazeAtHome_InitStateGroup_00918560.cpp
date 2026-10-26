// CAIStateGroup_GazeAtHome::InitStateGroup(const CWorld&, CThingAICreature&) -- clear the gaze marker and register as the home idle group (7)
// Leaf: ?InitStateGroup@CAIStateGroup_GazeAtHome@@MAEXABVCWorld@@AAVCThingAICreature@@@Z
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00918560, length 136 bytes (+ int3 padding).
// Callees: CBaseIntelligentPointer::CBaseIntelligentPointer @ 0x00A01B10, ::SetPItem @ 0x00A01B90,
// ::GetPItem @ 0x00A01B50, ::~CBaseIntelligentPointer @ 0x00A01C10, CVectorMap::LowerBound @ 0x0040F020
// (inlined interface lookup), CTCIdleScheduler::RegisterIdleStateGroup @ 0x00801920.
//
// Shape: `GazeMarker = CIntelligentPointer<CThingGameObject>(0)` -- an 8-byte stack temporary (base ctor,
// then push 0 / the derived vtable 0x0127B4D8 / SetPItem), copied into the member at +0x14 through
// SetPItem(GetPItem()) and destroyed through the base destructor (the derived destructor is compiler-
// generated, so no vtable reset). Then the creature's idle scheduler (interface type 0xd3 = bit 19 of the
// presence dword at +0x38) is fetched by the inlined GetInterface template (its key temporary is homed in
// the dead `creature` argument slot) and this group is registered as EIdleStateGroup 7.
// The engine headers (CAIStateGroup_GazeAtHome.h) are flattened data-only layouts without the vptr chain,
// so `this` is modelled locally on the landed CAIStateGroup_Gaze model (0x009182B0).
class CTCBase;
class CThingGameObject;

struct CInterfaceMap {
    struct SEntry { long type; CTCBase* component; };
    SEntry* first;
    SEntry* last;
    SEntry* LowerBound(const long* type);      // 0x0040F020
};

class CThingAICreature {
public:
    unsigned char _pad0[0x20];
    unsigned long m_presentBits[9];             // +0x20 std::bitset of interface types
    CInterfaceMap m_interfaces;                 // +0x44
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

class CWorld;
class CAIStateGroupBase;

class CTCIdleScheduler { public:                       // interface type 0xd3
    void RegisterIdleStateGroup(CAIStateGroupBase* group, long idleGroup);   // 0x00801920
};

class CBaseObject;
class CBaseObjectPointer;
class CBaseIntelligentPointer {                       // vtable 0x0129C95C (Ego_r: vptr + PData, 8 bytes)
public:
    CBaseIntelligentPointer();                           // 0x00A01B10
    virtual ~CBaseIntelligentPointer();                  // 0x00A01C10
    CBaseObject* GetPItem() const;                       // 0x00A01B50
    void SetPItem(CBaseObject* item);                    // 0x00A01B90
private:
    CBaseObjectPointer* PData;                           // +0x04
};

template <class T> class CIntelligentPointer : public CBaseIntelligentPointer { public:
    CIntelligentPointer(T* item) : CBaseIntelligentPointer() { SetPItem(item); }
    CIntelligentPointer& operator=(const CIntelligentPointer& other)
    {
        SetPItem(other.GetPItem());
        return *this;
    }

    T* GetPItem() const { return (T*)CBaseIntelligentPointer::GetPItem(); }
    void SetPItem(T* item) { CBaseIntelligentPointer::SetPItem((CBaseObject*)item); }
};

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

struct CAIStateGroup_Gaze : CAIStateGroupBase {
    CIntelligentPointer<CThingGameObject> GazeMarker;   // +0x14
    float ChanceOfRunning;                      // +0x1C
    float MinJourneyForRunning;                 // +0x20
    float MaxDistanceForCreaturesToLookAt;      // +0x24
    long PrimaryMarkerDef;                      // +0x28
    long SecondaryMarkerDef;                    // +0x2C
};

struct CAIStateGroup_GazeAtHome : CAIStateGroup_Gaze {
protected:
    virtual void InitStateGroup(const CWorld& world, CThingAICreature& creature);
};

void CAIStateGroup_GazeAtHome::InitStateGroup(const CWorld& world, CThingAICreature& creature)
{
    GazeMarker = CIntelligentPointer<CThingGameObject>(0);
    CTCIdleScheduler* scheduler;
    if (creature.GetInterface(&scheduler, 0xd3))
        scheduler->RegisterIdleStateGroup(this, 7);
}