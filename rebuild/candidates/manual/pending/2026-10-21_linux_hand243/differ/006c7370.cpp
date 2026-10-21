// CDeedManager::TransferState(CPersistContext& context, ?) -- persist the deed manager: the next handle, the two crime flags, the crime village, then the opinion-deed map flattened into a vector of records (built from the map when saving, re-inserted into the map when loading)
// Grade: DIFFER(332v342) -- register-role rotation (retail this in esi, context in edi; ours swapped) and an 8-byte wider frame for the local record vector / flag
// Leaf: TransferState
// Retail: Fable.exe (TLC, VC7.1) function @ 0x006C7370, length 340 bytes, `ret 8` (8-byte aligned frame).
// Callees: CPersistContext::Transfer<long> @ 0x00410BE0 ("NextHandle"), Transfer<signed char> @ 0x004045C0 ("WeaponOutCrimeEnabled" default
// true, "GuardsIgnoreCrimes" default false), Transfer<CCharString?> @ 0x006C9C60 ("VillageForCrimes"), vector<COpinionDeedRecord>::reserve
// @ 0x006C8560, the map-to-vector flatten @ 0x006C9B70 (`__fastcall` dest vector, flag byte; begin/end iterators by value), Transfer
// <COpinionDeedRecord> @ 0x006C9EE0 (landed, "OpinionDeedMap"), the opinion-deed tree insert_unique @ 0x006C8400 (landed), free @ 0x00BFEA14.
// Layout: CDeedManager NextHandle +0x1c, OpinionDeedMap tree +0x0c (header +0xc, count +0x10), WeaponOutCrimeEnabled +0x2c, GuardsIgnoreCrimes
// +0x2d, VillageForCrimes +0x20; CPersistContext Mode +0x18 (1 text save, 3 binary save).
// Shape: a save (mode 1 or 3) flattens the map into a reserved local vector before the transfer; a load transfers into an empty local vector
// and inserts each 8-byte record as a pair into the map.
extern "C" void __cdecl free(void* block);                       // 0x00BFEA14
struct CCharString { void* PStringData; };
struct COpinionDeedRecord { long Key; long Value; };
struct CRecordVector {
    COpinionDeedRecord* first; COpinionDeedRecord* last; COpinionDeedRecord* end;
    CRecordVector() : first(0), last(0), end(0) {}
    ~CRecordVector() { if (first) free(first); }
    void reserve(unsigned long count);                             // 0x006C8560
};
struct SDeedNode { long colour; SDeedNode* parent; SDeedNode* left; SDeedNode* right; COpinionDeedRecord value; };
struct SPairIB { SDeedNode* node; bool inserted; };
struct CDeedTree {
    SDeedNode* header; unsigned long count; unsigned char comp[4];
    SPairIB InsertUnique(const COpinionDeedRecord& value);          // 0x006C8400
};
void __fastcall FlattenDeedMap(CRecordVector* dest, bool* flag, SDeedNode* first, SDeedNode* last);   // 0x006C9B70
struct CPersistContext {
    unsigned char _pad[0x18];
    long Mode;                                                     // +0x18
    void Transfer(const char* name, long& value, const long& defaultValue);                 // 0x00410BE0
    void Transfer(const char* name, signed char& value, const signed char& defaultValue);   // 0x004045C0
    void Transfer(const char* name, CCharString& value);                                    // 0x006C9C60
    void Transfer(const char* name, CRecordVector& value);                                  // 0x006C9EE0
};
struct CDeedManager {
    unsigned char _pad[0x0c];
    CDeedTree   OpinionDeedMap;                                    // +0x0c
    long        _pad18;
    long        NextHandle;                                        // +0x1c
    CCharString VillageForCrimes;                                  // +0x20
    long        _pad24[2];
    signed char WeaponOutCrimeEnabled;                             // +0x2c
    signed char GuardsIgnoreCrimes;                                // +0x2d
    void TransferState(CPersistContext& context, long unused);
};

void CDeedManager::TransferState(CPersistContext& context, long unused)
{
    context.Transfer("NextHandle", NextHandle, 0L);
    context.Transfer("WeaponOutCrimeEnabled", WeaponOutCrimeEnabled, (signed char)1);
    context.Transfer("GuardsIgnoreCrimes", GuardsIgnoreCrimes, (signed char)0);
    context.Transfer("VillageForCrimes", VillageForCrimes);
    if (context.Mode == 1 || context.Mode == 3) {
        CRecordVector records;
        records.reserve(OpinionDeedMap.count);
        bool flag;
        FlattenDeedMap(&records, &flag, OpinionDeedMap.header->left, OpinionDeedMap.header);
        context.Transfer("OpinionDeedMap", records);
    }
    else {
        CRecordVector records;
        context.Transfer("OpinionDeedMap", records);
        COpinionDeedRecord* last = records.last;
        for (COpinionDeedRecord* it = records.first; it != last; ++it) {
            COpinionDeedRecord value = *it;
            OpinionDeedMap.InsertUnique(value);
        }
    }
}
