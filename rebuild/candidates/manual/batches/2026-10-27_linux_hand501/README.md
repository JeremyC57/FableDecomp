# Chunk 501 (agent501, 2026-10-27): 40 of 40 single functions byte-exact, every name proven

Queued 40-row chunk of single functions under 128 bytes (address order, 0x00E04B50..0x00EC3C50: entity and level
scripts, CConversation, script predicates). One Opus 5.5 agent at high effort; 36 rows matched on the first grade.

- **Landed:** all 40 rows with `vland.py --land` (verify.log: 15 MATCH, 25 RELOCATION_MATCH, every test PASS, no
  RELOC_IDENTITY). Nothing parked or skipped.
- **Identities:** 40 proven (ghidra_out/labels_chunk501_2026-10-27.tsv): 21 entity-script and 14 level-script methods
  (vtable slot + landed Alloc/ctor store + Ego_r name + parent layout; four level scripts without a landed ctor are
  proven by a layout only one Ego_r class matches plus their Main's strings), CConversation::Copy 0x00E54D10 and
  ::Transfer 0x00E579E0, CIsVariableFalse::operator() and Clone, CPredicate_And<CIsThingAlive,CIsVariableFalse>::Clone.
  Three enum values have no PDB name (ESheriffState 4, BriarRoseOpinion 0, EscortTraderManagerPhase 0).

Label corrections (NOTES.md): landed 0x00E90750 / 0x00E92900 (`CVtable_012EC138` / `Alloc_CVtable_012ec138`) are
CV_GuildMasterScript's ctor and Alloc; landed 0x00F336F0 (`CVtable_012e5ff0`) is CConversation::CConversation(); landed
0x0040E1E0 (a free function) is the protected static `__fastcall` CPersistContext::GetDefaultVal<CCharString>; 0x00EA86F0
is the CV_MurderTwistScript ctor; 0x00E57BB0 / 0x00E9C360 are the BeggarAndChild / IntroductionToTrophies ctors;
0x00E257C0 is Transfer<CQR_EscortTrader_ManagerScript::EscortTraderManagerPhase> (the catalog enum name is invented);
0x008AD5F0 is vector<CScriptThing>::_M_insert_overflow; the binding for 0x00CD3152 uses an invented CScriptThingVector
(real: std::vector<CScriptThing>::erase); 0x00EA4FF0 is CLostTraderBrother's Alloc (the MakeRandomComment catalog row
runs into it, so the owner lookup misattributes the vtable store at 0x00EA5024). Header debt: CGameScriptInterface and
CScriptThing headers carry no vptr (20 SHARED_HEADER_AVAILABLE warnings, local models as in chunk 499).
