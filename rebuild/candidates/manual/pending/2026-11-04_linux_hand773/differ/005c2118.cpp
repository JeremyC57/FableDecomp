// CPersistTraits<NInventory::CItem>::TransferIn(CPersistContext& context, CStringParser& parser, NInventory::CItem& value) -- text load of an inventory item "(<has data>,[(<count>,<names>...,kills,wielded,trophy[,questPersistent,refCount]),]<def name>,<count>,<price>)"
// Grade: DIFFER(1571v1569) -- frame-slot order: retail packs defName -0xC, name -0x10, symbol/type -0x14, count/number -0x18, pData/i -0x1C; VC7.1 here gives name -0xC .. defName -0x1C, and keeps i in memory (retail caches it in esi); 6 grades
// Leaf: ?TransferIn@?$CPersistTraits@VCItem@NInventory@@@@QAEXAAVCPersistContext@@AAVCStringParser@@AAVCItem@NInventory@@@Z
// Flags: none
#pragma optimize("y", off)
#pragma optimize("s", on)
#include <string.h>
#pragma intrinsic(strcmp)
extern "C" void __cdecl free(void* p);
struct CBasicStringChar {
    char*         PData;
    unsigned long StringLength;
    static long __fastcall Compare(const char* a, const char* b);   // 0x00411570
    __forceinline bool operator==(const char* text) const { return Compare(PData, text) == 0; }
};
class CCharString { public:
    CBasicStringChar* PStringData;
    CCharString();                                     // 0x0099E4B0
    CCharString(const char* text, long length = -1);   // 0x0099EBF0
    ~CCharString();                                    // 0x0099EAE0
    bool operator!=(const char* text) const;           // 0x0099E960
    __forceinline bool operator==(const char* text) const
    {
        if (PStringData == 0) {
            if (strcmp(text, "") == 0)
                return true;
            return false;
        }
        return *PStringData == text;
    }
};
enum EParsedItemType { PARSED_ITEM_NULL = 0, PARSED_ITEM_SYMBOL = 1, PARSED_ITEM_STRING = 2, PARSED_ITEM_INTEGER = 3, PARSED_ITEM_FLOAT = 4 };
class CStringParser { public:
    long ReadAsInteger();                              // 0x009BA540
    CCharString ReadAsSymbol();                        // 0x009BA790
    EParsedItemType PeekNextItemType();                // 0x009BA900
};
class CPersistContext;
class CPersistTraitsSLONG { public:
    static void __fastcall TransferIn(CPersistContext& context, CStringParser& parser, long& value);   // 0x009E2590
};
__forceinline long TransferInLong(CPersistContext& context, CStringParser& parser)
{
    long value;
    CPersistTraitsSLONG::TransferIn(context, parser, value);
    return value;
}
class CDefinitionManager { public:
    long GetDefGlobalIndexFromName(const CCharString& name) const;   // 0x009AD410
};
class CGameDefinitionManager : public CDefinitionManager { public:
    static CGameDefinitionManager& __fastcall Get();   // 0x0044C6B0
};
namespace std {
template <class T> class vector {
public:
    T* _M_start;
    T* _M_finish;
    T* _M_end_of_storage;
    vector() : _M_start(0), _M_finish(0), _M_end_of_storage(0) { }
    ~vector() { if (_M_start) free(_M_start); }
    void resize(unsigned long n, const T& x);           // 0x004428F0
    T& operator[](unsigned long n) { return _M_start[n]; }
};
}
struct SCountBlock;
template <class T> class CCountedPointer { public:
    T* m_pObject;
    SCountBlock* m_pBlock;
    ~CCountedPointer();                                     // 0x005C04BF
};
namespace NInventory {
class CPerItemData { public:
    std::vector<long> Augmentations;                        // +0x00
    long NumberOfKills;                                     // +0x0C
    long TimeWielded;                                       // +0x10
    long BestTrophyWitnessesAheadToDate;                    // +0x14
    bool QuestPersistent;                                   // +0x18
    long QuestPersistentRefCount;                           // +0x1C
    float BestFishWeight;                                   // +0x20
    CPerItemData() : NumberOfKills(0), TimeWielded(0), BestTrophyWitnessesAheadToDate(0), QuestPersistent(true), QuestPersistentRefCount(-1) { }
};
class CItem { public:
    long ItemType;                                          // +0x00
    long NumberOfItems;                                     // +0x04
    CCountedPointer<CPerItemData> PPerItemData;             // +0x08
    long AverageBoughtPrice;                                // +0x10
    CItem(long type, long number, const CPerItemData* data, long price);   // 0x005BEC40
    CItem& operator=(const CItem& other);                   // 0x005C0C24
};
}
template <class T> class CPersistTraits { public:
    T DefaultVal;
    CPersistTraits() : DefaultVal() { }
    void TransferIn(CPersistContext& context, CStringParser& parser, T& value);
};
template <> class CPersistTraits<NInventory::CItem> { public:
    void TransferIn(CPersistContext& context, CStringParser& parser, NInventory::CItem& value);
};

void CPersistTraits<NInventory::CItem>::TransferIn(CPersistContext& context, CStringParser& parser, NInventory::CItem& value)
{
    CPersistTraits<bool> boolTraits;
    CPersistTraits<CCharString> stringTraits;
    NInventory::CPerItemData* pData = 0;
    NInventory::CPerItemData data;
    if (!(parser.ReadAsSymbol() == "("))
        return;
    bool hasData = false;
    boolTraits.TransferIn(context, parser, hasData);
    if (!(parser.ReadAsSymbol() == ","))
        return;
    if (hasData) {
        if (!(parser.ReadAsSymbol() == "("))
            return;
        long count;
        CPersistTraitsSLONG::TransferIn(context, parser, count);
        if (!(parser.ReadAsSymbol() == ","))
            return;
        data.Augmentations.resize(count, 0);
        for (long i = 0; i < count; ++i) {
            if (parser.PeekNextItemType() != PARSED_ITEM_INTEGER) {
                CCharString name;
                stringTraits.TransferIn(context, parser, name);
                if (name != "" && name != "NULL")
                    data.Augmentations[i] = CGameDefinitionManager::Get().GetDefGlobalIndexFromName(name);
            }
            else
                parser.ReadAsInteger();
            if (!(parser.ReadAsSymbol() == ","))
                return;
        }
        data.NumberOfKills = TransferInLong(context, parser);
        if (!(parser.ReadAsSymbol() == ","))
            return;
        data.TimeWielded = TransferInLong(context, parser);
        if (!(parser.ReadAsSymbol() == ","))
            return;
        data.BestTrophyWitnessesAheadToDate = TransferInLong(context, parser);
        CCharString symbol = parser.ReadAsSymbol();
        if (symbol == ",") {
            boolTraits.TransferIn(context, parser, data.QuestPersistent);
            if (!(parser.ReadAsSymbol() == ","))
                return;
            data.QuestPersistentRefCount = TransferInLong(context, parser);
            if (!(parser.ReadAsSymbol() == ")"))
                return;
        }
        if (!(parser.ReadAsSymbol() == ","))
            return;
        pData = &data;
    }
    CCharString defName;
    stringTraits.TransferIn(context, parser, defName);
    long type = CGameDefinitionManager::Get().GetDefGlobalIndexFromName(defName);
    if (parser.ReadAsSymbol() == ",") {
        long number;
        CPersistTraitsSLONG::TransferIn(context, parser, number);
        if (parser.ReadAsSymbol() == ",") {
            long price = TransferInLong(context, parser);
            if (parser.ReadAsSymbol() == ")")
                value = NInventory::CItem(type, number, pData, price);
        }
    }
}
