# NF444: GameCODA render-class header port (pilot)

No repo writes, no git. One batch, `batches/nf444_hdr` (NF444: 14 rows, header CSalRender.h; NF445 grew it to 28, see the NF445 sections). Every row grades MATCH / RELOCATION_MATCH,
NF444 held the batch for closure (below); NF445 closed it (all 28 ACCEPT). Locks: all 28 held by NF445.

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

# NF445 (takes over NF444; work dir agentNF445, batch still named nf444_hdr)
- Disclosure: one read-only `git log -1` was run at the start by mistake (no other git use).
- Step 1: overlay hdr/engine/CSalSw3DVoice.h opt-in FABLE_RETAIL_CSALSW3D_CREATE (under PASS23): includes CodaProcessingTypes.h (the
  shared CHRTFMODE typedef), public virtual Create(const CCodaSource*), private Initialize(CHRTFMODE) / CreateRenderer(CHRTFMODE).
  hdr/engine/CCoda3DSource.h opt-in FABLE_RETAIL_CCODA3DSOURCE_HRTFMODE: SDK accessor GetHrtfMode() (m_HrtfMode, retail +0x1CC).
- 0x00C556E0 Create: RELOCATION_MATCH first grade; macro set of 0x00C54C10 + the two opt-ins. Callee spellings kept through
  declaration-only shells (CSoftwareVoice_00c5a1d0::Setup, CAudioRingBuffer::Init, g_scratchRing_0143bcb0); Is3D / +0x1CC through
  CCodaSource::Get3D + GetHrtfMode. UND diff v HEAD: only ?Open@CAudioSource -> ?Initialize@CSalSw3DVoice.
- 0x00C55680 Initialize: RELOCATION_MATCH first grade (member stores; float 0.0f shares ebx). UND diff: CreateStream -> CreateRenderer,
  Prime -> ?Initialize@CSalRender.
- 0x00C55270 CreateRenderer (/EHsc, no /Ob1 needed): RELOCATION_MATCH first grade. Derived local models on CSalRender.h (MEMBERS):
  overrides with SDK signatures; derived dtors declared out of line (retail 0x00C554E0 = `jmp 0x00C6CAC0`, so ~CVtable_012b8a64 now
  jumps to ??1CVtable_012bed7c, the landed 0x00C6CAC0, instead of a local copy). UND diff: only vtable-slot names (none table-bound)
  and ??1CVtable_012bed7c (DEF pick-any -> UND landed self).
- 0x00C6E660 folded into unit 00c6e630 (owner body between ~CSalRender and ClearFilterHistory); thin row MATCH /Ob1.
- 8 funclets 0x01216180..0x012161CD: source = HEAD include of the parent (comment respelled), parent copied to $W/src/compiled/00/c5/ under its
  HEAD file name; vland grades them with funclet_offset 0..77: RELOCATION_MATCH x8. Tests = the parent's linked test (HEAD has no
  rebuild/tests/01/21/global_Unwind01216196_01216196_test.cpp although the catalog names it: the batch supplies tests/01216196_test.cpp).
- 7 NF444 binder tests rewritten (00c6e760, 00c70120, 00c70b00, 00c701d0, 00c70bb0, 00c70a30, 00c71400; tools/mk7tests.py): linked
  against the row object over CSalRender.h; the HEAD ones modelled CVtable_012beda4 / CCodaObjectBase locally. 3 mutants FAIL.

## NF445 gates (all 28 rows; batches/nf444_hdr/evidence/)
- vland_new.log: strict (LANDVERIFY_STRICT_HEADERS=1) --recheck-landed WINS 28 / 28, every test PASS. Warns: STANDIN_LEAF x4 (catalog names
  the batch renames), RELOC_NAME = batch renames + pre-existing callee labels (C3DAnimationUtil ctors, CVtable_012B5C60::Initialise).
- fgate.txt (tools/fgate.sh): H v N pickany_vs_retail / pk_all (funclets --funclet); reachflip pairs 28, 0 flips. Only N NEQ: the
  pre-existing unreached 18-byte ??0CVtable_012bedcc copy in 0x00C70A30 / 0x00C71400 (same in HEAD). pk --addrs=addrs_batch.tsv (only
  the 8 names this batch binds): unit objects EQ 5, funclets EQ 2 (parent + __ehhandler 0x012161D8), 0 NEQ.
- mutants.txt: 556e0 x2, 55680 x2, 55270 x3, 6e660 (owner) x1, 70120 / 6e760 / 70b00 x1: every mutant DIFFERs and FAILs its test.
- name_gate_{head,new}.txt + name_gate_diff.txt (tools/ngate2.sh, ngdiff.py; payload flags, tests included, summary line asserted):
  33 new FAIL lines, all at batch-owned addresses (splits / agree_unverified the batch's TABLE_CHANGES and self respells resolve) or
  RELOC_NAME of the batch's renames, except ??_7CSalMutex@@6B@ at 0x012B8FC8 (00c6e630 / 00c6e6a0): an existing table spelling
  (witness 0x00C5ED90), not a new one; HEAD's rows used ??_7CCriticalSectionOwner_012B8FC8 there (witness 0x00C5D570, not ours).
- descriptive_check.txt: --extra BEST_GUESS.tsv (header only): 0 FAIL.
- equiv445_includers.tsv: gcc-cpp equivalence, repo include v overlay (3 headers), 32 includer rows (src + test): 0 differ.
- closure_new.txt: no new object defines or references any old spelling (CVtable_012beda4, CreateProcessor, Open / CreateStream /
  Setup@CAudioSource, Prime@CStreamHandle, CRenderCall_00c6e660, Hook / VecDel@T). closure2_outside.txt: HEAD objects of the 37 other
  sources that mention the old names: no UND; 0x00C70750 / 0x00C71120 only DEF their own inline ?Lock@CVtable_012beda4 (pre-existing).
- unddiff_head_v_new.txt: UND / DEF diffs are only the batch renames and vtable-slot names (none table-bound).
- vt432.txt (VT_SELFMAP=selfmap445.tsv, table = reloc_bindings + this batch's TABLE_CHANGES): 0x012BEDA4 uauuu (slot 1 agree; 0 ??_E,
  2-4 _purecall unknown); WRONG only CCodaObjectBase slot 0 (pre-existing ??_GCVtable_012b5868 at 0x00C3C840) and the scratch
  CSalSw3DVoice table (below).
## Finding: CSalSw3DVoice vftable 0x012B8AE0 v the header's PASS22 slot order (evidence/vt_012b8ae0_slots.tsv, vt_012b8ae0_retsig.tsv,
##   vt_scratch_sw3dvt.cpp; tools/vtsig.py)
- 0x012B8AE0 has 28 slots (0x012B8B50 is a separate vftable, referenced at 0x00C557E4). The scratch TU (evidence only, macro set of the
  batch's voice rows) emits 28 slots: CSalVoice.h's FABLE_RETAIL_PASS22_CSALSW_CALCPARAMS branch (Create, SetBuffer, Release, IsSoftware,
  Is3D, IsCreated, ...) then the CSalSwBaseVoice protected virtuals. (SDK 2.10's own CSalVoice introduces Create, SetBuffer, IsSoftware,
  Is3D, PlayBuffer ...; Release / IsCreated / IsVoice are CCodaObject overrides there, so the 2.10 order itself does not fix slots 2-3.)
- Argument sizes (ret imm; tail jumps followed) agree with the header in 26 of 28 slots (0, 1, 4-27).
- Slots 2 and 3 do not, as argument-size evidence only: retail slot 2 0x00C5A1F0 takes no argument (CSalSwBuffer::Release on +0xB4,
  m_pcBuffer = 0, tail jump to 0x00C59710 `mov [ecx+2Ch],80000000h; ret`); retail slot 3 0x00C3F460 takes one and returns 0
  (m_pcBuffer = 0; xor eax,eax; ret 4). The header has SetBuffer(const CCodaSource*) (one argument) at slot 2 and Release() (none) at
  slot 3. Not changed: no batch row emits or calls these slots.
- vt432 WRONG on the scratch table (slots 4-6, 13, 19-22, 25-27) are spelling clashes with landed stand-in selves at those addresses
  (?Get@T@@, ?Hook@T@@, ?Start@CAudioSource ...), not order differences; slot 1 = ?Create@CSalSw3DVoice (0x00C556E0) as expected.
## Regression to rule on (R-NF445-1): SIMD vftable slot spellings in 0x00C55270
- 0x00C55270's emitted ??_7CVtable_012b8a64 / 8aa0 name SDK-signature overrides (evidence/slotnames_55270.tsv). With the landed slot leafs
  in VT_SELFMAP (selfmap445_slots.tsv), vt432 goes uWaaW -> uWWWW on both tables (vt432_slots_H.txt v _N.txt): HEAD agreed with the
  landed Process (0x00C6D630 / 0x00C6F3E0) and SetVolume (0x00C6D120) leafs; the new object agrees with none. Not table-bound, so
  name_gate / pk_all / the tracker do not see it. Unavoidable under the settled SDK-signature design (CSalRender's pure Process can only be
  overridden with its own signature). The NF444 ctor binders' derived vftables are in the same state (HEAD emitted a 1-slot v00 table).
  Proposal: land, then respell the slot rows (NAME_FIXES.tsv, RULINGS_NEEDED.tsv).
- Closure re-checked (advisor): xref gives one retail caller each for 0x00C55680 (0x00C5572E), 0x00C55270 (0x00C55689), 0x00C6E660
  (0x00C556A3), all batch rows; 0x00C556E0 is reached only through vftable slot 0x012B8AE4. closure2_caudio.txt: HEAD objects of all 68
  sources naming CAudioSource / 0x012B8AE0 reference no old self spelling of the batch outside the batch's own rows.

## NF445 resume: R-NF445-1 conditional checks (coordinator grant; batch unchanged, all checks pass)
1. Signature check (evidence/sigcheck_19.tsv, tools/sigcheck.py): all 19 RULINGS_NEEDED addresses take the same argument bytes as the
   CSalRender.h SDK slot signature (slot 1: 0, slot 2: 8, slot 3: 12, slot 4: 28) and the same argument count. For the three:
   - 0x00C6D630: landed ?Process@CVtable_012b8a64@@UAEXPBFPBQAJ@Z; 0x00C55270 emits ?Process@CVtable_012b8a64@@UAEXPBFAAY03PAH@Z; retail ret 8.
   - 0x00C6F3E0: landed ?Process@CVtable_012b8aa0@@UAEXPBFPBQAJ@Z; emitted ?Process@CVtable_012b8aa0@@UAEXPBFAAY03PAH@Z; retail ret 8.
   - 0x00C6D120: landed ?SetVolume@CVtable_012bed7c@@UAEXMK_N@Z; emitted ?SetGain@CVtable_012bed7c@@UAEXMPBM_N@Z (also by 00c6ca20 / 00c6cac0); retail ret 0Ch.
   Same size, different types / name: the landed stand-ins are the stale side -> OK to land.
2. vt432 HEAD v new over every vftable-emitting row object (vt432_full_{H,N}.txt: self_symbols.tsv + batch selves; vt432_slots_{H,N}.txt:
   + the landed leafs of the slot rows, which self_symbols.tsv lacks), per-slot diff vt432_slotdiff.tsv (tools/vtdiff.py):
   - agree -> WRONG: only 0x00C6D630, 0x00C6F3E0 (slot 2) and 0x00C6D120 (slot 3) in 0x00C55270's two tables (and its 8 funclets).
   - unknown/absent -> WRONG: the binder slots (visible debt) and 0x00C3C840 CCodaObjectBase slot 0 through the thin rows (already WRONG in
     HEAD 00c6e630 / 00c6e6a0). Everything else: absent <-> unknown (newly emitted slots) or -> agree (ClearFilterHistory 0x00C6E690, 9 slots).
   - DEBT_SLOTS.tsv: 20 addresses (address, slot tables, emitted spelling, landed leaf, kind, emitting rows).
3. /FORCE link, one object per TU (H 20 objects, N 16; tools/forcelink.py, evidence/forcelink_{H,N}.log, forcelink_delta.txt):
   unresolved 27 -> 35. Added (22): only vtable-slot override names (ClearFilterHistory / Process / SetGain / SetPosition of CVtable_012b8a64,
   8aa0, bed7c, bed90, bedb8, bedcc, bede0). Removed (14): ?CreateStream@CAudioSource, ?Prime@CStreamHandle (now defined in-batch),
   ?AudioFree@@YAXPAX@Z, ?Dtor_00c6e630@T, the stand-in slot names (?Clear / ?Update@CVtable_012beda4, ?Process@CVtable_012b8a64 / 8aa0
   PBQAJ, ?SetVolume@CVtable_012bed7c) and five ?v00@ stand-ins. Duplicates 3 -> 2: ??1CVtable_012bed7c (HEAD 0x00C55270 local copy v
   0x00C6CAC0) is gone; the pre-existing ??0CVtable_012bedcc / bede0 (0x00C70A30 / 0x00C71400 reference ctors v 0x00C70120 / 0x00C70B00) remain.
4. Emitters (evidence/emitters_8a64_8aa0.txt): of the 10 sources naming 012b8a64 / 012b8aa0, only 0x00C55270 (and its 8 funclet rows, same
   TU) emits ??_7CVtable_012b8a64 / ??_7CVtable_012b8aa0. The SIMD ctor rows 0x00C55070 / 0x00C550D0 store the same retail vftables but emit
   them as ??_7C3DAnimationUtil@@6B@ (one spelling for both 0x012B8A64 and 0x012B8AA0); 0x00C554C0 / 0x00C55550 / 0x00C554E0 / 0x00C55570
   emit no ??_7.
5. 0x012B8AE0 slots 2 / 3 (evidence/voice_slot_calls.tsv, tools/voicecalls.py): retail 0x00C3C000..0x00C80000 has four virtual calls on a
   CCodaSource m_pVoice (+0x50), all through +0xC, none through +8: 0x00C43E95 (row 0x00C43E20), 0x00C43EF4 / 0x00C43F34 (0x00C43EC0),
   0x00C441B1 (0x00C44170). Each pushes the source (`push esi`, = this) and tests the int result (`test eax,eax`). The three rows grade
   RELOCATION_MATCH at HEAD and spell it as slot 3 Rebind(CAudioObject*) / DetachOwner(this). That is a SetBuffer(const CCodaSource*)
   call at slot 3, the second source against the header's PASS22 order (SetBuffer slot 2, Release slot 3). No landed row calls voice slot 2.
   Header not changed.

## NF445 fix after the failed landing (batch nf444_hdr_c; the coordinator rebuilt head/payload.json from orig/ = HEAD text)
- Cause of the two option S member test FAILs: the catalog PassPattern of 0x00C6E690 / 0x00C6E6D0 is `PASS_00c6e690` / `PASS_00c6e6d0`
  (HEAD gap-scan tests), while the batch tests printed `00c6e690_TEST PASS` / `00c6e6d0_TEST PASS`. NF444/NF445's own dry runs passed only
  because the batch payload carried its own pass_pattern. Fix: tests/00c6e690_test.cpp and tests/00c6e6d0_test.cpp now print
  `PASS_<addr>` / `FAIL_<addr>` (behaviour unchanged); head payload pass_pattern of those two set to the HEAD catalog value. All 28 head
  entries now equal the repo source and the catalog PassPattern.
- Landing simulation (tools/landsim.py; evidence/landsim_recheck.log, landsim_symcheck.txt): repo include + hdr/engine, catalog +
  CATALOG_CHANGES, rows at their HEAD Source paths (thin rows include CVtable012beda4_CVtable012beda4_00c6e630.cpp), tests at the HEAD
  TestSource paths, payloads from recheck.py, vland --recheck-landed with LANDVERIFY_STRICT_HEADERS=1 (+ LANDVERIFY_STRICT_LEAF=1 for the
  S members): main group WINS 24/24, option S members WINS 4/4 (all PASS); land_scripted's symbol rule: 0 BAD; renamed selves checked;
  no includer outside the batch.
