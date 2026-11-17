# 2026-11-17 landing nf53: name-fix round 7, NF53 (58 rows: engine-side tail)

Owner: "Continue the naming fix queue with two agents until complete".

NF53 took the engine-side tail of the queue (targets at or above 0x00800000): 78 small entries with 101 binder rows, from NF41's
probe and NF44's binder lists. Brief, worklist and records: `naming/2026-11-17_agentNF53/`.

## Rows (103 = 101 worklist rows + 0x009ADE10 and 0x00A23C60; all terminal; STATUS.tsv)

| Status | Rows |
|---|---|
| ported | 58 |
| not-a-binder | 7 (0x009C80F0, 0x00B39080, 0x00B393D0, 0x009E45D0, 0x0073B130, 0x00BC3F30, 0x00C1A510) |
| blocked | 38 (unchanged) |

**Blocked rows:**
- **30 port not attempted:** one probe grade each; each needs its class chain moved onto the shared headers (STATUS.tsv names it).
- **3 deferred to NF52's rows:** funclets 0x01213BC8 / 0x01213BDE (parent 0x00C2DBE0) and 0x00C2E080 (depends on ~CSegmentList).
  NF52 has since landed, so these can be picked up again.
- **3 with the binding solved but not the bytes:** 0x008A7980 (8 grades, a stack-slot difference), 0x008F95D0 (9 grades,
  DIFFER 276 v 263) and 0x00C497E0 (libpk declares `class CCodaFrame`, landed rows use `struct`; queued).
- **1 authoring row:** 0x00C13D20.
- **1 Leaf defect:** 0x008DF3A0's `// Leaf:` names a symbol the row does not emit (queued).

## Entries (APPLIED.tsv, 78)
- **39 done.**
- **37 blocked**, each naming its blocked binder rows.
  - nf44 113 (InsertDriver) has 1 of 3 binders ported (0x00C2E610). Its section-2 lines are kept, so InsertDriver and the
    CEventTriggerRef ctor now have split bindings until 0x00C2E4D0 / 0x00C2E7A0 are ported (queued).
- **2 rejected (REJECTED.tsv):**
  - nf44 120: the retail trace string `libpk\PKSerialiseLoad.h` names the template that 0x00C17940 already carries.
  - nf44 122: 0x00C0DF20 calls a `sprintf("%p")` appender, so the argument is `const void*`, as landed.

## Rows and checks
- **Self renames (section 1, 26 rows; 24 file moves for sources and tests).** Among them:
  - `CDefinitionManager::GetDef<CEngineThemeDef>` (was `<CSoundThemeDef>`) and GetPDefFromClassIndex (was GetPDefFromGlobalIndex);
  - C3DMeshInfo::AreTexturesLoaded (was IsBuilt);
  - CEngineResourceManager / CGraphicBankManager ProcessLoading, CEngineShadowRenderer::UpdateShadowScene,
    CEngineTreeFilerBase::AddNodeAutoExpandTree, the CEnginePrimitiveManagerStaticMeshes ctor;
  - two scalar deleting destructors (QuadricOptimiserInternals::COptimisedVertex, CBankData);
  - `NScript::CQR_EscortTraderScript` WatchForExpression* and `vector<CCountedPointer<COptimisedPrimitive>>::_M_insert_overflow`;
  - `CCountedPointer<CVertexBufferWin32>::Reset`, `CVectorMap<unsigned long, unsigned long>` Sort / Update, ReadBankData.
- **Coordinator correction:** 0x00C21C20's change is a parameter respell on a descriptive owner (`Row00c21c20::Object::Initialize`).
  NF53's section 1 would have made that owner the catalog name; the coordinator kept the catalog name `CSoundEvent::Initialise`,
  which a landed caller (0x00C12940) and libpk/CSoundEventGroup.h support. Settling the leaf is queued.
- **Provisional self names (queued):** 0x00C62920 keeps the descriptive `CAudioStreamOwner_00c62920`; caller 0x00C47F10 spells it
  CPKDecompressedStream.
- **Funclets:** nine landed funclets include a changed parent: 0x012115F0, 0x012115F8, 0x01211E25, 0x01211E30, 0x01212A58,
  0x01213C1A, 0x0121406C, 0x01214077 and 0x01216442. All nine were rechecked with their parents. funclet_includes_check reports
  1,355 checked, 0 broken.
- **Recheck (`logs/recheck_part0..2.log`):** 67 graded from the repo after landing (58 rows + 9 funclets). All are RELOCATION_MATCH
  (two under /EHsc, 0x01211E25 under /GS) or MATCH (0x00A23C60), every test PASS.
  - 0x01211E25 had given OBJDUMP_ERR in NF53's harness; the coordinator's recheck grades it RELOCATION_MATCH[/GS].
- **Checks:**
  - `naming/rebind.py` removed 177 binding lines and added 130. It warned on 0x00C119D0 (address-only catalog name) and 0x00C21BA0
    (descriptive ctor owner) and took the largest function, the one vland graded;
  - bindings_multi_check is at its baseline (`NullCoord`).

## Headers (13 opt-ins in 10 files; lines added only, default path unchanged)
CPlayerGui.h, CBankFileAsync.h, CEngineResourceManager.h, CMeshDataBank.h, CEngineShadowRenderer.h (2), C3DMeshInfo.h,
CEngineTreeFiler.h (2), CContainmentVolume.h, CEnginePrimitiveManagerStaticMeshesBase.h and
CCreatureAction_ScorpionPrepareTailPlunge.h. Details: `naming/2026-11-17_agentNF53/hdr/README.md`.
- `FABLE_RETAIL_CENGINESHADOWRENDERER_PACKED_VECTORS` is the one opt-in that changes layout, and only under its macro.
- `FABLE_RETAIL_CCONTAINMENTVOLUME_MOVABLERESOURCE` gives the class its CMovableResource base (RTTI) under its macro.

## Name fixes queued (`land1117fu`, 22)
From NF53's NOTES.md "Follow-ups":
- callers that still bind stale spellings of NF53's renamed rows (0x009F6890, 0x00B3B870, 0x00B3B8C0, 0x00B444F0, 0x00B9E950,
  0x00B687D0, 0x00C17260, 0x00C16D90, 0x00C12940, 0x00C2E2B0, 0x00C2E190, 0x00C47F10);
- self rows whose names should become the ctors this pass now binds (0x00C31D80 PKCAutoFile, 0x00C3BB40, 0x00C1EEE0
  CEventTriggerRef) and other self rows (0x00C3AA50, 0x00B8E690, 0x00AD1650 / 0x00AD1690 / 0x00AD37E0);
- the nf44 113 split bindings, 0x00C1D950 / 0x00C20C10 / 0x00C30E90 signatures, the CCodaFrame class key and the 0x008DF3A0 Leaf.

## Rule slips (NF53 `work/slips.txt`)
- Five Bash calls exited non-zero (two `ls` of missing files, a `cat` of a missing lock, a Python IndexError from a helper named
  `dis.py` that shadowed the standard library, a `grep -c` with no match).
- Duplicate TIMEOUT lines in grades.log were removed afterwards.
