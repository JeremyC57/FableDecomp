# agentFAM09 notes: the `_static_init::g_<Type>_<addr>` family (399 rows)

## Result
- 307 of 399 rows were renamed to their PDB object names. The other 92 were left as they are (see APPLIED.tsv and REJECTED.tsv for each reason and candidate name).
- 41 landed binder rows and 15 tests that used the old placeholder were respelled.
- Every changed row grades RELOCATION_MATCH and every test passes. No row used more than 3 grades. STATUS.tsv has the per-row results.

## How the names were proven
- **Ego_r.pdb names.** Ego_r.pdb `dynamic initializer for '<name>'` (ghidra_out/egor_pdb_names.tsv) follows retail's .CRT$XCU order. The table runs from 0x01374208 to 0x013749AC, and the initializers run from 0x01219E90 to 0x01224CE0 in the same order.
- **FableWin.pdb names.** FableWin.pdb (_fablewin_api.tsv) gives the second source. A TU keeps its internal order in both builds. The link order of TUs differs between builds.
- **Matching inside a TU.** Each run of rows was matched against the PDB run for the same TU. The literal the initializer passes (or the default ctor), the neighbouring named rows, and the object sizes confirm the match. Each row's `// Names (agentFAM09)` comment cites the donor addresses and the group evidence.
- **Anniv360 Test PDB (third source).** It is cited only where it agrees. The public mangled name gives access and const. S_LDATA32 gives internal-linkage objects.

## Conventions used
- **Leaf.** The real VC7.1 object-file leaf of each initializer is `_$E1`, so every renamed row carries `// Leaf: _$E1` and was graded with it.
  - The RENAMES.tsv `mangled_leaf` column follows the landed ST_HEAD_TURN_H convention: `??__E<name parts>@@YAXXZ`. Anonymous-namespace objects get no `?A0x` hash, because the hash depends on the TU.
- **Constness of anonymous-namespace class objects can't be proven.** The Anniv debug types drop const on class types. The proof: ST_HEAD_TURN_H shows "CCharString", yet its mangled name is `@@B`.
  - The rule used: const where an existing landed binding already spells `@@B`. That covers the weapons description strings, the sign posts, and SDoorOpenAnim / SDoorCloseAnim, which keep their landed const spelling.
  - Otherwise non-const. Scalar consts (long and float) are const by the Anniv types.
- **Shared names across rows.** Several names are header-defined internal-linkage constants, so many rows share one name and each TU has its own copy:
  - WATER_MESH_VERT_SPAN_SINE and WATER_MESH_VERT_SPAN_COSINE (one pair per water TU)
  - REACTION_MANAGER_DEF_INSTANTIATION_NAME (32 copies)
  - MIN_ / RANGE_TIME_TO_LEAVE_BETWEEN_SITTINGS and TIME_TO_LEAVE_SHOUT_MESSAGE (2 each)
  - Each copy is still one address with one name. I propose file names `staticinit_<NAME>_<addr>.cpp`.
- **Binders for objects in another TU.** These were respelled as follows:
  - anonymous-namespace objects: `namespace { extern [const] T NAME; }` (the existing NEWLINE pattern)
  - globals: `extern [const] T NAME;`
  - NCombatActionHelpers::WeaponPosLeft/RightFootHelperName: `namespace NCombatActionHelpers { extern ... }`
  - GNullName: `extern CCharString GNullName;`
  - The last two are file statics in retail, so these extern declarations approximate the retail code. The bytes are unaffected.
- **LoadAllSectionName binders.** 0x00521240 and 0x005223F0 keep their existing local CLevelLoader model. I only replaced the placeholder extern with the header's `static CCharString LoadAllSectionName` plus the inline accessor, so no new local model was added.

## Left as is / follow-ups
- **0x0121E1F0 binder not respelled.** CTCWife::MarriageEnabledMaleVoice was renamed. Its binder 0x008330D0 (CThingAICreature::AutoSetupSexuality) still says g_CCharString_013bab2c.
  - That row has its own local CCharString model, so it cannot include engine/CTCWife.h without a header migration. A local CTCWife model would be a new local model, which the rules forbid.
- **Comment-only mentions, not changed:**
  - 0x00520D00 is BUSY and only mentions the old name in a comment.
  - 0x00761C00 only mentions the old name in a comment.
- **Atexit destructor thunks (`_$E2` rows).** These rows still name these objects descriptively and are outside this family:
  - `_global::AtexitDtor_CCharString_at_<obj>_<addr>` labels
  - `g_AutoTinyThunkObject_<obj>` binders (289 files)
  - With these names landed, the thunks can bind the real names. I respelled only 0x012299E0, 0x012299F0 and 0x01229A00 here, because they used family placeholders.
- **Access of CScriptThing::NullString (0x0143E8EC).** Three sources disagree:
  - engine/CScriptThing.h declares it public by default (`@@2`). The landed callers bind that.
  - Under FABLE_RETAIL_CSCRIPTTHING_ORIGINAL_ACCESS it is private.
  - The Anniv publics say protected (`@@1`).
  - The row uses the header as it is. Someone should settle this in the header.
- **0x0121F7E0.** It is landed as `CTCCreatureModeManager::CModeBase::$E_NO_FRAMES_BEFORE_PLAYING_IDLE_ANIMS`. It could use the new FABLE_RETAIL_CMODEBASE_NOFRAMES opt-in.
- **NAME_FIXES.tsv: 97 other spellings of the now-named objects** in landed binders. Some are invented descriptive names; others are wrong.
  - Largest: g_EmptyWideString / g_emptyWideString / Empty_013bca24, which are CWideString::NullString, in about 50 files.
  - g_EmptyString / g_EmptyCharString are CScriptThing::NullString, in about 47 files.
  - Also the 13-file g_DayOnlySuffix / g_NightOnlySuffix.
  - ComboStrikes::* are really the STRIKE_EVENT_* globals.
  - CAFile::CurrentPath is really NFile::CurrentPath: Ego_r `NFile::dynamic initializer for 'CurrentPath'` plus Anniv `?CurrentPath@NFile@@3VCWideString@@A`.
  - g_def_004aa8b0, kDefaultMapName, kHomeMapName and g_7332d0 bind the same address but may be unrelated. Check them first.
  - The coordinator should respell these; they are outside this placeholder family.
- **Possible mislabels in other families** (for the coordinator):
  - 0x0121A670 is landed as CThing::TCsToBeRemoved but is a tree. Ego_r order and size make it NThingManager::UIDsToReplaceWithUID. The real CThing::TCsToBeRemoved (a list) is 0x0121A440, left in this family because its type is not modelled.
  - The `_global::$E_InventoryOrientationRHSet2/3/9/10/11` rows are the anonymous-namespace RIGHT_ / LEFT_ / BACKWARDS_*_FORTY_FIVE_DEGREES_ORIENTATION initializers.
- **Left rows (92).** Their candidate names are in APPLIED.tsv:
  - atexit-only CStatic rows: the object's type is not modelled. Most are singleton PClass pointers, CCountedPointers or arrays.
  - list and tree rows: element types and the owning class statics are not modelled.
  - object rows: CCRCSymbolMap / CSymbolMap / CShaderStats / CShaderRenderManager / CThreadedFileManager / CBankFileManager are built through stand-in classes. Rule 5 says rename these only with a proven class and a shared header.
  - libpk objects: they have no PDB name.
  - three scalars and two strings: their names are not proven.
