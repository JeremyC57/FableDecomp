// CTCDummyVillager::SetupFromVillager(CThingAICreature&, bool) -- remember the villager, copy its def type and home/work buildings into the dummy, and mark the dummy a guard when the villager's role is 3
// Leaf: SetupFromVillager
//
// Retail 0x0072F2F0, 142 bytes (`ret 8`). Ego_r.pdb `?SetupFromVillager@CTCDummyVillager@@QAEXAAVCThingAICreature
// @@_N@Z`. Callees: CBaseIntelligentPointer::CBaseIntelligentPointer @ 0x00A01B10 (then the
// CIntelligentPointer<CThingAICreature> vtable 0x0123E7E0), ::SetPItem @ 0x00A01B90 (twice),
// ::GetPItem @ 0x00A01B50 (three times), ::~CBaseIntelligentPointer @ 0x00A01C10, CTCDummyVillager::
// SetupFromData(unsigned long, CThingBuilding*, CThingBuilding*, bool) @ 0x0072F170, the villager's
// virtual slot 48 (+0xc0, its villager role).
// Layout = Ego_r.pdb CTCDummyVillager: Creature +0x30, Guard +0x49; retail CThing def index word +0x8c,
// CThingAICreature home / work building intelligent pointers at +0x168 / +0x170.
// Shape: `Creature = CIntelligentPointer<CThingAICreature>(&villager)` (temporary built in the frame,
// inline operator= taking GetPItem() into a local before SetPItem, temporary destroyed out of line);
// the home building and the def type are named locals read before the call so their loads precede
// the argument pushes (ebx / ebp).
struct CBaseObject;
struct CThingBuilding;
struct CThingAICreature;

struct CBaseIntelligentPointer {                            // 8 bytes: vtable + item pointer
    virtual void v00();
    CBaseObject* m_pItem;                                   // +4
    CBaseIntelligentPointer();                              // 0x00A01B10
    ~CBaseIntelligentPointer();                             // 0x00A01C10
    void SetPItem(CBaseObject* item);                       // 0x00A01B90
    CBaseObject* GetPItem() const;                          // 0x00A01B50
};
template<class T> struct CIntelligentPointer : CBaseIntelligentPointer {
    virtual void v00();
    CIntelligentPointer() : CBaseIntelligentPointer() { }
    CIntelligentPointer(T* item) : CBaseIntelligentPointer() { SetPItem((CBaseObject*)item); }
    CIntelligentPointer& operator=(const CIntelligentPointer& other)
    {
        CBaseObject* item = other.CBaseIntelligentPointer::GetPItem();
        SetPItem(item);
        return *this;
    }
    T* GetPItem() const { return (T*)CBaseIntelligentPointer::GetPItem(); }
};

struct CThing {
    virtual void v00(); virtual void v01(); virtual void v02(); virtual void v03();
    virtual void v04(); virtual void v05(); virtual void v06(); virtual void v07();
    virtual void v08(); virtual void v09(); virtual void v10(); virtual void v11();
    virtual void v12(); virtual void v13(); virtual void v14(); virtual void v15();
    virtual void v16(); virtual void v17(); virtual void v18(); virtual void v19();
    virtual void v20(); virtual void v21(); virtual void v22(); virtual void v23();
    virtual void v24(); virtual void v25(); virtual void v26(); virtual void v27();
    virtual void v28(); virtual void v29(); virtual void v30(); virtual void v31();
    virtual void v32(); virtual void v33(); virtual void v34(); virtual void v35();
    virtual void v36(); virtual void v37(); virtual void v38(); virtual void v39();
    virtual void v40(); virtual void v41(); virtual void v42(); virtual void v43();
    virtual void v44(); virtual void v45(); virtual void v46(); virtual void v47();
    virtual long GetVillagerRole();                         // slot 48 (+0xc0)
    unsigned char  _pad04[0x8c - 0x04];
    unsigned short DefIndex;                                // +0x8c
};

struct CThingAICreature : CThing {
    unsigned char _pad90[0x168 - 0x90];                     // (CThing rounds up to 0x90)
    CIntelligentPointer<CThingBuilding> HomeBuilding;       // +0x168
    CIntelligentPointer<CThingBuilding> WorkBuilding;       // +0x170
};

struct CTCDummyVillager {
    virtual void v00();
    CThing*                               ParentThing;      // +0x04 (CTCBase)
    unsigned char                         _pad08[0x30 - 0x08];
    CIntelligentPointer<CThingAICreature> Creature;         // +0x30
    unsigned char                         _pad38[0x49 - 0x38];
    bool                                  Guard;            // +0x49
    void SetupFromData(unsigned long defType, CThingBuilding* home, CThingBuilding* work, bool respawnable);   // 0x0072F170
    void SetupFromVillager(CThingAICreature& villager, bool respawnable);
};

void CTCDummyVillager::SetupFromVillager(CThingAICreature& villager, bool respawnable)
{
    Creature = CIntelligentPointer<CThingAICreature>(&villager);
    CThingBuilding* home = villager.HomeBuilding.GetPItem();
    unsigned long defType = villager.DefIndex;
    SetupFromData(defType, home, villager.WorkBuilding.GetPItem(), respawnable);
    if (villager.GetVillagerRole() == 3)
        Guard = true;
}
