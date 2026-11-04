// CAIStateGroup_EatAtTable::OnReplace() -- drop modes 0x28, free the table seat (getting up when seated), drop/replace the carried tankard, then drop the sitting mode unless the get-up took it
// Plan phase 2 (2026-11-04), CVectorMap wave (stand-in map): the local CInterfaceMap stand-in (and its same-TU LowerBound body) replaced by the shared engine/CVectorMap.h (Ego_r: CVectorMap<ETCInterfaceType,CTCBase*,CKeyPairCompareLess<..> > derives CArray<pair<K,V> >, Compare +0x0C, Dirty +0x0D; the same-TU noinline LowerBound is the header's); entries read as pair first/second, the map end through end(). Spellings unified: 0040f020: ?LowerBound@CInterfaceMap@@QAEPAUSEntry@1@PBJ@Z -> ?LowerBound@?$CVectorMap@W4ETCInterfaceType@@PAVCTCBase@@V?$CKeyPairCompareLess@W4ETCInterfaceType@@PAVCTCBase@@@@@@QAEPAU?$pair@W4ETCInterfaceType@@PAVCTCBase@@@std@@ABW4ETCInterfaceType@@@Z
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
class CTCBase;
enum ETCInterfaceType { };                                   // CTCBase interface type (values unused here)
#include "engine/CVectorMap.h"
typedef CVectorMap<ETCInterfaceType, CTCBase*, CKeyPairCompareLess<ETCInterfaceType, CTCBase*> > CInterfaceMap;

class CThing { public:
    unsigned char _pad0[0x20];
    unsigned long m_presentBits[9];             // +0x20
    CInterfaceMap m_interfaces;                 // +0x44
    unsigned char _pad9[0x3d];
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
        CInterfaceMap::value_type* it = map->LowerBound((const ETCInterfaceType&)type);
        if (it == map->end() || interfaceType < it->first)
            it = map->end();
        return (T*)it->second;
    }
    template<class T> __forceinline bool GetInterface(T** ppOut, long interfaceType)
    {
        if (m_presentBits[interfaceType >> 5] & (1UL << (interfaceType & 31))) {
            long type = interfaceType;
            CInterfaceMap* map = &m_interfaces;
            CInterfaceMap::value_type* it = map->LowerBound((const ETCInterfaceType&)type);
            if (it == map->end() || interfaceType < it->first)
                it = map->end();
            *ppOut = (T*)it->second;
            return true;
        }
        return false;
    }
};
class CBaseObject;
class CBaseObjectPointer;
class CBaseIntelligentPointer {                       // vtable 0x0129C95C (Ego_r: vptr + PData, 8 bytes)
public:
    CBaseIntelligentPointer();                           // 0x00A01B10
    virtual ~CBaseIntelligentPointer();                  // 0x00A01C10
    CBaseObject* GetPItem() const;                       // 0x00A01B50
    void SetPItem(CBaseObject* item);                    // 0x00A01B90
private:
    CBaseObjectPointer* PData;                           // +0x04
};
template <class T> class CIntelligentPointer : public CBaseIntelligentPointer {
public:
    T* GetPItem() const { return (T*)CBaseIntelligentPointer::GetPItem(); }
    void SetPItem(T* item) { CBaseIntelligentPointer::SetPItem((CBaseObject*)item); }
};
class CCharString { public:
    void* m_pData;
    CCharString(const char* text, long length = -1);   // 0x0099EBF0
    ~CCharString();                                     // 0x0099EAE0
};
class CAIBrain { public:
    unsigned char _pad[0x18];
    CThing* PCreature;                          // +0x18
};
bool __fastcall GetUpFromSitting(CThing* creature, long movement, const CCharString* anim, CAIBrain* brain);   // 0x0085FF10
class CTCCreatureModeManager { public:                 // interface 0x31
    void RemoveMode(unsigned long mode);        // 0x0073ABA0
};
class CTCCarrying { public:                            // interface 0x46
    CThing* GetThingInPrimarySlot() const;            // 0x006A1440
    void RemoveThingFromPrimarySlot(bool b);    // 0x006A1E00
};
class CTCTavernTankard { public:                       // interface 0xba
    void ReplaceTankard();                      // 0x00947C30
};
struct CTCSeatTable {                           // interface 0xb8
    unsigned char _pad[0x10];
    CThing** SeatUsers;                         // +0x10
    bool IsSeatTaken(long seat);                // 0x007B8570
    void FreeSeat(long seat, long actionPoint); // 0x007B8AA0
    void SetOccupant(long seat, CThing* occupant);   // 0x007B85F0
};
class CAIStateGroupBase { public:
    virtual ~CAIStateGroupBase();
    CAIBrain* PBrain;                           // +0x04
    unsigned char _pad[0x0c];
    CThing* GetCreature() const { return PBrain->PCreature; }
};
struct CAIStateGroup_EatAtTable : CAIStateGroupBase {
    CIntelligentPointer<CThing> Table;              // +0x14
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