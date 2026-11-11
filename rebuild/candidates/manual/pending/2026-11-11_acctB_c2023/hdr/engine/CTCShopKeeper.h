#ifndef FABLE_ENGINE_CTCSHOPKEEPER_H
#define FABLE_ENGINE_CTCSHOPKEEPER_H
// CTCShopKeeper (new, chunk 765): RTTI .?AVCTCShopKeeper@@, single base CTCBase at +0 (ghidra_out/rtti_bases.tsv). Ego_r.pdb members
// (struct_layouts_egor.tsv): Customer (CIntelligentPointer<CThingCreatureBase>) +0x0C, Shop (CIntelligentPointer<CThing>) +0x14, then a
// vector (PDB 16 bytes, retail 12) +0x1C; GetCustomer 0x00798D80 reads +0x0C. Name set: ?GetCustomer@CTCShopKeeper@@QBEPAVCThingCreatureBase@@XZ.
// Only the members and methods rows use are modelled (no virtual overrides).
#ifndef FABLE_RETAIL_CTCBASE
#define FABLE_RETAIL_CTCBASE
#endif
#include "engine/CTCBase.h"
#include "engine/CIntelligentPointer.h"
#include "engine/CThingCreatureBase.h"
#ifdef FABLE_RETAIL_CTCSHOPKEEPER_SHOP
class CTCShop;                                                           // chunk 795
#endif
class CTCShopKeeper : public CTCBase {
public:
#ifdef FABLE_RETAIL_CTCWIFE_MARRY
    void DetachFromShop();
#endif
    CThingCreatureBase* GetCustomer() const;                             // 0x00798D80
#ifdef FABLE_RETAIL_CTCSHOPKEEPER_SHOP
    // Retail opt-in (chunk 795, row 0x0092FAB0): the interface key (TCI_SHOP_KEEPER, the row's inline GetTC key 0x90) and Ego_r.pdb
    // ?GetShopTC@CTCShopKeeper@@QBEPAVCTCShop@@XZ (0x00798F50, landed) / ?GetHint@CTCShopKeeper@@QAE?AVCCharString@@XZ (0x00798D30: Ego_r
    // orders GetHint 0x0079E7C0 before GetShopTC 0x0079E900, as retail; the body returns a CCharString built from one literal).
    static ETCInterfaceType __fastcall StatGetInterfaceType() { return TCI_SHOP_KEEPER; }
    CTCShop* GetShopTC() const;                                          // 0x00798F50
    CCharString GetHint();                                               // 0x00798D30
#endif
#ifdef FABLE_RETAIL_CTCSHOPKEEPER_CUSTOMER
    // Retail opt-in (chunk 2023, row 0x0094C050 CAIStateGroup_SellToBuyer::Update): Ego_r.pdb / FableWin.pdb
    // ?GetMinDistFromStall@CTCShopKeeper@@QAEJXZ / ?GetMaxDistFromCustomer@CTCShopKeeper@@QAEJXZ (Ego_r donors 0x0079E7A0 / 0x0079E7B0, just
    // before GetHint 0x0079E7C0, as retail 0x00798D10 `xor eax,eax` / 0x00798D20 `mov eax,0Ah` before GetHint 0x00798D30) and
    // ?AddNewCustomer@CTCShopKeeper@@QAEXV?$CIntelligentPointer@VCThingCreatureBase@@@@@Z (Ego_r 0x0079E860 right after GetCustomer
    // 0x0079E850, as retail 0x00798D90 after GetCustomer 0x00798D80). All out of line in retail: declarations only.
    long GetMinDistFromStall();                                          // 0x00798D10
    long GetMaxDistFromCustomer();                                       // 0x00798D20
    void AddNewCustomer(CIntelligentPointer<CThingCreatureBase> customer);   // 0x00798D90
#endif
protected:
    CIntelligentPointer<CThingCreatureBase> Customer;                    // +0x0c
    CIntelligentPointer<CThing> Shop;                                    // +0x14
};
#endif
