# NF98 notes (round 8, the unattempted ports)

## Descriptive names

## Entries

### q11261 / q11262 / q11274 / q11275 / q11293 (group 1)
- Recipe: each action temporary's class from its ctor's vtable store (RTTI), put on the generated engine/CCreatureAction_<X>.h through a
  `_CLASS` opt-in; the owning row's local CThing / CThingCreatureBase / CCombatActionBase models replaced by the shared headers.
- Bodies written from the bytes (FableWin.pdb names the member, release inlines it; the statements are descriptive, read off the retail
  expansion): CCreatureAction_UndeadRiseUp ctor (0x0074ACE0: a named CCharString "RISE_UP_STRIKE" local, SetCombatAnimation, then
  StillUnderground = true), CTCKraken::SetHeadTracking (0x00924930), CPixelFormat::operator== over IsEquivalent (0x009BF350),
  CTCInventoryWeapons::GetActiveRangedWeapon (0x0072CBA0), CTCDiscovery::IsDiscovered (0x009378F0, row-local class).
- 0x0072C790 declares the ForEnflame dtor (`virtual`, out of line 0x0072D390) in the _CLASS opt-in: undeclared, VC expanded it into
  ~CCreatureAction_SheatheItemToInventory 0x0048C950 (RELOC_CONFLICT). The landed 0x0072D390 emits the implicit dtor through its TU harness;
  0x0072C790 only references it, so the two do not clash.
- 0x00924930's own leaf is in the anonymous namespace (`?FinishAction@CCreatureAction_KrakenHighAttackLoop@?A0x<hash>@@UAEXXZ`): the hash is
  per TU; vland matches the leaf by substring and gives the same warning as the landed control 0x00924B90. Coordinator decision whether
  that warning is accepted (it was for 0x00924B90).
- One definition (checked with objdump -t on every graded object, then each emitted COMDAT's landed owner from reloc_bindings / Leaf lines):
  - SetPItemRemoved: 0x0072C790 expands it (per-TU body opt-in) and the landed 0x0072D360 defined it strongly -> LNK2005. Fixed: the opt-in
    body is now the exact statement 0x0072D360 defined (`PItemRemoved.SetPItem(item.GetPItem())`, 0x0072C790 regraded RELOCATION_MATCH), and
    0x0072D360 includes it and emits it pick-any through the nf88 anonymous-namespace reference. Link proof in work/link/: ctl.log (landed
    owner + 0x0072C790) LNK2005 + LNK1169 exit 145; new.log (new owner + 0x0072C790) exit 0, LNK4088 only. The control object was compiled
    once outside g.py (work/link/ctl_0072d360.obj, the landed source unchanged; not in grades.log).
  - CSurface::GetWidth / GetHeight: 0x009BF350 emits the header inlines as COMDATs (default CSurface.h path) while 0x00415BD0 / 0x00415BF0
    define them strongly (SIZE_OUTOFLINE). Pre-existing: landed includers 0x00A156D0 and 0x00BE1E70 already emit the same inline. Needs the
    nf88 pick-any treatment on 0x00415BD0 / 0x00415BF0 (coordinator item, not claimed).
  - CCreatureAction_OnGenerate ctor: 0x008A9100 emits ??0..@AAV (header inline); the landed 0x0074DBB0 defines a by-pointer ctor (no Leaf line):
    different symbols, no clash today, but 0x0074DBB0's spelling is wrong (NAME_FIXES).
  - The other COMDATs emitted (CVectorMap::LowerBound, bitset::test, IS_THING_ALIVE, list iterators, CScriptThing members ...) are template /
    header inlines every landed includer already emits.
- Binder completeness: WORKLIST lists one caller per spelling (reloc_bindings). cs.py over 0x006644F0 / 0x0062E900 and a source grep of each
  landed caller leaves 28 rows on stand-in spellings (work/cs_rows_left.txt), beyond the 40-grade family budget (36 used).
- q11274: cs.py found five more landed binders of 0x009F9E00 on a made-up non-const `GetSurface`; respelled in place (claimed): 0x0066CB00,
  0x009FD4E0, 0x00B2C4E0, 0x00B87680, 0x00B89A20 (work/b8.log). 0x0055FC90 is locked by NF97: entry deferred.
- q11293: three more binders of 0x00662000 respelled in place (claimed): 0x00914B20, 0x00986E30, 0x0092D7C0 (work/b7.log).

### Rule slips
- One foreground `sleep 1` in a Bash call (not allowed; no effect on results).
- Order: q11274 rows were done before q11261 inside the same group-1 item.
- One Bash call hung on a `grep -c` with an empty file argument (read stdin) and was stopped; no file was written by it.

### q11251 (one definition, GetRandomTextureMorphs 0x0086B810)
- Done on NF93's plan in 3 grades (0x0086BE40, 0x0086B810, 0x0086B9D0 regrade, all first-try RELOCATION_MATCH). The body is NF93's / e01's text
  moved unchanged (no new statements). 0x0086B810 carries `// Flags: /d2inlT49`: its build_candidates entry has no CompilerFlags, set
  '/O2 /Oy /W3 /d2inlT49' at landing. The two control objects for the link proofs (work/link/ctl_0072d360.obj, work/link2/ctl_0086be40.obj)
  were compiled outside g.py from the landed sources, unchanged.
- 0x0086B9D0 includes CRandomAppearanceMorph.h without the new opt-in (default path token-identical); not changed, so not in rows/.

### q11333 / q11397 / q11420 (NF93 part 3 self rows)
- All three self rows ported off their dozen-class local models as explicit instantiations of the engine/CThingSearchTools.h templates,
  each with the define set of a landed row of the same template or the same retail TU: 0x008E3C50 on 0x007A3CA0 (+ ROUGHAREA_TYPEDLOCAL,
  the 0x00746560 opt-in), 0x0084F850 on its caller 0x0084E590 (ROUGHAREA_EGOD), 0x006DFA50 on its caller 0x006DDD30 (ALLTHINGSBODY +
  TYPEMASK, STLP_LIST_PUSHBACK_NOINLINE, CTHINGAICREATURE_STATTYPEFLAGS, CTHING_SCANINCLUSION_ISALIVE). No new opt-in. The filters with
  no shared header stay row-local, copied from the caller rows (selfcheck: not NEWLOCAL).
- The catalog names of 0x008E3C50 / 0x0084F850 were a propagated GetPBestThingInRoughArea<..> label (RENAMES s1 carries both names).
- 0x006DFA50's Bind5 operator() is FABLE_FORCEINLINE with an Inline-budget note, as its landed local model had it (plain inline: VC7.1
  calls it out of line once push_back is out of line). No __forceinline / __asm token is new (selfcheck TOKENS clean).
- The repo moved during the session (land NF97 20:29, merges 20:49 / 20:56): none of my overlay base headers and none of my rows' landed
  sources changed (checked against orig/ and git log); grades before and after it agree (recheck_final).
- A second foreground sleep slip: one status check began with `sleep 0` (no wait, but the token is disallowed).
- STATUS uses `ported: respelled in place` for the 9 binder rows whose local declaration was respelled without moving the row onto the
  shared headers (0x00914B20, 0x00986E30, 0x0092D7C0, 0x0066CB00, 0x009FD4E0, 0x00B2C4E0, 0x00B87680, 0x00B89A20, 0x0086BE40).
- Grades: 46 g.py compiles in grades.log (group 1 family 36 of 40; q11251 3; q11420 3; q11397 1; q11333 3), plus the two link-proof
  control compiles outside g.py. No row over its budget (max 8, 0x0074ACE0).
- Rule slip: one compile overlapped a background vland (gg.sh 0x0084F850 ran while vland b11 for 0x008E3C50 was still running). Both rows
  rechecked afterwards (b11, b12, recheck_final) and match.
- Rows in rows.txt not started (their entries are open or deferred): 0x00518130, 0x005185F0, 0x0063FAE0 (q11280 / q11283), 0x00670D50 (q11286),
  0x0078954D (q11281), 0x0078B070, 0x00848500, 0x008C64F0 (q11260), 0x00B9DC60 (q11277). 0x0086B9D0 regraded unchanged (q11251).
- 0x0092D7C0: the Leaf line I added first carried the PDB UAE spelling, which the row does not define (vland "matches no symbol");
  corrected to the QAE symbol it defines and regraded (b14); the PDB spelling goes to NAME_FIXES.
- recheck_final warnings other than SHARED_HEADER_AVAILABLE: STANDIN_LEAF on 0x0084F850 / 0x008E3C50 / 0x00942270 (catalog names differ:
  RENAMES s1 carries them) and RELOC_NAME lines on bindings these rows already had (catalog labels, not spellings I introduced) plus
  0x006A7400 (NAME_FIXES).
- One-definition re-check on the six later rows: every emitted COMDAT with a landed owner is a header inline or STLport / template body
  that landed includers already emit, except GetRandomTextureMorphs, whose owner 0x0086B810 is now pick-any (work/link2/).
