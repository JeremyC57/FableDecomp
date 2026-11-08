# hdr/ overlay -- chunk 890 header changes (additive opt-ins; default paths unchanged; diff against rebuild/include)

## engine/NUISystem_CTable.h -- `FABLE_RETAIL_CTABLE_CONSTRUCT`
Row 0x00551EA0 NUISystem::CTable::ConstructSpritesToDraw. Declares the three virtuals CTable introduces after
CChangingStateComponent's last slot (141), protected, in vtable order (rtti-vtables CTable 0x0124A224: slot 142 0x00551EA0,
143 0x005518E0, 144 0x00551BC0; the row calls 143/144 as `call [eax+23Ch]` / `call [eax+240h]`):
- `?ConstructSpritesToDraw@CTable@NUISystem@@MAEXXZ`
- `?ConstructHorizontalLine@CTable@NUISystem@@MAEXV?$CCountedPointer@VCComponent@NUISystem@@@@0AAVC2DVector@@KK@Z`
- `?ConstructVerticalLine@CTable@NUISystem@@MAEXV?$CCountedPointer@VCComponent@NUISystem@@@@0AAVC2DVector@@KK@Z`
Sources: Ego_r.pdb and FableWin.pdb (ghidra_out/egor_pdb_names.tsv, fablewin_pdb_names.tsv). Parameter names descriptive.
Declarations only (no layout change; the landed 0x005518E0 / 0x00551BC0 still use their own local models -- at landing they can
move onto this opt-in).

## stlport/fable_map.h -- `FABLE_RETAIL_STLP_TREE_FIND_MEMBER_TEMPLATE`
Row 0x00551EA0. Owner decision D1 (2026-11-03) applied to the non-const `_Rb_tree::find`: STLport 4.6.1 `_tree.h`
`template <class _KT> iterator find(const _KT& __k) { return iterator(_M_find(__k)); }` (under _STLP_MEMBER_TEMPLATES). Same
body as the default; only the symbol changes, to the member-template spelling retail's one-address-one-name rule needs:
0x00553520 = `??$find@W4ETableSprites@NUISystem@@@?$_Rb_tree@W4ETableSprites@NUISystem@@U?$pair@$$CBW4ETableSprites@NUISystem@@V?$CCountedPointer@VCComponent@NUISystem@@@@@std@@...`
(called out of line at three sites of the row, expanded at the others). The const twin is untouched.
