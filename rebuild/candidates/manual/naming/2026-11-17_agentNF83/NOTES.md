# NF83 notes (round 7, slice q11010..q11043 without q11015 / q11016)

Totals: 88 rows (43 ported, 13 not-a-binder, 32 blocked, 0 deferred). 32 entries (14 done, 1 done-already, 17 blocked).
Each row's evidence is in STATUS.tsv; each entry's is in APPLIED.tsv. Every ported row is RM/MATCH + vland PASS
(recheck_final.log).

## Descriptive names (not found in any PDB as a symbol for that address; marked per the 2026-11-10 rule)
- 0x00869900 / 0x006DC970: `std::map<...>::clear` (`_Rb_tree::clear` bodies, STLport 4.6). Descriptive by library: retail is STLport and both PDBs are Dinkumware builds.
- 0x0059AF53: `_Deque_iterator<...>::operator--`. The STLport spelling is descriptive by library and the iterator traits argument is unproven.
- All other STLport leaves landed here (`_M_decrement`, `_M_subtract`, `_M_pop_back_aux`, `push_back`, `~_Rb_tree`, the pair `??_G`) are descriptive by library in the same way.
- 0x00860130: `CBrainOutput::SetAsPerformAction`. The name is in Ego_d.pdb only (one source plus the body's behaviour). It is the best available name, but it has a single source.
- `g_bAggressiveModeAvailable` (used by 0x00487E90 / 0x00487EB0): a descriptive global name; no PDB names this global.

## In-place edits of pre-existing local models (NF79 precedent: access, const and parameter type only; no stand-in renamed to a real class name)
- 0x00BC61F0: the local CEnginePrimitiveManagerOcclusionBase / CEnginePrimitiveGroupManager constructors now take `CArray<CEngineScene*>&` (engine/CArray.h plus a `class CEngineScene;` forward declaration).
- 0x00B6E260: RenderBackgroundList was made private.
- 0x0093A540 / 0x0093A5C0 / 0x0093A8C0: the Bandit Idle / Eat / Drink members were made private and return bool.
- 0x00662210: respelt in place to the PDB parameter form `const CCreatureActionBase&` and const.

## Coordinator items
- COLLISION: the landed 0x006DC020 defines the same `??1?$_Rb_tree<EDamageAttribute,...>` leaf as 0x00451AC6 (NAME_FIXES.tsv). Retail has no identical-code folding, so one of the two names is wrong. 0x006DC020 has no caller, and no name for it is proven.
- 0x0085FF10 (outside my rows) binds the namespace-less `CModeSitBase::RemoveModeWithAction`; 0x008D7230 now defines the namespaced one.
- 0x0059B576 (outside my rows): its own `_M_push_back_aux_v` leaf is a stand-in. 0x0059B61C binds the STLport spelling.
- 0x01436E9C: Ghidra label fix only (q11032).
- 0x0059A238 has a companion `CFrontEndManager_Action_0059a238_match.cpp` (behaviour-complete, not byte-identical, found by fcheck). Only the graded `CFrontEndManager_Action_0059a238.cpp` was changed. The companion still spells the old `CCountedPointer` / `PClass` forms.
- 0x004585BD / 0x00789358 (outside my rows) still bind stand-ins for 0x00451AC6.
- The three `??_G` rows 0x006A0950 / 0x006A0970 / 0x006A0990 (q11030) need retail branches of the flattened SheatheItemToInventory_For* headers.

## Blocked groups (details in STATUS.tsv)
- q11021 / q11022 CIntelligentPointer: 0x007C6760 and 0x00693B30 hit the T* constructor's `static_cast<CBaseObject*>` with T incomplete. Seven namespaced STL helper rows need the stlport containers in place of local models. The other rows each need a shared header for the owning class.
- q11036: the five `??_G` binders of 0x00BEB550 need re-authoring as real `??_G` on engine/CEngineSubPrimitiveBase.h, and three sub-primitive classes have no header.
- q11041..q11043: the NF79 blockers stand: GetMesh DIFFER 747v755; CParticleProgram / animated-mesh signatures; renaming macros and an offset cast in 0x00B90270. 0x00BC61F0 ported.
- q11017: 0x009FE380 stayed DIFFER 181v181 on one ebx/ebp swap after 4 forms (work/009fe380_a.cpp, _b.cpp).

## Final recheck (recheck_final.log)
- All 43 rows: RM/MATCH + PASS.
- Every SHARED_HEADER_AVAILABLE warning comes from a local model that was already in the landed row. This pass added none.
- Every RELOC_NAME warning is either a stale catalog or Ghidra label or a stand-in that was already in the landed row. Ghidra labels to refresh: 0x01436EA0 / 0x01436E24 / 0x01436E9C / 0x01436E60 are the NEngineInternal GP* globals. Stand-ins to respell when those rows are next touched: 0x004A96F0 (CIntelligentPointer<CThing> ctor bound by 0x004DB2E7) and 0x00835620 (??0CBrainOutput).
- Landed binders outside my rows that still declare stand-in forms of 0x00662210 / 0x00668D90 / 0x0059A8F3 / 0x0059B61C are listed in NAME_FIXES.tsv. They were found by source inspection and not graded.
- The 0x0059A238 section-1 line is marked INTERIM: its leaf is still not the PDB one (q11014 blocked).
