// test for 0x005C0E81 CPersistTraits<NInventory::CItem>::TransferOut -- text save of an item with and without per-item data
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#ifndef _MSC_VER
#define __fastcall
#define __forceinline inline
#endif
class CPersistContext { public: int dummy; };
class CCharString {
public:
    char* PStringData;
    CCharString() { PStringData = (char*)calloc(1, 1); }
    CCharString(const char* text, long length = -1) { PStringData = (char*)malloc(strlen(text) + 1); strcpy(PStringData, text); }
    CCharString(const CCharString& other) { PStringData = (char*)malloc(strlen(other.PStringData) + 1); strcpy(PStringData, other.PStringData); }
    ~CCharString() { free(PStringData); }
    const CCharString& operator=(const CCharString& other) { char* p = (char*)malloc(strlen(other.PStringData) + 1); strcpy(p, other.PStringData); free(PStringData); PStringData = p; return *this; }
    const CCharString& operator+=(const char* text) { size_t n = strlen(PStringData); PStringData = (char*)realloc(PStringData, n + strlen(text) + 1); strcpy(PStringData + n, text); return *this; }
};
class CPersistTraitsSLONG {
public:
    static void __fastcall TransferOut(CPersistContext& context, CCharString& out, const long& value) { char b[32]; sprintf(b, "%ld", value); out += b; }
};
class CDefString {
public:
    long Index;
    operator CCharString() const { char b[32]; sprintf(b, "DEF%ld", Index); return CCharString(b); }
};
class CDefinitionManager {
public:
    CDefString GetDefNameFromGlobalIndex(long index) const { CDefString s; s.Index = index; return s; }
};
class CGameDefinitionManager : public CDefinitionManager {
public:
    static CGameDefinitionManager& __fastcall Get() { static CGameDefinitionManager m; return m; }
};
namespace std {
template <class T> class allocator { };
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
    std::vector<long> Augmentations;
    long  NumberOfKills;
    long  TimeWielded;
    long  BestTrophyWitnessesAheadToDate;
    bool  QuestPersistent;
    long  QuestPersistentRefCount;
    float BestFishWeight;
};
class CItem {
public:
    long ItemType;
    long NumberOfItems;
    void* PPerItemData_Data;
    void* PPerItemData_Info;
    long AverageBoughtPrice;
};
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
    void TransferOut(CPersistContext& context, CCharString& out, const CCharString& value) { out += value.PStringData; }
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

int main()
{
    int fails = 0;
    CPersistContext context;
    CPersistTraits<NInventory::CItem> traits;
    long augs[2] = { 3, 0 };
    NInventory::CPerItemData data;
    data.Augmentations._M_start = augs; data.Augmentations._M_finish = augs + 2; data.Augmentations._M_end_of_storage = augs + 2;
    data.NumberOfKills = 5; data.TimeWielded = 6; data.BestTrophyWitnessesAheadToDate = 7;
    data.QuestPersistent = true; data.QuestPersistentRefCount = 1; data.BestFishWeight = 2.5f;
    NInventory::CItem item;
    item.ItemType = 7; item.NumberOfItems = 2; item.PPerItemData_Data = &data; item.PPerItemData_Info = 0; item.AverageBoughtPrice = 100;
    CCharString out;
    traits.TransferOut(context, out, item);
    if (strcmp(out.PStringData, "(TRUE,(2,DEF3,NULL,5,6,7,TRUE,1),DEF7,2,100)") != 0) { printf("with data: %s\n", out.PStringData); ++fails; }
    item.PPerItemData_Data = 0;
    CCharString out2("X");
    traits.TransferOut(context, out2, item);
    if (strcmp(out2.PStringData, "X(FALSE,DEF7,2,100)") != 0) { printf("no data: %s\n", out2.PStringData); ++fails; }
    data.QuestPersistent = false; data.Augmentations._M_finish = augs; item.PPerItemData_Data = &data;
    CCharString out3;
    traits.TransferOut(context, out3, item);
    if (strcmp(out3.PStringData, "(TRUE,(0,5,6,7,FALSE,1),DEF7,2,100)") != 0) { printf("empty augs: %s\n", out3.PStringData); ++fails; }
    if (fails == 0) printf("005c0e81_TEST PASS\n");
    return fails;
}