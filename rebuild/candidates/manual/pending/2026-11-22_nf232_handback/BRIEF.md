# Name-fix agent NF232 (2026-11-22, round 10): the nf231 follow-ups

Owner (2026-11-21): "we can start a new agent when one lands / finishes". You are the ONLY naming agent. Authoring agent AUTH21 (locks
AUTH21: 0x00B4D370, 0x00BD3070, 0x00502E90) and tools agent C2T1 are running: do not touch their rows or `$S/w22/auth21/`, `$S/w22/c2t1/`.
Codex pass 27 rows are off limits, and so is 0x0042BEC0 (needs a ruling).

## Your brief is NF231's with overrides
Read `$S/w22/agentNF231/BRIEF.md` and the briefs it chains to (NF230 ... NF215 "Lane", `$S/w22/agentNF232/NF214_BRIEF.md`). Paths ->
`$S/w22/agentNF232`, names -> `NF232`. On a conflict: this file > NF231 > earlier. nf231 LANDED (`batches/2026-11-22_landing_nf231/README.md`,
whose rulings you work under; hand-back `pending/2026-11-22_nf231_handback/`). Start from the repo tree as it is now.
Never write into /home/user/FableDecomp; never run git, not even read-only; wait on jobs by PID (kill -0), never pgrep -f. Disk is tight
(~3.5 GB free): clean your own scratch as you go (guarded, literal paths).

## Entries (ENTRIES.tsv), in this order
1. **Self rows** (q12629-q12631): 0x004336B0 `@OperatorNew@4` -> `?Set@CStartupDefinitionManager@@SIXPAV1@@Z` (on the header's SET opt-in);
   0x00412450 and 0x00411B00 erase self rows -> the STLport spellings their landed binders emit (ruling 2: retail is a VC7.1 STLport build).
   Census every binder and table line; one spelling per address (0x00412450 has three table lines now).
2. **The CNewFrontendGameComponent vtable split** (q12633): 0x0042EA8F (ctor) and 0x0042F50B (dtor) name the first vtable differently
   (`@@6BCBase@@@` v `@@6BCBaseClassNonCopyable@@@`) and the second `@@6BCGameComponentInterface@@@` while the header's second base is
   CDeviceResetCallback. Census, then port both emitters onto the header so both emit retail's vtable names (RTTI / PDBs). They must define
   FABLE_RETAIL_CNEWFRONTENDGAMECOMPONENT_SETQUIT, or slot 4 binds the base's 0x00415930. NF231 `work/vt_e1.txt` has the data.
3. **Row ports** (q12632): 0x0054F410 / 0x0054F370 NUISystem::CText::SetValue onto the shared headers (they model CCharString, CWideString,
   CDataBank, CMainGameComponent and a 98-placeholder-virtual CText locally), then header tests for them.
4. **Tests** (q12634) for 0x0041D98A, 0x00446720, 0x00599D5C, 0x00542140, 0x0059A238, as far as context allows (define the private static
   in the test as 0x0042EBB6 / 0x0041D0A8 do; scope `#define private public` to one header if needed).
For every entry: recheck every changed row, pk.py every new copy, census spellings (vtable slots included), link-check, TABLE_CHANGES.tsv.
Hand back with the NF231 file set and the decisions you need.
