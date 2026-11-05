# Plan phase 6: what is left of steps 3-6 after phase 5 (2026-11-05)

Phase 5 handed back w1 CCountedPointer 2,436, w2 CScriptThing 150, w3 CIntelligentPointer 1,488, w4 value-class leftovers 54
(4,128 changed rows, 4,022 distinct files; apply in that order: later waves start from the earlier waves' text). No shared
header changed, so no sub-macro and no includer regrade. Index: `$S/plan/idx` refreshed at the phase start to 3b0bb8ba6
(OK 53,896 / NO_FUNC 441 / LEAFGUESS 186 / COMPILE_FAIL 5, no ERR; BIND_BASE / MULTINAME / QUEUE_STATUS in `$P/idxpost`).
**Refresh it again once the four waves are applied** (`$P/run_idx_p5.sh`, then `p1/post.py $S/plan/idx $P/idxpost`).

## Tools (`$S/plan/p5/tools`; copy to p6 and set P)
- `hdrmig5.py` = hdrmig4 plus:
  - `field_map` {Class: {slots, aliases}}: word-member models respelled by offset to the header names; alias classes (SCountBlock
    ...) renamed to the header class. `nontemplate_typed`: a non-template stand-in whose first member is X* is Class<X>.
  - `accept_pdb_majority`: a callee respell to a PDB spelling that is already the majority binding of that target.
  - a stale index entry (symindex2 key changed) is re-measured, not trusted; `include_if_used`; a prerequisite include below the
    class block is moved up; `ptr_triple.note`.
  - `base`: start from a previous wave's (unapplied) text. Patch `// Leaf:` lines when a row's own symbol changes class key
    (0x00945E70 needed a re-grade for this): do it before grading.
- `conscheck.py WAVE SUB...`: one target, one spelling check of a wave's respells against the index.
- `wave.py` reads `RELOCATION_MATCH[/EHsc]` as RELOCATION_MATCH.

## Step 3 leftovers (local rows after phase 5; localcopies_post.tsv)
- **CCountedPointer 704**: non-template `void*` stand-ins 208 (T unknown: needs the typed caller / PDB T as s6 w1 did), local
  members the header lacks 76 (Release / IsValid / AddRef: map each to the header member only with the same body), local
  `CCountedPointer<void>` 30, out-of-class DeleteData / ShareData definers 24 (the row IS the COMDAT: needs an explicit
  instantiation shape), bytes_differ 214, stray deleter respells 29 (targets with 3-4 spellings: decide one per target first).
- **CIntelligentPointer 2,057**: 1,225 fail because the local typed `T* GetPItem()` / `SetPItem(T*)` become the header's
  Get() / Set(), whose static_cast needs CBaseObject and T complete with T : CBaseObject. They move once the owner classes
  (CThing ...) come from shared headers (step 5). Non-template stand-ins 284.
- **C3DVector 319, CVectorMap 821, CArray 605**: unchanged reasons from p4/NEXT.md (method / operator models the header lacks,
  LOCAL_STD, same-TU definers); CArray rows that call local Size() / GetData() need a method_map decision (vector::size() and
  CArray::GetAsCArray are the retail members).

## Step 4: CScriptThing (272 local rows left)
- 65 inline-depth + 17 two-view rows and the 121 CActiveEntityScript<T> ??_G rows: unchanged (p4/NEXT.md).
- The rest: methods the header lacks (GetThing 13, IsValid 12, InlineCopy 9, GetPosition 5 ...), local enum typedefs
  (`typedef long ECameraOp / EScriptAIPriority / NEntityEvents::EEventType`) that clash with engine/fable_nscript_forward.h
  (removing them makes int literals need the enumerators: spell them from the Ego_r enums, as w2 did for
  HERO_ABILITY_HEAL_LIFE_SPELL), non-template CCountedPointer stand-ins 15, bytes_differ 73 (inline dtor / copy bodies).

## Step 5: game classes (local rows): CThing 4,190, CTCBase 1,223, CWorld 1,086, CThingCreatureBase 1,071,
CDefinitionManager 760, CMainGameComponent 732, CPlayer 711, CThingPlayerCreature 587, CThingPhysical 297, CThingGameObject 251,
CGameScriptInterface 209. Plan unchanged (p4/NEXT.md: lift the GetTC template body into engine/CThing.h behind a sub-macro,
then offset-map members). Doing CThing first also unblocks most of the CIntelligentPointer leftovers.

## Vendor headers (not started): MFC CString 249 local rows; CSoundEventManager family; reverb family; strmbase models (p4 list).

## Step 6: stand-ins and queued name fixes
- STANDIN_INACCURATE.tsv families as in s6 NEXT.md. CCountedPointer family left after s6 + phase 5: 31
  `?Uninit_copy@@YIPAUElem@@...` (STL model), 17 `?ShareData@CCPtr@@QAEXPAXPAJ@Z` (pragma s, out-of-line release), 12 converting
  ctors (Base/Derived classes needed), 4 `?AssignFrom@CCPtr@@` operator= definers (0x00419463 / 0x004194BA / 0x0042FE34 /
  0x005C077F: T disagrees between the row comment, the catalog and the caller binding), 4 `?SetData@T@@` DeleteData, 3
  `?SetStaticCast@T@@`, 4 `?clear@CRetailList_CountedPointer@@` (list::clear needs FABLE_RETAIL_STLP_LIST_CLEAR).
- Deleter targets with several stand-in spellings (one decided spelling each, then respell all binders): 0x00429F43, 0x0050BE40,
  0x0056C0F0, 0x005ECF50, 0x00668610 (4 spellings), 0x007180B0, 0x00786A30, 0x008AA810, 0x008C2D60, 0x00A99FF0, 0x00CB8D30.
- 0x004CC340 `?IS_THING_ALIVE_004cc340@@YI_NAB[U|V]CBaseIntelligentPointer@@@Z`: respell its three binders (0x0065C781,
  0x007888B4, 0x00788229) together (w3 deferred two of them for this).
- NAME_FIXES_QUEUE (QUEUE_STATUS at 3b0bb8ba6: open 721, partial 2,878, unparseable 1,484, satisfied 19):
  - wave 35b / 35c / 35d lists: DEFERRED 75 / 48 / 7, NAME_FIXES 15 / 15 / 6 (`naming/2026-11-04_wave35[bcd]/`);
  - `land0511a` 16 lines (11 partial, 5 open), `land0511b` 14 (all partial);
  - phase 5 narrowed 50 lines (w1 line 1445 0x00CDEE00: 1 binder left; w3 49 lines on 0x00A01B50 GetPItem / 0x00A01C10
    ~CBaseIntelligentPointer: 143 / 106 binders left);
  - **0x0041E5F2 conflict**: queue lines 62 / 568 / 579 / 701 ask for `?GetInstance@CManager@NUISystem@@SIPAV12@XZ`
    (Ego_r.pdb; 551 binders today), line 649 for `?GetInstance@CFrontEndManager@NUISystem@@SIPAV12@XZ` ("both PDBs"; 25
    binders), and 23 bind `?GetInstance@CFrontEndManager@@SIPAU1@XZ`, plus 9 more stand-ins. One retail address, one name:
    the owner / coordinator must pick (CManager is the majority and the Ego_r name; check whether FableWin really has the
    CFrontEndManager spelling at this address or a different one) before any wave respells it.
  - **0x009F1720** (reported, bytes not changed): the landed row's leaf `??1T@@QAE@XZ` stores a `??_7T@@6B@` vtable relocation
    (index BIND: dir32, target 0x00000000) where retail stores the literal 0 (exe bytes `c7 01 00 00 00 00 c3`): a
    reloc-masked false match. It needs a non-polymorphic model (`*(int*)this = 0`); that changes the row's object bytes from
    a reloc to an immediate, so it is a coordinator / owner re-land, not a naming wave.
- engine/CCountedPointer.h line 29 comment fix (0x00CE1040 is CCountedPointer<CEntityScriptBase>::ShareData; s6 NEXT.md).

## Lane notes
- vland 300-row batches took 5-7 minutes each, 2 at once, with gate NJ=4 and hdrmig NJ=2 alongside.
- One accidental read-only `git rev-parse HEAD` at the start of phase 5 (to record INDEX_HEAD); no other git.
