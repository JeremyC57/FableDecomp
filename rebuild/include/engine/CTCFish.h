#ifndef FABLE_ENGINE_CTCFISH_H
#define FABLE_ENGINE_CTCFISH_H
// New header (w22 agent2057 scout, row 0x007CACB0 CTCHeroReceiveItems::GlobalUpdate): the fish thing component on the polymorphic
// CTCBase (define FABLE_RETAIL_CTCBASE and FABLE_RETAIL_DEF_CLASS_BASE). Layout: Ego_r.pdb CTCFish, 28 bytes (struct_layouts_egor.tsv:
// PDef CDefPointer<const CFishDef> +0x0C, Length +0x10, Weight +0x14, Speed +0x18; no container member, so retail = Ego_r). Retail
// GlobalUpdate reads Length at [fish+10h] and Weight at [fish+14h] (0x007CB162 `mov ecx,[ebp+10h]`, `fld [ebp+14h]`).
// TCI_FISH 220: FableWin.pdb ?StatGetInterfaceType@CTCFish@@SI?AW4ETCInterfaceType@@XZ (0x02371680) + the row's PeekTC key 0DCh and
// HasTC bit (word +0x38, 10000000h). FableWin.pdb inline ?GetLength@CTCFish@@QBEMXZ (0x026D8DC0) / ?GetWeight@CTCFish@@QBEMXZ
// (0x026D8E00), 17 bytes / 9 instructions each in FableWin; no Ego_r.pdb / Fable.exe copy (expanded in retail). Other FableWin
// members (GetPDef, GetSpeed, SetLength, SetWeight) are not declared: not needed.
#include "engine/CTCBase.h"
#include "engine/ETCInterfaceType.h"
#include "engine/CDefPointer.h"
class CFishDef;
class CTCFish : public CTCBase {
public:
    static ETCInterfaceType __fastcall StatGetInterfaceType() { return TCI_FISH; }
#ifdef FABLE_RETAIL_CREATEFISH_PASS29
    CDefPointer<const CFishDef> GetPDef() const { return PDef; }
    void SetLength(float x) { Length=x; }
    void SetWeight(float x) { Weight=x; }
#endif
    float GetLength() const { return Length; }
    float GetWeight() const { return Weight; }
#ifdef FABLE_RETAIL_CTCFISH_MOSTRECENTFISHWEIGHT
    // NF92 opt-in (round 7, q11209 row 0x005BEDCB NInventory::CItem::UpdateItemStats): Ego_d.pdb PUB32 ?MostRecentFishWeight@CTCFish@@2MA
    // (public static float), retail 0x013BAE80: CGameScriptInterface::UpdateFishWeight 0x008959B0 stores the caught fish's Weight (+0x14)
    // there and UpdateItemStats keeps it as the item's best weight when heavier, then zeroes it. Declaration only: no layout change.
    static float MostRecentFishWeight;                                   // 0x013BAE80
#endif
private:
    CDefPointer<const CFishDef> PDef;                                    // +0x0c
    float Length;                                                        // +0x10
    float Weight;                                                        // +0x14
    float Speed;                                                         // +0x18
};
FABLE_STATIC_ASSERT_AT(26, sizeof(CTCFish) == 0x1c);
#endif
