# Naming pass wave 10 (2026-10-30): 543 rows respelled (584 row-edits); the migration-unblocking respellings

Inputs (brief `../briefs/BRIEF_W10.md`): NAME_FIXES_QUEUE rows for chunks 575-585 and migration waves 1-5 (134 rows), the
chunk 586 libpk lock family (added mid-wave), 0x0143E8F8. Every spelling checked against both PDB name lists; `_W` lands as `G`.
Sub-waves, each with its own recheck: sw1 283 rows (0x00CD23B9 IsValid 139; 0x006E7510/30 Min/MaxWorldFrameForMessages,
protected const, 74; 0x00449970 `CPlayer& CPlayerManager::GetMainPlayer()` 75), sw1c 38 (GetMainPlayer free/accessor stand-ins
rewritten to the real member call; 7 tests updated), sw1b 38 (ScriptedThingBase slots QAE -> UAE), sw586 174 + sw586b 3
(PKCProtectionInstance ctor/dtor, g_soundLock, the static-init rows, _CODA_IsSourcePlaying@4, 00c5a1d0 as
CVtable_012B5C60::Initialise), sw2 20 (rest of the queue), sw3 28 (0x0143E8F8). 154 EH funclet stub rows of edited parents
follow their parents (regraded against the overlay parents).
0x0143E8F8 decision: `?g_pGameScriptInterface@@3PAVCGameScriptInterface@@A` (no PDB data symbol; every landed use calls
CGameScriptInterface virtuals through it, slots 89 / 653; 152 rows already spelled it so; 28 respelled; 9 flattened-header
rows deferred with a cast plan). Evidence in DECISIONS_W10.tsv (120 targets: 16 applied, 11 partly, 47 no change, 46 open).
Rejected approach: inline forwarders for the GetMainPlayer accessor classes compiled as separate functions still calling the
invented name (passing only because that name was bound) -- replaced by direct call-site rewrites.
Renames (RENAMES.tsv, all applied with git mv + build_candidates.ps1): 006e7510 GetMin.., 00c35ca0 global_CODA_IsSourcePlaying,
00c5a1d0 CVtable012b5c60_Initialise, 01224b00 staticinit_g_soundLock.
Bindings: 178 retired (incl. 14 stale), 9 added (exact PDB spellings), 13 kept, 1 no-bind (PKCCriticalSection ctor has no PDB
name); multi-check 0. Labels: 7 proven (`ghidra_out/labels_naming_wave10_2026-10-30.tsv`); 5 catalog names corrected.
Pre-existing RELOC_CONFLICT on 00e383a0 (`??BCDefString@@QBE?AVCCharString@@XZ`) noted, unchanged since wave 9.
Deferred (DEFERRED.tsv, 26 items with plans): GetMainPlayer rows on the method-less generated CPlayerManager.h (14; plan: have
gen_class_headers.py emit PDB methods), dtor/ctor stand-ins 0x007E74D0 / 0x007E72A0 / 0x006E7B80 (24 / 8 / 21: an explicit
`x.~T()` makes VC7.1 emit ??_G + operator delete; objects must become real locals), PKCCriticalSection ctor/dtor (26 / 40
rows), large families (Compare 137, _M_increment 99, CCountedPointer operator= 82, 0x006E7AB0 49 probable, CParentDefClassBase
64/13/16), wave 9's 321 open decisions (not reached).
Recheck: recheck.log (coordinator full regrade of all 543 changed rows after composing against the current tree).
