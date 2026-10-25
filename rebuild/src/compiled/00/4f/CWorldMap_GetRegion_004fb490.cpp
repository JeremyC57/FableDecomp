// CWorldMap::GetRegion(long index) -- the non-const reference to Regions[index]
// Leaf: ?GetRegion@CWorldMap@@QAEAAVCRegion@@J@Z
// Retail: Fable.exe (TLC, VC7.1) @ 0x004fb490, 15 bytes, thiscall `ret 4`: `mov eax,[esp+4] ; mov edx,[ecx+2Ch] ;
//   imul eax,eax,58h ; add eax,edx`.
// Renamed from NInventory::CTCInventoryBase::GetCategoryWithIndex (a stub returning CCategory*) (FINDINGS chunk 381: the label looked wrong).
// Name: Ego_r.pdb / FableWin.pdb carry both ?GetRegion@CWorldMap@@QAEAAVCRegion@@J@Z and ?GetRegion@CWorldMap@@QBEABVCRegion@@J@Z;
//   retail keeps the two byte-twin bodies 0x4FB490 / 0x4FC180 (no identical-code folding). This is the non-const one:
//   callers reach the map through CWorld vtable slot +0x30 = CWorld::GetWorldMap() (RTTI/PDB ?GetWorldMap@CWorld@@UAEAAVCWorldMap@@XZ, landed 0x4AE8B0) and write through the result: CGameScriptInterface::SetSoundThemesAsEnabledForRegion 0x88E0B0 stores CRegion::SoundThemesEnabled (+0x56) and SetCreatureGeneratorsEnabled 0x892CF0 stores CreatureGenerationEnabled (+0x55, PDB +0x61); the const twin is 0x4FC180.
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
    CRegion& GetRegion(long index);
};

CRegion& CWorldMap::GetRegion(long index)
{
    return RegionsStart[index];
}
