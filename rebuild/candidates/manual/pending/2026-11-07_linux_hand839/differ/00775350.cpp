// CTCShop::UpdateStock() -- advances the shop's stock by the days since it was last checked: per day each stocked item's MaxStock reverts towards the shop def's default by MaxStockRevertPerDay and the item restocks to MaxStock on its restock period (selling SalesPerDay otherwise), or, after a long gap, the revert is applied in bulk either side of the last restock day; stock entries the def no longer lists that are empty and never restock are erased, the special item expires after NumDaysToKeepSpecialItem, and with enough gold donated a random def item at SpecialItemPriceMult may be placed as the new special item; DayLastChecked is brought up to date
// Grade: DIFFER(1971v1988) -- function-wide inline budget split (documented blocker family): retail auto-inlines the TU-local MaxStock revert helper 0x007720D0 at two of its five sites (the big-gap `period == 0` site and the per-day loop) and calls it at the other three, and expands both _Rb_tree::erase(key) bodies (equal_range in place, erase(first,last) 0x007775B0 called) and random_shuffle; VC7.1 here expands the helper at all five sites (static / extern, __stdcall / __fastcall, inline / non-inline, fabs(double): a3-a7) and then has no budget left for erase(key) x2 and random_shuffle (called out of line). Everything else matches: GetDef / GetSubDef / CDefPointer shape, the stock loop, the sorted listed-types vector with binary_search, the special-item expiry, the roll (GFFloatRandom inline body: expanded at the roll, called from the per-day loop as retail) and the three vector frees. The /d2inlS5/20/60 meters (measurement only) do not split the helper sites.
// Next idea: find what made retail's helper expensive to the inliner -- try it as a template (`template<class T> T MoveTowards(T, T, T)`), as a member of a helper struct, or with the step argument taken by const reference (the three called sites push the step first via `fstp [esp]`); if the budget order is loop-depth-first, moving the big-gap branch into its own static function (inlined once) may reproduce the 2-expanded / 3-called split.
// Leaf: ?UpdateStock@CTCShop@@QAEXXZ
// Retail: Fable.exe (TLC, VC7.1) @ 0x00775350, 1988 bytes, thiscall, plain `ret`; frame 0x54, ebx/ebp/esi/edi.
// Identity (two sources): Ego_r.pdb / FableWin.pdb ?UpdateStock@CTCShop@@QAEXXZ (engine/CTCShop.h FABLE_RETAIL_CTCSHOP declares it);
//   the landed FrameUpdate 0x00775B40 calls it, and OnInitialActivate 0x00775B20 tail-jumps to it.
// Callees: CTCBase::GetDefinitionManager 0x006869D0, CGameDefinitionManager::GetDef<CThingBaseDef> 0x0042AF3C, CDefClassBase::
//   GetSubDef<CShopDef> 0x005130A0, CTCBase::GetWorld 0x00686A70, CTCBase::GetPWorldSeed 0x00686DA0, GFFloatRandom 0x004978A0,
//   GFROR13 0x00497890, rand 0x00BFEB16, _CIpow 0x00BFEB40, _CIfmod 0x00BFEB28, CTCShop::FindDefaultStockItem 0x00772250, the
//   max-stock revert helper 0x007720D0 (descriptive: MoveTowards(value, target, step); expanded at two sites, called at three),
//   vector<long>::_M_insert_overflow 0x00440990, sort<long*> 0x00778540, map<long,CShopItem>::operator[] 0x00778280,
//   _Rb_tree<long,...,CShopItem>::erase(iterator, iterator) 0x007775B0, free 0x00BFEA14.
// Layout: CTCShop (engine/CTCShop.h) ParentThing +0x04, Stock +0x34, SpecialItem +0x40, DaysSinceSpecialItemPlaced +0x44,
//   DayLastChecked +0x48, GoldDonated +0x4c; CThing DefGlobalIndex +0x8c; CWorld PGameTimeManager +0x1c, CGameTimeManager DayCount
//   +0x14; CShopDef (Ego_r.pdb) DefaultStock +0x40, MaxStockRevertPerDay +0x88, NumDaysToKeepSpecialItem +0xc0,
//   MinAmountBeforeSpecialItem +0xc4, ProbabilityOfSpecialItemPerDay +0xc8, SpecialItemPriceMult +0xcc; CShopItemDef Type +0x28,
//   Item.PriceMultiplier +0x30.
#define FABLE_RETAIL_CCHARSTRING
#define FABLE_RETAIL_CCLASS
#define FABLE_RETAIL_DEF_CLASS_BASE
#define FABLE_RETAIL_CDEFSTRINGTABLE
#define FABLE_RETAIL_CRESOURCELIST
#define FABLE_RETAIL_CDEFINITIONMANAGER
#define FABLE_RETAIL_CGAMEDEFINITIONMANAGER
#define FABLE_RETAIL_CTCBASE
#define FABLE_RETAIL_CTCBASE_WORLDSEED
#define FABLE_RETAIL_CTHING
#define FABLE_RETAIL_CINTELLIGENTPOINTER
#define FABLE_RETAIL_CWORLD
#define FABLE_RETAIL_CTCSHOP
#define FABLE_RETAIL_STLP_MAP
#define FABLE_RETAIL_STLP_TREE_ERASE
#define FABLE_RETAIL_STLP_TREE_ERASE_KEY
#define FABLE_RETAIL_STLP_SORT_FASTCALL
#define FABLE_RETAIL_STLP_BINARY_SEARCH
#define FABLE_RETAIL_STLP_RANDOM_SHUFFLE
#include <math.h>
#include "stlport/fable_containers.h"
#include "fable_string.h"
#include "engine/CThing.h"
#include "engine/CTCBase.h"
#include "engine/CIntelligentPointer.h"
#include "engine/CWorld.h"
#include "engine/CDefPointer.h"
#include "engine/CDefClassBase.h"
#include "engine/CDefString.h"
#include "engine/CDefinitionManager.h"
#include "engine/CGameDefinitionManager.h"
#include "engine/CThingBaseDef.h"
#include "engine/CShopItemDef.h"
#include "engine/CTCShop.h"
class CGameTimeManager {                                            // no shared header (Ego_r.pdb layout; the landed FrameUpdate model)
public:
    long GetDayCount() const { return DayCount; }
private:
    CWorld*             World;                                      // +0x00
    CMainGameComponent* GameComponent;                              // +0x04
    float               Time;                                       // +0x08
    float               PrevTime;                                   // +0x0c
    float               TimeStep;                                   // +0x10
    long                DayCount;                                   // +0x14
};
class CShopDef : public CDefClassBase {                             // Ego_r.pdb CShopDef (retail 0xd0): only the members this row reads
public:
    unsigned char _pad_0x08[0x40 - sizeof(CDefClassBase)];
    std::vector<CShopItemDef> DefaultStock;                         // +0x40
    unsigned char _pad_0x4c[0x88 - 0x4c];
    float MaxStockRevertPerDay;                                     // +0x88
    unsigned char _pad_0x8c[0xc0 - 0x8c];
    long  NumDaysToKeepSpecialItem;                                 // +0xc0
    long  MinAmountBeforeSpecialItem;                               // +0xc4
    float ProbabilityOfSpecialItemPerDay;                           // +0xc8
    float SpecialItemPriceMult;                                     // +0xcc
};
FABLE_STATIC_ASSERT(sizeof(CShopDef) == 0xd0);
long FABLE_FASTCALL GFROR13(unsigned long value);                                     // 0x00497890 ?GFROR13@@YIJK@Z (FableWin.pdb, engine/GFROR13.h)
extern "C" int rand(void);                                                            // 0x00BFEB16
namespace std { template<class _RandomAccessIter> void __fastcall sort(_RandomAccessIter, _RandomAccessIter); }   // 0x00778540
// GFFloatRandom 0x004978A0 (?GFFloatRandom@@YIMMPAK@Z, __fastcall: seed in ecx, the float on the stack): the retail body (max * 65536
// as the fmod range, the seed stepped `* 0x24A1 + 0x24DF` then rotated by GFROR13, `fild; fadd 2^32` unsigned conversion, _CIfmod,
// times 1 / 65536; 0 when the range is at most 0.0001). One inline body: retail expands it at the special-item roll (max = 1.0f
// folds the range test away) and calls the out-of-line copy from the per-day sales loop.
inline float FABLE_FASTCALL GFFloatRandom(float max, unsigned long* seed)
{
    double range = max * 65536.0;
    *seed = *seed * 0x24a1 + 0x24df;
    *seed = GFROR13(*seed);
    if (range <= 0.0001)
        return 0.0f;
    return (float)(fmod((double)*seed, range) * (1.0 / 65536.0));
}
template<> void std::vector<long>::_M_insert_overflow(long* position, const long& x, const std::__true_type&, unsigned int fillLength, bool atEnd);   // 0x00440990 (out of line in retail)
// descriptive: move `value` towards `target` by `step` (snap when within step); the TU-local 0x007720D0 (three floats, callee-cleaned,
// placed before FindDefaultStockItem 0x00772250): VC7.1 auto-inlines it at two of the five sites and calls it at the other three
static float __stdcall MoveTowards(float value, float target, float step)
{
    float diff = value - target;
    if (fabsf(diff) < step)
        return target;
    if (diff > 0.0f)
        return value - step;
    return value + step;
}

void CTCShop::UpdateStock()
{
    CDefPointer<const CThingBaseDef> thingDef;
    GetDefinitionManager().GetDef(ParentThing->GetDefGlobalIndex(), &thingDef);
    CDefPointer<const CShopDef> shopDef;
    if (!thingDef->GetSubDef(&shopDef))
        return;
    long day = GetWorld().PeekGameTimeManager().GetDayCount();
    long daysSince = day - DayLastChecked;
    std::map<long, CShopItem>::iterator it;
    for (it = Stock.begin(); it != Stock.end(); ++it) {
        CShopItem& item = it->second;
        if (daysSince > 100000) {
            long period = item.RestockPeriod;
            if (period == 0) {
                item.MaxStock = MoveTowards(item.MaxStock, FindDefaultStockItem(it->first)->MaxStock, (float)daysSince * shopDef->MaxStockRevertPerDay);
            } else {
                long lastRestock = ((day - 100000) / period) * period;
                if (lastRestock > DayLastChecked) {
                    float days = (float)daysSince;
                    item.MaxStock = MoveTowards(item.MaxStock, FindDefaultStockItem(it->first)->MaxStock, (float)(lastRestock - DayLastChecked) * shopDef->MaxStockRevertPerDay * days);
                    item.MaxStock = MoveTowards(item.MaxStock, FindDefaultStockItem(it->first)->MaxStock, (float)(day - lastRestock - 100000) * shopDef->MaxStockRevertPerDay * days);
                } else {
                    item.MaxStock = MoveTowards(item.MaxStock, FindDefaultStockItem(it->first)->MaxStock, (float)daysSince * shopDef->MaxStockRevertPerDay);
                }
            }
        } else {
            for (long d = DayLastChecked + 1; d <= day; d++) {
                float revert = shopDef->MaxStockRevertPerDay;
                item.MaxStock = MoveTowards(item.MaxStock, FindDefaultStockItem(it->first)->MaxStock, revert);
                if (item.RestockPeriod != 0) {
                    if (d % item.RestockPeriod == 0) {
                        item.Quantity = (std::max)(item.Quantity, item.MaxStock);
                    } else {
                        GFFloatRandom(1.0f, GetPWorldSeed());
                        item.Quantity = (std::max)(item.Quantity - item.SalesPerDay, 0.0f);
                    }
                }
            }
        }
    }
    std::vector<long> listed;
    for (const CShopItemDef* def = shopDef->DefaultStock.begin(); def != shopDef->DefaultStock.end(); ++def)
        listed.push_back(def->Type);
    std::sort(listed.begin(), listed.end());
    std::vector<long> unlisted;
    for (it = Stock.begin(); it != Stock.end(); ++it) {
        if (!std::binary_search(listed.begin(), listed.end(), it->first)) {
            if (it->second.Quantity == 0.0f && (it->second.RestockPeriod == 0 || it->second.MaxStock == 0.0f))
                unlisted.push_back(it->first);
        }
    }
    for (long* key = unlisted.begin(); key != unlisted.end(); ++key)
        Stock.erase(*key);
    if (SpecialItem != -1) {
        DaysSinceSpecialItemPlaced += daysSince;
        if (DaysSinceSpecialItemPlaced > shopDef->NumDaysToKeepSpecialItem) {
            Stock.erase(SpecialItem);
            SpecialItem = -1;
        }
    }
    if (SpecialItem == -1) {
        if (GoldDonated > shopDef->MinAmountBeforeSpecialItem) {
            float chance = (float)pow(1.0f - shopDef->ProbabilityOfSpecialItemPerDay, (float)daysSince);
            if (GFFloatRandom(1.0f, GetPWorldSeed()) > chance) {
                GoldDonated -= shopDef->MinAmountBeforeSpecialItem;
                std::vector<long> candidates;
                for (const CShopItemDef* def = shopDef->DefaultStock.begin(); def != shopDef->DefaultStock.end(); ++def) {
                    if (fabsf(def->Item_PriceMultiplier - shopDef->SpecialItemPriceMult) < 0.0001f)
                        candidates.push_back(def->Type);
                }
                if (candidates.size() != 0) {
                    std::random_shuffle(candidates.begin(), candidates.end());
                    SpecialItem = candidates[0];
                    const CShopItem* def = FindDefaultStockItem(SpecialItem);
                    if (def) {
                        Stock[SpecialItem] = *def;
                        Stock[SpecialItem].Quantity = 1.0f;
                    }
                    DaysSinceSpecialItemPlaced = 0;
                }
            }
        }
    }
    DayLastChecked = day;
}
