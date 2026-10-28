// CPersistTraits<NInventory::CItem>::TransferOut(CPersistContext& context, CCharString& out, const NInventory::CItem& value) -- text save of an inventory item as "(<has data>,[(<count>,<names>...,kills,wielded,trophy,questPersistent,refCount),]<def name>,<count>,<price>)"
// Leaf: ?TransferOut@?$CPersistTraits@VCItem@NInventory@@@@QAEXAAVCPersistContext@@AAVCCharString@@ABVCItem@NInventory@@@Z
// Retail: Fable.exe (TLC, VC7.1) function @ 0x005C0E81, 587 bytes, `ret 0Ch` (traits `this` unused).
// Ego_r.pdb: ?TransferOut@?$CPersistTraits@VCItem@NInventory@@@@QAEXAAVCPersistContext@@AAVCCharString@@ABVCItem@NInventory@@@Z;
//   CItem (0x14) = { long ItemType +0, long NumberOfItems +4, CCountedPointer<CPerItemData> PPerItemData +8, long
//   AverageBoughtPrice +0x10 }; CPerItemData = { vector<long> Augmentations +0 (TLC STLport: 12 bytes, so every later
//   member sits 4 below Ego_r), long NumberOfKills +0xC, long TimeWielded +0x10, long BestTrophyWitnessesAheadToDate
//   +0x14, bool QuestPersistent +0x18, long QuestPersistentRefCount +0x1C, float BestFishWeight +0x20 (not saved) }.
// Callees (PDB spellings): CCharString() @ 0x0099E4B0 (the string traits' DefaultVal), CCharString(const char*, long)
//   @ 0x0099EBF0 ("NULL"), CCharString(const CCharString&) @ 0x0099EC30, operator= @ 0x0099EFB0 (returns const&),
//   ~CCharString @ 0x0099EAE0, operator+=(const char*) @ 0x0099F100 (returns const&),
//   CPersistTraitsSLONG::TransferOut @ 0x009E2560 (FableWin.pdb static `__fastcall` SI: context in ecx, out in edx,
//   const long& on the stack, `ret 4`), CPersistTraits<CCharString>::TransferOut @ 0x0040E630,
//   CGameDefinitionManager::Get @ 0x0044C6B0 (static `__fastcall`), CDefinitionManager::GetDefNameFromGlobalIndex
//   @ 0x009ACCC0 (const, by-value CDefString), CDefString::operator CCharString() const @ 0x00415DD0.
// Shape: the binary twin 0x005C0A98 in text form: the string traits (DefaultVal) is built first and destroyed last; the
//   bools go through the inline TRUE/FALSE writer; each long through the inline CPersistTraits<long> writer, whose
//   copy lands in a dead argument slot; the item's def name is copy-initialised from the CDefString conversion
//   temporary. The augmentation index goes through STLport's `operator[]` = `*(begin() + n)`: that form gives retail's
//   byte-offset CSE (`shl ecx,2 ; cmp [ecx+eax],0 ... push [ecx+eax]`); `*(_M_start + n)` CSEs the whole address.
//   EBP frame + size TU (optimize "y" off + "s" on, below).
#pragma optimize("y", off)
#pragma optimize("s", on)
#define CItem CItemLayout
#include "engine/CItem.h"
#undef CItem
#define CDefinitionManager CDefinitionManagerLayout
#include "engine/CDefinitionManager.h"
#undef CDefinitionManager
#pragma pack(push, 1)
struct CBasicStringChar {
    char*         PData;
    unsigned long StringLength;
    unsigned long DataLength : 31;
    bool          UseFastExtend : 1;
};
class CCharStringData { public:
    CBasicStringChar Data;
    long             NoRefs;
};
#pragma pack(pop)
class CPersistContext;
class CCharString {
public:
    CCharStringData* PStringData;
    CCharString();                                     // 0x0099E4B0
    CCharString(const char* text, long length = -1);   // 0x0099EBF0
    CCharString(const CCharString& other);             // 0x0099EC30
    ~CCharString();                                    // 0x0099EAE0
    const CCharString& operator=(const CCharString& other);  // 0x0099EFB0
    const CCharString& operator+=(const char* text);   // 0x0099F100
};
class CPersistTraitsSLONG {
public:
    static void __fastcall TransferOut(CPersistContext& context, CCharString& out, const long& value);   // 0x009E2560
};
class CDefString {
public:
    long Index;
    CDefString(const CDefString& other);
    operator CCharString() const;                      // 0x00415DD0
};
class CDefinitionManager : public CDefinitionManagerLayout {
public:
    CDefString GetDefNameFromGlobalIndex(long index) const;   // 0x009ACCC0
};
class CGameDefinitionManager : public CDefinitionManager {
public:
    static CGameDefinitionManager& __fastcall Get();   // 0x0044C6B0
};
namespace std {
template <class T> class allocator {
};
template <class T, class Alloc = allocator<T> > class vector {
public:
    T* _M_start;
    T* _M_finish;
    T* _M_end_of_storage;
    const T* begin() const { return _M_start; }
    unsigned int size() const { return _M_finish - _M_start; }
    const T& operator[](unsigned int n) const { return *(begin() + n); }
};
}
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
class CItem : public CItemLayout {                    // engine/CItem.h: ItemType +0, NumberOfItems +4,
};                                                     //   PPerItemData (CCountedPointer<CPerItemData>) +8/+0xC, AverageBoughtPrice +0x10
}

template <class T> class CPersistTraits;
template <> class CPersistTraits<long> {
public:
    long DefaultVal;
    __forceinline void TransferOut(CPersistContext& context, CCharString& out, const long& value)
    {
        long copy = value;
        CPersistTraitsSLONG::TransferOut(context, out, copy);
    }
};
template <> class CPersistTraits<bool> {
public:
    bool DefaultVal;
    __forceinline void TransferOut(CPersistContext& context, CCharString& out, const bool& value)
    {
        if (value)
            out += "TRUE";
        else
            out += "FALSE";
    }
};
template <> class CPersistTraits<CCharString> {
public:
    CCharString DefaultVal;
    void TransferOut(CPersistContext& context, CCharString& out, const CCharString& value);   // 0x0040E630
};
template <> class CPersistTraits<NInventory::CItem> {
public:
    void TransferOut(CPersistContext& context, CCharString& out, const NInventory::CItem& value);
};

void CPersistTraits<NInventory::CItem>::TransferOut(CPersistContext& context, CCharString& out, const NInventory::CItem& value)
{
    CPersistTraits<CCharString> stringTraits;
    CPersistTraits<long> longTraits;
    CPersistTraits<bool> boolTraits;
    NInventory::CPerItemData* data = (NInventory::CPerItemData*)value.PPerItemData_Data;
    out += "(";
    bool hasData = data != 0;
    boolTraits.TransferOut(context, out, hasData);
    out += ",";
    if (hasData) {
        out += "(";
        long count = data->Augmentations.size();
        longTraits.TransferOut(context, out, count);
        out += ",";
        for (long i = 0; i < data->Augmentations.size(); ++i) {
            CCharString name("NULL");
            if (data->Augmentations[i] > 0)
                name = CGameDefinitionManager::Get().GetDefNameFromGlobalIndex(data->Augmentations[i]);
            stringTraits.TransferOut(context, out, name);
            out += ",";
        }
        longTraits.TransferOut(context, out, data->NumberOfKills);
        out += ",";
        longTraits.TransferOut(context, out, data->TimeWielded);
        out += ",";
        longTraits.TransferOut(context, out, data->BestTrophyWitnessesAheadToDate);
        out += ",";
        boolTraits.TransferOut(context, out, data->QuestPersistent);
        out += ",";
        longTraits.TransferOut(context, out, data->QuestPersistentRefCount);
        out += "),";
    }
    long itemType = value.ItemType;
    CCharString defName = CGameDefinitionManager::Get().GetDefNameFromGlobalIndex(itemType);
    stringTraits.TransferOut(context, out, defName);
    out += ",";
    longTraits.TransferOut(context, out, value.NumberOfItems);
    out += ",";
    longTraits.TransferOut(context, out, value.AverageBoughtPrice);
    out += ")";
}