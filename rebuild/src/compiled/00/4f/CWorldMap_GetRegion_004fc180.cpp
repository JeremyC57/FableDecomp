// CWorldMap::GetRegion(long index) const -- the const reference to Regions[index]
// Leaf: ?GetRegion@CWorldMap@@QBEABVCRegion@@J@Z
// Retail: Fable.exe (TLC, VC7.1) @ 0x004fc180, 15 bytes, thiscall `ret 4`: `mov eax,[esp+4] ; mov edx,[ecx+2Ch] ;
//   imul eax,eax,58h ; add eax,edx`.
// Renamed from the catalog name NInventory::CTCInventoryBase::GetCategoryWithIndex (source already CWorldMap_GetRegion, a non-const stub on a local struct S) (FINDINGS chunk 381: the label looked wrong).
// Name: Ego_r.pdb / FableWin.pdb carry both ?GetRegion@CWorldMap@@QAEAAVCRegion@@J@Z and ?GetRegion@CWorldMap@@QBEABVCRegion@@J@Z;
//   retail keeps the two byte-twin bodies 0x4FB490 / 0x4FC180 (no identical-code folding). This is the const one:
//   CWorld::IsCreatureGenerationEnabled 0x49EA40 (QBE) reaches the map through CWorld vtable slot +0x34 = CWorld::PeekWorldMap() const (RTTI/PDB ?PeekWorldMap@CWorld@@UBEABVCWorldMap@@XZ, landed 0x4AE8C0) and calls this body; the non-const twin 0x4FB490 is the one reached through GetWorldMap() (+0x30).
// Layout (struct_layouts_egor, re-based to the retail STLport sizes): CWorldMap::Regions is vector<CRegion> at PDB +0x34
//   -> retail +0x2C (the LoadedScriptedMapBrushes map and the CArray<CMapInfo> ahead of it are 4 bytes smaller each);
//   CRegion is PDB 0x64 -> retail 0x58 (two 12-byte vectors and a 12-byte map; SoundThemesEnabled +0x62 -> +0x56).
//   engine/CWorldMap.h / engine/CRegion.h are the PDB layout, so a local model is used.
class CRegion {
public:
    unsigned char _pad_0x00[0x58];                 // retail sizeof 0x58
};

class CWorldMap {
public:
    unsigned char _pad_0x00[0x2C];
    CRegion*      RegionsStart;                    // +0x2C Regions._M_start
    CRegion*      RegionsFinish;                   // +0x30
    CRegion*      RegionsEndOfStorage;             // +0x34
    const CRegion& GetRegion(long index) const;
};

const CRegion& CWorldMap::GetRegion(long index) const
{
    return RegionsStart[index];
}
