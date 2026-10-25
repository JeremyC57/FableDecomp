// CPersistTraits<NInventory::CItem>::TransferOut(CPersistContext* context, CCharString* out, const NInventory::CItem* value) -- text save of an inventory item as "(<has data>,[(<count>,<names>...,kills,wielded,trophy,questPersistent,refCount),]<def name>,<count>,<price>)"
// Grade: DIFFER(585v587) -- only the augmentation element address in the loop: retail CSEs the byte offset (mov ecx,i ; shl ecx,2 ; cmp [ecx+eax],0 ... push [ecx+eax]), ours CSEs the whole address (lea eax,[eax+ecx*4] ; cmp [eax],0 ... push [eax]); the item-type local fixed the tail
// Leaf: ?TransferOut@?$CPersistTraits@VCItem@NInventory@@
// Retail: Fable.exe (TLC, VC7.1) function @ 0x005C0E81, 587 bytes, `ret 0Ch` (traits `this` unused).
// Ego_r.pdb: ?TransferOut@?$CPersistTraits@VCItem@NInventory@@@@QAEXAAVCPersistContext@@AAVCCharString@@ABVCItem@NInventory@@@Z;
//   CItem (0x14) = { long ItemType +0, long NumberOfItems +4, CCountedPointer<CPerItemData> PPerItemData +8, long
//   AverageBoughtPrice +0x10 }; CPerItemData = { vector<long> Augmentations +0 (TLC STLport: 12 bytes), long NumberOfKills
//   +0xC, long TimeWielded +0x10, long BestTrophyWitnessesAheadToDate +0x14, bool QuestPersistent +0x18, long
//   QuestPersistentRefCount +0x1C, float BestFishWeight +0x20 (not saved) }.
// Callees: CCharString() @ 0x0099E4B0 (string traits DefaultVal), CCharString(const char*, long) @ 0x0099EBF0 ("NULL"
//   0x012393E4), CCharString(const CCharString&) @ 0x0099EC30, operator= @ 0x0099EFB0, ~CCharString @ 0x0099EAE0,
//   operator+=(const char*) @ 0x0099F100 ("(" 0x0122E028, "," 0x0122E024, ")" 0x0122E020, ")," 0x01253CB4, "TRUE"
//   0x0122D6D4, "FALSE" 0x0122D6CC), TransferOutLong @ 0x009E2560 (`__fastcall` context, out, &copy),
//   CPersistTraits<CCharString>::TransferOut @ 0x0040E630, CGameDefinitionManager::Get @ 0x0044C6B0 (`__fastcall`),
//   GetDefNameFromGlobalIndex @ 0x009ACCC0 (by-value CDefString), CDefString::operator CCharString @ 0x00415DD0.
// Shape: the binary twin 0x005C0A98 in text form: the string traits (DefaultVal) is built first and destroyed last; the
//   bools go through the inline TRUE/FALSE writer; each long through the inline copy-and-TransferOutLong writer (copy in
//   a dead argument slot); the item's def name copy-initialised from the CDefString conversion temporary.
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
class CPersistContext;
class CCharString {
public:
    CCharStringData* PStringData;
    CCharString();                                     // 0x0099E4B0
    CCharString(const char* text, long length = -1);   // 0x0099EBF0
    CCharString(const CCharString& other);             // 0x0099EC30
    ~CCharString();                                    // 0x0099EAE0
    CCharString& operator=(const CCharString& other);  // 0x0099EFB0
    CCharString& operator+=(const char* text);         // 0x0099F100
};
void __fastcall TransferOutLong(CPersistContext* context, CCharString* out, long* value);   // 0x009E2560
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

template <class T> struct CPersistTraits;
template <> struct CPersistTraits<long> {
    __forceinline void TransferOut(CPersistContext* context, CCharString* out, const long* value) const
    {
        long copy = *value;
        TransferOutLong(context, out, &copy);
    }
};
template <> struct CPersistTraits<bool> {
    __forceinline void TransferOut(CPersistContext* context, CCharString* out, const bool* value) const
    {
        if (*value)
            *out += "TRUE";
        else
            *out += "FALSE";
    }
};
template <> struct CPersistTraits<CCharString> {
    CCharString DefaultVal;
    void TransferOut(CPersistContext* context, CCharString* out, CCharString* value);   // 0x0040E630
};
template <> struct CPersistTraits<NInventory::CItem> {
    void TransferOut(CPersistContext* context, CCharString* out, const NInventory::CItem* value);
};

void CPersistTraits<NInventory::CItem>::TransferOut(CPersistContext* context, CCharString* out, const NInventory::CItem* value)
{
    CPersistTraits<CCharString> stringTraits;
    CPersistTraits<long> longTraits;
    CPersistTraits<bool> boolTraits;
    NInventory::CPerItemData* data = value->PPerItemData.Data;
    *out += "(";
    bool hasData = data != 0;
    boolTraits.TransferOut(context, out, &hasData);
    *out += ",";
    if (hasData) {
        *out += "(";
        long count = data->Augmentations.size();
        longTraits.TransferOut(context, out, &count);
        *out += ",";
        for (long i = 0; i < data->Augmentations.size(); ++i) {
            CCharString name("NULL");
            if (data->Augmentations[i] > 0)
                name = CGameDefinitionManager::Get().GetDefNameFromGlobalIndex(data->Augmentations[i]);
            stringTraits.TransferOut(context, out, &name);
            *out += ",";
        }
        longTraits.TransferOut(context, out, &data->NumberOfKills);
        *out += ",";
        longTraits.TransferOut(context, out, &data->TimeWielded);
        *out += ",";
        longTraits.TransferOut(context, out, &data->BestTrophyWitnessesAheadToDate);
        *out += ",";
        boolTraits.TransferOut(context, out, &data->QuestPersistent);
        *out += ",";
        longTraits.TransferOut(context, out, &data->QuestPersistentRefCount);
        *out += "),";
    }
    long itemType = value->ItemType;
    CCharString defName = CGameDefinitionManager::Get().GetDefNameFromGlobalIndex(itemType);
    stringTraits.TransferOut(context, out, &defName);
    *out += ",";
    longTraits.TransferOut(context, out, &value->NumberOfItems);
    *out += ",";
    longTraits.TransferOut(context, out, &value->AverageBoughtPrice);
    *out += ")";
}
