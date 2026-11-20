# NF193 notes (round 10, remaining mixed names, first 50 in queue order)

## q4133 (0x0122A200 GlobalList atexit destructor; likewise 0x012288B0 gIME)
- Both landed GlobalMethodTailThunk stand-ins re-authored as the `_$E2` of their static's TU (the source of the landed initializer, Leaf
  `_$E2`), on shared headers: 0x0122A200 on engine/LSocket.h + stlport/fable_containers.h (repo opt-ins FABLE_RETAIL_LSOCKET_SEND_PASS23 /
  _THINK); 0x012288B0 on fable_ime.h (FABLE_RETAIL_CIME). Both RELOCATION_MATCH PASS (work/q4133_vland.log); initializers 0x0121F850 /
  0x01219E90 rechecked unchanged.
- Names: Ego_r.pdb 0x0101D810 / FableWin.pdb 0x04011B20 and Ego_r 0x0101B2F0 / FableWin 0x0400E790 (`dynamic atexit destructor for ...`),
  plus the `push <thunk> ; call atexit` in each initializer. Exe scan: the only referencer of each thunk is its initializer (binds the TU-local
  `_$E2`).
- Spelling counts: 0x013BB000 / 0x013B7CD8 lose the AutoTinyThunk stand-ins (shrink). 0x004051C0 moves onto ?Shutdown@CIME@NUISystem@@QAEXXZ,
  the self row's own Leaf (narrows). 0x008F9F50: ?Invoke_008f9f50@AutoTinyThunkTarget@@QAEXXZ replaced by the STLport spelling
  ??1?$_List_base@PAVLSocket... (descriptive-by-library, the catalog name); the address keeps 3 spellings (self `SortTreeRecursively`,
  ??1?$list from 0x0090AA40, ??1?$_List_base from 0x0122A200): count-neutral. NAME_FIXES has the wrong ones.
- Pick-any: the new 0x0122A200 object defines the _List_base / list / allocator COMDATs the HEAD 0x0121F850 object also defines
  (cmpbody: ~list 5 = 5, ~_List_base 62 = 62, clear 47 = 47, bytes and relocs equal). The ~_List_base copy grades RELOCATION_MATCH at
  0x008F9F50 itself (q.py with the row source and that leaf), so it is the owner's body. ??1?$list's table line (to 0x008F9F50, witness
  0x0090AA40) is a stand-in binder line; its body is a jmp thunk, not 0x008F9F50's body (no owner row: 0x008FA060 is not landed).
  0x012288B0 newly defines only ??0 / ??1CIME (inline, no landed owner row, no table line) and gIME.
- Pair debt: each static is now defined by two rows (initializer + atexit destructor): the known LNK2005, EXTRA_RULES 2026-11-20. The
  initializers landed earlier, so "both rows in the same landing" holds only if the coordinator lands the rechecked initializers with these
  (decision for the coordinator; DEBT lines in NAME_FIXES.tsv).
- File moves: both self rows move off global_GlobalMethodTailThunk*.cpp; no funclet row includes either file (grep of rebuild/src, rebuild/tests).
- 288 more GlobalMethodTailThunk rows exist in 01/22 (most are atexit thunks of other statics): a family for a later pass.

## q4135 (0x00405350 two landed sources) -- coordinator file action, no row change
- build_candidates.ps1 (Address 00405350) builds `00/40/NUISystemCIME_LoadIMESettings_00405350.cpp` / `_test.cpp` (PassPattern
  `00405350_TEST PASS`). The other pair `00/40/CIME_LoadIMESettings_00405350.cpp` / `rebuild/tests/00/40/CIME_LoadIMESettings_00405350_test.cpp`
  is an older draft of the same body (signed char globals instead of bool, no union fallback) that nothing builds.
- Action (I may not write the repo): `git rm` the CIME_ pair; repoint `rebuild/manifest/functions.tsv` (line 112, source column) and
  `rebuild/ARTIFACT_INDEX.tsv` (line 74) to the NUISystemCIME_ files. Older naming index files (MULTINAME.tsv, ppcmp.tsv) are history.
- Side note: the kept row binds 0x013B7CD8 as `_g_FableImeEnabled_013B7CD8`, the first byte of gIME (stand-in; not this entry).

## q4153 (0x013BAE68 VALUE_DISPLAY_DURATION) -- deferred, owner decision
- Census (exe scan REFS + source grep): initializer 0x0121F220 defines `namespace { const float VALUE_DISPLAY_DURATION }` (binds the
  TU-local spelling); the four landed readers 0x007B7A70, 0x007B7C30, 0x007B7C80, 0x007B7CD0 declare `extern float
  g_OverheadValueExpiryFrames` (?g_OverheadValueExpiryFrames@@3MA). In retail all five are one TU (CTCOverheadDisplay), so no extern
  spelling exists; the choice between keeping the extern stand-in and per-row anonymous-namespace declarations (per-TU ?A0x hash, unresolved
  at link) is the owner decision the entry names. The same holds for MIN_SPEECH_FRAMES 0x013BAB90, JACK_* 0x013BAFD8 / 0x013BAFE0 and
  RANGE_TIME_TO_SIT 0x013BB6F8 (not censused here).

## q4155 (descriptive initializer rows) -- catalog names only
- The five rows already define the PDB-proven objects (agentFAM09 removed the g_scaled_* stand-ins; checked: no g_scaled_ spelling left
  in them except "Was ..." comments). Their catalog names `_static_init::<object>` named the initializer as the object; respelled to the
  sibling convention `<module>::$E_<object>` (RENAMES s1; Leaf `_$E1` and bytes unchanged). PDB names checked in both name sets
  (Ego_r 0x01011F40 / 0x01011F50 / 0x01011F80 / 0x01010E80 / 0x010175B0; FableWin 0x03FF7360 / 0x03FF7380 / 0x03FF73C0 / 0x03FF2F20 /
  0x04000190). File names follow at landing (batch_rerow); no funclet includes them.
- 0x0121E260's test used invented stems; rewritten on the 16 retail literals (no class models). The row still carries a local CTCWife
  model and invented WIFE_APPEARANCE_* enumerators (pre-existing; engine/CTCWife.h only forward-declares the enum, enumerator names
  are in no name set).
- The anonymous-namespace objects themselves (q4153's decision) are untouched.

## q4191 (0x00568440 basic_string(const char*, const allocator&))
- 0x008F9820 LSocket::Open ported: the CNetErrorString stand-in and the local `namespace std { allocator }` model replaced by std::string of
  stlport/fable_string.h, with the new overlay opt-in FABLE_RETAIL_STLP_STRING_CSTR_CTOR_DECL_ONLY. Without it the object grew a pick-any
  copy of _M_range_initialize<const char*> whose header body differs from 0x005672D0 (and from the owner row's QAE copy: work/q4191/ri.txt;
  the ctor copy itself grades RELOCATION_MATCH at 0x00568440). The new COMDATs (~basic_string, _String_base / allocator helpers) have no
  table line and no landed self-leaf owner.
- 0x008F8CC0 LSocket::Listen blocked (port): retail inlines the ctor chain at two report sites and calls it at the third. Shared-header
  forms: all calls (415v468), and an inline ctor opt-in probe changes nothing. Its HEAD form is FABLE_FORCEINLINE on the local stand-in.
- Both rows keep pre-existing local LSocket / LAddress models (global namespace) and bare Leaf lines (`?Open@LSocket@@`,
  `?Listen@LSocket@@`); retail is ?Open@LSocket@LHNetworkLib@@QAE_NAAULAddress@2@0W4LSOCKET_PROTOCOL@2@@Z /
  ?Listen@LSocket@LHNetworkLib@@QAE_NAAULAddress@2@@Z (Ego_r). Porting them onto engine/LSocket.h is a separate re-port (not done).
- Side finding: 0x005672D0's own row defines `??$_M_range_initialize@PBD@...QAEXPBD0ABUforward_iterator_tag@1@@Z` (public) while the
  shared header and the table (witness 0x00563480) spell it AAE (private, STLport): an existing self-vs-callers split, not touched.

## q4195 (0x013BDA24 textureCacheMem)
- 0x009A1660 UninitializeStreamingFontTextureCache now declares `namespace { extern unsigned char* textureCacheMem; }` like the landed
  0x009A3640. Sources: Ego_d.pdb PUB32 ?textureCacheMem@?A0xfe46a155@@3PAEA (egod_carved_names.tsv) + 0x009A3640's binding; the per-TU
  ?A0x hash differs by TU path (expected, not a split). RENAMES s2 writes the new symbol as the bare prefix `?textureCacheMem@?A0x` (the rest is `<per-TU hash>@@3PAEA`).
- The four array globals 0x013BDA34 / 38 / 3C / 40 in the same row keep descriptive g_pStreamingFontCache*Table names (not entries here;
  0x009A3640 declares anonymous-namespace glyphTextureCache* names for some of them: a follow-up).

## q4244 (0x013BD6EC _hStrings)
- Second source found: Ego_r.pdb S_PUB32 `_hStrings` (/tmp/claude-0/egod_pdb/Ego_r.publics.txt) and the string in Ego_d.pdb; plus the
  extern "C" declaration in engine/ErrorDialog.h that 0x0099DCB0 binds. 0x0099E260 (the definer) respelled; RELOCATION_MATCH PASS.

## q4279 (0x0099BE70 operator+) -- done-already
- Self Leaf is the PDB spelling; reloc_bindings.tsv binds only `??H@YI?AVCWideString@@ABV0@0@Z` at 0x0099BE70; the wrong AddWide spelling
  is in no source. 147 REFS rows not recompiled (table + grep, the GOTCHAS nf120 method).

## q4284 / q4460 (CWideString / CBaseClass destructors, mega-families)
- Work unit = the WORKLIST witness rows (advisor). Ported: 0x004A9140, 0x0047B880 (header-less local DV / Host keep their tokens, now
  with CWideString members; explicit member destructor calls), 0x006BEBF0 (N006bebf0::CWideString stand-in -> fable_string.h; also moves
  its copy ctor 0x0099B720 and operator= 0x0099B7D0 onto the PDB spellings), 0x00A90FC0 / 0x00A91390 (engine/CBaseClass.h was included
  inside the ExternalRetry namespace; moved to global scope).
- 0x004A9140 / 0x0047B880 newly define the pick-any `??_GCWideString@@QAEPAXI@Z` (from the explicit `~CWideString()` calls): no table
  line, no landed self-leaf owner; landed rows with the same calls (e.g. 0x00566530) emit it too.
- Left: July rows with no Leaf line (0x00403510, 0x00412020, 0x004407B0, 0x004857E0, 0x006568BB, 0x00A006E0, 0x00529CC0, 0x00A5E650):
  re-authoring work. 0x00AF5900 needs a whole-row port onto engine/CParticleSystem.h. The NF181 census rows (?Dtor@<Class> etc.) were not
  attempted. 0x00CF8A70 is already right (stale table witness).
- Tests of 0x0047B880 / 0x006BEBF0 / 0x00A90FC0 / 0x00A91390 are unchanged standalone models (they never named the shared class).

## Vendor entries deferred (q4570, q4900, q4902, q5055, q5112-q5115, q5117, q5118)
- All ten addresses sit in the libpk + CODA range 0x00C086C0..0x00C89C10 (docs/pipeline/LIBRARY_LINKING.md; the 0x00C34780 row itself says
  "libpk in-memory stream reader"); CSegmentFile / CSampleRef / CStreamBuffer are libpk/ headers and in no PDB; CHLRvbParams (q5118) is in
  CODAApiDbg.lib. Per the brief they are deferred (each needs a self-row re-author or a placeholder-class respell, not a light rename).

## q4195 / q4244 / q4501 / q4504 / q4546 summary
- q4501 / q4504: self rows 0x00B48F20 / 0x00B48F90 renamed QAE -> AAE (access only; full mangled Leaf now, the old one was bare); binders
  0x00B495C0 / 0x00B49610 respelled; 0x00B46660 / 0x00B47BC0 / 0x00B47FD0 already AAE. Every REFS row ends on AAE (no split).
- q4546: 0x004AECF0 ported onto fable_string.h (its local CCharString model removed); 0x005926C0 / 0x005B15C2 / 0x005B1FBC: the
  header-less local classes now derive from CWideString (narrow base rule). Full Leaf lines added to these four rows (each is the symbol
  the HEAD object already defined).

## q4509 / q4915 / q5044 (names proven, binders left)
- q4509 0x00661EA0 = ?SetUpdateMovementVectorThisFrame@CThingCreatureBase@@QAEX_N@Z (NAME_FIXES; both PDBs + bitfield layout + retail
  neighbourhood). The self row has no Leaf line and the binder 0x0089B780 needs a whole-row port, so no rename is applied (no-new-split).
- q4915: self row already right; the binder 0x0046311F is a no-Leaf July row.
- q5044 0x013BAE1D = ?BettingActive@CTCAIScratchpad@@2_NA (three PDB PUB32s + header; the entry's "PDB name unknown" is wrong). Binders
  are row ports or no-Leaf rows; 0x0069E980 is held by NF192.

## Casts
- No stand-in type was cast to a real one. 0x00999230 / others untouched. (0x00B49610 / 0x00B495C0 keep their pre-existing local models.)

## Descriptive names
- None added. 0x004A9140 / 0x0047B880 / 0x005926C0 / 0x005B15C2 / 0x005B1FBC / 0x006BEBF0 keep their existing descriptive / address-suffixed
  self names (DV, Host, CClass_<addr>, N006bebf0): naming of those families is paused (EXTRA_RULES 2026-11-12).

## Flags lines
- None added or changed.

## Header users (hdr/stlport/fable_string.h)
- The only overlay change is the new FABLE_RETAIL_STLP_STRING_CSTR_CTOR_DECL_ONLY `#ifndef`; no landed includer defines it, so none of
  them changes (default path token-identical).

## Rule slips
- Queue order: the ten vendor entries (q4900 .. q5118) were recorded `deferred` before q4860 (between q4570 and q4900) was looked at;
  q4860 was then recorded (held by NF194). Also q5052 and q5064 (mega-families, before q5112 in queue order) are left `open` while the
  later vendor entries q5112-q5118 are `deferred`. Only deferral records were affected, no row work.
- Bash calls: one probe call exited 1 (a `cat` of a lock owner file that does not exist, at q4133); no other slip known.
- grades_used counts in STATUS.tsv are from my grade log (work/grades.log) plus the vland runs; pick-any probes at another address are
  counted on the row they checked.

## Advisor follow-up checks (before hand-back)
- 0x00B47FD0 HEAD probe: binds ?CreateMoreBlocks@?$CDynamicObjectHeap@$0EI@$0BA@@@AAEXXZ at 0x00B48F90 (AAE): the q4504 rename leaves no
  QAE binder. 0x00B46660 / 0x00B47BC0 are the table's AAE witnesses.
- q4279 suspects (REFS rows with no direct global operator+ declaration / fable_string.h include, 45): 0041e6d3 004804de 004830a1 00540320
  0056ad40 00570332 00571280 005729d8 00573792 00574885 005759bf 0058f9bc 00598a1c 0059ca95 0059cfbd 005a04f7 005a3734 005a3e47 005a45f6
  005a4cdd 005a5b31 005a6769 005a731f 005a79e6 005a8175 005a9cbe 005aaf28 005ad571 005aecc0 005af64e 005b0aac 005c4f42 005c6b26 005c71aa
  005c7452 005c90fc 005cacb9 005e4be0 005eb9e0 005f1b10 0074bc20 007da400 007da520 009c33a0 009c47a0: every one includes shared engine/ or
  fable_ headers (which bring fable_string.h's global operator+, lines 388-396) and defines no local CWideString. Not recompiled.
- q4284 / q4546 "rows left" are the table witnesses only; the NGameText family (NF182 census) is unprobed: a missing table line is not
  proof that no NGameText row binds a namespaced spelling.
- New COMDATs with no table line, searched by name in auto-re-candidates.tsv: no catalog owner for ??_GCWideString (0x004A9140 / 0x0047B880),
  ~basic_string<char> / _String_base<char>::_M_deallocate_block / allocator<char> (0x008F9820), _List_base<LSocket*> ctor / clear and the
  list<LSocket*> ctor (0x0122A200), NUISystem::CIME ctor / dtor (0x012288B0). The only owned copy (~_List_base<LSocket*>, 0x008F9F50)
  grades RELOCATION_MATCH at its address.
- No funclet row includes any of the five q4155 staticinit_* files or the two thunk files (grep of rebuild/src, rebuild/tests). None of the
  changed or claimed addresses is in UNMATCHED_ROWS.tsv. No other landed row defines hStrings (engine/ErrorDialog.h only declares it).

## For the coordinator
- q4133: the pair rule says "both rows in the same landing"; the initializers 0x0121F850 / 0x01219E90 are already landed (rechecked
  here, unchanged). Your call whether the atexit rows land now with the DEBT lines.
- q4155: I used the sibling `<module>::$E_<object>` catalog convention; the catalog also uses the "`dynamic initializer for '...''" form
  (e.g. 0x0121F850). Pick one.
- q4195 applies the per-TU anonymous-namespace form (as 0x009A3640 already does), the form q4153 leaves to the owner for the other statics.
- 0x005926C0: the stand-in member call became a base class (CWideString base at +0); base versus first member is unproven (same bytes).
- Locks claimed beyond rows.txt: 0x012288B0, 0x008F9820, 0x0121E260, 0x0121F7D0, 0x0121F800, 0x0121E3D0, 0x01221B80, 0x00B49610,
  0x009BDE70 (was `released`; now NF193, blocked).
