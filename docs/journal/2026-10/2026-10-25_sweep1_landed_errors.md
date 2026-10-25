# sweep1: landed-errors sweep, 2026-10-25

Queue: `docs/journal/FINDINGS_LOG.md` lines 557-599 (FINDINGS chunks 420-457, the near-miss pass 2 note, rename
pass 1, the rasm1 note). All of it is processed. The `$S/names/callees` CANON queue was not started.

## Counts (final, after a full re-grade of every row with its test)

| file | rows | notes |
|---|---|---|
| RENAMES.tsv | 397 | every row MATCH or RELOCATION_MATCH, test PASS (`<addr>_TEST PASS`) |
| FIXES.tsv | 38 | every row MATCH or RELOCATION_MATCH, test PASS (the ps1 pass pattern) |
| LABELS.tsv | 71 | non-landed addresses only (5 that landed during the pass were dropped; their landed names match) |
| UNRESOLVED.tsv | 51 | claim + why not applied; owner decisions flagged |

Every address appears in at most one of RENAMES / FIXES. None of them is in `ALREADY_APPLIED_rename1.tsv`. Every
old_source_path exists and equals the ps1 Source. Grades: 89 MATCH, 346 RELOCATION_MATCH.

## Method
- Tools in `tools/` (copied from `$S/w22/rename1/tools`, outputs redirected here):
  - `cat.py`: catalog, ps1, manifest and label lookup.
  - `g.py`: grades the way recheck does (catalog oracle, ps1 flags, `// Leaf:`, reloc identity, then the test).
  - `row.py`: re-grades and appends a row, and refuses anything that is not MATCH/RELOC, not PASS, or has a hard
    reloc error.
- Helper scripts in `work/`:
  - `work/regrade.py` re-grades listed rows.
  - `work/exactleaf.py` compiles a source and sets its `// Leaf:` to the exact object symbol the grader selects.
  - Per-chunk generators: `work/c447/`, `work/c454/`, `work/c457/`, `work/c571/`, ...
- Evidence: every claim checks at least 2 sources. The sources used were:
  - PDB name sets: Ego_r and the second set, as name sets
  - `struct_layouts_egor`
  - RTTI and vtables
  - xrefs and call sites
  - retail bytes
  - the landed callee and caller sources and reloc bindings
  - the upstream library text for Xiph libogg/libvorbis 1.0.1, zlib 1.2.1 and DirectShow strmbase

## Owner decision applied: STLport is `namespace std`
- All sweep sources now spell STLport `std`. 43 sources (39 renames, 4 fixes, with their tests) had been written in
  `_STL`, the namespace their landed neighbours used before the repo's conversion. They were converted with
  `\b_STL\b -> std`, which leaves `_STLP_alloc_proxy` and similar untouched.
- After conversion, all 43 re-graded with the same grades and PASS. Their new_name / new_module fields were rewritten
  to `std::`.
- `_STL` remains only in quotes of `rebuild/oracles/reloc_bindings.tsv` symbols inside evidence text, and in the
  UNRESOLVED rows that record the question. Those rows are marked "resolved by owner: std".
- The repo has 0 `namespace _STL` sources.

## Final hygiene done in this pass
- **Exact Leaf.** 190 sources had a prefix or bare `// Leaf:` (e.g. `??6CString@@`, `UpdateItems`) or none.
  `work/exactleaf.py` replaced each with the exact object symbol the grader selects, and each was re-graded.
  - 0xC7B8F0 had a wrong prefix: `??_3` is operator<<=, and the row is operator>>=. It is now `??_2Is32vec4@@QAEAAV0@H@Z`.
  - 0x9C7060 (comment-only fix) now names `??0CCompressorZlib_Retail@@QAE@ABU0@@Z`. The landed model uses `_Retail`
    layout classes (see owner items).
- **Line 1 template arguments.** 8 line-1 comments had `X::Y` inside `<...>`; the qualifiers were stripped there
  (kept further down).
- **Banned words.** Wording was changed in 2 TSV rows, 2 UNRESOLVED rows, the 0xC43370 header comment and the
  0xC61FB0 test identifier (renamed `g_pEmitterSeen`), which contained a banned substring. A recursive case-insensitive grep of
  every output for the four banned tokens is now empty.
- **Flags.** At most one `// Flags:` line per source, matching the ps1 CompilerFlags.

## Naming choices (disputed or invented classes)
- **libpk sound events.** Chunk 431's CPanel/CPanelItem are `CSoundEventManager` / `CSoundEvent` (chunk 434's
  spelling); chunk 435 had kept `CPanelItem`.
  - The audio owner family is `CAudioOwner` (ctor 0xC094A0 had been landed as CSteeringGroup) and
    `CAudioParamBlock`.
  - Its CString is libpk's MFC-style `CString`. Its `Append` overloads are `CString::operator<<`, its assign is
    `operator=(const char*)`, and 0xC0BCC0 is `CString::SafeStrlen` (static `__stdcall`).
- **libpk queues** are templated:
  - `PKCQueue<T>`
  - `PKCStaticQueue<T>`, `PKCOneShotQueue<T>` and `PKCMailbox<T>` (named after the `.h` traces in the bodies)
  - `CRequestQueue` and `CReplyMailbox` (descriptive composites)
- **No-RTTI voice effects.** These keep the address names `CVtable_012bedcc` / `CVtable_012bede0` (base
  `CVtable_012beda4`).
  - The ctors 0xC70120 / 0xC70B00 used upper case (`CVtable_012BEDCC`), a second spelling of the same class.
  - They were renamed to the lower case that the class's other members, and the landed base ctor/dtor, use, so the
    base-ctor call now binds the landed `??0CVtable_012beda4@@QAE@XZ`.
- **Xiph code.** libogg/libvorbis 1.0.1 functions carry their Xiph names (`extern "C" __fastcall`, `@name@N`).
  - Chunk 446 rows use module `_global`, following agent446's landed ov_*/ogg_* rows.
  - Chunk 447/448 rows use `libogg` / `libvorbis`, following agent447's landed rows.
  - The repo is mixed today (see owner items).
- **CScriptConversation map.** `std::_Rb_tree<long,pair<const long,CScriptConversation>,...>::find` 0x6E6990 and
  `_Rb_tree_iterator<...CMeshEffectPriorityEntry...>::operator++` 0x4C2560 replace the invented
  CRetailTree_longkey / CRetailTreeIterator classes. The other generated `CRetailTree_*` and `CRetailTreeIterator`
  stand-ins (about 40 + 48 bodies) are not renamed here.
- **Nested classes and access.**
  - `CSubtitleRenderer::CSetTextInfo` is nested.
  - `CTCCreatureExpression::CExpressionState` is nested (PDB). 0x7293A0 now calls the landed
    `std::list<CTCCreatureExpression::CExpressionState>::push_front`.
  - The EClothingSuitPart set/tree chain uses STLport's 5-parameter names with QAE (public) access, as the bindings
    bind them (STLport itself declares `_M_insert` private and `_M_create_node` protected).
  - CTCSoundPlayer's GetPotentialCriteriaStrings / InternalPlaySoundEvent are private (AAE), as in Ego_r.pdb.
- **Other conventions.**
  - Template algorithm rows use the plain function name (`std::sort`, `std::__final_insertion_sort`), as the repo
    does.
  - `SortIfDirty` follows the repo/FINDINGS spelling (FableWin may call it Update).

## This session's additions
- **0x6E6990** `std::_Rb_tree<long,pair<const long,CScriptConversation>,...>::find` (MATCH). The leaf is the
  RemoveConversation 0x6E5990 binding in std form.
- **0x66BD00** CTCSoundPlayer::PlayAttachedCriteria (FIX):
  - The flat vector and its dead `_M_start = 0` store were replaced by the twin 0x66BDF0's STLport
    vector<CCharString> recipe (`__forceinline` dtors, the found-handle return under `inline_depth(0)`).
  - The callees are private per the PDB.
  - It grades RELOCATION_MATCH on the first try.
- **0x7293A0** ForcePrimaryExpression (FIX): the model list was replaced by `std::list<CExpressionState>`.
  - One `front()` reference local gives 101v99.
  - `front()` at each use, an iterator local, or a raw node pointer all match.
- **Chunk 447 libogg/libvorbis** (18 renames, all from the unmodified Xiph 1.0.1 text, first grade):
  - libogg: oggpack_writeinit / write / writealign / reset / writeclear / read
  - codebooks: _ilog, vorbis_book_init_encode / encode / decode, vorbis_staticbook_destroy
  - frees and clears: _vi_psy_free, mapping0_free_info, res0_free_info, _ve_envelope_clear
  - vorbis_bitrate_init (the shipped bitrate.c, 484 bytes)
  - The three free_info rows and staticbook_destroy no longer model the MSVCR71 free import as a defined or
    undefined function-pointer global.
  - New labels: 0xC94FD0 _make_words, 0xC951B0 _book_unquantize, 0xC96320 decode_packed_entry_number,
    0xC8F650 vorbis_dsp_clear.
- **0xC31B90** (FIX): its CString callees were renamed to this pass's CString names (operator=, operator<<,
  SafeStrlen; FINDINGS line 565, "SafeStringLength in one source").
- **0x9E1DA0** GFAXToVX: recorded in UNRESOLVED as an owner decision. The qualifier is kept and the staged agent902
  fix is not re-proposed.
- **LABELS:** 0xC53B10 Pow10 (rasm1: the manifest says Pow2_Float; the constant is log2(10)) and 0x66BC10
  CTCSoundPlayer::GetPotentialCriteriaStrings (PDB).

## Chunk 467 (coordinator)
0x49E1D0 is renamed CWorld::ReceiveLocalEvent (RENAMES). The GetMainPlayer RELOC_CONFLICT has no wrong source:
- 0x449960 is the const overload (`QBE`, calls the const GetPlayer 0x449910).
- 0x449970 is the non-const overload (calls GetPlayer 0x4498C0).
- Row 603 of `rebuild/oracles/reloc_bindings.tsv` (`?GetMainPlayer@CPlayerManager@@QAEPAUCPlayer@@XZ 00449960 00435000`)
  is stale. Delete it; re-recording 0x435000 adds the QBE binding. Details are in UNRESOLVED 0x435000.

## Owner items (decide or apply together)
1. **Apply chains together**, or RELOC_NAME warnings remain:
   - 0x5BA8CF with 0x5BA3AC
   - 0x5BB025 with 0x5BABE1
   - 0x5BB3B3 with 0x5BB025
   - 0x4550AC with 0xF27650
   - 0x6E6990 / 0x4C2560 with any further CRetailTree* renames
   - the CString operator renames (0xC0C200, 0xC0DEC0, 0xC0DF00, 0xC0BA40) with the 0xC31B90 fix
2. **Stale bindings or comments:**
   - 0x494106 binds the non-nested `??1CSetTextInfo@@QAE@XZ`.
   - 0x493CF5 binds ?InitPrimitiveSlot.
   - The landed ctor 0x48DDF6 is non-nested.
   - The comment on 0x5BAF94 says all 15 insert callers are in one function; they are in 3.
3. **Conflicts held in UNRESOLVED:**
   - 0x81E0F0: two landed bodies bind one `sort<pair<ul,CSoundPair>*>` name. Recheck that the 0x81 map really holds
     CSoundPair.
   - 0x4B7800 and 0xCB6230: swap vs iter_swap.
   - 0xCACD20: CUnknown cv-qualifier (purity).
   - 0x9E1DA0: qualifier kept by owner request.
4. **Catalog names lag sources** (catalog regeneration only; ps1 Module/Source are already right):
   - 0xC0BCC0 (catalog `_Cons_val<...>`, source CString::SafeStrlen)
   - 0xCAF580 (catalog CEngineLandscapePatch::Destructor, source ~CCmdQueue)
   - 0x431F10 (catalog CThingBuildingDef::operator=, source CDefClassBase::operator=)
5. **Models still using placeholders:**
   - 0xC31B90 is still "CShaderResource::LoadPixelShaderData". Its body is a libpk PKSerialiseCParser CString
     transfer (EMEM log `.\PKSerialiseCParser.cpp` line 100; callers 0xC3B7AB / 0xC3B86E / 0xC3B9A1). It needs a
     class identity before a rename.
   - 0xC73A00 `CString_00c73a00::ClassifyShaderExtension` (.ogg/.wma test) declares SafeStrlen as a free function.
   - 0x9C7060's landed model uses `CCompressorZlib_Retail` / `CACompressor_Retail` layout classes. The engine
     headers carry Ego_r's 16-byte vector.
   - Landed sources without a Leaf line that rows here call: 0x99A2F0 (CBaseClassNonCopyable ctor) and 0xF37390
     (AllocateDepthBufferSurface).
6. **Access spelling of STLport tree members** (chunk 424): `_M_copy`, `_M_erase` and `_M_insert` are private in
   STLport. The landed models and the rows that bind them use public (QAE). Nothing in the binary decides this; if
   AAE is wanted, every tree row in a family changes together.
7. **Xiph module spelling is mixed** (`_global` for agent446 and its renames, `libogg`/`libvorbis` for
   agent447/448/454). One unifying pass would settle it.
8. **Smaller items:**
   - 0x121AEC0's manifest label `$E2` is `_$E1`. The landed descriptive name was kept.
   - g_pScriptDef vs pGlobalScriptDef: Ego_r.pdb names the global `pGlobalScriptDef` (LABELS 0x122AE7E); the
     landed quest scripts call it g_pScriptDef.
   - 0x401400 is not renamed; its name is already right.

## GOTCHAS one-liners (to fold in)
- An STLport set/tree ctor whose empty temporaries must share one frame slot needs a same-TU
  `#pragma auto_inline(off)` stand-in body for the callee ctor. With it, VC7.1's mangling backref for `std` in that
  TU's leaf became `@3@`, not `@2@`, so take the Leaf from the object.
- STLport `less<T>` derives from `binary_function`, so a value-initialised `_Compare()` is not zero-filled. Model the
  base, or the bytes differ.
- In the CTCSoundPlayer criteria pair, the twin's recipe (STLport vector, `__forceinline` dtors, a found-handle
  return under `#pragma inline_depth(0)`) replaces a dead-store hack and matches on the first grade.
- In libvorbis 1.0.1 under /Gr, the shipped Xiph text grades as written. Declare free/calloc
  `__declspec(dllimport)`, never as a function-pointer global.
- A prefix `// Leaf:` can silently select the wrong function: the grader falls back to the largest one. `??_2` is
  operator>>= and `??_3` is operator<<=. Set Leaf from the object symbol.
