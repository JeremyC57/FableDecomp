# NF444: GameCODA render-class header port (pilot)

No repo writes, no git. One batch, `batches/nf444_hdr` (14 rows, header CSalRender.h). Every row grades MATCH / RELOCATION_MATCH,
but the batch is **HOLD (closure)**: three more landed objects reference the old base names (below). Locks released.

## What was done
- **Header** `hdr/engine/CSalRender.h`: add-only `FABLE_RETAIL_CSALRENDER_MEMBERS` (`CSalRender();`, protected `CSalMutex m_mutexUpdate`
  +0x04, `float m_fGain` +0x20, `bool m_bUpdate` +0x24, size 0x28 asserted under `_MSC_VER && _M_IX86`). SDK CodeView names; retail
  offsets (the retail CSalMutex is 0x1C = CSalOutputKS.h's PASS23 layout, no m_bActive; SDK 2.10 size is 0x2C). Default path
  token-identical: the only includer, 0x00C54C10, preprocesses identically (evidence/equiv_includers.tsv). A whole-tree equiv_check
  from a scratch copy reports only `__FILE__` path artifacts (rows that include `../src/...`), so it was stopped; the header has no
  other includer (evidence/csalrender_includers_files.txt).
- **Option S unit 0x00C6E630** (retail SalRender.obj slice): owner ~CSalRender; thin rows ClearFilterHistory 0x00C6E690, ctor
  0x00C6E6A0, ??_GCSalRender 0x00C6E6D0. Names: SDK SalRender.obj symbols + retail vtable 0x012BEDA4 slots / derived calls (2 sources).
  Needs FABLE_RETAIL_CSALPLATFORM_CREATE_MIXER_PASS23 + CSALOUTPUTKS_INLINE_MEMBERS + CCODAOBJECTBASE_INLINE_DTOR + CSALRENDER_MEMBERS.
- **/Ob1** (R-NF444-1): under /Ob2 ??_GCSalRender inlines the same-TU dtor (54v30). Retail ??_G 0x00C6D350 also calls the 11-byte
  0x00C6CAC0. With /Ob1 the five derived-dtor TUs emit the retail 30-byte ??_G, EQ at 0x00C6D350 / E1D0 / ECB0 / 0x00C70A30 / 71400
  (36-byte NEQ under /Ob2). All 14 rows carry /Ob1 in head/payload.json.
- **10 binders** (derived ctors 0x00C6CA20 / D9D0 / E760 / 0x00C70120 / 0B00, dtors 0x00C6CAC0 / DA30 / E780 / 0x00C701D0 / 0BB0): base
  is the shared header, own symbols unchanged (`; target` in RESULTS). Their HEAD tests still pass on the new objects.
- **Tests** (unit): include the header, link the row object (vt.py third argument), concrete TestRender subclass; the ??_G test
  captures CSalRender's own vtable between the base and derived vptr stores. Mutant (gain 1.0 / ClearFilterHistory writing) fails 6a0 / 690.

## Gates (evidence/)
- Grades: 14/14 (fg.py, strict headers), verify_and_land --recheck-landed strict: WINS 14, only RELOC_NAME warns from the catalog labels this batch renames (vland_new.log).
- pk_all / pickany HEAD v new: 0 NEQ in N with post-landing addresses (pk/N_*.pkall_addrs.tsv: CSalRender family + CSalMutex
  copies EQ at 0x00C5D550 / D570 / D5B0, every derived ??_G EQ at its home); reachflip 0 flips.
- name_gate HEAD v new (name_gate_diff.txt): new FAILs are only the splits / RELOC_NAME labels this batch's TABLE_CHANGES / RENAMES
  remove, ??_7CSalMutex (pre-existing multi-stand-in address; the SDK name as landed 0x00C5ED90 uses), ClearFilterHistory agree_unverified
  (SDK name + 1-byte body + slot), and LOCAL_CLASS SHrtfHeader in 0x00C70120 / 0x00C70B00.
- descriptive_check --extra BEST_GUESS.tsv (header only): 0 FAIL.

## Why HOLD: closure measured (evidence/closure_head.txt; HEAD objects of all 48 rows naming CVtable_012beda4)
- 0x00C55270 (`?CreateProcessor@CAudioSource@@QAEPAVCVtable_012beda4@@K@Z`, + EH funclets 0x01216180..0x012161CD): UND ??1CVtable_012beda4
  through its local SIMD-class vtables. Respelling its base changes its own mangled leaf, so it must go straight to the NAME_FIX
  `?CreateRenderer@CSalSw3DVoice@@AAEPAVCSalRender@@W4CHRTFMODE@@@Z` (CSalSw3DVoice.h port + binder of caller 0x00C55680).
- 0x00C70A30 / 0x00C71400 (??_GCVtable_012bedcc / bede0): UND ??0CVtable_012beda4 from a scaffolding `new` helper. They need binders
  with concrete local models (CSalRender's Process / SetGain / SetPosition are pure).
- No other object references ??0 / ??1 / ??_7 of the old class (0x00C70750 / 0x00C71120 define their inline Lock locally).

## Not done (steps 2-3)
- None of NF441's seven NAME_FIXES is carried: each needs the derived-class headers + 0x00C55270 together (cascade: derived ctor
  -> SIMD ctors 0x00C55070..0x00C55110 -> CreateRenderer -> 0x00C55680).
- Retail derived layouts are 6-channel, not SDK 2.10's 4: `new` sizes in 0x00C55270 are Disable 0x38 (= SDK-4), Pan 0x78 (SDK 0x60),
  PanDelay 0x214 (SDK 0x1E4), HrtfMMX 0x66C, HrtfSSE2 0xC6C. ParamsPan = FilterPan[6], ParamsPanDelay = FilterPanDelay[6], m_fGainPan /
  m_itd 6 entries. Do not transcribe SDK offsets.
- SIMD ctors: retail CreateRenderer inlines the PanDelaySSE2 / MMX ctor (`call 0x00C6CA20; mov [esi],0x012B8A64`) but calls the other
  four (0x00C55090 ...); SIMD dtors 0x00C554E0.. are bare `jmp` (implicit dtors).
- 0x00C6E660 is ?Initialize@CSalRender@@QAEXXZ (45 = 45 bytes, SDK SalRender.obj member; NF441 sdk_rel.json does not keep section order); fold into the unit with a 0x00C55680 binder (R-NF444-2).
- Debt: 5 HEAD binder tests still model CVtable_012beda4 locally (00c6e760, 00c70120, 00c701d0, 00c70b00, 00c70bb0); binder vtables
  carry 5 slots with slot 1 = CSalRender::ClearFilterHistory (right for Pan / Disable) and slots 2-4 `_purecall` (HEAD: 1 unresolved
  `v00` slot); file names keep the stand-in spellings (sources edited in place).
- Further: ??_GCVtable_012b5868 0x00C3C840 is CCodaObjectBase's ??_G shape (31 B, not checked); CSalMutex ??0 / ??1 / ??_G EQ at 0x00C5D550 / D570 / D5B0 (unnamed rows).

## Corrections after review
- "0 NEQ with post-landing addresses" uses `evidence/addrs_post.tsv`, which also binds names this batch does NOT bind (the five
  ??_GCVtable_012b… homes and the three CSalMutex addresses). Under the repo table at landing those copies are NOADDR, not EQ.
- Copies per object HEAD v new (pk lines): 00c6ca20 1/2, 00c6d9d0 2/3, 00c6e760 0/1 (each ctor object now emits a ??_GCVtable_012b… copy,
  EQ at its home under /Ob1), unit objects 4-5 / 9 (CSalRender + CSalMutex + CCodaObjectBase COMDATs), dtor binders 1/1, Hrtf ctors 2/2.
- All evidence/pk/N_* files are the final /Ob1 builds (gate.sh rerun); reachflip over the 14 pairs: 0 flips.
- mkbinder.py reproduces the binders except two later hand edits: the `virtual ~CVtable_012bed7c/90/b8();` lines of the three ctor binders.
- NAME_FIXES.tsv: 0x00C6E660 Initialize, CSalMutex copies 0x00C5D550 / D570 / D5B0 (not applied; out of this batch).

## Resume 1 (coordinator rulings R-NF444-1 / -2)
- R-NF444-1 (a): the SDK does NOT agree. Release CODAApi.lib SalRender.obj `??_GCSalRender` (45 B) relocates ??_7CSalRender,
  ??1CSalMutex, ??_7CCodaObjectBase, _CodaFree: it EXPANDS ~CSalRender. Same in every SDK same-object ??_G (??_GCSalRenderPanDelay /
  Pan / Disable / Hrtf / HrtfMMX are 36 B = the expanded 11-B dtor). Only the SIMD ??_G in SalSw3DVoice.obj call (dtor in another
  object). Retail calls them all (30 B). The SDK 2.10 build also calls ~CSalMutex out of line where retail expands it, so the two
  builds differ in inlining. Per the coordinator, the ruling goes back.
- (b) CATALOG_CHANGES now carries CompilerFlags /O2 /Oy /W3 /Ob1 for the unit (4) and the 5 derived dtor binders (the rows that need
  it). Ctor binders and the two new ??_G binders use default flags (they grade and their copies match either way).
- (c) /Ob0 does not grade the same: 0x00C6E630 DIFFER 25v34, 0x00C6E6A0 DIFFER 34v38, 0x00C6CA20 DIFFER 146v150 (the inline CSalMutex
  bodies are then called). /Ob1 is the only setting that matches every row.
- SHrtfHeader removed: 0x00C70120 / 0x00C70B00 now use the SDK CSalRenderHrtf(MMX) leading members `m_audioBuffer[0x190]` (+0x28) and
  `short* m_pAudio` (+0x1B8, = align16(m_audioBuffer + 0x80), the first statement of the ctor body). RELOCATION_MATCH, HEAD tests PASS.
- Two more binders: 0x00C70A30 / 0x00C71400 (??_GCVtable_012bedcc / bede0) on CSalRender.h; a TU-local reference ctor (anonymous
  namespace, NF404 scaffolding form) emits the ??_G. They now reference ??0CSalRender. pk: the same pre-existing unreached 18-byte NEQ
  ??0CVtable_012bedcc / bede0 pick-any copy as HEAD; 0 flips.
- ClearFilterHistory second source (body tie of slot 1): the slot-1 overrides 0x00C6CAD0 (23 B) / 0x00C6F300 (58 B) / 0x00C6D560 (44 B)
  equal the SDK ?ClearFilterHistory@CSalRenderPanDelay / PanDelayMMX / PanDelaySSE2 sizes 23 / 58 / 44 exactly, so slot 1 is
  ClearFilterHistory by body. HrtfMMX 0x00C701E0 is 92 B v the SDK 19 B (which calls mmxMemclear192) and does not tie.
- Gates on the 16 rows: grades (strict), tests PASS, pk HEAD v new + reachflip 0 flips over 16 pairs, descriptive_check 0 FAIL.
  Not rerun this resume: vland dry run, name_gate diff, vt432 slot check (step 5).

## Still blocking (closure): 0x00C55270 cannot be ported as asked without a larger cascade
- 0x00C55680 is `?Initialize@CSalSw3DVoice@@AAE_NW4CHRTFMODE@@@Z`: SDK 96 B = retail 96 B, and its SDK relocations
  (CreateRenderer, ?Initialize@CSalRender) are retail's two calls (0x00C55270, 0x00C6E660). Both SDK names are PRIVATE (`AAE`). A binder that keeps the
  self ?Open@CAudioSource cannot call the private CSalSw3DVoice::CreateRenderer (the access is part of the mangling), so 0x00C55680
  must be renamed to its SDK name.
- That moves the problem to its caller 0x00C556E0 (table ?Open@CAudioSource witness). It is `?Create@CSalSw3DVoice@@UAEHPBVCCodaSource@@@Z`:
  SDK 99 B = retail 99 B, with relocations CSalSwBaseVoice::Create, l_frameBuffer, CSalSwBuffer::Create, Initialize. It calls the private Initialize, so it must
  be a CSalSw3DVoice member too. It is virtual. Its other callees' HEAD spellings (stand-ins) would then have to be kept from inside a headered class.
- So the closure is: 0x00C55270 (+8 funclets), 0x00C55680, 0x00C556E0, the 0x00C6E660 fold-in (R-NF444-2), and the derived-ctor callees
  of 0x00C55270 kept under HEAD's spellings (local models of CVtable_012b8a8c ... on CSalRender.h; they must override Process / SetGain /
  SetPosition with SDK signatures to stay concrete for `new`, which changes only unresolved vtable-slot names, none of them table-bound).
  The scope is larger than this resume could carry. The locks stay NF444 (27 plus 0x00C556E0 not yet locked).

## Resume 2 (R-NF444-1 granted; renames of 0x00C55680 / 0x00C556E0 accepted)
- Noted dissent: SDK 2.10 expands ~CSalRender in its ??_GCSalRender (45 B) and every same-object ??_G (36 B). That build also calls
  ~CSalMutex out of line where retail expands it, so it inlines differently from retail. The basis is retail's own bytes:
  /Ob0 fails three rows, /Ob2 fails the separate ??_G bodies, and only /Ob1 does both. /Ob1 is already on 0x00D1EB80 and 0x00C95580.
- Whole TU flagged: CATALOG_CHANGES carries CompilerFlags /O2 /Oy /W3 /Ob1 for all 16 rows; the payload carries /Ob1 for all 16.
- Scope check (evidence/ob1_scope.txt): a seeded sample of 20 landed rows in 0x00C50000..0x00C7FFFF (not in this batch), regraded with
  their catalog flags + /Ob1: 20 / 20 still MATCH / RELOCATION_MATCH. The range also holds non-GameCODA rows (0x00C623F0, 0x00C628A0).
- Derived binder models now declare the retail-owned overrides with the SDK signatures (ClearFilterHistory for PanDelay / HrtfMMX /
  HrtfSSE2; Process / SetGain / SetPosition for all five), so the emitted derived vftables no longer put CSalRender's slot 1 or
  `_purecall` where retail has an own method.
- vt432 (evidence/vt432.txt; table_post.tsv = reloc_bindings with this batch's TABLE_CHANGES; VT_SELFMAP = selfmap.tsv): agree 8,
  WRONG 4, unknown 76.
  - The 4 WRONG lines are all slot 0 of ??_7CCodaObjectBase: `??_ECCodaObjectBase` against retail 0x00C3C840, whose landed self is the
    stand-in ??_GCVtable_012b5868. This is pre-existing naming debt at 0x00C3C840 (it is CCodaObjectBase's ??_G); the landed
    0x00C5ECA0 / 0x00C5ED90 rows emit the same vftable.
  - CSalRender 0x012BEDA4: slot 0 ??_E -> ??_G 0x00C6E6D0, slot 1 ClearFilterHistory 0x00C6E690 (agree). Slots 2-4 are `__purecall`,
    and retail 0x00BFEA08 x3 is the LIBRARY_MATCH _purecall; vt432 lists them unknown because the table does not bind __purecall.
  - The derived slots are unknown: their override names are not bound yet. They are the step-2 debt.
- Oracle fix: 0x00C70A30 / 0x00C71400 are 30 bytes (catalog 30); resume 1 wrote 28-byte oracle rows by mistake (fixed).
- The strict verify_and_land dry run (--recheck-landed) gives WINS 16 / 16 (evidence/vland_new.log). reachflip over 16 pairs: 0 flips.

## Frozen closure (evidence/closure2_head.txt, closure_head.txt; reloc_bindings)
- Table binders: ?Open@CAudioSource -> 0x00C55680 (witness 0x00C556E0); ?Prime@CStreamHandle -> 0x00C6E660 (witness 0x00C55680);
  ?CreateStream@CAudioSource -> 0x00C55270 (witness 0x00C55680). Nothing in the table binds 0x00C556E0.
- Retail vftable 0x012B8AE0 (CSalSw3DVoice, 28 slots): 0x00C556E0 is slot 1. The header chain gives Create slot 1: CCodaObject's
  ~CCodaObject is slot 0 under FABLE_RETAIL_PASS22_CSALSW_CALCPARAMS, and CSalVoice's first virtual is Create. So the existing order
  agrees at slot 1. Slots 2..27 were not checked.
- Objects that reference the old spellings:
  - 0x00C556E0 (UND ?Open@CAudioSource) is the only referencer of 0x00C55680;
  - 0x00C55680 (UND ?CreateStream, ?Prime) is the only referencer of 0x00C55270 / 0x00C6E660;
  - 0x00C55270 and its 8 funclets define only their own symbol;
  - no object references 0x00C556E0 by name.
- Frozen set: 28 rows. These are the 16 built rows plus 0x00C6E660 (into unit 00c6e630), 0x00C55270, 0x01216180, 0x0121618B, 0x01216196,
  0x012161A1, 0x012161AC, 0x012161B7, 0x012161C2, 0x012161CD, 0x00C55680, 0x00C556E0. No other private caller appears.
- NOT ported in this run (context exhausted): 0x00C55270 (+8 funclets), 0x00C55680, 0x00C556E0, the 0x00C6E660 fold-in.
- name_gate HEAD v new on the 16 rows (evidence/name_gate_diff.txt): every new FAIL is at a batch-owned address (0x00C6E630 / 690 / 6A0,
  vftable 0x012BEDA4) or a RELOC_NAME label for the batch's own renames, except `??_7CSalMutex` at 0x012B8FC8. That spelling is
  already bound in the table (witness 0x00C5ED90) and used by landed 0x00C5ECA0 / 0x00C5ED90; HEAD's rows used the stand-in
  ??_7CCriticalSectionOwner_012B8FC8 there. So it is an existing spelling, not a new split. No new LOCAL_CLASS (SHrtfHeader is gone).
