# Landing acctB_b24d (Account A, 2026-11-24): 0x00ED8680 NScript::CV_TalentlessBardScript::CTB_Bard::Main, staged by Account B

Owner ruling (2026-11-24): B verifies and stages; A lands. B's row and notes: `pending/2026-11-24_acctB_b24/` (rows/00ed8680.cpp,
hdr/00ed8680, notes/00ed8680/ESCALATE.md). A re-graded it on its own tree with B's add-only header overlay
(`FABLE_RETAIL_CTB_BARD_DECLS` in NScript_CV_TalentlessBardScript.h; hdrcheck2: every existing macro `=same`):
RELOCATION_MATCH PASS before landing (`logs/verify.log`) and again from the repo (`logs/recheck.log`). keypayload changed only the
test's class keys (struct -> class, as the PDB records them). No ASSERTs in this quest-script row.

## Pick-any: landed as debt under the identical-HEAD-copy exception (advisor decision, owner delegated, 2026-11-24)
The object emits `??1CScriptGameResourceObjectMovieBase@@UAE@XZ` (25 bytes), NEQ to retail 0x006E7B80 and called by the leaf. The
NF340 new-row gate holds such a row; this one lands under the narrow exception recorded in EXTRA_RULES (2026-11-24 entry). All four
conditions were checked:
1. **Same copy already landed.** `logs/movbase_vs_00d62480.txt` (`tools/decomp_pipeline/linux_lane/lane_tools/naming_batch/agent_tools/nf343/comdat_eq_coordinator.py`): the copy
   equals landed 0x00D62480's HEAD copy at object level: bytes, relocation symbols (CCountedPointer<MovieBase> dtor, the
   CScriptGameResourceObjectBase vftable, ~CBaseObject), COMDAT selection (2, select-any) and section characteristics.
2. **No fix on record.** Agent NF343 (`pending/2026-11-24_nf343_handback`, NOTES group 2): both copy-EQ texts (M: the inline body at row
   end; D: the declaration removed) make ~MovieBase EQ 66, but they move this leaf 3636 v 3652. B's
   FABLE_RETAIL_CSCRIPTGAMERESOURCEOBJECTMOVIEBASE_OUTOFLINE_DTOR gives 3636 too. The cause (a decl-only CCountedPointer dtor) sits in 134
   landed sources.
3. **Nothing else.** Under both checkers on A's object (`logs/pickany.log`, `logs/pk_all.log`, row length 3652, source a = the leaf's own
   relocations), ~MovieBase is the only leaf-called NEQ copy. The three other NEQ copies are not leaf-called and are bound at HEAD
   (debt already queued): ??_GCBaseClass 0x0099A470 (route4-identity), ??_GCScriptThing 0x004AB170 (nf328), ~CSpawnedFuncBase 0x00CDD4C0
   (nf340).
4. **Booked.** The row carries a `// Pick-any:` note; the T3 line nf340 0x006E7B80 lists 0x00ED8680 (and 0x00D62480) as emitters.

Link order: 0x00ED8680 sits after both 0x006E7B80 and 0x00D62480, and the copy is identical to 0x00D62480's, so the copy the linker keeps
does not change.
