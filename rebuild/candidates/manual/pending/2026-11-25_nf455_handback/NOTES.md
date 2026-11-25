# NF455 (2026-11-25): r454b self respells onto the PDB spelling

Input: `$S/w22/r454b_input.tsv` (456 lines: 27 dtor, 261 ctor, 87 op, 81 method). Scope this session: destructors, then the
cleanest systematic group (one-argument `(CThing*)` -> `(CThing&)` constructors). Scratch: `$S/w22/agentNF455`.

## Batches
| batch | rows | what | status |
|---|---:|---|---|
| b1_dtor_novtable | 6 | `??1X@@QAE@XZ` -> `??1X@@UAE@XZ`: `virtual` on the in-class dtor of a `__declspec(novtable)` model | ACCEPT |
| b2_ctor_thingref | 13 | `??0X@@QAE@PAVCThing@@@Z` -> `??0X@@QAE@AAVCThing@@@Z`: one-argument component / creature-mode ctors with no landed binder of the old spelling | ACCEPT |
| b3_ctor_thingref_alloc | 16 (8 units) | the same respell for 8 ctors whose landed `CTCX_Alloc` factory binds the old spelling: ctor + factory (callee `CTCX(CThing&)`, `new CTCX(*owner)`) | ACCEPT |
| b2f_ctor_thingref_fwonly | 5 | the same respell, name in FableWin.pdb only | HOLD (r455c); every gate passes |
| b2h_ctor_thingref_dupdef_hold | 3 | 00740340, 007710d0: another landed row already defines the PDB-spelled ctor with a body (NF281 D2); 0073fdb0: T3_PORT_QUEUE column 3 (nf298 unit) -- HOLD.tsv | T3; gates pass |

Land order: b1, b2, b3 (independent: no shared rows or targets). 35 rows ACCEPT (6 + 13 + 16), 27 distinct selves respelt (6 dtors, 21 ctors) plus 8 caller respells.

Batch layout (NF442 form): `rows/`, `orig/` (exact HEAD copies of rows and tests), `head/payload.json` (HEAD payload from recheck.py),
RESULTS.tsv, SELF_SYMBOLS.tsv (address, symbol, status, leaf_given, current_line), TABLE_CHANGES.tsv (header only: no binder lines),
UNITS.tsv, HEADERS.tsv (empty), CHECKS.tsv, evidence/ (vland HEAD/new logs, objproof). Tests are unchanged (no `tests/`): every
test is a standalone behaviour model that does not link the row.

## Method (every ACCEPT row)
1. Edit: the smallest change that makes the row DEFINE the PDB spelling (b1: `virtual`; b2: the parameter `CThing* x` -> `CThing& x`
   in declaration and definition, every use inside the ctor rewritten `*x` -> `x`, `x->` -> `x.`, `x` -> `&x`; the callee declarations
   are untouched). One `// Naming (NF455, r454b)` comment line under `// Leaf:` (or the top comment block). b1 also fixes two comments that
   said "non-virtual destructor" (007420e0, 0084b840).
2. Prechecks: clang `-fsyntax-only` of the HEAD source, the HEAD test and the new source (work/pre.log, all PASS) before any compile.
3. vland `--recheck-landed` HEAD payload (recheck.py) and new payload at ONE LANDVERIFY_WORK path (tools/runpair.sh, after NF442's).
   Gate: same grade (RELOCATION_MATCH), test PASS both, the per-row WARN set identical HEAD v new, 0 LITERAL.
4. objproof (tools/objproof.py) on vland's own HEAD and new objects: every non-debug section's bytes and relocations equal under the
   symbol renames, weak externals included, `.drectve` included. Gate: ALL_EQ with exactly the one self rename.
5. Binder census BY TARGET ADDRESS in reloc_bindings.tsv (column 2), plus a retail call-site scan (nftools/xref.py) mapped to catalog
   rows (tools/owner_of.py), plus the HEAD objects of every landed caller found (UND symbols). **The table misses binders:** the 17 landed
   `CTCX_Alloc` factories that call b2/b3 targets have no table line at those targets; 8 of them bind the old self spelling exactly (they
   became b3's units), 9 bind other stand-in spellings (`(void*)`, a namespace-less class, `CPlayerGuiDef*`), unresolved before and after. b1: 0 table lines at the 6 targets; the only retail callers are the 6 classes' scalar deleting destructors
   (landed, each calling a free `<Class>_dtor` stand-in; they never bound the QAE spelling and are unchanged -- port debt, below).
6. Sandbox (`$W/sandbox`: copies of reloc_bindings / self_symbols / descriptive_names, ghidra_out linked): SELF_SYMBOLS applied
   (tools/sbapply.py), `FABLE_ROOT=<sandbox> apply_table.py TABLE_CHANGES.tsv --dry` 0 misses, multi-target 1 -> 1, and the sandbox copy
   of descriptive_check.py: 0 FAIL. The repo table was compared byte-equal afterwards.
7. Names: every ACCEPT self symbol is in both the Ego_r and the FableWin PDB name sets (none Ego_d-only); the old spelling is in neither.
   Per-row tie (b2/b3): retail `ret 4` = one 4-byte argument; the row emits and stores its own class's vftable `??_7<Class>`; for the
   6 ctors with two PDB spellings (all in b2) the second spelling is a longer overload of the same ctor (`(CThing&, float)` etc.),
   ruled out by `ret 4`; none of them is a copy ctor.
   Old-name census over landed sources (`X(CThing*` per class): only multi-argument overloads, factories already measured, and
   0x008ED950, which binds `??0CModeDazed@@QAE@PAVCThing@@@Z` (global scope) at 0x00740160, the landed CModeStunned ctor: an identity
   question for that row, not a binder of 0x00740250. Screens: T3_PORT_QUEUE by column (0073fdb0 in column 3 -> held; 004e7cc3,
   007e5f00, 008d9220 only in column 4, not reserved, NF316 Q3), Codex pass 28 reserved, Account B pending and claims: no other hit.
   New-name census over landed sources: 0x00740420 and 0x007710A0 define two of the new names with
   bodies (held as b2h); the other hits are bodiless declarations that now resolve. Sandbox self/table clash check: 0.
8. Link (b3): `/FORCE` link of the 16 HEAD objects and of the 16 new objects: 0 duplicates both, the same 8 unresolved base ctors both;
   the unit ctors resolve in both sets.
   No target is in descriptive_names.tsv, so REGISTRY_CORRECTIONS.tsv is empty.

## Destructors (27 input lines)
| class | rows | addresses | outcome |
|---|---:|---|---|
| A: novtable model, pure rename | 6 | 007420e0 007e7ed0 0084b840 00bba140 00bcde40 00bdfd80 | b1_dtor_novtable (ACCEPT) |
| B: the model stores and emits its own vftable | 16 | 00446be0 00549a00 00762480 00810fe0 00811040 00811be0 00884b60 00987e20 009be1e0 009d5ee0 00a24930 00a24f30 00a5c6e0 00a65690 00a752f0 00af5900 | held: RULING r455a |
| C: access UAE -> MAE | 1 | 00a85ff0 | held: RULING r455b (unit with 0x00A86F60) |
| D: model not polymorphic | 3 | 004e108c 006c1be0 0077ee70 | port: `virtual` adds a vptr and moves every offset (DIFFER 35v28, 66v59, 103v97) |
| E: reject | 1 | 00a3b970 | the PDB QAE is a revision difference: in retail CTextureRenderer derives from CBaseVideoRenderer (virtual dtor), so VC7.1 can only mangle UAE (the row's own notes, test-compiled); the PDBs' class had no base |

- Class B evidence (`evidence/dtor_slots_classB.txt`): adding `virtual` makes VC7.1 emit `??_G<Class>` (+ `??_E` alias, UND `??3`) and
  grows the emitted `??_7<Class>@@6B@` by one slot: the model's placeholder `v00` stays slot 0 and `??_E` lands at slot 1 (00a5c6e0:
  slot 2), while retail keeps the deleting dtor at slot 0 (00a65690: slot 15). So the vftable changes: the standing "a vtable changes:
  stop" case (nf297 stopped 00446be0 on it). The landed scalar-deleting-destructor rows of these classes carry invented selves
  (`?ScalarDeletingDestructor@C<Class>@@QAEPAXI@Z` or free `?<Class>_scalar_deleting_destructor@@YIPAX...`), so the real fix is a
  per-class port: the dtor in slot 0 of the local model (the base's dtor virtual too) and the deleting-dtor row on `??_G<Class>@@UAEPAXI@Z`.
- 0x00a85ff0 (class C): `protected:` before the virtual dtor gives ALL_EQ under three renames `??1 / ??_E / ??_G ...@@UAE` -> `@@MAE`
  (`evidence/objproof_00a85ff0.txt`; `??_ECPhysicsFacesSegmentBranch@@MAEPAXI@Z` is itself in the Ego_r name set). The emitted vftable is
  byte-equal but one of its slot symbols is renamed, and the landed deleting dtor 0x00A86F60 defines `??_G...@@UAEPAXI@Z`; its vftable
  model also spells HasChildren/Sizeof/GetNoFaces UAE non-const where the landed selves are MBE. Land 00a85ff0 + 00A86F60 as one unit if
  r455b allows a vftable slot rename.
- FableWin-only names (one PDB set): 006c1be0, 00811be0, 00884b60, 00987e20, 009be1e0 (all outside b1).

## Systematic findings (no compiles; `evidence/classify3.tsv`, `evidence/arity.tsv`)
The real_symbol column holds `A | B | C` when the PDB has several spellings (224 lines 1 spelling, 163 two, 69 three or more); each line
was matched against every spelling and the one needing the fewest normalizations kept.

| pattern (minimal normalization) | lines | notes |
|---|---:|---|
| pointer vs reference | 166 | 160 ctors: `(CThing*)` -> `(CThing&)` 52, `(CThingCreatureBase*)` -> `&` 55, two-arg creature actions the rest |
| `CPersistContext::Transfer<T>` overload | 58 | ops: the PDB spelling takes a `char const*` name (and often a default `T const&`); retail `ret N` matches the LANDED form, not the PDB one, in all 58: a different overload, not a respell |
| access only (public -> private / protected) | 50 | 44 methods, 5 ctors, 1 dtor; the q1 friend recipe applies |
| virtual only | 26 | the destructors above |
| wchar_t (`_W`) vs unsigned short (`G`) | 15 (+1 with enum, +1 mixed) | see below |
| const | 15 | 9 ops |
| enum vs int/long | 10 | 8 ctors |
| ptr/ref + const / access / enum | 13 | |
| other parameter types (stand-in classes, base-type changes e.g. `CThing*` -> `CThingCreatureBase&`) | 64 | per-row work |
| scope differs | 10 | e.g. `CTools` vs `NCameraModes::CTools` |
| parameter count differs | 8 | |
| STL scope (std:: vs landed stand-in) | 7 | |
| PDB spelling is the copy ctor | 6 | a different function: not a respell |
| stand-in type/scope, cc, classkey | 5 | |

- **wchar.** 17 lines have `wchar_t` in at least one demangled PDB spelling (7 methods, 2 ctors, 8 ops); for 15 of them the
  best-matching spelling differs from the landed one ONLY by `unsigned short` -> `wchar_t` (`G` -> `_W`), 1 adds an enum, 1 has other
  type changes. The 15: 5 single-spelling lines (00406e30 CreateSaveGame, 00437260 CWStringParser, 0048dbf9
  GetNumberOfCharactersOfTypeInString, 00a0fd90 WordWrap_SetCallback, 00a10310 WordWrap_FindNonWhiteSpaceForward; all in both Ego_r and
  FableWin), the 8 CWideString operators (`!=`, `=`, `+=`, `+`: three PDB overloads each, so a tie is needed per row), and the
  GFStringToFloat / GFStringToInt `wchar_t*` overloads. (An earlier substring count of `_W` gave 23; 6 of those were class names such
  as `CCreatureAction_WalkInto`.) The PDBs keep `G` elsewhere (e.g. `CArray<unsigned short>` in CPhysicsFacesSegmentBranch's ctor), so
  the donor builds used native `wchar_t` (/Zc:wchar_t) and the source distinguishes the two types; retail's RTTI strings contain neither
  `_W` nor `G` template arguments, so the bytes cannot settle retail's flag. A compile-flag / typedef question, not per-row naming:
  NF305 Q3 makes a `_W` need T3. No flags were changed.
- **Arity screen** (PDB stack bytes v retail `ret N`, best spelling): ctor 246 TIE / 10 MISMATCH; method 66 / 1; op 22 / 65 (58 are the
  Transfer overloads); dtor 22 TIE (5 with no ret at the end). A TIE is necessary, not sufficient.

## Holds and rulings needed
- **r455a (16 dtors, class B):** may a destructor self respell add `??_G` and a vftable slot in the dtor's TU, or must each class be
  ported (dtor in slot 0, base dtor virtual, deleting-dtor row respelled to `??_G<Class>@@UAEPAXI@Z`) as one unit?
- **r455b (0x00a85ff0 + 0x00A86F60):** a vftable slot symbol renamed `??_E...UAE` -> `MAE` with the table bytes equal.

## Port debt seen (not changed)
- The 6 b1 classes' scalar deleting destructors (0x007420C0, 0x007E7EB0, 0x0084B820, 0x00BBA240 + 5 sibling particle groups,
  0x00BCDE20, 0x00BDFD60) call free `__fastcall <Class>_dtor` stand-ins and have invented selves; none is in the binding table.
- b2 rows keep their base-ctor callee declarations (`CTCBase(CThing*)`, `CModeBase(CThing*)`, stand-in bases such as
  `CModeBase_00861e00`); several are wrong spellings of `(CThing&)` PDB ctors (T1 binder work, not this ruling). 004e7cc3, 0073fdb0,
  007e5f00 and 008d9220 are named as binders in T3_PORT_QUEUE reason columns (not reserved, NF316 Q3); their callee lines are unchanged.

## Systematic finding about the binding table
- reloc_bindings.tsv is not a complete binder census: the factory rows above, and the 11 deleting-dtor rows of b1's classes, have no
  line at their callee targets. Census by retail call sites + compiled HEAD objects, never by the table alone.

## Locks
Kept (rows in batch folders): b1 6, b2 13, b3 16, b2f 5, b2h 3 = 43. Released: the 20 non-batch dtor rows and the 9 non-binder Alloc rows.

## Resume point
- Next group: the 23 remaining `(CThing&)` ctors (the table shows one binder each, but the table misses factory binders: census by retail
  call site + compiled HEAD objects; unit = row + its callers, p -> *p at the call site, NF313 Q2), then
  `(CThingCreatureBase&)` creature actions (55), then the 50 access-only lines (q1 friend recipe).
- Tools: tools/classify3.py, arity.py, dtoredit.py, predit.py, runpair.sh, objproof.py, sbapply.py, dtorslots.py, owner_of.py, nf.py.
