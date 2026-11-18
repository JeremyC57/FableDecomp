#ifndef FABLE_ENGINE_CBUYABLEHOUSEDEF_RETAIL_H
#define FABLE_ENGINE_CBUYABLEHOUSEDEF_RETAIL_H
// New header (egod e10, row 0x006C14D0 CTCBuyableHouse::OnInitialActivate). Only the generated pad model
// engine/_quarantine/CBuyableHouseDef.h existed. RTTI .?AVCBuyableHouseDef@@ : CSubDefClassBase (Fable.exe; landed 0x006C1B00 /
// 0x006C1250 model it so). Members: Ego_d.pdb class record 0x2F237 (field list 0x2F236: Copy, GetSizeofClass, Alloc, StatGetTypeName,
// IsBuyable, InitiallyEmpty, Price, Rent, DaysBetweenRentPayments, MaxRentBags, IsScripted, IsBuyableWithoutSignpost, Transfer) = Ego_r.pdb
// struct_layouts_egor.tsv names, at the retail offsets (packed 0x25-byte base; STLport vectors 12 bytes: Price +0x28, Rent +0x34, sizeof
// 0x4C; landed 0x006BF9E0 reads IsScripted +0x48, IsBuyableWithoutSignpost +0x49). The inline
// ?StatGetTypeName@CBuyableHouseDef@@SI?AVCCharString@@XZ (Ego_r.pdb / FableWin.pdb) builds the 'CBuyableHouseDef' literal the landed
// GetSubDef<CBuyableHouseDef> 0x006C1B00 passes to GetPSubDefInfo; with it a row can use the shared GetSubDef body
// (FABLE_RETAIL_CDEFCLASSBASE_GETSUBDEF_BODY). Needs FABLE_RETAIL_DEF_CLASS_BASE and fable_string.h.
#include <stddef.h>
#include "fable_string.h"
#include "stlport/fable_containers.h"
#include "engine/CSubDefClassBase.h"
class CBuyableHouseDef : public CSubDefClassBase {
public:
    static CCharString FABLE_FASTCALL StatGetTypeName() { return CCharString("CBuyableHouseDef"); }
    bool IsBuyable;                                                      // +0x25
    bool InitiallyEmpty;                                                 // +0x26
    std::vector<long> Price;                                             // +0x28
    std::vector<long> Rent;                                              // +0x34
    long DaysBetweenRentPayments;                                        // +0x40
    long MaxRentBags;                                                    // +0x44
    bool IsScripted;                                                     // +0x48
    bool IsBuyableWithoutSignpost;                                       // +0x49
};
FABLE_STATIC_ASSERT(offsetof(CBuyableHouseDef, Price) == 0x28);
FABLE_STATIC_ASSERT(offsetof(CBuyableHouseDef, IsScripted) == 0x48);
FABLE_STATIC_ASSERT(sizeof(CBuyableHouseDef) == 0x4c);
#endif // FABLE_ENGINE_CBUYABLEHOUSEDEF_RETAIL_H
