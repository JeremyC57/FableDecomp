// Grade: DIFFER(3v1344) -- parked unreached (skeleton only): inline CMemoryDataInputStream::Read template x6 (64-bit position checks, as landed 0x00757990), per-definition new + map lookup + inline vector push_backs, two block reads and a dirty re-sort; not reached beyond decode.
// CDefinitionManager::LoadBinaryDefinitions() -- PARK SKELETON (decoded only)
// Leaf: LoadBinaryDefinitions
// Retail: Fable.exe (TLC, VC7.1) function @ 0x009AFB90, length 1344 bytes.
// Decode: inline CMemoryDataInputStream::Read template x6 (64-bit position checks, as landed 0x00757990), per-definition new + map lookup + inline vector push_backs, two block reads and a dirty re-sort; not reached beyond decode.
struct CDefinitionManager {
    unsigned char _pad[0x100];
    void LoadBinaryDefinitions(void* stream);
};

void CDefinitionManager::LoadBinaryDefinitions(void* stream)
{
}
