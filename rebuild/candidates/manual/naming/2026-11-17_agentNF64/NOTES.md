# NF64 notes (round 7, slice land1117fu, q10637..q10723)

The per-row and per-entry outcomes are in STATUS.tsv and APPLIED.tsv. RENAMES.tsv covers rows/ only:

- Section 1 lists self renames.
- Section 2 lists binder respellings, taken from before/after compiles in work/bo and work/bn.

The rows in work/ready/ are complete, graded (RELOCATION_MATCH) and in recheck_final.log. They are held because landing one alone
would newly split a landed caller that is outside rows.txt. Each must land as a joint set (below), with the caller fix that
NAME_FIXES.tsv names.

## Joint-landing sets (work/ready)

- **0x0045C3CF, 0x0045C79E, 0x0045DD2C, 0x004E99AE, 0x004EC219, 0x004EC33E** (group A tree operator=). Land each with these
  callers respelled to the `??4?$_Rb_tree@...` form:
  - 0x0045C2E8;
  - 0x00ADC0D0 and 0x00ADC120;
  - 0x0067CE50;
  - 0x004EC174.
- **0x00868CA0 (NF61's re-author) + 0x007E2600.** 0x007E2600 is respelled here: it takes an EDamageAttribute parameter in its
  own pre-existing CHitParameters model, as DAMAGE_LIGHTNING. The PDB self spelling would newly split 0x00798770, 0x0080CA20 and
  0x00838BC0 (each binds `...QAEXJM...`).
- **0x00A90F00.** The self-signature rename would split 0x00A8B820.
- **0x00694020 + 0x008D5000** (q10720, `?FrameUpdate@CCreatureActionBase@@MAEXXZ`). The QAE callers 0x0069FCE0, 0x00859D80 and
  0x00848AE0 must be respelled to MAE. 0x00849BE0, 0x008426E0, 0x00846BC0 and 0x00854B90 already bind MAE.
- **0x008D5000 itself** was an extern "C" `@FrameUpdate@4` stand-in. Re-authored on engine/CCreatureAction_FireMissileWeapon.h, it
  defines `?FrameUpdate@CCreatureAction_FireMissileWeapon@@UAEXXZ` (Ego_r.pdb 0x0090BCF0 / FableWin.pdb 0x02ADAB00). Its behaviour
  test was rewritten on the same headers (work/ready/008d5000_test.cpp, PASS). This self rename has no NAME_FIXES line, and no
  caller or vtable-slot check was run for 0x008D5000 itself (it is a virtual, reached through the vtable). Check that before landing.
- **0x0058148D** (CTCHaste::UnsetActionSpeedMultiplier, RM). This row is held for its test, not for a split. The row object now
  carries the visible STLport erase(const key&) / erase(first, last) COMDATs:
  - stubbing erase(const key&) in the test collides with them (LNK2005);
  - leaving the stub out needs a real map<void*,float> in the test, and no header opt-in provides those bodies.
  The rewritten test is in work/ready/0058148d_test.cpp (it still fails to link).
- **0x00467831** (vector<bool>::_M_insert_overflow, STLport spelling). Land with 0x004676EF respelled. 0x0053CB4F already binds
  the new spelling.
- **0x004E9021** (CPersistTraits<CWeaponTrailGraphicSet>::TransferBinaryIn). Land with its rows.txt caller 0x004E8FCD. That
  caller is blocked: it needs a CPersistTraits<CWeaponTrailGraphicSet> traits-object opt-in (its 8-byte `this` is set to -1,
  -1). 0x004E9156 already binds the new spelling.

## Judgement calls

1. **Tree vs class operator= (group A, 27 rows).** Each row is named `std::_Rb_tree<...>::operator=`, not `std::map<...>::operator=`.
   - Four landed callers (0x006303C0, 0x00465C54, 0x004321F0, 0x00631720) bind the tree form, and seven landed self rows use it.
   - The bodies are the STLport `_tree.c` operator=, called from `map::operator=` (inline).
   - 0x0046230E and 0x004621E9 bind real class-level `operator=` names from the PDB (CHeroAbilityStatWeight). NAME_FIXES records
     the 0x0046230E line as a JUDGEMENT CALL.
2. **q10720 rejected.** The queued QAE spelling is wrong. Both Ego_r.pdb (0x0051A410) and FableWin.pdb (0x01DB65E0) give
   `?FrameUpdate@CCreatureActionBase@@MAEXXZ`.
   - The MAE fix is prepared in work/ready.
   - 0x00694020 was first marked not-a-binder from the classify1 probe; that was corrected to blocked/ready.
3. **Local-model edits (no new model).** These rows got access-only or type-only edits inside pre-existing local models; the NF59
   precedent accepts this.
   - 0x0068B7C0: button queries moved onto the local base as protected const.
   - 0x00745730 / 0x00745C70: private UpdatePerception / UpdateHearing / UpdateDistanceFromPerceivedThing.
   - 0x00A792E0: ClearNodes made private.
   - 0x00464ACE and 0x00467831: `class allocator` as the second argument; `_M_insert_overflow` protected.
   - 0x007E2600: EDamageAttribute parameter.
   - 0x00979E90: anonymous-namespace `const CThingCreatureBase&` callee.
4. **0x00403EB0** is a COMDAT. It is re-authored on the CPersistTraits.h `FABLE_RETAIL_CPERSISTTRAITS_BOOL_CLASS` spelling
   (`value = is.ReadEBOOL()`); the link takes the same instance from its other TUs.
5. **Clear self rows with stand-in names.** The tree clear rows that group A's operator= calls (`_global::RbTree_Clear_*`) keep
   their landed names. They are callees, not entry addresses in this slice.
6. **Resize group (q10639).**
   - Every binder of 0x004428F0 was checked against Ego_r.pdb: each resized member is vector<long> or CArray<long>.
   - 0x009FBBD0's argument is CHistogram<long>; its Data member is CArray<long>, and Ego_r.pdb 0x00B6BBA0 / FableWin.pdb
     0x030DDEB0 give `?CalcAlphaHistogram@CTexture@@QBEXAAV?$CHistogram@J@@@Z`. Its self leaf was corrected with it (RENAMES s1;
     no landed caller).
   - The claim rests on that per-member type check. A separate landed vector<unsigned long>::resize (0x00464931) does not rule out
     ICF folding into 0x004428F0: it is a different body variant (ERASE_RANGE_OUTOFLINE).
   - 0x00771DE0's out-of-line `clear()` now binds `?clear@?$vector@J...@QAEXXZ` at 0x00485D1A. That address is landed under the
     placeholder `CClass_00485d1a::ApplyPair_00485d1a`, and the row already bound a non-matching CLongVector::clear. This is not
     sent as a NAME_FIXES line: there is only one source (body shape: `push [ecx+4]; push [ecx]; call erase`).
7. **CTCHaste rows (q10667 / q10668).** The type-0x8C component in 0x0078B640 and 0x008C7480 is CTCHaste:
   - Its Multipliers member is a std::map<void*,float> in Ego_r.pdb.
   - The erase + update pair is the inlined `UnsetActionSpeedMultiplier` (NF61's `_UNSET_INLINE` opt-in).
   - Both rows were ported. The other binders, 0x00694430 and 0x0079D2D0, are outside rows.txt (NAME_FIXES).
   - Their landed behaviour tests (rebuild/tests/.../*_008c7480_test.cpp, *_0078b640_test.cpp) are self-contained: each carries its
     own copy of FinishAction. Their PASS therefore does not exercise the re-authored row objects, which were verified by grade
     (RELOCATION_MATCH) and binding only. This is the same object shape that makes 0x0058148D's row-linked test fail.
8. **Entries whose self row was already right but whose queued wrong binders are outside rows.txt** are recorded as blocked, with
   NAME_FIXES lines from the queued evidence (these binders were not re-probed): q10653, q10661, q10663, q10664, q10674.
9. **ICF questions left for the coordinator.**
   - 0x004E37BF: CGlobalDef's members are vector<CCharString> / vector<CDialogueLayerDef>, but the row calls the address named
     ~vector<CTargetPoint>.
   - 0x00CD3A16: an operator[] of map<long,CCharString> calling the insert named map<int,CCharString>.
