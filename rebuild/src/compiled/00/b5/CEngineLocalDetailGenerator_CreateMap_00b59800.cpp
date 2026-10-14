// CEngineLocalDetailGenerator::CreateMap(SMapEntry& entry) -- when the entry has no local-detail map yet, allocate and construct one (0x70 bytes) from the entry
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00B59800, length 64 bytes.
// Callees: operator new @ 0x00BFEA1A (non-throwing: the null result is kept as a null map), the map constructor @ 0x00BDEFC0 (the entry by pointer; the manifest
// label "CProjectileWeapon::CProjectileWeapon" is propagated). Shape: `test [esi+4] ; jne` around the allocation, the null path stores the zero register,
// the constructed path stores eax; `this` is unused (`ret 4`, esi holds the entry).
void* __cdecl operator new(unsigned int size);     // 0x00BFEA1A

struct SMapEntry;
struct CLocalDetailMap_00bdefc0 {                  // 0x70 bytes
    unsigned char _body[0x70];
    CLocalDetailMap_00bdefc0(SMapEntry* entry);    // 0x00BDEFC0
};

struct SMapEntry {
    long                      m_id;
    CLocalDetailMap_00bdefc0* m_pMap;              // +0x04
};

struct CEngineLocalDetailGenerator {
    void CreateMap(SMapEntry& entry);
};

void CEngineLocalDetailGenerator::CreateMap(SMapEntry& entry)
{
    if (!entry.m_pMap)
        entry.m_pMap = new CLocalDetailMap_00bdefc0(&entry);
}