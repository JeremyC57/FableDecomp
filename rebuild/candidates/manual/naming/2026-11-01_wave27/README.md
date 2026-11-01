# Naming pass wave 27 (2026-11-01): 40 landed rows respelled + 3 header dependents rechecked -- NScript entity Main nested and private virtual, CFileDataOutputStream retail layout, private CPixelShader, access-only Leafs, two-target vector re-models, CSymbolMap::UpdateDirty binders

Inputs: brief `naming/briefs/BRIEF_W27.md`, QUEUE_W27_input.tsv (39 rows: wave 26 deferred / partial rows plus the 22 c695 rows), the
DECISIONS_W26_RESOLVED approvals, wave 26 DEFERRED.tsv. One Opus 5.5 agent authored the overlay in `$S/naming/w27` (AGENT_README.md; scripts/ here;
own recheck `logs/recheck_final.log`, 43 rows). The coordinator copied ov_final (41 sources / tests and engine/CFileDataOutputStream.h), applied
the 8 RENAMES (git mv + build_candidates.ps1), BINDINGS (32 RETIRE / 16 ADD / 10 KEEP, 0 CONFLICT; the table equals the agent's after-table) and
LABELS (18), and rechecked the 40 changed rows plus the header's three unchanged includers 0x00994400 / 0x00996F30 / 0x009CDF50
(`logs/coordinator_recheck.log`; `scripts/coordinator_mk_rc_list.py` builds a recheck payload from a row list). `bindings_multi_check.py` 0.

Families, committed in this order (`runs/<fam>.rows.txt`): main 7 (each NScript entity nested in its script class with Main private virtual
(EAE); the local CEntityScriptBase takes vtable 0x012C3224's slot list), fdosw 5 (opt-in `FABLE_RETAIL_FILE_DATA_OUTPUT_STREAM` layout of
engine/CFileDataOutputStream.h, vtable 0x0129A6F8; 0x00994480 is the virtual Write; private CreateRetailBank / Save), pxs 3 (private CPixelShader
ctor and CreatePixelShader through friend lines), acc 5 (access-only Leafs: GetTattooNames QAE, UpdateBlockedActionPoints IAE, FindTeleports AAE),
dm 4 (CDefinitionManager's protected members on their PDB parameter types; the invented ?Finalise binders now call CSymbolMap::UpdateDirty),
virt 2 (GetPossibleCollisionSet MBE; LoadTopologyFromFile UAE without a _vptr member), vl2 5 (STLport vector<T, allocator<T> > with CArray<T> where
the PDB says CArray; each second target on its bound spelling), pb 2 (0x004128A0 is the STLport vector<CWideString>::push_back; GetProfileNames
IAE), sym 3 (UpdateDirty() const; CSymbolMap::DeallocateUnusedMemory), c695 4 (~CScriptModuleFOVZoom qualified, the virtual
NInventory::CTCInventoryBase::AddCloseInventoryEvent, protected RemoveInputProcess, ~_List_base<CCountedPointer<CTCScriptedControl::CActionBase>>
-- the compiler emits the queued spelling exactly).

Queue outcomes (QUEUE_W27.tsv): 6 respelled, 3 partial, 7 done, 22 deferred, 1 hold. Decisions: `DECISIONS_W27_RESOLVED.tsv` -- the two friend
lines ratified (PDB access + retail caller; confirm the LF_FRIENDCLS records on the Windows lane), the opt-in header contract accepted, 0x004091C0
deferred until its second targets are proven, the 0x007345D0 / CTCCombat family and the CTCScriptedControl::CActionBase nesting approved for wave
28, 0x005359A0 / 0x006BC4A0 held, 0x006A5230 deferred. Scope 3 (ccsns, 009994a0 / 00999620, c690 sort rows, DEFERRED_W26 families) not reached.
