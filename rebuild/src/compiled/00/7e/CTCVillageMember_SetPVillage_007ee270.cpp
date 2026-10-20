// CTCVillageMember::SetPVillage(CTCVillage* village) -- points the member at the village's thing (or clears it) and, for a creature owner, makes its game-state and level persistence follow whether it now belongs to a village
// Leaf: SetPVillage
// Retail: Fable.exe (TLC, VC7.1) function @ 0x007EE270, 183 bytes (`ret 4`). Ego_r.pdb
// `?SetPVillage@CTCVillageMember@@QAEXPAVCTCVillage@@@Z`.
// Callees: CBaseIntelligentPointer::CBaseIntelligentPointer() 0x00A01B10 (then the CIntelligentPointer<CThing> vtable
//          0x01238C6C), ::SetPItem(CBaseObject*) 0x00A01B90, ::GetPItem() const 0x00A01B50, ::~CBaseIntelligentPointer()
//          0x00A01C10; CThing::SetAsGameStatePersisted(bool) 0x004C7880.
// Layout: CTCVillageMember (Ego_r.pdb, retail equal): ParentThing +0x04, PVillage (CIntelligentPointer<CThing>) +0x10; the
//         village's thing is its CTCBase ParentThing (+0x04); CThing ThingType byte +0x10 (1 = creature), flag byte +0x92
//         bit 4 LevelPersistent (PDB +0xa2).
// Shape: both assignments go through a CIntelligentPointer<CThing> temporary (base constructor, the argument pushed before
//        the derived vtable store, SetPItem) copied into the member by an inline operator= that fetches GetPItem() into a
//        local first (else the member address is formed before the call) and destroyed through the base destructor (no
//        user destructor in the derived class, so no vtable re-store); `village ? true : false` written at both uses
//        gives one `setne bl` pushed whole for the call and merged into the bit field (a named bool goes through the
//        frame).
struct CBaseObject;
struct CThing {
    unsigned char _pad_0x00[0x10];
    unsigned char ThingType;                    // +0x10 (descriptive; 1 = creature)
    unsigned char _pad_0x11[0x81];
    unsigned char DeletedOnOwnerLevelOutOfScope : 1;   // +0x92 bit 0
    unsigned char Drawable : 1;
    unsigned char Dying : 1;
    unsigned char ForceFrameUpdate : 1;
    unsigned char LevelPersistent : 1;          // bit 4
    unsigned char GameStatePersisted : 1;
    unsigned char PersistOnUnloadWhenInLimbo : 1;
    unsigned char PausedDuringCutscenes : 1;
    void SetAsGameStatePersisted(bool persisted);   // 0x004C7880
};

struct CBaseIntelligentPointer {                // 8 bytes: vptr + item
    virtual void v00();
    CBaseObject* m_pItem;
    CBaseIntelligentPointer();                  // 0x00A01B10
    ~CBaseIntelligentPointer();                 // 0x00A01C10
    CBaseObject* GetPItem() const;              // 0x00A01B50
    void SetPItem(CBaseObject* item);           // 0x00A01B90
};
template <class T> struct CIntelligentPointer : CBaseIntelligentPointer {   // CIntelligentPointer<CThing>, vtable 0x01238C6C
    virtual void v00();
    CIntelligentPointer() { }
    CIntelligentPointer(T* item) : CBaseIntelligentPointer() { SetPItem((CBaseObject*)item); }
    CIntelligentPointer& operator=(const CIntelligentPointer& other)
    {
        CBaseObject* item = other.GetPItem();
        SetPItem(item);
        return *this;
    }
};

struct CTCVillage {
    void*   __vftable;                          // +0x00
    CThing* ParentThing;                        // +0x04 (CTCBase)
};

struct CTCVillageMember {
    void*         __vftable;                    // +0x00
    CThing*       ParentThing;                  // +0x04 (CTCBase)
    unsigned char _pad_0x08[0x8];
    CIntelligentPointer<CThing> PVillage;       // +0x10
    void SetPVillage(CTCVillage* village);
};

void CTCVillageMember::SetPVillage(CTCVillage* village)
{
    PVillage = CIntelligentPointer<CThing>(0);
    if (village)
        PVillage = CIntelligentPointer<CThing>(village->ParentThing);
    if (ParentThing->ThingType == 1) {
        ParentThing->SetAsGameStatePersisted(village ? true : false);
        ParentThing->LevelPersistent = village ? true : false;
    }
}