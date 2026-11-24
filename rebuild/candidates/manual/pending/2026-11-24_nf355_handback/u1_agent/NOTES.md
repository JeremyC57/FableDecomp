# NF355 hand-back: CThingNoiseDef destructor class port (T3 nf334 0x0045C5F6) + 0x007FC800 EndAttackBeam, Account A naming queue, 2026-11-24

Folder `$S/w22/agentNF355/` (S = the 40effb6c scratchpad). Lock name NF355.

## Result

| item | rows | status | where |
|---|---|---|---|
| 1-2 class model + implicit destructor | 0x0045C5F6 | RELOCATION_MATCH (vland PASS), all copies EQ, **HOLD for rulings R1-R4** | `held/u1` |
| 3 binder | 0x0045C5DA | objsec ALL_EQ under one rename, vland PASS, HOLD (unit) | `held/u1` |
| 4 EndAttackBeam + callees | 0x007FC800 | vland PASS, objsec equal except `.drectve` + libcp, HOLD (unit) | `held/u1` |

Nothing is in `batches/`: every row passes its measured gates, but the unit needs rulings before it lands (a new LNK2005, a row that
authors an uncatalogued function, a pick-any device, and opt-ins that edit existing header lines).

## The class model (item 1)
- **Base chain (RTTI + Ego_r):** CThingNoiseDef : CThingBaseDef : CParentDefClassBase : CDefClassBase : CDefPointeeBase : CResource.
  Noise (CTCNoiseDef : CSubDefClassBase : CDefClassBase : ...) sits at Ego_r +0x74 and retail +0x70. The +0x70 store is Noise's own
  CDefPointeeBase base: retail stores 0x01230BA0 there, which is ~CDefPointeeBase's inline vptr reset.
- **0x00454B4C = `??1CThingBaseDef@@UAE@XZ`.**
  - FableWin.pdb names it, and FableWin's ~CThingNoiseDef calls it. Ego_r inlines it, so that set is silent (NF348 D2).
  - The retail body matches FableWin's calls: `~CThingComponentSet` 0x00450BA0 on +0x39, then `jmp ~CParentDefClassBase` 0x00430300.
  - It has no vtable store, so it is compiler-generated.
  - Its landed self is the free stand-in `?Dest_val@@YIXPAUPair@@@Z` (NAME_FIXES).
- **Size 0xA4** is stated three times in retail: GetSizeofClass 0x00455F14 `mov eax,0A4h`, Alloc 0x0045C5C1 `push 0A4h`, and the ctor
  0x00455EF0.
  - The generator quarantined the class for donor 0xA8 against GetSizeofClass 0xA4.
  - The header's retail layout already is 0xA4, so the quarantined text is promoted unchanged as the default branch.
- **Headers** (`hdr/engine/`, originals in `orig/hdr_*`; HEADERS.tsv):
  - `CThingNoiseDef.h` (new):
    - default branch: the generator's flat text;
    - `FABLE_RETAIL_CTHINGNOISEDEF_VIRTUAL_MODEL`: the polymorphic class, with an inline ctor, Alloc, Transfer / Copy / GetSizeofClass
      (slots 18-20), Noise, and asserts;
    - binder opt-in `_DTOR_DECL`.
  - `CThingComponentSet.h` (new): the generator's output plus an add-only `~CThingComponentSet();` declaration.
  - `CThingBaseDef.h`: `_COMPONENTS` types Components, editing the existing line under `#ifdef`/`#else`. `_CTOR_DECL` is add-only
    (`??0CThingBaseDef@@QAE@XZ`, both PDBs, 0x006772D0).
  - `CTCNoiseDef.h`: `_IMPLICIT_DTOR` wraps the existing `virtual ~CTCNoiseDef();` line. Only FableWin.pdb has `??1CTCNoiseDef`, and retail
    never calls it out of line.
- **equiv_check** of old against new over the closure of CThingBaseDef.h + CTCNoiseDef.h (978 rows, nf352 closure.py): 0 differing.
  - The planted control flags 526 rows.
  - The other 452 rows reach the headers only through conditional includes: in a sample of 4, the planted token is absent from their
    preprocessed text.
  - The two new headers have no landed includer.

## The emitter (item 2): faithful form found, no forcing device
- **Retail's form:**
  - Ego_r puts Alloc, the ctor, the deleting dtor and the dtor of CThingNoiseDef in one object, `definition_manager.obj`. InitAndCompile is
    in the same object.
  - Retail places Alloc 0x0045C5C1, then `??_G` 0x0045C5DA, then `??1` 0x0045C5F6.
  - So the row is `CDefClassBase* CThingNoiseDef::Alloc() { return new CThingNoiseDef; }` on the header model. The inline ctor's vftable
    store emits the vftable, then `??_G`, then the implicit `??1`. Leaf `??1CThingNoiseDef@@UAE@XZ` (GOTCHAS agentFAM13).
- **Every function the TU emits equals retail** (both checkers, `--leaf`, logs/pk_final.txt):
  - Alloc = 0x0045C5C1;
  - `??0` = 0x00455EF0;
  - `??_G` = 0x0045C5DA;
  - `??1CThingBaseDef` = 0x00454B4C;
  - `??0CTCNoiseDef` = 0x0044C1BB;
  - `??_GCTCNoiseDef` = 0x0044F71B;
  - `??_GCDefPointeeBase` = 0x0042D9A0;
  - 4 NOADDR inline def-chain dtors.
- **Vftables:** the three emitted vftables match retail slot for slot (logs/vtable.txt). Slot 0 is `??_E`, VC7.1's alias of `??_G`.
- **The leaf** is HEAD's bytes and relocations under the two UND renames (logs/leafcmp_0045c5f6.txt).
- **Measurements** (evidence/measure/MEASUREMENTS.txt; none is a candidate):
  - at /O2 the dtor is expanded into `??_G` and no `??1` exists (a1, a3, p1). The size pragma is needed; the def TU rows use it.
  - a declared `virtual ~CThingBaseDef();` was not needed: the faithful implicit form (typed Components) gives the tail jump.
  - the constructor-TU form was not measured. It cannot avoid a second strong `??0`, and Alloc already reproduces every body.

## Rulings needed
- **R1: Alloc authored in the emitter row.**
  - 0x0045C5F6's TU strongly defines `?Alloc@CThingNoiseDef@@SIPAVCDefClassBase@@XZ`, retail 0x0045C5C1. It has no catalog row and is
    EQ to retail.
  - It resolves InitAndCompile 0x0044C72B's reference (table line `?Alloc@CThingNoiseDef...` 0045c5c1, witness 0044c72b; unresolved at
    HEAD).
  - It is retail's own body and name, not MakeOne. But a later 0x0045C5C1 row must not define it again.
  - The 12 landed sibling Allocs (e.g. 0x0045C5AB) are free `X_Alloc` stand-ins with the ctor out of line, so they emit nothing.
  - Accept, or require an Alloc row first?
- **R2: new duplicate with the ctor row.**
  - The link of the unit plus 00455ef0, 00923f60 and 0044c1bb (logs/link_u2.txt) gives a new LNK2005, `??0CThingNoiseDef@@QAE@XZ`:
    0x00455EF0's strong stand-in self against this object's pick-any copy.
  - The copy is EQ to retail 0x00455EF0. The owner's relocations name stand-ins (`??0CMemberBase_0044c0c0`, a throwaway vftable) at the
    same retail addresses.
  - The data COMDAT `??_7CThingNoiseDef@@6B@` is 1 slot (`?v00`) in 0x00455EF0's object against 25 retail-exact slots here
    (logs/vtable_copies.txt). The same holds for `??_7CTCNoiseDef` against 0x0044C1BB / 0x00455EF0.
  - `??0CTCNoiseDef` was already LNK2005 at HEAD (0x0044C1BB strong against 0x00455EF0's copy). This adds one more EQ pick-any copy.
  - Direct comparison (logs/owner_copies.txt):
    - `??0CThingNoiseDef`: 0x00455EF0's HEAD copy and this one have equal bytes and relocations under one rename
      (`??0CMemberBase_0044c0c0` = `??0CSubDefClassBase`, both 0x0044C0C0);
    - `??0CTCNoiseDef`: 0x0044C1BB's copy equals this one under `??0CShoveHaPennyRegionDef` = `??0CSubDefClassBase`.
    - The owners' `??_7CThingNoiseDef` / `??_7CTCNoiseDef` relocations carry the same names, but resolve to their own 1-slot COMDATs.
  - Precedent: EXTRA_RULES 2026-11-20 size-pragma ("one LNK2005 with identical bytes; queue the owner's port").
  - Proposal: queue the 0x00455EF0 / 0x0044C1BB ports onto these headers (NAME_FIXES).
- **R3: pick-any device.**
  - Under the row-head pragma (form S, evidence/measure/formS_0045c5f6.cpp), `??_GCDefPointeeBase` is the /Os body (`pop ecx`): NEQ
    with retail 0x0042D9A0 (`add esp,4`).
  - That copy is reached from the leaf through `??_7CDefPointeeBase`. pk_all prints reach=n only because its walk skips data COMDATs.
  - The landed row is form p3: `engine/CDefPointeeBase.h` is included above the pragma, and that copy is then EQ.
  - Form S against p3 (logs/formS_v_p3.txt): two sections change, both from /Os to /O2 mode:
    - `??_GCDefPointeeBase` changes bytes and alignment;
    - `??1CDefPointeeBase` (NOADDR) changes only its alignment flag, with the same bytes.
  - The size-pragma scaffolding rule (EXTRA_RULES 2026-11-20), condition 2: owner 0x0042D9A0 has no pragma and default catalog flags, so
    p3 gives its copy the owner's mode.
  - It carries a `// Pick-any:` note, marked scaffolding (include order).
  - No landed identical /Os copy was found. The only candidate, 0x004D9861, is T3-reserved and was not compiled.
  - Accept p3, or take form S (kept: evidence/measure/formS_0045c5f6.cpp, its logs in evidence/measure/formS/) as debt?
- **R4: header opt-ins** (HEADERS.tsv; equiv 0, control flags):
  - (a) `_COMPONENTS` and `_IMPLICIT_DTOR` edit existing lines under `#ifdef` (NF309 Q2 form).
  - (b) `CThingNoiseDef.h` is promoted from `_quarantine`. At landing:
    - remove `engine/_quarantine/CThingNoiseDef.h`;
    - set INDEX.tsv `CThingNoiseDef` to HAND_RETAIL.
    - Landed rows that model a class named CThingNoiseDef locally will now get SHARED_HEADER_AVAILABLE warnings (NF325 Q3): 0x00455EF0,
      0x00455F14, 0x00455F1A, 0x00463923, 0x0044C72B, 0x007498E0, 0x00C3A740, 0x00C3A760 and 0x00C3A780 (grep).
  - (c) `_DTOR_DECL` declares `virtual ~CThingNoiseDef();`, which retail leaves implicit, for binders only. It is the same practice as
    CParentDefClassBase.h's default text (declared, with 0x00430300 as the implicit dtor's out-of-line COMDAT). Both PDB sets name
    `??1CThingNoiseDef@@UAE@XZ`. The header forbids it in the emitter.
  - (d) `CThingComponentSet.h`: my generator copy (OUT redirected, run from the repo root) reports 0 trusted landed files for both classes,
    so its evidence line may differ from a repo run.

## Unit (UNITS.tsv)
- 0x0045C5F6 drops `?EndAttackBeam@CTCKraken@@QAEXXZ`. 0x007FC800 defines it in the same landing.
- The drop test:
  - HEAD and new links both resolve EndAttackBeam (00923f60 in the set);
  - no table line is witnessed by 0x0045C5F6 on that name;
  - the old 0x007FC800 self `?CTCKraken_EndAttackBeam@@YIXPAVCTCKraken@@@Z` is named only by its own row and test.
- **Binder coverage:** 0x009243B0, 0x00924520 and 0x00924930 were not compiled. Their sources declare `void EndAttackBeam();` in a
  CTCKraken model and call it, as NF348 judged.
- **0x007FC800:**
  - It is NF348's held/b3 draft, with the callees moved to the PDB names (c666 / c704). The owners 0x007FB870 / 0x007FB1C0 define
    `AAEXXZ`, per their sources and the table lines witnessed by SetMode 0x007FC730. The owners were not compiled (T3 nf302 column 2).
  - Its T3 nf302 lines are resolved by this respell.
  - **libcp, NF318 Q1, each condition:**
    - a move onto the class's shared header (engine/CTCKraken.h) under existing switches (`FABLE_RETAIL_CTCBASE` / `_DEF_CLASS_BASE` /
      `_CINTELLIGENTPOINTER` / `_CCHARSTRING`, all widely landed);
    - every other section is equal under the 3 renames (logs/proof_007fc800.txt);
    - `.drectve` HEAD `LIBC, OLDNAMES`, new `libcp, LIBC, OLDNAMES`;
    - the UND sets are equal under the renames;
    - grade RELOCATION_MATCH PASS.
- **Table** (TABLE_CHANGES.tsv): `apply_table --dry`: replace 1, delete 4, add 2, misses 0, no new multi-target.
  - The stand-ins deleted or replaced (`Base454b4c`, `Sub_vtbl_1230ba0`, `CThingNoiseDef_Destruct`, `@Kraken_PreEnd@4`, `@Kraken_Finish@4`)
    are named by no other landed source or test (grep).
- **Link, new set:** the vftables' slots add new UNDs, as NF323 Q6 allows: Copy / GetSizeofClass / Transfer of both classes.
  - 0x00463923's self is `QAE` (NAME_FIXES).
  - The CTCNoiseDef slot owners were not checked.
- **Census of new names:** no other landed source defines `??1CThingNoiseDef`, `?Alloc@CThingNoiseDef`, `??1CThingBaseDef` or
  `??_GCThingNoiseDef`. The comment-only mentions are 0x004979A0 and 0x008E31A0.

## Rule notes
- **No repo writes:**
  - The repo tree's newer files are other agents' (agent_tools/nf352, nf354).
  - My generator copy writes only into `work/gen*`.
  - `apply_table --dry` wrote nothing.
- No git. No `rm`; deletions went through clean.py.
- **Concurrent compiles:** an `hgrade` of 0045c5da overlapped a grade of the same row for a few seconds. The final 0045c5da object was
  rebuilt alone (g3) and is the one proved.
- **Same-call prechecks (slips):**
  - the probes p1 / p2 and p3 were prechecked and launched in the same call;
  - grade g3 (0045c5f6 / 0045c5da) was launched in the same call as a comment-only edit of CThingNoiseDef.h and its precheck.
  - Each precheck passed before the compile started.
- **Generator run:** the first run used `python3 -I` without `-B`. A later check found no new `__pycache__` in the repo.
- **HEAD pick-any of 0x0045C5F6** (logs/pk_head_0045c5f6.txt): one NEQ, the row's own self `?EndAttackBeam@CTCKraken@@QAEXXZ` compared
  with 0x007FC800, where the table binds that name. It is not leaf-referenced, and it is gone in the new object.
- **Headers:** `orig/hdr_CThingBaseDef.h` and `orig/hdr_CTCNoiseDef.h` still equal the repo at hand-back (cmp). The two new headers do
  not exist in the repo.

## Locks
Claimed as NF355: 0045c5f6, 0045c5da, 00455ef0, 007fc800, 00923f60, 0044c1bb. All six are released: lock.sh printed RELEASED for each,
and no nflocks owner file reads NF355. No NF353 / NF354
lock, B claim or Codex reservation was touched.

## Tools (tools/)
- **Copied from NF351:** nf.py, exp.py, disfn.py, objsec2.py, drectve.py and the others, repointed to NF355.
- **New:**
  - `vtmap.py`: retail vftable slots, with names;
  - `vtcmp.py`: an emitted vftable against retail;
  - `vtdump.py`: vftable COMDAT copies across objects;
  - `leafcmp.py`: one COMDAT HEAD against new, under renames;
  - `pk2.sh`: both checkers with `--leaf` and an address file;
  - `preov.sh`: clang precheck with an overlay;
  - `expbatch.sh`;
  - `gen_class_headers_nf355.py`: the generator with OUT redirected.
