# Naming pass, wave 2 (two agents, split by family)

Wave 1 (agent's notes: `naming/DECISIONS.tsv`, `CONFLICTS.md`, `wave1/`, `tools/`, `runs/`) is landed in the repo
working tree: 315 rows (SetCombatAnimation, CDataInputStream::ReadWithSrcChunkOverflow, the CBase / _s_Default /
CWideString conflicts). The operator= family (`naming/opeq/`) is committed. Wave 1's tools (`tools/run.sh`,
`wave2.py`, `tokindex.py`, `mkpayload.py`, `vcheck.py`, `apply.py`) and `wave1/WAVE2_ROWS.tsv` (exact row -> wrong-symbol
bindings) are yours to reuse -- copy them into your own directory before changing them.

## Rules (same as wave 1, plus one)
- Never run git. Never write under /home/user/FableDecomp. Never kill a process. Never run a recursive find/glob over `/`
  or the whole scratchpad. Lane: `source $S/wine/env.sh; cd /home/user/FableDecomp`.
  `S=/tmp/claude-0/-home-user-FableDecomp/40effb6c-294b-5111-9a88-ca0c32f095ec/scratchpad`.
- Take every file you copy from the CURRENT repo working tree (it contains wave 1 + operator=).
- Proven decisions only (`DECISIONS.tsv` confidence = proven; re-check anything you doubt against the PDB lists).
- Every edited row must recheck MATCH / RELOCATION_MATCH with test PASS (dry run `--recheck-landed`, never `--land`).
- NEW: deliver each family as a deterministic, idempotent transform script (`fam_<name>.py <file>...`, like
  `naming/opeq/transform.py`) plus its verified file list, so the two agents' edits compose in any order on shared files.
  Hand-edits are allowed only for rows a script cannot express; deliver those as an overlay and list them.
- No header edits under `rebuild/include/` unless you prove (recheck) every row that includes the header; if a fix needs
  a header member (e.g. `engine/CDataInputStream.h` lacking `ReadWithSrcChunkOverflow`), list it in `HEADERS.md` with the
  exact line and the affected rows instead.
- Aim for at most ~500 edited rows; stop and report earlier rather than leaving a family half-verified.

## Agent A (`naming/w2a/`): static getters, CBase ctors, the rest of ReadWithSrcChunkOverflow
- 0x0049D870 `?GetFrame@CWorld@@SIJXZ` (58 wrong spellings, 225 row bindings)
- 0x0041E5F2 `NUISystem::CManager::GetInstance` (22 / 144)
- 0x0099A2D0 `??0CBaseClass@@QAE@XZ` (14 / 141) and 0x0099A2F0 `??0CBaseClassNonCopyable@@IAE@XZ` (5 / 144); also the
  remaining `??1CBase@@UAE@XZ` emitters (0x0099A2E0 `??1CBaseClass@@UAE@XZ`, 0x0099A300 `??1CBaseClassNonCopyable@@MAE@XZ`)
- 0x004014A0 `?Calc@CCRC@@SIKKPBXK@Z` (10 / 21); 0x0040D2A0 `CUserProfileManager::Get` (22 / 50); 0x0044C6B0
  `CGameDefinitionManager::Get` (28 / 56)
- the rest of 0x00993CA0 (models declaring it public; CInputStreamAccess / CDataInputStream_Methods / InputReader /
  InputCalls; the 3 namespaced adapters; the header item for HEADERS.md)

## Agent B (`naming/w2b/`): the large families
- 0x0040F020 LowerBound: ~748 rows emit `?LowerBound@CInterfaceMap@@QAEPAUSEntry@1@PBJ@Z`; final symbol is the CVectorMap
  template form in DECISIONS.tsv. Largest; do it only if one script can carry it, else measure and report.
- 0x00A01C10 `~CBaseIntelligentPointer`: 382 rows emit QAE where the PDB has UAE (virtual) -- check that making it
  virtual does not add a vptr to a model that had none (bytes must not change).
- 0x004AA840 `AutoTiny*Destructor`: ~506 rows should call `~CScriptThing`.
- 0x0099EBF0 CCharString ctor: 171 rows emit `PBDH` where the PDB has `PBDJ` (int vs long).
- 0x00404500 `CheckCRC`: 62+ rows emit QAE where the PDB has IAE (protected).
- 0x009AD9E0 `GetPDefFromGlobalIndex`: 74 rows.
Do them in the order that maximises verified rows per hour; measure each with a baseline pair run first.

## Hand-back (each agent, in its directory)
`fam_*.py` + `fam_*.files.txt` (verified paths), `overlay/` (hand-edited rows only), `BINDINGS.tsv`
(`old_symbol target new_symbol note`, with KEEP / RETARGET notes like wave 1), `LABELS.tsv`, `HEADERS.md`, `APPLIED.tsv`,
`DEFERRED.tsv`, the recheck logs, and a final report message (counts per family, reverts and why, what is left).
</content>
</invoke>
