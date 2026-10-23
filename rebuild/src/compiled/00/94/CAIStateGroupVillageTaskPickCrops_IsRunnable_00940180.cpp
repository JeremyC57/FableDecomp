// CAIStateGroup_VillageTaskPickCrops::IsRunnable() -- in working hours, ask the creature's village for a crop
// field for this creature; runnable (and the field remembered in PTaskThing) when one is offered.
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00940180, length 182 bytes (+ int3 padding).
// Callees: CAIStateGroup_WorkBase::IsInSchoolHours @ 0x00940120 (manifest name; the work-hours test),
// CVectorMap::LowerBound @ 0x004365B0 (inlined out-parameter GetInterface, villager 0x23 = bit 3 of +0x24; the
// miss path reads the never-written slot), GFGetPVillageInterface @ 0x007EE540 (__fastcall; the inline villager
// accessor calls it once on the hit path with the result unused), CTCVillage::GetCropField(CThing*) @ 0x006590E4
// (manifest "_Umove<EWeaponClass*>" propagated), CBaseIntelligentPointer ctor/SetPItem/GetPItem/dtor (the converting
// temporary, CIntelligentPointer<CThing> vtable 0x0128B0A0) on PTaskThing +0x14.
// Shape: the villager accessor is an inline helper (component kept in esi); the village is not null-tested; the
// creature argument is pushed before the village call (object expression evaluated after the argument).
// No engine-header member names beyond the padding; local model.
// ---- shared AI state-group model (same shapes as the landed CAIStateGroup_* sources) ----
struct CTCBase;
struct CTCVillager;
struct CTCVillage;
CTCVillage* __fastcall GFGetPVillageInterface(CTCVillager* villager);    // 0x007EE540 (manifest IS_THING_ALIVE label propagated)

struct CInterfaceMap {
    struct SEntry { long type; CTCBase* component; };
    SEntry* first;
    SEntry* last;
    SEntry* LowerBound(const long* type) const;       // 0x004365B0 / 0x0040F020 (byte-identical instantiations)
};

struct CThing {
    unsigned char _pad0[0x20];
    unsigned long m_presentBits[9];             // +0x20 std::bitset of interface types
    CInterfaceMap m_interfaces;                 // +0x44
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

struct CBaseIntelligentPointer {
    virtual void v00();
    CThing* m_pItem;                            // +0x04
    CBaseIntelligentPointer();                  // 0x00A01B10
    CThing* GetPItem() const;                   // 0x00A01B50
    void SetPItem(CThing* item);                // 0x00A01B90
    ~CBaseIntelligentPointer();                 // 0x00A01C10
};

struct CIntelligentPointerThing : CBaseIntelligentPointer {   // CIntelligentPointer<CThing>, vtable 0x0128B0A0
    virtual void v00();
    CIntelligentPointerThing(CThing* item) : CBaseIntelligentPointer() { SetPItem(item); }
    CIntelligentPointerThing& operator=(const CIntelligentPointerThing& other) { SetPItem(other.GetPItem()); return *this; }
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
};

struct CTCVillage {
    CThing* GetCropField(CThing* creature);     // 0x006590E4
};

// inline villager accessor: the out-parameter lookup plus the village check on the hit path
__forceinline CTCVillager* GetVillagerTC(CThing* creature)
{
    CTCVillager* villager;
    if (creature->GetInterface(&villager, 0x23))
        GFGetPVillageInterface(villager);
    return villager;
}

struct CAIStateGroup_WorkBase : CAIStateGroupBase {
    bool IsInSchoolHours();                     // 0x00940120
};

struct CAIStateGroup_VillageTaskPickCrops : CAIStateGroup_WorkBase {
    CIntelligentPointerThing PTaskThing;        // +0x14 CIntelligentPointer<CThing>
    virtual bool IsRunnable();
};

bool CAIStateGroup_VillageTaskPickCrops::IsRunnable()
{
    if (IsInSchoolHours()) {
        CTCVillager* villager = GetVillagerTC(PBrain->PCreature);
        CThing* field = GFGetPVillageInterface(villager)->GetCropField(PBrain->PCreature);
        if (field != 0) {
            PTaskThing = field;
            return true;
        }
    }
    return false;
}