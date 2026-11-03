# Owner decisions 2026-11-03 (D1 STLport member templates, D2 tag constructors, D3 GameCODA EAX SDK names)

Rule added to `tools/decomp_pipeline/linux_lane/lane_tools/chunk_pool/EXTRA_RULES.md` under "Owner decisions, 2026-11-03";
symbol forms in `rebuild/candidates/manual/naming/ESTABLISHED_SPELLINGS.tsv`. Per-step state: `STATUS.tsv`. Labels:
`ghidra_out/labels_decisions_2026-11-03.tsv` (= `LABELS.tsv`), applied with `apply_labels.py`. Table edits: `BINDINGS.tsv`
(applied with `coordinator/apply_bindings.py`; `bindings_multi_check.py` = 0 after every decision). File moves: `RENAMES.tsv`.
Scripts: `scripts/` (run from the repo root with `S` set to the lane scratch). Grades: `logs/`.

## D1: STLport `_Rb_tree::find` / `_M_find` are member templates
- Evidence: STLport 4.6.1 `stl/_tree.h` 457-467 declares `template <class _KT> iterator find(const _KT&)` (+ const twin) and
  private `template <class _KT> _Rb_tree_node<_Value>* _M_find(const _KT&) const` under `_STLP_MEMBER_TEMPLATES` (set for VC7.1);
  Codex pass 12 spells `??$_M_find@...` on 0x004B9370 identically.
- Respelled: 86 landed rows (28 definers + 58 binders). Flat local `map` models that owned `_M_find` moved into a 5-argument
  `_Rb_tree<K,V,_Select1st,less,allocator> _M_t`; KeyOfValue added to 4-argument trees; definers instantiate
  `_M_find<K>` explicitly. Five invented definers (0x005937E1, 0x006DAB90, 0x00990320, 0x00A7F880, 0x006BD0F0) regenerated as real
  `_Rb_tree::find<K>` bodies with binder-proven types and renamed (`RENAMES.tsv`). Leafs: `logs/d1_canonical_symbols.tsv`.
- Recheck: baseline `logs/d1_baseline_grades.tsv`; after 86/86 MATCH|RELOCATION_MATCH + PASS (`logs/d1_recheck_grades.tsv`).
- Bindings: RETIRE 35 (27 `?_M_find@` + 8 `?find@?$_Rb_tree`), ADD 28 canonical member-template symbols.
- Queue: 9 rows closed; 15 `dec1103` rows for the invented local containers still bound to the 28 targets (`logs/d1_queue_added.tsv`).
- **Held: 0x00520D00, 0x0051FD80, 0x00B2D9F0.** The `_M_find` blocker is gone, but the bodies do not compile on our headers: they
  need 42 / 39 / 21 modified shared headers (fable_containers, CThing, CWorld, CDefPointer, C3DVector, ...); read-only vland fails
  on a `CClass` identifier, a negative-subscript static assert, missing `CStringParser` ctor / `SkipPastString`, and a
  `template<> CBasicString<char>::operator<` syntax error.

## D2: `__true_type` / `__false_type` carry an empty user constructor
- `rebuild/include/stlport/fable_containers.h` default path now declares both tags with `T() { }`; the hand721 opt-in
  `FABLE_RETAIL_STLP_TAG_CTORS` branch is folded in and no tree file defines the macro any more (the two pending drafts in
  `pending/2026-11-03_linux_hand721/` still `#define` it; harmless, drop on their next round).
- Evidence: retail passes the tag temporary's address without zero-filling it (UpdateShadowedSpotLights 0x00B47F82..91;
  SwitchDoorsOnAndOff 0x00DEFB10 differed by exactly one `movb $0,0x27(%esp)` without it).
- Recheck on a full mirror overlay: all 2,263 transitive landed includers, 61 MATCH + 2,202 RELOCATION_MATCH, all PASS, 0
  regressions (`logs/d2_includer_recheck_grades.tsv`).
- Landed 0x00DEFB10 `NScript::CQ_SecretPassageScript::SwitchDoorsOnAndOff` (pass 12 wave D source rebased onto our headers + new
  `engine/NScript_CQ_SecretPassageScript.h` from the Codex row): RELOCATION_MATCH PASS (`logs/d2_land_00defb10.log`). New binding
  `??$_Construct@VCScriptThing@@V1@@std@@...` on 0x008AC120; the older `?ConstructCopy_CScriptThing_Retail` (0x00DFDE30) queued `dec1103`.
- `MIGRATION_TAGS.tsv`: 2,464 landed rows declare a local tag WITH a user ctor (the brief's 558 was an estimate) and 630 WITHOUT
  one. Untouched; the second group must be proved before any migration.

## D3: GameCODA 2.10 EAX translation spelled with the SDK names
- Evidence: Codex pass 12 SDK_COMPARISON (original GameCODA 2.10 EAXListener.obj / EAXBuffer.obj / EAX4Translation.obj / Memory.obj
  debug types and relocations) + the native layout; both member models were dumped and agree on every offset (sizes 0x4f0 / 0x3c0 /
  0xc); `NAMING_EVIDENCE.md` of the six pass 12 rows.
- Headers (one class, one header, one model): `engine/CEaxContext.h` -> `CEAXListener.h` (+ `CEAXBufferList`),
  `CEaxSource.h` -> `CEAXBuffer.h`, `CPKObject.h` -> `CCodaObjectBase.h` (git mv; SDK member names, access and offsets, pass 11 names
  in the offset comments, the pass 11 descriptive methods kept on the new classes). New: `CodaEnvironment.h` (CCodaObject,
  CCodaEnvironment, CENVIRONMENT), `CodaMemory.h`, `EAXNativeTypes.h`, `DSound3D.h`, `KernelStreaming.h`, `CSalOutputKS.h`,
  `CSalOpenAL.h`, `CSalOpenALReverb.h` (Codex copies). `EAX40.h` gains the 4 extern "C" `EAXPROPERTYID_EAX40_FXSlot0..3`.
- `CCodaObjectBase::operator new / delete` are inline forwarders to the SDK's extern "C" `CodaMalloc` / `CodaFree`
  (0x00C3C360 / 0x00C3C370, byte-identical Memory.obj bodies): every retail `new` / `delete` binds `_CodaMalloc` / `_CodaFree`.
- Respelled 136 landed rows: class tokens (`CEaxContext`/`CEAXContext` -> `CEAXListener`, `CEaxSource` -> `CEAXBuffer`,
  `CEaxSourceNode` -> `CEAXBufferList`, `CPKObject` -> `CCodaObjectBase`, `SEaxEvent` -> `_CHANGES`, `SGuid` -> `_GUID`), SDK struct
  tags in local models (`_EAXVECTOR`, `_EAXREVERBPROPERTIES`, ...), the 4 pass 11 rows onto the SDK members, the SDK-proven method
  names (`AddToChangesList` 0x00C5FFB0, private `Set3DBuffer` 0x00C60FF0, `CommitEAX3` 0x00C65A00, `GetEAX4FXSlot` 0x00C66330,
  `GetEAX4Source` 0x00C66C90, `IsNaN(const _D3DVECTOR&)` 0x00C60B30, `CodaMalloc` / `CodaFree` + hooks `CODA_MemAlloc` /
  `CODA_MemFree`), and eight mislabeled definers renamed onto the class (0x00C64FB0 / 0x00C65170 / 0x00C65450 / 0x00C65690 /
  0x00C65870 from `CEntity::*`, 0x00C64840 `SetOcclusionParameters`, 0x00C5FF50 `CEAXBufferList` ctor, 0x00C3C830
  `~CCodaObjectBase`). Binders of the pass 11 dispatchers (0x00C4E350, 0x00C4E380, 0x00C51F00, 0x00C52980) call the SDK names.
- GUIDs: EAX 4.0 GUIDs extern "C" (`_EAX_NULL_GUID`, `_EAX_PrimaryFXSlotID`, `_EAXPROPERTYID_EAX40_FXSlot0..3`,
  `_EAX_REVERB_EFFECT`; `DEFINE_GUID` = `EXTERN_C`, EAX4Translation.obj). 0x012BEE90 follows the same evidence rule the other way:
  the byte-identical original SAL Update object relocates the C++ `?SAL_EAX30_ListenerProperties@@3U_GUID@@B`, so its four binders
  (0x00C56590, 0x00C568F0, 0x00C71820, 0x00C71CF0) use that name.
- Recheck: baseline 133/133 PASS (`logs/d3_baseline_grades.tsv`); final 142/142 = 136 respelled + 6 landed, 27 MATCH + 115
  RELOCATION_MATCH, all PASS, every Leaf an exact obj symbol (`logs/d3_recheck_grades.tsv`, `logs/d3_leaf_updates.tsv`). The
  two EH funclets that `#include` a renamed parent (0x012163D0 / 0x012163F0) follow the new paths.
- Landed: 0x00C5FFB0 `CEAXListener::AddToChangesList`, 0x00C60FF0 `CEAXBuffer::Set3DBuffer`, 0x00C66330
  `CEAXListener::GetEAX4FXSlot`, 0x00C66C90 `CEAXBuffer::GetEAX4Source`, 0x00C5E650 `CSalOutputKS::CreateBuffer`, 0x00C71870
  `CSalOpenALReverb::Update` (all RELOCATION_MATCH PASS, `logs/d3_land_eax6.log`).
- Bindings: RETIRE 119 old spellings + 3 stale entries no source emits (`??_7CConsoleVariable@@6B@`,
  `?Remove@CConsoleVariableRegistry@@...`, `?PostMessage@CMessageOwner@@...`), ADD 104 respelled symbols, +16 recorded by the landing.
- Queue: 33 rows closed, 3 annotated as partial (4179, 4379, 1497), 8 `dec1103` rows (`logs/d3_queue_added.tsv`): the 20 / 10
  invented spellings of `_CodaMalloc` / `_CodaFree`, 14 of the root vtable, `?Set@PropertyInterface@N00c52840` (0x00C68F90),
  `?Remove@CRegistrationList` (0x00C5FF60), `?KSDATAFORMAT_SUBTYPE_PCM` vs the SDK `__uuidof` symbol (0x012B8DC0), and
  `CSalOpenAL` / `g_OpenAL` vs the landed `CAudioContext` / `g_AudioContext` (0x00C4D740 / 0x0143BDB0, class rename outside D3).
- Not done (no SDK record): method names of 0x00C67F60 / 0x00C68510 / 0x00C68F90 and the setters stay descriptive on the SDK class;
  the ~115 local-model rows keep their local class copies (renamed, not yet migrated onto the shared headers).
