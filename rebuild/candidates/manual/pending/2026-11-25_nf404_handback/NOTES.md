# NF404: T3 nf401 (0x009E15E0 skipped as briefed)

Work dir: `$S/w22/agentNF404` (no repo writes, no git). Batches in `batches/<b>/` in the agentNF398 layout:
RESULTS.tsv, SELF_SYMBOLS.tsv, TABLE_CHANGES.tsv, UNITS.tsv, HEADERS.tsv (where a header changes), rows/, tests/, orig/ (HEAD byte
copies of every replaced source, test and header), hdr/ (new headers), head/payload.json, logs/ (vland HEAD / new), evidence/.

| Batch | Item | Rows | Verdict |
|---|---|---|---|
| b1 | 5: RELOC_CONFLICT on ??1CCharString | 004af220, 0056c840 | ACCEPT |
| b3 | 2: five IsThingAlive_<addr> | 008d04b0, 0093a890, 009445c0, 00944ce0, 0095ee40 | ACCEPT |
| b4 | 1: 0x007FB120 table name | 007fb120 (+ joined binder 00924c70) | ACCEPT (table name proven wrong) |
| b5 | 3: CWound_00c5eca0 stand-in | 00c5eca0, 00c5ed90, 00c5e3d0, 00c3c830 + funclets 01216360..01216394 | ACCEPT pending Q1 (locks kept) |
| h4 | 4: 0x00CAC300 respell | 00cac300, 012170c0, 012170c8 | HOLD_RULING Q2 (locks released) |

## Names and their two sources

- **0x007FB120 `?HeadReadyToStrike@CTCKraken@@QAE_NXZ`** (was the table's `?IsTargetAlive@CTCKraken@@QBE_NXZ`, self `?IsThingAlive_007fb120@@YI_NAAUCHolder@@@Z`).
  (1) FableWin.pdb 0x02778AA0 (tc_kraken.obj, 35 B, IS_THING_ALIVE on this+0x20 = PTarget). (2) Ego_r.pdb 0x007F6450, same shape as retail.
  Cross-check: retail CAIStateGroup_KrakenAttack::Update calls it at 0x00924EDE, the branch that matches the donor Update's HeadReadyToStrike
  call (FableWin 0x02C5FE12). `IsTargetAlive` is in neither PDB name set (it is a descriptive label from labels_landing_2026-11-08e.tsv),
  so it goes to NAME_FIXES.tsv. Non-const (QAE) as both PDBs spell it. engine/CTCKraken.h already declares `bool HeadReadyToStrike();`.
- **b3, the five virtuals.** Each name comes from (1) FableWin.pdb and (2) Ego_r.pdb at the same vtable slot. The donor bodies are
  IS_THING_ALIVE on the same member (the debug base is 4 bytes larger).

  | Row | Name | Retail vtable, slot | FableWin.pdb | Ego_r.pdb |
  |---|---|---|---|---|
  | 008d04b0 | `?IsAlive@CGameScriptThing@@UBE_NXZ` | 0x012852CC, slot 75 | 0x02B00770 | 0x00907510 |
  | 0093a890 | `?IsStillValid@CAIStateGroup_BanditSitRoundCampfire@@UBE_NXZ` | 0x0128B78C, slot 5 | 0x02B7DC90 | 0x0095DCE0 |
  | 009445c0 | `?IsStillValid@CAIStateGroup_WorkSawingWood@@MBE_NXZ` (protected) | 0x0128BCBC, slot 5 | 0x02BAC230 | 0x009683C0 |
  | 00944ce0 | `?IsStillValid@CAIStateGroup_WorkHammerAnvil@@MBE_NXZ` (protected) | 0x0128BCF8, slot 5 | 0x02BAD3B0 | 0x00968C10 |
  | 0095ee40 | `?IsStillValid@CAIStateGroup_OpinionReactionRoot@@UBE_NXZ` | 0x0128C984, slot 5 | 0x02C276B0 | 0x009833D0 |
- **b1 callees.** 0x0099B510 is `??1CWideString@@QAE@XZ`. Sources: (1) reloc_bindings, witnesses 00406e30 and 009979e0; (2) its body
  decrements NoRefs at +0x0C, as Ego_r.pdb ~CWideString 0x00B02740 does at its own layout. 0x0099EAE0 is `??1CCharString@@QAE@XZ`. Sources:
  (1) the landed self, RELOCATION_MATCH; (2) ReleaseData 0x0099E9B0 reads NoRefs at +0x0D, as Ego_r ~CCharString 0x00AFA3F0 does.
  The member layout ScriptName CCharString +12 / Text CWideString +16 / Title CWideString +20 is CQuestManager::CBoast
  (struct_layouts_egor.tsv). FableWin.pdb has `??1CBoast@CQuestManager@@QAE@XZ` (0x01C4C370), and the table already binds 0x0056C840 as
  `??1?$pair@QAVCMenuEntry@NUISystem@@VCBoast@CQuestManager@@@std@@QAE@XZ` (witness 0056ad40). The two selves stay stand-ins in b1; their
  respell is queued in T3.txt.
- **b5, class CSalOutputKS (GameCODA).** The class and member names are the SDK's, matching the header's existing "names from original
  GameCODA SDK COFF". Sources:
  (1) The GameCODA 2.10 SDK library `$S/sdk/gamecoda/tree/CODA/lib/Win32/CODAApi.lib`, member SalOutputKS.obj (extracted to
  work/coda/SalOutputKS.obj; all 327 members in work/coda/all/):
  - `??1CSalOutputKS@@UAE@XZ` destroys the members in the same order: ReleaseBuffer via [vt+0x10], then mutex, thread, event, pin, file.
  - `??0CSalOutputKS@@QAE@XZ` makes the same vtable and handle stores in the same zeroing order.
  - The member destructors are byte-identical (relocations masked) to the retail out-of-line copies the six funclets jump to:
    ??1CSalFile 0x00C5EBD0, ??1CSalKsPin 0x00C5EC50, ??1CSalEvent 0x00C5E420, ??1CSalThread 0x00C5E480, ??1CSalOutput 0x00C56E60.

  (2) The repo table already binds `??0CSalOutputKS@@QAE@XZ -> 0x00C5ED90` (witness CSalPlatform 0x00C3F9D0). That constructor stores
  vtable 0x012B914C, which is this destructor's vtable (slot 3 = landed CSalOutputKS::CreateBuffer 0x00C5E650).

  Retail expands CSalMutex's ctor/dtor inline where SDK 2.10 calls them out of line, and retail's vtable has more slots (version drift).
  Header PASS23 already models the retail slot order. The six vtable names follow from which member each vtable is stored for (TABLE_CHANGES).
- **0x00C3C830 `??1CCodaObjectBase@@UAE@XZ`** keeps its name (owner decision D3). The new point is that the destructor is *inline*:
  retail ~CSalOutputKS expands it (closing `mov [esi],0x012B5868`, no call), and all 14 SDK objects that emit it carry it as a 7-byte Any
  COMDAT, never NoDuplicates.
- **0x00CAC300 (held).** The target spelling is the row's own Vendor-proof line. See RULINGS_NEEDED Q2 for why it is not local.

## What changed and why (beyond the respells)

- b5 first new set: the /FORCE link grew duplicates twice, because HEAD had strong definitions where the unit now emits inline pick-any
  copies. Both were fixed by re-rowing the strong owner in the nf86/nf88 pick-any form ("Fidelity: scaffolding, not retail" reference
  function), which needed the two locks I took:
  - 0x00C5E3D0 `?Close@CSalFile@@QAEXXZ`.
  - 0x00C3C830 `??1CCodaObjectBase@@UAE@XZ` (evidence/link_b5wide.txt: 1 -> 2). Its inline body went into engine/CCodaObjectBase.h under
    FABLE_RETAIL_CCODAOBJECTBASE_INLINE_DTOR, not into the derived header.
  - Final link (evidence/link_b5final.txt) covers the unit plus 20 neighbouring landed rows: every landed home of a CSal* /
    CCodaObjectBase member, the four siblings, CreateBuffer and CSalPlatform. Duplicates 1 -> 1; the 1 is the HEAD-existing ?MakeHolder
    between 00c5e440 and 00c5e4a0.
- The 00c5eca0 destructor calls ReleaseBuffer through the `CSalOutput` view. A direct call gave E8 where retail has `call [0x012B9160]`
  (DIFFER 211v212).
- b5 TABLE_CHANGES: 00c5e440 (??_GCEvent_00c5eca0) and 00c5e4a0 (??_GCMutex_00c5eca0) are left with unbound ??_7 stand-in names.
  reloc_identity exempts ??_7 from RELOC_CONFLICT, and pk is identical on the repo table and the changed table (s5repH v s5trH).
  Their respell is in T3.txt.

## Pick-any owners in the links (all batches)

Every addressed pick-any copy a new object emits had its landed owner in the /FORCE link:
- **b3** (evidence/link_b3owners.txt): IS_THING_ALIVE<CThing> 0x004CC340 and IS_THING_ALIVE<CThingObject> 0x0078B2A0.
- **b4** (evidence/link_b4owners.txt): 0x004CC340; 00924c70's LowerBound 0x0040F020 / 0x006804C0 and bitset::test 0x00410DE0.
- **b5** (link_b5final.txt): every CSal* / CCodaObjectBase home.
- **b1**: the only new addressed copy is ??_GCWideString (compiler-generated, always Any).

Duplicates did not grow anywhere: b1 0 -> 0, b3 0 -> 0, b4 0 -> 0, b5 1 -> 1 (the HEAD-existing ?MakeHolder).

## Coordinator note: the wineserver -k window (2026-11-25 01:45-01:47 UTC)

I never ran `wineserver -k`. Two steps fell in that window, and both were rerun:
- **b5 new-side vland (first attempt): rerun.** cl TIMEOUT. The cause was mine and is now fixed: the overlay CSalOutputKS.h
  quoted-includes "KernelStreaming.h" and siblings that were not in the overlay dir, so cl hung. I symlinked them in and killed only my own
  stuck PIDs (1427, 1428, 27866). The b5 unit was then rerun several times; the final record is logs/vland_b5f_* (work/wd_b5f). Its objects are COMDAT-identical to the
  b5b / b5c / b5d objects that the pk and link evidence used.
- **Item-4 probe compile (01:45:19): rerun at 02:19 UTC.** The source is identical, it is RELOCATION_MATCH again, and all 23 COMDATs are EQ to
  the window object (batches/h4/evidence/).

Everything else finished before 01:45 or started after 01:49:
- b1 / b3 / b4 vland at 01:31-01:38; their links at 01:33-01:39.
- HEAD objects in work/hobj: 01:33-01:44 and 01:59-02:12.
- The b5 links at 02:00-02:18.

The censuses use gcc cpp, not Wine.

## Census (equiv_check, all 57158 landed sources/tests)

- **b3**, the five headers (evidence/equiv_b3.tsv): one row differs, 0x0093AB70. The cause is FABLE_STATIC_ASSERT __LINE__ names
  shifting. Regraded with the overlay: RELOCATION_MATCH, source identical, 90/90 COMDATs EQ (evidence/regrade_0093ab70_comdats.txt).
- **b5**, CSalOutputKS.h alone (first version): 0 differ.
- **b5 final**, CSalOutputKS.h + CCodaObjectBase.h together (evidence/equiv_b5b.tsv): 0 differ.

## Locks

- **Kept as NF404 (ACCEPT rows, and the b5 rows pending Q1):**
  - Task rows: 007fb120, 008d04b0, 0093a890, 009445c0, 00944ce0, 0095ee40, 00c5eca0, 004af220, 0056c840
  - Funclets: 01216360, 01216368, 01216373, 0121637e, 01216389, 01216394
  - Joined rows: 00924c70, 00c5ed90, 00c5e3d0, 00c3c830
- **Released (item-4 hold):** 00cac300, 012170c0, 012170c8.
- **Not touched:** 00433530 and the b5 siblings (00c5e440, 00c5e4a0, 00c5ead0, 00c5e2b0 and the home rows). I only compiled their HEAD
  sources read-only for pk and link evidence.

## Not done / debt

- The selves of 0x004AF220 / 0x0056C840 are still stand-ins (T3).
- The CSalOutputKS slot functions and member homes are landed under stand-in names (T3). They show as NEW_UNRESOLVED vtable slot
  functions in the b5 link, which is expected.
- Q1: the ??_GCSalKsPin 53 B v 30 B pick-any residue. Retail's COMDAT order shows that the 30 B body is this TU's own revision. SDK 2.10
  emits 53 B, exactly like our model. This is a gap in the opt-in's model, not a second body from another object. The coordinator decides
  whether to land b5 (locks kept) or hold it.
