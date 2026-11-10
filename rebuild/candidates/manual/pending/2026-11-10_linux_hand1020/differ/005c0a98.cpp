// CPersistTraits<NInventory::CItem>::TransferBinaryOut(CDataOutputStream& stream, const NInventory::CItem& value) -- binary save of an inventory item: the per-item data block (augmentation names, kill / wield / trophy counts, quest persistence) behind a presence bool, then the item's def name, count and average price
// Grade: DIFFER(368v369, oracle cut to 369) -- on shared headers (1 grade): retail pushes the CDefString->CCharString conversion temporary's address (GetString 0x00415DD0 hidden return) BEFORE evaluating CGameDefinitionManager::Get() / GetDefNameFromGlobalIndex at both def-name sites; ours pushes it after (-1 byte). An explicit .GetString() gets the push order but `push eax` instead of `lea ; push` for operator= (365 bytes); FORCEINLINE_CONVERSION inert.
// Leaf: ?TransferBinaryOut@?$CPersistTraits@VCItem@NInventory@@@@QAEXAAVCDataOutputStream@@ABVCItem@NInventory@@@Z
// Retail: Fable.exe (TLC, VC7.1) @ 0x005C0A98, 369 bytes, thiscall `ret 8` (traits `this` unused); EBP frame, size TU.
#pragma optimize("y", off)
#pragma optimize("s", on)
#define FABLE_RETAIL_CCHARSTRING
#define FABLE_RETAIL_CDEFSTRINGTABLE
#define FABLE_RETAIL_DEF_CLASS_BASE
#define FABLE_RETAIL_CTCBASE
#define FABLE_RETAIL_CRESOURCELIST
#define FABLE_RETAIL_CDEFINITIONMANAGER
#define FABLE_RETAIL_CPERSISTCONTEXT
#define FABLE_RETAIL_CDATAOUTPUTSTREAM
#define FABLE_RETAIL_CDATAOUTPUTSTREAM_WRITESTRING
#define FABLE_RETAIL_CTCINVENTORYBASE_CONFISCATED
#define FABLE_RETAIL_CTCINVENTORYBASE_PERITEMDATA
#include "fable_string.h"
#include "engine/CDataOutputStream.h"
#include "engine/CDefString.h"
#include "engine/CGameDefinitionManager.h"
#include "engine/NInventory_CTCInventoryBase.h"
#include "engine/CPersistTraits.h"

template<> class CPersistTraits<NInventory::CItem> {
public:
    NInventory::CItem DefaultVal;
    void TransferBinaryOut(CDataOutputStream& stream, const NInventory::CItem& value);
};

void CPersistTraits<NInventory::CItem>::TransferBinaryOut(CDataOutputStream& stream, const NInventory::CItem& value)
{
    const NInventory::CPerItemData* data = value.PPerItemData.Data;
    bool hasData = data != 0;
    stream.WriteEBOOL(hasData);
    if (hasData) {
        stream.WriteSLONG(data->Augmentations.size());
        for (unsigned int i = 0; i < data->Augmentations.size(); ++i) {
            CCharString name("NULL");
            if (data->Augmentations[i] > 0)
                name = CGameDefinitionManager::Get().GetDefNameFromGlobalIndex(data->Augmentations[i]);
            stream.WriteSLONG(name.GetLength());
            stream.WriteString(name);
        }
        stream.WriteSLONG(data->NumberOfKills);
        stream.WriteSLONG(data->TimeWielded);
        stream.WriteSLONG(data->BestTrophyWitnessesAheadToDate);
        stream.WriteEBOOL(data->QuestPersistent);
        stream.WriteSLONG(data->QuestPersistentRefCount);
    }
    CCharString defName = CGameDefinitionManager::Get().GetDefNameFromGlobalIndex(value.ItemType);
    stream.WriteSLONG(defName.GetLength());
    stream.WriteString(defName);
    stream.WriteSLONG(value.NumberOfItems);
    stream.WriteSLONG(value.AverageBoughtPrice);
}
