# wave 6 RELOC_CONFLICT settlements for the const/non-const getter pairs (exact replacements; HE_OV=<overlay>).
# Names (Ego_r.pdb CTCBase / CThing / CWorldMap / CPlayerManager member lists, retail bodies, 2026-10-26 one-address pass):
#   0x00686D40 ?GetPlayerManager@CTCBase@@IAEAAVCPlayerManager@@XZ   0x00686D50 ?PeekPlayerManager@CTCBase@@IBEABVCPlayerManager@@XZ
#   0x00686D20 ?PeekWorldMap@CTCBase@@IBEABVCWorldMap@@XZ
#   0x004C7A10 ?PeekPlayer@CThing@@QBEABVCPlayer@@XZ (calls the const CPlayerManager::GetPlayer 0x00449910)
#   0x004C7A30 ?GetPlayer@CThing@@QAEAAVCPlayer@@XZ  (calls the non-const CPlayerManager::GetPlayer 0x004498C0)
#   0x004FB490 ?GetRegion@CWorldMap@@QAEAAVCRegion@@J@Z (non-const; reached through GetWorldMap)
#   0x004FC180 ?GetRegion@CWorldMap@@QBEABVCRegion@@J@Z (const; reached through PeekWorldMap, CWorld slot 13 +0x34)
#   0x00449960 ?GetMainPlayer@CPlayerManager@@QBEABVCPlayer@@XZ (calls the const GetPlayer 0x00449910)
#   0x00487DC0 ?GetPControlledCreature@CPlayer@@QBEPAVCThingPlayerCreature@@XZ (chunk 561)
#   0x004FC210 ?GetRegionNumberFromName@CWorldMap@@QBEJABVCCharString@@@Z, 0x00829D90
#   ?InitialiseMiniMapFileLoading_Region@CMiniMapDisplay@@QAEXABVCRegion@@ABVCCharString@@_N@Z,
#   0x005FBEA0 ?GetRegionTeleporter@CTCInventoryMap@@ABEPAVCThing@@ABVCRegion@@@Z (FableWin.pdb, private const)
import os, sys
sys.path.insert(0, os.path.dirname(os.path.abspath(__file__)))
from he import edit, path

def edit_all(a, k, old, new, n):
    p, rel = path(a, k)
    t = open(p, encoding='utf-8', errors='surrogateescape', newline='').read()
    c = t.count(old)
    if c != n:
        if c == 0 and new in t: print('already', a, k); return
        raise SystemExit('COUNT %d!=%d in %s for %r' % (c, n, rel, old[:80]))
    open(p, 'w', encoding='utf-8', errors='surrogateescape', newline='').write(t.replace(old, new)); print('ok', a, k)

# --- 007fbc90 CTCKraken::GetCentreMarker: 0x00686D40 is the protected GetPlayerManager returning CPlayerManager&
edit('007fbc90', 'src', '    CPlayerManager*     GetPlayerManager();             // 0x00686D40\n',
     'protected:\n    CPlayerManager&     GetPlayerManager();             // 0x00686D40 (Ego_r.pdb: protected, CPlayerManager&)\npublic:\n')
edit('007fbc90', 'src', 'GetPlayerManager()->GetMainPlayer()', 'GetPlayerManager().GetMainPlayer()')

# --- 007b0950 CTCFishing::FinishFishing: the const chain PeekPlayerManager -> GetMainPlayer const -> GetPControlledCreature
edit('007b0950', 'src', '// Callees: CTCBase::GetPlayerManager @ 0x00686D50, CPlayerManager::GetMainPlayer @ 0x00449960, CMainPlayer::GetCharacterThing\n'
     '//          @ 0x00487DC0 (labels on these three are folded),',
     '// Callees: CTCBase::PeekPlayerManager @ 0x00686D50, CPlayerManager::GetMainPlayer const @ 0x00449960,\n'
     '//          CPlayer::GetPControlledCreature @ 0x00487DC0 (Ego_r.pdb names; the const chain),')
edit('007b0950', 'src', 'class CPlayer {\npublic:\n    void RemoveMode(long mode);                         // 0x00634190\n};',
     'class CThingPlayerCreature;\nclass CPlayer {\npublic:\n    void RemoveMode(long mode);                         // 0x00634190\n'
     '    CThingPlayerCreature* GetPControlledCreature() const;   // 0x00487DC0\n};')
edit('007b0950', 'src', 'class CMessageManager {\n', 'class CThingPlayerCreature : public CThing { };\nclass CMessageManager {\n')
edit('007b0950', 'src', 'class CPlayerManager {\npublic:\n    class CMainPlayer* GetMainPlayer();                 // 0x00449960\n};\n'
     'class CMainPlayer {\npublic:\n    CThing* GetCharacterThing();                        // 0x00487DC0\n};\n',
     'class CPlayerManager {\npublic:\n    const CPlayer& GetMainPlayer() const;               // 0x00449960\n};\n')
edit('007b0950', 'src', '    CPlayerManager* GetPlayerManager();                 // 0x00686D50\n',
     'protected:\n    const CPlayerManager& PeekPlayerManager() const;    // 0x00686D50\npublic:\n')
edit('007b0950', 'src', 'CThing* hero = GetPlayerManager()->GetMainPlayer()->GetCharacterThing();',
     'CThing* hero = PeekPlayerManager().GetMainPlayer().GetPControlledCreature();')

# --- 0068eb20 NInputProcess::IsThingToKeepStrafing: 0x004C7A10 is CThing::PeekPlayer
edit('0068eb20', 'src', 'CThing::GetPlayer const 0x004C7A10 (twice)', 'CThing::PeekPlayer 0x004C7A10 (twice)')
edit('0068eb20', 'src', 'before the GetPlayer()->GetJoystickDeviceNumber() argument chain.', 'before the PeekPlayer().GetJoystickDeviceNumber() argument chain.')
edit('0068eb20', 'src', '    CPlayer* GetPlayer() const;                     // 0x004C7A10\n', '    const CPlayer& PeekPlayer() const;              // 0x004C7A10\n')
edit('0068eb20', 'src', 'creature.GetPlayer()->GetJoystickDeviceNumber()', 'creature.PeekPlayer().GetJoystickDeviceNumber()')
edit('0068eb20', 'src', '    CPlayer* player = creature.GetPlayer();\n', '    const CPlayer* player = &creature.PeekPlayer();\n')

# --- 007e0750 CTCHeroSpecialMovement::FrameUpdate: PeekPlayer through ParentThing, GetPlayer through the thing reference
edit('007e0750', 'src', '// Callees: CThing::GetPlayer @ 0x004C7A10 and its const twin @ 0x004C7A30,',
     '// Callees: CThing::PeekPlayer (const) @ 0x004C7A10 and CThing::GetPlayer @ 0x004C7A30,')
edit('007e0750', 'src', 'the second and third player reads go through the const\n//   GetPlayer on a const thing reference.',
     'the first player read is PeekPlayer through ParentThing, the second and third go through the\n//   non-const GetPlayer on the thing reference.')
edit('007e0750', 'src', 'class CThing { public:\n    CPlayer* GetPlayer();                               // 0x004C7A10\n'
     '    const CPlayer* GetPlayer() const;                   // 0x004C7A30\n};',
     'class CThing { public:\n    const CPlayer& PeekPlayer() const;                  // 0x004C7A10\n'
     '    CPlayer& GetPlayer();                               // 0x004C7A30\n};')
edit('007e0750', 'src', '    const CThing& GetThing() const { return *ParentThing; }\n', '    CThing& GetThing() const { return *ParentThing; }\n')
edit('007e0750', 'src', 'ParentThing->GetPlayer()->m_pInterface', 'ParentThing->PeekPlayer().m_pInterface')
edit_all('007e0750', 'src', 'GetThing().GetPlayer()->', 'GetThing().GetPlayer().', 3)

# --- GetRegion callers that reach 0x004FC180 (the const twin)
CONSTREG = '    const CRegion& GetRegion(long index) const;    // 0x004FC180\n'
edit('004189c2', 'src', '    CRegion& GetRegion(long index);                // 0x004FC180\n', CONSTREG)
for k in ('src', 'tst'):
    edit('00449f90', k, '    CRegion& GetRegion(long region);                    // 0x004FC180\n',
         '    const CRegion& GetRegion(long region) const;        // 0x004FC180\n')
edit('00449f90', 'tst', 'CRegion& CWorldMap::GetRegion(long r) {', 'const CRegion& CWorldMap::GetRegion(long r) const {')
# 005fc0c0: RegionCorrespondances is list<pair<const CRegion*, const CRegion*>> (landed 0x00617230 push_back)
edit('005fc0c0', 'src', 'region-correspondence list<pair<CRegion*, CRegion*>> +0x24C.', 'region-correspondence list<pair<const CRegion*, const CRegion*>> +0x24C.')
edit('005fc0c0', 'src', 'struct CRegionPair {                                    // pair<CRegion*, CRegion*>\n'
     '    CRegion* first;\n    CRegion* second;\n    CRegionPair(CRegion* a, CRegion* b) : first(a), second(b) {}\n',
     'struct CRegionPair {                                    // pair<const CRegion*, const CRegion*>\n'
     '    const CRegion* first;\n    const CRegion* second;\n    CRegionPair(const CRegion* a, const CRegion* b) : first(a), second(b) {}\n')
edit('005fc0c0', 'src', '    CRegion& GetRegion(long map);                       // 0x004FC180\n',
     '    const CRegion& GetRegion(long map) const;           // 0x004FC180\n')
edit('005fc0c0', 'src', '        CRegion* a = &GetWorld().GetWorldMap()->GetRegion(from->MapNumber);\n'
     '        CRegion* b = &GetWorld().GetWorldMap()->GetRegion(to->MapNumber);\n',
     '        const CRegion* a = &GetWorld().GetWorldMap()->GetRegion(from->MapNumber);\n'
     '        const CRegion* b = &GetWorld().GetWorldMap()->GetRegion(to->MapNumber);\n')
# 005fc840: PeekPlayerManager / PeekWorldMap (CTCBase), the const GetMainPlayer, the private const GetRegionTeleporter
edit('005fc840', 'src', '// Callees: player-manager accessor @ 0x00686D50 (`[g_system]+0x30`), main-player accessor @ 0x00449960, the player\'s\n'
     '//          character thing @ 0x00487DD0 (CIntelligentPointer::GetPItem on +0x2C), world-map accessor @ 0x00686D20\n'
     '//          (`[g_system]+0x28`), CWorldMap::GetRegion(long) @ 0x004FC180 (returns CRegion&),\n'
     '//          CTCInventoryMap::GetRegionTeleporter(CRegion*) @ 0x005FBEA0,',
     '// Callees: CTCBase::PeekPlayerManager @ 0x00686D50 (`[g_system]+0x30`), CPlayerManager::GetMainPlayer const @ 0x00449960,\n'
     '//          the player\'s character thing @ 0x00487DD0 (CIntelligentPointer::GetPItem on +0x2C), CTCBase::PeekWorldMap\n'
     '//          @ 0x00686D20 (`[g_system]+0x28`), CWorldMap::GetRegion(long) const @ 0x004FC180 (returns const CRegion&),\n'
     '//          CTCInventoryMap::GetRegionTeleporter(const CRegion&) const @ 0x005FBEA0 (private),')
edit('005fc840', 'src', 'class CPlayerManager { public:\n    CPlayer* GetMainPlayer() const;                 // 0x00449960\n};',
     'class CPlayerManager { public:\n    const CPlayer& GetMainPlayer() const;           // 0x00449960\n};')
edit('005fc840', 'src', '    CRegion& GetRegion(long index);           // 0x004FC180\n', '    const CRegion& GetRegion(long index) const;   // 0x004FC180\n')
edit('005fc840', 'src', 'class CTCInventoryMap { public:\n    CPlayerManager* GetPlayerManager();       // 0x00686D50\n'
     '    CWorldMap* GetWorldMap();                 // 0x00686D20\n    CThing* GetRegionTeleporter(CRegion* region);       // 0x005FBEA0\n',
     'class CTCBase { protected:\n    const CWorldMap& PeekWorldMap() const;          // 0x00686D20\n'
     '    const CPlayerManager& PeekPlayerManager() const;   // 0x00686D50\n};\n'
     'class CTCInventoryMap : public CTCBase {\n    CThing* GetRegionTeleporter(const CRegion& region) const;   // 0x005FBEA0\npublic:\n')
edit('005fc840', 'src', 'const CThing* hero = GetPlayerManager()->GetMainPlayer()->PeekPControlledCreature();',
     'const CThing* hero = PeekPlayerManager().GetMainPlayer().PeekPControlledCreature();')
edit('005fc840', 'src', 'GetRegionTeleporter(&GetWorldMap()->GetRegion(regionIndex))', 'GetRegionTeleporter(PeekWorldMap().GetRegion(regionIndex))')
# 00602f30: the const region goes to InitialiseMiniMapFileLoading_Region(const CRegion&, const CCharString&, bool)
edit('00602f30', 'src', 'CMiniMapDisplay::InitialiseMiniMapFileLoading_Region(CRegion*, CCharString*, bool) @ 0x00829D90',
     'CMiniMapDisplay::InitialiseMiniMapFileLoading_Region(const CRegion&, const CCharString&, bool) @ 0x00829D90')
edit('00602f30', 'src', '    CRegion& GetRegion(long index);                // 0x004FC180\n', CONSTREG)
edit('00602f30', 'src', '    void InitialiseMiniMapFileLoading_Region(CRegion* region, CCharString* worldName, bool b);   // 0x00829D90\n',
     '    void InitialiseMiniMapFileLoading_Region(const CRegion& region, const CCharString& worldName, bool b);   // 0x00829D90\n')
edit('00602f30', 'src', '            CRegion& region = GetWorld()->PeekWorldMap()->GetRegion(ActiveRegionIndex);\n',
     '            const CRegion& region = GetWorld()->PeekWorldMap()->GetRegion(ActiveRegionIndex);\n')
edit('00602f30', 'src', 'InitialiseMiniMapFileLoading_Region(&region, &world->CurrentWorldName, false);',
     'InitialiseMiniMapFileLoading_Region(region, world->CurrentWorldName, false);')
# 006c17b0: the region goes to CTCHeroStats::Add/RemoveHousePosition(const CRegion&, ...)
edit('006c17b0', 'src', '    CRegion& GetRegion(long region);                // 0x004FC180\n', '    const CRegion& GetRegion(long region) const;    // 0x004FC180\n')
# 00891820: GetRegionNumberFromName and GetRegion are const members (source and the linking test)
for k in ('src', 'tst'):
    edit('00891820', k, '    long GetRegionNumberFromName(\n        const CCharString& regionName);\n    CRegion& GetRegion(long regionNumber);\n',
         '    long GetRegionNumberFromName(\n        const CCharString& regionName) const;   // 0x004FC210\n'
         '    const CRegion& GetRegion(long regionNumber) const;   // 0x004FC180\n')
edit('00891820', 'src', '        CRegion& region =\n', '        const CRegion& region =\n')
edit('00891820', 'tst', 'long CWorldMap::GetRegionNumberFromName(\n    const CCharString& regionName)\n{',
     'long CWorldMap::GetRegionNumberFromName(\n    const CCharString& regionName) const\n{')
edit('00891820', 'tst', 'CRegion& CWorldMap::GetRegion(long regionNumber)\n{', 'const CRegion& CWorldMap::GetRegion(long regionNumber) const\n{')
edit('00891820', 'tst', 'static CWorldMap* g_ObservedLookupWorldMap;', 'static const CWorldMap* g_ObservedLookupWorldMap;')
edit('00891820', 'tst', 'static CWorldMap* g_ObservedGetRegionWorldMap;', 'static const CWorldMap* g_ObservedGetRegionWorldMap;')
