# Landing nf291 (Account A, 2026-11-23): the last T1 standin_alias entries (1 row, 1 header opt-in, 3 table lines)

Hand-back: `pending/2026-11-23_nf291_handback/`; helpers (tbl.py full symbols from reloc_bindings, mkstatus.py, eqrun.sh) under
`naming_batch/agent_tools/nf291/`. All 110 entries reached: 1 row, 2 table-only, 107 skipped-port (queued T3). The T1 standin_alias tier
is now fully reached; what is left of it is port work.

- **0x007022E0 CTCQuestCard::IsVignette via binder 0x00896A30 SetQuestCardObjective:** new add-only opt-in
  `FABLE_RETAIL_CTCQUESTCARD_FLAT_ISVIGNETTE` in `engine/CTCQuestCard.h`'s flat branch (`bool IsVignette() const;`, both PDBs, the exe call
  at 0x00896AC4); equiv_check over 1,407 includer rows: 0 differ, the planted control flags 10. Row: the `#define` merged into an existing
  comment line (47 lines), the stand-in struct emptied; objsec ALL_EQ under the one rename. Q1: 0x00896A30's T3 queue line concerns other
  members (NF289 entry 24); this change is only its IsVignette line (accepted). The spelling stays unresolved in the link (as at HEAD): the
  self row defines only a generated descriptive name (T3 self port, NAME_FIXES).
- **Table:** 0x0062FE60 `??0CClass_0062fe60@@QAE@XZ` -> `??0CUIDef@@QAE@XZ`, what witness 0x00433670's HEAD object binds (Q3: replace,
  so the split it really has stays visible; the NUISystem respell is a binder self port). 0x007F5560: the coordinator compiled witness
  0x007F53A0's HEAD object (`logs/witness_007f53a0_head.log`, RELOCATION_MATCH); it binds
  `?GetSubDef@CThingBaseDef@@QBE_NPAV?$CDefPointer@$$CBVCSoundAtmospheresDef@@@@@Z`, exactly the symbol NF291 derived, so the replace applies.
- Q4 ruling: a flat branch marked "GENERATED ... do not edit by hand" may take an add-only declaration opt-in: gen_class_headers.py keeps
  any header carrying a `FABLE_RETAIL_` mark (RETAIL_MARK, "hand retail correction kept"), so a regenerate cannot drop it. Recorded in the
  standing brief; CWorldMap (entry 34) and CTCBuyableHouse (NF289 37 / 38) go to NF292.

## Recheck
`logs/recheck.log` (landed tree): 0x00896A30 RELOCATION_MATCH PASS, 0 RELOC_CONFLICT.
