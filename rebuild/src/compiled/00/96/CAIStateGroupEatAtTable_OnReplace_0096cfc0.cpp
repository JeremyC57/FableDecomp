// CAIStateGroup_EatAtTable::OnReplace() -- drop modes 0x28, free the table seat (getting up when seated), drop/replace the carried tankard, then drop the sitting mode unless the get-up took it
// Retail: Fable.exe (TLC, VC7.1) function @ 0x0096CFC0, length 463 bytes.
// Callees: CVectorMap::LowerBound @ 0x0040F020 (interfaces 0x31 / 0xb8 / 0x46 / 0xba), CTCCreatureModeManager::RemoveMode
//          @ 0x0073ABA0, GetPItem @ 0x00A01B50, seat-table IsSeatTaken @ 0x007B8570 (manifest "CTCInventoryBase::
//          GetSelectedCategory" is propagated: a byte-vector at +0x34), FreeSeat @ 0x007B8AA0, SetOccupant @ 0x007B85F0
//          (manifest "CVillageTaskCarryCrate::AssignCreatureToTask" is propagated), CCharString @ 0x0099EBF0 / ~ @ 0x0099EAE0,
//          GetUpFromSitting @ 0x0085FF10 (__fastcall), CTCCarrying::GetThingInPrimarySlot @ 0x006A1440 /
//          RemoveThingFromPrimarySlot @ 0x006A1E00, CTCTavernTankard::ReplaceTankard @ 0x00947C30, CThing::Kill @ 0x004C9B80.
// Layout: PDB CAIStateGroup_EatAtTable: Table +0x14 (CIntelligentPointer<CThing>), SeatIndex +0x1c, SeatActionPointIndex +0x20;
//          no engine header, modelled locally.
// Shape (LowerBound): the CVectorMap<ETCInterfaceType, CTCBase*>::LowerBound instantiation is defined in this TU (noinline),
//   as retail compiled it in the caller's TU; with its body visible VC7.1 packs the lookup keys and temporaries as retail.
struct CTCBase;
struct CInterfaceMap {
    struct SEntry { long type; CTCBase* component; };
    SEntry* first;
    SEntry* last;
    SEntry* LowerBound(const long* type);       // 0x0040F020
};
#pragma auto_inline(off)
// Same-TU body of the CVectorMap<ETCInterfaceType, CTCBase*>::LowerBound instantiation (a template member, compiled in the
// caller's TU): binary search for the first entry whose type is not below the key read through the pointer.
__declspec(noinline) CInterfaceMap::SEntry* CInterfaceMap::LowerBound(const long* type)
{
    SEntry* it = first;
    long count = last - first;
    while (count > 0) {
        long half = count / 2;
        if (it[half].type < *type) {
            it += half + 1;
            count -= half + 1;
        } else
            count = half;
    }
    return it;
}
#pragma auto_inline(on)

struct CThing {
    unsigned char _pad0[0x20];
    unsigned long m_presentBits[9];             // +0x20
    CInterfaceMap m_interfaces;                 // +0x44
    unsigned char _pad1[0x45];
    unsigned char m_flags91;                    // +0x91 bit 0: dead
    void Kill(bool b);                          // 0x004C9B80
    unsigned long HasTC(long interfaceType) const
    {
        return m_presentBits[interfaceType >> 5] & (1UL << (interfaceType & 31));
    }
    template<class T> __forceinline T* GetTC(long interfaceType)
    {
        long type = interfaceType;
        CInterfaceMap* map = &m_interfaces;
        CInterfaceMap::SEntry* it = map->LowerBound(&type);
        if (it == map->last || interfaceType < it->type)
            it = map->last;
        return (T*)it->component;
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
    CThing* GetPItem() const;                   // 0x00A01B50
};
struct CCharString {
    void* m_pData;
    CCharString(const char* text, long length = -1);   // 0x0099EBF0
    ~CCharString();                                     // 0x0099EAE0
};
struct CAIBrain {
    unsigned char _pad[0x18];
    CThing* PCreature;                          // +0x18
};
bool __fastcall GetUpFromSitting(CThing* creature, long movement, const CCharString* anim, CAIBrain* brain);   // 0x0085FF10
struct CTCCreatureModeManager {                 // interface 0x31
    void RemoveMode(unsigned long mode);        // 0x0073ABA0
};
struct CTCCarrying {                            // interface 0x46
    CThing* GetThingInPrimarySlot();            // 0x006A1440
    void RemoveThingFromPrimarySlot(bool b);    // 0x006A1E00
};
struct CTCTavernTankard {                       // interface 0xba
    void ReplaceTankard();                      // 0x00947C30
};
struct CTCSeatTable {                           // interface 0xb8
    unsigned char _pad[0x10];
    CThing** SeatUsers;                         // +0x10
    bool IsSeatTaken(long seat);                // 0x007B8570
    void FreeSeat(long seat, long actionPoint); // 0x007B8AA0
    void SetOccupant(long seat, CThing* occupant);   // 0x007B85F0
};
struct CAIStateGroupBase {
    virtual ~CAIStateGroupBase();
    CAIBrain* PBrain;                           // +0x04
    unsigned char _pad[0x0c];
    CThing* GetCreature() const { return PBrain->PCreature; }
};
struct CAIStateGroup_EatAtTable : CAIStateGroupBase {
    CBaseIntelligentPointer Table;              // +0x14
    long SeatIndex;                             // +0x1c
    long SeatActionPointIndex;                  // +0x20
    virtual void OnReplace();
};

void CAIStateGroup_EatAtTable::OnReplace()
{
    CTCCreatureModeManager* modes;
    GetCreature()->GetInterface(&modes, 0x31);
    modes->RemoveMode(0x28);
    bool removeSitMode = true;
    if (Table.GetPItem()) {
        CTCSeatTable* table;
        if (!Table.GetPItem()->GetInterface(&table, 0xb8))
            return;
        if (table->IsSeatTaken(SeatIndex)) {
            bool gotUp;
            {
                CCharString anim("SIT_AT_TABLE_OUTOF");
                gotUp = GetUpFromSitting(GetCreature(), 11, &anim, PBrain);
            }
            if (gotUp)
                removeSitMode = false;
        }
        table->FreeSeat(SeatIndex, SeatActionPointIndex);
        table->SeatUsers[SeatIndex] = 0;
        CTCCarrying* carrying;
        GetCreature()->GetInterface(&carrying, 0x46);
        CThing* item = carrying->GetThingInPrimarySlot();
        if (item && !(item->m_flags91 & 1)) {
            carrying->RemoveThingFromPrimarySlot(false);
            if (item->HasTC(0xba))
                item->GetTC<CTCTavernTankard>(0xba)->ReplaceTankard();
            else {
                item->Kill(true);
                table->SetOccupant(SeatIndex, 0);
            }
        }
    }
    if (removeSitMode)
        modes->RemoveMode(11);
}