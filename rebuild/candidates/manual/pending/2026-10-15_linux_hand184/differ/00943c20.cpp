// Grade: DIFFER(206v206) -- register allocation only: retail tests the villager bit through eax and forms PBrain/creature/&key in ecx/eax/edx; ours uses ecx, then edx/eax/eax | tried: masked-word HasTC, bool-return HasTC
// CAIStateGroup_WorkInWarehouse::IsRunnable() -- a villager in working hours whose village offers it a warehouse
// task thing (remembered in PTaskThing) is runnable.
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00943C20, length 202 bytes (+ int3 padding).
// Callees: the work-hours test @ 0x0091A6F0, CVectorMap::LowerBound @ 0x0040F020 (inlined out-parameter
// GetInterface, villager 0x23 = bit 3 of +0x24; the miss path reads the never-written slot), GFGetPVillageInterface
// @ 0x007EE540 (__fastcall), CTCVillage::GetWarehouseTaskThing(CThing*) @ 0x00658EA0, the converting
// CIntelligentPointer<CThing> temporary (vtable 0x0128B0B0: ctor @ 0x00A01B10, SetPItem @ 0x00A01B90, GetPItem
// @ 0x00A01B50, dtor @ 0x00A01C10) assigned to PTaskThing +0x1c.
// Shape: the villager bit tested inline first (`if (bits & mask) return true; return false;` HasTC), then the out-parameter lookup leaves the
// component in eax; the village null-tested; success when the pointer holds a thing afterwards.
// No engine header for this class; local model.
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
    __forceinline bool HasTC(long interfaceType) const
    {
        if (m_presentBits[interfaceType >> 5] & (1UL << (interfaceType & 31)))
            return true;
        return false;
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

struct CIntelligentPointerThing : CBaseIntelligentPointer {   // CIntelligentPointer<CThing>, vtable 0x0128B0B0
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
    CThing* GetWarehouseTaskThing(CThing* creature);    // 0x00658EA0
};

struct CAIStateGroup_WorkBase : CAIStateGroupBase {
    bool IsInWorkHours();                    // 0x0091A6F0
};

struct CAIStateGroup_WorkInWarehouse : CAIStateGroup_WorkBase {
    unsigned char _pad_0x14[8];
    CIntelligentPointerThing PTaskThing;        // +0x1c CIntelligentPointer<CThing>
    virtual bool IsRunnable();
};

bool CAIStateGroup_WorkInWarehouse::IsRunnable()
{
    if (PBrain->PCreature->HasTC(0x23) && IsInWorkHours()) {
        CTCVillager* villager;
        PBrain->PCreature->GetInterface(&villager, 0x23);
        CTCVillage* village = GFGetPVillageInterface(villager);
        if (village != 0) {
            PTaskThing = village->GetWarehouseTaskThing(PBrain->PCreature);
            if (PTaskThing.GetPItem() != 0)
                return true;
        }
    }
    return false;
}
