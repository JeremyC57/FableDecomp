# 2026-11-20 landing nf135: PRIORITY slice 7 fifth pass (6 rows + 1 coordinator owner fix)

Records: `naming/2026-11-20_agentNF135/` (selfcheck 0 problems). 5 entries: q6453 / q6455 done (all 18 referencing rows of 0x0073A680
CModeBase::GetWorld now bind `?GetWorld@CModeBase@CTCCreatureModeManager@@QBEAAVCWorld@@XZ`; table 5 -> 1); q4747, q4802, q6408 open (NF138).

## Rows (6): 6 RELOCATION_MATCH, every test PASS (`logs/recheck.log`)
- 0x008E7CF0, 0x008E7B90, 0x008D7630, 0x008DCB60, 0x008DC900: off their local stand-ins onto the shared headers; each own leaf takes the
  two-PDB spelling (RENAMES section 1; files move under the NCreatureMode module names).
- 0x008DF310 respelled in place with 0x008DCB60 (ExtinguishFlames QAE -> AAE).

## Coordinator fix: 0x008C7A70 GetSubDef<CSnowTrollDef> (owner) re-authored for one definition
0x008D7630 now instantiates `CDefClassBase::GetSubDef<CSnowTrollDef>` from engine/CDefClassBase_GetSubDef.h (a pick-any COMDAT), while the
owner row defined the same symbol strongly as an explicit specialization: linking the two gave LNK2005. The owner now instantiates the
same shared body (identical statements) and takes its address in an anonymous-namespace helper (GOTCHAS nf86 / nf88), on the shared
CSnowTrollDef.h / CDefinitionManager.h models. Graded RELOCATION_MATCH + PASS; its GetSubDef is a COMDAT section. Link test
(`LINK=` cleared) over the 6 rows + the owners 0x008C7A70 / 0x008E7790 / 0x008E3300 + 0x00956E70 / 0x008DB810: the CSnowTrollDef clash is
gone; the two left (GetSubDef<CBriarRoseDef>, <CJackDragonDef>) exist at HEAD with the same objects (`logs/link_control_head.log`:
0x008E7790 vs 0x00956E70, 0x008E3300 vs 0x008DB810) and are queued as debt for the same fix.

## Headers (add-only opt-in)
engine/CSnowTrollDef.h FABLE_RETAIL_CSNOWTROLLDEF: a retail branch `CSnowTrollDef : CSubDefClassBase` (same shape as CBriarRoseDef.h). No
landed includer defines it before this landing; 0x008D7630 and the owner 0x008C7A70 use it. Accepted (advisor rules: add-only, default path
token-identical).

## Checks
- 256 rows rechecked, 256 good; gate 0; bindings_multi_check 1.
- Table: no set grew except witness 0x009ADA10 0 -> 1 (the owner's existing GetPDefFromGlobalIndex binding, now recorded). Shrank:
  0x0073A680 5 -> 1, 0x00833A10 2 -> 1, 0x008E7790 4 -> 2, 0x008E3300 3 -> 2, 0x008C7A70 3 -> 2, 0x008621A0 8 -> 6, and 0x006B3950,
  0x007446A0, 0x00861D00, 0x0080C460, 0x0074A5D0, 0x004C08C0.
- One-definition: `logs/symbol_diff.txt` (own leaves, shared-header inlines, STLport and CDefPointer template COMDATs) and the link test above.
