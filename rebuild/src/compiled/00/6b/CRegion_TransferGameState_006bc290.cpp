// CRegion::TransferGameState(CPersistContext& context) -- persist the region's three game-state switches (appear on the world map, creature generation, sound themes)
// Leaf: ?TransferGameState@CRegion@@QAEXAAVCPersistContext@@@Z
// Retail: Fable.exe (TLC, VC7.1) function @ 0x006BC290, 182 bytes (+ int3 padding), `ret 4` (thiscall).
// Callers: four sites, 0x004FDF25 / 0x004FDFC6 / 0x00507A7A / 0x00507B6E (the world-map region load/save loops).
// Callees: CPersistContext::CheckCRC @ 0x00404500 (the empty key "" @ 0x0122D70E, x3), CPersistTraits<bool>::
//   TransferBinaryIn @ 0x00403EB0 (load), CDataOutputStream::WriteBool @ 0x00993E30 (save).
// Identity: Ego_r.pdb ?TransferGameState@CRegion@@QAEXAAVCPersistContext@@@Z.
// Layout: Ego_r.pdb CRegion with the retail 12-byte STLport vectors/map (engine/CRegion.h keeps the PDB's 16-byte
//   containers, so a local model is used): Maps +0, VisMaps +0xC, Name +0x18, DisplayName +0x1C, RegionDefName +0x20,
//   PDef +0x24, MiniMapGraphic +0x28, MiniMapScale +0x2C, MiniMapOffset +0x30, MiniMapRegionExitTextOffsets +0x38,
//   WorldMapOffset +0x44, NameGraphicOffset +0x4C, AppearOnWorldMap +0x54, CreatureGenerationEnabled +0x55,
//   SoundThemesEnabled +0x56.
// Shape: three inline CPersistContext::Transfer<bool> calls (the landed CHighlightItemDef::Transfer 0x0077CE00 template):
//   the member address formed into ebx before the CRC call, `sub eax,2 ; je ; dec eax ; jne` on the mode, the empty
//   traits temporary in the dead argument slot; the last member reuses `this` (`add edi,56h`).
// Label correction: the catalog `CGameState_TransferBoolFlags` is descriptive.
class CMemoryDataInputStream;
class CDataOutputStream { public:
    void WriteBool(bool value);                         // 0x00993E30
};
template <class T> struct CPersistTraits;
template <> struct CPersistTraits<bool> {
    CPersistTraits() {}
    void TransferBinaryIn(CMemoryDataInputStream* stream, bool* value);   // 0x00403EB0
    static void TransferBinaryOut(CDataOutputStream* stream, const bool& value) { stream->WriteBool(value); }
};
class CPersistContext {
public:
    unsigned char           _pad00[0x18];
    long                    m_mode;                     // +0x18 (2 = load, 3 = save)
    unsigned char           _pad1c[0x08];
    CMemoryDataInputStream* m_pLoadStream;              // +0x24
    CDataOutputStream*      m_pSaveStream;              // +0x28
    void CheckCRC(const char* name);                    // 0x00404500
    template <class T> void Transfer(const char* name, T& value)
    {
        CheckCRC(name);
        switch (m_mode) {
        case 2:
            CPersistTraits<T>().TransferBinaryIn(m_pLoadStream, &value);
            break;
        case 3:
            CPersistTraits<T>::TransferBinaryOut(m_pSaveStream, value);
            break;
        }
    }
};

class CCharStringData;
class CDefPointeeBase;

class CRegion {                                         // retail layout (12-byte STLport containers)
public:
    long*            Maps[3];                           // +0x00 vector<long>
    long*            VisMaps[3];                        // +0x0C vector<long>
    CCharStringData* Name;                              // +0x18 CCharString
    CCharStringData* DisplayName;                       // +0x1C CCharString
    CCharStringData* RegionDefName;                     // +0x20 CCharString
    CDefPointeeBase* PDef;                              // +0x24 CDefPointer<const CRegionDef>
    CCharStringData* MiniMapGraphic;                    // +0x28 CCharString
    float            MiniMapScale;                      // +0x2C
    float            MiniMapOffset[2];                  // +0x30 C2DVector
    void*            MiniMapRegionExitTextOffsets[3];   // +0x38 map<CCharString, C2DVector>
    float            WorldMapOffset[2];                 // +0x44 C2DVector
    float            NameGraphicOffset[2];              // +0x4C C2DVector
    bool             AppearOnWorldMap;                  // +0x54
    bool             CreatureGenerationEnabled;         // +0x55
    bool             SoundThemesEnabled;                // +0x56
    void TransferGameState(CPersistContext& context);
};

void CRegion::TransferGameState(CPersistContext& context)
{
    context.Transfer("", AppearOnWorldMap);
    context.Transfer("", CreatureGenerationEnabled);
    context.Transfer("", SoundThemesEnabled);
}