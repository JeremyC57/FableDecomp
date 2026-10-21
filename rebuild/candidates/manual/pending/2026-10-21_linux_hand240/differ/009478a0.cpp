// CAIStateGroup_StayNearFoodHall::IsRunnable() -- in work hours, latch the nearest food hall (building property 0x80) of the creature's village and its gather point; runnable while the creature is more than 15m (XY) from that point
// Grade: DIFFER(ours v retail) -- first draft: retail keeps a runnable bool in bl (xor bl,bl ... mov al,bl) that ours folds to a constant, so the register plan shifts (keeper/village ebp vs ebx)
// Retail: Fable.exe (TLC, VC7.1) function @ 0x009478A0, length 400 bytes.
// Callees: CBaseIntelligentPointer ctor/SetPItem/GetPItem/dtor (temporary CIntelligentPointer<CThing>, vtable 0x01244344),
//          the inlined GetInterface 0x23 (CTCVillageMember, bit 3 of +0x24; one-ternary pick into ecx), CTCVillageMember::
//          GetPVillage @ 0x007EE540, CAIStateGroup_WorkBase::IsInWorkHours @ 0x00947840, the village's nearest-building
//          query @ 0x00658E38 (position, property 0x80), CThing::GetPosition @ 0x004C73D0, the out-of-line
//          CIntelligentPointer<const CThing> copy ctor @ 0x00947810 (by-value argument), the creature's gather-point query
//          @ 0x008334F0 (C3DVector by hidden pointer).
// Globals: 225.0f (15m squared) @ 0x012922C0.
// Shape: first draft: the result bool, the copy-constructed by-value pointer argument and the gather-point copy.
// Layout: no engine header for this class; modelled locally on the PDB CAIStateGroupBase (PBrain +0x04) with the gather
//          point C3DVector at +0x14 and the food hall pointer at +0x20 (descriptive).
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
struct C2DVector {
    float X, Y;
    float GetLengthSquared() const { return X * X + Y * Y; }
};
struct CThing;
struct CBaseIntelligentPointer {
    virtual void v00();
    CThing* m_pItem;                            // +0x04
    CThing* GetPItem() const;                   // 0x00A01B50
    CBaseIntelligentPointer();                  // 0x00A01B10
    ~CBaseIntelligentPointer();                 // 0x00A01C10
    void SetPItem(CThing* item);                // 0x00A01B90
};
struct CIntelligentPointerConstThing : CBaseIntelligentPointer {   // CIntelligentPointer<const CThing>
    virtual void v00();
    CIntelligentPointerConstThing(const CBaseIntelligentPointer& other);   // 0x00947810 (out of line)
};
struct CIntelligentPointerThing : CBaseIntelligentPointer {   // CIntelligentPointer<CThing>, vtable 0x01244344
    virtual void v00();
    __forceinline CIntelligentPointerThing(CThing* item) : CBaseIntelligentPointer() { SetPItem(item); }
    __forceinline CIntelligentPointerThing& operator=(const CIntelligentPointerThing& other) { CThing* item = other.GetPItem(); SetPItem(item); return *this; }
};
struct CThing {
    unsigned char _pad0[0x20];
    unsigned long m_presentBits[9];             // +0x20
    CInterfaceMap m_interfaces;                 // +0x44
    const C3DVector& GetPosition();             // 0x004C73D0
    C3DVector GetGatherPoint(CIntelligentPointerConstThing building);   // 0x008334F0 (descriptive)
    template<class T> __forceinline bool GetInterface(T** ppOut, long interfaceType)
    {
        if (m_presentBits[interfaceType >> 5] & (1UL << (interfaceType & 31))) {
            long type = interfaceType;
            CInterfaceMap* map = &m_interfaces;
            CInterfaceMap::SEntry* it = map->LowerBound(&type);
            *ppOut = (T*)((it == map->last || interfaceType < it->type) ? map->last : it)->component;
            return true;
        }
        return false;
    }
};
struct CVillage {
    CThing* FindNearestBuildingWithProperty(const C3DVector& pos, long property);   // 0x00658E38 (descriptive)
};
struct CTCVillageMember {                       // interface 0x23
    CVillage* GetPVillage() const;              // 0x007EE540
};
struct CAIBrain {
    unsigned char _pad[0x18];
    CThing* PCreature;                          // +0x18
};
struct CAIStateGroupBase {
    virtual ~CAIStateGroupBase();
    CAIBrain* PBrain;                           // +0x04
    long ScriptAIPriority;                      // +0x08
    void* Name_PStringData;                     // +0x0c
    unsigned char Terminated;                   // +0x10
    unsigned char Priority;                     // +0x11
    unsigned char Enabled;                      // +0x12
    unsigned char _pad_0x13;
    CThing* GetCreature() const { return PBrain->PCreature; }
};
struct CAIStateGroup_StayNearFoodHall : CAIStateGroupBase {
    C3DVector GatherPoint;                      // +0x14 (descriptive)
    CIntelligentPointerThing PFoodHall;         // +0x20 (descriptive)
    bool IsInWorkHours();                       // 0x00947840
    virtual bool IsRunnable();
};

bool CAIStateGroup_StayNearFoodHall::IsRunnable()
{
    bool runnable = false;
    PFoodHall = CIntelligentPointerThing(0);
    CTCVillageMember* member;
    if (GetCreature()->GetInterface(&member, 0x23)) {
        CVillage* village = member->GetPVillage();
        if (village && IsInWorkHours()) {
            PFoodHall = CIntelligentPointerThing(village->FindNearestBuildingWithProperty(GetCreature()->GetPosition(), 0x80));
            if (!PFoodHall.GetPItem())
                return false;
            CThing* creature = GetCreature();
            GatherPoint = creature->GetGatherPoint(PFoodHall);
            const C3DVector& pos = GetCreature()->GetPosition();
            C2DVector delta;
            delta.X = pos.X - GatherPoint.X;
            delta.Y = pos.Y - GatherPoint.Y;
            bool near = delta.GetLengthSquared() < 225.0f;
            return !near;
        }
    }
    return runnable;
}
