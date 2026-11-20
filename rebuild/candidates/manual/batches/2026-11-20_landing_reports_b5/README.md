# 2026-11-20 landing reports_b5 (Account B's re-port, landed by A after B's wind-down): CPSCSingleSprite part 1 (8 rows)

Source: `pending/2026-11-20_winddown/reports_b5/` (B's README kept as `B_README.md`). Every base (8 rows, 2 headers) equals HEAD
(checked by reversing B's diffs).

## Rows (8): 4 MATCH + 4 RELOCATION_MATCH, every test PASS, the same grades as before
- Model swaps onto engine/CPSCSingleSprite.h (statements unchanged): 0x00B0A0F0, 0x00B0A100, 0x00B0A130, 0x00B05E10, 0x00B05EA0.
- Stand-ins rewritten on the header: 0x00AF7DF0 Alloc (static fastcall), 0x00B0A190 DatabaseClone, 0x00B07570 ClearAllPrimitives
  (CIEngine::ClearChildPrimitive slot 30).
- 0x00B05EA0 renamed from the catalog's `vfunc_18` to `NParticleEngine::CPSCSingleSprite::DatabaseCopy` (vtable 0x0129FB84 slot 18 =
  CPSCBase::DatabaseCopy; Ego_r.pdb `?DatabaseCopy@CPSCSingleSprite@NParticleEngine@@UAEXABVCPSCBase@2@@Z`); file renamed.

## Headers (add-only opt-ins; token-identical without them apart from pin_asserts' `_AT` form)
engine/CPSCSingleSprite.h `FABLE_RETAIL_CPSCSINGLESPRITE_MEMBERS` and engine/CPSCSplineBase.h `FABLE_RETAIL_CPSCSPLINEBASE_MEMBERS`
(declarations only, from the Ego_r.pdb mangled names). They are the base for the 13 CPSCSingleSprite / CEnginePrimitiveSpriteTrail rows
B's reports_REMAINING.md lists.

## Checks
- 11 rows rechecked (8 + the headers' 3 includers), all good; gate 0; bindings_multi_check 1.
- Table: two bindings move from stand-ins to PDB spellings, no address grows: 0x00B09360 `??0CPSCSingleSprite@NParticleEngine@@QAE@XZ`
  (was the global-namespace stand-in), 0x00B22530 `?DatabaseCopy@CPSCSplineBase@NParticleEngine@@UAEXABVCPSCBase@2@@Z` (virtual, was QAE).
- One definition (`logs/symbol_diff.txt`): own leaves and shared-header inlines; the only out-of-line definitions of the new members are
  the rows' own leaves. Link test over the 8 and the context link with the 3 includers: no LNK2005.
