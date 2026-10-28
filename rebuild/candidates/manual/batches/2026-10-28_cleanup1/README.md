# Cleanup pass 1 (2026-10-28): wrong identities respelled in 1,826 landed rows

Owner request: fix every open naming / identity issue in landed rows, as close to retail as evidence allows. One Fable
agent at high effort (restarted once after a container restart), applied and rechecked by the coordinator. NOTES.md
has the agent's per-issue evidence and its "Not fixed" list.

- **0x004C73D0 / 0x006A4D00** (byte-identical `mov eax,[ecx+60h] ; add eax,0Ch ; ret`, two functions): Ego_r.pdb donor
  order (OnDie / PeekPos / PeekLastPos / OnCreate = retail 0x006A4CF0 / 0x006A4D00 / 0x006A4D10 / 0x006A4D20) gives
  0x006A4D00 `?PeekPos@CThingPhysical@@QBEABVC3DVector@@XZ`; 0x004C73D0 is `?GetPos@CThing@@QBEABVC3DVector@@XZ`.
- **0x00487DC0 / 0x00487DD0**: every landed caller now calls `CPlayer::GetPControlledCreature() const` /
  `PeekPControlledCreature() const`.
- **STLport access**: protected `_M_*` helpers spelled IAE per STLport 4.6 `stl/_vector.h`.
- **IS_THING_ALIVE**: 0x0043DBD0 = `<CThingPlayerCreature*>` (all 26 retail feeds are GetPControlledCreature results),
  0x0043DBF0 = `<const CThingPlayerCreature*>` (all feeds are PeekPControlledCreature); 0x0064A568 / 0x0065196C rebound.
- **Chunk 517-518 review items**: CBaseClass / CBaseClassNonCopyable ctor and dtor access, the message filter ctors
  0x006AF290 / 0x006AF2B0, FindMostRecentMessage<Bind2>, CAIStateGroupBase PeekWorld / GetWorld (133 rows), and others.

Coordinator application:
- 1,840 source and test files copied from the agent's fix tree after checking that the chunk 517 rows it touched were
  edited on top of their landed versions; no file renames.
- `rebuild/oracles/reloc_bindings.tsv`: the agent's 1,568 edits were keyed by caller, but the table records a symbol once
  (first caller), so the edits were re-applied by (old symbol, target): 492 rows rewritten, 63 new symbols recorded.
- Three rows reverted to their landed form because the respelling made one symbol bind two addresses: 0x007B0950 and
  0x00935160 (the non-const `GetMainPlayer` is 0x00449970; they call 0x00449960, landed const QBE), 0x00957F20
  (`CIntelligentPointer<const CThing>` ctor belongs to 0x004A96F0, not 0x0056C860). No symbol binds two targets after.
- 22 catalog names corrected (auto-re-candidates.tsv); 29 labels in ghidra_out/labels_cleanup1_2026-10-28.tsv.
- Independent recheck (`vland.py --recheck-landed`, dry, each row's own pass pattern and flags from build_candidates.ps1,
  oracle from the catalog): 1,826 of 1,826 MATCH / RELOCATION_MATCH PASS (verify_recheck.log).

Left for later: 0x00487DC0 still has 2 stray spellings and 0x004C73D0 8 (rows landed after the pass started); 0x00A01B50
(an intelligent-pointer GetPItem) carries about 100 invented spellings (pre-existing); 0x0049D810 GetPRandomSeed (17
spellings), 0x00449970 GetMainPlayer externs, and the review lists NOTES.md names as not reached.
