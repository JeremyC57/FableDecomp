// CAIStateGroup_GoToBed::FindFreeBanditBed() -- choose the nearest of the region's sleep points that is mine and not blocked, record its position and orientation, and claim it
// Retail: Fable.exe (TLC, VC7.1) @ 0x0092C8A0, 656 bytes.
// Callees: CVectorMap::LowerBound 0x0040F020 / const 0x004365B0 (same-TU stand-ins; inlined GetInterface 0x23 (const)
//   and 0xAC), the region accessor 0x007EE540, CBaseIntelligentPointer ctor 0x00A01B10 / GetPItem 0x00A01B50 / SetPItem
//   0x00A01B90 / dtor 0x00A01C10, CThingFilter_IsMySleepPoint::operator() 0x006A8930, the blocking test 0x006A8A30,
//   CThing::GetPosition 0x004C73D0, the graphic's virtual slot 72 (+0x120, orientation), the sleep point's SetUser
//   0x0087C390 (catalog "AssignCreatureToTask", propagated).
// Layout: CAIStateGroup_GoToBed PBed +0x14, IsBedSleepPointMarker +0x1c, BedPos +0x3c, Rhset +0x48 (Ego_r.pdb).
// Shape: the out-of-range lookups read an uninitialised component (retail jumps to a load of the empty slot); the list
// head is a named local; the blocked filter is declared beside the sleep-point filter; operator= names the item first.
struct CTCBase;
struct C3DVector { float X, Y, Z; };
struct CRightHandedSet { float M[6]; };
struct CInterfaceMap {
    struct SEntry { long type; CTCBase* component; };
    SEntry* first;
    SEntry* last;
    SEntry* LowerBound(const long* type);       // 0x0040F020
    const SEntry* LowerBound(const long* type) const;   // 0x004365B0
};
#pragma auto_inline(off)
CInterfaceMap::SEntry* CInterfaceMap::LowerBound(const long* type)
{
    SEntry* it = first;
    int len = int(last - first);
    while (len > 0) {
        int half = len / 2;
        SEntry* middle = it + half;
        if (middle->type < *type) {
            it = middle + 1;
            len = len - half - 1;
        } else
            len = half;
    }
    return it;
}
const CInterfaceMap::SEntry* CInterfaceMap::LowerBound(const long* type) const
{
    const SEntry* it = first;
    int len = int(last - first);
    while (len > 0) {
        int half = len / 2;
        const SEntry* middle = it + half;
        if (middle->type < *type) {
            it = middle + 1;
            len = len - half - 1;
        } else
            len = half;
    }
    return it;
}
#pragma auto_inline(on)
float __fastcall GFFloatRandom(unsigned long* seed, float range);   // 0x004978A0

struct CThing {
    const C3DVector& GetPosition() const;       // 0x004C73D0
    unsigned char _pad0[0x20];
    unsigned long m_presentBits[9];             // +0x20 std::bitset of interface types
    CInterfaceMap m_interfaces;                 // +0x44
    unsigned char _pad4c[0x60 - 0x4c];
    struct CThingGraphic* PGraphic;             // +0x60
    unsigned char _pad64[0x91 - 0x64];
    unsigned char m_flags91;                    // +0x91 bit 0: dead, bit 5
    bool HasTC(long type) const
    {
        if (m_presentBits[type >> 5] & (1UL << (type & 31)))
            return true;
        return false;
    }
    __forceinline CTCBase* GetTC(long type)
    {
        long key = type;
        CInterfaceMap* map = &m_interfaces;
        CInterfaceMap::SEntry* it = map->LowerBound(&key);
        return (it == map->last || type < it->type ? map->last : it)->component;
    }
    __forceinline CTCBase* GetTC(long type) const
    {
        long key = type;
        const CInterfaceMap* map = &m_interfaces;
        const CInterfaceMap::SEntry* it = map->LowerBound(&key);
        return (it == map->last || type < it->type ? map->last : it)->component;
    }
};
struct CThingGraphic {
    virtual void v000(); virtual void v004(); virtual void v008(); virtual void v00c(); virtual void v010(); virtual void v014();
    virtual void v018(); virtual void v01c(); virtual void v020(); virtual void v024(); virtual void v028(); virtual void v02c();
    virtual void v030(); virtual void v034(); virtual void v038(); virtual void v03c(); virtual void v040(); virtual void v044();
    virtual void v048(); virtual void v04c(); virtual void v050(); virtual void v054(); virtual void v058(); virtual void v05c();
    virtual void v060(); virtual void v064(); virtual void v068(); virtual void v06c(); virtual void v070(); virtual void v074();
    virtual void v078(); virtual void v07c(); virtual void v080(); virtual void v084(); virtual void v088(); virtual void v08c();
    virtual void v090(); virtual void v094(); virtual void v098(); virtual void v09c(); virtual void v0a0(); virtual void v0a4();
    virtual void v0a8(); virtual void v0ac(); virtual void v0b0(); virtual void v0b4(); virtual void v0b8(); virtual void v0bc();
    virtual void v0c0(); virtual void v0c4(); virtual void v0c8(); virtual void v0cc(); virtual void v0d0(); virtual void v0d4();
    virtual void v0d8(); virtual void v0dc(); virtual void v0e0(); virtual void v0e4(); virtual void v0e8(); virtual void v0ec();
    virtual void v0f0(); virtual void v0f4(); virtual void v0f8(); virtual void v0fc(); virtual void v100(); virtual void v104();
    virtual void v108(); virtual void v10c(); virtual void v110(); virtual void v114(); virtual void v118(); virtual void v11c();
    virtual const CRightHandedSet* GetRHSet();  // slot 72 (+0x120)
};
struct CBaseIntelligentPointer {
    virtual void v00();
    CThing* m_pItem;
    CBaseIntelligentPointer();                  // 0x00A01B10
    ~CBaseIntelligentPointer();                 // 0x00A01C10
    CThing* GetPItem() const;                   // 0x00A01B50
    void SetPItem(CThing* item);                // 0x00A01B90
};
struct CIntelligentPointerThing : CBaseIntelligentPointer {   // CIntelligentPointer<CThing>, vtable 0x01238C6C
    virtual void v00();
    CIntelligentPointerThing(CThing* item) : CBaseIntelligentPointer() { SetPItem(item); }
    CIntelligentPointerThing& operator=(const CIntelligentPointerThing& other)
    {
        CThing* item = other.GetPItem();
        SetPItem(item);
        return *this;
    }
};
struct CPointerListNode {
    CPointerListNode* _M_next;
    CPointerListNode* _M_prev;
    CBaseIntelligentPointer _M_data;
};
struct CRegionThing {
    unsigned char _pad00[0x70];
    CPointerListNode* _M_node;                  // +0x70 STLport list of sleep points
};
struct CTCRegionLink {                          // interface 0x23
    CRegionThing* GetPRegion();                 // 0x007EE540
};
struct CThingFilter_IsMySleepPoint {
    CThing* Creature;
    CThingFilter_IsMySleepPoint(CThing* creature) : Creature(creature) { }
    bool operator()(CThing* thing) const;       // 0x006A8930
};
struct CThingFilter_IsBlocked {
    bool operator()(CThing* thing) const;       // 0x006A8A30
};
struct CTCSleepPoint {                          // interface 0xAC
    void SetUser(CThing* user);                 // 0x0087C390
};
struct CAIBrain {
    unsigned char _pad00[0x18];
    CThing* Creature;                           // +0x18
};
struct CAIStateGroupBase {
    virtual void v00();
    CAIBrain* PBrain;                           // +0x04
    unsigned char _pad08[0x0c];
    CThing* GetCreature() const { return PBrain->Creature; }
};
struct CAIStateGroup_GoToBed : CAIStateGroupBase {
    CIntelligentPointerThing PBed;              // +0x14
    bool IsBedSleepPointMarker;                 // +0x1c
    unsigned char _pad1d[0x3c - 0x1d];
    C3DVector BedPos;                           // +0x3c
    CRightHandedSet Rhset;                      // +0x48
    bool FindFreeBanditBed();
};

bool CAIStateGroup_GoToBed::FindFreeBanditBed()
{
    const CThing* creature = GetCreature();
    if (creature->HasTC(0x4a)) {
        CTCRegionLink* link;
        if (creature->HasTC(0x23))
            link = (CTCRegionLink*)creature->GetTC(0x23);
        CRegionThing* region = link->GetPRegion();
        if (region && region->_M_node->_M_next != region->_M_node) {
            CThingFilter_IsMySleepPoint isMine(GetCreature());
            CThingFilter_IsBlocked isBlocked;
            PBed = CIntelligentPointerThing(0);
            float bestDistanceSquared = 0.0f;
            CPointerListNode* head = region->_M_node;
            for (CPointerListNode* node = head->_M_next; node != head; node = node->_M_next) {
                CThing* bed = node->_M_data.GetPItem();
                if (bed && !(bed->m_flags91 & 1) && isMine(bed) && !isBlocked(bed)) {
                    CThing* me = GetCreature();
                    const C3DVector& bedPos = bed->GetPosition();
                    const C3DVector& myPos = me->GetPosition();
                    float dx = myPos.X - bedPos.X;
                    float dy = myPos.Y - bedPos.Y;
                    float dz = myPos.Z - bedPos.Z;
                    float distanceSquared = dz * dz + dy * dy + dx * dx;
                    if (!PBed.GetPItem() || bestDistanceSquared > distanceSquared) {
                        PBed.SetPItem(bed);
                        bestDistanceSquared = distanceSquared;
                        BedPos = bed->GetPosition();
                        Rhset = *bed->PGraphic->GetRHSet();
                    }
                }
            }
            if (PBed.GetPItem()) {
                IsBedSleepPointMarker = true;
                CTCSleepPoint* point;
                CThing* chosen = PBed.GetPItem();
                if (chosen->HasTC(0xac))
                    point = (CTCSleepPoint*)chosen->GetTC(0xac);
                point->SetUser(GetCreature());
                return true;
            }
        }
    }
    PBed = CIntelligentPointerThing(0);
    return false;
}
