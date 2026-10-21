// CAIStateGroup_WorkInWarehouse::Update() -- while a crate task is set and the group is not stopping: through the creature's villager component find its village; if the village accepts the task for the creature, move the crates and hand the (still set) task back as complete; then take the village's best crate task for the creature; terminate
// Grade: DIFFER(ours v retail) -- 344v354: retail hoists &PCrate into ebp for the whole loop (this in edi, spilled) and aligns both loop heads; the lane keeps this in ebp and re-forms `lea [ebp+1Ch]` at each use (village in edi). Same calls and tests.
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00944300, length 354 bytes (ebp frame aligned with `and esp,-8`, plain `ret`).
// Callees: CBaseIntelligentPointer::GetPItem @ 0x00A01B50, CAIStateGroupBase::IsStopping @ 0x008FD010, the inlined interface
//          lookup for type 35 = bit 3 of the bitset dword at +0x24 (CTCVillager; the vector-map lower_bound is expanded inline
//          here, no LowerBound call), GFGetPVillageInterface(CTCVillager*) @ 0x007EE540 (__fastcall; manifest "IS_THING_ALIVE<..>"
//          is propagated), CTCVillage::SetCrateTaskAsAcceptedByThing(CThing*, CThing*) @ 0x006585BA, CAIStateGroup_WorkInWarehouse::
//          MoveCrates @ 0x00943CF0, the village's crate-task completion taking a by-value CIntelligentPointer<CVillageTaskCarryCrate>
//          @ 0x0065A053 (manifest "CTCInventoryBase::GetItemDescription" is propagated; it forwards the item to the list at
//          village+0x60), CTCVillage::GetBestCrateTaskForThing(CThing*) @ 0x00658EA0, CBaseIntelligentPointer ctor 0x00A01B10 /
//          SetPItem 0x00A01B90 / dtor 0x00A01C10 (vtable 0x0128B0B0 = CIntelligentPointer<CVillageTaskCarryCrate>), Terminate
//          @ 0x008FCF40.
// Shape: a rotated `while (PCrate.GetPItem())` loop with `if (IsStopping()) break;` at the aligned loop head; the villager
//          out-pointer is a function-scope local the lookup only writes on a hit (its never-written home is loaded into ebx
//          before the loop); the completion argument is built in the call area from the raw task pointer; the new task goes
//          through the implicit-conversion temporary (frame slot, 8-byte aligned -> the `and esp,-8` frame) and the inline
//          operator= (`SetPItem(other.GetPItem())`).
// Layout: Ego_r.pdb CAIStateGroup_WorkInWarehouse: PTask (CIntelligentPointer<CVillageTaskCarryCrate>) +0x1c, named PCrate as in
//          the landed InitCreaturePosition @ 0x00943AE0; +0x14 is the work building pointer. No engine header.
struct CTCBase;
struct CThing;
struct CVillageTaskCarryCrate;

struct CInterfaceMap {
    struct SEntry { long type; CTCBase* component; };
    SEntry* first;
    SEntry* last;
    SEntry* LowerBound(const long* type)        // STLport lower_bound over the 8-byte {type, component} entries (inlined here)
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
            *ppOut = (T*)((it == map->last || interfaceType < it->type) ? map->last : it)->component;
            return true;
        }
        return false;
    }
};

struct CBaseIntelligentPointer {
    virtual void v00();
    CThing* m_pItem;                            // +0x04
    CBaseIntelligentPointer();                  // 0x00A01B10
    ~CBaseIntelligentPointer();                 // 0x00A01C10
    CThing* GetPItem() const;                   // 0x00A01B50
    void SetPItem(CThing* item);                // 0x00A01B90
};
struct CIntelligentPointerCrateTask : CBaseIntelligentPointer {   // CIntelligentPointer<CVillageTaskCarryCrate>, vtable 0x0128B0B0
    virtual void v00();
    __forceinline CIntelligentPointerCrateTask(CThing* item) : CBaseIntelligentPointer() { SetPItem(item); }
    __forceinline CIntelligentPointerCrateTask& operator=(const CIntelligentPointerCrateTask& other) { SetPItem(other.GetPItem()); return *this; }
};

struct CTCVillager;                             // interface 35
struct CTCVillage {
    bool SetCrateTaskAsAcceptedByThing(CThing* task, CThing* creature);      // 0x006585BA
    void CompleteCrateTask(CIntelligentPointerCrateTask task);              // 0x0065A053
    CThing* GetBestCrateTaskForThing(CThing* creature);                     // 0x00658EA0 (CVillageTaskCarryCrate*)
};
CTCVillage* __fastcall GFGetPVillageInterface(CTCVillager* villager);       // 0x007EE540

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
    bool IsStopping();                          // 0x008FD010
    void Terminate();                           // 0x008FCF40
};

struct CAIStateGroup_WorkInWarehouse : CAIStateGroupBase {
    unsigned char PWorkBuilding[8];             // +0x14 CIntelligentPointer<CThing>
    CIntelligentPointerCrateTask PCrate;        // +0x1c
    bool MoveCrates();                          // 0x00943CF0
    virtual void Update();
};

void CAIStateGroup_WorkInWarehouse::Update()
{
    CIntelligentPointerCrateTask& crate = PCrate;
    CTCVillager* villager;
    while (crate.GetPItem() != 0) {
        if (IsStopping())
            break;
        PBrain->PCreature->GetInterface(&villager, 35);
        CTCVillage* village = GFGetPVillageInterface(villager);
        if (village != 0) {
            if (village->SetCrateTaskAsAcceptedByThing(crate.GetPItem(), PBrain->PCreature)) {
                if (MoveCrates()) {
                    if (crate.GetPItem() != 0)
                        village->CompleteCrateTask(crate.GetPItem());
                }
            }
            crate = village->GetBestCrateTaskForThing(PBrain->PCreature);
        }
    }
    Terminate();
}
