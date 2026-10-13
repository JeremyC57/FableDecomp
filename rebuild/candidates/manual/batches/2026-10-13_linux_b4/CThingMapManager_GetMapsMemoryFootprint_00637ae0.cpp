// CThingMapManager::GetMapsMemoryFootprint()
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00637AE0, oracle length 44 bytes.
// Grade: landed -- byte-exact (MATCH/RELOCATION_MATCH) and behaviour PASS under the Linux lane
//        (Wine + DDK 13.10.4035 cl.exe, 2026-10-13); the lander's winning pragma, if any, heads the file.
//
// Retail bytes:  push ebx ; push esi ; push edi ; mov edi,ecx ; mov esi,[edi+14h] ; mov eax,[edi+18h] ; xor ebx,ebx ; cmp esi,eax ; je R ; L: mov eax,[esi] ; mov ecx,[eax] ; call CThingMap::GetMemoryFootprint ; add ebx,eax ; mov eax,[edi+18h] ; add esi,4 ; cmp esi,eax ; jne L ; R: pop edi ; pop esi ; mov eax,ebx ; pop ebx ; ret
// engine/CThingMapManager.h (PDB): +0x08 CArray ThingMaps, +0x18 CArray ActiveThingMaps. The
// first CArray is 4 bytes smaller in retail, so ActiveThingMaps' {first,last} sit at +0x14/+0x18.
// Each active entry points at a CThingMapInfo whose first member is the CThingMap; the footprints
// (0x0085CFE0, __thiscall) are summed.
// Layout note: in the shipped binary a std::vector / CArray / map / set / CVectorMap is 4 bytes
// smaller than in Ego_r.pdb (the editor build carries an extra word per container) and a
// std::list is 8 bytes smaller (retail keeps only the head pointer; the PDB list is 0xC bytes).
// A retail std::vector is {first, last, end} with `first` at +0. Members after such containers
// therefore sit earlier than the generated engine header says: 4 bytes per vector/map/set/CArray
// and 8 bytes per list that precede them.

struct CThingMapCalls {
    long GetMemoryFootprint();          // 0x0085CFE0
};

struct CThingMapInfo {
    CThingMapCalls* PMap;               // +0x00
};

struct CThingMapManager_Retail {
    void*          __vftable;           // +0x00
    void*          PWorld;              // +0x04 (PDB)
    unsigned char  ThingMaps[12];       // +0x08 CArray<CThingMapInfo> (retail 12 bytes)
    CThingMapInfo** ActiveThingMaps_First;   // +0x14 (PDB CArray at +0x18)
    CThingMapInfo** ActiveThingMaps_Last;    // +0x18
    long GetMapsMemoryFootprint();
};

long CThingMapManager_Retail::GetMapsMemoryFootprint()
{
    long total = 0;
    for (CThingMapInfo** it = this->ActiveThingMaps_First; it != this->ActiveThingMaps_Last; ++it)
        total += (*it)->PMap->GetMemoryFootprint();
    return total;
}
