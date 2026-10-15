// CPersistContext::TransferMap(const char* name, CObjectAugmentationParticleSetMap& map) -- transfer a map with a zeroed two-word default element
// Leaf: TransferMap
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00757160, length 37 bytes.
// Callees: CPersistContext::TransferMap<CObjectAugmentationParticleSet, EObjectAugmentationType>(const char*, map&,
// const CObjectAugmentationParticleSet&) @ 0x00756D00 (CheckCRC, a CCharString local and a four-way mode switch).
// The catalog label "Transfer<C2DVector>" is propagated.
// Shape: an 8-byte frame temporary zeroed through ONE register (`xor eax,eax` stored twice: integer members) before
// the pushes, its address pushed with the map and name slots; `this` stays in ecx; `add esp,8 ; ret 8`.
// Layout: CPersistContext from the PDB (Ego_r.pdb, 0x2c bytes); no engine header exists, so a local model is used.
struct CObjectAugmentationParticleSetMap;
struct CObjectAugmentationParticleSet {          // two words, both zero by default
    long m_first;
    long m_second;
    CObjectAugmentationParticleSet() : m_first(0), m_second(0) { }
};
struct CDefinitionManager;
struct CSymbolMap;
struct CStringParser;
struct CCharString;
struct CMemoryDataInputStream;
struct CDataOutputStream;

// CPersistContext (0x2c bytes, PDB layout Ego_r.pdb; no engine header, HAND_OWNED in engine/INDEX.tsv)
struct CPersistContext {
    CDefinitionManager*     PDefinitionManager;           // +0x00
    CSymbolMap*             PSymbolMap;                   // +0x04
    CStringParser*          PParser;                      // +0x08
    void (__cdecl*          ErrorCallback)(CCharString*); // +0x0c
    CCharString*            PStringToClean;               // +0x10
    bool                    UseDefaults;                  // +0x14
    bool                    UseCRCCheck;                  // +0x15
    bool                    SingleTransfer;               // +0x16
    bool                    Finished;                     // +0x17
    long                    Mode;                         // +0x18 EMode: 0 text load, 1 text save, 2 binary load, 3 binary save
    bool                    Binary;                       // +0x1c
    CCharString*            PStringToSaveTo;              // +0x20
    CMemoryDataInputStream* PLoadStream;                  // +0x24
    CDataOutputStream*      PSaveStream;                  // +0x28

    void TransferMap(const char* name, CObjectAugmentationParticleSetMap& map, const CObjectAugmentationParticleSet& defaultValue);   // 0x00756D00
    void TransferMap(const char* name, CObjectAugmentationParticleSetMap& map);
};

void CPersistContext::TransferMap(const char* name, CObjectAugmentationParticleSetMap& map)
{
    TransferMap(name, map, CObjectAugmentationParticleSet());
}