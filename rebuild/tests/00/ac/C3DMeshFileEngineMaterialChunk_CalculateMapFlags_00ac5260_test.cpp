#include <cstdio>
struct CCharStringData { const char* m_pChars; unsigned long m_length; };
struct CCharString {
    CCharStringData* m_pStringData;
    unsigned long GetLength() const { return m_pStringData == 0 ? 0 : m_pStringData->m_length; }
};
struct CMapFilenames { CCharString DiffuseMap, BumpMap, ReflectionMap, IlluminationMap; };
struct C3DMeshFileEngineMaterialChunk {
    CMapFilenames* MapFilenames;
    unsigned long DiffuseMapBankIndex, BumpMapBankIndex, ReflectionMapBankIndex, IlluminationMapBankIndex;
    unsigned long MapFlags;
    void CalculateMapFlags()
    {
        MapFlags = 0;
        if (MapFilenames) {
            if (MapFilenames->DiffuseMap.GetLength() != 0) MapFlags |= 1;
            if (MapFilenames->BumpMap.GetLength() != 0) MapFlags |= 2;
            if (MapFilenames->ReflectionMap.GetLength() != 0) MapFlags |= 4;
            if (MapFilenames->IlluminationMap.GetLength() != 0) MapFlags |= 8;
        }
        else {
            if (DiffuseMapBankIndex) MapFlags |= 1;
            if (BumpMapBankIndex) MapFlags |= 2;
            if (ReflectionMapBankIndex) MapFlags |= 4;
            if (IlluminationMapBankIndex) MapFlags |= 8;
        }
    }
};
int main()
{
    CCharStringData full = { "a.tga", 5 }, empty = { "", 0 };
    CMapFilenames names; names.DiffuseMap.m_pStringData = &full; names.BumpMap.m_pStringData = 0;
    names.ReflectionMap.m_pStringData = &empty; names.IlluminationMap.m_pStringData = &full;
    C3DMeshFileEngineMaterialChunk c; c.MapFilenames = &names; c.MapFlags = 0xFF;
    c.DiffuseMapBankIndex = 0; c.BumpMapBankIndex = 7; c.ReflectionMapBankIndex = 3; c.IlluminationMapBankIndex = 0;
    c.CalculateMapFlags();
    int ok = c.MapFlags == 9;
    c.MapFilenames = 0;
    c.CalculateMapFlags();
    ok = ok && c.MapFlags == 6;
    if (!ok) { std::printf("00ac5260_TEST FAIL\n"); return 1; }
    std::printf("00ac5260_TEST PASS\n");
    return 0;
}