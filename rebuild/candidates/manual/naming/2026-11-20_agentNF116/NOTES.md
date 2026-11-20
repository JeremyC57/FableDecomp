# NF116 notes (round 10, PRIORITY reconciliation: the STLport basic_string<char> member-template family)

Counts come from APPLIED.tsv / STATUS.tsv / recheck_final.log.
- Entries (5 queue lines, 4 addresses): 3 done (q4186 0x00567400, q4187 0x00567D60, q4188 0x00569140), 0 done-already, 0 rejected,
  2 blocked (q4189 = q4192, 0x00567D00: family, 2 REFS rows left), 0 deferred, 0 open.
- Rows: 27 ported (rows/, all RELOCATION_MATCH + PASS in recheck_final.log), 5 not-a-binder (008400b0, 00841de0, 00563480, 0083f650,
  00840740 already bind the claims; listed in STATUS), 3 blocked (00567d00 self rename staged, 00569e30 port staged, 0056a2d0 not attempted).
- Header opt-ins: 3 (hdr/README.md): engine/LHHttp2.h FABLE_RETAIL_LHHTTP2_OPEN, engine/LSocket.h FABLE_RETAIL_LSOCKET_LADDRESS_NAMEPORT_CTOR,
  engine/LHHttpHeaderStatus.h FABLE_RETAIL_LHHTTPHEADERSTATUS_LOCATION (retail branch; pin_asserts pinned 8). No change to stlport/fable_string.h
  or fable_string_bodies.h (no header claim needed).
- Grading used `FABLE_HDR_OVERLAY=$S/w22/agentNF116/gov` (symlinks to hdr/ plus the repo's engine/LHNetworkTypes.h; hdr/README.md says why).

## Probe (work/probe.tsv, work/cas_probe.tsv)
At HEAD the binders spelled the four addresses 18 ways (0x00567400 6, 0x00567D00 5, 0x00567D60 4, 0x00569140 3; work/probe.tsv): QAE (public) template members from row-local STLport models, the
`CRetailString` / `CNetString_Retail` stand-ins, a non-template local `std::basic_string` (`?append@basic_string@std@@...`,
`??$append@PAD@basic_string@std@@...`), and `_M_append`. Only 008400b0 (on stlport/fable_string.h) bound all four PDB-style AAE spellings.
STLport 4.6 _string.h declares all four members private (the shared header does too), so the claims hold (two sources: the STLport
declaration in fable_string.h and the landed header rows' bindings).

## Fold check
No fold: 0x00567400 / 0x00567D60 (append <char*> / <const char*>) and 0x00567D00 / 0x005672D0 (_M_range_initialize <char*> / <const char*>)
are byte-identical up to rel32 operands but the linker kept both of each pair; callers reach each by iterator type (oracle byte compare).

## How the rows moved
- In place (13 rows on row-local STLport template models): `private:` around the 3-argument member, access restored after
  (tools/privatize.py); 0083fce0 also respelled `_M_append` -> `append` and its free operator+ now goes through a public 2-argument
  `append(f, l)` wrapper (STLport's own). No new local model.
- Ports (14 rows): stand-in classes removed, rows moved onto stlport/fable_string.h (+ fable_string_bodies.h where retail's TU saw the
  bodies), engine/LHHttp2.h, engine/LHHttpHeaders_bodies.h, engine/LHStringFunctions_bodies.h, engine/LSocket.h, engine/LHHttpHeaderStatus.h.
  00841f60 / 008f8cc0 / 008f96e0 keep their existing local (global) LAddress / LSocket models (self leaves unchanged; in-place respell of
  the member type); 008f8cc0 also keeps its CNetErrorString stand-in (0x00568440 binding unchanged, out of scope) and lost its local
  `std::allocator` (now the header's, same mangling).

## Other bindings that changed (RENAMES section 2; none creates a new spelling at a referenced address)
- 0x005672D0 (_M_range_initialize<const char*>): 0083fce0 now binds the AAE spelling 00563480 already binds (the in-place model covers
  both instances with one template).
- 0x00569E30: 0083fc00 now binds the template _M_insert_overflow spelling 0083fce0 / 008400b0 / 00840740 / 00840b60 bind.
- 0x005693B0 (assign): 00841ee0 now binds the template spelling 008f8bb0 binds (was ?Assign@CRetailString).
- 0x0083EBD0 toLowerCase: 0083ee10 (its only REFS row) now binds the STLport DV spelling; the self row's Leaf is the Ego_r Dinkumware DU
  spelling (NAME_FIXES.tsv).

## Blocked: 0x00567D00 (q4189 / q4192), family
- 00569e30 vector<string>::_M_insert_overflow: a port (work/00569e30_ready.cpp) grades RELOCATION_MATCH and binds the claim, but it moves its
  0x00568F10 (__uninitialized_fill_n) and 0x00569A40 (_M_clear) bindings to template spellings no landed row defines or binds. Those self
  rows must move first: 0x00568E70 / 0x00568F10 (__uninitialized_copy / fill_n, non-template basic_string models) and 0x00569A40 (old
  `CRetailVector_SBufferEntry12` stand-in, no Leaf line: authoring-scale). Not handed back.
- 0056a2d0 vector<string>::push_back (non-template local model) moves with 00569e30 (it binds both). Not attempted.
- The 00567d00 self rename (in-place private:, RELOCATION_MATCH) is staged in work/00567d00_ready.cpp and waits for both.
The 14 binders handed back move to the AAE spelling four landed rows already bind, so the binders' spellings of the address go from 5 to 2 (AAE plus the
non-template one of 00569e30 / 0056a2d0) and no spelling is created.

## Needs header claim
- stlport/fable_string_bodies.h: a guard around its two _M_throw_* definitions (`#ifndef FABLE_RETAIL_STLP_STRING_THROW_INLINE`) would let
  THROW_INLINE TUs include it; rows 0x00841B70 / 0x00841C50 could then drop their in-row copy of the STLport append body (marked
  `// Fidelity:`). Not needed for this hand-back.
- engine/LHHttpHeaderStatus.h is GENERATED (gen_class_headers.py): a regeneration drops the NF116 retail branch; the coordinator should
  carry it over (as NF113 asked for engine/CTCWeapon.h).
Otherwise none. (A non-blocking finding: engine/LHHttpHeaderStatus.h is a generated flattened record; the NF116 retail branch is the typed struct
the landed local models at 0x0083F420 / 0x00840B60 / 0x00569D30 could move onto later.)

## Flags lines
None added (every row grades with its build entry's flags: 008f8cc0 /GS).

## Descriptive names
None made up. Descriptive-by-library (STLport 4.6 text, no PDB public: both PDBs are Dinkumware builds): 0x00567E80
`std::basic_string<char>::_M_append_dispatch<char*>`, 0x00569330 `std::basic_string<char>::substr`, 0x00841B70 / 0x00841C50
`std::operator+` templates. PDB names applied: 0x0083F5C0 LHHttpHeaderStatus::GetLocationHost, 0x0083F600 GetLocationURI (Ego_r.pdb),
0x0083FC00 AddHeader, 0x0083F870 SetProxySettings, 0x0083FB20 SetConnectionDetails, 0x00841EE0 LAddress ctor, 0x0083EE10 StartsWith.

## EH funclets
Include scan at hand-back (grep of the 27 source basenames over rebuild/src and rebuild/tests *.cpp): no landed file includes any of them. Self renames that
move the parent's file (catalog name changes): 0x00567E80, 0x00569330, 0x0083F5C0, 0x0083F600, 0x00841EE0; none has funclets.

## Binding diff and gate (work/pdiff_all.txt; gate_rows.txt / gate_targets.json in the hand-back dir)
- tools/pdiff.sh (HEAD vs rows/, overlay gov/) over all 27 rows: besides the four family targets, the changed bindings are the RENAMES
  section-2 lines (0x005672D0, 0x00569E30, 0x005693B0, 0x0083EBD0, 0x00565360, 0x0083F420); each moves to a spelling a landed row already
  binds or defines (0x00565360 / 0x0083F420: the self rows' own Leaf), except 0x0083EBD0 whose only REFS row is 0083ee10 (self Leaf DU: NAME_FIXES).
  The anonymous-namespace hash of 0055f090 / 0055f140's CryptoMatic (`?A0x...`) differs per compile path only. q.py's DIFFER(281v280) /
  (661v660) on 00567400 / 00567d60 / 0083fce0 is the stripped trailing int3, the same at HEAD.
- `gate.py scan` (myrows.txt = rows/): 21 targets, 32 other referencing rows (gate_rows.txt, gate_targets.json); the pairs/eval step is
  the coordinator's.

## Locks
NF116 holds all 35 family rows in $S/w22/nflocks (including the three "released" NF111 locks it took over); the coordinator can release them.

## Rule slips
- One heredoc python f-string failed to parse (a stray brace); rewritten with a template file. No file was written by the failed call.
- Failed forms (STATUS grades_used 2, one failed form each): 00841b70 / 00841c50 / 00567e80 / 00569330 (declarations only, then the same-TU
  STLport bodies), 0083fb20 (SRC_FAIL C3861 Open before the LHHttp2.h opt-in).
