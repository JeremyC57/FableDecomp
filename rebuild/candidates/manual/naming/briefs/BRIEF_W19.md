# Naming pass, wave 19 (one agent beside two chunk agents on the lane)

Owner rules 2026-11-01, in force for every decision: consider how retail did it and recreate it faithfully; and **when a catalog name is
wrong, fix it**. Resolved decisions: `naming/2026-11-01_wave18/DECISIONS_W18_RESOLVED.tsv` (+ waves 13-17); canonical globals / members in
`naming/ESTABLISHED_SPELLINGS.tsv` (0x0129BA3C is DELTA_NOTIONAL_ZERO since wave 18; the class statics are reached through their PDB inline
accessors: `?Get@CMainGameComponent@@SIPAV1@XZ`, the CTBaseSingletonNoDefaultAlloc `Get`, `?GetPCurrentThingManager@CThing@@`,
`?GetConstantFPS@CMainGameComponent@@SIJXZ` -- wave 18 README). Rules, mechanics and hand-back as `briefs/BRIEF_W18.md` / `BRIEF_W17.md`
(read both, the wave 18 README + AGENT_README + DEFERRED.tsv + QUEUE_W18.tsv and the wave 18 `scripts/` -- compose18.py, fam_q18.py,
fam_glob18.py, hand18/ -- reused as compose19.py etc.). RENAMES.tsv paths repo-rooted; new_name the catalog's bare spelling.
Labels the chunks proved are in `ghidra_out/labels_chunk680..684_*.tsv` and `labels_naming_wave18_2026-11-01.tsv`.

## Scope, in this order (name-only unless noted; every family rechecked against HEAD, one commit per family)
1. **Wave 18 DEFERRED.tsv** (`$S/naming/w19/DEFERRED_W18_INPUT.tsv`, 437 rows) and the partial/deferred rows of `QUEUE_W18_outcomes.tsv`:
   the three [recheck-fail] rows (0x004E34B2 + 0x004DD782 ~vector<pair<long,CCharString>> destroy chain -- the real STLport _Destroy text;
   the pgui edit of 0x007E5230 beside its pmgc edit), PeekThingSearchTools 0x008FD000 (31 rows) / PeekTypeList 0x0049C780 / 0x00947780 on
   the const / std::list models, HasTC 0x006A80A0 with reference-taking filter templates, the 13 GetPWorldSeed stand-in-class rows, the STL
   stand-in callers (00ad77f0 00ad62b0 00ad7e80 00ad6ae0 00a385a0 00ad3940), 00caf680.
2. **The c684 queue rows** (`QUEUE_W19_input.tsv`; the coordinator appends c685 / c686 / c687 rows as those chunks land -- re-read before each
   scope step): `ProcessDisposalLists` -> `?ProcessDisposalList@CEngineLandscapeMemoryManager@@AAEXXZ` (private) in the three landed Allocate*
   callers 0x00BDA360 / 0x00BDA460 / 0x00BDA560; `CreateExperienceOrbs` -> `?Create@CTCDExperienceOrb@@SIXAAVCThing@@@Z` in landed OnDie
   0x00664300; CDspGain::ApplyRamps' sixth parameter bool in the landed Apply 0x00C74CE0 / Crossfade 0x00C74D30 (re-grade); the NUISystem-
   namespaced CGraphicInfoFrame bindings 0x009FE380 / 0x009FF6B0; the NVectorMath namespace on the GFInterpolateVectors bindings.
3. **The destructor families** (0099a300 / 0099a2e0 / 0099a410 / 009fc550 / 00aa75c0 / 00f35b40 / 00cbd510) where the bytes hold, then
   **the c680 rows without a mangled final**: 0x00C73660 DetectWindowsVersion's caller 0x00C73770 (signature re-author, byte-identical only),
   0x009C4F20 ExitThread volatile (member-type change with the chunk 680 Retail-volatile evidence), 0x00C25860 Cross (prove from the PDB
   C3DVector set first), 0x0063A300 / 0x0063A390 GetThemeSizeZAt (prove which), the CODA globals 0x0143BB58 / 0x0143BB48 (one spelling each
   from the GameCODA SDK), 0x01238C8C the CScriptThing vtable.
4. **The remaining mega families, biggest first**: CBasicString::Compare, GetPItem, ~CCharString, GetSubDef, GetInstance, `Transfer<>`, AddEvent;
   then the remaining cg10 targets (`CHATGPT_NAMING_QUEUE.tsv`).
Not in this wave: the 12 direct-read ConstantFPS rows (owner item, DECISIONS_W18), byte-moving re-authoring beyond scopes 1-3, 0x00C28690
(libpk name unknown), VIRTUAL_SCREEN_* (gated), the engine header changes (migration wave 10). 0x00440730 stays on HOLD.

Hand back BINDINGS.tsv, LABELS.tsv, RENAMES.tsv, DEFERRED.tsv, QUEUE_W19.tsv (every queue row with its outcome), the per-family rows
files, logs and scripts, as wave 18 did; leave no lv_* directory and no background job. A recheck of more than ~900 rows must be run in
parts (the coordinator's recheck is split by rows not yet in the log).
