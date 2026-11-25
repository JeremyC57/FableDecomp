# NF427 hand-back (2026-11-25): creature-action family virtual members + NF425's 111 wrong-slot Clone rows

Tree: worked against the repo as found on 2026-11-25 (the tracker gave 896 open family rows / 466 classes, not the brief's 941 at
f18a42da20; the tree moved). Every `orig/` copy was re-checked against the repo at hand-back (0 stale). No repo writes, no git,
no `wineserver -k`. Codex pass 29 reservations: 0 overlap with any locked or batched row.

## Result
**233 rows ACCEPT in 8 batches** (108 Part 2 + 125 Part 1), every row on all gates below. 89 headers get one add-only
`FABLE_RETAIL_<C>_VIRTUAL_MODEL` opt-in each (hdr/engine, orig/hdr_<C>.h; each batch carries the headers its rows use plus
HEADERS.tsv). Four batches need a ruling first (RULINGS_NEEDED.tsv): p2c (q1), p2d (q2), g1f (q3), and the three tail-pad
models used by p2a / p2b (q4).

| batch | rows | part | shape / generator | grade (HEAD = new) | notes |
|---|---:|---|---|---|---|
| p2a | 50 | 2 | CLONE_INLINE moved onto my VIRTUAL_MODEL (gen_vth427.py) | RELOCATION_MATCH PASS | r1 condition: 0 WRONG on every row |
| p2b | 13 | 2 | same | RELOCATION_MATCH PASS | same |
| p2c | 38 | 2 | CLONE_INLINE, no engine header: local class + full retail override set (gen_vthl427.py) | RELOCATION_MATCH PASS | RULINGS q1; 1 rewitness |
| p2d | 7 | 2 | same, header debt: hand-written headers genmodel cannot extend (NF427_HDRDEBT=1) | RELOCATION_MATCH PASS | RULINGS q2 |
| g1a | 50 | 1 | G1 CONST (gen_const427.py) | MATCH PASS | 1 empty-body row: no mutation (NF419 r1) |
| g1b | 47 | 1 | G1 CONST | MATCH PASS | |
| g1f | 9 | 1 | G1 CONST, float literal | RELOCATION_MATCH PASS | RULINGS q3; 5 HEAD entries left out (byte-dump comments, head/HEAD_FROM_REPO.txt) |
| g2a | 19 | 1 | G2 JMP (gen_jmp427.py) | RELOCATION_MATCH PASS | 19 callee respells `_ForwardTarget*` -> base member (no table lines on either spelling) |

Part 2 accounting (111 rows): 108 ACCEPT (p2a/p2b/p2c/p2d); 3 HOLD and lock released: 0x0066A880 (GenericResponseToDrain: its NF425 h3
row has no local class and its hand header could not take a model), 0x00860290 (GetUpFromSitting: NF323 Q3 / NF327 unit with its two
binders, T3 held lines), 0x008EDBD0 (CCreatureAction_Die: one unit with the four rtti_conflict FinishAction rows).

## Shape histogram (evidence/shapes.tsv, retail body from the catalog, tools/shape427.py)
| member group | shape | rows |
|---|---|---:|
| Clone | OTHER (NF425's family) | 334 |
| virtual override | OTHER (body-specific) | 161 |
| ??0 constructor | OTHER (split spellings) | 142 |
| virtual override | RET_TRUE | 93 |
| virtual override | RET_FALSE | 32 |
| virtual override | JMP (tail call to an ancestor's member) | 26 |
| ??_E / ??_G | deleting destructor | 33 / 21 |
| ??1 | destructor | 13 |
| virtual override | GET_MEMBER / RET_FLOAT_GLOBAL / NOBYTES / RET_LONG / EMPTY / RET_ZERO / EMPTY_RET4 | 11 / 9 / 8 / 6 / 3 / 3 / 1 |
Self forms of the virtual rows (evidence/classify.tsv): `?C_M@@YI...` no-arg fastcall, `@C_M@4` extern "C" fastcall, `_C_M`
extern "C", flattened `R __fastcall C_M(C*)`, non-virtual `?M@C@@QAE...`. The generators key on the retail shape, not the stand-in form.

## Model layer (the shared step for both parts)
- tools/slotdiff427.py: per class, retail vftable v its RTTI direct base's, dword by dword (evidence/slotdiff.tsv / slotclass.tsv):
  405 classes OK (every differing slot RTTI-labelled for the class and in a PDB name set), 60 stand-in class names with no RTTI vftable.
- tools/genhdr427.sh + tools/genmodel.py (NF322/NF329 generator, repointed; stamp now "NF427, 2026-11-25 ... as run by NF329"):
  families CA (base CCombatActionBase branch 1, 0x114), CB (CCreatureActionBase, 0xA8), CH:<Base> (chained onto a model made here).
  120 models generated and probed; the 89 that ACCEPT rows use are handed back (31 unused in work/hdr_unused, T3 debt line).
- tools/probe427.py (vprobe2 form, reads each model's Needs): emitted ??_7 == retail slot count, every slot SAME (slot 0 `??_E` for
  retail `??_G` is VC7.1's weak alias, as NF329). 4 failed (genmodel inserted inside an existing opt-in) and were dropped.
- tools/tailpad427.py (RULINGS q4): CastWillEnflameSpell / TeleportIn / ForcePushOutOf: the generated trailing `_pad_` member under
  pack(1) made the inlined copy longer than retail (LEN 34 v 71, 83 v 59); dropped inside the new opt-in only, sizeof/offsets asserted.
- hdrcheck.py: 89/89 SAME (dropping the opt-in gives the repo header byte for byte); orig copies current.
- Includers: 339 landed rows/tests (and 12 repo headers) include an edited header. equiv_check (both trees scratch copies): 0 differing.
  Planted control (a typedef just before each model block): 40 includers flagged = exactly those that reach the block; the other 299
  define an outer branch switch of that header and never reach it (e.g. 0x004E3158 defines FABLE_RETAIL_CCOMBATACTIONGENERICRESPONSETODECAPITATION).
  HEAD sides of every batch were also compiled with the overlay (vland HEAD grades unchanged).

## Generators (one per shape)
- gen_vth427.py (Part 2, header): NF425's i/h3 row text + MODEL + Needs, `return new C(*this);`. Gates: leaf EQ (relocs masked),
  DEFs only leaf/??_7/??0 copy/??_G, base copy ctor + ??2 in UND, slot gate: emitted ??_7 count == retail and every slot names retail's
  RTTI label (0 DIFF) and the vtw425 count gives 0 WRONG. h3 rows drop their old branch switch.
- gen_vthl427.py (Part 2, local): the NF425 local class gets every slotdiff declaration (PDB spelling via llvm-undname); same gates.
- gen_const427.py (G1): `R C::M() const { return <constant>; }` on the model, constant decoded from retail (`mov al,1` / `xor al,al` /
  `mov eax,imm` / `fld [pool]` with the float read from Fable.exe and spelled to round-trip / bare `ret`). Identity: RTTI label ==
  tracker symbol, label unique, PDB name set; unit: old self identifier named nowhere else; census: no other `C::M(` definition.
  Linked test: qualified call `p->C::M()` on static storage, checks the constant, prints the HEAD pass pattern.
- gen_jmp427.py (G2): `void C::M() { Owner::M(); }` when retail is one `jmp` to an RTTI ancestor's same-named member whose landed self
  equals its RTTI label; UND must be exactly that label. Linked test defines Owner::M, checks one forwarded call with the same `this`.
- flatten_pilot.py v5 (NF329, repointed) was run as a cross-check on 64 flattened rows (62 ACCEPT, objcheck equal under the rename);
  all 62 are CONST rows and land through G1 (G1 also spells float literals the flatten form kept as data symbols).

## Gates (per batch; CHECKS.tsv, evidence/)
- vland HEAD / new at one LANDVERIFY_WORK path: grades equal (table above), tests PASS, 0 LITERAL warns, 0 new warn kinds
  (RELOC_NAME removed: p2a 6, p2c 5, p2d 2, g2a 19). p2a/p2b re-graded on the final text after a comment-only provenance fix (vland_<b>n3).
- objgate (gate425.py): CONST / JMP: every section equal under the self rename (+ the expected callee rename for g2a) except
  `.drectve +libcp` (NF318 Q1). g1f: + the added `__real@<bits>` COMDAT, bits verified equal to retail at the HEAD data symbol's VA.
  Part 2: only the class ??_7 COMDAT and an added pick-any ??_G (ruling r1), ??_G EQ retail slot 0 on 108/108 (retg.tsv).
- Slot check ($S/vtw425.py as tools425/vtw427.py, evidence/vtw.tsv): **0 WRONG on all 108 Part 2 rows**; my label check: 0 DIFF.
  G1/G2 objects emit no ??_7.
- pk_all / pickany_vs_retail (pkgate425.py): 0 regressions, 0 new NEQ, 0 reach flips (Part 2: the one added copy is the ??_G).
- /FORCE link (linkgate425.py): duplicates HEAD -> new p2a 15->15, p2b 3->3, p2c 9->9, others 0->0; every new self defined once; callee
  renames unresolved->resolved (p2a 6, p2b 2, p2c 5, p2d 2, g1f 9 literals, g2a 19); 0 lost. Definers: base copy ctors + NF425's
  vt_cand emitters (252 + 155 + 41 compiled HEAD with the overlay), g2a: the 4 target owner rows. Control: a copied object gives
  LNK4006 (evidence/linkgate_control.txt).
- apply_table --dry: p2c 1 rewitness, 0 misses, multi-target 1 -> 1; other batches no table changes.
- reserved_check (RESERVED_REFS=""): 0 reserved, 0 mentions, all 8 batches.
- Mutation: every row FAILs its mutant (Clone `return 0;`, G1 constant flipped, G2 forwarding call dropped), except the one empty-body
  g1a row (no mutation possible; leaf bytes equal HEAD, NF419 r1).
- name_gate.py: G1/G2 rows 125/125 PASS. Part 2: the rows themselves pass except p2c LOCAL_CLASS (q1) and p2d row
  SHARED_HEADER_AVAILABLE (q2); every Part 2 TEST fails SHARED_HEADER_AVAILABLE because it is NF425's self-contained test that models
  CCombatActionBase locally (NF425 test form, same as its h1/h2 batches) -> T3 test debt (line in T3.txt). DEBT `??2` agree_unverified (operator new) everywhere.
- Census (evidence/census_selves.tsv): 0 other landed source defines any new self; hits only in other rows' standalone tests (test-local
  stubs, NF329 census rule).

## Locks
233 ACCEPT rows locked NF427. Released: 0x00692960 and the flatten/G1 rows that did not ACCEPT, plus the 3 Part 2 HOLD rows
(0x0066A880 0x00860290 0x008EDBD0, were NF425_VTHOLD). No NF425_VTHOLD lock remains.

## Not done / could not check
- Constructors (??0 142, split spellings), ??1 (13), ??_G/??_E (54: NF424 G1 can now run on the 89 models), OTHER-shape overrides (161),
  GET_MEMBER (11), DestroyMembers stand-ins (18): residue with a reason per row in T3.txt (482 lines + 6 debt lines).
- vtall: covered by the full-slot label check (see Post-review checks); landed copies with FEWER slots are those rows' debt -> T3 vtable line.
- The lander must rebuild 5 g1f HEAD entries from the repo (byte-dump comments; head/HEAD_FROM_REPO.txt, their orig/rows omitted).

## Tools (tools/, tools425/)
tools/: wl427.py cls427.py classes427.py shape427.py slotdiff427.py screen427.py genhdr427.sh genmodel.py probe427.py tailpad427.py
hdrcheck.py dispo427.py flatten_pilot.py (+ copies vprobe2 vtcmp declsrc nf pe). tools425/ (NF425 pipeline repointed): gen_vth427.py
gen_vthl427.py gen_const427.py gen_jmp427.py mut427.sh runnew427.sh namegate427.sh vtw427.py, lib425 (reads evidence/classify.tsv),
runpair425.sh chain.sh mut425.sh gate425.py pkgate425.py warncmp.py linkgate425.py link377.py retg425.py vtcmp425.py finalize425.py
(+ CONST / CONSTF / JMP shapes, vtw gate) mkbatch425.py defobj.py.

## Post-review checks (advisor review before hand-back)
- In-class / pick-any definer census of all 233 new selves (tools/inclass427.py, evidence/inclass_census.tsv): no landed source,
  repo header or overlay header defines any of them in a class body, and no reloc_bindings / self_symbols line binds any of them at
  another address (regex control fires on a planted in-class body). So the 0-definer link gates of g1a / g1b / g1f / g2a are complete.
- Catalog: Module == the new self's class and the source file is named <Class>_<Member>_<addr>.cpp for all 233 rows: no RENAMES.tsv.
- No HEAD row had a `// Flags:` line or catalog CompilerFlags (nothing to carry into the G1 / G2 rewrites).
- T3_PORT_QUEUE lines the ACCEPT rows resolve (evidence/t3_resolved.tsv, 75): nf324 port 68, nf324 held 5 (0x006F5940 0x008550A0
  0x008550B0 0x008BFFB0 0x009536C0: their reason, "model declares M only under a non-landed switch" / "no HEAD payload", is gone:
  the model declares M; 0x008BFFB0 HEAD entry is in g1f head/HEAD_FROM_REPO.txt), nf330 port 2 (0x008C0270 0x008C02A0, in p2a).
- vtall (NF425's cross-object ??_7 comparison): every Part 2 row's emitted ??_7 has retail's full slot count with every slot naming
  retail's RTTI label (0 DIFF; evidence/gen/gv_all.tsv, gvl_all.tsv), so no other landed copy can name a retail slot these copies miss,
  which is vtall's criterion. Other landed copies that name FEWER slots are those rows' own debt (T3 vtable line).
- Device used: one `_VIRTUAL_MODEL` opt-in per class (NF329 form) proven by hdrcheck + equiv_check over every includer, not a
  per-member `FABLE_RETAIL_<CLASS>_<MEMBER>` opt-in with an includer regrade.
- Final re-check: orig rows / tests / headers equal the repo (0 stale); byte scan of the whole hand-back (hex-pair runs, 0x-lists,
  \x strings): 0 hits.
