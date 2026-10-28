# Retry chunk 538 (agent538, 2026-10-28): 5 of 8 parked rows land (RELOCATION_MATCH), 3 re-parked

| Address | Function | What fixed it |
|---|---|---|
| 0x004A6550 | CWorld::PostInit | PDB-exact callee signatures (NProfileTimer::StartProfile, NProgressDisplay::DisplayProgress, CDisplayEngine::Get3DEngine returning CIEngine&, reference ctors) and one plain `g_pQuestManager.Reset(new CQuestManager(...))` instead of spelled-out operator new + placement new + null check |
| 0x005064C0 | CWorldMap::PostRegionLoad | same-TU GetAllThings body; CThing's real flag bitfields at +0x91 |
| 0x005066E0 | CWorldMap::CWorldMap | same-TU `vector<unsigned short>::resize` body |
| 0x00521240 | CThingManager::LoadEntitiesBinary | same-TU `_M_find` and `_M_insert_overflow` bodies, inline by-value GetLoadAllSectionName(), statement order |
| 0x005424B0 | NUISystem::CIconText::~CIconText | (oracle label CText::~CText was wrong) |

Re-parked (pending/2026-10-27_linux_hand538/differ): 0x004A34F0 CWorld::AutoLoadRegionLeaving (564v564, two stack slots
swapped, lane gap); 0x00502620 CWorldMap::LoadLevel (561v559, register rotation); 0x00542780 NUISystem::CIconText::AddIcon
(682v682, x87 load scheduling).

NAME_FIXES.tsv: 59 rows, deferred to the naming pass. Notable: `CCharString::operator=` returns `const CCharString&`
(`ABV0`; ~900 landed files declare a non-const return, CWideString ~150); PDB classes must be `class`, not `struct`;
0x009D8240 NProfileTimer::StartProfile; 0x00501150 LoadLevelFromFiles; 0x0099B800 CWideString::operator=(const wchar_t*);
0x009A4EC0 GFGetSystemManager; 0x00993B10 ~CMemoryDataInputStream.

Review at landing: `g_pQuestManager` (0x013B89FC) is a `CCountedPointer<CQuestManager>` here (retail calls member Reset
0x004A9A10 on it) while older landed rows declare `CQuestManager*` -- one address, two mangled names, for the naming pass.
