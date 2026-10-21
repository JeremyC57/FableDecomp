// CAIStateGroup_GoToPub::OnReplace() -- free the seat and the bar point, get up from the table when sitting (removing the sitting mode if that fails or when not sitting), leave hands-off mode, and clear every tracked pointer and the seat index
// Retail: Fable.exe (TLC, VC7.1) function @ 0x009498B0, length 497 bytes (+ int3 padding). Vtable slot 10.
// Callees: CVectorMap::LowerBound @ 0x0040F020 (inlined GetInterface 0x31 CTCCreatureModeManager = bit 17 of +0x24),
// CAIStateGroup_GoToPub::FreeSeat @ 0x009486D0, FreeBarPoint @ 0x00949320, CCharString(const char*, long) @ 0x0099EBF0 /
// ~CCharString @ 0x0099EAE0 ("SIT_AT_TABLE_OUTOF" @ 0x01293074), GetUpFromSitting @ 0x0085FF10 (__fastcall),
// CTCCreatureModeManager::RemoveMode @ 0x0073ABA0 (modes 0x0b and 0x2d), CBaseIntelligentPointer ctor @ 0x00A01B10 /
// GetPItem @ 0x00A01B50 / SetPItem @ 0x00A01B90 / dtor @ 0x00A01C10 (vtables CIntelligentPointer<CThingBuilding> 0x01244344,
// <CThingObject> 0x0125A858, <CThingAICreature> 0x0123E7E0, <CThing> 0x01238C6C).
// Shape: the get-up result is kept in a block-scoped bool and folded into the remove-mode flag after the name's destructor
// (`if (gotUp) removeMode = false;`): retail keeps the flag's initial true in a frame byte and selects it into al; the pointer
// resets are converting-ctor temporaries assigned through the inline operator= (the <CThingObject> vtable cached in edi).
// Layout: GoToPub from Ego_r.pdb (base 0x14): PPub +0x14, PBar +0x1c, PTable +0x24, PBarman +0x2c, PTankard +0x34, SeatIndex
// +0x3c, Sitting +0x70, BarActionPointIndex +0x74, HandsOffMode +0xa0.
struct CTCBase;
struct CInterfaceMap {
    struct SEntry { long type; CTCBase* component; };
    SEntry* first;
    SEntry* last;
    SEntry* LowerBound(const long* type);       // 0x0040F020
};
struct CThing {
    unsigned char _pad0[0x20];
    unsigned long m_presentBits[9];             // +0x20
    CInterfaceMap m_interfaces;                 // +0x44
    template<class T> __forceinline void GetInterface(T** ppOut, long interfaceType)
    {
        if (m_presentBits[interfaceType >> 5] & (1UL << (interfaceType & 31))) {
            long type = interfaceType;
            CInterfaceMap* map = &m_interfaces;
            CInterfaceMap::SEntry* it = map->LowerBound(&type);
            if (it == map->last || interfaceType < it->type)
                it = map->last;
            *ppOut = (T*)it->component;
        }
    }
};
struct CThingBuilding;
struct CThingObject;
struct CThingAICreature;
struct CBaseIntelligentPointer {
    virtual void v00();
    CThing* m_pItem;                            // +0x04
    CBaseIntelligentPointer();                  // 0x00A01B10
    ~CBaseIntelligentPointer();                 // 0x00A01C10
    CThing* GetPItem() const;                   // 0x00A01B50
    void SetPItem(const CThing* item);          // 0x00A01B90
};
template<class T> struct CIntelligentPointer : CBaseIntelligentPointer {
    virtual void v00();
    __forceinline CIntelligentPointer(T* item) { SetPItem((const CThing*)item); }
    __forceinline CIntelligentPointer& operator=(const CIntelligentPointer& other) { SetPItem(other.GetPItem()); return *this; }
};
struct CCharString {
    void* PStringData;
    CCharString(const char* text, long length = -1);   // 0x0099EBF0
    ~CCharString();                                      // 0x0099EAE0
};
struct CTCCreatureModeManager {                 // interface 0x31
    void RemoveMode(unsigned long mode);        // 0x0073ABA0
};
struct CAIBrain {
    unsigned char _pad[0x18];
    CThing* PCreature;                          // +0x18
};
bool __fastcall GetUpFromSitting(CThing* creature, long movement, const CCharString* anim, CAIBrain* brain);   // 0x0085FF10

struct CAIStateGroupBase {
    virtual ~CAIStateGroupBase();
    CAIBrain* PBrain;                           // +0x04
    unsigned char _pad08[0x0c];                 // +0x08..+0x13
};
struct CAIStateGroup_GoToPub : CAIStateGroupBase {
    CIntelligentPointer<CThingBuilding> PPub;   // +0x14
    CIntelligentPointer<CThingObject> PBar;     // +0x1c
    CIntelligentPointer<CThingObject> PTable;   // +0x24
    CIntelligentPointer<CThingAICreature> PBarman;   // +0x2c
    CIntelligentPointer<CThing> PTankard;       // +0x34
    long SeatIndex;                             // +0x3c
    unsigned char _pad40[0x70 - 0x40];          // Pos, RHSet, ActionPointIndex, NextFramePub, FrameLeavePub
    bool Sitting;                               // +0x70
    long BarActionPointIndex;                   // +0x74
    unsigned char _pad78[0xa0 - 0x78];          // BarPos, BarRHSet, BarIndex
    bool HandsOffMode;                          // +0xa0
    void FreeSeat();                            // 0x009486D0
    void FreeBarPoint();                        // 0x00949320
    virtual void OnReplace();
};

void CAIStateGroup_GoToPub::OnReplace()
{
    bool removeMode = true;
    CTCCreatureModeManager* modes;
    PBrain->PCreature->GetInterface(&modes, 0x31);
    FreeSeat();
    if (BarActionPointIndex != -1)
        FreeBarPoint();
    if (Sitting) {
        bool gotUp;
        {
            CCharString anim("SIT_AT_TABLE_OUTOF");
            gotUp = GetUpFromSitting(PBrain->PCreature, 0xb, &anim, PBrain);
        }
        if (gotUp)
            removeMode = false;
        Sitting = false;
    }
    if (removeMode)
        modes->RemoveMode(0xb);
    if (HandsOffMode) {
        HandsOffMode = false;
        modes->RemoveMode(0x2d);
    }
    PPub = 0;
    PBar = 0;
    PTable = 0;
    PBarman = 0;
    PTankard = 0;
    SeatIndex = -1;
}
