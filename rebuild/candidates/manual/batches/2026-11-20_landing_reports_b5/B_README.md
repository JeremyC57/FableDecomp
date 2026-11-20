# b5 (reports re-port, Account B, 2026-11-20): CPSCSingleSprite part 1 -- 8 rows onto engine/CPSCSingleSprite.h -- READY

Rows: 0x00AF7DF0 Alloc, 0x00B05E10 SmoothFadeValue, 0x00B05EA0 DatabaseCopy (catalogued "vfunc_18"), 0x00B07570 ClearAllPrimitives,
0x00B0A0F0 GetComponentType, 0x00B0A100 GetComponentSubType, 0x00B0A130 EnterTimelineEnd, 0x00B0A190 DatabaseClone.

## Grades (vland1.log STRICT, --recheck-landed, overlay hdr/)
Every row grades exactly as the baseline: MATCH x4 (00b07570 00b0a0f0 00b0a100 00b0a130) and RELOCATION_MATCH x4, tests PASS.
No HEADER_POLICY or SHARED_HEADER warning is left in any of the 8. Existing includers of the two touched headers were regraded with
the overlay: 0x00B05C60, 0x00B075D0 and 0x00B202D0 all RELOCATION_MATCH PASS.

## Rows
- Model swaps only (statements unchanged): 0x00B0A0F0, 0x00B0A100, 0x00B0A130, 0x00B05E10 (keeps its `// Retail-asm:` line; the
  GFFloatToLongNear helper), and 0x00B05EA0. 0x00B05EA0 calls CPSCSplineBase::DatabaseCopy qualified (0x00B22530), so it needs the
  new CPSCSplineBase opt-in. It also gets a Leaf line, ?DatabaseCopy@CPSCSingleSprite@NParticleEngine@@UAEXABVCPSCBase@2@@Z: the
  landed row had none, and the catalog name "vfunc_18" is wrong (see NAME_FIXES).
- Rewritten on the header (the landed texts were stand-ins):
  - 0x00AF7DF0: `static CPSCBase* FABLE_FASTCALL Alloc() { return new CPSCSingleSprite; }`. It was a free cdecl CPSCSingleSprite_Alloc;
    with no arguments the code is the same.
  - 0x00B0A190: `CPSCBase* psc = Alloc(); psc->DatabaseCopy(*this); return psc;`, with Ego_d's local `psc`. It was an IDrawableObj
    fiction calling slot 0x12 through a member-pointer table, with Alloc bound as GetDrawableObject_00af7df0.
  - 0x00B07570: on CIEngine::ClearChildPrimitive (slot 30) and CEnginePrimitiveHandle::IsNull(), with Ego_d names pengine /
    system_group / c. It replaces the local CEngineInterface::RemovePrimitive / CPrimitiveGroupHandle{m_pLink,m_pGroup} models.
    Macros: CIENGINE, CENGINEPRIMITIVEHANDLE(_DATA), CIVCOUNTEDPOINTER(_CONVERT), CENGINEPERSISTENTPRIMITIVEBASE, DEF_CLASS_BASE.
- Tests: the landed tests are self-contained behaviour replicas (they do not link the row object). They still compile and pass
  unchanged, so none is replaced.

## Headers (hdr/; hdr.diff, add-only; the one -/+ pair is pin_asserts.py pinning the size assert to FABLE_STATIC_ASSERT_AT(69, ...))
- engine/CPSCSingleSprite.h, FABLE_RETAIL_CPSCSINGLESPRITE_MEMBERS: the class's member set from the Ego_r.pdb mangled names (ctor,
  static __fastcall Alloc, the virtual overrides, ~, and the protected GetEndPos / SmoothFadeValue / AddTrailPosition /
  SetTrailPositionsFromSpline). Declarations only. AddToBoundingSphere sits under #ifndef of the existing hand3c3 opt-in.
- engine/CPSCSplineBase.h, FABLE_RETAIL_CPSCSPLINEBASE_MEMBERS: CPSCSplineBase's own members per Ego_r. Declarations only; the
  GetComponentCost declaration is guarded against the existing opt-in. Its asserts were already _AT pinned.

## Names (NAME_FIXES.tsv)
(see NAME_FIXES.tsv: catalog 0x00B05EA0 vfunc_18 -> DatabaseCopy; stand-in names retired by 0x00AF7DF0 / 0x00B0A190 / 0x00B07570)
