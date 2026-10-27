# Chunk 508 (agent508, 2026-10-27): 29 of 30 single functions byte-exact (128-256 bytes)

Queued chunk of single functions of 128-256 bytes (30 rows, address order, from 0x00DD8FD0): NScript quest, level and
entity scripts (spawned-function bodies, constructors, OnPersist, Init). One Opus 5.5 agent at high effort.

- **Landed:** 29 rows with `vland.py --land` (all RELOCATION_MATCH, every test PASS, no RELOC_IDENTITY), among them
  CV_TravellingHeroesScript::ThunderChatter (0x00EE29D0, from the spawn string "ParentClass." + "ThunderChatter").
- **Parked (1, pending/2026-10-27_linux_hand508/differ/):** 0x00EE82D0 CCS_OakValeRevisitedScript::Main (172v181: retail
  opens with `mov dword [esp+8],0` into an unread local and pads the loop head with one nop; the same open residue as
  chunk 507's 0x00D6BAC0).
- **Identities:** all 30 row names proven (vtable slot plus the landed Alloc/ctor that stores it, spawn strings against
  Ego_r names, OnPersist keys, allocation sizes); proven landed rows in ghidra_out/labels_chunk508_2026-10-27.tsv. Probable:
  0x00EE8390 CCS_OakValeRevisitedScript::OakVale (by elimination) and the CDefString const/non-const split below.

CDefString identity: 0x00415D70 and 0x00415DD0 are byte-identical `operator CCharString` twins. The landed 0x008E7040
binds 0x00415D70 non-const and the landed 0x0071C7C0 binds 0x00415DD0 const; row 0x00EC8230 reaches it through a
non-const CScriptDef* and uses the non-const spelling (QAE). Ego_r lists only the const spelling.

Label corrections (NOTES.md): landed 0x00E76540 "CEM_Rhodri::Init" is CEM_Sheriff::Init; 0x00E5FC70 is
CBodyGuard::GetGoldCost; 0x00E376D0 the CV_AssassinAttacksScript ctor; 0x00E5DDE0 the CBodyGuard ctor; 0x00E64D90 the
CChickenSign ctor; 0x00412B20 vector<CWideString>::resize; 0x00E241B0 ~CIsThingAlive; 0x00EFC300
CHeroTurningDebuggerScript::MoveHeroTest; 0x00EEE7F0 / 0x00EFC250 are CSpawnedFunc<CEmptyScript> /
CSpawnedFunc<CHeroTurningDebuggerScript> slots; the CVtable_012E2428 / _012E8080 / _012efee8 stand-ins are
CV_AmbushScamScript, CV_ChapelOfEvilScript and CMurderTwistSheriffGuard. Review: landed 0x00E1FEB0 ResetRandomSpeechTime
uses a timer at +0x84 where CV_AmbushScamScript's is at +0x7c. Header debt: local models of CGameScriptInterface,
CScriptThing, CScriptBase, CEntityScriptBase and CSpawnedFuncBase (27 SHARED_HEADER_AVAILABLE warnings).
