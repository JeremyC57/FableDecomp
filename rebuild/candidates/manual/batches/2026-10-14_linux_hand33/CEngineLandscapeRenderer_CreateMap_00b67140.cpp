// CEngineLandscapeRenderer::CreateMap(CEngineMap* map, long index) -- gives the engine map a new 0x60-byte CEngineLandscapeMap built over (map, index), stored in the map's first word; null there when the allocation fails
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00B67140, length 50 bytes (64 with padding).
// Callees: operator new @ 0x00BFEA1A (non-throwing: the null result skips the constructor and stores 0), CEngineLandscapeMap::CEngineLandscapeMap(CEngineMap*, long)
// @ 0x00BDD2F0. Shape: `this` is unused (`ret 8`); the map pointer is re-read for the store in the null arm, the constructor's returned `this` is stored directly.
void* __cdecl operator new(unsigned int size);     // 0x00BFEA1A

struct CEngineMap;

struct CEngineLandscapeMap {                       // 0x60 bytes
    unsigned char _body[0x60];
    CEngineLandscapeMap(CEngineMap* map, long index);   // 0x00BDD2F0
};

struct CEngineMap {
    CEngineLandscapeMap* m_pLandscapeMap;          // +0x00
};

struct CEngineLandscapeRenderer {
    void CreateMap(CEngineMap* map, long index);
};

void CEngineLandscapeRenderer::CreateMap(CEngineMap* map, long index)
{
    map->m_pLandscapeMap = new CEngineLandscapeMap(map, index);
}
