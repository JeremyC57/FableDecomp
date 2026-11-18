# NF69 notes (round 7, Ego_d entries q10819..q10858, tag egod1117)

## Outcome
- 40 entries: 39 done, 1 rejected (q10855), 0 blocked.
- 33 rows in rows.txt: 31 ported, 2 not-a-binder (0x004E1AAF, 0x0047EEE4). Two extra rows claimed and ported: 0x004E0064 and
  0x007F5D00 (binders of 0x004E0080 / 0x004DA8B7 under stand-ins). All 33 changed rows: RELOCATION_MATCH + test PASS in one vland
  run (recheck_final.log).
- Header opt-ins: six overlays (hdr/README.md). No default-path change is needed.

## By group
1. **Hit locations.**
   - The static is declared by engine/NHitLocations_CHitLocation.h's FABLE_RETAIL_HITLOCATIONS_BOUND branch (private, the exact
     Ego_d type). The init / atexit pair defines it on fable_map.h, as the landed Gossip pair 0x0121F160 / 0x01229CA0 does.
   - StatRegisterClass 0x008162F0 and ProcessHitLocationCode 0x00815E80 are members on that header.
   - HLCAutoKill 0x008F5CF0 is re-authored as the free `__fastcall` codelet.
   - The tree rows are explicit instantiations on stlport/fable_map.h (the 0x005E9100 / 0x005685E0 forms).
2. **Combat sequences.**
   - CCombatSequenceBase has no live shared header: engine/CCombatSequenceBase.h is quarantined, and the landed rows 0x008B0B60 /
     0x006F1770 model it locally. My rows declare it locally with only what each needs: the three protected static maps with Ego_d's
     typedefs, and StatRegisterClass. The readers also get the virtuals they override and FableWin.pdb's protected GetCreature /
     GetTarget.
   - CCombatSequenceDef is also quarantined: the readers model only its three module types (+0x48 / +0x4C / +0x50).
   - 0x008B0D10 now sits on engine/CCombatManager.h (it modelled CCombatManager / CCharString locally; the header exists).
3. **Targeting destructors.**
   - 0x004DA8B7 is ~CTCTargetingBase. q10855 is rejected (REJECTED.tsv); its catalog name moves in RENAMES section 1.
   - 0x004E0080 is ~CTCTargetingAI: the Ego_r layout matches and its only caller is the CTCTargetingAI ??_G. RENAMES section 1.
   - All three are VC7.1 implicit destructors (no vptr store), as is the landed ~CTCTargeted 0x004DBF56. The overlay opt-ins drop the
     PDB-generated user declarations, and DestroyOne emission helpers (ungraded, as in 0x004DBF56) make VC7.1 emit them out of line.
   - The four ??_G rows (0x004E37A3, 0x004E1023, 0x004E0064, 0x007F5D00) keep their landed free-function self leaves. Only their
     callees moved onto the headers.
     - **Open authoring:** emitting the compiler's `??_G<Class>@@UAEPAXI@Z` needs a TU that emits the class's vtable, i.e. its
       constructor. Not attempted.
   - 0x004E1023 writes ~CTCTargetingSpirit's member and base destruction out by name. A free function does not expand the implicit
     destructor the way retail's compiler-generated ??_G does: calling it by name gave 28 v 38 bytes.
   - 0x007F5D00 is not a size TU: there, the implicit ~CTCTargetingBase expands (88 v 30 bytes). It therefore takes the header's
     declared destructor (no IMPLICIT_DTOR opt-in).
4. **0x00CD3A16** is ported fully onto fable_map.h + fable_string.h as `map<int, CCharString>::operator[]`.
   - It binds the landed int insert 0x00CD36C2 exactly (iterator form included).
   - Its other two callees, 0x00CD20CE (pair) and 0x00CD28F4 (_M_lower_bound), still have long self names: NAME_FIXES.tsv.
   - It uses the existing opt-in FABLE_RETAIL_PASS22_LIP_SYNC_SAMPLE_READ only for STLport's `__default_constructed` operator[]
     text (the macro name is historical).
5. **0x009C8890** is ported onto engine/NGameText_CDataBank.h with FABLE_RETAIL_NGAMETEXT_GETPGROUPENTRY.
   - CGroupEntry : CResource, as EGOD_FINDINGS section 5 settles.
   - CDataBank's second base CResourceBank is from RTTI.
   - The callees now carry both PDBs' spellings: AsyncUpdatePending, BeginLoadingGroup, and the private const BeginLoading /
     BuildGroup. They were the made-up FlushAsyncUpdates / QueueGroupLoad and public non-const forms.
   - The row also defines CDataBank::BeginLoadingGroup (0x009C8220's function, as in its source file). VC7.1 then calls it on the
     not-loaded path and expands it on the first-load path, as retail does. The row object therefore also carries a
     BeginLoadingGroup body; graded on its own against 0x009C8220 (leaf ?BeginLoadingGroup@CDataBank@NGameText@@QBEXK@Z) it is
     RELOCATION_MATCH, so the helper is that row's exact body.
   - 0x0047EEE4 is not-a-binder: its 0x009C8890 binding is right.
     - **Open authoring:** its whole-row local models (CWideString, CCharString, NGameText::CGroupEntry : CRefCounted,
       NGameText::CDataBank, NUISystem::CManager, CIVCountedPointer) need porting onto their headers.

## Caveats
- The reader rows' local CCombatSequenceBase declares only the virtuals they use (destructor, IsValid, OnStart, OnStop). The declaration
  order is not the vtable order (slots 0 / 12 / 17 / 18); no vtable is emitted in those TUs.
- The four ??_G rows (0x004E37A3, 0x004E1023, 0x004E0064, 0x007F5D00) keep their stand-in free-function self leaves; only the callees
  were ported (see group 3).
- RENAMES section 1 `old_leaf`: for rows without a full `// Leaf:` line, the old leaf was derived from the old source text (class,
  method, signature), not read from an object; rows the old source graded by catalog name say so.
- RENAMES section 2 holds symbol pairs from rebuild/oracles/reloc_bindings.tsv (old) and the new objects (new). Callee stand-ins that
  file did not record are `#` comment lines.
- Binder completeness: absrefs.py over the four maps (+0 / +4) and callers.py over every section-1 address list only my rows, plus
  three landed callers that already bind the right spelling: 0x004EE23F (StatRegisterClass), 0x006ED3F0 (RegisterCombatSequences AAE)
  and 0x00CBFB7D (the int operator[]). No new self leaf is bound at another address (reloc_bindings.tsv grep).

## Descriptive names (not PDB spellings)
- Descriptive-by-library: every STL spelling. The tree rows (_M_insert / insert_unique) and the NAME_FIXES entries for _M_erase,
  operator--, the member-template find, ~vector / ~list, _M_lower_bound and the pair constructor are spelled from STLport 4.6
  (stlport/fable_map.h, fable_containers.h). Ego_d's VC10 Dinkumware internals do not carry over. The four map types themselves are
  Ego_d's and mangle the same.
- Enum values: EHitLocationCodelet / ECombatSequence*Type values are written as casts. Ego_d names the enumerators but no shared header
  defines them, and the findings quote them abbreviated.
- Parameter names (thing, hit_parameters, creature, target, special_sequences, allow_pending) are descriptive.
- Left as found (not mine, not proven here):
  - the CreateSeq* factory spellings bound by 0x008B0D10; the catalog calls them `_global::Alloc_CCombatSequence_*`;
  - the landed binding name RegisterCombatSequence 0x006EAB60, which is in neither PDB name set.

## Checks
- **Funclets:** no 01/2x Unwind row includes any changed row's file (grep of each landed source file name).
- **Warnings in recheck_final.log:**
  - RELOC_NAME and STANDIN_LEAF are catalog names that the RENAMES / NAME_FIXES above correct.
  - RELOC_CONFLICT `_$E2` is the TU-local atexit symbol, which every static-init TU reuses.
