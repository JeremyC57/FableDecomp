# Naming pass wave 16 (2026-11-01): 145 landed rows respelled to the proven c667-c674 names (plus one wave 15 deferral)

Inputs: brief `naming/briefs/BRIEF_W16.md`, wave 15 README / DEFERRED / scripts, `naming/ESTABLISHED_SPELLINGS.tsv`, QUEUE_W16_input.tsv
(184 NAME_FIXES_QUEUE rows c667-c674). One agent authored the overlay in `$S/naming/w16`. In `scripts/`, compose16.py (+ compose16_steps.py)
recomposes every stage on fresh copies of the current repo. hand16/ holds the per-family hand stages; fam_sp.py, fam_q.py and fam_leaf.py
are the generic respells. ov_final has 147 files: 145 sources + 2 tests, the tests of 0x00A0B6C0 and 0x00AC1EE0. Their old tests linked
the source by the old names.

Recheck against HEAD (vland --recheck-landed through scripts/vrun.py, `logs/recheck_final.log`, `runs/final.grades.tsv`):
**145/145 PASS (8 MATCH, 137 RELOCATION_MATCH)**. Name-only: no byte moved, so no row is deferred for a byte diff.
BINDINGS.tsv: 36 RETIRE / 61 ADD / 7 KEEP. `runs/reloc_bindings.after.tsv` = repo table - RETIRE (43 lines; some symbols had duplicate
lines) + ADD. `bindings_multi_check.py` reports 0 (`logs/bindings_multi_check.log`). LABELS.tsv has 19 labels. RENAMES.tsv has 38 git mv +
build_candidates.ps1 Module/Source/TestSource changes, including the three wave 15 leftovers. Apply them after the content commits.

## Families, in commit order (`runs/<fam>.rows.txt`)

- **gdt 53**: the ??_G stand-ins become the compiler-emitted `??_G<Class>@@UAEPAXI@Z`, plus their `??1` callee.
  - The stand-ins were `CLandscapeBackgroundPatch` vector-deleting copies, free `__fastcall *_scalar_deleting_destructor` functions and
    `?ScalarDeletingDestructor@`.
  - Emitters follow the landed precedents. Most rows define the class's PDB ctor with an empty body; it emits the vtable, which emits the ??_G.
  - MakeOne_<addr> (`return new X;`) is used for the classes with no PDB ctor: CASoundManager, CSoundBankLH, CSoundManagerLH.
  - Kill_00b44420 (`delete p;`) is used for the non-virtual CEngineMap ??_G.
- **qctor 8**: the quest-script ctors become `??0<C>@NScript@@QAE@PAVCScriptDataBase@@AAVCGameScriptInterfaceBase@@@Z`.
  - The two Create_CClass_* rows become `?Alloc@<C>@NScript@@SIPAVCScriptBase@@...`.
  - 0x00ED4AA0 CV_SwordInTheStoneScript moves into namespace NScript.
  - User destructors that retail does not have are dropped (0x00ED76E0, 0x00EE41F0, 0x00E81F60).
- **stl 6**: STLport 4.6 text, explicitly instantiated.
  - 0x00510D40 and 0x00A53300: `_List_base<CIntelligentPointer<...>>::clear`.
  - 0x00A3FF60: `~_List_base<CSoundObjectLH*>`.
  - 0x00A40BD0: `~vector<pair<pair<CCharString,CCharString>,CSoundBankLH*>>`.
  - 0x009C2F50: `~vector<CQuickDrawTriInfo>`, using the std block of ~CRenderManager2D 0x009C2FD0.
  - 0x009AA780: `_Rb_tree<...map<CCharString,NBankFileManager::CContainedBank>...>::_M_erase`. The placeholders were replaced; the
    STLport text is kept.
- **own 14**: own-row respells.
  - IsAcceptButton const; CTCOpinionSource(CThing&); GetTotalTimeWorldFrames / GetFramesRunning / PeekDefinitionManager; CAIStateGroupBase::GetWorld.
  - CInitBaseClass::IsValid (and 0x004175E5's call); CIndexBuffer::Uninitialise; CSoundBankLH::Unload_00a0b6c0 (descriptive).
  - ClearMemoryBlocks private; CEngineLocalDetailGenerator::DeleteMap; SetScriptActiveStatus (+ ScriptActiveStatuses).
  - 0x00887A30: SetBaseWeight becomes SetDefaultThemeWeight (c669 README).
- **data 4**: data symbols.
  - 0x004CD670: the static members ConversationAttitudeToAnimName / ConversationAttitudeToAnimGroupName.
  - 0x004CD9B0: re-authored as GetAnimName. **The catalog label GetAnimGroupName is wrong**: the function reads 0x013B8A38; its twin
    0x004CD9C0 reads 0x013B8A44.
  - 0x01224C6D: the ScriptActiveStatuses map initializer.
  - 0x01228ED8: the CrimeText TU _$E2.
- **sp 33**: the CStringParser ctor callback becomes `P6IX0@Z` (`??0CStringParser@@QAE@ABVCCharString@@P6IX0@Z0J@Z`) in every caller.
- **queue 22**: caller respells (fam_q) for 15 targets.
- **leaf 4**: Set{TL,TR,BL,BR}Node protected (IAE).
- **w15d 2**: wave 15 DEFERRED 0x00634870 (C2011) and its caller 0x00487BD0.
  - The CPlayer_Methods stand-in becomes CPlayer on the layout header, through the landed `#define X X_Layout` precedent (0x00830150).
  - AddMode takes EPlayerMode.

## Queue outcomes (QUEUE_W16.tsv, 184 rows)

134 respelled, 12 respelled-partial, 16 deferred, 22 already-correct. Each row carries rows_edited / rows_left / detail.

- 0x00440730 (HOLD) is not in the queue input and was not touched.
- The GFEnterAllocatorLock / GFLeaveAllocatorLock / CMemoryAllocator::GetAllocationCount labels already exist in
  `ghidra_out/labels_chunk674_2026-10-27.tsv`; there is nothing to do.

## Deferred (DEFERRED.tsv)

- The queue rows that are deferred or partial, with reasons.
- The 0x009AA780 sibling placeholder rows (follow-up).
- Wave 15's DEFERRED rows, carried unchanged except 00634870, which this wave resolved.

Scope 2 (the bo / scb / bcn / bip / cmb / sai remainders and 00861d00) and scope 3 (the mega families) were not reached beyond 00634870.
