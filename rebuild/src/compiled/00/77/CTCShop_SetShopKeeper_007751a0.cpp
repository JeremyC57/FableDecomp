// CTCShop::SetShopKeeper(CThingCreatureBase* keeper) -- replace the shop keeper: unlock the old keeper's workplace and clean the stock up, take the new keeper, then refresh the display stock and lock the new keeper's workplace
// Leaf: ?SetShopKeeper@CTCShop@@QAEXPAVCThingCreatureBase@@@Z
// Flags: none
// Retail: Fable.exe (TLC, VC7.1) @ 0x007751A0, 419 bytes; Ego_r.pdb ?SetShopKeeper@CTCShop@@QAEXPAVCThingCreatureBase@@@Z.
// Callees: CTCShop::IsPermanentlyStocked const 0x007721A0, StockDisplay 0x00774C60, SetStockState 0x00773C00,
//   RemoveStockItemsFromShop 0x00773E10, CBaseIntelligentPointer ctor/GetPItem/SetPItem/dtor 0x00A01B10/0x00A01B50/0x00A01B90/
//   0x00A01C10, CVectorMap<ETCInterfaceType,CTCBase*>::LowerBound 0x0040F020 (TCI_TRESPASSABLE 189); TCI_TAVERN 171 is a bit test.
// Layout: Ego_r CTCShop PShopKeeper +0x10 (CIntelligentPointer<CThingCreatureBase>), CTCTrespassable::Trespassable +0x0c.
// Fix (chunk 759): engine/CBaseObject.h + engine/CIntelligentPointer.h (FABLE_RETAIL_CINTELLIGENTPOINTER_ASSIGN for
//   `PShopKeeper = item`), the FableWin.pdb inline __fastcall templates operator==(const CIntelligentPointer<T>&, const CBaseObject*)
//   and IS_THING_ALIVE(const T&) (VC7.1 then reloads ParentThing for the TCI_TAVERN test as retail does: `Get() == ParentThing`
//   reuses edi, 3 diff lines), the keeper test and the tavern test as one `||` condition over a single SetStockState(0) (an
//   if / else-if pair duplicates the call setup: 2 lines), and PeekTC over the real engine/CVectorMap.h with std::bitset
//   component bits (the old draft's eax/ecx swap after GetPItem: 8 lines).
#define FABLE_RETAIL_CVECTORMAP_FORCEINLINE_FIND
#include "engine/ETCInterfaceType.h"
#include "engine/CVectorMap.h"
class CTCBase { public: virtual ~CTCBase(); };
class CTCTrespassable : public CTCBase { public:                  // TCI_TRESPASSABLE
    unsigned char _pad04[0x0c - 0x04];
    bool Trespassable;                              // +0x0c
};
#define FABLE_RETAIL_BASE_OBJECT
#include "engine/CBaseObject.h"
#define FABLE_RETAIL_CINTELLIGENTPOINTER
#define FABLE_RETAIL_CINTELLIGENTPOINTER_ASSIGN
#include "engine/CIntelligentPointer.h"
class CThing : public CBaseObject {
public:
    unsigned char  _pad08[0x20 - 0x08];
    std::bitset<274> UsingComponents;               // +0x20
    CVectorMap<ETCInterfaceType, CTCBase*, CKeyPairCompareLess<ETCInterfaceType, CTCBase*> > Components;   // +0x44
    unsigned char  _pad54[0x91 - 0x54];
    unsigned char  m_flags91;                       // +0x91 bit 0: dead
    unsigned char  _pad92[0xd4 - 0x92];
    unsigned char  m_houseLockingOverridden : 1;    // +0xd4 bit 0 (building)
    unsigned char  _padd5[0x170 - 0xd5];
    CIntelligentPointer<CThing> PWorkplace;         // +0x170 (creature; descriptive)
    CThing* GetPWorkplace() const { return PWorkplace.Get(); }
    bool HasTC(ETCInterfaceType type) const { return UsingComponents.test(type); }
    template<class T> __forceinline bool PeekTC(ETCInterfaceType id, T** ptc)
    {
        if (HasTC(id)) {
            ETCInterfaceType type = id;
            CVectorMap<ETCInterfaceType, CTCBase*, CKeyPairCompareLess<ETCInterfaceType, CTCBase*> >* map = &Components;
            std::pair<ETCInterfaceType, CTCBase*>* it = map->LowerBound(type);
            if (it == map->end() || id < it->first)
                it = map->end();
            *ptc = static_cast<T*>(it->second);
            return true;
        }
        return false;
    }
};
class CThingCreatureBase : public CThing { };
// FableWin.pdb ??$?8VCThingCreatureBase@@@@YI_NABV?$CIntelligentPointer@VCThingCreatureBase@@@@PBVCBaseObject@@@Z and
// ??$IS_THING_ALIVE@V?$CIntelligentPointer@VCThingCreatureBase@@@@@@YI_NABV?$CIntelligentPointer@VCThingCreatureBase@@@@@Z (both inline).
template<class T> inline bool __fastcall operator==(const CIntelligentPointer<T>& p, const CBaseObject* item) { return p.GetPItem() == item; }
template<class T> inline bool __fastcall IS_THING_ALIVE(const T& p)
{
    if (p.Get() != 0) {
        if (!(p.Get()->m_flags91 & 1))
            return true;
    }
    return false;
}
class CTCShop { public:
    virtual void v00();
    CThing* ParentThing;                            // +0x04
    bool ToBeRemoved;                               // +0x08
    bool IsInGlobalUpdateTCs;                       // +0x09
    long StockState;                                // +0x0c
    CIntelligentPointer<CThingCreatureBase> PShopKeeper;   // +0x10
    bool IsPermanentlyStocked() const;              // 0x007721A0
    void StockDisplay();                            // 0x00774C60
    void SetStockState(long state);                 // 0x00773C00
    void RemoveStockItemsFromShop();                // 0x00773E10
    void SetWorkplaceLocked(bool locked);
    void SetShopKeeper(CThingCreatureBase* keeper);
};

void CTCShop::SetShopKeeper(CThingCreatureBase* keeper)
{
    if (keeper == PShopKeeper.Get())
        return;
    if (IS_THING_ALIVE(PShopKeeper)) {
        CThing* workplace = PShopKeeper.Get()->GetPWorkplace();
        if (workplace && !(workplace->m_flags91 & 1) && !workplace->HasTC(TCI_TAVERN)) {
            workplace->m_houseLockingOverridden = 0;
            CTCTrespassable* trespassable;
            if (workplace->PeekTC(TCI_TRESPASSABLE, &trespassable))
                trespassable->Trespassable = false;
        }
        RemoveStockItemsFromShop();
    }
    {
        CIntelligentPointer<CThingCreatureBase> item(keeper);
        PShopKeeper = item;
    }
    if (IS_THING_ALIVE(PShopKeeper)) {
        if (IsPermanentlyStocked()) {
            StockDisplay();
            if (PShopKeeper == ParentThing || ParentThing->HasTC(TCI_TAVERN))
                SetStockState(0);
        }
        CThing* workplace = PShopKeeper.Get()->GetPWorkplace();
        if (workplace && !(workplace->m_flags91 & 1) && !workplace->HasTC(TCI_TAVERN)) {
            workplace->m_houseLockingOverridden = 1;
            CTCTrespassable* trespassable;
            if (workplace->PeekTC(TCI_TRESPASSABLE, &trespassable))
                trespassable->Trespassable = true;
        }
    }
}