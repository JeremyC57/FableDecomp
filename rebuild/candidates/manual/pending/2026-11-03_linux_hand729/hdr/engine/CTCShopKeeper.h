#ifndef FABLE_ENGINE_CTCSHOPKEEPER_H
#define FABLE_ENGINE_CTCSHOPKEEPER_H
// Retail model (chunk 729, hand-written, declarations only): Ego_r.pdb CTCShopKeeper (TCI_SHOP_KEEPER 144; members Customer, Shop,
// PreviousCustomers, CustomerIndex, OtherPeopleInShop, FrameLastCalcedOtherPeopleInShop from +0x0C are not modelled, so no size is
// asserted). ?GetShopTC@CTCShopKeeper@@QBEPAVCTCShop@@XZ is retail 0x00798F50.
#include "engine/CTCBase.h"
#include "engine/ETCInterfaceType.h"
class CTCShop;
class CTCShopKeeper : public CTCBase {
public:
    static ETCInterfaceType __fastcall StatGetInterfaceType() { return TCI_SHOP_KEEPER; }
    CTCShop* GetShopTC() const;                                          // 0x00798F50
};
#endif
