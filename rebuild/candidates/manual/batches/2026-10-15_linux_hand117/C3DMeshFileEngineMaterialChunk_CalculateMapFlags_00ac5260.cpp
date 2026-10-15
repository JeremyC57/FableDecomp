// C3DMeshFileEngineMaterialChunk::CalculateMapFlags() -- bit 0..3 set for each of the diffuse, bump, reflection and illumination maps present (by file name when the name set exists, else by bank index)
// Retail: Fable.exe (TLC, VC7.1) @ 0x00AC5260, 144 bytes, no calls.
// Retail layout of the landed constructor 0x00AC56F0 (the PDB header engine/C3DMeshFileEngineMaterialChunk.h sits
// 8 bytes high): MapFilenames +0x34 (CIVCountedPointer<CMapFilenames>, four CCharStrings at +8..+0x14 per the
// landed CMapFilenames constructor 0x00AC5190), the four bank indices +0x38..+0x44, MapFlags +0x48.
// A map name counts when its string block exists and its length (+4) is non-zero (inline null-case-first
// length accessor); the first `|= 1` after the zero store folds to `= 1`, the shared last test merges both arms.
struct CCharStringData {
    const char*   m_pChars;                         // +0x00
    unsigned long m_length;                         // +0x04
};
struct CCharString {
    CCharStringData* m_pStringData;
    unsigned long GetLength() const { return m_pStringData == 0 ? 0 : m_pStringData->m_length; }
};

struct CMapFilenames {
    virtual void v00();
    long m_count;                                   // +0x04
    CCharString DiffuseMap;                         // +0x08
    CCharString BumpMap;                            // +0x0C
    CCharString ReflectionMap;                      // +0x10
    CCharString IlluminationMap;                    // +0x14
};

struct C3DMeshFileEngineMaterialChunk {
    virtual ~C3DMeshFileEngineMaterialChunk();
    unsigned char _base_body[0x28];                 // +0x04 .. +0x2B (C3DMeshFileChunk, retail size)
    long MaterialIndex;                             // +0x2C
    CCharString Name;                               // +0x30
    CMapFilenames* MapFilenames;                    // +0x34 CIVCountedPointer<CMapFilenames>
    unsigned long DiffuseMapBankIndex;              // +0x38
    unsigned long BumpMapBankIndex;                 // +0x3C
    unsigned long ReflectionMapBankIndex;           // +0x40
    unsigned long IlluminationMapBankIndex;         // +0x44
    unsigned long MapFlags;                         // +0x48
    void CalculateMapFlags();
};

void C3DMeshFileEngineMaterialChunk::CalculateMapFlags()
{
    MapFlags = 0;
    if (MapFilenames) {
        if (MapFilenames->DiffuseMap.GetLength() != 0)
            MapFlags |= 1;
        if (MapFilenames->BumpMap.GetLength() != 0)
            MapFlags |= 2;
        if (MapFilenames->ReflectionMap.GetLength() != 0)
            MapFlags |= 4;
        if (MapFilenames->IlluminationMap.GetLength() != 0)
            MapFlags |= 8;
    }
    else {
        if (DiffuseMapBankIndex)
            MapFlags |= 1;
        if (BumpMapBankIndex)
            MapFlags |= 2;
        if (ReflectionMapBankIndex)
            MapFlags |= 4;
        if (IlluminationMapBankIndex)
            MapFlags |= 8;
    }
}
