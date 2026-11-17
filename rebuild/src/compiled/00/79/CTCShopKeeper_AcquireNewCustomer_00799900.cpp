// CTCShopKeeper::AcquireNewCustomer() -- look for the nearest potential customer in the 30m square around the keeper (nearest to the shop); latch it as the current customer and remember it, else when nobody (not even an existing shopper) is around, empty the customer list
// Leaf: ?AcquireNewCustomer@CTCShopKeeper@@QAEXXZ
// Plan phase 5 (2026-11-05), CIntelligentPointer wave: the local CBaseIntelligentPointer / CIntelligentPointer stand-ins replaced by the shared engine/CIntelligentPointer.h, retail (byte-packed) branch FABLE_RETAIL_CINTELLIGENTPOINTER (Ego_r.pdb: vptr +0x0, PData +0x4; ctor 0x00A01B10, dtor 0x00A01C10, GetPItem 0x00A01B50, SetPItem 0x00A01B90).
// Plan phase 4 (2026-11-05), C3DVector wave: the local C3DVector stand-in replaced by the shared engine/C3DVector.h (PDB C3DVector TPI 0x2783: public X/Y/Z at 0/4/8, size 12).
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00799900, 351 bytes (plain `ret`).
// Callees: CTCShopKeeper::GetShopTC 0x00798F50; CThing::GetPos 0x004C73D0 (x2); __ftol2 0x00BFEA70 (x2);
//          CThingFilter_IsPotentialCustomer(CTCShopKeeper*, bool) 0x00799D80 (x2); CTCBase::GetThingSearchTools 0x00686D90;
//          CThingSearchTools::GetPBestThingInRoughArea<CThingFilter_IsPotentialCustomer, CThingCompare_Nearest,
//          CThingCreatureBase> 0x0079A050; CThingSearchTools::GetPFirstThingInRoughArea<CThingFilter_IsPotentialCustomer,
//          CThingCreatureBase> 0x0079A280 (manifest label is a propagated twin); CBaseIntelligentPointer ctor 0x00A01B10 /
//          SetPItem 0x00A01B90 / GetPItem 0x00A01B50 / dtor 0x00A01C10 (CIntelligentPointer<CThingCreatureBase> vtable
//          0x0123E7D8); CTCShopKeeper::AddNewCustomer(CIntelligentPointer) 0x00798D90; CTCShopKeeper::EmptyCustomerList 0x00798E10.
// Layout (Ego_r.pdb CTCShopKeeper, 72 bytes; retail as the landed GetShopTC/AddNewCustomer): ParentThing +0x04 (CTCBase),
//          Customer +0x0c; CThingFilter_IsPotentialCustomer 24 bytes; CThingCompare_Nearest 12 bytes (Pos).
// Shape: the shop position is a named copy taken before the keeper's own position; the box is a named C2DBoxI built from
//          two truncated coordinates (+/-15); the best-thing result is ignored and the out pointer latched through a
//          temporary; the by-value AddNewCustomer argument is copy-constructed in its stack slot.
// Name-fix (agentNF55, round 7): the CIntelligentPointerCreature / CTCShop / CTCShopKeeper stand-ins replaced by the shared
//   engine/CTCShopKeeper.h (opt-ins SHOP, CUSTOMER and NF55's ACQUIRE) over the retail CThing / CThingCreatureBase chain, so the
//   by-value argument is CIntelligentPointer<CThingCreatureBase> and AddNewCustomer binds Ego_r.pdb
//   ?AddNewCustomer@CTCShopKeeper@@QAEXV?$CIntelligentPointer@VCThingCreatureBase@@@@@Z; the two search-tool calls are the Ego_r.pdb
//   member-template instances ??$GetPBestThingInRoughArea@VCThingFilter_IsPotentialCustomer@@VCThingCompare_Nearest@@VCThingCreatureBase@@@
//   CThingSearchTools@@... / ??$GetPFirstThingInRoughArea@VCThingFilter_IsPotentialCustomer@@VCThingCreatureBase@@@CThingSearchTools@@...
// NF62 (2026-11-17, round 7): finished on the headers -- C2DBoxI (ORIGINAL_FOUR_COORD_CTOR), CThingCompare_Nearest and CThingSearchTools
//   (BESTROUGH / FIRSTROUGH member templates) from their shared headers, EmptyCustomerList / AcquireNewCustomer declared by the overlay
//   opt-in FABLE_RETAIL_CTCSHOPKEEPER_ACQUIRE; CThingFilter_IsPotentialCustomer (no shared header) stays local. Retail reads `customer`
//   after the temporary's base ctor 0x00A01B10 and stores the vptr after that read (0x007999B6..0x007999C4): the latch is a scoped
//   default-constructed CIntelligentPointer<CThingCreatureBase> set through FableWin.pdb ?Set@..QAEXPAV..@Z, then assigned; the by-value
//   (T*) constructor (the only PDB form) reads the pointer before the base ctor (NF55's DIFFER).
#define FABLE_RETAIL_CCHARSTRING
#define FABLE_RETAIL_BASE_OBJECT
#define FABLE_RETAIL_CCLASS
#define FABLE_RETAIL_DEF_CLASS_BASE
#define FABLE_RETAIL_CTCBASE
#define FABLE_RETAIL_CTHING
#define FABLE_RETAIL_CINTELLIGENTPOINTER
#define FABLE_RETAIL_CINTELLIGENTPOINTER_ASSIGN
#define FABLE_RETAIL_CINTELLIGENTPOINTER_COPY
#define FABLE_RETAIL_CTHINGPHYSICAL
#define FABLE_RETAIL_CTHINGGAMEOBJECT
#define FABLE_RETAIL_CTHINGCREATUREBASE
#define FABLE_RETAIL_CTCSHOPKEEPER_SHOP
#define FABLE_RETAIL_CTCSHOPKEEPER_CUSTOMER
#define FABLE_RETAIL_CTCSHOPKEEPER_ACQUIRE
#define FABLE_RETAIL_C3DVECTOR
#define FABLE_RETAIL_C2DBOXI_ORIGINAL_FOUR_COORD_CTOR
#define FABLE_RETAIL_CTHINGSEARCHTOOLS
#define FABLE_RETAIL_CTHINGSEARCHTOOLS_BESTROUGH
#define FABLE_RETAIL_CTHINGSEARCHTOOLS_FIRSTROUGH
#include "fable_string.h"
#include "engine/C3DVector.h"
#include "engine/CIntelligentPointer.h"
#include "engine/CThing.h"
#include "engine/CTCBase.h"
#include "engine/CThingCreatureBase.h"
#include "engine/CTCShopKeeper.h"
#include "engine/CTCShop.h"
#include "engine/C2DExtentsI.h"
#include "engine/C2DBoxI.h"
#include "engine/CThingCompare_Nearest.h"
#include "engine/CThingSearchTools.h"
class CThingFilter_IsPotentialCustomer { public:
    CThing* ShopperContainer;                      // +0x00
    C3DVector Pos;                                 // +0x04
    CTCShopKeeper* TCShopKeeper;                   // +0x10
    bool AllowExistingShopper;                     // +0x14
    CThingFilter_IsPotentialCustomer(CTCShopKeeper* shopKeeper, bool allowExistingShopper);   // 0x00799D80
};

void CTCShopKeeper::AcquireNewCustomer()
{
    CTCShop* shop = GetShopTC();
    if (!shop)
        return;
    C3DVector shopPos = shop->GetParentThing().GetPos();
    const C3DVector& pos = GetParentThing().GetPos();
    long x = (long)pos.X;
    long y = (long)pos.Y;
    C2DBoxI area(x - 15, y - 15, x + 15, y + 15);
    CThingCreatureBase* customer = 0;
    PeekThingSearchTools().GetPBestThingInRoughArea(area, CThingFilter_IsPotentialCustomer(this, false),
        CThingCompare_Nearest(shopPos), &customer);
    {
        CIntelligentPointer<CThingCreatureBase> new_customer;
        new_customer.Set(customer);
        Customer = new_customer;
    }
    if (Customer.GetPItem())
        AddNewCustomer(Customer);
    else if (!PeekThingSearchTools().GetPFirstThingInRoughArea(area, CThingFilter_IsPotentialCustomer(this, true), &customer))
        EmptyCustomerList();
}
