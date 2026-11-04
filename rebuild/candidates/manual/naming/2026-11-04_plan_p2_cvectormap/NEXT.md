# Plan phase 3: what the plan agent learned in phase 2 (2026-11-04)

## Tools (all in `$S/plan/p2/tools`; the index is `$S/plan/idx`, brought to HEAD after chunk 773 + phase 1)
- **`hdrmig2.py CONFIG OUT @rows`**: phase 1's hdrmig.py for template / value classes. It strips every local definition,
  specialisation and forward declaration of the config classes (any indentation, `template <..>` head included), and a local
  `namespace std {..}` block when every class in it is one the STLport model provides. It includes the header where the
  first block stood and applies `renames` / `rewrites` to code only. `repad` shrinks the pad after a by-value member
  whose local model was smaller. Acceptance is gate.py's rule (length, masked sha1, relocation targets). A symbol may
  change only to a header spelling: struct U -> class V, `allowed_symbol_map`, or `target_symbol_map` (retail target ->
  decided spelling). `?A0x` path hashes are ignored.
- **`aliasmap.py CONFIG OUT @rows`**: rows whose map is a non-template stand-in class (`CInterfaceMap` ...). The class
  becomes a typedef of the header instance, its same-TU LowerBound body goes, and entry fields / end pointer / pointer key
  are respelled. 143 of 198 CInterfaceMap rows passed on the first run.
- **`variants.py CLASS OUT`** groups local copies by normalised block text. **`localcopies.py OUT`** counts, for every
  engine header, the landed rows that still define the class locally (`$P/localcopies.tsv`). That count is the "fully
  migrated" gate for strict mode.
- **`leafplan.py`**, **`wave.py`** (assemble a V1-format wave from gate + vland logs, with `--before` parity baseline),
  **`vgrade2.sh` / `vqueue.sh`** (a vland queue, at most N at once), **`regrade.sh WAVE`** (coordinator regrade from the
  working tree), **`fdiff.sh`** (diff one function's disassembly in two objects).

## Step 3 leftovers (value classes)
- **CVectorMap** (2,665 local rows before, about 2,190 after this wave):
  - `cvectormap/READY_PHASE3.tsv`: 672 rows that pass hdrmig2 today. Regate after the wave lands, then grade.
  - The bitset block (160 rows, and most CThing rows of step 5): rows model `std::bitset<274>` with an out-of-line `test()`
    (0x00410DE0) and an empty stub + pad. Add `FABLE_RETAIL_STLP_BITSET_TEST_DECL` to stlport/fable_containers.h (test
    declared only, owner approval: retail calls it out of line in size-optimised TUs), and re-pad the stub member
    (+35 bytes).
  - const_lowerbound (120): make the map pointer / iterator const before migrating.
  - Stand-in maps: 148 alias rows deferred by shape. A further ~60 rows bind 0x0040F020 / 0x004365B0 under nested or
    namespaced models (`MazeTable::Find`, `KidTable::FindEntry`, `CThing::CInterfaceMap_Retail`, `NCreatureMode::CInterfaceMap`
    ...). These keep the 12 queue lines open (`cvectormap/QUEUE_LINES.tsv`).
  - The other 320 variants (about 1,300 rows) were not tried. Codex pass 13's 0x00501450 waits on CVectorMap::LowerBound
    (HANDOFF).
- **C3DVector** (3,450 local rows): the top variant `class C3DVector { public: float X, Y, Z; };` covers 1,371 rows and went
  45/45 on hdrmig2 (sample `c3dvector/t45`, no symbol changes). This is the next big cheap wave: config `c3dvector/cfg_v0.json`.
  Variants 1 (x,y,z lower case, 188) and 3 (`float X; float Y; float Z;`, 110) need `renames` for x/y/z member accesses.
- **CArray** (1,321 local rows): the top three variants are empty `class CArray : public std::vector<T> {}` (666 rows). They
  need the local std::vector stub removed (std_provided) and nothing else.
- **CCountedPointer** (3,140): the top variants are `{ T* Data; CCPPointerInfo* Info; }` (411 + 220 + 85). Check the shared
  header's member names first.
- **CIntelligentPointer** (2,706): the top variant (617) derives CBaseIntelligentPointer with GetPItem / SetPItem inline. It
  needs the owner's packed branch (`FABLE_RETAIL_CIntelligentPointer` pack(1)) decision per row.
- Variant lists: `$P/variants_<Class>.tsv`, `cvectormap/variants.tsv`, `c3dvector/variants.tsv`.

## Step 4: CScriptThing / CScriptBase family
- **Local copies today:** CScriptThing 432 rows (4,258 includers), CEntityScriptBase 341 (2,556), CScriptBase 99 (1,079),
  CGameScriptThing 41, CActiveEntityScript 35 (205).
- **Not investigated in phase 2.** Start from these:
  - the 65 inline-depth rows: `docs/pipeline/INLINE_DEPTH_REVISIT.md`; the CScriptThing out-of-line copy / dtor split is
    shared by 16+ of them;
  - the 17 two-view rows and the 121 `??_G` CActiveEntityScript<T> rows: naming waves 33 / 35 NAME_FIXES and DEFERRED;
  - the parked NScript Main rows of chunks 770 / 772 / 773.
- **Order:** a CScriptThing header-first wave with hdrmig2 (the class is in many TUs as a by-value member of the script
  classes, so expect `repad` work). Then the `??_G` generator against the shared header.

## Step 5: game-class offset waves
- **Local copies:** CThing 4,188 rows (only 83 include engine/CThing.h), CTCBase 1,223, CWorld 1,086, CThingCreatureBase
  1,071, CDefinitionManager 760, CMainGameComponent 732, CPlayer 709, CThingPlayerCreature 585, CThingPhysical 297,
  CThingGameObject 251, CGameScriptInterface 210.
- **Most CThing copies are the same block:** the interface bitset at +0x20 (`m_presentBits[9]` or a `std::bitset<274>`
  stub), the component map at +0x44, `GetTC` / `GetInterface` inline. So CThing waves should follow the bitset sub-macro and
  the CVectorMap wave, and reuse `repad` (map the row's members by offset to struct_layouts_egor.tsv names; retail offsets
  differ from Ego_r after +0x4c, see the row comments `[Ego_r in brackets]`).

## Step 6 inputs
- **`leaf/STANDIN.tsv`:** 3,623 rows whose graded symbol is a stand-in (`?Get@T@@QAEHXZ`, `?DestVal@Slot@@QAEXXZ` ...).
  2,904 of them share the symbol with other rows. Their leaf is honest now, and vland no longer warns on them.
- **`leaf/DEFERRED.tsv`:**
  - 132 rows define another function's PDB / catalog name (base-class `??_G` for derived classes, forwarders defining their
    callee);
  - 55 rows whose leaf is the PDB spelling but whose source emits a near miss (fix the source);
  - 6 rows that define several functions.
- **`cvectormap/NAME_FIXES.tsv`:** 3 rows bind the non-const LowerBound spelling to the const 0x004365B0.

## Vendor headers (phase 1 follow-ups, with the coordinator's placeholder rule)
- **The rule:** a vendor class's placeholders (CMember_<addr>, Method_<addr>, CUnproven*) become behaviour names in the same
  header wave, and callers move once.
- **The headers to build:**
  - MFC CString: about 247 local rows, at least four spellings of 0x0139C900, `_afxPchNil`. Extend engine/CString.h behind
    a FABLE_RETAIL_CSTRING_* sub-macro.
  - The CSoundEventManager family: CSoundEvent, CAudioContext, CAudioObject, CDspOps / CDspObject / CDspGain, PKCTRRedblackTree,
    PKCBufferedReader. These need per-method decisions on conflicting parameter types.
  - The reverb family as one header (CReverbDiffuser / CReverbDiffuserStage / CDspOps). Its `target` is the history
    CRingBuffer, and 0x00C75AB0 is waiting on it.
  - The strmbase models with virtual dtors for the `??_G` rows 0x00CA71E0 / CAB750 / CAAF90 / CAB120 / CABC60 and the
    CRendererInputPin ctor 0x00CA4F40.
- **Tooling:** hdrmig2 handles `class X : public Y {}` and template heads now, so libpk headers can reuse it. Use
  `target_symbol_map` for the decided names, so a placeholder never moves to another placeholder.

## Open questions for the owner
1. **Stand-in leaves.** The leaf wave makes 3,623 `// Leaf:` lines name stand-in symbols (`T::Get`, `Slot::DestVal`). They
   are accurate about what is graded, but the debt is now visible only in STANDIN.tsv. Should vland warn when the graded
   symbol is not the catalog function's name, so these show up on every regrade?
2. **The bitset sub-macro.** Is `FABLE_RETAIL_STLP_BITSET_TEST_DECL` (STLport bitset<274>::test declared only, called out of
   line at 0x00410DE0) the right retail model for the size-optimised TUs? It unblocks 160 CVectorMap rows and most CThing
   rows.
3. **0x00A9C960** grades DIFFER(48v27) under `--recheck-landed` today, for the landed source too (`leaf/DEFERRED.tsv`). Is
   this a landed row that no longer reproduces?
4. **Phase-3 budget.** It could cover CVectorMap READY (672), C3DVector variant 0 (about 1,371) and CArray empty-vector
   variants (666): about 2,700 rows of mechanical header moves.
5. **Strict mode** (`$P/strict_classes.txt`, `$P/strict_note.txt`): a per-class list (147 classes with zero local copies
   today), grown after each wave by localcopies.py, instead of the all-or-nothing LANDVERIFY_STRICT_HEADERS.
