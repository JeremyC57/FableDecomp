# NF436 notes (2026-11-25)

No repo write, no git. Lane: VC7.1 RTM under Wine ($S/wine/env.sh). HEAD and new vland ran at one LANDVERIFY_WORK path per batch
(tools/runpair432.sh; HEAD root work/rsroot, new root work/rs<b> = repo symlinks + the batch's hdr/engine overlay). Table for every gate:
today's rebuild/oracles/reloc_bindings.tsv (work/tab_head.tsv).

## Survey (task 1)
- work/survey73.tsv: per debt row the retail byte size, class size, header presence, header facts. work/fwvt73.txt: FableWin.pdb `??_7<C>` slot
  names + retail vftable slot addresses + landed selves for all 70 classes (CMurderTwistSheriffGuard is nested: 0x00EB2000, task 2 skip).
- work/slotstat73.tsv: of the 276 override slots, 265 are landed under the FableWin spelling, 3 are not landed, 8 are landed under a
  wrong spelling (7 `QAE` non-virtual Init / Main, 1 stand-in `?PersistTransfer_TimeDisplayActive_00eed030@CPersistable_00eed030@@`):
  NAME_FIXES.tsv. The rows whose vftable holds one of those cannot reach 0 WRONG as plain Alloc ports (T3).
- tools/fwcls.py reads FableWin.pdb LF_CLASS (base, methods + attrs, members). For every ported class: base CLevelScriptBase<NScript::T>,
  user ctor + static Alloc + overrides all public, destructor compiler-generated. Header T checked equal to FableWin's T (all 31; the 8 new
  headers take it from FableWin). Header size assert checked equal to the row's `new` size.

## Generators
- gen/gen436a.py (Alloc on a header): row = the old row's comment block (its Model paragraph replaced) + FABLE_RETAIL_NSCRIPT +
  FABLE_RETAIL_CLEVELSCRIPTBASE (+ the opt-in, + per-row extras) + include + `inline` user ctor (0x00D80C00 pattern, unless the header
  defines it) + Alloc. Test (gen434 pattern) links the row object: CScriptBase ctor / dtor by /alternatename, operator new / delete
  recorded; checks size, Game / MasterScriptData, each FableWin override through the vptr, `delete` -> ~CScriptBase + free.
  Per-row extras (a1): 0x00D2A7C0 / 0x00E93CF0 / 0x00EF9130 FABLE_RETAIL_CCHARSTRING + fable_string.h, CCountedPointer.h, CScriptThing.h
  (their headers do not include them; taken from landed includers); 0x00D55D10 FABLE_RETAIL_CQ_GUILDTRAININGPREMELEESCRIPT (the header's
  own guard macro).
- gen/gen436n.py (NEW header, n1): gen434 shape + the class's own members (Ego_r.pdb layout - 0x14, names / types cross-checked with
  FableWin.pdb), nested enum EscortTraderManagerPhase from FableWin LF_ENUM. Sizes: Ego_r - 0x14 = the Alloc's `new` size for all 8.
- All names are two-source (FableWin.pdb publics / LF_CLASS + Ego_r.pdb, retail vftable slots): no descriptive / best-guess names.

## Gates (batches/*/CHECKS.tsv, evidence/; driver tools/full436.sh)
| gate | a48 (5) | a1 (26 = a1 25 + a1t 1, gated together) | n1 (8) |
|---|---|---|---|
| vland HEAD / new | 5/5 RELOCATION_MATCH PASS kept | 26/26 kept | 8/8 kept |
| warn kinds H -> N | SHARED_HEADER_AVAILABLE -> none | same | none -> none |
| LITERAL | 0 | 0 | 0 |
| name_gate --test | 5 PASS | 26 PASS | 8 PASS |
| fwchk (slot names = FableWin ??_7) | 10/10 OK | 52/52 OK | 16/16 OK |
| vt432 plain table | 0 WRONG (HEAD 11) | 0 WRONG (HEAD 92) | 0 WRONG (HEAD 23) |
| vt432 + landed row leaves | 0 WRONG | 0 WRONG | 0 WRONG |
| objsec | leaf equal; NOT_EQ by design | same | same |
| ??_G<C> / ??_G?$CLevelScriptBase<T> at retail slot 0 | 10/10 EQ | 52/52 EQ | 16/16 EQ |
| copies with no EQ retail home (home434) | 0 of 25 | 0 of 130 | 0 of 40 |
| pk new reached NEQ / reach flips | 0 / 0 | 0 / 0 | 0 / 0 |
| /FORCE link with definers | 30 definers, dups 10 -> 10 same set | 153, 58 -> 58 same set | 47, 17 -> 17 same set |
| unresolved (mangled) HEAD -> new | 68 -> 63, none new | 323 -> 297, none new | 123 -> 115, none new |
| apply_table --dry | n/a (empty) 0 misses | same | same |
| reserved_check | 0 | 0 | 0 |
| hdrcheck (hdrcheck434, patched: #ifndef unwrap, trailing comments) | 4 SAME | 22 SAME | 8 NEW |
| coordinator hdrcheck.py | 4 same | 20 same + 2 cpp-artifact DIFF (note) | 8 NEW |
| mutation (Alloc args swapped) | 5/5 FAIL | 26/26 FAIL | 8/8 FAIL |
- pk's only NEQ in every new object is the unreached 5-byte `??1?$CLevelScriptBase<CQ_SunnyvaleMasterData>` copy against the table's
  0x00CBD510 binding (NF434 T3 line 0x00CBD510, as in s48a); reached NEQ 0 in HEAD and new.
- Duplicates: the pick-any `??0<C>` copy each Alloc TU emits v the landed strong ctor rows (0x00CDE350 etc.); HEAD's local models emitted
  the same copies. Control fires (10 / 58 / 17 duplicates listed).
- equiv_check copy against copy (old = cp rebuild/include, new = + all 34 headers): 57,160 rows, 0 differ. Planted control (typedef outside
  any opt-in in CV_DemonDoorsScript.h) flags its two includers 0x00E6EA00 / 0x00E6F6E0 and not two non-includers.
- Definer census: tools/definers.py = every landed row (self_symbols + `// Leaf:` map, work/leafmap.tsv built from the repo) whose self is
  a symbol a HEAD or new object defines or references.

## Home names
work/home_names.tsv: every unique EQ home of an emitted copy (??0<C>, ??0?$CLevelScriptBase<T>) carries the same landed self / table name: 42 OK, 0 mismatch; the 45 others are non-unique sets (??_G / 5-byte ??1 thunks), covered by the slot-0 check.

## Not done / not checked
- 34 debt rows (T3.txt), NF434 tasks 2 and 3.
- naming_status.py tracker HEAD v new not run.
- Lane note: tools/tl.sh passes the row / test paths to cl.exe from work/cc; give it absolute paths (a relative path hangs cl under Wine
  with no error, as a missing include does). One such hang in this session; killed by PID.

## Locks
ACCEPT rows locked NF436 (39). No HOLD rows were locked.
