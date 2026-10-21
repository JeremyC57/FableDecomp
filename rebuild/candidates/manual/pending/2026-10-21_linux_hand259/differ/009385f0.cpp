// CAIStateGroup_UsesSeat::MoveToSeat(long movement) -- walk to the seat position while the seat stays valid, then turn to face the seat
// Leaf: MoveToSeat
// Grade: DIFFER(461v457) -- frame packing: retail homes the `reached` flag and the look-at vector pieces in the dead movement-argument slot and keeps the GetInterface presence bit as a memory byte (frame 0x2c); ours gives them their own slots (0x38) and swaps esi/edi
// Retail: Fable.exe (TLC, VC7.1) @ 0x009385F0; best of the probes graded under VC7.1 RTM (probe/009385f0.cpp).
struct CTCBase;
struct C3DVector {
    float x, y, z;
    C3DVector() { }
    C3DVector(float ax, float ay, float az) : x(ax), y(ay), z(az) { }
    C3DVector operator+(const C3DVector& o) const { return C3DVector(x + o.x, y + o.y, z + o.z); }
};
struct CRightHandedSet {
    C3DVector Up;                               // +0x00
    C3DVector Forward;                          // +0x0c
};
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
    template<class T> __forceinline bool GetInterface(T** ppOut, long interfaceType)
    {
        bool has = (unsigned char)(m_presentBits[interfaceType >> 5] >> (interfaceType & 31)) & 1;
        if (has) {
            long type = interfaceType;
            CInterfaceMap* map = &m_interfaces;
            CInterfaceMap::SEntry* it = map->LowerBound(&type);
            *ppOut = (T*)((it == map->last || interfaceType < it->type) ? map->last : it)->component;
            return true;
        }
        return false;
    }
};
struct CThingCreature : CThing {
    const C3DVector& GetPhysicsPosition() const;    // 0x006A4D00
};
struct CBaseIntelligentPointer {
    virtual void v00();
    CThing* m_pItem;                            // +0x04
    CBaseIntelligentPointer();                  // 0x00A01B10
    CThing* GetPItem() const;                   // 0x00A01B50
    void SetPItem(CThing* item);                // 0x00A01B90
    ~CBaseIntelligentPointer();                 // 0x00A01C10
};
struct CIntelligentPointerThingObject : CBaseIntelligentPointer {  // CIntelligentPointer<CThingObject>
    virtual void v00();
};
struct CTCUserActionPointBase {                 // interface 0x2d
    bool GetFreeActionPoint(CThing* user, C3DVector* pos, CRightHandedSet* orientation, long* index);   // 0x007FEDE0
};
struct CAIBrain {
    unsigned char _pad[0x18];
    CThingCreature* PCreature;                  // +0x18
};
struct CAIStateGroupBase {
    virtual ~CAIStateGroupBase();
    virtual void v01(); virtual void v02(); virtual void v03(); virtual void v04(); virtual void v05();
    virtual void v06(); virtual void v07(); virtual void v08(); virtual void v09(); virtual void v10();
    virtual void v11(); virtual void v12(); virtual void v13();
    virtual bool IsSeatValid();                 // slot 14 (+0x38)
    CAIBrain* PBrain;                           // +0x04
    long ScriptAIPriority;                      // +0x08
    void* Name_PStringData;                     // +0x0c
    unsigned char Terminated;                   // +0x10
    unsigned char Priority;
    unsigned char Enabled;
    unsigned char _pad_0x13;
    long AIAction_MoveToPos(long repeat, C3DVector pos, bool* reached, long movement, bool a, bool b,
        CThing* thing, bool c, float speed);    // 0x0097EC00
    long AIAction_LookAtPos(long repeat, C3DVector pos, long flags, float speed);   // 0x0097D580
};
struct CAIStateGroup_UsesSeat : CAIStateGroupBase {
    CIntelligentPointerThingObject PSeat;       // +0x14
    bool GotSeatPos;                            // +0x1c
    bool Sitting;                               // +0x1d
    long SeatPosIndex;                          // +0x20
    C3DVector SeatPos;                          // +0x24
    bool SittingOnBench;                        // +0x30
    bool MoveToSeat(long movement);
};

bool CAIStateGroup_UsesSeat::MoveToSeat(long movement)
{
    if (PSeat.GetPItem() == 0 || !IsSeatValid())
        return false;
    CTCUserActionPointBase* point;
    PSeat.GetPItem()->GetInterface(&point, 0x2d);
    CRightHandedSet orientation;
    if (!point->GetFreeActionPoint(PBrain->PCreature, &SeatPos, &orientation, &SeatPosIndex))
        return false;
    GotSeatPos = true;
    long result;
    bool reached;
    do {
        if (PSeat.GetPItem() == 0 || !IsSeatValid())
            return false;
        result = AIAction_MoveToPos(2, SeatPos, &reached, movement, true, true, 0, true, 0.0f);
    } while (result == 4);
    if (result != 2)
        return false;
    if (PSeat.GetPItem() == 0 || !IsSeatValid())
        return false;
    C3DVector forward = orientation.Forward;
    C3DVector lookPos = forward + PBrain->PCreature->GetPhysicsPosition();
    do {
        if (PSeat.GetPItem() == 0 || !IsSeatValid())
            return false;
        result = AIAction_LookAtPos(2, lookPos, 0x11, 0.0027777f);
    } while (result == 4);
    if (result != 2)
        return false;
    if (PSeat.GetPItem() == 0 || !IsSeatValid())
        return false;
    return true;
}
