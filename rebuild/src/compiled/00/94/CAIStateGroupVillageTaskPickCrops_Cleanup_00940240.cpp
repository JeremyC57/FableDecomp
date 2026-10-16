// CAIStateGroup_VillageTaskPickCrops::Cleanup() -- hand the remembered crop field back to the creature's village
// Grade: RELOCATION_MATCH -- landing retry: named creature local before RemoveThingFromTask (fixes the eax push)
// (RemoveThingFromTask) and clear PTaskThing.
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00940240, length 208 bytes.
// Callees: CBaseIntelligentPointer::GetPItem @ 0x00A01B50, CVectorMap::LowerBound @ 0x0040F020 (inlined HasTC +
// entry-returning FindEntry, villager 0x23 = bit 3 of +0x24), GFGetPVillageInterface @ 0x007EE540 (__fastcall),
// CTCVillage::RemoveThingFromTask(CIntelligentPointer<CVillageTaskBase>, CThing*) @ 0x00659EFC (the by-value pointer
// built in its argument slot: ctor @ 0x00A01B10, vtable 0x0125A8A4, SetPItem @ 0x00A01B90 from PTaskThing), then
// `PTaskThing = 0` through the converting temporary (vtable 0x0128B0A0) and its dtor @ 0x00A01C10.
// Shape: the bool HasTC result is spilled to a frame byte (`shr ecx,3 ; and cl,1 ; mov [esp+13h],cl`); the
// by-value argument's converting ctor takes the source pointer by reference (GetPItem after the vtable store).
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
    SEntry* LowerBound(const long* type);       // 0x004365B0 / 0x0040F020 (byte-identical instantiations)
};

struct CThing {
    unsigned char _pad0[0x20];
    unsigned long m_presentBits[9];             // +0x20 std::bitset of interface types
    CInterfaceMap m_interfaces;                 // +0x44
    bool HasTC(long interfaceType) const
    {
        return (m_presentBits[interfaceType >> 5] & (1UL << (interfaceType & 31))) != 0;
    }
    __forceinline CInterfaceMap::SEntry* FindEntry(long interfaceType)
    {
        long type = interfaceType;
        CInterfaceMap* map = &m_interfaces;
        CInterfaceMap::SEntry* it = map->LowerBound(&type);
        CInterfaceMap::SEntry* sel;
        if (it == map->last) sel = map->last;
        else if (it->type <= interfaceType) sel = it;
        else sel = map->last;
        return sel;
    }
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

struct CIntelligentPointerTask : CBaseIntelligentPointer {   // CIntelligentPointer<CVillageTaskBase>, vtable 0x0125A8A4
    virtual void v00();
    CIntelligentPointerTask(const CBaseIntelligentPointer& other) : CBaseIntelligentPointer() { SetPItem(other.GetPItem()); }
};

struct CTCVillage {
    void RemoveThingFromTask(CIntelligentPointerTask task, CThing* creature);   // 0x00659EFC
};

struct CAIStateGroup_VillageTaskPickCrops : CAIStateGroupBase {
    CIntelligentPointerThing PTaskThing;        // +0x14 CIntelligentPointer<CThing>
    virtual void Cleanup();
};

void CAIStateGroup_VillageTaskPickCrops::Cleanup()
{
    if (PTaskThing.GetPItem() != 0) {
        bool hasVillager = PBrain->PCreature->HasTC(0x23);
        if (hasVillager) {
            CTCVillage* village = GFGetPVillageInterface((CTCVillager*)PBrain->PCreature->FindEntry(0x23)->component);
            CThing* creature = PBrain->PCreature;
            village->RemoveThingFromTask(PTaskThing, creature);
        }
        if (hasVillager)
            PTaskThing = 0;
    }
}