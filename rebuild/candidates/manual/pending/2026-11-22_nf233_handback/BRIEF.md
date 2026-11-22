# Name-fix agent NF233 (2026-11-22, round 10): the nf232 follow-ups (CNewFrontendGameComponent family, 0x00495780)

Owner (2026-11-21): "we can start a new agent when one lands / finishes". You are the ONLY naming agent. Running: authoring agent AUTH22
(Fable 5.2 trial; locks AUTH22: 0x00502E90, 0x0066B370, 0x00A68DE0, 0x00839420, 0x00651273, 0x00816410) and tools agent C2T1; rows
0x00B4D370 / 0x00BD3070 wait on C2T1. Do not touch any of those or `$S/w22/auth2*/`, `$S/w22/c2t1/`. Codex pass 27 rows and 0x0042BEC0 are off limits.

## Your brief is NF232's with overrides
Read `$S/w22/agentNF232/BRIEF.md` and the briefs it chains to (NF231 ... NF215 "Lane", `$S/w22/agentNF233/NF214_BRIEF.md`). Paths ->
`$S/w22/agentNF233`, names -> `NF233`. On a conflict: this file > NF232 > earlier. nf232 LANDED (`batches/2026-11-22_landing_nf232/README.md`,
whose rulings you work under; hand-back `pending/2026-11-22_nf232_handback/`, NOTES entry 2 = the family census). Start from the repo tree now.
Never write into /home/user/FableDecomp; never run git, not even read-only; wait on jobs by PID (kill -0), never pgrep -f. Disk is tight
(~3.4 GB free): clean your own scratch as you go (guarded, literal paths).

## Entries (ENTRIES.tsv), in this order
1. **The CNewFrontendGameComponent vtable / ctor / dtor family** (q12638; approved as one header unit): 0x0042F50B (dtor), 0x0042EA8F (ctor),
   binder 0x00412F90, and 0x0051D8E0's binders. Retail RTTI implies `...@@6BCBaseClassNonCopyable@@@` / `...@@6BCDeviceResetCallback@@@`;
   prove the vtable names from the PDBs / RTTI before porting. Close the NOTES gaps as add-only opt-ins (slot 1 Init on the RUN path, the
   device-reset overrides 0x0042DC5F / 0x0042DC60, the CGame& / const Init& ctor, base ctor 0x0051D8E0, the ctor-built members). The inline
   ~CGameComponent: measure at stock flags first; NO __forceinline (ASM_RULES: only where retail used asm). Both emitters define SETQUIT.
   The 0x01230C94 table line (6BCGameComponentInterface stand-in) moves with it. If the dtor cannot match without a device, write it up.
2. **0x00495780** (q12639): port onto CFrontendGameComponent.h (members + Shutdown) so its erase binder takes the STLport spelling
   (0x00412450 2 -> 1).
3. **Self rows** (q12635-q12637): 0x00412450 (if anything left), 0x00433D3A `Reset<CStartupDefinitionManager>` onto CCountedPointer.h,
   0x013B8760 `PClass`.
4. **Tests** (q12640) as far as context allows (0x0041D98A: port the row onto its header first).
For every entry: recheck every changed row, pk.py every new copy, census spellings (vtable slots included), link-check, TABLE_CHANGES.tsv.
Hand back with the NF232 file set and the decisions you need.
