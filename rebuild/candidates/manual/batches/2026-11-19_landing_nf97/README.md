# 2026-11-19 landing nf97: naming round 8, NF97 (28 of the 29 rows held at nf95b, now on the shared headers)

NF97 (advisor decision, owner delegated) ported the rows the nf95b landing held for new local models: NF95's names and bindings are
unchanged; the class models moved onto the shared headers, in the rows and in 6 tests. Records: `naming/2026-11-19_agentNF97/`
(selfcheck.log: 0 problems). It stopped for context with 6 entries open.

## Entries: 8 done (q11276, q11292, q11320, q11325, q11331, q11335, q11376, q11465's binder 0x0062ABA0), 2 blocked, 6 open
- **blocked: port:** q11273 (0x009D1320, about 9 local models) and q11436 (0x0055FC90, about 12 local models and two same-TU stand-in
  bodies); STATUS lists the opt-ins each still needs. **open:** q11400, q11444, q11447, q11448, q11236, q11237.

## Rows (28): the 22-row CGameEventDataPackage family + 0x00437870, 0x00643BE2, 0x0065F0FA, 0x006A7610, 0x007554C0, 0x008645E0
9 self renames (RENAMES_applied.tsv, 16 file moves) and 19 respellings. Recheck from the repo after landing (`logs/recheck.log`):
MATCH / RELOCATION_MATCH, every test PASS. Tests keep their catalog PassPattern, so the two pattern changes NF95 asked for are not needed.

## Headers (token-identical without the opt-ins; asserts pinned, 0 name mismatches)
engine/CGameEventDataPackage.h: `FABLE_RETAIL_CGAMEEVENTDATAPACKAGE_TYPED_READERS` (new: inline GetUBYTE / GetUWORD / GetFloat / GetSLONG,
FableWin.pdb names, bodies `// Inferred:`; six rows DIFFER with a direct GetData call and match with the reader) and `_GETUHUGE` (NF95);
engine/CIsHornTaskWithThisHorn.h `_CTOR_OUTOFLINE` (NF95). Includer recheck: the one other landed includer, 0x0065BE6C, RELOCATION_MATCH PASS.

## Checks
`naming/rebind.py`: removed 38, added 26; `bindings_multi_check.py` at baseline; funclets 1,355 checked, 0 broken; selfcheck 0 problems.

## Queued: `land1119nf97` (2 new catalog fixes: 0x009F16E0 GetNoEvents, 0x009F16D0 GetEvent; 0x009F18F0 GetData was already queued).
## Rule slips (NOTES): one foreground sleep; two blocks at 0 grades; STATUS / APPLIED for the family written after the batch payload.
