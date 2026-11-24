# Landing nf323 (Account A, 2026-11-24): 14 Clone rows and 6 FableWin-only / add-only declaration rows (14 header opt-ins)

Hand-back: `pending/2026-11-24_nf323_handback/`; helpers under `naming_batch/agent_tools/nf323/`.
- **Entries:** 53, plus 5 FableWin-only rows taken from the released nf320 lines (Q5).
- **Outcome:** 27 done, 11 skipped-held, 20 skipped-port.
- **Landed:** 20 of the 27. 7 are held under the rulings below.

## What landed
- **14 CCreatureAction_\*::Clone rows**, built by NF323's scripted chain clonetri.py → ovlist.py → mkmodel.py → mkclone.py → cloneproof.py.
  They cover:
  - 5 on class models already in the headers;
  - 3 with add-only `_RTTI_DECLS` overrides, including NF320's pilot 0x0084C6C0, whose slot 10 GetAnimExtraFlags is now declared;
  - 5 on a new `_VIRTUAL_MODEL` / `#else` flat-text opt-in in generated headers;
  - 0x008E6F50 on the 81-slot retail CCombatActionBase branch (`FABLE_RETAIL_KRAKEN_ROAR`).
- **6 add-only declaration rows:**
  - 0x004DEA03 CFireballSpellLevelDef::Transfer, in both PDBs;
  - 0x00AB7910 and 0x00AB7920 on CJoystickDX, built on NF319's block;
  - 0x00A7D950, 0x00A7D960 and 0x00B4B6B0, which are RTTI plus FableWin (NF320 Q3).
  0x00B4B6B0's declaration sits in the `_MANAGER` branch of CEnginePrimitiveManager2DParticleGroup only; the `_RENDER` branch lacks it
  (NF323 Q7, acceptable because a landed row defines `_MANAGER`).
- **Headers:** 14 edited headers, each add-only under an opt-in, with moved asserts pinned.

## Proof
- **Clone rows** (cloneproof.py, `evidence/cloneproof_fin.txt`):
  - the Clone bytes and relocations are unchanged apart from the self rename;
  - no definition is lost, and the only added definitions are `??_G` / `??_E`;
  - the emitted `??_7` matches retail slot for slot, with the same slot count (70, or 81 for KRAKEN_ROAR);
  - the new `??_G` body is byte-equal to retail's slot-0 body;
  - `.drectve` is HEAD plus libcp.
  - Control: NF320's pilot objects give NOT_OK at slot 10.
  The UND set grows by exactly what the emitted `??_7` / `??_G` reference (NF323 Q6, under NF320 Q1).
- **Declaration rows:**
  - the override probe passes, and its made-up-name control fails;
  - libcp.py shows one DEF rename and equal UND / DEF sets;
  - `.drectve` is equal, or HEAD plus libcp.
- **All rows:**
  - precheck under the locks;
  - vland HEAD and new under one work path;
  - a link of HEAD and new with 21 control objects: no new duplicate, and nothing newly unresolved.
- **Headers:** equiv_check over 57,146 rows, 0 differing; the planted control flags the landed includers.
- **At landing:**
  - all 68 orig/ copies equal HEAD;
  - no `reloc_bindings.tsv` line targets any of the 27 addresses;
  - no other landed row defines any of the new symbols;
  - recheck of the 20 rows plus every direct includer of the 14 headers: 50 of 50 PASS;
  - every row grades its new symbol.

## Rulings on NF323's questions
- **Q1:** the 6 brand-new header files take the opt-in form. gen_class_headers.py's flat header is the default, and the model goes under
  `_VIRTUAL_MODEL`. An unguarded new header would make the polymorphic model the default for every later includer. The AlertToPlayer
  header is a precedent to review, not a licence. Their 6 rows are held (drafts in `held_new_headers/`).
- **Q2 (PickUpJug):** held, unless the out-of-line call is the base's copy constructor by two sources: the `??0<Base>@@QAE@ABV0@@Z` symbol
  in the PDB sets, plus a landed row or a donor call. Then it is an add-only declaration with a table `add`.
- **Q3:** NF309 Q2 covers PDB access on a virtual. 0x00860290 lands as one unit with its binders 0x0085FF10 and 0x00860150, all defining
  the opt-in, followed by two table replaces after ref_scan. Held until then.
- **Q4:** the CScriptThing / ScriptedThing pick-any unit goes to one dedicated pass (NF326).
- **Q5:** accepted.
- **Q6:** confirmed, as its own exception under NF320 Q1: a Clone TU's UND set may grow by exactly what its emitted `??_7` / `??_G`
  reference.
- **Q7:** acceptable; the `_RENDER` gap is noted above.

## Queued
- 38 T3 lines `nf323` (the 7 held rows included).
- 62 NAME_FIXES_QUEUE lines `nf323`:
  - 21 deleting-destructor rows defining free-function stand-ins;
  - 38 owner rows of overrides the new vtables reference;
  - the 0x00860290 binders, 0x01276C2C and 0x00463BDB.

## Recheck
`logs/recheck.log` (landed tree, 50 rows): 50 PASS.
