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
class CTCShopKeeper : public CTCBase {
public:
    CThingCreatureBase* GetCustomer() const;                             // 0x00798D80
protected:
    CIntelligentPointer<CThingCreatureBase> Customer;                    // +0x0c
    CIntelligentPointer<CThing> Shop;                                    // +0x14
};
#endif
