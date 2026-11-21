# Landing nf211 (Account A, 2026-11-21): 0x0047DB73 bitset binder, CCombatManager::GetCombatDebugInfo, IsInMeleeAsAttacked binder (3 rows)

Agent NF211 was the single running agent (owner order: one agent at a time). It finished both entries: q11772 done, and q4389 blocked on its
two out-of-slice rows only. Its files are in `naming/2026-11-21_agentNF211/`, with the evidence under `work/` in the scratch agent directory.

- 3 rows land, all RELOCATION_MATCH, every test PASS.
  - 0x0047DB73 (q11772): NF209's held draft, unchanged but for one comment line. Its call to 0x00410DE0 moves from the local `?Do@D@@` to
    `?test@?$bitset@$0BBC@@std@@QBE_NI@Z`, which closes that address (2 -> 1). Its 8-byte HasTC COMDAT is copy-against-copy debt, per the
    nf210 ruling: 0x004118C0 has no landed row, and the copy links with 0x00488620's with no LNK2005.
  - 0x006EAC50: the row nf210 held. A leaf-only self rename to `?GetCombatDebugInfo@CCombatManager@@QBE?AVCCombatDebugInfo@@ABVCThingCreatureBase@@@Z`
    (Ego_r + FableWin). Fixes from the advisor's plan:
    - The new `FABLE_RETAIL_STLP_VECTOR_COPY_DECL_ONLY` opt-in makes the `vector<C3DVector>` copy ctor external. The CCombatDebugInfo
      copy-ctor copy now equals 0x006F3000's object (79 = 79), and its 0x006F0810 calls are external on both sides (the same target under
      the nf205 amendment).
    - The default-ctor copy's owner is landed 0x006F2BD0: q.py RELOCATION_MATCH, 46 = 46 bytes. Its rename is queued.
    - 0x006E99B0 GetDefenderZone and 0x006F0250 `_M_find` bind their self Leaves (2 -> 1 each).
    - The old self leaf is bound nowhere (table 0, sources 0).
  - 0x006ED510: respelled in place, `IsCombatant()` -> `IsInMeleeAsAttacked() const`, onto 0x00662430's self Leaf (both PDBs). No defined
    symbol changes.
- Headers (add-only, `orig` equals HEAD, pin_asserts pinned 0; no landed row defines either macro):
  - stlport/fable_containers.h: VECTOR_COPY_DECL_ONLY, an `#elif` of the COPY_OUTOFLINE block.
  - engine/CThingCreatureBase.h: ISINMELEEASATTACKED (NF210's draft, unchanged).
- Table hygiene: the recheck dropped `?IsCombatant@CThingCreatureBase@@QAE_NXZ` 0x00662430, whose witness was 0x006ED510. Landed 0x006ED9E0
  still binds that spelling (its source declares and calls `IsCombatant()`), so the line is kept with 0x006ED9E0 as its witness, and
  0x00662430 stays at 2 spellings. The respell of 0x006ED9E0 is queued.
- Pick-any (`logs/pickany_check.txt`): two lines are OK. The coordinator graded the MISSING lines with q.py, the row source at each owner
  address:
  - 0x004DCFBB `~vector<C3DVector>` RELOCATION_MATCH (its catalog name is a stand-in);
  - 0x00403A10 RELOCATION_MATCH and 0x00403A00 MATCH.

  The HasTC line is the accepted debt above.
- Link (`logs/link_landing.log`): 1 LNK2005 against 0 at HEAD, `?GetCombatantInfo@CCombatManager@@`. 0x006EAC50's COMDAT copy grades
  RELOCATION_MATCH at its owner 0x006E9360, which defines it strongly; landed 0x006E94D0 already emits the same identical copy. This is
  accepted as debt (nf181 / nf184), and making the owner pick-any is queued.
- Recheck (`logs/recheck.log`): 2257 good, 0 bad. That covers 2200 includers of the two headers and the referencing rows, with the
  universal-binder exclusion for 0x00410DE0 (339 referencers). Split gate 0 created.
- tablecheck: shrinks at 0x006F0810 6 -> 5, 0x006F0250 3 -> 2, 0x00410DE0 2 -> 1 and 0x006E99B0 2 -> 1 (0x00662430 stays 2, see above);
  none grows. No out-of-image oracle line.
- Queued: 0x006F2BD0 `??0CCombatDebugInfo@@QAE@XZ` (FableWin + body, with 0x006F3000's copy ctor), 0x006ED9E0's IsCombatant respell,
  owner 0x006E9360 pick-any, and the host-model tests of 0x006EAC50 / 0x006ED510.
