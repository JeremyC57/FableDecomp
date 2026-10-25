// CPersistTraits<NInventory::CItem>::TransferBinaryOut(CDataOutputStream& stream, const NInventory::CItem& value) -- binary save of an inventory item: the per-item data block (augmentation names, kill/wield/trophy counts, quest persistence) behind a presence bool, then the item def name, count and average price
// Grade: DIFFER(365v369) -- register allocation only: retail keeps value in ebx, the augmentation index in edi and reloads stream from its slot until after the loop (then edi, and esi at the tail), homing the presence bool and the name in the dead value slot; ours keeps stream in edi, the index in ebx and homes them in [ebp-8] / the stream slot
// Leaf: ?TransferBinaryOut@?$CPersistTraits@VCItem@NInventory@@
// Retail: Fable.exe (TLC, VC7.1) function @ 0x005C0A98, 369 bytes, `ret 8` (traits `this` unused).
// Ego_r.pdb: ?TransferBinaryOut@?$CPersistTraits@VCItem@NInventory@@@@QAEXAAVCDataOutputStream@@ABVCItem@NInventory@@@Z;
//   CItem (0x14) = { long ItemType +0, long NumberOfItems +4, CCountedPointer<CPerItemData> PPerItemData +8, long
//   AverageBoughtPrice +0x10 }; CPerItemData = { vector<long> Augmentations +0 (TLC STLport: 12 bytes), long NumberOfKills
//   +0xC, long TimeWielded +0x10, long BestTrophyWitnessesAheadToDate +0x14, bool QuestPersistent +0x18, long
//   QuestPersistentRefCount +0x1C, float BestFishWeight +0x20 (not saved) }.
// Callees: CDataOutputStream::WriteEBOOL @ 0x00993E30, WriteLong @ 0x00993EB0, WriteString @ 0x009959F0,
//   CGameDefinitionManager::Get @ 0x0044C6B0 (`__fastcall`), GetDefNameFromGlobalIndex @ 0x009ACCC0 (by-value CDefString),
//   CDefString::operator CCharString @ 0x00415DD0, CCharString(const char*, long) @ 0x0099EBF0 ("NULL" 0x012393E4),
//   CCharString(const CCharString&) @ 0x0099EC30, operator= @ 0x0099EFB0, ~CCharString @ 0x0099EAE0.
// Shape: the presence bool is homed in the dead `value` slot (value lives in ebx); each augmentation is written as a
//   name string that defaults to "NULL" and is assigned from the def manager when the index is positive; the item's own
//   def name is copy-initialised from the CDefString (conversion temporary in the `value` slot, then the copy ctor).
//   EBP frame + size TU (`#pragma optimize("y", off)` + `("s", on)`).
#pragma optimize("y", off)
#pragma optimize("s", on)
#pragma pack(push, 1)
struct CBasicStringChar {
    char*         PData;
    unsigned long StringLength;
    unsigned long DataLength : 31;
    bool          UseFastExtend : 1;
};
struct CCharStringData {
    CBasicStringChar Data;
    long             NoRefs;
};
#pragma pack(pop)
class CCharString {
public:
    CCharStringData* PStringData;
    CCharString(const char* text, long length = -1);   // 0x0099EBF0
    CCharString(const CCharString& other);             // 0x0099EC30
    ~CCharString();                                    // 0x0099EAE0
    CCharString& operator=(const CCharString& other);  // 0x0099EFB0
    unsigned long GetLength() const { return PStringData == 0 ? 0 : PStringData->Data.StringLength; }
};
class CDefString {
public:
    long Index;
    CDefString(const CDefString& other);
    operator CCharString() const;                      // 0x00415DD0
};
class CGameDefinitionManager {
public:
    static CGameDefinitionManager& __fastcall Get();   // 0x0044C6B0
    CDefString GetDefNameFromGlobalIndex(long index);  // 0x009ACCC0
};
class CDataOutputStream {
public:
    void* __vftable;
    CCharStringData* NewLine_PStringData;
    void WriteEBOOL(bool value);                       // 0x00993E30
    void WriteLong(long value);                        // 0x00993EB0
    void WriteString(const CCharString& text);         // 0x009959F0
};
namespace std {
template <class T> class vector {
public:
    T* _M_start;
    T* _M_finish;
    T* _M_end_of_storage;
    unsigned int size() const { return _M_finish - _M_start; }
    const T& operator[](unsigned int n) const { return *(_M_start + n); }
};
}
struct CCPPointerInfo;
template <class T> class CCountedPointer {
public:
    T*              Data;
    CCPPointerInfo* Info;
    T* operator->() const { return Data; }
    bool IsNull() const { return Data == 0; }
};
namespace NInventory {
class CPerItemData {
public:
    std::vector<long> Augmentations;                   // +0x00
    long  NumberOfKills;                               // +0x0C
    long  TimeWielded;                                 // +0x10
    long  BestTrophyWitnessesAheadToDate;              // +0x14
    bool  QuestPersistent;                             // +0x18
    long  QuestPersistentRefCount;                     // +0x1C
    float BestFishWeight;                              // +0x20
};
class CItem {
public:
    long ItemType;                                     // +0x00
    long NumberOfItems;                                // +0x04
    CCountedPointer<CPerItemData> PPerItemData;        // +0x08
    long AverageBoughtPrice;                           // +0x10
};
}

__forceinline void WriteCCharString(CDataOutputStream& stream, const CCharString& text)
{
    stream.WriteLong(text.GetLength());
    stream.WriteString(text);
}

template <class T> struct CPersistTraits;
template <> struct CPersistTraits<NInventory::CPerItemData> {
    __forceinline void TransferBinaryOut(CDataOutputStream& stream, const NInventory::CPerItemData& value)
    {
        stream.WriteLong(value.Augmentations.size());
        for (unsigned int i = 0; i < value.Augmentations.size(); ++i) {
            CCharString name("NULL");
            if (value.Augmentations[i] > 0)
                name = CGameDefinitionManager::Get().GetDefNameFromGlobalIndex(value.Augmentations[i]);
            WriteCCharString(stream, name);
        }
        stream.WriteLong(value.NumberOfKills);
        stream.WriteLong(value.TimeWielded);
        stream.WriteLong(value.BestTrophyWitnessesAheadToDate);
        stream.WriteEBOOL(value.QuestPersistent);
        stream.WriteLong(value.QuestPersistentRefCount);
    }
};
template <> struct CPersistTraits<NInventory::CItem> {
    void TransferBinaryOut(CDataOutputStream& stream, const NInventory::CItem& value);
};

void CPersistTraits<NInventory::CItem>::TransferBinaryOut(CDataOutputStream& stream, const NInventory::CItem& value)
{
    NInventory::CPerItemData* data = value.PPerItemData.Data;
    bool hasData = data != 0;
    stream.WriteEBOOL(hasData);
    if (hasData) {
        CPersistTraits<NInventory::CPerItemData> dataTraits;
        dataTraits.TransferBinaryOut(stream, *data);
    }
    CCharString defName = CGameDefinitionManager::Get().GetDefNameFromGlobalIndex(value.ItemType);
    WriteCCharString(stream, defName);
    stream.WriteLong(value.NumberOfItems);
    stream.WriteLong(value.AverageBoughtPrice);
}
