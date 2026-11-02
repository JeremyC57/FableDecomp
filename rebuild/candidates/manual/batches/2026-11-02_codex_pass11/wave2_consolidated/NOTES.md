# cons11: Codex pass 11 consolidation notes for the coordinator

## Result

- 28 of 36 rows pass on one overlay (`hdr/`).
  - Final vland, read-only: `vland_final.log`. It reports WINS 28: all RELOCATION_MATCH with behaviour PASS.
  - `grades.tsv` and `STATUS.tsv` list each row.
  - `payload.json` holds the 28 passing rows only.
- 8 Thing-family rows were not consolidated: 0047a9f0, 00606300, 00666ef0, 00667450, 00667d30, 007355f0, 0074a5d0, 00775c60.
  - They are not parked (each had fewer than 10 attempts). They are left for the next pass.
  - What they need is in "Header debt" below.

## Default-path proof and includer rechecks

**Default path.** `tmp/hdrcheck_final.txt`:
- Every changed existing header gives `default=same`.
- The repo's own macros give `=same`, except these four (additive extensions of existing branches):
  - `FABLE_RETAIL_CCHARSTRING` (fable_string.h)
  - `FABLE_RETAIL_CTEXTURE`
  - `FABLE_RETAIL_CSIMPLEMEMORYPOOL`
  - `FABLE_RETAIL_NSCRIPT` (CScriptThing `NullCoord`)
- No landed source defines a macro introduced here (`tmp/new_macros.txt`).

**Includer rechecks** (`vland --recheck-landed`, overlay on, read-only):

| Directory | Includers checked | Result |
|---|---|---|
| `recheck2/` | the landed definers of CCHARSTRING (0121ef70, 0121e3a0), CTEXTURE (00a3b970) and CSIMPLEMEMORYPOOL (00aa4170) | 4/4 RELOCATION_MATCH PASS |
| `recheck3/` | all 3,344 landed sources that define FABLE_RETAIL_NSCRIPT, in 4 shards | 3,344/3,344 PASS (3,210 RELOCATION_MATCH + 134 MATCH); WINS 836 per shard (`recheck3/SUMMARY.txt`) |

## Decisions the coordinator must make

1. **CCountedPointer ctor (conflict).**
   - Row 00626e40 needs `CCountedPointer(T* data) : Data(data) { if (Data) ... }`. Retail tests the member and reloads it from memory.
   - The landed default tests the parameter, `if (data)`.
   - The member test is opted in with `FABLE_RETAIL_CCOUNTEDPOINTER`, and only 00626e40 defines it.
   - Choosing one model needs a recheck of the ~7k CCountedPointer includers with `if (Data)`. That was not run.
2. **CParentDefClassBase dtor.**
   - Retail's dtor is compiler-generated. In 004317d0 it is expanded inline with no vptr store. 0x00430300 is the out-of-line COMDAT, called from 122 sites.
   - The repo declares `virtual ~CParentDefClassBase();`.
   - `FABLE_RETAIL_CPARENTDEFCLASSBASE` hides the declaration, the same pattern as `FABLE_RETAIL_CLEVELSCRIPTBASE`. Making it the default is header debt.
3. **CScriptThing copy assignment.**
   - 00d17470, 00d76030 and 00e13310 expand `operator=` inline as `PImp = other.PImp`. 194 retail sites call 0x008AB980.
   - Following the header's own contract, each row defines `inline CScriptThing& CScriptThing::operator=(...)` after the include. The header is unchanged.
   - Long-term model: an inline-defined `operator=`, which needs an NSCRIPT recheck.
4. **CLevelScriptBase members.**
   - Codex made `Game`/`MasterScriptData` references. The repo (and Ego_r CTCBase-style evidence) uses pointers.
   - I kept the repo model and rewrote the sources to `Game->`.
   - Similarly, `CTCBase::ParentThing` stays `CThing*` (Ego_r struct_layouts) and 00626e40 uses `ParentThing->`.
5. **NullCoord name.** `?NullCoord@CScriptThing@@1VC3DVector@@A` (0x0143E8E0) replaces the landed spellings `?g_ZeroPos@@3VC3DVector@@B` and `?g_ZeroVector@@3VC3DVector@@A/B` (Ego_r PUBLIC32; Codex NAME_FIXES). Respell those callers.
6. **CSimpleMemoryPool.** `Allocator->New<bool>` became `AllocateType<bool>`, after FableWin `??$AllocateType@_N@CSimpleMemoryPool@@QAEPA_NK@Z` (0x03237A60). `New<T>` (FableWin `CSimpleMemoryPool::New<C3DAnimationSequence>`) is not modelled.
7. **Second models removed.**
   - These were not added: `engine/CWideString.h`, `engine/CWideStringData.h`, `engine/CDefStringTable.h`, `engine/CCharString.h`, and the stlport46/geometry.h std model.
   - They live in fable_string.h (`FABLE_RETAIL_CWIDESTRING`), fable_definition_table.h (`FABLE_RETAIL_CDEFSTRINGTABLE`) and stlport/fable_containers.h + fable_map.h.
8. **Row-local STL definitions.**
   - 0052cf40 dropped its own `_Destroy`/`_List_base::clear`, and 00531560 dropped its own iterator tags; the shared fable_containers.h provides them, with identical bytes.
   - Explicit specializations a row needs stay in the row (for example 00626e40 `list<CComponent*>::~list`, 00531560 `vector<unsigned long>` copy ctor and `__find`).
9. **SECOND_SPELLINGS.tsv** (43 rows, from `tools/spell.py` on the final vland work dir) lists every address whose landed reloc_bindings spelling differs from ours. Notable ones:
   - `CTCWife::CourtingAdviceGroupNames` vs `g_CourtingAdviceGroupNames`
   - `CTCOracleMinigame::SymbolStrings` vs `g_OracleSymbolNames`
   - `NullCoord`
   - `CMemoryAllocator::EnterSynchronisation/LeaveSynchronisation` (KI) vs `GFEnter/LeaveAllocatorLock`
   - the 009a3640 anon-namespace cache globals vs `g_pStreamingFontCache*`
   - the C3DAnimFile accessor spellings
   - `basic_string` private dispatch members
   - `CParticleEmitter` default-distance statics vs `NParticleEngine::Get*Distance`

## Header debt (for the 8 Thing rows and later)

- **CThing.h (new).**
  - It still defines `FABLE_RETAIL_CCLASS`/`FABLE_RETAIL_DEF_CLASS_BASE` for its includers. Rows should opt in themselves.
  - `PeekBaseDef` is declared only. The 00775c60 shop row needs its inline body after `CThingBaseDef` is complete.
  - GetTC/PeekTC were merged to one non-`__forceinline` form. 00667450/00667d30 used `__forceinline` GetTC/PeekTC, and that needs PDB evidence or another form.
- **Existing headers still multi-version in Codex.**
  - Each needs one per-class model merged with any repo retail branch. CThingSearchTools.h and CWorldMap.h, for example, carry repo `FABLE_RETAIL_CTHINGSEARCHTOOLS` text plus four Codex cluster variants.
  - The headers: CThingPhysical, CThingGameObject, CThingCreatureBase, CTCBase (Codex reference-`ParentThing` variant), CTCPhysicsBase, CThingSearchTools, CWorldMap, CWorld, CPlayer, CPlayerManager, CDefinitionManager, CResourceList, CDefPointer, CIVCountedPointeeBase (00667450 deletes inside Release), CTCCombat, CTCHeroExperience, the CCombatAbility* family, and CPlayerGui.
- **Pruned new files** (`tmp/pruned/`) for those rows: CTCShop, CShopItem, CGameDefinitionManager, CIntelligentPointer, CRightHandedSet, CFailedAllocationHandler, CPhysicsMeshObject, CMeshIntersectionData, CTCShapeManager, CThingFilter_None, CIDrawWorldMap, CHeroLogBook.
- **`__declspec(noinline)`** remains on CVectorMap::LowerBound (same-TU body, EXTRA_RULES chunk 422) and in the STL model where the earlier rows needed it.

## Files

- `hdr/` with `hdr/README.md`
- `src/` and `test/` (28 rows each)
- `payload.json`
- `STATUS.tsv` and `grades.tsv`
- `SECOND_SPELLINGS.tsv`
- `vland_final.log`
- `recheck2/` and `recheck3/`
- `tmp/hdrcheck_final.txt`
- `LESSONS.md`
- `tools/`: the scripts used (q.py, par.sh, reg.sh, mksrc.py, adopt.py, fileres.py, fdiff.py, calls.py, callers.py, spell.py, mkpayload.py)
