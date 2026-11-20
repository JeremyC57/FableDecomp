# NF140 notes (round 10, PRIORITY slice 11 fourth pass)

Counts (STATUS.tsv / APPLIED.tsv): 5 entries: 3 done (q4056, q4197, q4198), 2 open (q4942, q4943: not started, context; long-slice exit).
36 rows listed: 32 ported (MATCH 1, RELOCATION_MATCH 31, all PASS), 2 not-a-binder (0x0042E98F, 0x009A3640), 2 not reached (the q4942 /
q4943 self rows 0x009FBF90 / 0x009FBFB0). 0 blocked. recheck_final.log: one vland run over the 32 ported rows. selfcheck: 0 problems.

## Locks
- 0x009BFE70, 0x009A1660, 0x009A4350 carried stale NF136 locks; the coordinator confirmed (2026-11-20) they read `released` after the nf136
  landing and told me to claim them. Claimed (owner NF140). Also claimed: 0x009BFF40, 0x009C0020, 0x009C00C0, 0x009C11A0, 0x0041641F,
  0x00417568, 0x00B24690, 0x009A59A0.

## q4056 (CDisplayManager::ResetDevice family, done)
- Both PDBs: ?ResetDevice@CDisplayManager@@AAE?AW4ECreateDeviceSuccess@1@ABUCCreateDeviceParameters@1@@Z. Nesting ECreateDeviceSuccess and
  CCreateDeviceParameters moves three more addresses onto their PDB leaves, so the family is 10 rows, handed back whole:
  self 0x009BFB30; binders 0x009BFE70, 0x009BFF40, 0x009C0020, 0x009C00C0; 0x009BFF40 SetDisplayMode leaf (callers 0x0041641F, 0x00417568,
  0x00B24690; 0x0042E98F already bound the PDB spelling, so the address had 3 spellings at HEAD and has 1 now); 0x009C0020 SetExclusiveMode
  leaf (caller 0x009A59A0); 0x009C11A0 copy constructor (caller 0x009C00C0). Exe-scan REFS (work/famrefs.tsv) of all five addresses: every
  referencing row is in the family.
- Form: in-place respell of each row's existing local CDisplayManager model (advisor decision 2026-11-20, "in-place respells of an existing
  local model"): the class tokens are unchanged (no row defines a class it did not define at HEAD); enum / struct moved inside the local
  CDisplayManager; enumerators take the Ego_d.pdb / engine/CDisplayManager.h text (FAILED 0, DEVICE_LOST 1, SUCCESS 2) mapped by value (the
  rows' old local names disagreed with each other: FAILED was 1 in four rows). objdump -t diff against the HEAD objects (work/headsyms/):
  only the own leaves change, plus the inline copy-constructor COMDAT of the nested struct in 0x009BFE70 / 0x009BFF40 / 0x009C0020.
- 0x009BFF40's own parameter list changes (`long refreshRate` -> `unsigned long`): both PDBs end `...W4ESurfaceMultisampleType@@K@Z`
  (two sources); its three other callers already declared it unsigned.
- 0x009C11A0: re-authored on engine/CDisplayManager.h (no local CDisplayManager existed to respell). It is the compiler-generated copy
  constructor: CCreateDeviceParameters declares none (FableWin.pdb lists only its default constructor besides this one), and C2DExtentsI has
  a user copy constructor (FableWin.pdb ??0C2DExtentsI@@QAE@ABV0@@Z; existing opt-in FABLE_RETAIL_C2DEXTENTSI_INITIALISE), so the implicit
  copy is memberwise. Emitted by the nf86 anonymous-namespace scaffolding as a pick-any COMDAT: MATCH. No header change. One definition
  improves: at HEAD the owner was a noduplicates COMDAT (selection 1) beside three pick-any copies of the same symbol; now all four are
  pick-any. Its test is rewritten on the header (same PassPattern).
- Debt (not changed): the self row 0x009BFB30 and the binders are still local models of CDisplayManager / CSurface / CPixelFormat ...
  (WARN SHARED_HEADER_AVAILABLE, as at HEAD); a header port of 0x009BFB30 is the CreateDevice 0x009BF7E0 recipe. 0x009BFB30 still binds
  0x009BEBB0 as GetDepthBufferFormat and 0x009BE2D0 under a public global-struct spelling (the CreateDevice row binds the PDB spellings):
  existing splits, untouched. 0x009BFE70 own leaf and 0x009BFF10 are in NAME_FIXES.
- Tests: all nine binder / self tests are self-contained (body verbatim, no link to the row object) and still PASS unchanged.
- Funclets: no Unwind funclet includes any of the family files.

## q4197 / q4198 (CMemoryAllocator::EnterSynchronisation / LeaveSynchronisation, done)
- Both PDBs: ?EnterSynchronisation@CMemoryAllocator@@KIXXZ / ?LeaveSynchronisation@CMemoryAllocator@@KIXXZ (protected static __fastcall;
  Ego_r 0x00B45B80 / 0x00B45B90 beside Init / UninitSynchronisation, FableWin 0x030BFA30 / 0x030BFA50). The bodies (EnterCriticalSection /
  LeaveCriticalSection of 0x013CAA70) are the lock the header's inline Allocate takes.
- Retail callers reach the protected statics only through the header inlines, so every binder was ported, not respelled:
  - 8 allocate-path rows (0x009C5310, 0x009D52D0, 0x00BD9AF0, 0x00BD9C40, 0x00BDA360, 0x00BDA3D0, 0x00BDA470, 0x00F370C0) onto the existing
    inline CMemoryAllocator::Allocate (transformer tools/alloc.py, proved by hand on 0x00BD9AF0; 0x009D52D0 needs the ALLOCATE_FORCEINLINE
    opt-in);
  - 12 deallocating rows (0x009A1660, 0x009A4350, 0x009C6020, 0x009C6D60, 0x009D61F0, 0x00A02190, 0x00A02220, 0x00A02370, 0x00A02570,
    0x00BD9B90, 0x00BD9CE0, 0x00BDA150) onto a new opt-in inline CMemoryAllocator::Deallocate (FableWin.pdb SIXPAX; the body of the landed
    0x009F2B20; proved on 0x00BD9B90 in 3 forms, then tools/dealloc.py graded the other 11 first time).
  - 0x009A3640 already bound the claim (not-a-binder).
  The local CMemoryAllocator models and the GF* stand-ins (GFCreateGlobalAllocator, GFEnter/LeaveAllocatorLock, the GFAllocate /
  GFFreeFromGlobalAllocator FABLE_FORCEINLINE helpers) are gone from all 20 rows; no row gained a class definition (selfcheck).
- Self rows 0x009F2000 / 0x009F2010 re-authored on engine/CMemoryAllocator.h under the PDB leaves; the critical section 0x013CAA70 is bound
  as the existing ?g_allocatorLock@@3PAEA (0x009F2310 / 0x009F2B20 spelling; no PDB data name found), dropping two stand-in spellings.
- Spelling counts (exe-scan referencers; work/headsyms vs final objects): 0x009F2000 and 0x009F2010 4 -> 1; the incidental callees move onto
  spellings landed rows already bind (Create KIXXZ and IAE InternalAllocate: 0x009F2BE0; PMemoryAllocator: 0x009A3640; IAE InternalDeallocate,
  IAE ~CMemoryAllocator, DestroyWhenEmpty: 0x009F2B20; GetNoAllocations QAEKXZ: 0x009A1660), except operator delete 0x009F1F90, which moves
  from ??3CMemoryAllocator@@SIXPAX@Z to the PDB ??3CMemoryAllocator@@KIXPAX@Z (both PDBs) in all ten of its referencers in this hand-back;
  the eleventh, 0x009A0AE0 (stand-in ?FreeThis@CEngineSubPrimitiveGameMesh@@QAEXXZ), keeps its spelling, so the count stays 2.
  0x013CAA94 drops from 5 spellings to 2 (DestroyWhenEmpty + 0x0122A9A0's ShuttingDown); 0x009F1F10 / 0x009F2870 / 0x009F2920 to 1.
- One definition improves: three HEAD rows (0x009A1660, 0x009C6020, 0x00A02220) defined ?GFFreeFromGlobalAllocator@@YIXPAX@Z as an inline
  COMDAT, the strong own leaf of 0x009A4350; they no longer do. New defined symbols are the header inlines only (Allocate, Deallocate,
  Destroy pick-any, ??_GCMemoryAllocator now IAE).
- 0x009A4350 keeps its stand-in own leaf; it is the out-of-line CMemoryAllocator::Deallocate (NAME_FIXES, one PDB).
- Tests: every test of these 22 rows is standalone (own copy of the code) and passes unchanged.
- Funclets: none include these files.

## Decision items for the coordinator
1. One definition of the CCreateDeviceParameters copy (q4056): 0x009BFE70 / 0x009BFF40 / 0x009C0020 define
   ??0CCreateDeviceParameters@CDisplayManager@@QAE@ABU01@@Z, the own leaf of 0x009C11A0, as inline COMDATs (they did the same under the
   old global spelling at HEAD). Link proof (link/, LINK= cleared, /DLL /NOENTRY /NODEFAULTLIB /FORCE:UNRESOLVED): HEAD 0x009C11A0 + HEAD
   0x009BFE70 -> LNK2005 + LNK1169 (exit 145, link/ctl.log); new 0x009C11A0 + new 0x009BFE70 / 0x009BFF40 / 0x009C0020 -> exit 0, LNK4088
   only (link/new.log). The allocator family links too: HEAD 0x009F2B20 + new 0x009A4350 / 0x00BD9B90 / 0x009C6020 / 0x009F2000 /
   0x009F2010 / 0x009D52D0 / 0x00BD9AF0 -> exit 0 (link/alloc.log).
2. Inline control (q4197 / q4198): the forced expansion of Allocate (0x009D52D0 only) and of Deallocate / Destroy (12 rows) is a
   FABLE_FORCEINLINE in shared-header opt-ins (precedent: FABLE_RETAIL_C2DEXTENTSI_FORCEINLINE_CTOR; the landed 0x009F2B20 and the HEAD
   rows already forced the same bodies row-locally). EXTRA_RULES 2026-11-08 prefers a per-row /d2inl flag (preference 2); I did not sweep
   one. Each affected row carries an `// Inline-budget:` line. If the flag form is required, 0x009D52D0 and 0x00BD9B90 are the rows to sweep.
3. 0x009A4350 is CMemoryAllocator::Deallocate out of line (NAME_FIXES, FableWin only); left on its stand-in leaf.

## Descriptive names (no PDB name found; existing landed spellings reused, none new)
- ?g_allocatorLock@@3PAEA (0x013CAA70, the allocator CRITICAL_SECTION; spelling of 0x009F2310 / 0x009F2B20), now also bound by 0x009F2000 / 0x009F2010.
- ?DestroyWhenEmpty@CMemoryAllocator@@1_NA (0x013CAA94; spelling of 0x009F2B20), now in the header opt-in.
- The kept stand-in leaf ?GFFreeFromGlobalAllocator@@YIXPAX@Z of 0x009A4350 (see decision 3).
- Scaffolding: 0x009C11A0 carries `// Fidelity: scaffolding, not retail` (the nf86 anonymous-namespace function that makes the TU emit the
  implicit copy constructor).

## Judgment calls disclosed
- 0x009BFB30: the local enum token ECreateDeviceResult became the nested ECreateDeviceSuccess (the PDB / header name; the leaf requires it).
- 0x009A59A0: its local CDisplayManager model gained a nested enum ECreateDeviceSuccess (it declared SetExclusiveMode as returning long);
  0x0042E98F is the landed precedent for the same nested enum in a caller model.
- Enumerator text follows Ego_d.pdb / the header; local rows had disagreeing enumerator names (values unchanged).

## Debt left (non-blocking)
- Tests of 0x009F2000 / 0x009F2010 (and the q4056 binders) are standalone copies of the old stand-in code; they pass but test the old names.
- 0x009BFB30 and the CDisplayManager binders remain local models (WARN SHARED_HEADER_AVAILABLE as at HEAD).

## Rule slips
- 18 q4197 / q4198 binder rows were drafted and graded in work/ before their locks were claimed; the locks were then claimed (all free, no
  acctB claims, nothing written outside my directory). The q4056 locks were claimed before drafting.
- One Bash call exited non-zero (an objdump symbol listing whose grep pattern needed -E; first baseline attempt), and one call was blocked
  by the harness (a foreground sleep). A `| head` on a loop cut the symbol-list loop short once; the missing list was regenerated (1 extra
  grade on 0x00BD9AF0, counted).

## Resume (q4942 / q4943)
Start from work/NF133_round1/APPLIED.tsv; re-probe 0x009C8700 / 0x009C8FB0 (nf131) and the six public-QAE rows (0x00A25530, 0x00A25E70,
0x00A269C0, 0x00A26A50, 0x00A26C60, 0x00A42FC0); July rows 0x00BEC000 / 0x00BEC470 / 0x00BEC950 are left as found.
