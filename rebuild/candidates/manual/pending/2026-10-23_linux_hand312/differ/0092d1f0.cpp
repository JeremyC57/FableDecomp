// Grade: DIFFER(1034v1031) -- Rhset.Forward copy (retail fld/fstp X,Y + Z stored twice), BedToRightOfCreature true block not cross-jumped to the shared false exit (mov al,1 hoisted), ebx/edi swap around GetRHSet; distance order fixed by statement accumulation
// CAIStateGroup_GoToBed::FindFreeBed(bool force) -- for a living home the creature may use, pick the nearest free bed of the house, then either claim a free action point on it or treat it as a sleep-point marker (record its position and orientation, take the sleep point, and note which side of the creature it is on)
// Retail: Fable.exe (TLC, VC7.1) @ 0x0092D1F0, 1040 bytes, ret 4.
// Callees: CBaseIntelligentPointer ctor 0x00A01B10 / GetPItem 0x00A01B50 / SetPItem 0x00A01B90 / dtor 0x00A01C10
//   (CIntelligentPointer<CThing> vtable 0x01238C6C), CThing::GetPosition 0x004C73D0, the creature category getter
//   0x00662000, CTCWife::IsMarriedToThePlayer 0x006CEC80, the building tests 0x0082DD90 / 0x0082DE20,
//   find_if<list iterator, CIsFreeBed> 0x0092E790 (__fastcall, hidden iterator result), CVectorMap::LowerBound 0x0040F020,
//   CTCUserActionPointBase::GetFreeActionPoint 0x007FEDE0, the graphic's slot 72 (+0x120, GetRHSet), the sleep point's
//   SetUser 0x0087C390, the C3DVector member 0x00A14260, CRightHandedSet::GetRight 0x0049CE00.
// Layout: CAIStateGroup_GoToBed PBed +0x14, IsBedSleepPointMarker +0x1c, BedToRightOfCreature +0x28,
//   ActionPointIndex +0x2c, BedPos +0x3c, Rhset +0x48 (Ego_r.pdb); the creature's home pointer at +0x168 (retail).
struct CTCBase;
struct C3DVector {
    float X, Y, Z;
    C3DVector() { }
    C3DVector(const C3DVector& o) : X(o.X), Y(o.Y), Z(o.Z) { }
    C3DVector(float x, float y, float z) : X(x), Y(y), Z(z) { }
    C3DVector operator-(const C3DVector& o) const { return C3DVector(X - o.X, Y - o.Y, Z - o.Z); }
    float GetMagnitudeSquared() const { float s = X * X; s += Z * Z; s += Y * Y; return s; }
    void A14260(float angle, long mode);        // 0x00A14260
};
__forceinline float Dot(const C3DVector& a, const C3DVector& b) { return a.X * b.X + a.Y * b.Y + a.Z * b.Z; }
struct CRightHandedSet {
    C3DVector Up;
    C3DVector Forward;
    C3DVector GetRight() const;                 // 0x0049CE00
};
struct CInterfaceMap {
    struct SEntry { long type; CTCBase* component; };
    SEntry* first;
    SEntry* last;
    SEntry* LowerBound(const long* type);       // 0x0040F020
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
#pragma auto_inline(on)

struct CThing {
    const C3DVector& GetPosition() const;       // 0x004C73D0
    long GetCreatureCategory() const;           // 0x00662000
    bool BuildingTestA() const;                 // 0x0082DD90
    bool BuildingTestB() const;                 // 0x0082DE20
    unsigned char _pad0[0x20];
    unsigned long m_presentBits[9];             // +0x20
    CInterfaceMap m_interfaces;                 // +0x44
    unsigned char _pad4c[0x60 - 0x4c];
    struct CThingGraphic* PGraphic;             // +0x60
    unsigned char _pad64[0x91 - 0x64];
    unsigned char m_flags91;                    // +0x91 bit 0: dead
    __forceinline bool HasTC(long type) const
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
struct CIntelligentPointerThing : CBaseIntelligentPointer {   // vtable 0x01238C6C
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
    CIntelligentPointerThing _M_data;
};
struct CBedIterator {
    CPointerListNode* _M_node;
    CBedIterator(CPointerListNode* node) : _M_node(node) { }
};
struct CIsFreeBed { };
CBedIterator __fastcall FindFreeBedIf(CBedIterator first, CBedIterator last, CIsFreeBed pred);   // 0x0092E790
struct CTCHouse {                               // interface 0x21
    unsigned char _pad00[0x0c];
    CPointerListNode* _M_node;                  // +0x0c list of beds
};
struct CTCWife {                                // interface 0xD7
    bool IsMarriedToThePlayer() const;          // 0x006CEC80
};
struct CTCUserActionPointBase {                 // interface 0x2D
    bool GetFreeActionPoint(CThing* creature, C3DVector* pos, CRightHandedSet* rhSet, long* index);   // 0x007FEDE0
};
struct CTCSleepPoint {                          // interface 0xAC
    void SetUser(CThing* user);                 // 0x0087C390
};
struct CThingCreature : CThing {
    unsigned char _pad92[0x168 - 0x92];
    CIntelligentPointerThing Home;              // +0x168
};
struct CAIBrain {
    unsigned char _pad00[0x18];
    CThingCreature* Creature;                   // +0x18
};
struct CAIStateGroupBase {
    virtual void v00();
    CAIBrain* PBrain;                           // +0x04
    unsigned char _pad08[0x0c];
    CThingCreature* GetCreature() const { return PBrain->Creature; }
};
struct CAIStateGroup_GoToBed : CAIStateGroupBase {
    CIntelligentPointerThing PBed;              // +0x14
    bool IsBedSleepPointMarker;                 // +0x1c
    unsigned char _pad1d[0x28 - 0x1d];
    bool BedToRightOfCreature;                  // +0x28
    long ActionPointIndex;                      // +0x2c
    unsigned char _pad30[0x3c - 0x30];
    C3DVector BedPos;                           // +0x3c
    CRightHandedSet Rhset;                      // +0x48
    bool FindFreeBed(bool force);
};

bool CAIStateGroup_GoToBed::FindFreeBed(bool force)
{
    IsBedSleepPointMarker = false;
    CThing* house = GetCreature()->Home.GetPItem();
    C3DVector myPos = GetCreature()->GetPosition();
    if (house && !(house->m_flags91 & 1)) {
        if (GetCreature()->GetCreatureCategory() == 1
            || (GetCreature()->HasTC(0xd7) && ((CTCWife*)GetCreature()->GetTC(0xd7))->IsMarriedToThePlayer())
            || !house->BuildingTestA() || house->BuildingTestB() || force || house->HasTC(0xab)) {
            PBed = CIntelligentPointerThing(0);
            if (house->HasTC(0x21)) {
                CTCHouse* beds = (CTCHouse*)house->GetTC(0x21);
                float bestDistanceSquared = 0.0f;
                CIsFreeBed isFree;
                for (CBedIterator it(beds->_M_node->_M_next); it._M_node != beds->_M_node; it._M_node = it._M_node->_M_next) {
                    it = FindFreeBedIf(it, CBedIterator(beds->_M_node), isFree);
                    if (it._M_node == beds->_M_node)
                        break;
                    if (PBed.GetPItem()) {
                        if (!((myPos - it._M_node->_M_data.GetPItem()->GetPosition()).GetMagnitudeSquared() < bestDistanceSquared))
                            continue;
                    }
                    PBed = it._M_node->_M_data;
                    bestDistanceSquared = (myPos - it._M_node->_M_data.GetPItem()->GetPosition()).GetMagnitudeSquared();
                }
            }
            if (PBed.GetPItem() && !(PBed.GetPItem()->m_flags91 & 1)) {
                CThing* bed = PBed.GetPItem();
                if (bed->HasTC(0x2d)) {
                    CTCUserActionPointBase* points = (CTCUserActionPointBase*)bed->GetTC(0x2d);
                    if (points->GetFreeActionPoint(GetCreature(), &BedPos, &Rhset, &ActionPointIndex)) {
                        BedToRightOfCreature = false;
                        return true;
                    }
                } else {
                    IsBedSleepPointMarker = true;
                    BedPos = PBed.GetPItem()->GetPosition();
                    Rhset = *PBed.GetPItem()->PGraphic->GetRHSet();
                    CTCSleepPoint* point;
                    CThing* sleepBed = PBed.GetPItem();
                    if (sleepBed->HasTC(0xac))
                        point = (CTCSleepPoint*)sleepBed->GetTC(0xac);
                    point->SetUser(GetCreature());
                    C3DVector forward = Rhset.Forward;
                    forward.A14260(0.25f, 0);
                    if (Dot(forward, Rhset.GetRight()) > 0.0f) {
                        BedToRightOfCreature = true;
                        return true;
                    }
                    BedToRightOfCreature = false;
                    return true;
                }
            }
        }
    }
    PBed = CIntelligentPointerThing(0);
    return false;
}
