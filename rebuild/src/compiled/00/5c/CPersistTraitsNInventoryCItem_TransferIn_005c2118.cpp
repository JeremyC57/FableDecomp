// CPersistTraits<NInventory::CItem>::TransferIn(CPersistContext& persist_context, CStringParser& parser, NInventory::CItem& val) -- text load of an inventory item "( has_per_item_data , [ ( number_of_augmentations , augmentation... , kills , wielded , trophy [ , quest_persistent , ref_count ] ) , ] item_type_def_name , number_of_items , average_bought_price )": augmentation names other than "" / "NULL" become def indices, integer entries are skipped; any missing separator returns with the item unchanged
// Inline-budget: no flag (/d2inlT50..200 change nothing in this /Os TU); one per-TU FABLE_FORCEINLINE contract and two body / declaration opt-ins: FABLE_RETAIL_CCHARSTRING_FORCEINLINE_TEXT_EQ (existing; retail expands CCharString::operator==(const char*) at every separator, the null-data `repe cmpsb` against "" and the Compare 0x00411570 call; without it DIFFER(1115v1569)); FABLE_RETAIL_CBASICSTRING_TEXT_EQ_COMPARE_ZERO (existing; the visible CBasicString<char>::operator==(const char*) body, expanded inside it; without it DIFFER(1447v1569)); FABLE_RETAIL_NINVENTORY_CITEM_TRANSFERIN (new; besides declaring the 4-argument constructor it leaves ~CItem implicit, so the temporary's destruction expands to the ~CCountedPointer<CPerItemData> 0x005C04BF call at 0x005C270E; with the out-of-line ~CItem still declared: DIFFER(1569v1569), a ??1CItem call).
// Leaf: ?TransferIn@?$CPersistTraits@VCItem@NInventory@@@@QAEXAAVCPersistContext@@AAVCStringParser@@AAVCItem@NInventory@@@Z
// Flags: none
// Retail: Fable.exe (TLC, VC7.1) @ 0x005C2118, 1569 bytes, thiscall `ret 0Ch` (the traits `this` unused); size TU with an ebp frame
//   (`push ebp ; mov ebp,esp ; sub esp,5Ch`, `leave`), no EH frame.
// Identity: Ego_r.pdb / FableWin.pdb ?TransferIn@?$CPersistTraits@VCItem@NInventory@@@@QAEXAAVCPersistContext@@AAVCStringParser@@AAVCItem@NInventory@@@Z,
//   the generic class's member specialised for NInventory::CItem (FableWin also names ??0 / ??1?$CPersistTraits@VCItem@NInventory@@@@).
//   Ego_d.pdb: debug length 7714; parameters persist_context, parser, val; locals per_item_data, slong_traits, pper_item_data,
//   item_type, bool_traits, has_per_item_data, number_of_items, string_traits, average_bought_price, item_type_def_name; block
//   { number_of_augmentations, symbol { c1 { augmentation_def_name } } }.
// Shape (Ego_d.pdb C13 lines 222..291, egod_lines.tsv, VC10 /Od sizes; that build has EH, so each construction is 12 B = lea / call /
//   state byte):
//   - 223 / 224 / 225 (12 B each) the bool, long and CCharString traits objects; 227 (4 B, `and [x],0`) pper_item_data = 0; 228 (12 B)
//     per_item_data. Retail keeps the CCharString traits' default at [ebp-8] and the bool traits' zeroed byte at [ebp-20h] (both
//     readers are called out of line with `this`); the long traits' reader is the inline generic body (a long read, then copied).
//   - 230 / 233 / 236 / 239 / 257 / 260 / 262 / 268 / 270 / 276 / 282 / 285 / 288 (480..519 B): one separator check each; in retail
//     `if (!(parser.ReadAsSymbol() == "x")) return;` (the debug build's diagnostic emits nothing here).
//   - 231 / 237 / 283 / 286 (4 B) the zero-initialised has_per_item_data, number_of_augmentations, number_of_items,
//     average_bought_price, each followed by its 18-byte reader call; 234 (12 B) `if (has_per_item_data)` opens the block.
//   - 240 (13 B) resize(number_of_augmentations, 0): VC10 /Od pushes memory operands directly (line 238: lea/push, two `push [ebp+x]`,
//     lea ecx, call = 18 B), so the by-value two-argument call (Ego_d's Dinkumware resize(n, long)) is push 0 + push [n] + lea + call
//     = 13 B and the one-argument form would be 11 B; retail calls resize(n, const long&) 0x004428F0 with the 0 in a frame word; 241 (34 B) the c1 loop; 243 (13 B) PeekNextItemType() != PARSED_ITEM_INTEGER; 245..251 the
//     augmentation_def_name block (the != "" && != "NULL" test 40 B, the def-index store 31 B); 254 the integer skipped.
//   - 264 (16 B) `CCharString symbol = parser.ReadAsSymbol();` 265 (24 B) `if (symbol == ",")`; 277 (6 B) pper_item_data = &per_item_data.
//   - 279..281 item_type_def_name and item_type; 290 (74 B) `val = NInventory::CItem(...)`: the temporary's implicit destructor is
//     ~CCountedPointer<CPerItemData> 0x005C04BF on its +8 (0x005C270E).
// Callees: CCharString() 0x0099E4B0 / ~CCharString 0x0099EAE0 / operator!= 0x0099E960, CStringParser::ReadAsSymbol 0x009BA790 /
//   PeekNextItemType 0x009BA900 / ReadAsInteger 0x009BA540, CBasicString<char>::Compare 0x00411570, CPersistTraits<bool>::TransferIn
//   0x00403FF0, CPersistTraits<CCharString>::TransferIn 0x0040E510, CPersistTraitsSLONG::TransferIn 0x009E2590,
//   vector<long>::resize(n, const long&) 0x004428F0, CGameDefinitionManager::Get 0x0044C6B0, CDefinitionManager::GetDefGlobalIndexFromName
//   0x009AD410, NInventory::CItem(long, long, const CPerItemData*, long) 0x005BEC40, CItem::operator= 0x005C0C24,
//   ~CCountedPointer<NInventory::CPerItemData> 0x005C04BF, free 0x00BFEA14.
// Overlay (add-only opt-ins): engine/NInventory_CTCInventoryBase.h FABLE_RETAIL_NINVENTORY_CITEM_TRANSFERIN (the CItem(long, long, const
//   CPerItemData*, long) declaration; the implicit destructor); engine/CPersistTraits.h FABLE_RETAIL_CPERSISTTRAITS_CCHARSTRING_TRANSFERIN_DECL.
// NF419 (2026-11-25): no `#pragma optimize("y", off)`: the leaf is byte- and relocation-identical without it (size mode keeps
//   its frame); it only framed end-of-TU template and compiler-generated copies (each copy checked in the T3 nf416 audit).
#pragma optimize("s", on)
#define FABLE_RETAIL_CPARSEDITEM
#define FABLE_RETAIL_CCHARSTRING
#define FABLE_RETAIL_CCHARSTRING_FORCEINLINE_TEXT_EQ
#define FABLE_RETAIL_CBASICSTRING_TEXT_EQ_COMPARE_ZERO
#define FABLE_RETAIL_CSTRINGPARSER
#define FABLE_RETAIL_CPERSISTCONTEXT
#define FABLE_RETAIL_CDATAOUTPUTSTREAM
#define FABLE_RETAIL_CPERSISTTRAITS
#define FABLE_RETAIL_CPERSISTTRAITS_CCHARSTRING_TRANSFERIN_DECL
#define FABLE_RETAIL_DEF_CLASS_BASE
#define FABLE_RETAIL_CRESOURCELIST
#define FABLE_RETAIL_CDEFINITIONMANAGER
#define FABLE_RETAIL_CGAMEDEFINITIONMANAGER
#define FABLE_RETAIL_CTCBASE
#define FABLE_RETAIL_CTCINVENTORYBASE_CONFISCATED
#define FABLE_RETAIL_CTCINVENTORYBASE_PERITEMDATA
#define FABLE_RETAIL_CTCINVENTORYBASE_PERITEMDATA_CTOR
#define FABLE_RETAIL_NINVENTORY_CITEM_TRANSFERIN
#include <string.h>
#include "engine/CStringParser.h"
#include "engine/CPersistContext.h"
#include "engine/CDataOutputStream.h"
#include "engine/CPersistTraits.h"
#include "engine/CGameDefinitionManager.h"
#include "engine/NInventory_CTCInventoryBase.h"


template<> void CPersistTraits<NInventory::CItem>::TransferIn(CPersistContext& persist_context, CStringParser& parser, NInventory::CItem& val)
{
    CPersistTraits<bool> bool_traits;
    CPersistTraits<long> slong_traits;
    CPersistTraits<CCharString> string_traits;
    NInventory::CPerItemData* pper_item_data = 0;
    NInventory::CPerItemData per_item_data;
    if (!(parser.ReadAsSymbol() == "("))
        return;
    bool has_per_item_data = false;
    bool_traits.TransferIn(persist_context, parser, has_per_item_data);
    if (!(parser.ReadAsSymbol() == ","))
        return;
    if (has_per_item_data) {
        if (!(parser.ReadAsSymbol() == "("))
            return;
        long number_of_augmentations = 0;
        slong_traits.TransferIn(persist_context, parser, number_of_augmentations);
        if (!(parser.ReadAsSymbol() == ","))
            return;
        per_item_data.Augmentations.resize(number_of_augmentations, 0);
        for (long c1 = 0; c1 < number_of_augmentations; ++c1) {
            if (parser.PeekNextItemType() != PARSED_ITEM_INTEGER) {
                CCharString augmentation_def_name;
                string_traits.TransferIn(persist_context, parser, augmentation_def_name);
                if (augmentation_def_name != "" && augmentation_def_name != "NULL")
                    per_item_data.Augmentations[c1] = CGameDefinitionManager::Get().GetDefGlobalIndexFromName(augmentation_def_name);
            } else
                parser.ReadAsInteger();
            if (!(parser.ReadAsSymbol() == ","))
                return;
        }
        slong_traits.TransferIn(persist_context, parser, per_item_data.NumberOfKills);
        if (!(parser.ReadAsSymbol() == ","))
            return;
        slong_traits.TransferIn(persist_context, parser, per_item_data.TimeWielded);
        if (!(parser.ReadAsSymbol() == ","))
            return;
        slong_traits.TransferIn(persist_context, parser, per_item_data.BestTrophyWitnessesAheadToDate);
        CCharString symbol = parser.ReadAsSymbol();
        if (symbol == ",") {
            bool_traits.TransferIn(persist_context, parser, per_item_data.QuestPersistent);
            if (!(parser.ReadAsSymbol() == ","))
                return;
            slong_traits.TransferIn(persist_context, parser, per_item_data.QuestPersistentRefCount);
            if (!(parser.ReadAsSymbol() == ")"))
                return;
        }
        if (!(parser.ReadAsSymbol() == ","))
            return;
        pper_item_data = &per_item_data;
    }
    CCharString item_type_def_name;
    string_traits.TransferIn(persist_context, parser, item_type_def_name);
    long item_type = CGameDefinitionManager::Get().GetDefGlobalIndexFromName(item_type_def_name);
    if (!(parser.ReadAsSymbol() == ","))
        return;
    long number_of_items = 0;
    slong_traits.TransferIn(persist_context, parser, number_of_items);
    if (!(parser.ReadAsSymbol() == ","))
        return;
    long average_bought_price = 0;
    slong_traits.TransferIn(persist_context, parser, average_bought_price);
    if (!(parser.ReadAsSymbol() == ")"))
        return;
    val = NInventory::CItem(item_type, number_of_items, pper_item_data, average_bought_price);
}