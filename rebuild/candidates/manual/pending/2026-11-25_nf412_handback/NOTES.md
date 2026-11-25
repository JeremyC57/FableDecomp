# NF412 hand-back (2026-11-25): forwarder GetClassTypeName rows respelt in the nf406_b1 form

## Update after NF414 (HEAD 2ddf623e62): batch b4 added
- NF414 landed real in-class names for 27 statics. 25 of my 28 held callers call one of them (all but 0x004D3F30, 0x004D4212,
  0x004E230A, whose statics 0x004D3EF9 / 0x004D41DF / 0x0049D450 NF414 did not touch). 0x004D4369 (CTCDExperienceOrb) has no forwarder
  caller (byte census); its GetClassTypeName 0x0077AB70 is an expanded body, like 0x0072A710 (T3).
- Re-sync: census and worklist rebuilt from the new HEAD (evidence/census_all.tsv, worklist.tsv: 142 REAL, 3 STANDIN; nothing else changed);
  b1..b3 HEAD copies (rows, tests), their statics' sources and RegisterTCs re-diffed: unchanged. RegisterTCs recompiled: same code.
- **batches/b4: 25 ACCEPT** (0x004D2B67, 0x004DA926 .. 0x004DAD92, 0x004DB3BB, 0x004E3457), same form, tools and gates as b1..b3:
  - 19 local classes over CTCActionUseBase.h / CTCCarriedActionUseBase.h / CTCDriverBase.h; 6 layout-rename (CTCRotate, CTCActionUseWindow,
    CTCTargeted + FABLE_RETAIL_CTCTARGETED, CTCActionUseReadable + CTCACTIONUSEREADABLE_PASS24 / CREADABLEDEF_USEINFO_PASS24, CTCActionUseBed +
    BED_USE_PASS28, CTCDExplosiveTrail + EXPLOSIVE_TRAIL_UPDATE_PASS28; header opt-ins only).
  - vland HEAD/new: 25 RELOCATION_MATCH PASS both; new WARNs none on 19, SHARED_HEADER_AVAILABLE on 6. objsec / relocs / pk / pvr / reach flips
    clean; mutation 25/25; /FORCE link with and without the binder 0 -> 0 duplicates, NEW_UNRESOLVED 0; 23 rows gain the libcp directive.
  - TABLE_CHANGES: 9 adds, the first lines for the statics RegisterTCs expands (2A91 2B04 2B7A 2B8D 2BA0 2BB3 2BC6 2BD9 2BEC), and 1 delete,
    ?Init_004d2b17@T@@QAEXXZ -> 004d2b17 (witness 004d2b67 only; the real CTCRotate spelling is already bound there by 004ee23f).
    apply_table --dry: 0 misses, multi-target 1 -> 1; all four batches together: 12 adds + 2 deletes, 0 misses, nothing new.
  - reserved_check (RESERVED_REFS empty): 0.
- /FORCE link re-run on the new HEAD for b1..b3 (same result, 0 -> 0, every callee resolved) and for all 142 rows at once
  (evidence/link_all142_binder.txt, link_all142_nobinder.txt): duplicates 0 -> 0; 142 unresolved stand-in callees at HEAD -> 0.
- Held now: 3 rows (RULINGS_NEEDED r1); NAME_FIXES.tsv trimmed to those 3 statics and 3 callers.
- Locks: 142 held as NF412 (117 + 25). The 3 held rows are not locked.

The sections below were written for b1..b3 before NF414; where they say 28 held rows, read 3 (the other 25 are b4).

## Summary
- **117 ACCEPT, 0 HOLD in the batches; 28 rows held out of the batches (not touched, not locked; RULINGS_NEEDED r1).**
  - batches/b1: 50 rows (0x004D2DF7 .. 0x004DADE5), TABLE_CHANGES 1 add.
  - batches/b2: 50 rows (0x004DAE0D .. 0x004E03BF), TABLE_CHANGES 1 add + 1 delete.
  - batches/b3: 17 rows (0x004E07FB .. 0x004E7FC8), TABLE_CHANGES 1 add.
  - One unit per row (UNITS.tsv). Layout copied from agentNF406/batches/b1: RESULTS, SELF_SYMBOLS, TABLE_CHANGES, UNITS, rows.txt,
    rows/, tests/, orig/rows + orig/tests (HEAD byte copies, re-diffed against the repo just before hand-back: unchanged),
    head/payload.json + oracle.tsv, logs/, evidence/.
- Each row goes from the extern "C" `__stdcall` stand-in `_<Class>_GetClassTypeName@4` calling the made-up `?Init_<static>@T@@QAEXXZ`
  (116 rows), or from `?GetClassTypeName@CTCTargetingSpirit_Calls@@QAE?AUCTargetingResult_Retail@@XZ` calling
  `??0CTargetingResult_Retail@@QAE@XZ` (0x004DEC09), to `?GetClassTypeName@<Class>@@UBE?AVCCharString@@XZ` (namespaced:
  `?GetClassTypeName@<Class>@NThingShot@@UBE...`) calling the landed static `?StatGetClassTypeName@<Class>@@SI?AVCCharString@@XZ`.
- All 117 locks are held as NF412. No header edits, no FLAGS change.

## Worklist (step 1)
- Census from bytes, not names (evidence/bytes_census.tsv): every landed source address whose oracle is the 16-byte forwarder
  `8b4c2404 e8xxxxxxxx 8b442404 c20400`. 149 rows; every call target is a landed StatGetClassTypeName row.
  - 4 are already real (nf406_b1). 145 are stand-ins (evidence/worklist.tsv).
  - Cross-check: retail slot 22 of the 340 CTCBase-derived vftables covers all 149 (each row's address is its class's slot 22).
  - self_symbols.tsv is not a reliable index (it holds 42 of the 312 static rows), so it was not used to find rows.
- The 145 split by callee:
  - **117: the static is landed under its real name** (object symbol of the static's HEAD source = `?StatGetClassTypeName@<Class>@@SI...`,
    evidence/static_bindings.tsv). These are b1..b3.
  - **28: the static is still a stand-in** (evidence/hold28.tsv). Held, see "Held rows".
- Against NF408's standin_callers.tsv (113): all 113 are in b1..b3. The other 4 of the 117 are 0x004D427E, 0x004D42D2, 0x004DF1AC (their
  statics carry an abbreviated `// Leaf: StatGetClassTypeName@<Class>` line, so a Leaf-based scan missed them) and 0x004DEC09 (a
  `??0CTargetingResult_Retail` callee, RELOC_NAME of another shape). The 28 held rows are the rest of NF406's 144 count.
- Out of scope: 188 landed GetClassTypeName rows with the 25-byte expanded body (no call; `_Methods` / non-virtual QAE selves). They are not
  the forwarder form. T3 line.

## Names and sources (per row in RESULTS.tsv)
- (1) Ego_r.pdb and FableWin.pdb name sets both contain the exact new self spelling (all 117 checked by script, evidence/worklist.tsv
  columns in_egor / in_fwin). The sets are keyed by donor address and are used as name sets only.
- (2) Retail vftable slot 22 (rebuild/manifest/rtti-vtables.tsv; the class of every vftable equals the row's class, checked) points at the
  row's address. Slot 22 is CTCBase's pure `GetClassTypeName` (engine/CTCBase.h polymorphic branch).
- (3) The body calls the landed static of the same class (call at row+4).
- ABI: `UBE?AVCCharString@@XZ` is exactly the retail bytes: const virtual thiscall, `this` unused, hidden result at [esp+4] forwarded in ecx to
  the static `__fastcall` (`SI`), `ret 4`.

## Row texts (tools/gen412.py)
- The nf406_b1 text: Leaf line, retail bytes, identity, respell line, header line; `FABLE_RETAIL_CCHARSTRING` + fable_string.h;
  `FABLE_RETAIL_CTCBASE` (polymorphic branch); the class with `static CCharString __fastcall StatGetClassTypeName();` (declaration only)
  and `virtual CCharString GetClassTypeName() const;`; `#pragma optimize("s", on)` (kept from HEAD) before `return StatGetClassTypeName();`.
  HEAD rows had no `// Flags:` line and no other pragma; none was added.
- **63 layout-rename rows** (the CTCBerserk 0x004D4A86 form) over the header that defines the class (namespace-aware: e.g.
  NThingShot::CTCShotDefinitionThrow is defined in engine/NThingShot_CTCShotDefinitionThrowPredictive.h; CTCSmashableWindow in
  engine/CTCSmashableBase.h; CTCSingleUserActionPoints in engine/CTCUserActionPointBase.h).
- **54 local-class rows** over the nearest RTTI ancestor with a header (30 over engine/CTCBase.h, 7 CTCCarriedActionUseBase, 5
  CTCCameraPointGeneralCase, 3 CTCActionUseBase, 2+2 NThingShot bases, 2 CTCGatherPointBase, 1 each CTCDriverBase, CTCSmashableBase,
  CTCContainerRewardHero). The header line says when the direct base has no header.
  - The four CTCActivationReceptor* rows (0x004DE64D, 0x004DE67B, 0x004DE6A3, 0x004E07FB) sit on engine/CTCBase.h: their direct base's
    header, engine/CTCActivationReceptorBase.h, is a generated flat layout that never derives from CTCBase (T3).
- Namespaced classes (NThingShot, 11 rows) are declared inside `namespace NThingShot { }` in row and test.
- **Prelude ladder.** P0 = FABLE_RETAIL_CTCBASE; P1 = + CINTELLIGENTPOINTER / CTHING / engine/CThing.h; P2 = + CCLASS / DEF_CLASS_BASE /
  ETCInterfaceType.h; P3 = P1 + P2. Each row keeps the first prelude whose check TU compiles (73 P0, 43 P1). When the class sits behind a
  header opt-in, that opt-in is defined (header opt-ins only, no header edits):
  - 004D4189 GIVE_ITEM_ACTION_PASS26; 004D6112 CTCSLEEP; 004D61F6 CTCWOBBLE_PASS22; 004D7DAC CTCEXCLUSIVEUSE; 004DAED6 CTCINVENTORYITEM;
    004DB27D CTCCARRIEDACTIONUSEFISHINGROD_PASS22; 004DB2A5 CONTEXTSENSITIVEITEM_QUERY_PASS28 + CTCCARRIEDACTIONUSEFISHINGROD_PASS22 (+ a
    `class CThingCreatureBase;` forward declaration the header needs); 004DB353 CTCKRAKENTENTACLE_TRACK_PASS26 + C3DVECTOR (C3DVector.h);
    004DB565 CTCHASNAME_NPCSHORTNAME; 004DC779 CTCSTOCKITEM_TARGETING + CENGINEPERSISTENTPRIMITIVEBASE; 004E036C CTCUSERACTIONPOINTBASE;
    004E03BF CTCNAMEDUSERACTIONPOINTS; 004E0D20 CTCPRECALCULATEDNAVIGATIONROUTE_LOAD_PASS26; 004E3D02 CTCENTITYEVENTS; 004E5F41
    KID_EVADE_PASS23; 004E7D6A CTCINVENTORYCLOTHING_CALCULATE; 004E338C NYMPH_ATTACK_PASS27 + CTHINGPHYSICAL / CTHINGGAMEOBJECT /
    CTHINGCREATUREBASE (CThingCreatureBase.h, so CNymphDef / CThingCreatureBase are complete for the header's inlines);
  - the 7 NThingShot rows over the shot-definition headers: CTCSHOTDEFINITIONBASE_MOVEMENT (the headers' own retail-size asserts need it);
  - 0x004DB072 CTCExpression: the landed includer 0x007EF200's define list + CThing.h, CThingCreatureBase.h, CExpressionDef.h. Every
    shorter prelude made cl hang (> 240 s, measured); this one compiles in under a second (T3).
- **Polymorphic proof, per row** (batches/<b>/evidence/polymorphic_ctcbase_check.txt and evidence/chk/<a>_chk.cpp): the check TU is the
  row plus `static_cast<CTCBase*>((Class*)0)`, `&CTCBase::GetClassTypeName` (exists only in the polymorphic branch) and
  `sizeof(Class) == sizeof(layout or base)` (no second vptr). All 117 compile. Controls (evidence/polymorphic_control.txt, b1): 0x004D2DF7
  and 0x004D548A with FABLE_RETAIL_CTCBASE dropped fail (C2039 GetClassTypeName not a member of CTCBase; C2118 on the size check).
- **Objects** (evidence/object_symbols.txt): each defines only the leaf and has one UNDEF, the class's static. objsec: leaf .text 16 bytes,
  data and characteristics equal to HEAD's, same COMDAT selection, no section added or dropped. One relocation, same offset and type; its
  target moves from the old callee to the static (evidence/relocs.txt). 59 rows (b1 12, b2 35, b3 12; objsec.txt) gain
  `/DEFAULTLIB:"libcp"` in .drectve: a C++ standard header behind the engine header (STLport). No code or data change. Precedent: the
  landed nf406_b1 row 0x004DDA94 (layout-rename over engine/CTCTargetingAI.h), compiled from HEAD, carries the same
  `/DEFAULTLIB:"libcp"` (checked with objdump). Allowed by gates412.py and recorded per row (NF408's gate held any .drectve delta, but
  its rows added no header).

## Tests
- HEAD tests were bare printf (116) or a self-contained model (0x004DEC09). Each new test (the nf406_b1 test, generated):
  - links the row object and supplies the class's static as a stub that returns the retail literal (read from the static's row);
  - CCharString has a user ctor, so it returns through a hidden slot, as in the row;
  - calls through a const reference and asserts one static call, the text, and construction into the caller's own slot (`g_slot == &r`).
  - Pass patterns kept from the catalog (`PASS_<addr>`; `004dec09_TEST PASS` for 0x004DEC09).
- Mutation control (tools/mut412.py, evidence/test_mutation.txt in each batch): the static called twice. 117/117 real PASS, mutant FAIL
  (`calls=2`).

## Gates (per batch; tools/runpair.sh, gates412.py, mut412.py, linkgate412.py)
- **vland** HEAD/new at one LANDVERIFY_WORK path per batch (work/wd_<b>, moved to wd_<b>h / wd_<b>n; logs/vland_head.log, vland_new.log;
  evidence/vland_cmp.txt):
  - HEAD 117 RELOCATION_MATCH PASS (WARN RELOC_NAME on every row, GENERIC_STRUCT on some). New 117 RELOCATION_MATCH PASS. No grade drop.
  - New WARNs: none on 62 rows; SHARED_HEADER_AVAILABLE on 55 layout-rename rows (the queued verifier false positive, T3). No
    LITERAL_DIFF / LITERAL_UNCHECKED / RELOC_NAME / STANDIN_LEAF / GENERIC_STRUCT on any new row.
- **pk_all + pickany_vs_retail** HEAD and new (evidence/pk/, pk_summary.txt): no copies besides the leaf on either side, 0 NEQ; pkcmp346
  GATE PASS; reach flips 0 (evidence/reachflip.txt).
- **/FORCE link** (link377.py, VC7.1 link /DLL /NOENTRY /NODEFAULTLIB, response file; evidence/link_binder.txt, link_nobinder.txt,
  linkgate.txt):
  - with RegisterTCs 0x004EE23F + the batch's 50/50/17 statics + the rows: duplicates 0 -> 0 in every
    batch; NEW_UNRESOLVED 0; every new callee DEFINED (in its static's object).
  - without the binder (statics + rows, as NF407 did): duplicates 0 -> 0; NEW_UNRESOLVED 0; every callee resolves. HEAD left the stand-in
    callees unresolved; new resolves all.
  - All 117 at once (evidence/link_all117_binder.txt, link_all117_nobinder.txt): duplicates 0 -> 0 both; HEAD 117 unresolved watched
    names -> new 0.
- **apply_table --dry** (logs/apply_table_dry.txt): b1 1 add, b2 1 add + 1 delete, b3 1 add; 0 misses; multi-target 1 -> 1, nothing new.
  All three together: same.
- **reserved_check** (logs/reserved_check.txt): reserved 0, mentions 0 in each batch.

- **Where these gates differ from NF406's:** the pk "new" side ran against the repo's reloc_bindings.tsv, not a troot with the batch's
  TABLE_CHANGES and SELF_SYMBOLS applied (with zero copies on either side the result cannot differ); the statics and the binder in the link
  sets are plain VC7.1 compiles (tools/cc.py, /O2 /Oy /W3 as recheck.py uses) of their current HEAD sources, not vland objects.
- **Alternate text** (not the primary): 10 engine headers already declare both members on the real class. A direct-include text (no
  layout rename) for 9 of them is under batches/<b>/evidence/alt_direct/ (*.cpp.txt + SUMMARY.txt): it compiles to the same .text bytes,
  leaf and one UNDEF static as the batch object (not run through vland). 0x004E761D CTCHitLocations: the direct text does not compile.

## Table (steps 3 and 4)
- Old spellings, fixed-string grep of rebuild/src, rebuild/oracles, rebuild/include, rebuild/tests, rebuild/manifest, decorated and undecorated
  (`<Class>_GetClassTypeName`, `Init_<addr>`, `CTargetingResult_Retail`, `CTCTargetingSpirit_Calls`) (evidence/oldspell_grep.txt):
  - outside the rows' own files only self_symbols.tsv (each row's line; SELF_SYMBOLS.tsv replaces them) and reloc_bindings.tsv line
    `??0CTargetingResult_Retail@@QAE@XZ -> 004d5853` (witness 004dec09 only) -> **delete** in b2 (the real static spelling is already bound
    at 0x004D5853 by 004ee23f, so a replace would duplicate that line).
  - CTCTargetingSpirit_Calls stays in other rows' texts (0x004DEBFF and four tests): a different stand-in for other functions (T3).
  - No vftable data row binds any old self.
- Statics the respelt callers now bind (evidence/static_bindings.tsv): 113 already bound under the real spelling (by 004ee23f);
  **3 adds**: CTCUndeadSoul -> 004d2dc8 (b1, witness 004d2df7), CTCInventoryItem -> 004d30db (b2, witness 004daed6), CTCInventoryClothing
  -> 004d2ea7 (b3, witness 004e7d6a). No static address carries another spelling (except 0x004D5853's deleted stand-in).
- The new self names are spelled nowhere else in rebuild/ except as comments in 5 engine headers that already declare them.

## Held rows (RULINGS_NEEDED r1)
- 28 forwarders whose static is a stand-in (evidence/hold28.tsv): 21 CTCActionUse* (14) / CTCCarriedActionUse* (7) rows, CTCRotate and
  CTCTargeted (free `?<Class>_StatGetClassTypeName@@YIPAX...` statics), CTCDRumble (`?StatGetClassTypeName_8@CCharString@@QAEPAV1@XZ`),
  CTCDExplosiveTrail (`@StatGetClassTypeName_9@4`), CTCTargetingPlayer (`??0CTCTargetingPlayer@@QAE@XZ`), CTCDNone and CTCDNavigationSeed
  (`??0CThingComponentDefNameTag@@QAE@XZ`).
- Their statics are proven (NAME_FIXES.tsv): FableWin.pdb name set (not in the Ego_r set), the retail static shape with the literal = the class
  name (read from retail), the caller's vftable slot 22; 18 are already bound under the real spelling by 004ee23f. The callers' names are
  proven as for the 117 (both name sets + slot 22).
- Not respelt: the static would need its own respell (and literal fix), and the caller alone would leave the callee unresolved without the
  binder. Locks not taken.

## Callee names
- Every callee is the landed static's own object symbol (compiled from its HEAD source, evidence/static_bindings.tsv column 3), so one
  address keeps one name.

## Budget / process
- Generator compiles: one ladder pass over 117 rows (up to 4 check compiles each), one opt-in retry for 27 rows (23 opt-in + 4 receptor rows), a second for 5, a third for
  CTCNymph; CTCExpression by hand (4 compiles). vland: one pilot pair (4 rows) + one pair per batch.
- cl hangs are real for some preludes (measured: the same file times out alone at 30 s and 240 s); the ladder uses a 35 s limit.
- **Rule slip, reported:** my first reserved_check run did not set RESERVED_REFS, so the script ran `git ls-tree` (read-only) internally once
  per batch. I re-ran it with RESERVED_REFS empty (no git); the logs in the batches are from the git-free runs, and the counts are the same.
- No wineserver -k; only my own processes. Tools: tools/ (census.py, worklist.py, gen412.py, mkbatch412.py, runpair.sh, gates412.py,
  mut412.py, linkgate412.py, finalize412.py, plus copies of NF406/NF408 helpers).
