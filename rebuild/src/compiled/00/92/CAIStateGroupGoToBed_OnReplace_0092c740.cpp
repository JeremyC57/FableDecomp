// CAIStateGroup_GoToBed::OnReplace() -- give the bed's action point (or the bed itself) back and forget the bed
// Retail: Fable.exe (TLC, VC7.1) function @ 0x0092C740, length 347 bytes (vtable 0x0128B104 slot 10).
// Callees: CBaseIntelligentPointer::GetPItem @ 0x00A01B50, CVectorMap<ETCInterfaceType, CTCBase*>::LowerBound @ 0x0040F020
//          (the inlined GetInterface: type 0x2D = bit 13 of the bitset dword +0x24, type 0xAC = bit 12 of +0x34),
//          the action-point owner's FreePoint(long) @ 0x007FEF40 (landed as CPointerListOwner_007fef40::Add: appends the
//          index to its free list), the bed component's GetOccupant @ 0x0087C3E0 and SetOccupant(CThing*) @ 0x0087C390
//          (manifest labels "_Cons_val<...>" / "CVillageTaskCarryCrate::AssignCreatureToTask" are propagated),
//          CBaseIntelligentPointer ctor @ 0x00A01B10, SetPItem @ 0x00A01B90, dtor @ 0x00A01C10 (the PBed = 0 temporary,
//          vtable 0x01238C6C).
// Shape: IS_THING_ALIVE is the two-GetPItem inline test, the component comes from a third GetPItem through the
//          out-parameter GetInterface (miss path reads the never-written out slot, which the clearing temporary
//          later reuses); the -1 marker stays in edi for the store; the occupant is compared with the creature
//          read before the call.
// Layout: PDB CAIStateGroup_GoToBed: PBed +0x14, IsBedSleepPointMarker +0x1C, ActionPointIndex +0x2C (retail agrees,
//          see the landed GoToBed rows). No engine header exists for the class; the base follows engine/CAIStateGroupBase.h.
class CTCBase;
class CThing;

struct CInterfaceMap {
    struct SEntry { long type; CTCBase* component; };
    SEntry* first;
    SEntry* last;
    SEntry* LowerBound(const long* type);       // 0x0040F020
};

class CThing { public:
    unsigned char _pad0[0x20];
    unsigned long m_presentBits[9];             // +0x20 std::bitset of interface types
    CInterfaceMap m_interfaces;                 // +0x44
    unsigned char _pad1[0x45];
    unsigned char m_flags91;                    // +0x91 bit 0: dead
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
template <class T> class CIntelligentPointer : public CBaseIntelligentPointer {   // CIntelligentPointer<CThing>, vtable 0x01238C6C
public:
    __forceinline CIntelligentPointer(T* item) : CBaseIntelligentPointer() { SetPItem(item); }
    __forceinline CIntelligentPointer& operator=(const CIntelligentPointer& other) { SetPItem(other.GetPItem()); return *this; }

public:
    T* GetPItem() const { return reinterpret_cast<T*>(CBaseIntelligentPointer::GetPItem()); }
public:
    void SetPItem(T* item) { CBaseIntelligentPointer::SetPItem(reinterpret_cast<CBaseObject*>(item)); }
};

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

class CAIStateGroupBase { public:
    virtual ~CAIStateGroupBase();
    CAIBrain* PBrain;                           // +0x04
    long ScriptAIPriority;                      // +0x08
    void* Name_PStringData;                     // +0x0c
    unsigned char Terminated;                   // +0x10
    unsigned char Priority;                     // +0x11
    unsigned char Enabled;                      // +0x12
    unsigned char _pad_0x13;
};

struct CTCActionPointOwner {                    // interface 0x2d
    void FreePoint(long index);                 // 0x007FEF40
};
class CTCBed { public:                                 // interface 0xac
    CThing* GetOccupant();                      // 0x0087C3E0
    void SetOccupant(CThing* occupant);         // 0x0087C390
};

struct CAIStateGroup_GoToBed : CAIStateGroupBase {
    CIntelligentPointer<CThing> PBed;              // +0x14
    bool IsBedSleepPointMarker;                 // +0x1c
    long SleepStartTimeVariation;               // +0x20
    long SleepEndTimeVariation;                 // +0x24
    bool BedToRightOfCreature;                  // +0x28
    long ActionPointIndex;                      // +0x2c
    virtual void OnReplace();
};

void CAIStateGroup_GoToBed::OnReplace()
{
    if (!IsBedSleepPointMarker) {
        if (ActionPointIndex != -1 && IS_THING_ALIVE(PBed)) {
            CTCActionPointOwner* points;
            PBed.GetPItem()->GetInterface(&points, 0x2d);
            points->FreePoint(ActionPointIndex);
            ActionPointIndex = -1;
        }
    }
    else if (IS_THING_ALIVE(PBed)) {
        CTCBed* bed;
        PBed.GetPItem()->GetInterface(&bed, 0xac);
        CThing* creature = PBrain->PCreature;
        if (bed->GetOccupant() == creature)
            bed->SetOccupant(0);
    }
    PBed = 0;
}