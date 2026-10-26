// CAIStateGroup_MinionPutOutFires::Update() -- while the building still burns: go to the well, put out fire, wait.
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00936020, length 78 bytes (+ int3 padding). Vtable slot 12
// of CAIStateGroup_MinionPutOutFires (0x0128B410).
// Callees: CBaseIntelligentPointer::GetPItem @ 0x00A01B50 (twice, inlined IS_THING_ALIVE on PBuildingOnFire
// +0x14), GoToWell @ 0x00935C40, PutOutFire @ 0x00935E40, CAIStateGroupBase::AIAction_DoNothing @ 0x0097E200
// and CAIStateGroupBase::Terminate @ 0x008FCF40 (tail-jumped from the merged failure path).
//
// Shape: the IS_THING_ALIVE guard (pointer set, dead bit +0x91 clear) exits straight to the epilogue; the
// loop body runs GoToWell then PutOutFire, each failure sharing one Terminate tail (VC7.1 merges the two
// identical `{ Terminate(); return; }` blocks and hoists the `mov ecx,esi`), and repeats while the wait
// action reports 4 (still running). PBuildingOnFire sits at +0x14 (PDB layout, 28 bytes).
class CTCBase;
class CThing;

class C3DVector { public:
    float X, Y, Z;
    C3DVector(const C3DVector& other) : X(other.X), Y(other.Y), Z(other.Z) { }
};

// A thing's component table: the presence bitset at +0x20 (tested inline, bit N of dword N/32) and the sorted
// {type, component} vector map at +0x44 searched with LowerBound @ 0x0040F020 -- the shape of the landed
// GetInterface_0x.. bodies, inlined into the AI bodies through GetTC<T>().
struct CInterfaceMap {
    struct SEntry { long type; CTCBase* component; };
    SEntry* first;
    SEntry* last;
    SEntry* LowerBound(const long* type);      // 0x0040F020
};

class CThing { public:
    unsigned char _pad0[0x20];
    unsigned long m_presentBits[9];             // +0x20 std::bitset of interface types
    CInterfaceMap m_interfaces;                 // +0x44
    unsigned char _pad1[0x45];
    unsigned char m_flags91;                    // +0x91 bit 0: dead
    C3DVector* GetPosition();                   // 0x004C73D0 (`[this+60h]+0Ch`)
    bool HasTC(long interfaceType) const
    {
        return (m_presentBits[interfaceType >> 5] & (1UL << (interfaceType & 31))) != 0;
    }
    template<class T> __forceinline bool GetInterface(T** ppOut, long interfaceType)
    {
        if (m_presentBits[interfaceType >> 5] & (1UL << (interfaceType & 31))) {
            long type = interfaceType;
            CInterfaceMap* map = &m_interfaces;   // named pointer: retail advances esi to the map once
            CInterfaceMap::SEntry* it = map->LowerBound(&type);
            if (it == map->last || interfaceType < it->type)
                it = map->last;
            *ppOut = (T*)it->component;
            return true;
        }
        return false;
    }
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
template <class T> class CIntelligentPointer : public CBaseIntelligentPointer {
public:
    T* GetPItem() const { return (T*)CBaseIntelligentPointer::GetPItem(); }
    void SetPItem(T* item) { CBaseIntelligentPointer::SetPItem((CBaseObject*)item); }
};

// IS_THING_ALIVE(ptr): the pointer is set and the thing's dead bit (+0x91 bit 0) is clear (inlined).
inline bool IS_THING_ALIVE(const CIntelligentPointer<CThing>& p)
{
    if (p.GetPItem() != 0) {
        if (!(p.GetPItem()->m_flags91 & 1))
            return true;
    }
    return false;
}

class CAIBrain { public:
    unsigned char _pad[0x18];
    CThing* PCreature;                          // +0x18
};

class CWorld;

class CAIStateGroupBase { public:
    virtual ~CAIStateGroupBase();               // slot 0
    virtual void v01(); virtual void v02(); virtual void v03(); virtual void v04(); virtual void v05();
    virtual void v06(); virtual void v07(); virtual void v08();
    virtual void Cleanup();                     // slot 9  (+0x24)
    virtual void OnReplace();                   // slot 10 (+0x28)
    virtual bool IsRunnable();                  // slot 11 (+0x2c)
    virtual void Update();                      // slot 12 (+0x30)
    virtual void v13(); virtual void v14(); virtual void v15(); virtual void v16(); virtual void v17();
    CAIBrain* PBrain;                           // +0x04
    long ScriptAIPriority;                      // +0x08
    void* Name_PStringData;                     // +0x0c
    unsigned char Terminated;                   // +0x10
    unsigned char Priority;                     // +0x11
    unsigned char Enabled;                      // +0x12
    unsigned char _pad_0x13;
    CWorld* GetPWorld();                        // 0x008FCFA0 (PBrain->+0x1c->+0x24)
    unsigned long* GetPWorldSeed() const;       // 0x008FCFD0 (world random seed)
    void Terminate();                           // 0x008FCF40 (Terminated = true)
    bool IsStopping();                          // 0x008FD010
    long AIAction_DoNothing();                  // 0x0097E200 (EAIActionReturn)
};

struct CAIStateGroup_MinionPutOutFires : CAIStateGroupBase {
    CIntelligentPointer<CThing> PBuildingOnFire;    // +0x14 CIntelligentPointer<CThingBuilding>
    bool GoToWell();                            // 0x00935C40
    bool PutOutFire();                          // 0x00935E40
    virtual void Update();
};

void CAIStateGroup_MinionPutOutFires::Update()
{
    if (!IS_THING_ALIVE(PBuildingOnFire))
        return;
    do {
        if (!GoToWell()) {
            Terminate();
            return;
        }
        if (!PutOutFire()) {
            Terminate();
            return;
        }
    } while (AIAction_DoNothing() == 4);
}