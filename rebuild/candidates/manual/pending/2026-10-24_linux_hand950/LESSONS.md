# Chunk 950 -- lessons from the authoring agent (fold into GOTCHAS at landing)

- **Conditional new-expression temporary** (`AddSpawnedFunction(new CSpawnedFunc<T>("Main", ...), "")`, 0x00CDD380):
  - The CCharString temporary in the new-expression's argument is built only when new succeeds. VC7.1 keeps an ebx flag and destroys the
    temporary under `test bl,1`.
  - A second string (the region name) must come from an inline helper called after the new-expression. As a direct argument it is built
    first.
- **Counted-pointer destructor in CreateActiveEntityScript** (0x00CF66E0): `CCountedPointer<T>::~CCountedPointer` needs a same-TU `noinline`
  body. A bare declaration swaps two frame slots and drops the ebp/ebx copies used by the later ShareData pushes. The
  `CActiveEntityScript<T>` constructors must be `__forceinline`.
- **Script constructor** (0x00CDD330): the intermediate CLevelScriptBase vtable store is dropped as dead. The inline base ctor loads its
  second argument first and stores +0x40 before +0x44.
- **Chunk getters** (0x00A98FA0 / 0x00A99030):
  - `chunk; chunk.SetStaticCast(FindXSubChunkOfType(...)); return chunk;` matches, with no NRVO.
  - The counted-pointer destructor's zero stores are dropped as dead, except on a temporary whose release is followed by another call.
- **STLport `_M_insert_overflow(__true_type)`** for 4-byte PODs (0x00436AC0 / 0x004408A0):
  - The body matches verbatim.
  - F005 and F007 are one source, split only by int3 padding.
  - A float fill copies through esi like any dword.
- **STLport `_M_insert_overflow(__false_type)`, 8-byte element, all inline** (0x004D1250): the body matches verbatim, with no EH because
  `_STLP_TRY` is empty.
- **The same `__false_type` insert with out-of-line `__fastcall` range helpers** (0x00449050): every empty tag temporary lands on the
  `atend` argument slot. That needs same-TU noinline helper bodies; with declarations only, the first tag gets its own frame byte.
- **AddEngineComponent** (0x00B29930): the progress text must be a named CCharString local in its own block. The STLport inline push_back
  homes the derived-to-base temporary in the dead `component` slot and the `__true_type` tag in the dead `flag` slot.
- **Lazily allocated STLport list member** (`if (!l) l = new std::list<X>; l->push_back(x);`, 0x00532D00): the dead
  `push ecx ; mov [esp+8],0` is the default-allocator temporary's lifetime slot. It appears only when `allocator` has its user
  `~allocator() {}`.
- **`_Rb_tree<CCharString,...>::_M_find`** (0x004B9370):
  - Put the real `CBasicString<char>::operator<` body (landed 0x00429950) under `auto_inline(off)`.
  - Add the inline CCharString::operator<: equal pointers -> false, null this -> true, null other -> false.
  - map and multimap share one `_Rb_tree` instantiation.
- **`__copy_backward_ptrs(..., const __false_type&)`** (0x00527CA0):
  - It is `__fastcall` `ret 8` with `__copy_backward(..., random_access_iterator_tag(), (int*)0)` inlined.
  - Any trivially assignable 8-byte element is block-copied as two dwords through edx, whatever its member types.
  - The `ret 4` variant is `copy_backward` -> `__copy_backward_aux(__true_type)` -> `__copy_backward_ptrs(__false_type)`, all inlined
    (0x00B855C0).
- **Entity-script `Alloc` factories** (0x00D1FC30): the natural PDB ctor matches, with `CEntityScriptBase(*parent.Game, me)` and
  `ParentClass(&parent)` in the init list and `PMasterData = data` in the body. Its store follows the member ctor call. No "root class"
  trick is needed.
- **Family-pass identity:**
  - For script rows, go from the vtable slot to the Alloc, then to the `CScriptInfoManager::RegisterScripts` (0x00CD52D0) name, then to
    `NScript::C<name>Script`. This is proven against the PDB name sets for all 324 script rows of F001-F003.
  - For STLport template rows, prove the element from the landed callers' container types.
  - xref hits inside a call instruction are false positives. Find factory-pointer binding sites by a raw dword search over .text.
- **gall.sh in family passes:** it uses `$S/tools/q.py`, which lacks the chunk oracle, so it prints nothing (its grep hides `no oracle`).
  Grade through the agent's own q.py (`gen/grade1.sh`).

Review at landing:
- **Rows over 4 grades:** none. The maximum was 4 (0x00CDD380, 0x00CF66E0). Grade counts are in the STATUS.tsv notes, plus one
  confirmation regrade per row with an unchanged source (`gen/final_grades.txt`).
- **Local models next to existing engine headers.** All sources are self-contained, with no #include, so they can be retyped with
  `retype_landed.py`:
  - F004/F006 model the C3DMeshFile*/C3DAnimFile* chunk classes and CChunkedFileSuperChunk locally.
  - F009 models CEngine locally, because engine/CEngine.h puts EngineComponents at +0x178 while retail has it at +0x168 (Ego_r CArray is
    16 bytes and the TLC STLport vector is 12).
  - F001 and F003 model CSpawnedFuncBase and CScriptBase locally; engine/CSpawnedFuncBase.h flattens the vptr.
  - F014 models CScriptThing locally next to engine/CScriptThing.h.
- **Reloc identity:** the catalog binds `??_7?$CSpawnedFunc@VCV_ExposeMayorScript@NScript@@@@6B@` to 0x012ECBA8 from landed 0x00E95F10.
  That vtable is CV_HauntedHouseScript's: it is stored by 0x00E93DA0, the RegisterMain of the "V_HauntedHouse" script vtable 0x012ECB94.
  Our 0x00E756D0 binds the name to the real ExposeMayor vtable 0x012E96E0. Rename landed 0x00E95F10 to
  `CSpawnedFunc<NScript::CV_HauntedHouseScript>` (and its "CV_ExposeMayorScript::DoMission 0x00E953E0" callee) before landing this chunk,
  or the lander will flag a double binding.
- **Catalog error:** reloc_bindings.tsv binds `??0CScriptThing@@QAE@XZ` to 0x008CFF40, a null-script-thing getter. The binding comes from
  landed 0x00892220 "EntityGetStandingOnThing", and 0x008CFF40 is itself landed as a CIsThingAlive predicate operator. The real default ctor is 0x006E7B40, which F014 calls through the stand-in `CMember_006e7b40`.
  When the catalog is fixed, rename the stand-in.
- **Already-landed rows with wrong names or types:**
  - 0x006B94C0, 0x004BAA30, 0x009A8090, 0x00A4EAC0, 0x00A4EAE0, 0x007ADB30, 0x00756D00, 0x00755640, 0x009AB120, 0x009AAF70, 0x009A7F80,
    0x009CCDC0, 0x007AE3D0 (F012 callers).
  - 0x00518850 vs 0x005198B0 make conflicting vector<CThing*> claims, and `CThingMask47` is invented.
  - The 557 `Create_CVtable_XXXX` rows are `NScript::<Script>::<Entity>::Alloc`.
  - 0x00DA34B0, 0x00F024C0 and 0x00DCC770 have the wrong script `::Main`.
  - 0x006E7B40 `CCarriedReadableDef::CCarriedReadableDef` is `CScriptThing::CScriptThing()`.
  - Details are in NOTES.md Label corrections.
- **Descriptive identities** (49 rows; listed in NOTES.md):
  - Element or pointee types: F005 17, F007 14, F008 12, F011 3, F013 2.
  - One F014 entity class: 0x00DCDE20 CGuardTeamMember, which has no PDB entry.
- **Catalog rows holding several functions:** none found. No row was skipped.
