// CAIStateGroup_GoToPub::GetPayPoint() -- ask the pub bar's tavern-bar component for a pickup slot for this creature (fills BarPos/BarRHSet/BarActionPointIndex/BarIndex)
// Grade: RELOCATION_MATCH (retry 10, RTM 3077) -- HasTC + the ternary GetTC model on a GetPItem() thing local: the bar loads straight into ecx before the GetPickupSlot pushes
// Retail: Fable.exe (TLC, VC7.1) function @ 0x009492A0, length 116 bytes (+ int3 padding).
// Callees: CBaseIntelligentPointer::GetPItem @ 0x00A01B50 (PBar at +0x1c), CVectorMap::LowerBound @ 0x0040F020
// (inlined interface lookup, type 0xb9 = CTCTavernBar, bit 25 of the presence dword at +0x34),
// CTCTavernBar::GetPickupSlot(CThing*, C3DVector*, CRightHandedSet*, long*, long*, bool) @ 0x007BA000.
//
// Tried: direct PBrain->PCreature argument (this in edi, creature loaded between the pushes), HasTC + pointer
// GetTC accessor (same), this creature local (best), GetTC local + creature local (same as best).
// Shape: the inlined out-parameter GetInterface template (map pointer in edi, key temporary in the `push ecx`
// slot) inside `if (..) { if (call) return true; } return false;` -- the nested form keeps the `mov al,1`
// exit before the shared `xor al,al` tail. The bar member addresses are formed from esi with the last one
// as `add esi,78h`. Members are the PDB names (struct_layouts_egor.tsv); no engine header for the class.
class CTCBase;
class C3DVector { public: float X, Y, Z; };
class CRightHandedSet { public: C3DVector Up; C3DVector Forward; };

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
    template<class T> __forceinline T* GetTC(long wanted)        // the landed 0x00584157 lookup: the entry pick as one ternary, the component read once
    {
        long type = wanted;
        CInterfaceMap* map = &m_interfaces;
        CInterfaceMap::SEntry* it = map->LowerBound(&type);
        return (T*)((it == map->last || wanted < it->type) ? map->last : it)->component;
    }
    unsigned long HasTC(long interfaceType) const { return m_presentBits[interfaceType >> 5] & (1UL << (interfaceType & 31)); }
};

class CTCTavernBar { public:                           // interface type 0xb9
    bool GetPickupSlot(CThing* creature, C3DVector* pos, CRightHandedSet* rhSet, long* actionPointIndex,
        long* barIndex, bool reserve);          // 0x007BA000
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

class CAIBrain { public:
    unsigned char _pad[0x18];
    CThing* PCreature;                          // +0x18
};

class CAIStateGroupBase { public:
    virtual ~CAIStateGroupBase();
    CAIBrain* PBrain;                           // +0x04
    unsigned char _pad[0x0c];                   // +0x08..+0x13
};

struct CAIStateGroup_GoToPub : CAIStateGroupBase {
    CIntelligentPointer<CThing> PPub;               // +0x14 CIntelligentPointer<CThingBuilding>
    CIntelligentPointer<CThing> PBar;               // +0x1c CIntelligentPointer<CThingObject>
    CIntelligentPointer<CThing> PTable;             // +0x24 CIntelligentPointer<CThingObject>
    CIntelligentPointer<CThing> PBarman;            // +0x2c CIntelligentPointer<CThingAICreature>
    CIntelligentPointer<CThing> PTankard;           // +0x34 CIntelligentPointer<CThing>
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
    bool GetPayPoint();
};

bool CAIStateGroup_GoToPub::GetPayPoint()
{
    CThing* barThing = PBar.GetPItem();
    if (barThing->HasTC(0xb9)) {
        CTCTavernBar* bar = barThing->GetTC<CTCTavernBar>(0xb9);
        CThing* creature = PBrain->PCreature;
        if (bar->GetPickupSlot(creature, &BarPos, &BarRHSet, &BarActionPointIndex, &BarIndex, false))
            return true;
    }
    return false;
}