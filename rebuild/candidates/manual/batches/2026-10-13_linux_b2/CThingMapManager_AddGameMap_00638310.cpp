// CThingMapManager::AddGameMap(long)
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00638310, oracle length 37 bytes.
// Grade: landed -- byte-exact (MATCH/RELOCATION_MATCH) and behaviour PASS under the Linux lane
//        (Wine + DDK 13.10.4035 cl.exe, 2026-10-13); the lander's winning pragma, if any, heads the file.
//
// Retail bytes:  push esi ; mov esi,ecx ; mov ecx,[esi+4] ; mov eax,[ecx] ; call [eax+30h] ; mov ecx,[esp+8] ; mov eax,[eax+20h] ; lea edx,[ecx+ecx*8] ; lea edx,[eax+edx*8] ; push edx ; push ecx ; mov ecx,esi ; call <add map> ; pop esi ; ret 4
// engine/CThingMapManager.h (PDB): +0x04 CWorld* PWorld. Asks the world (virtual slot 12) for its
// map table owner, indexes that owner's 72-byte map records (pointer at +0x20) by `mapIndex`, and
// registers the record with the non-virtual worker at 0x006381A0 (mapIndex, record).
#include "engine/CThingMapManager.h"

struct CGameMapRecord { unsigned char _raw[72]; };     // 72-byte per-map record (opaque)

struct CGameMapTableCalls {
    unsigned char   _pad_0x00[0x20];
    CGameMapRecord* Maps;              // +0x20 record array
};

struct CWorldVirtuals {
    virtual void v00(); virtual void v04(); virtual void v08(); virtual void v0c();
    virtual void v10(); virtual void v14(); virtual void v18(); virtual void v1c();
    virtual void v20(); virtual void v24(); virtual void v28(); virtual void v2c();
    virtual CGameMapTableCalls* GetGameMapTable();   // slot 12 (+0x30)
};

struct CThingMapManager_Methods : CThingMapManager {
    void AddThingMap(long mapIndex, CGameMapRecord* record);    // 0x006381A0
    void AddGameMap(long mapIndex);
};

void CThingMapManager_Methods::AddGameMap(long mapIndex)
{
    CGameMapTableCalls* table = reinterpret_cast<CWorldVirtuals*>(this->PWorld)->GetGameMapTable();
    AddThingMap(mapIndex, &table->Maps[mapIndex]);
}
