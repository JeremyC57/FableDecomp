# NF326 (2026-11-24): the CScriptThing pick-any unit (63 ENTRIES)

Brief: BRIEF.md (this folder) + rebuild/candidates/manual/naming/briefs/BRIEF_NF_BATCH.md. Unit: the ten CScriptThing members whose landed
self rows define `CScriptThing_Methods` stand-ins while landed NScript / CGameScriptInterface rows define the members in-TU
(GetDefName 0x004AA8D0, GetDataString 0x004AA900, SetDataString 0x004AA930, GetPos 0x004AA980, GetAngleXY 0x004AAA40,
MsgIsHitBySpecialAbilityFrom 0x004AAD20, MsgIsHitByAnySpecialAbilityFrom 0x004AAD40, MsgIsHitByAnyAggressiveSpecialAbilityFrom 0x004AAD60,
IsBeingCarriedBy 0x004AAFE0, IsNull 0x004AB150), decided together with every landed copy.

## Result (STATUS.tsv, 63 lines)
- **10 unit members: skipped-held.** Decision (a) for all ten, drafted and proven end to end. They are held only because the prefix says to
  ASK before landing anything under (a) (Q1). GetDataString carries one copy that could not be brought to retail's bytes (Q2).
- **52 skipped-port, 1 skipped-held (0x00904380, NF318 Q2):** the other entries. None is resolved by the unit decision. Each is one of:
  - a binder or self that binds through a stand-in class, parameter type or container (re-read from today's tracker spellings,
    `tools/status_other.py`);
  - the ScriptedThing family (NF321's port reasons);
  - a base-class-model port (0x00905320).

  Two of them, the ~CScriptThing entry 0x004AA840 and the copy-constructor entry 0x004ABE90, have the same one-definition shape as the unit
  (Q4). Six others (0x00E39140, 0x00E6AD20, 0x00EDC980, 0x00EDCB80, 0x00F08590, 0x00F09950) are also unit copy rows. Their bodies are
  respelled in rows/, but their own names do not change, so their STATUS stays skipped-port, with a note.
- No REJECTED entries, no TABLE_CHANGES and no header change. hdr/ holds only NONE.txt, so there is nothing to equiv_check.
- **Ready to land after a yes to Q1:** the 111 rows in rows.txt, as ONE landing:
  - 10 selfs;
  - 101 copy rows (74 out-of-class copies plus 27 in-class bodies of local `CScriptThing` models).

  Proof below.

## Census (CENSUS.tsv; tools/srccensus.py, tools/objcensus.py, tools/copycmp.py, tools/census.py)
Text alone was not trusted. These rows were compiled at HEAD with VC7.1 (all MATCH / RELOCATION_MATCH), 171 objects in all:
- every row that names a member by text;
- every row that models `class CScriptThing` / `struct CScriptThing` locally (in-class scan, evidence/inclass_scan.txt);
- every row whose retail body calls one of the ten addresses;
- the 10 selfs, 0x004AA840 and 0x004ABE90.

Their COFF symbol tables were then read with tools/objcensus.py (DEF with the COMDAT selection, UND, absent; work/objcensus_before.tsv).

- **Copies by object** (evidence/comdat_census_before.tsv, 164 member COMDATs). Every one is pick-any (selection 2) and already carries
  the PDB spelling:

  | Member | Copies |
  |---|---|
  | GetDefName | 5 |
  | GetDataString | 6 |
  | SetDataString | 9 |
  | GetPos | 104 |
  | GetAngleXY | 5 |
  | MsgIsHitBySpecialAbilityFrom | 3 |
  | MsgIsHitByAnySpecialAbilityFrom | 2 |
  | MsgIsHitByAnyAggressiveSpecialAbilityFrom | 20 |
  | IsBeingCarriedBy | 2 |
  | IsNull | 8 |

- **Why the copies are emitted.** Those TUs also define CScriptThing's constructor / destructor inline. That emits
  `??_7CScriptThing@@6B@` (110 of the 171 objects), and with it every inline virtual of the class.
- **Text-only copies** (defined but not emitted): 0x00D8ABF0 (GetPos) and 0x00E0D210 (MsgIsHitByAnyAggressiveSpecialAbilityFrom).
- **Local-model copies.** 27 objects define their copies inside a LOCAL `class CScriptThing` / `struct CScriptThing` model
  (header-policy debt, left as it is). The text census misses them; the object census does not.
- **References.** 39 to 95 objects per member carry the PDB symbol UND (their vftable slot). 20 landed rows have RETAIL bodies that call
  a self address:
  - 0x00CBFB7D calls all six called members;
  - 13 more rows call GetDataString;
  - 6 more call GetAngleXY;
  - four members have no retail caller at all.

  All 20 already bind the PDB spelling (evidence/refbind.tsv; each object's relocations read as retail target -> symbol). So no reference
  needs a respell.
- **Stand-in spellings today:** only the ten selfs (`?M@CScriptThing_Methods@@...`; four of them also take int parameters and are
  non-const).
- **Pass-28 rows.** 0x00D43DB0, 0x00E03F70 and 0x00E49020 are census objects. Their pass-28 RESERVED lines read "landed ...; no longer
  reserved". Their copies already equal retail, so they were compiled only and are unchanged.

## Decision per member: what retail did
- **(b) is excluded for all ten.** (b) needs retail's copies to be calls of the out-of-line function.
  - The exe census (tools/census_addr.py, evidence/census_exe.tsv) finds 0 to 2 copy rows among the retail callers per member. GetPos
    has 0 calls from its copy rows, and four members have no retail caller at all.
  - So retail EXPANDS these members inline in the script TUs. Retail's CScriptThing.h had them as inline bodies, and 0x004AA8D0 ...
    0x004AB150 are the vftable's out-of-line copies (contiguous, in slot order, next to the other CScriptThing members).
- **A strong self is a link error.** A real link (LINK= cleared, /FORCE:UNRESOLVED, so a duplicate is fatal) of a strong self (NF318's
  drafts, MATCH) with the copies gives LNK2005 on GetDefName and IsNull (evidence/link2_strong_control.txt). The destructor already fails
  this way today: 0x004AA840 (strong) + 0x00E6A0B0 (pick-any) is LNK2005 + LNK1169 at HEAD (evidence/link2_dtor_head.txt).
- **(a) as drafted: the self is pick-any too.**
  - Each self row defines the member `inline` on the real class (engine/CScriptThing.h, FABLE_RETAIL_NSCRIPT).
  - It emits the member through the landed nf86 / nf88 / NF92 qualified-call scaffolding: `thing.CScriptThing::M(...)` in an
    anonymous-namespace helper under `#pragma inline_depth(0)`, the 0x00540CF0 / 0x008E72B0 precedent for virtual members.
  - The copies' COMDATs already carry the same PDB name, and the references follow.
  - Pick-any is only safe when every emitted copy has retail's bytes. That is what the respells below establish.
- **Copy bodies.**
  - At HEAD, 48 of the 164 member copies already equal retail (relocation fields zeroed, same relocation symbols; tools/copycmp.py,
    evidence/copycmp.tsv). 116 do not. Example: `if (!PImp.Data) return NullCoord; return PImp.Data->GetPos();` gives 21 bytes, where
    retail has 18.
  - The copies that DO equal retail are exactly the ones written with the CCountedPointer accessors (`PImp.IsNull()` / `PImp->`). That is
    very probably retail's own spelling.
  - 115 of the 116 were respelled, across 101 rows, to the self's body shape: a local `imp` read once from the data member. The 116th is
    0x00DF6010's, below. Only the body changes; the qualifier, signature, parameter names, the data expression and the null object the
    row already names are kept.
    - 74 out-of-class copies: tools/respell.py.
    - 27 in-class bodies: 24 with tools/respell_inclass.py, which parses each model's pointee type, data expression, callee and null
      object; 3 by hand with tools/manual3.py.
  - The accessor spelling was tried first (work/v2_rows). It gives the same retail bytes, but every respelled object then also emits
    unreferenced pick-any `?IsNull@?$CCountedPointer@VCScriptThing@@@@QBE_NXZ` /
    `??C?$CCountedPointer@VCScriptThing@@@@QBEPAVCScriptThing@@XZ` COMDATs (NF320 Q2), so it was dropped. The local-`imp` shape emits
    nothing new.
  - **GetAngleXY in 0x00DD9520 / 0x00DDAE40.** The named-local `float angle` shape flipped their own leaf (DIFFER, same length). The
    parenthesised form the landed copies use (`return (imp->GetAngleXY());`) keeps the leaf and gives retail's copy (probe prB).
  - **0x00DF6010 (GetDataString, FABLE_FORCEINLINE copy).** Its own leaf goes DIFFER 6493 v 6493 (one `lea` base/index swap) under every
    spelling tried: local `imp` (const and non-const), the accessor form, and `if (CScriptThing* imp = PImp.Data)`. The row is a c2
    symbol-count fit (its own Grade line). It is left unchanged, so its GetDataString copy stays 45 bytes against retail's 42 (Q2;
    variants in work/held_rows/).
- **Per member:** (a) for all ten. For GetDataString, that is (a) with the 0x00DF6010 copy as known debt (Q2), or (c) if that debt is not
  acceptable.

## Proof (work/, evidence/)
1. **Prechecks** (evidence/pre.log, evidence/pre_zs.log, evidence/pre_same.log). Each ran in an earlier call than its grade, after the
   lock was held.
   - clang -fsyntax-only ran on every HEAD source, every new row and every landed test of a changed row.
   - 19 HEAD sources of changed rows fail clang only on MSVC-only syntax in their own text: binding a temporary to a non-const
     reference, an explicit `~map` specialisation, or a deleted-dtor override. `cl /Zs` passed (D1 / NF300 D1 extension).
   - For the 19 changed rows among them, the clang errors equal their HEAD's (NF305 Q2), and `cl /Zs` of the new text passed.
   - The tests that fail clang do so only in VC7.1 headers (iostream / functional / cwchar) or on MSVC-only syntax; `cl /Zs /Od /W3`
     passed.
   - A check over the logs finds no FAIL without a `cl /Zs` pass, and no changed row without a precheck.
2. **Pair grades, HEAD and new at ONE work path** (tools/pairgrade.py; evidence/pairgrade_all.log). All 10 selfs (5 MATCH,
   5 RELOCATION_MATCH) and all 101 copy rows grade the same on both sides. The graded symbol of each self is the PDB symbol.
3. **vland --recheck-landed** (see "vland" below).
4. **objsec, selfs** (tools/selfproof.py on the vland objects -> evidence/selfproof_vland.txt): 10 OK.
   - The member's bytes and relocations equal HEAD's stand-in; the COMDAT selection goes 1 -> 2 (pick-any); the member COMDAT equals
     Fable.exe at the address.
   - The UND set is equal, `.drectve` is equal, there is no `??_7` / `??_R`, and every other section is equal.
   - The DEF set gains exactly the anonymous-namespace helper `?ReferenceScriptThing<M>@?A0x...` (Q1).
5. **objsec, copies** (tools/copyproof.py on the vland objects -> evidence/copyproof_vland.txt): 101 OK.
   - External symbol tables are equal: same names, same selections.
   - Every non-debug section is equal in bytes and relocations, EXCEPT the member COMDATs. Compiler-internal `$L<n>` labels are compared
     by section and value, because their numbers shift with the inline body's own labels.
   - The member COMDATs change bytes only, and now equal both Fable.exe at the self address and the self draft's COMDAT.
   - Relocation names that differ from the self's but point at the same retail address (the nf205 amendment; NAME_FIXES):
     - 0x00EE2A80's GetDefName copy names `?g_EmptyCharString@@3VCCharString@@A`, where the self names `CScriptThing::NullString`
       (both 0x0143E8EC);
     - 0x00DE3F90 names `g_ZeroVector` and 0x00DA4E30 names `g_ZeroPos`, where the self names `CScriptThing::NullCoord` (all 0x0143E8E0).
6. **COMDAT census after** (evidence/comdat_census_after.tsv): 173 of the 174 member COMDATs (164 copies + 10 selfs) equal retail. The
   one left is 0x00DF6010's GetDataString (Q2). Before: 116 of 164 differed.
7. **Links** (tools/link2.py: LINK= cleared, /OPT:NOREF, /MAP):
   - **All 171 census objects, HEAD v new, /FORCE** (evidence/link2_all_{head,new}_force.txt). /FORCE keeps the pre-existing duplicates
     from stopping the map.
     - The duplicate multisets are IDENTICAL: 124 lines (~CScriptThing 91, copy constructor 25, CTimer ctor/dtor 6,
       CBasicString::Compare 2). None is on a unit member.
     - The unresolved sets are IDENTICAL (794).
     - The map keeps the ten self objects for the ten members in the new set. At HEAD it keeps copies with non-retail bytes (e.g. GetPos
       from 0x0089A510).
   - **Strict links without the strong ~CScriptThing / copy-constructor definers, /FORCE:UNRESOLVED**
     (evidence/link2_unit_{head,new}_{strict,force}.txt; the first 144-object census set). HEAD and new are identical: the same 7
     pre-existing CTimer / Compare duplicates, and none on a unit member.
   - **Strong-self control** (NF318's strong GetDefName / IsNull + the same objects): LNK2005 on both members.
8. **Names:**
   - No landed row binds a stand-in self spelling (exe census + object census).
   - The ten PDB symbols are in both PDB name sets and in labels_rtti_port.tsv (one line each).
   - The helper names `ReferenceScriptThing*` are new (0 hits under rebuild/) and live in each TU's own `?A0x` namespace.
9. **orig/** holds the HEAD copy of every changed row (114 files). They were checked equal to the repo at the end of the run.
10. **Reproduction:** rows/ is reproduced from the repo by tools/mkpick.py (selfs), tools/respell.py (out-of-class), tools/manual3.py and
    tools/respell_inclass.py (in-class).
    - The pilot selfs 0x004AA8D0 / 0x004AB150 and 0x004AAA40 differ from the generated text only in comment lines.
    - 0x004AAA40's comment was corrected at the end of the run. It was prechecked, pair graded again (RELOCATION_MATCH both sides, tag
      ang2) and selfproofed OK, and its non-debug sections equal the vland object. vland --recheck-landed of the edited text (tag `ang`,
      evidence/recheck_regrade.log): RELOCATION_MATCH on both sides.

## vland
`vland --recheck-landed`, HEAD and new under one LANDVERIFY_WORK path per batch (tools/runpair.sh; payloads from tools/nf.py pay):
- **Batches u00-u06 (111 rows):** 6 MATCH + 105 RELOCATION_MATCH on BOTH sides. Per-row warnings are the same on both sides; they are the
  pre-existing SHARED_HEADER_AVAILABLE notes of the rows' local models. Logs: recheck_final.log, work/recheck_u0*{h,n}.log.
- **Header drift.** engine/CGameScriptInterface.h changed in the repo during the run (02:15, by another agent). 51 changed rows have it in
  their include closure. 34 of them had been graded in batches u00-u04 before the change, so they were prechecked again (clang; `cl /Zs`
  for 0x00D73970 / 0x00F04CC0, whose deleted-dtor override clang rejects on both sides) and regraded as batches r00-r04
  (evidence/recheck_regrade.log):
  - 34 RELOCATION_MATCH on both sides, per-row lines identical;
  - copyproof on the fresh objects: 34 OK (evidence/copyproof_vland_r.txt);
  - every non-debug section equals the first run's object (68 of 68).

  Final dependency check: `clang++ -M` with the precheck flags, run on all 333 sources (111 rows, their HEAD sources and their tests;
  work/deps/). Every dependency's mtime was compared with the object vland graded from it (.obj and .t.obj, both sides). No dependency is
  newer than its object, except rows/004AAA40.cpp's own comment edit, which was rechecked as tag `ang` (below). The regraded rows' tests
  do not include the changed header. The link and census objects (work/pobj, work/hobj) equal the vland objects in every non-debug
  section (222 of 222), so the link and COMDAT-census evidence holds for the current tree.

## Questions
- **Q1 (the (a) form, all ten members).** The self moves to the PDB spelling as a PICK-ANY COMDAT (inline member + the nf86 / nf88 / NF92
  qualified-call helper). A strong self is LNK2005 against the copies, which expand the member the way retail does and emit it with the
  vftable.
  - Each self object gains one anonymous-namespace helper DEF (NF321 Q2 sends exactly this to the whole-unit route), and its COMDAT
    selection goes 1 -> 2.
  - The 101 copy rows change only the bytes of their pick-any member COMDATs, toward retail. Symbol tables and every other section stay
    equal.

  May the unit land as one landing (111 rows)?
- **Q2 (GetDataString, 0x00DF6010).** That row's GetDataString copy stays 45 bytes (retail 42), because no spelling keeps its own fitted
  leaf. With the unit landed, the linker keeps whichever copy comes first (the self, in a link that lists it first). Two options:
  - land GetDataString with this copy as queued debt (EXTRA_RULES 2026-11-20: copies present at HEAD are debt; NAME_FIXES line);
  - or hold GetDataString ((c)) and land the other nine. The respelled GetDataString copies can land either way, because they only move
    toward retail's bytes.
- **Q3 (spelling of the copies).** The copies that already equal retail use the CCountedPointer accessors. The respelled ones and the
  selfs use a local `imp` (identical bytes); the accessor form emits two unreferenced pick-any CCountedPointer COMDATs in each respelled
  object (NF320 Q2). Two options:
  - accept the mixed text;
  - or rule the accessor-emitted COMDATs harmless, so every copy can take the accessor spelling (work/v2_rows, graded: all but 0x00DF6010
    RELOCATION_MATCH).
- **Q4 (~CScriptThing 0x004AA840, copy constructor 0x004ABE90).** Both are linked today with LNK2005 against their pick-any copies (strong
  self; 0x00DDE230 and 0x00E6B750 also define the destructor strongly).
  - The destructor: 92 objects (3 strong + 89 pick-any). 86 of the pick-any copies equal retail; 3 local-model copies (0x00DA6740,
    0x00E33B00, 0x00E7F7F0) do not.
  - The copy constructor: 26 objects (1 strong + 25 pick-any), all equal to retail.
  - The same pick-any self form would end those duplicates. Their ENTRIES lines are binder ports (stand-in classes), so I did not draft
    them. Should they be queued as a follow-up unit (NAME_FIXES lines)?

## Locks
- Kept on the 111 changed rows (rows.txt).
- The other census rows, compiled only and unchanged, are released. They include 0x00DF6010, 0x004AA840, 0x004ABE90 and the three
  in-class rows whose models needed no change (0x00D3A1A0, 0x00DA6240, 0x00E2E520).

## Rule slips (work/slips.txt)
- One Bash call contained a literal `sleep 0` (no wait happened, but the rule bans any sleep).
- One pair grade (tag dd, rows 0x00DD9520 / 0x00DDAE40) was launched in the same call as the precheck of those exact files. Identical
  probe texts had passed clang in an earlier call; only a comment line differed. I sent SIGTERM to its PID and relaunched it as tag dd2 in
  a later call. The proof uses dd2.
- Multi-row scans and the HEAD census compiles ran with `timeout 3600` on their own launch lines (the prefix allows a longer timeout for
  a multi-row scan).
- Two `awk -v a=<addr>` lookups treated addresses like 00e36390 as numbers (0e36390 = 0) and matched several rows. They only fed `cp`
  into orig/: two copies failed, and nothing else was written. orig/ was then rebuilt with python and checked equal to the repo.
- One Bash call contained a stray `rm -rf /dev/null` (a typing slip). The harness denied the whole call and nothing ran. The call was
  resent without it.
