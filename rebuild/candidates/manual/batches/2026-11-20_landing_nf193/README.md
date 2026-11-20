# Landing nf193 (Account A, 2026-11-20): remaining mixed queue, first 50 (18 rows)

Agent NF193 (round 10) handed back early (context) at q5044 with 50 entries: 7 done, 1 done-already, 7 blocked, 12 deferred and 23
open (q5052 onward). Its files are in `naming/2026-11-20_agentNF193/`. The open entries and q4860 (its row was locked by NF194) go back
to the queue unclaimed (24 entries).

- 18 rows land, all RELOCATION_MATCH, every test PASS (1 test-only row, 0x0121E260).
- Lock slip: NF193 also edited 0x009A1660 (q4195) while NF192 held its lock. NF192's landed nf192 version covers all five glyph-cache
  statics, including NF193's `textureCacheMem`, so NF193's draft is dropped (kept in `naming/.../dropped/`) and q4195 is credited to
  nf192.
- q4133 (static pairs): 0x0122A200 is re-authored as the `_$E2` atexit destructor of the LSocket::GlobalList TU, and 0x012288B0 likewise
  for gIME. Their initializers 0x0121F850 / 0x01219E90 are already landed (rechecked, unchanged). Both rows of each pair define the
  static: the static-pair rule (EXTRA_RULES 2026-11-20), with its two DEBT lines queued. The landing link shows 0 LNK2005 either way.
  - 0x0122A200 now binds 0x008F9F50 as STLport `~_List_base<LSocket*>` (its copy grades RELOCATION_MATCH there), replacing an
    `Invoke_008f9f50` stand-in the table lacked. 0x0090AA40 binds the folded twin `~list<LSocket*>` there, and the address's own row
    carries the junk name SortTreeRecursively. The spelling count is unchanged (tablecheck GREW 1 -> 2 only because the stand-in had no
    line); picking one name is queued.
- q4155: the five initializer rows already define their proven objects. Their catalog names were `_static_init::<object>`, and the
  agent proposed `$E_<object>`. The catalog takes the exact PDB spelling (both name sets) instead, e.g.
  "`anonymous namespace'::`dynamic initializer for 'JACK_FADE_DOWN_TIME''" and
  "`dynamic initializer for 'CTCCreatureModeManager::CModeBase::NO_FRAMES_BEFORE_PLAYING_WAVE_ANIMS''". The files keep their
  `global_E...` / `..._E...` names, as precedent 0x0121AD20 does. 0x0121E260's test is rewritten on the 16 retail literals.
- q4135: no row change. The catalog builds `00/40/NUISystemCIME_LoadIMESettings_00405350.cpp`; the stale duplicate
  `CIME_LoadIMESettings_00405350.cpp` and its test are removed, and manifest/functions.tsv, ARTIFACT_INDEX.tsv and
  integration/gfmain_calls.tsv now point at the built file.
- Other ports: 0x0099E260 binds and defines `_hStrings` (Ego_r S_PUB32; engine/ErrorDialog.h only declares it, no other definer);
  0x008F9820 LSocket::Open onto std::string of stlport/fable_string.h (q4191); 0x004AECF0 onto fable_string.h, plus 0x005926C0 /
  0x005B15C2 / 0x005B1FBC (q4546); 0x00A90FC0 / 0x00A91390 (q4460); 0x004A9140 / 0x0047B880 / 0x006BEBF0 (q4284).
  - 0x005926C0: the stand-in member call becomes the PDB `??0CWideString` through a CWideString base of the header-less local class
    (narrow base rule, nf189 0x005EC990 precedent). Base versus first member gives the same bytes and is unproven; queued as debt.
- Self renames: 0x00B48F20 / 0x00B48F90 QAE -> AAE (access only, full mangled Leaf; q4501 / q4504); every referencing row now binds AAE.
- Header: stlport/fable_string.h (`orig` equals HEAD) gets FABLE_RETAIL_STLP_STRING_CSTR_CTOR_DECL_ONLY, an `#ifndef` around the
  out-of-class `basic_string(const C*, const A&)` body. Without it, 0x008F9820 emitted a `_M_range_initialize<const char*>` copy that
  differs from owner 0x005672D0. The default path is token-identical, pin_asserts pinned 0, and no landed includer defines the macro.
- Pick-any (`logs/pickany_check.txt`): the NO-OWNER-OBJ lines are this landing's data definitions (`_hStrings`, gIME, GlobalList). The
  MISSING lines are the ~_List_base / ~list copies against 0x008F9F50's junk-named owner (graded by the agent at the address). Other
  new COMDATs (??_GCWideString, ~basic_string<char> helpers, the CIME ctor / dtor) have no owner.
- Deferred: ten libpk / GameCODA vendor entries (light renaming only), q4153 (owner decision). Blocked: q4191 (0x008F8CC0 inline-budget
  split), q4284 / q4460 / q4546 / q4915 (no-Leaf July rows to re-author), q4509 / q5044 (names proven, in NAME_FIXES; binders need ports).
- Link (`logs/link_landing.log`): 0 LNK2005, as at HEAD.
- Recheck (`logs/recheck.log`): rebind multi-check 1 (baseline); split gate 0 created. Includers and referencing rows 26 good, 0 bad;
  owner recheck good.
- tablecheck (`logs/tablecheck.txt`): seven addresses shrink (0x0099B510 9 -> 6, 0x0099B720 14 -> 13, 0x0099AED0 7 -> 3, 0x0099A2E0
  8 -> 6, 0x0099B7D0, 0x00B48F20, 0x013BD6EC), one grows (0x008F9F50, above). No out-of-image oracle line added.
