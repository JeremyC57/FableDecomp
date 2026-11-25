// CPersistTraits<NInventory::CItem>::TransferBinaryOut(CDataOutputStream& os, const NInventory::CItem& val) -- binary save of an inventory item: a has-per-item-data bool, then (if set) the augmentation count and each augmentation's def name ("NULL" for an index <= 0) as length + string, the kill / wield / trophy counts, the quest-persistent bool and ref count; then the item's def name (length + string), count and average bought price
// Inline-budget: no flag; one header contract and one header opt-in decide expanded vs called: FABLE_RETAIL_CDEFSTRING_OUTOFLINE_CONVERSION (existing; retail calls the conversion ??B 0x00415DD0 with its hidden result pushed before the object expression, 0x005C0AF6 / 0x005C0B97; with the plain inline forwarder (also called here) the push follows the GetDefNameFromGlobalIndex call: DIFFER(368v369)); FABLE_RETAIL_NINVENTORY_CITEM_GETTERS (new; the four expanded CItem getters, the Ego_d spelling; reading the members directly: DIFFER(364v369)).
// Leaf: ?TransferBinaryOut@?$CPersistTraits@VCItem@NInventory@@@@QAEXAAVCDataOutputStream@@ABVCItem@NInventory@@@Z
// Flags: none
// Retail: Fable.exe (TLC, VC7.1) @ 0x005C0A98, 369 bytes, thiscall `ret 8` (the traits `this` unused); size TU with an ebp frame
//   (`push ebp ; mov ebp,esp ; sub esp,0Ch`, `leave`), no EH frame. Same TU as the landed text twin TransferOut 0x005C0E81, binary
//   twin TransferBinaryIn 0x005C19A3 and TransferIn 0x005C2118.
// Identity: Ego_r.pdb / FableWin.pdb ?TransferBinaryOut@?$CPersistTraits@VCItem@NInventory@@@@QAEXAAVCDataOutputStream@@ABVCItem@NInventory@@@Z
//   (the value by const reference). Ego_d.pdb: debug length 682; parameters os, val; locals pper_item_data, has_per_item_data,
//   def_name; block { c1 (long) { augmentation_def_name } }.
// Shape (Ego_d.pdb C13 lines 190..219, egod_lines/005c0a98.tsv, VC10 /Od sizes):
//   - 191 (11 B, `mov ecx,[val] ; call ; mov [x],eax`) pper_item_data = val.GetPerItemData(); 192 (12 B) the != 0 bool; 193 WriteEBOOL;
//     194 (12 B) `if (has_per_item_data)`; 196 (170 B) debug-only (no retail trace); 197 (17 B) WriteSLONG(Augmentations.size()).
//   - 198 (39 B) the c1 loop; 200 (22 B) `CCharString augmentation_def_name("NULL")`; 201 (16 B) `if (...Augmentations[c1] > 0)`;
//     203 (68 B) the assignment from the CDefString conversion; 205 (17 B) WriteSLONG(GetLength()); 206 (12 B) WriteString.
//   - 208..212 (14 / 14 / 14 / 16 / 14 B) the four longs and the bool through pper_item_data.
//   - 214 (67 B) `CCharString def_name = ...GetDefNameFromGlobalIndex(val.GetItemType())`: copy-initialised (retail copy-constructs
//     def_name at [ebp-4] from the conversion temporary at [ebp+0Ch], 0x005C0BBA); 217 / 218 (17 B each, `mov ecx,[val] ; call` +
//     WriteSLONG) val.GetNumberOfItems() / val.GetAverageBoughtPrice().
// Callees: CDataOutputStream::WriteEBOOL 0x00993E30 / WriteSLONG 0x00993EB0 / WriteString 0x009959F0, CCharString(const char*, long)
//   0x0099EBF0 ("NULL" 0x012393E4), CCharString(const CCharString&) 0x0099EC30, operator= 0x0099EFB0, ~CCharString 0x0099EAE0,
//   CGameDefinitionManager::Get 0x0044C6B0, CDefinitionManager::GetDefNameFromGlobalIndex 0x009ACCC0, CDefString::operator CCharString
//   0x00415DD0.
// Overlay (add-only opt-ins): engine/NInventory_CTCInventoryBase.h FABLE_RETAIL_NINVENTORY_CITEM_GETTERS (GetItemType, GetNumberOfItems,
//   GetPerItemData, GetAverageBoughtPrice; inferred one-member bodies); engine/CPersistTraits.h FABLE_RETAIL_CPERSISTTRAITS_BINARYOUT_CONSTREF
//   (the primary template's writer by const reference, declaration only, so this member can be specialised as TransferIn is).
// Inferred: the getter bodies (Ego_d / FableWin names; each returns its member, which the retail loads show).
// NF419 (2026-11-25): no `#pragma optimize("y", off)`: the leaf is byte- and relocation-identical without it (size mode keeps
//   its frame); it only framed end-of-TU template and compiler-generated copies (each copy checked in the T3 nf416 audit).
#pragma optimize("s", on)
#define FABLE_RETAIL_CCHARSTRING
#define FABLE_RETAIL_CDEFSTRINGTABLE
#define FABLE_RETAIL_CDEFSTRING_OUTOFLINE_CONVERSION
#define FABLE_RETAIL_DEF_CLASS_BASE
#define FABLE_RETAIL_CTCBASE
#define FABLE_RETAIL_CRESOURCELIST
#define FABLE_RETAIL_CDEFINITIONMANAGER
#define FABLE_RETAIL_CDATAOUTPUTSTREAM
#define FABLE_RETAIL_CDATAOUTPUTSTREAM_WRITESTRING
#define FABLE_RETAIL_CTCINVENTORYBASE_CONFISCATED
#define FABLE_RETAIL_CTCINVENTORYBASE_PERITEMDATA
#define FABLE_RETAIL_NINVENTORY_CITEM_GETTERS
#define FABLE_RETAIL_CPERSISTTRAITS_BINARYOUT_CONSTREF
#include "fable_string.h"
#include "engine/CDataOutputStream.h"
#include "engine/CDefString.h"
#include "engine/CGameDefinitionManager.h"
#include "engine/NInventory_CTCInventoryBase.h"
#include "engine/CPersistTraits.h"

template<> void CPersistTraits<NInventory::CItem>::TransferBinaryOut(CDataOutputStream& os, const NInventory::CItem& val)
{
    const NInventory::CPerItemData* pper_item_data = val.GetPerItemData();
    bool has_per_item_data = pper_item_data != 0;
    os.WriteEBOOL(has_per_item_data);
    if (has_per_item_data) {
        os.WriteSLONG(pper_item_data->Augmentations.size());
        for (long c1 = 0; c1 < pper_item_data->Augmentations.size(); ++c1) {
            CCharString augmentation_def_name("NULL");
            if (pper_item_data->Augmentations[c1] > 0)
                augmentation_def_name = CGameDefinitionManager::Get().GetDefNameFromGlobalIndex(pper_item_data->Augmentations[c1]);
            os.WriteSLONG(augmentation_def_name.GetLength());
            os.WriteString(augmentation_def_name);
        }
        os.WriteSLONG(pper_item_data->NumberOfKills);
        os.WriteSLONG(pper_item_data->TimeWielded);
        os.WriteSLONG(pper_item_data->BestTrophyWitnessesAheadToDate);
        os.WriteEBOOL(pper_item_data->QuestPersistent);
        os.WriteSLONG(pper_item_data->QuestPersistentRefCount);
    }
    CCharString def_name = CGameDefinitionManager::Get().GetDefNameFromGlobalIndex(val.GetItemType());
    os.WriteSLONG(def_name.GetLength());
    os.WriteString(def_name);
    os.WriteSLONG(val.GetNumberOfItems());
    os.WriteSLONG(val.GetAverageBoughtPrice());
}