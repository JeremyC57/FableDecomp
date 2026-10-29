# wave 7 hand edits: CWorldMap::GetRegion settlements (coordinator RELOC_CONFLICT 0064e019) and the region stand-ins.
#   0x004FB490 ?GetRegion@CWorldMap@@QAEAAVCRegion@@J@Z (non-const; through CTCBase::GetWorldMap / CWorld slot 12 GetWorldMap)
#   0x004FC180 ?GetRegion@CWorldMap@@QBEABVCRegion@@J@Z (const; through PeekWorldMap / CWorld slot 13 PeekWorldMap)
#   0x004FB150 ?GetLoadedRegion@CWorldMap@@QBEJXZ, 0x004FC210 ?GetRegionNumberFromName@CWorldMap@@QBEJABVCCharString@@@Z,
#   0x004FC190 ?GetRegionNumberMapIsIn@CWorldMap@@QBEJJ@Z (Ego_r.pdb; w6 DECISIONS / bound by 00729c50)
#   CWorld slot 12 / 13 = FableWin ?GetWorldMap@CWorld@@UAEAAVCWorldMap@@XZ / ?PeekWorldMap@CWorld@@UBEABVCWorldMap@@XZ
#   0x0064DD9A ?UpdateRegionExitNames@CDrawMiniMap@NPlayerGui@@IAEXABVCRegion@@@Z, 0x0064DFDE ?SetRegion@...@QAEXABVCRegion@@@Z,
#   0x0064E019 ToggleCentering: Ego_r / FableWin CDrawMiniMap order UpdateRegionExitNames, SetRegion, ToggleCentering
#   (0x69730B, 0x697592, 0x6975CC; retail 0x64DD9A, 0x64DFDE, 0x64E019, SetRegion calls the first last).
#   0x00686D20 ?PeekWorldMap@CTCBase@@IBEABVCWorldMap@@XZ (wave 1 DECISIONS)
import os, sys
sys.path.insert(0, os.path.dirname(os.path.abspath(__file__)))
from he7 import edit, edit_n, word, struct_to_class

# --- 0064e019 NPlayerGui::CDrawMiniMap::ToggleCentering
F = '0064e019'
edit(F, 'src', '// Callees: CDrawMiniMap::IsActive() @ 0x00643E09, the world\'s virtual slot 13 (`call [eax+34h]`, the world map, twice),\n'
     '// CWorldMap::GetCurrentRegionIndex() @ 0x004FB150',
     '// Callees: CDrawMiniMap::IsActive() @ 0x00643E09, the world\'s virtual slot 13 PeekWorldMap (`call [eax+34h]`, twice),\n'
     '// CWorldMap::GetLoadedRegion() @ 0x004FB150')
edit(F, 'src', '// CWorldMap::GetRegion(long) @ 0x004FC180 (0x58-byte regions), CDrawMiniMap::ApplyRegion(CRegion*) @ 0x0064DD9A (as the landed\n// SetRegion 0x0064DFDE calls it)',
     '// CWorldMap::GetRegion(long) const @ 0x004FC180 (0x58-byte regions), CDrawMiniMap::UpdateRegionExitNames(const CRegion&) @ 0x0064DD9A\n// (Ego_r.pdb order; the landed SetRegion 0x0064DFDE calls it)')
edit(F, 'src', '    CRegion& GetRegion(long index);                // 0x004FC180\n', '    const CRegion& GetRegion(long index) const;    // 0x004FC180\n')
edit(F, 'src', '    virtual CWorldMap* GetWorldMap();              // slot 13 (+0x34)\n', '    virtual const CWorldMap& PeekWorldMap() const; // slot 13 (+0x34)\n')
edit(F, 'src', '    bool IsActive();                               // 0x00643E09\n    void ApplyRegion(CRegion* region);             // 0x0064DD9A\n    void ToggleCentering();\n',
     '    bool IsActive();                               // 0x00643E09\n    void ToggleCentering();\nprotected:\n    void UpdateRegionExitNames(const CRegion& region);   // 0x0064DD9A\n')
edit(F, 'src', '            ApplyRegion(&World->GetWorldMap()->GetRegion(World->GetWorldMap()->GetLoadedRegion()));\n',
     '            UpdateRegionExitNames(World->PeekWorldMap().GetRegion(World->PeekWorldMap().GetLoadedRegion()));\n')

# --- 0064dfde NPlayerGui::CDrawMiniMap::SetRegion(const CRegion&) (own identity: Ego_r.pdb / FableWin.pdb)
F = '0064dfde'
edit(F, 'src', 'struct CDrawMiniMap {\n    char pad0[0xc];', 'namespace NPlayerGui {\nclass CDrawMiniMap { public:\n    char pad0[0xc];')
edit(F, 'src', '    void SetRegion(CRegion* r);\n    void ApplyRegion(CRegion* r);              // 0x64dd9a\n};\n',
     '    void SetRegion(const CRegion& r);\nprotected:\n    void UpdateRegionExitNames(const CRegion& r);   // 0x0064DD9A (Ego_r.pdb, protected)\n};\n}\n')
# CMiniMapDisplay::InitialiseOnRegionLoad overloads (Ego_r.pdb: CCharString& at 0x732840, CRegion& at 0x734380 = retail
# 0x0082A180 / 0x0082BA00 in the same order; ?InitialiseOnRegionLoad@CMiniMapDisplay@@QAEXABVCRegion@@@Z bound by 004fc8a0)
edit(F, 'src', '    void SetRegionDisp(CRegion* r);            // 0x82ba00\n    void InitialiseOnRegionLoad(void* s);      // 0x82a180\n',
     '    void InitialiseOnRegionLoad(const CRegion& region);      // 0x0082BA00\n    void InitialiseOnRegionLoad(const CCharString& name);    // 0x0082A180\n')
edit(F, 'src', 'class CRegion;\n', 'class CRegion;\nclass CCharString;\n')
edit(F, 'src', '    this->disp.SetRegionDisp(r);\n', '    this->disp.InitialiseOnRegionLoad(r);\n')
edit(F, 'src', '        this->disp.InitialiseOnRegionLoad(this->World + 0x84);\n', '        this->disp.InitialiseOnRegionLoad(*(const CCharString*)(this->World + 0x84));\n')
edit(F, 'src', 'void CDrawMiniMap::SetRegion(CRegion* r) {', 'void NPlayerGui::CDrawMiniMap::SetRegion(const CRegion& r) {')
edit(F, 'src', '    this->ApplyRegion(r);\n', '    this->UpdateRegionExitNames(r);\n')

# --- 007e27f0 CTCFineDialogue::Show: 0x00686D20 is CTCBase::PeekWorldMap; the "CWorld" model is the CWorldMap
F = '007e27f0'
edit(F, 'src', 'CTCBase::GetWorld @ 0x00686D20,\n//          CWorld::GetCurrentRegionIndex @ 0x004FB150, CWorld::GetRegion @ 0x004FC180,',
     'CTCBase::PeekWorldMap @ 0x00686D20,\n//          CWorldMap::GetLoadedRegion @ 0x004FB150, CWorldMap::GetRegion (const) @ 0x004FC180,')
edit(F, 'src', 'class CWorld {\npublic:\n    long GetCurrentRegionIndex();                       // 0x004FB150\n    CRegion& GetRegion(long index);                     // 0x004FC180\n};',
     'class CWorldMap {\npublic:\n    long GetLoadedRegion() const;                       // 0x004FB150\n    const CRegion& GetRegion(long index) const;         // 0x004FC180\n};')
edit(F, 'src', '    CWorld* GetWorld();                                 // 0x00686D20\n    void AddToGlobalUpdateList();                       // 0x00686ED0\n};',
     '    void AddToGlobalUpdateList();                       // 0x00686ED0\nprotected:\n    const CWorldMap& PeekWorldMap() const;              // 0x00686D20\n};')
edit(F, 'src', 'GetWorld()->GetRegion(GetWorld()->GetCurrentRegionIndex()).Name', 'PeekWorldMap().GetRegion(PeekWorldMap().GetLoadedRegion()).Name')

# --- 00892cf0 / 00892d80 CGameScriptInterface region-flag rows: CWorldMap_Retail / CRegion_Retail are CWorldMap / CRegion
for F in ('00892cf0', '00892d80'):
    struct_to_class(F, 'src', 'CRegion_Retail'); struct_to_class(F, 'src', 'CWorldMap_Retail')
    word(F, 'src', 'CRegion_Retail', 'CRegion'); word(F, 'src', 'CWorldMap_Retail', 'CWorldMap')
    edit(F, 'src', '    long GetCurrentRegionNumber();                  // 0x004FB150', '    long GetLoadedRegion() const;                   // 0x004FB150')
    edit(F, 'src', '    CRegion& GetRegion(long region);         // 0x004FC180\n    CRegion& GetRegionEntry(long region);    // 0x004FB490 (same body as GetRegion;',
         '    const CRegion& GetRegion(long region) const;    // 0x004FC180\n    CRegion& GetRegion(long region);                // 0x004FB490 (the non-const twin;')
    edit(F, 'src', '    long GetRegionNumberFromName(const CCharString& name);   // 0x004FC210', '    long GetRegionNumberFromName(const CCharString& name) const;   // 0x004FC210')
    edit(F, 'src', '    virtual CWorldMap* GetWorldMap();   // slot 12 (+0x30)\n    virtual CWorldMap* PeekWorldMap();   // slot 13 (+0x34)',
         '    virtual CWorldMap& GetWorldMap();   // slot 12 (+0x30)\n    virtual const CWorldMap& PeekWorldMap() const;   // slot 13 (+0x34)')
    edit(F, 'src', 'World->PeekWorldMap()->GetRegionNumberFromName(regionName)', 'World->PeekWorldMap().GetRegionNumberFromName(regionName)')
    edit(F, 'src', 'World->GetWorldMap()->GetRegionEntry(region)', 'World->GetWorldMap().GetRegion(region)')

# --- 0088e390 GetWanderingPopulationScriptDefNameInCurrentRegion: slot 13 is PeekWorldMap, then the const GetRegion
F = '0088e390'
struct_to_class(F, 'src', 'CRegion_Retail'); struct_to_class(F, 'src', 'CWorldMap_Retail')
word(F, 'src', 'CRegion_Retail', 'CRegion'); word(F, 'src', 'CWorldMap_Retail', 'CWorldMap')
edit(F, 'src', 'CWorldMap::GetCurrentRegionNumber @ 0x004FB150', 'CWorldMap::GetLoadedRegion @ 0x004FB150')
edit(F, 'src', '    long GetCurrentRegionNumber();                 // 0x004FB150', '    long GetLoadedRegion() const;                  // 0x004FB150')
edit(F, 'src', '    CRegion* GetRegion(long region);        // 0x004FC180', '    const CRegion& GetRegion(long region) const;   // 0x004FC180')
edit(F, 'src', '    long GetRegionNumberFromName(const CCharString& name);      // 0x004FC210', '    long GetRegionNumberFromName(const CCharString& name) const;   // 0x004FC210')
edit(F, 'src', '    virtual CWorldMap* GetWorldMapForEdit();  // slot 12 (+0x30)\n    virtual CWorldMap* GetWorldMap();         // slot 13 (+0x34)',
     '    virtual CWorldMap& GetWorldMap();         // slot 12 (+0x30)\n    virtual const CWorldMap& PeekWorldMap() const;   // slot 13 (+0x34)')
edit(F, 'src', '    CRegion* region = World->GetWorldMap()->GetRegion(World->GetWorldMap()->GetCurrentRegionNumber());',
     '    const CRegion* region = &World->PeekWorldMap().GetRegion(World->PeekWorldMap().GetLoadedRegion());')

# --- 0065a28f CTCVillage::GetNormalisedTimeLeftOnFines: 0x004FB490 is the non-const GetRegion (fam_tcb7 moves the CTCBase
#     accessors 0x00686D30 / 0x00686A70 into CTCBase afterwards)
F = '0065a28f'
edit(F, 'src', '    long GetRegionNumberMapIsIn(long map);              // 0x004FC190\n    CRegion* GetRegionWithIndex(long region);           // 0x004FB490\n',
     '    long GetRegionNumberMapIsIn(long map) const;        // 0x004FC190\n    CRegion& GetRegion(long region);                    // 0x004FB490\n')
edit(F, 'src', 'the region-by-number getter 0x004FB490 (propagated', 'CWorldMap::GetRegion 0x004FB490 (propagated')
edit(F, 'src', '    CRegion* region = GetWorldMap()->GetRegionWithIndex(', '    CRegion* region = &GetWorldMap()->GetRegion(')
