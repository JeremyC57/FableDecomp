# wave 7 hand edits, part d: CTCBase accessor rows the family could not take (w6 DEFERRED "hand" + fam_tcb7 skips).
#   CTCBase (Ego_r.pdb): protected GetWorld() const 0x00686A70 (CWorld&), PeekWorld() const 0x00686A80, PeekWorldMap() const
#   0x00686D20, GetWorldMap() 0x00686D30, GetPlayerManager() 0x00686D40, PeekPlayerManager() const 0x00686D50.
#   CPlayerManager (Ego_r.pdb): GetMainPlayer() const 0x00449960 (const CPlayer&; bound by 007b0880), GetMainPlayer()
#   0x00449970, GetMainPlayerNumber() const 0x00449700.  CThingObject::Create 0x00703210 is the Ego_r static __fastcall
#   ?Create@CThingObject@@SIPAV1@JABVC3DVector@@JPBVCRightHandedSet@@KABVCCharString@@@Z (third argument the player number).
import os, sys
sys.path.insert(0, os.path.dirname(os.path.abspath(__file__)))
from he import edit
from he7 import edit_n, word

TCB_PPM = 'class CTCBase { protected:\n    const CPlayerManager& PeekPlayerManager() const;   // 0x00686D50 (Ego_r.pdb CTCBase, protected)\n};\n'

# ---- 005819f2 / 00581fd1 CTCInventory: PeekPlayerManager().GetMainPlayer() const (the dropped item-count chain)
from he import path
def has(F, s, k='src'):
    return s in open(path(F, k)[0], encoding='utf-8', errors='surrogateescape').read()
for F in ('005819f2', '00581fd1'):
  if not has(F, 'class CTCBase { protected:'):
    edit(F, 'src', '// 0x00449960\n};\n\nCWorld* GetWorld();', '// 0x00449960 (Ego_r.pdb const overload)\n};\n\n@@TCB@@')
    edit(F, 'src', 'class CWorld { public:\n    CPlayer* GetMainPlayer();  ', 'class CPlayerManager { public:\n    const CPlayer& GetMainPlayer() const;')
    p_, _ = __import__('he').path(F, 'src'); s = open(p_).read()
    if '@@TCB@@' in s:
        import re as _re
        s = _re.sub(r'@@TCB@@[ \t]*// 0x00686D50\n', TCB_PPM, s); open(p_, 'w').write(s)
    edit(F, 'src', 'class CTCInventory { public:', 'class CTCInventory : public CTCBase { public:')
    edit(F, 'src', '    GetWorld()->GetMainPlayer()->m_pItems->GetCount();', '    PeekPlayerManager().GetMainPlayer().m_pItems->GetCount();')
F = '005819f2'
edit(F, 'src', '// Callees: 0x00686D50 GetWorld (free: `return g_pGame->m_30;`, 0x013B8A1C), 0x00449960\n// GetMainPlayer (__fastcall: `return GetPlayer(m_1c);`)',
     '// Callees: 0x00686D50 CTCBase::PeekPlayerManager (`return g_pGame->m_30;`, 0x013B8A1C), 0x00449960\n// CPlayerManager::GetMainPlayer() const (`return GetPlayer(m_1c);`)')
edit(F, 'src', '// neighbours are CTCInventory methods. The chain world -> main player', '// neighbours are CTCInventory methods. The chain player manager -> main player')
F = '00581fd1'
edit(F, 'src', '// Callees: GetWorld 0x00686D50 (free: `return g_pGame->m_30;`), CWorld::GetMainPlayer 0x00449960,',
     '// Callees: CTCBase::PeekPlayerManager 0x00686D50 (`return g_pGame->m_30;`), CPlayerManager::GetMainPlayer() const 0x00449960,')
edit(F, 'src', '// Ego_r PDB: ?GetQuickAccessItemButtonIndex@CTCInventory@@QAEJJ@Z.', '// Ego_r PDB: ?GetQuickAccessItemButtonIndex@CTCInventory@@QBEJJ@Z (const).')
edit(F, 'src', '    long GetQuickAccessItemButtonIndex(long item);\n', '    long GetQuickAccessItemButtonIndex(long item) const;\n')
edit(F, 'src', 'long CTCInventory::GetQuickAccessItemButtonIndex(long item)\n', 'long CTCInventory::GetQuickAccessItemButtonIndex(long item) const\n')
edit(F, 'src', '    long& operator[](unsigned long n) { return first[n]; }', '    long& operator[](unsigned long n) const { return first[n]; }')

# ---- 005ea9b0 CTCTavernGame::CreateGamePiece: PeekPlayerManager().GetMainPlayerNumber() feeds CThingObject::Create's player
F = '005ea9b0'
edit(F, 'src', '// Callees: CTCBase::GetWorld @ 0x00686D50, CWorld::GetMap @ 0x00449700 (catalog label "DrawGetEnvironment"),',
     '// Callees: CTCBase::PeekPlayerManager @ 0x00686D50, CPlayerManager::GetMainPlayerNumber @ 0x00449700 (Ego_r.pdb; catalog "DrawGetEnvironment"),')
edit(F, 'src', '// CThing::GetPos @ 0x004C73D0, CThingObject::Create (fastcall) @ 0x00703210,',
     '// CThing::GetPos @ 0x004C73D0, CThingObject::Create (static __fastcall, Ego_r.pdb) @ 0x00703210,')
edit(F, 'src', 'class CThingObject : public CThing { public: };\n',
     'class CThingObject : public CThing { public:\n'
     '    static CThingObject* __fastcall Create(long defIndex, const C3DVector& pos, long playerNumber, const CRightHandedSet* rh,\n'
     '                                           unsigned long flags, const CCharString& name);   // 0x00703210 (Ego_r.pdb)\n};\n')
edit(F, 'src', 'class CMap;\nCThingObject* __fastcall CreateThingObject(long defIndex, const C3DVector& pos, CMap* map, const CRightHandedSet& rh,\n'
     '                                           unsigned long flags, const CCharString& name);   // 0x00703210\n'
     'class CWorld { public: CMap* GetMap(); };                      // 0x00449700\n',
     'class CPlayerManager { public: long GetMainPlayerNumber() const; };   // 0x00449700 (Ego_r.pdb)\n' + TCB_PPM)
edit(F, 'src', 'class CTCTavernGame { public:', 'class CTCTavernGame : public CTCBase { public:')
edit(F, 'src', '    CWorld* GetWorld_00686d50();                                 // 0x00686D50\n', '')
edit(F, 'src', '    CMap* map = GetWorld_00686d50()->GetMap();', '    long player = PeekPlayerManager().GetMainPlayerNumber();')
edit(F, 'src', 'CreateThingObject(defIndex, pos, map, rh, 0, "");', 'CThingObject::Create(defIndex, pos, player, &rh, 0, "");')

# ---- polymorphic X whose slot 0 is the virtual destructor: CTCBase gets its own virtual ~CTCBase (retail slot 0), X derives
#      from it as the primary base (shared vfptr at +0, so neither the layout nor X's slot numbers move)
TCB_POLY_PPM = ('class CTCBase { public:\n    virtual ~CTCBase();                                           // slot 0 (0x00686830)\n'
                'protected:\n    const CPlayerManager& PeekPlayerManager() const;   // 0x00686D50 (Ego_r.pdb CTCBase, protected)\n};\n')
F = '005bc527'
edit(F, 'src', '// Callees: the player-manager getter @ 0x00686D50', '// Callees: CTCBase::PeekPlayerManager @ 0x00686D50')
edit(F, 'src', 'and its player-number accessor @ 0x00449700 (`mov eax,[ecx+1Ch]`),',
     'and CPlayerManager::GetMainPlayerNumber() const @ 0x00449700 (`mov eax,[ecx+1Ch]`),')
edit(F, 'src', '    long GetMainPlayerNumber();                   // 0x00449700\n};\nCPlayerManager* GetPlayerManager_00686d50();               // 0x00686D50\n',
     '    long GetMainPlayerNumber() const;             // 0x00449700 (Ego_r.pdb)\n};\n' + TCB_POLY_PPM)
edit(F, 'src', 'namespace NInventory {\n\nclass CTCInventoryBase {\npublic:\n', 'namespace NInventory {\n\nclass CTCInventoryBase : public ::CTCBase {\npublic:\n')
edit(F, 'src', '(signed char)GetPlayerManager_00686d50()->GetMainPlayerNumber();', '(signed char)PeekPlayerManager().GetMainPlayerNumber();')

TCB_POLY_PW = ('class CTCBase { public:\n    virtual ~CTCBase();                                           // slot 0 (0x00686830)\n'
               'protected:\n    const CWorld& PeekWorld() const;   // 0x00686A80 (Ego_r.pdb CTCBase, protected)\n};\n')
# ---- 005faa30 CTCInventoryMap::ResetWorldMapDisplay: PeekWorld().PeekWorldMap().GetLoadedRegion() (slot-0 placeholder -> ~CTCBase)
F = '005faa30'
edit(F, 'src', '// Shape: WorldMapActive (+0x234) = true; CTCBase::GetWorld() const 0x00686A80', '// Shape: WorldMapActive (+0x234) = true; CTCBase::PeekWorld() const 0x00686A80')
edit(F, 'src', 'the world\'s virtual slot 13 (`call\n// [edx+34h]`) returns the current map object, whose region-index getter 0x004FB150',
     'the world\'s virtual slot 13 PeekWorldMap (`call\n// [edx+34h]`) returns the world map, whose GetLoadedRegion() const 0x004FB150')
edit(F, 'src', 'struct CWorldRegionMap {\n    long GetActiveRegionIndex();                   // 0x004FB150\n};\n',
     'class CWorldMap { public:\n    long GetLoadedRegion() const;                  // 0x004FB150 (Ego_r.pdb)\n};\n')
edit(F, 'src', '    virtual CWorldRegionMap* GetRegionMap();       // slot 13 (+0x34)\n',
     '    virtual const CWorldMap& PeekWorldMap() const; // slot 13 (+0x34) (FableWin.pdb)\n')
edit(F, 'src', 'struct CTCInventoryBase {\n    virtual void v00();\n    unsigned char _base_04[0x12C];                 // .. +0x130\n    CWorld* GetWorld() const;                      // 0x00686A80 (CTCBase)\n};\n',
     TCB_POLY_PW + 'struct CTCInventoryBase : public CTCBase {\n    unsigned char _base_04[0x12C];                 // .. +0x130\n};\n')
edit(F, 'src', '    long NumRegions;                               // +0x23C\n    virtual void v00();\n', '    long NumRegions;                               // +0x23C\n')
edit(F, 'src', 'ActiveRegionIndex = GetWorld()->GetRegionMap()->GetActiveRegionIndex();', 'ActiveRegionIndex = PeekWorld().PeekWorldMap().GetLoadedRegion();')

# ---- 00602e30 CTCInventoryMap::OnCreate: PeekWorld().PeekWorldMap()->GetNoMaps() const (0x004FB470, PDB GetNoMaps QBEJXZ)
F = '00602e30'
edit(F, 'src', 'CTCBase::GetWorld\n// 0x00686A80, CWorld slot 13 (+0x34, GetWorldMap), CWorldMap::GetNoRegions 0x004FB470 (manifest',
     'CTCBase::PeekWorld\n// 0x00686A80, CWorld slot 13 (+0x34, PeekWorldMap), CWorldMap::GetNoMaps 0x004FB470 (manifest')
edit(F, 'src', '    long GetNoRegions();                        // 0x004FB470\n', '    long GetNoMaps() const;                     // 0x004FB470 (PDB ?GetNoMaps@CWorldMap@@QBEJXZ)\n')
edit(F, 'src', '    virtual CWorldMap* GetWorldMap();           // slot 13 (+0x34)\n', '    virtual const CWorldMap* PeekWorldMap() const;   // slot 13 (+0x34) (FableWin.pdb)\n')
edit(F, 'src', 'struct CTCInventoryBase_Map {                   // retail 0x130 bytes\n    virtual void v00();\n',
     TCB_POLY_PW + 'struct CTCInventoryBase_Map : public CTCBase {  // retail 0x130 bytes\n')
edit(F, 'src', '    CWorld* GetWorld() const;                   // 0x00686A80 (CTCBase)\n', '')
edit(F, 'src', 'NumRegions = GetWorld()->GetWorldMap()->GetNoRegions() - 1;', 'NumRegions = PeekWorld().PeekWorldMap()->GetNoMaps() - 1;')

# ---- 00602f30 CTCInventoryMap::ProcessButtonAReleased: PeekWorld() (three uses), const PeekWorldMap slot 13
F = '00602f30'
edit(F, 'src', 'CTCBase::GetWorld() const @ 0x00686A80 (manifest "CViewBase::GetDisplayEngine")', 'CTCBase::PeekWorld() const @ 0x00686A80 (manifest "CViewBase::GetDisplayEngine")')
edit(F, 'src', 'kept across the second GetWorld call', 'kept across the second PeekWorld call')
edit(F, 'src', '    virtual CWorldMap* PeekWorldMap();             // slot 13 (+0x34)\n', '    virtual const CWorldMap* PeekWorldMap() const; // slot 13 (+0x34) (FableWin.pdb)\n')
edit(F, 'src', 'struct CTCInventoryBase_Map {                      // retail 0x130 bytes\n    virtual void v00(); virtual void v01();',
     TCB_POLY_PW + 'struct CTCInventoryBase_Map : public CTCBase {     // retail 0x130 bytes\n    virtual void v01();')
edit(F, 'src', '    CWorld* GetWorld() const;                      // 0x00686A80 (CTCBase)\n', '')
edit(F, 'src', 'const CRegion& region = GetWorld()->PeekWorldMap()->GetRegion(ActiveRegionIndex);', 'const CRegion& region = PeekWorld().PeekWorldMap()->GetRegion(ActiveRegionIndex);')
edit(F, 'src', '            CWorld* world = GetWorld();\n', '            const CWorld* world = &PeekWorld();\n')
edit(F, 'src', 'if (GetWorld()->m_teleportAllowed && ', 'if (PeekWorld().m_teleportAllowed && ')

# ---- 0073a8e0 CTCCreatureModeManager::AllocateMode: PeekWorld().GetPRandomSeed() const (0x0049D810 bound QBE by 00662040)
F = '0073a8e0'
edit(F, 'src', 'CTCBase::GetWorld() const @ 0x00686A80, CWorld::GetPRandomSeed() @ 0x0049D810', 'CTCBase::PeekWorld() const @ 0x00686A80, CWorld::GetPRandomSeed() const @ 0x0049D810')
edit(F, 'src', '    unsigned long* GetPRandomSeed();                    // 0x0049D810\n', '    unsigned long* GetPRandomSeed() const;              // 0x0049D810\n')
edit(F, 'src', 'class CTCCreatureModeManager { public:\n    virtual void v00();\n', TCB_POLY_PW + 'class CTCCreatureModeManager : public CTCBase { public:\n')
edit(F, 'src', '    CWorld* GetWorld() const;                           // 0x00686A80 (CTCBase)\n', '')
edit(F, 'src', 'unsigned long* seed = GetWorld()->GetPRandomSeed();', 'unsigned long* seed = PeekWorld().GetPRandomSeed();')

# ---- 00730f90 CTCDoor::UpdateOpenStatusTriggerTypePerson: PeekThingSearchTools() const 0x00686D90 and the Ego_r template
#      instance ??$GetPFirstThingInRoughArea@V?$CFilter_Bind2@VCThingFilter_InArea@@VCThingFilter_IsWithinZRange@@@@VCThingCreatureBase@@@
#      CThingSearchTools@@QBE_N... 0x007313B0 (the stand-in struct was that CFilter_Bind2), PeekWorld() const 0x00686A80
F = '00730f90'
edit(F, 'src', 'CTCBase::GetThingSearchTools\n//   @ 0x00686D90,', 'CTCBase::PeekThingSearchTools\n//   @ 0x00686D90,')
edit(F, 'src', 'CThingCreatureBase> @ 0x007313B0, CTCBase::GetWorld const @ 0x00686A80,', 'CThingCreatureBase> const @ 0x007313B0, CTCBase::PeekWorld const @ 0x00686A80,')
edit(F, 'src', 'struct CInAreaZRangeFilter {                    // CFilter_Bind2<CThingFilter_InArea, CThingFilter_IsWithinZRange>\n'
     '    const CThingFilter_InArea* First;\n    const CThingFilter_IsWithinZRange* Second;\n'
     '    CInAreaZRangeFilter(const CThingFilter_InArea& a, const CThingFilter_IsWithinZRange& b) : First(&a), Second(&b) { }\n};\n',
     'template <class TFilter1, class TFilter2> class CFilter_Bind2 { public:\n'
     '    const TFilter1* First;\n    const TFilter2* Second;\n'
     '    CFilter_Bind2(const TFilter1& a, const TFilter2& b) : First(&a), Second(&b) { }\n};\n')
edit(F, 'src', '    bool GetPFirstThingInRoughArea(const C2DBoxI& box, const CInAreaZRangeFilter& filter, CThingCreatureBase** out);   // 0x007313B0\n',
     '    template <class TFilter, class TThing>\n'
     '    bool GetPFirstThingInRoughArea(const C2DBoxI& box, const TFilter& filter, TThing** out) const;   // 0x007313B0 (Ego_r.pdb instance)\n')
edit(F, 'src', 'class CTCDoor { public:\n    virtual void v00();\n',
     'class CTCBase { public:\n    virtual ~CTCBase();                                           // slot 0 (0x00686830)\n'
     'protected:\n    const CWorld& PeekWorld() const;   // 0x00686A80 (Ego_r.pdb CTCBase, protected)\n'
     '    const CThingSearchTools& PeekThingSearchTools() const;   // 0x00686D90 (Ego_r.pdb CTCBase, protected)\n};\n'
     'class CTCDoor : public CTCBase { public:\n')
edit(F, 'src', '    CThingSearchTools* GetThingSearchTools() const; // 0x00686D90 (CTCBase)\n    const CWorld* GetWorld() const;             // 0x00686A80 (CTCBase)\n', '')
edit(F, 'src', 'GetThingSearchTools()->GetPFirstThingInRoughArea(box.ToInteger(), CInAreaZRangeFilter(',
     'PeekThingSearchTools().GetPFirstThingInRoughArea(box.ToInteger(), CFilter_Bind2<CThingFilter_InArea, CThingFilter_IsWithinZRange>(')
edit(F, 'src', 'LastCreatureFoundWF = GetWorld()->GetFrame();', 'LastCreatureFoundWF = PeekWorld().GetFrame();')
edit(F, 'src', '        const CWorld* world = GetWorld();\n', '        const CWorld* world = &PeekWorld();\n')

# ---- 0075c530 CTCCameraPointDefinitionBase::IsAngleValid: PeekWorld() (two uses)
F = '0075c530'
edit(F, 'src', '// Callees: CTCBase::GetWorld() const @ 0x00686A80,', '// Callees: CTCBase::PeekWorld() const @ 0x00686A80,')
edit(F, 'src', 'class CTCCameraPointDefinitionBase {\npublic:\n#define VF(n) virtual void v##n();\n    VF(00) VF(01)',
     TCB_POLY_PW + 'class CTCCameraPointDefinitionBase : public CTCBase {\npublic:\n#define VF(n) virtual void v##n();\n    VF(01)')
edit(F, 'src', '    CWorld* GetWorld() const;                   // 0x00686A80 (CTCBase)\n', '')
edit_n(F, 'src', 'GetWorld()->', 'PeekWorld().', 2)

# ---- 00779340 CTCDRegionExit::SetUpPEntranceConnectedToFromUID: GetThingManager() 0x00686AE0 (bound by 006ca4e0, Ego_r),
#      PeekComponent() const 0x00686D60 (const CMainGameComponent&), GetWorldMap() 0x00686D30
F = '00779340'
edit(F, 'src', '// vtable 0x01238C6C is CIntelligentPointer<CThing>), the CTCBase subsystem accessor @ 0x00686D60 (virtual slot 9, bool),',
     '// vtable 0x01238C6C is CIntelligentPointer<CThing>), CTCBase::PeekComponent @ 0x00686D60 (its virtual slot 9, bool),')
edit(F, 'src', 'struct CGameSubsystem {                         // [0x013B8A1C]+0x1C, not identified (landed CTCCrateStack 0x007B6A80)\n',
     'class CMainGameComponent { public:             // [0x013B8A1C]+0x1C: CTCBase::PeekComponent\'s const CMainGameComponent& (Ego_r.pdb)\n')
edit(F, 'src', '    virtual bool IsSuppressed();                // slot 9 (+0x24)\n', '    virtual bool IsSuppressed() const;          // slot 9 (+0x24) (name from use)\n')
edit(F, 'src', 'class CTCDRegionExit {\npublic:\n    virtual void v00();\n',
     'class CTCBase { public:\n    virtual ~CTCBase();                                           // slot 0 (0x00686830)\n'
     '    CThing* ParentThing;                        // +0x04\n    bool    ToBeRemoved;                        // +0x08\n'
     '    bool    IsInGlobalUpdateTCs;                // +0x09\nprotected:\n'
     '    CThingManager& GetThingManager();                    // 0x00686AE0 (Ego_r.pdb CTCBase, protected)\n'
     '    const CMainGameComponent& PeekComponent() const;     // 0x00686D60 (Ego_r.pdb CTCBase, protected)\n'
     '    CWorldMap& GetWorldMap();                            // 0x00686D30 (Ego_r.pdb CTCBase, protected)\n};\n\n'
     'class CTCDRegionExit : public CTCBase {\npublic:\n')
edit(F, 'src', 'class CTCDRegionExit : public CTCBase {\npublic:\n    CThing* ParentThing;                        // +0x04\n    bool    ToBeRemoved;                        // +0x08\n'
     '    bool    IsInGlobalUpdateTCs;                // +0x09\n', 'class CTCDRegionExit : public CTCBase {\npublic:\n')
edit(F, 'src', '    CThingManager*  GetThingManager();          // 0x00686AE0 (CTCBase)\n    CGameSubsystem* GetGameSubsystem();         // 0x00686D60 (CTCBase)\n'
     '    CWorldMap*      GetWorldMap();              // 0x00686D30 (CTCBase)\n', '')
edit(F, 'src', 'GetThingManager()->GetThingByUID(', 'GetThingManager().GetThingByUID(')
edit(F, 'src', 'if (!GetGameSubsystem()->IsSuppressed())', 'if (!PeekComponent().IsSuppressed())')
edit_n(F, 'src', 'GetWorldMap()->', 'GetWorldMap().', 3)

# ---- 007dd990 CTCCreatureFlyInCircles::CheckForHits: PeekWorld().GetCurrentPlayer() const (0x0049D9D0 bound QBE by 0095f9a0)
F = '007dd990'
edit(F, 'src', 'Callees: CTCBase::GetWorld() const @ 0x00686A80', 'Callees: CTCBase::PeekWorld() const @ 0x00686A80')
edit(F, 'src', '    CPlayer* GetCurrentPlayer();                            // 0x0049D9D0\n', '    CPlayer* GetCurrentPlayer() const;                      // 0x0049D9D0\n')
edit(F, 'src', 'struct CTCCreatureFlyInCircles {\n    virtual void v00();\n', TCB_POLY_PW + 'struct CTCCreatureFlyInCircles : public CTCBase {\n')
edit(F, 'src', '    CWorld* GetWorld() const;                               // 0x00686A80 (CTCBase)\n', '')
edit(F, 'src', 'CThing* hero = GetWorld()->GetCurrentPlayer()->GetPControlledCreature();', 'CThing* hero = PeekWorld().GetCurrentPlayer()->GetPControlledCreature();')

# ---- 0057cb7e CTCHeroStats::ProcessDrunkenness: the CTCBase run is Ego_r CTCBase less 0x2B33F0 (GetDefinitionManager
#      0x006B1DC0 -> 0x006869D0, GetWorld 0x006B1E60 -> 0x00686A70, PeekWorld -> 0x00686A80, PeekPlayerManager 0x006B2140 ->
#      0x00686D50, GetPWorldSeed 0x006B2190 -> 0x00686DA0)
F = '0057cb7e'
edit(F, 'src', '// Callees: CTCBase::GetWorld() @ 0x00686A70 / GetWorld() const @ 0x00686A80,', '// Callees: CTCBase::GetWorld() const @ 0x00686A70 / PeekWorld() const @ 0x00686A80,')
edit(F, 'src', 'CTCBase::GetDefinitions() @ 0x006869D0,', 'CTCBase::GetDefinitionManager() const @ 0x006869D0,')
edit(F, 'src', 'CTCBase::GetPRandomSeed() @ 0x00686DA0,', 'CTCBase::GetPWorldSeed() const @ 0x00686DA0,')
edit(F, 'src', 'CTCBase::GetPlayerManager() @ 0x00686D50, CPlayerManager::GetMainPlayer() @ 0x00449960,',
     'CTCBase::PeekPlayerManager() const @ 0x00686D50, CPlayerManager::GetMainPlayer() const @ 0x00449960,')
edit(F, 'src', 'struct CDefinitionHolder {\n', 'class CGameDefinitionManager { public:\n')
edit(F, 'src', 'class CTCHeroStats { public:\n',
     'class CTCBase { protected:                           // Ego_r.pdb CTCBase protected accessors\n'
     '    const CGameDefinitionManager& GetDefinitionManager() const;   // 0x006869D0\n'
     '    CWorld& GetWorld() const;                        // 0x00686A70\n'
     '    const CWorld& PeekWorld() const;                 // 0x00686A80\n'
     '    const CPlayerManager& PeekPlayerManager() const; // 0x00686D50\n'
     '    unsigned long* GetPWorldSeed() const;            // 0x00686DA0\n};\n\n'
     'class CTCHeroStats : public CTCBase { public:\n')
edit(F, 'src', '    CWorld* GetWorld();                              // 0x00686A70\n    const CWorld* GetWorld() const;                  // 0x00686A80\n'
     '    CDefinitionHolder* GetDefinitions() const;       // 0x006869D0\n    unsigned long* GetPRandomSeed();                 // 0x00686DA0\n'
     '    CPlayerManager* GetPlayerManager() const;        // 0x00686D50\n'
     '    const CWorld* GetConstWorld() const { return GetWorld(); }\n'
     '    const CHeroStatsDef* GetHeroStatsDef() const { return GetDefinitions()->PHeroStatsDef; }\n',
     '    const CWorld* GetConstWorld() const { return &PeekWorld(); }\n'
     '    const CHeroStatsDef* GetHeroStatsDef() const { return GetDefinitionManager().PHeroStatsDef; }\n')
edit(F, 'src', 'if (GetWorld()->GetGameScriptInterface()->IsInMovieSequence())', 'if (GetWorld().GetGameScriptInterface()->IsInMovieSequence())')
edit(F, 'src', '                                 GetPRandomSeed())) * fps', '                                 GetPWorldSeed())) * fps')
edit(F, 'src', 'CThingPlayerCreature* hero = GetPlayerManager()->GetMainPlayer().GetPControlledCreature();',
     'CThingPlayerCreature* hero = PeekPlayerManager().GetMainPlayer().GetPControlledCreature();')
edit(F, 'src', '// definition reads each call GetDefinitions();', '// definition reads each call GetDefinitionManager();')

# ---- 0057d990 CTCHeroStats::SetHeroKickPositionForCurrentRegion: GetWorld() const 0x00686A70 (CWorld&), PeekWorldMap() const
#      0x00686D20, CWorld slot 12 GetWorldMap (FableWin), CWorldMap::GetRegion(long) 0x004FB490 (Ego_r non-const overload)
F = '0057d990'
edit(F, 'src', '// Callees: CTCBase::GetWorld() @ 0x00686A70 (twice), CTCBase::GetWorldMap() @ 0x00686D20,\n'
     '//          CWorldMap::GetCurrentRegionNumber() @ 0x004FB150,',
     '// Callees: CTCBase::GetWorld() const @ 0x00686A70 (twice), CTCBase::PeekWorldMap() const @ 0x00686D20,\n'
     '//          CWorldMap::GetLoadedRegion() const @ 0x004FB150,')
edit(F, 'src', '    CRegion& GetRegion_004fb490(long index);                  // 0x004FB490\n', '    CRegion& GetRegion(long index);                 // 0x004FB490\n')
edit(F, 'src', '    virtual CWorldMap* GetWorldMap();                // slot 12 (+0x30)\n', '    virtual CWorldMap& GetWorldMap();                // slot 12 (+0x30)\n')
edit(F, 'src', 'class CTCHeroStats { public:\n',
     'class CTCBase { protected:\n    CWorld& GetWorld() const;                        // 0x00686A70 (Ego_r.pdb CTCBase, protected)\n'
     '    const CWorldMap& PeekWorldMap() const;           // 0x00686D20 (Ego_r.pdb CTCBase, protected)\n};\n\n'
     'class CTCHeroStats : public CTCBase { public:\n')
edit(F, 'src', '    CWorld* GetWorld() const;                        // 0x00686A70\n    CWorldMap* GetWorldMap() const;                  // 0x00686D20\n', '')
edit(F, 'src', '    CWorld* world = GetWorld();\n', '    CWorld* world = &GetWorld();\n')
edit(F, 'src', 'OutstandingCrimes[world->GetWorldMap()->GetRegion_004fb490(GetWorldMap()->GetLoadedRegion()).Name];',
     'OutstandingCrimes[world->GetWorldMap().GetRegion(PeekWorldMap().GetLoadedRegion()).Name];')
edit(F, 'src', 'status.LastTimeExited = GetWorld()->GetFrame();', 'status.LastTimeExited = GetWorld().GetFrame();')

# ---- 005fad40 CTCInventoryMap::TeleportHeroToGuildSealRecallLocation: PeekPlayerManager / GetWorld / PeekWorld,
#      CPlayerManager::GetMainPlayer() const 0x00449960 and GetMainPlayerNumber() const 0x00449700 (Ego_r.pdb)
F = '005fad40'
edit(F, 'src', '// Callees: CTCBase::GetPlayerManager() const @ 0x00686D50, CPlayerManager::GetMainPlayer @ 0x00449960,',
     '// Callees: CTCBase::PeekPlayerManager() const @ 0x00686D50, CPlayerManager::GetMainPlayer() const @ 0x00449960,')
edit(F, 'src', 'CTCBase::GetWorld\n//   @ 0x00686A70 and its const twin @ 0x00686A80,', 'CTCBase::GetWorld\n//   @ 0x00686A70 and PeekWorld @ 0x00686A80,')
edit(F, 'src', '//   CPlayerManager::GetMainPlayerID @ 0x00449700', '//   CPlayerManager::GetMainPlayerNumber @ 0x00449700')
edit(F, 'src', '    CPlayer* GetMainPlayer() const;                   // 0x00449960\n    long GetMainPlayerID() const;                     // 0x00449700\n',
     '    const CPlayer& GetMainPlayer() const;             // 0x00449960 (Ego_r.pdb)\n    long GetMainPlayerNumber() const;                 // 0x00449700 (Ego_r.pdb)\n')
edit(F, 'src', '\nclass CTCInventoryMap { public:\n    CPlayerManager* GetPlayerManager() const;         // 0x00686D50 (CTCBase)\n'
     '    CWorld* GetWorld();                               // 0x00686A70 (CTCBase)\n    const CWorld* GetWorld() const;                   // 0x00686A80 (CTCBase)\n'
     '    const CWorld& GetWorldConst() const { return *GetWorld(); }\n',
     '\nclass CTCBase { protected:                            // Ego_r.pdb CTCBase protected accessors\n'
     '    CWorld& GetWorld() const;                         // 0x00686A70\n    const CWorld& PeekWorld() const;                  // 0x00686A80\n'
     '    const CPlayerManager& PeekPlayerManager() const;  // 0x00686D50\n};\n'
     'class CTCInventoryMap : public CTCBase { public:\n    const CWorld& GetWorldConst() const { return PeekWorld(); }\n')
edit(F, 'src', 'const CThing* hero = GetPlayerManager()->GetMainPlayer()->PeekPControlledCreature();', 'const CThing* hero = PeekPlayerManager().GetMainPlayer().PeekPControlledCreature();')
edit(F, 'src', '        GetWorld()->DisplayMiniMapOnRegionLoad = true;', '        GetWorld().DisplayMiniMapOnRegionLoad = true;')
edit(F, 'src', 'PlayerManager->GetMainPlayerID();', 'PlayerManager->GetMainPlayerNumber();')
edit(F, 'src', '        GetWorld()->HandleMoveHeroToRegionGameEvent(&event);', '        GetWorld().HandleMoveHeroToRegionGameEvent(&event);')

TCB_POLY_GW_PW = ('class CTCBase { public:\n    virtual ~CTCBase();                                           // slot 0 (0x00686830)\n'
                  'protected:\n    CWorld& GetWorld() const;                    // 0x00686A70 (Ego_r.pdb CTCBase, protected)\n'
                  '    const CWorld& PeekWorld() const;             // 0x00686A80 (Ego_r.pdb CTCBase, protected)\n};\n')
# ---- 008ecc80 CTCDegradableBase::OnHit: GetWorld() in OnHit, PeekWorld() in the const IsAvailableToHit (0x008EC7E0, this TU)
F = '008ecc80'
edit(F, 'src', '// Callees: CTCBase::GetWorld() @ 0x00686A70 and its const twin @ 0x00686A80', '// Callees: CTCBase::GetWorld() const @ 0x00686A70 and PeekWorld() const @ 0x00686A80')
edit(F, 'src', '// Shape: the frame is read through the NON-const GetWorld (0x00686A70),', '// Shape: the frame is read through GetWorld (0x00686A70),')
edit(F, 'src', 'the\n// const GetWorld (0x00686A80) + frame getter', 'the\n// PeekWorld (0x00686A80) + frame getter')
edit(F, 'src', 'class CTCDegradableBase { public:\n    virtual void v00();\n', TCB_POLY_GW_PW + 'class CTCDegradableBase : public CTCBase { public:\n')
edit(F, 'src', '    CWorld*       GetWorld();                               // 0x00686A70 (CTCBase)\n    const CWorld* GetWorld() const;                         // 0x00686A80 (CTCBase)\n', '')
edit(F, 'src', '    if (GetWorld()->GetFrame() - LastWorldFrameHit > 1)', '    if (PeekWorld().GetFrame() - LastWorldFrameHit > 1)')
edit(F, 'src', '    long frame = GetWorld()->GetFrame();', '    long frame = GetWorld().GetFrame();')

# ---- 006d8580 CTCHeroExperience::FrameUpdate: PeekWorld() (the paused test, twice), GetWorld() (the frame)
F = '006d8580'
edit(F, 'src', '// Callees: CTCBase::GetWorld() const @ 0x00686A80 (twice)', '// Callees: CTCBase::PeekWorld() const @ 0x00686A80 (twice)')
edit(F, 'src', 'CTCBase::GetWorld() @ 0x00686A70 (result unused)', 'CTCBase::GetWorld() const @ 0x00686A70 (result unused)')
edit(F, 'src', 'so the const GetWorld/GetThingManager\n// overloads are called', 'so PeekWorld and the const GetThingManager\n// overload are called')
edit(F, 'src', 'class CTCHeroExperience {\npublic:\n    virtual void v00();\n    void* ParentThing;                                      // +0x04\n'
     '    bool ToBeRemoved;                                       // +0x08\n    bool IsInGlobalUpdateTCs;                               // +0x09\n',
     'class CTCBase { public:\n    virtual ~CTCBase();                                     // slot 0 (0x00686830)\n'
     '    void* ParentThing;                                      // +0x04\n    bool ToBeRemoved;                                       // +0x08\n'
     '    bool IsInGlobalUpdateTCs;                               // +0x09\nprotected:\n'
     '    CWorld& GetWorld() const;                               // 0x00686A70 (Ego_r.pdb CTCBase, protected)\n'
     '    const CWorld& PeekWorld() const;                        // 0x00686A80 (Ego_r.pdb CTCBase, protected)\n};\n'
     'class CTCHeroExperience : public CTCBase {\npublic:\n')
edit(F, 'src', '    CWorld* GetWorld();                                     // 0x00686A70\n    const CWorld* GetWorld() const;                         // 0x00686A80\n', '')
edit_n(F, 'src', '!self->GetWorld()->GetThingManager()->', '!self->PeekWorld().GetThingManager()->', 2)
edit(F, 'src', '    long frame = GetWorld()->GetFrame();', '    long frame = GetWorld().GetFrame();')

# ---- 00706950 CTCHeroAttachableAppearanceModifiers::AddAppearanceModifierWithReplace: GetDefinitionManager() const 0x006869D0,
#      GetWorld() 0x00686A70 (the three message blocks), PeekWorld() 0x00686A80 (the frame stamp)
F = '00706950'
edit(F, 'src', '-- GetDefinitions 0x006869D0,', '-- CTCBase::GetDefinitionManager 0x006869D0,')
edit(F, 'src', '_M_insert_overflow 0x00708E40, GetWorld() const 0x00686A80, CWorld::GetFrame', '_M_insert_overflow 0x00708E40, PeekWorld() 0x00686A80, CWorld::GetFrame')
edit(F, 'src', 'named `CWorld* world = GetWorld();`', 'named `CWorld* world = &GetWorld();`')
edit(F, 'src', '(an inline `GetWorld()->MessageEventManager->AddMessage(...)` pushes first)', '(an inline `GetWorld().MessageEventManager->AddMessage(...)` pushes first)')
edit(F, 'src', 'class CDefinitionHolder : public CDefinitionManager { };\n', 'class CGameDefinitionManager : public CDefinitionManager { };\n')
edit(F, 'src', 'class CTCHeroAttachableAppearanceModifiers {\npublic:\n    virtual void v00();\n',
     'class CTCBase { public:\n    virtual ~CTCBase();                                           // slot 0 (0x00686830)\nprotected:\n'
     '    const CGameDefinitionManager& GetDefinitionManager() const;   // 0x006869D0 (Ego_r.pdb CTCBase, protected)\n'
     '    CWorld& GetWorld() const;                       // 0x00686A70 (Ego_r.pdb CTCBase, protected)\n'
     '    const CWorld& PeekWorld() const;                // 0x00686A80 (Ego_r.pdb CTCBase, protected)\n};\n'
     'class CTCHeroAttachableAppearanceModifiers : public CTCBase {\npublic:\n')
edit(F, 'src', '    const CDefinitionHolder* GetDefinitions() const;   // 0x006869D0 (CTCBase)\n    CWorld* GetWorld() const;                       // 0x00686A80 (CTCBase)\n'
     '    CWorld* GetWorld();                             // 0x00686A70 (CTCBase)\n', '')
edit(F, 'src', 'if (GetDefinitions()->GetDef(defIndex, &thingOut)', 'if (GetDefinitionManager().GetDef(defIndex, &thingOut)')
edit_n(F, 'src', '                CWorld* world = GetWorld();\n', '                CWorld* world = &GetWorld();\n', 3)
edit(F, 'src', 'FrameLastChanged = ((const CTCHeroAttachableAppearanceModifiers*)this)->GetWorld()->GetFrame();', 'FrameLastChanged = PeekWorld().GetFrame();')

# ---- 004bd200 NGraphicAppearance::CTCGraphicAppearance::WasRenderedLastFrame() const: CTCBase::GetWorld is itself const
#      (Ego_r ?GetWorld@CTCBase@@IBEAAVCWorld@@XZ), so the const_casts go
F = '004bd200'
edit(F, 'src', '// Callees: CTCBase::GetWorld @ 0x00686A70 (twice),', '// Callees: CTCBase::GetWorld() const @ 0x00686A70 (twice),')
edit(F, 'src', '\nnamespace NGraphicAppearance {\n\nclass CTCGraphicAppearance {\npublic:\n',
     '\nclass CTCBase { protected:\n    CWorld& GetWorld() const;                       // 0x00686A70 (Ego_r.pdb CTCBase, protected)\n};\n'
     '\nnamespace NGraphicAppearance {\n\nclass CTCGraphicAppearance : public ::CTCBase {\npublic:\n')
edit(F, 'src', '    CWorld* GetWorld();                             // 0x00686A70 (CTCBase)\n', '')
edit(F, 'src', 'if (const_cast<CTCGraphicAppearance*>(this)->GetWorld()->GetGameScriptInterface()->IsInMovieSequence())',
     'if (GetWorld().GetGameScriptInterface()->IsInMovieSequence())')
edit(F, 'src', 'long lastRenderFrame = const_cast<CTCGraphicAppearance*>(this)->GetWorld()->LastRenderFrame;', 'long lastRenderFrame = GetWorld().LastRenderFrame;')

TCB_PW = 'class CTCBase { protected:\n    const CWorld& PeekWorld() const;   // 0x00686A80 (Ego_r.pdb CTCBase, protected)\n};\n'
# ---- 00732730 CTCClock::GetHandAnimationUpdateAmount: PeekWorld().PGameTimeManager->Time (Ego_r CWorld +0x1C scoped_ptr
#      <CGameTimeManager>, CGameTimeManager::Time +0x08)
F = '00732730'
edit(F, 'src', '// Callees: the global world-component accessor @ 0x00686A80 (`mov eax,[g_world] ; mov eax,[eax+24h] ;\n'
     '// ret`, __cdecl; the landed 0x0077DB90 calls it too), whose +0x1c record holds the normalised time of\n// day at +8.',
     '// Callees: CTCBase::PeekWorld() const @ 0x00686A80 (`mov eax,[g_world] ; mov eax,[eax+24h] ;\n'
     '// ret`; the landed 0x0077DB90 calls it too), whose PGameTimeManager (+0x1c) holds the normalised time of\n// day, Time, at +8 (Ego_r.pdb).')
edit(F, 'src', 'struct CTimeOfDay {\n    unsigned char _pad[8];\n    float         Time;                     // +0x08 normalised time of day\n};\n'
     'struct CWorldTimeComponent {\n    unsigned char _pad[0x1c];\n    CTimeOfDay*   TimeOfDay;                // +0x1c\n};\n'
     'CWorldTimeComponent* GetWorldTimeComponent();   // 0x00686A80\n',
     'class CGameTimeManager { public:\n    unsigned char _pad[8];\n    float         Time;                     // +0x08 normalised time of day\n};\n'
     'class CWorld { public:\n    unsigned char     _pad[0x1c];\n    CGameTimeManager* PGameTimeManager;     // +0x1c scoped_ptr<CGameTimeManager>\n};\n'
     + TCB_PW)
edit(F, 'src', 'class CTCClock { public:', 'class CTCClock : public CTCBase { public:')
edit(F, 'src', 'float time = GetWorldTimeComponent()->TimeOfDay->Time;', 'float time = PeekWorld().PGameTimeManager->Time;')

# ---- 00744d00 CTCPerceiveAlert::FindAlertEvent: PeekWorld() (CWorld +0x60 PMessageEventManager, Ego_r.pdb)
F = '00744d00'
edit(F, 'src', '// Callees: the world getter @ 0x00686A80 (`[0x013B8A1C]+24h`, +0x60 = CMessageEventManager),',
     '// Callees: CTCBase::PeekWorld() const @ 0x00686A80 (`[0x013B8A1C]+24h`, +0x60 = PMessageEventManager),')
edit(F, 'src', 'struct CWorldEvents {\n', 'class CWorld { public:\n')
edit(F, 'src', '    CMessageEventManager* GetPMessageEventManager() { return PMessageEventManager; }\n};\n',
     '    CMessageEventManager* GetPMessageEventManager() const { return PMessageEventManager; }\n};\n' + TCB_PW)
edit(F, 'src', 'struct CTCPerceiveAlert {\n', 'struct CTCPerceiveAlert : public CTCBase {\n')
edit(F, 'src', '    CWorldEvents* GetPWorld();                  // 0x00686A80\n', '')
edit(F, 'src', 'CMessageEvent* ev = GetPWorld()->GetPMessageEventManager()->FindMostRecentMessage(', 'CMessageEvent* ev = PeekWorld().GetPMessageEventManager()->FindMostRecentMessage(')

# ---- 0074e560 CTCContainerRewardHero::CreateNewObject: CThingObject::Create 0x00703210 (Ego_r static __fastcall),
#      PeekWorld().GetPRandomSeed() const (0x0049D810, bound QBE)
F = '0074e560'
edit(F, 'src', '// Callees: the thing factory Create(long, const C3DVector*, long, CRightHandedSet*, ulong, const CCharString*) 0x00703210\n// (`__fastcall`),',
     '// Callees: CThingObject::Create(long, const C3DVector&, long, const CRightHandedSet*, ulong, const CCharString&) 0x00703210\n// (static `__fastcall`, Ego_r.pdb),')
edit(F, 'src', 'the TC\'s game accessor 0x00686A80 and its\n// random-seed accessor 0x0049D810,', 'CTCBase::PeekWorld() const 0x00686A80 and the world\'s\n// random-seed accessor 0x0049D810,')
edit(F, 'src', 'CThing* __fastcall CreateThing(long def, const C3DVector* position, long kind, CRightHandedSet* orientation,\n'
     '                               unsigned long flags, const CCharString* scriptName);   // 0x00703210\n',
     'class CThingObject : public CThing { public:\n'
     '    static CThingObject* __fastcall Create(long def, const C3DVector& position, long kind, const CRightHandedSet* orientation,\n'
     '                                           unsigned long flags, const CCharString& scriptName);   // 0x00703210 (Ego_r.pdb)\n};\n')
edit(F, 'src', 'struct CGameComponents {\n    unsigned long* GetRandomSeed();                     // 0x0049D810\n};\n',
     'class CWorld { public:\n    unsigned long* GetPRandomSeed() const;              // 0x0049D810\n};\n' + TCB_PW)
edit(F, 'src', 'class CTCContainerRewardHero {\npublic:\n', 'class CTCContainerRewardHero : public CTCBase {\npublic:\n')
edit(F, 'src', '    CGameComponents* GetGame();                         // 0x00686A80\n', '')
edit(F, 'src', 'CThing* thing = CreateThing(def, &position, 5, 0, 0, &scriptName);', 'CThing* thing = CThingObject::Create(def, position, 5, 0, 0, scriptName);')
edit(F, 'src', 'float spin = GFFloatRandom(1.0f, GetGame()->GetRandomSeed());', 'float spin = GFFloatRandom(1.0f, PeekWorld().GetPRandomSeed());')

# ---- 006d0180 CTCWife::Divorce: CTCBase::GetPlayerManager() 0x00686D40 (the opinion TC's GetWorld stays: it is called
#      on another TC, so retail reaches it through an inline public helper of CTCCreatureOpinionOfHero not yet identified)
F = '006d0180'
edit(F, 'src', 'CTCBase world/player-manager\n//   getters 0x00686A70 / 0x00686D40;', 'CTCBase::GetWorld 0x00686A70 (on the opinion\n//   TC) / CTCBase::GetPlayerManager 0x00686D40;')
edit(F, 'src', 'class CTCWife { public:\n    virtual void v00();\n',
     'class CTCBase { public:\n    virtual ~CTCBase();                         // slot 0 (0x00686830)\nprotected:\n'
     '    CPlayerManager& GetPlayerManager();         // 0x00686D40 (Ego_r.pdb CTCBase, protected)\n};\n'
     'class CTCWife : public CTCBase { public:\n')
edit(F, 'src', '    CPlayerManager* GetPlayerManager();         // 0x00686D40\n', '')
edit(F, 'src', 'CThing* hero = GetPlayerManager()->GetMainPlayer().GetPControlledCreature();', 'CThing* hero = GetPlayerManager().GetMainPlayer().GetPControlledCreature();')

import re as _re
from he7 import _rw
def resub(F, rx, repl, k='src', n=1):
    """regex replacement, exactly n matches unless already applied (repl text present and no match)"""
    def f(t):
        c = len(_re.findall(rx, t))
        if c == 0: return None
        if c != n: raise SystemExit('RESUB %d!=%d in %s for %r' % (c, n, F, rx))
        return _re.sub(rx, repl, t)
    _rw(F, k, f)

TCB_GPM = 'class CTCBase { protected:\n    CPlayerManager& GetPlayerManager();   // 0x00686D40 (Ego_r.pdb CTCBase, protected)\n};\n'
# ---- free-function stand-ins for CTCBase::GetPlayerManager 0x00686D40 (ecx already holds `this` at the call in retail)
for F, cls in (('0072e940', 'class CTCActionUseBuyHouse {\npublic:\n'), ('007828e0', 'struct CTCBoastingArea {\n'),
               ('00782a90', 'struct CTCBoastingArea {\n'), ('007aa480', 'class CTCAssassinRush { public:\n')):
    resub(F, r'CPlayerManager\* GetPlayerManager\(\);[ \t]*// 0x00686D40\n', TCB_GPM.replace('\\', '\\\\'))
    edit(F, 'src', cls, cls.replace(' {', ' : public CTCBase {', 1))
    edit(F, 'src', 'GetPlayerManager()->GetMainPlayer()', 'GetPlayerManager().GetMainPlayer()')
    resub(F, r'the player-manager accessor @ 0x00686D40', 'CTCBase::GetPlayerManager @ 0x00686D40')
    resub(F, r'\(__cdecl, no arguments: the game singleton', '(it ignores `this`: the game singleton')

# ---- 00785b40 CTCVolumeContainmentTracker::FrameUpdate: GetWorld() const 0x00686A70; the "world view" is CWorld itself
F = '00785b40'
edit(F, 'src', '// Callees: CTCBase::GetWorld() 0x00686A70;', '// Callees: CTCBase::GetWorld() const 0x00686A70;')
edit(F, 'src', 'class CWorld { public: static long __fastcall GetFrame(); };            // 0x0049D870\n\nstruct CWorldView {\n'
     '    virtual void v00(); virtual void v01(); virtual void v02(); virtual void v03(); virtual void v04();\n'
     '    virtual void v05(); virtual void v06(); virtual void v07();\n'
     '    virtual C3DVector GetCameraPosition();      // slot 8 (+0x20)\n};\n',
     'class CWorld { public:\n'
     '    virtual void v00(); virtual void v01(); virtual void v02(); virtual void v03(); virtual void v04();\n'
     '    virtual void v05(); virtual void v06(); virtual void v07();\n'
     '    virtual C3DVector GetCameraPosition();      // slot 8 (+0x20) (name from use)\n'
     '    static long __fastcall GetFrame();          // 0x0049D870\n};\n')
edit(F, 'src', 'class CTCVolumeContainmentTracker { public:\n    virtual void v00();\n', TCB_POLY_GW_PW + 'class CTCVolumeContainmentTracker : public CTCBase { public:\n')
edit(F, 'src', '    CWorldView* GetWorld();                     // 0x00686A70 (CTCBase)\n', '')
edit(F, 'src', '        CWorldView* world = GetWorld();\n', '        CWorld* world = &GetWorld();\n')

# ---- 007fbe50 CTCKraken::UpdateBeamExplosion: GetDefinitionManager() const 0x006869D0 (Ego_r; GetDefGlobalIndexFromName is const,
#      Ego_r ?GetDefGlobalIndexFromName@CDefinitionManager@@QBEJABVCCharString@@@Z), GetWorld() const 0x00686A70 (the
#      "display engine" +0x34 is CWorld::PCurrentGameCamera, Ego_r.pdb)
F = '007fbe50'
edit(F, 'src', '//   CTCBase GetDisplayEngine 0x00686A70 (engine+0x34 slot 10 hands back the camera manager),',
     '//   CTCBase::GetWorld 0x00686A70 (the world\'s current game camera +0x34, slot 10, hands back the camera manager),')
edit(F, 'src', 'struct CCameraSource {\n', 'class CAIGameCameraBase { public:\n')
edit(F, 'src', 'class CDisplayEngine { public:\n    unsigned char _pad00[0x34];\n    CCameraSource* PCameraSource;                     // +0x34\n'
     '    CCameraSource* GetCameraSource() const { return PCameraSource; }\n};\n',
     'class CWorld { public:\n    unsigned char      _pad00[0x34];\n    CAIGameCameraBase* PCurrentGameCamera;            // +0x34 (Ego_r.pdb)\n'
     '    CAIGameCameraBase* GetGameCamera() { return PCurrentGameCamera; }   // inline here (Ego_r ?GetGameCamera@CWorld@@QAEPAVCAIGameCameraBase@@XZ)\n'
     '    static long __fastcall GetFrame();                // 0x0049D870\n};\n')
edit(F, 'src', '    long GetDefGlobalIndexFromName(const CCharString& name);   // 0x009AD410\n};\n',
     '    long GetDefGlobalIndexFromName(const CCharString& name) const;   // 0x009AD410 (Ego_r.pdb)\n};\n'
     'class CGameDefinitionManager : public CDefinitionManager { };\n')
edit(F, 'src', 'class CWorld { public: static long __fastcall GetFrame(); };                             // 0x0049D870\n', '')
edit(F, 'src', 'class CTCKraken { public:\n',
     'class CTCBase { protected:                            // Ego_r.pdb CTCBase protected accessors\n'
     '    const CGameDefinitionManager& GetDefinitionManager() const;   // 0x006869D0\n'
     '    CWorld& GetWorld() const;                         // 0x00686A70\n};\n'
     'class CTCKraken : public CTCBase { public:\n')
edit(F, 'src', '    CDefinitionManager* GetDefinitionManager();       // 0x006869D0\n    CDisplayEngine* GetDisplayEngine();               // 0x00686A70\n', '')
edit(F, 'src', 'CreateThing(GetDefinitionManager()->GetDefGlobalIndexFromName("KRAKEN_BEAM_EXPLOSION"),', 'CreateThing(GetDefinitionManager().GetDefGlobalIndexFromName("KRAKEN_BEAM_EXPLOSION"),')
edit(F, 'src', '        GetDisplayEngine()->GetCameraSource()->GetCameraManager(&cameras);', '        GetWorld().GetGameCamera()->GetCameraManager(&cameras);')

# ---- 006ca400 CTCHero::SetTitle: GetDefinitionManager() const 0x006869D0, GetWorld() const 0x00686A70
F = '006ca400'
edit(F, 'src', '// Callees: CTCBase::GetDefinitions() const @ 0x006869D0,', '// Callees: CTCBase::GetDefinitionManager() const @ 0x006869D0,')
edit(F, 'src', 'struct CDefinitionHolder : CDefinitionManager { };\n', 'class CGameDefinitionManager : public CDefinitionManager { };\n')
edit(F, 'src', 'class CTCHero {\npublic:\n    void* __vfptr;',
     'class CTCBase { protected:                         // Ego_r.pdb CTCBase protected accessors\n'
     '    const CGameDefinitionManager& GetDefinitionManager() const;   // 0x006869D0\n'
     '    CWorld& GetWorld() const;                      // 0x00686A70\n};\n\n'
     'class CTCHero : public CTCBase {\npublic:\n    void* __vfptr;')
edit(F, 'src', '    CDefinitionHolder* GetDefinitions() const;      // 0x006869D0 (CTCBase)\n    CWorld* GetWorld();                             // 0x00686A70 (CTCBase)\n', '')
edit(F, 'src', 'CDefPointer<const CDefClassBase> def = GetDefinitions()->GetPDefFromGlobalIndex(titleObjectDef);',
     'CDefPointer<const CDefClassBase> def = GetDefinitionManager().GetPDefFromGlobalIndex(titleObjectDef);')
edit(F, 'src', '                CWorld* world = GetWorld();\n', '                CWorld* world = &GetWorld();\n')

# ---- 00724010 CTCPhysicsStandard::OnCreate: PeekWorld() const 0x00686A80 (slot 0 is the CTCBase destructor)
F = '00724010'
edit(F, 'src', '// Callees: CTCBase::GetWorld 0x00686A80,', '// Callees: CTCBase::PeekWorld 0x00686A80,')
edit(F, 'src', 'struct CTCPhysicsStandard {\n    V4(0)                                           // 0-3\n',
     TCB_POLY_PW + 'struct CTCPhysicsStandard : public CTCBase {\n    virtual void v0b(); virtual void v0c(); virtual void v0d();   // 1-3 (slot 0: ~CTCBase)\n')
edit(F, 'src', '    const CWorld& GetWorld() const;                 // 0x00686A80 (CTCBase)\n', '')
edit(F, 'src', '        const CWorld& world = GetWorld();\n', '        const CWorld& world = PeekWorld();\n')

# ---- 00590d32 CTCInventory::CTCInventory: PeekPlayerManager() const 0x00686D50 through NInventory::CTCInventoryBase's CTCBase
F = '00590d32'
edit(F, 'src', 'CTCBase 0x00686D50 -> 0x00449960 ->', 'CTCBase::PeekPlayerManager 0x00686D50 -> CPlayerManager::GetMainPlayer() const 0x00449960 ->')
edit(F, 'src', 'namespace NUISystem { class CComponent; }\n\nnamespace NInventory {\nclass CTCInventoryBase {',
     'namespace NUISystem { class CComponent; }\nclass CPlayerManager;\n' + TCB_POLY_PPM +
     '\nnamespace NInventory {\nclass CTCInventoryBase : public ::CTCBase {')
edit(F, 'src', '    CPlayer* GetMainPlayer() const;                                 // 0x00449960\n',
     '    const CPlayer& GetMainPlayer() const;                           // 0x00449960 (Ego_r.pdb)\n')
edit(F, 'src', '    CPlayerManager* GetPlayerManager() const;                 // 0x00686D50 (CTCBase)\n', '')
edit_n(F, 'src', 'GetPlayerManager()->GetMainPlayer()->PProfile', 'PeekPlayerManager().GetMainPlayer().PProfile', 2)

# ---- 007ba550 CTCTavernBar::OnInitialActivate: GetWorld() const 0x00686A70 (CWorld +0x10 DefinitionManager, Ego_r.pdb),
#      inline CWorld::PeekDefinitionManager() const (FableWin.pdb ?PeekDefinitionManager@CWorld@@QBEABVCGameDefinitionManager@@XZ),
#      PeekPlayerManager() const 0x00686D50 -> CPlayerManager::GetMainPlayer() const 0x00449960 -> CPlayer +0x28 Number
#      (Ego_r.pdb; FableWin.pdb ?GetNumber@CPlayer@@QBEJXZ, inline in retail) as CThingObject::Create's player number
F = '007ba550'
edit(F, 'src', '//   CCharString @ 0x0099EBF0 / ~ @ 0x0099EAE0, CTCBase::GetWorld @ 0x00686A70 / 0x00686D50, CDefinitionManager::\n'
     '//   GetDefGlobalIndexFromName @ 0x009AD410, the world\'s level getter @ 0x00449960, CThingObject::Create @ 0x00703210,',
     '//   CCharString @ 0x0099EBF0 / ~ @ 0x0099EAE0, CTCBase::GetWorld @ 0x00686A70, CTCBase::PeekPlayerManager @ 0x00686D50,\n'
     '//   CDefinitionManager::GetDefGlobalIndexFromName @ 0x009AD410, CPlayerManager::GetMainPlayer() const @ 0x00449960,\n'
     '//   CThingObject::Create (static __fastcall, Ego_r.pdb) @ 0x00703210,')
edit(F, 'src', 'class CThingObject : public CThing { public:\n    template <class T>\n',
     'class CThingObject : public CThing { public:\n'
     '    static CThingObject* __fastcall Create(long defIndex, const C3DVector& pos, long playerNumber, const CRightHandedSet* rh,\n'
     '                                           unsigned long flags, const CCharString& name);   // 0x00703210 (Ego_r.pdb)\n'
     '    template <class T>\n')
edit(F, 'src', 'class CDefinitionManager { public: long GetDefGlobalIndexFromName(const CCharString& name); };   // 0x009AD410\n'
     'struct CWorldA {\n    unsigned char _pad[0x10];\n    CDefinitionManager* DefinitionManager;\n'
     '    CDefinitionManager* GetDefinitionManager() const { return DefinitionManager; }\n};\n'
     'struct CLevel { unsigned char _pad[0x28]; CMap* Map; };\n'
     'struct CWorldB { CLevel* GetLevel(); };                 // 0x00449960\n'
     'CThingObject* __fastcall CreateThingObject(long defIndex, const C3DVector& pos, CMap* map, const CRightHandedSet* rh,\n'
     '                                           unsigned long flags, const CCharString& name);   // 0x00703210\n',
     'class CDefinitionManager { public: long GetDefGlobalIndexFromName(const CCharString& name) const; };   // 0x009AD410 (Ego_r.pdb)\n'
     'class CGameDefinitionManager : public CDefinitionManager { };\n'
     'class CWorld { public:\n    unsigned char _pad[0x10];\n    CGameDefinitionManager* DefinitionManager;   // +0x10 (Ego_r.pdb)\n'
     '    const CGameDefinitionManager& PeekDefinitionManager() const { return *DefinitionManager; }   // inline here (FableWin.pdb)\n};\n'
     'class CPlayer { public:\n    unsigned char _pad[0x28];\n    long Number;                                // +0x28 (Ego_r.pdb)\n'
     '    long GetNumber() const { return Number; }   // inline here (FableWin.pdb ?GetNumber@CPlayer@@QBEJXZ)\n};\n'
     'class CPlayerManager { public:\n    const CPlayer& GetMainPlayer() const;       // 0x00449960 (Ego_r.pdb)\n};\n'
     'class CTCBase { protected:                    // Ego_r.pdb CTCBase protected accessors\n'
     '    CWorld& GetWorld() const;                   // 0x00686A70\n'
     '    const CPlayerManager& PeekPlayerManager() const;   // 0x00686D50\n};\n')
edit(F, 'src', 'class CTCTavernBar { public:\n', 'class CTCTavernBar : public CTCBase { public:\n')
edit(F, 'src', '    CWorldA* GetWorld();                                // 0x00686A70\n    CWorldB* GetWorldB();                               // 0x00686D50\n', '')
edit(F, 'src', 'GetWorld()->GetDefinitionManager()->GetDefGlobalIndexFromName("OBJECT_VILLAGE_TAVERN_JUG");',
     'GetWorld().PeekDefinitionManager().GetDefGlobalIndexFromName("OBJECT_VILLAGE_TAVERN_JUG");')
edit(F, 'src', '        CMap* map = GetWorldB()->GetLevel()->Map;\n        CThingObject* jug = CreateThingObject(defIndex, pos, map, 0, 0, "");',
     '        long player = PeekPlayerManager().GetMainPlayer().GetNumber();\n        CThingObject* jug = CThingObject::Create(defIndex, pos, player, 0, 0, "");')
