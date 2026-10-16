// CWorldManager::InitializeSteeringAfterSave(CSteeringSaveRecord*) -- steering library: rebuild a steering group for a saved record when its world entry still exists
// Flags: /EHsc
// Retail: Fable.exe @ 0x00C096E0, 120 bytes, `ret 4`, EH frame (handler 0x01210C33; state 0 guards the new'd block).
// Library code (the AI steering module at 0x00C0xxxx-0x00C4xxxx, no RTTI); `this` is not used.
// Callees: the __fastcall lookup 0x00C1C1A0 taking the record's +0x24 and +0x2C words (manifest label
// "CWorldMap::SaveToFile" is wrong; the callee ignores the second register), operator new 0x00BFEA1A (0x84 bytes),
// CSteeringGroup::CSteeringGroup(CSteeringSaveRecord*, CSteeringWorldEntry*) 0x00C094A0 (label "CSteeringGroup::Initialize").
// Shape: a missing entry or a failed allocation returns 0; `return new CSteeringGroup(...)` keeps the allocation in the
// EH frame slot for the constructor's unwind.
struct CSteeringWorldEntry;

struct CSteeringSaveRecord {
    unsigned char _pad[0x24];
    void*         WorldKey;                     // +0x24
    unsigned char _pad28[0x4];
    void*         EntryKey;                     // +0x2C
};

CSteeringWorldEntry* __fastcall LookupSteeringWorldEntry(void* worldKey, void* entryKey);   // 0x00C1C1A0

struct CSteeringGroup {
    CSteeringGroup(CSteeringSaveRecord* record, CSteeringWorldEntry* entry);   // 0x00C094A0
    unsigned char _body[0x84];
};

struct CWorldManager {
    CSteeringGroup* InitializeSteeringAfterSave(CSteeringSaveRecord* record);
};

CSteeringGroup* CWorldManager::InitializeSteeringAfterSave(CSteeringSaveRecord* record)
{
    CSteeringWorldEntry* entry = LookupSteeringWorldEntry(record->WorldKey, record->EntryKey);
    if (entry)
        return new CSteeringGroup(record, entry);
    return 0;
}
