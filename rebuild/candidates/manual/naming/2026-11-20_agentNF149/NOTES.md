# NF149 notes (round 10, PRIORITY pick list: CIndexBuffer / CVertexBuffer, q4552 q4555..q4560 q6203)

Counts (from APPLIED.tsv / STATUS.tsv): 8 entries: 7 done, 1 blocked (q4552: row 0x00BF3380 left, blocked: port). 40 rows listed:
26 ported, 11 not-a-binder, 3 blocked (0x00BF3380 port; 0x00BE7D70 / 0x00BFE050 authoring). Final recheck: recheck_final.log, one vland
--recheck-landed run over all 26 ported rows.

## Method
- Census first (work/census.txt): every REFS row compiled from HEAD and its binding pairs at the 8 targets (+ vtable 0x0129BA20) listed.
  11 rows already bound the PDB spellings (not-a-binder); 19 bound stand-ins.
- Self rows (8) moved onto engine/CIndexBuffer.h / CVertexBufferBase.h / CVertexBufferWin32.h, then binders by family transformers
  (work/b/xf_*.py), each graded with a full binding-pair diff and a defined-symbol diff against the HEAD object (tools/pdiff2.sh).
- Self renames: every target address already had landed rows on the PDB spelling and nothing bound the old self leaves (census), so
  each address's spelling set only shrinks. 0x0129BA20: ??_7CVertexBufferWin32_Retail@@6B@ left by 0x009E2830 / 0x00A8E160 /
  0x00A8EDA0 / 0x00BDA3D0; 0x00BC5420 (not in my slice) still binds it (existing; follow-up).
- Self renames of rows outside the entry list (both PDBs, every matched referencing row already on the new spelling, refs.py):
  0x00BDA3D0 AllocateVertexBuffer, 0x00BDA360 AllocateIndexBuffer. 0x009E2830 (five-argument constructor): FableWin.pdb + the body
  (Ego_r has no out-of-line copy because it is inline in release; no landed row references 0x009E2830): the "PDB + body" rule.
- Collateral s2 (no address gains a spelling): 0x00A0AA70 (0x00AC1FB0: stand-in NotifyLock -> ?WaitWhileResourceIsBusy@CRenderManagerCore@@,
  the spelling 0x009FA450 binds; NAME_FIXES), 0x0099A2F0 (0x00A8EDA0: ??0CBase@@QAE@XZ -> ??0CBaseClassNonCopyable@@IAE@XZ).

## Decisions
- q6203 / trap B: the header's inline CVertexBufferWin32() emitted pick-any (nf86 `new` scaffolding under optimize("s")) graded
  RELOCATION_MATCH on the first grade: the retail store order (+4 / +0x10 before the vtable store) comes out of the header form, so no
  header lever was needed.
- 0x009E2E30: the full header port graded RELOCATION_MATCH but vland gave RELOC_IDENTITY (0x0099A2F0 bound twice in one row: the inlined
  buffer constructor's real CBaseClassNonCopyable and CFontManager's own N_0099a2f0::CBaseObject stand-in base). Kept the in-place respell
  of the row's existing local CVertexBufferWin32 model (Initialise's PDB parameter types only). Full port waits on engine/CFontManager.h.
- In place (accepted rule: respelling inside a local model the row already had): 0x009E2E30 only. 0x00BDA360 was first respelled in place,
  then fully ported (the 0x00BDA3D0 recipe).
- Binders whose owner class has a header but stays the row's existing local model (in place, no re-base, self leaf unchanged): 0x00BBF380,
  0x00BC3E10 (owner via a renaming macro at HEAD), 0x00B61FF0 / 0x00B620A0 (CSkyRendererMethods stand-in; 0x00B620A0 now uses the
  sky header's retail branch), 0x00BAA6A0 (CEnginePrimitiveManagerRepeatedZSpritesMethods), the C3DPrimitive2 rows. Their own leaves are
  stand-ins (PDB: ?BuildBuffers@...@@AAEXXZ, ?BuildStarFieldVB@CEngineSkyRenderer@@AAEXXZ, ?SetupVertexBuffer@C3DPrimitive2@@AAE_NW4EVertexType@@KKK@Z):
  follow-ups, not in my entries.
- 0x00BE7D70 / 0x00BFE050: July rows without a Leaf line, DIFFER as landed; they stand for the buffer calls with made-up free functions
  (?LockLandscapeIndexBuffer@@..., ?LockLandscapeBackgroundVertexBuffer@@...) that relocate at no retail target. Left alone (brief rule);
  recorded blocked: authoring. The entries are marked done on the matched binders.

## New local models (three-condition rule)
- The row-local `struct IDirect3DIndexBuffer9 : IDirect3DResource9` (+ `_D3DINDEXBUFFER_DESC`) in 0x00AC1F00 / 0x00AC1F10 / 0x00AC1FB0 and
  their tests: no header under rebuild/include defines IDirect3DIndexBuffer9 (CIndexBuffer.h / IDirect3DDevice9.h forward-declare it); the
  landed sibling 0x00AC1EE0 models it locally; its base IDirect3DResource9 comes from engine/Direct3DSurface9.h. SDK names and slots.
- Tests: CFakeIndexBuffer / CTestVertexBuffer are test doubles deriving from the row-local interface / the shared CVertexBufferBase.

## Tests
- Rewritten on the shared headers (link the row object, test doubles for callees): 0x00AC1F00, 0x00AC1F10, 0x00AC1F70, 0x00AC1FB0,
  0x00AC20A0, 0x00AC20B0, 0x00A63150, 0x00A8E160, 0x009E2830 (PassPatterns kept). Binder tests unchanged (standalone host copies; PASS).
  Test debt: those binder tests still model the buffers locally under the old stand-in names.

## Descriptive names
- None new. Scaffolding helpers ReferenceVertexBufferWin32Ctor / ReferenceVertexBufferWin32InitialiseCtor are anonymous-namespace,
  `// Fidelity: scaffolding, not retail` (nf86).

## Flags lines
- None added (every changed row grades with its build entry's flags; none had CompilerFlags).

## EH funclets
- No 01/21 Unwind row includes any changed row's file (grep of every changed file name under rebuild/src/compiled/01).

## Header debt / follow-ups (for the coordinator)
- engine/CIndexBuffer.h keeps PrimitiveType as `unsigned char[4]` (0x00AC1F70 stores through an EPrimitiveType cast): needs header claim
  to type it.
- 0x00BF3380 (blocked: port, NOT ATTEMPTED; resume it): port onto CEngineVertexBufferPool.h with two add-only opt-ins (CEngineDynamic3DResource<T,K>
  ctor PoolNext(0); ReleaseResources on CEngineDynamic3DResourceManager<T>). 0x00BF32B0 is not landed and this row is its only binder, and
  0x00BF3250 already binds the template vtable spelling, so the port creates no split. STATUS has the detail.
- COORDINATOR DECISION: 0x009E2890 (CVertexBufferWin32 ??_G row, not mine) models CVertexBufferWin32 locally; its object (compiled from HEAD,
  work/census_2890.txt) defines pick-any ??0CVertexBufferWin32@@QAE@XZ with an EMPTY body and ??_7CVertexBufferWin32@@6B@ with a one-slot
  layout, beside the header's COMDATs that 0x00A8E160 (and the header includers) emit. All pick-any, so no LNK2005, but the linker may pick
  the empty one. Pre-existing for the vtable; port 0x009E2890 onto the header (nf86 scaffolding) in the same landing or next.
- 0x00BC5420 still binds ??_7CVertexBufferWin32_Retail@@6B@ (0x0129BA20).
- Stand-in self leaves of the binders above (owner-header ports).

## One-definition check (advisor)
- Symbols defined by two or more hand-back objects (work/defs_new_*.txt, `uniq -d`): header inlines / compiler COMDATs only (constructors,
  vtables, ??_G, implicit ~CVertexBufferBase, GetSize, CMemoryAllocator::Allocate, <new>'s placement new, and the CArray / CMemoryDataInputStream
  in-class inlines the binder rows already had at HEAD).
- The nine new strong definitions (four CIndexBuffer members, CVertexBufferBase::Lock / Unlock, CVertexBufferWin32::Initialise, the two
  CEngineLandscapeMemoryManager::Allocate*Buffer) appear in no HEAD object of the other REFS rows (pH objdump) and in no out-of-class
  definition elsewhere in rebuild/src/compiled (grep). No link test was needed.

## Decisions the coordinator should see
- Five entries (q4555, q4557, q4558, q4559, q4560) are `done` on their matched binders while STATUS lists 0x00BE7D70 / 0x00BFE050 under
  them as `blocked: authoring` (July DIFFER rows that bind nothing). Both counts stand: 7 entries done / 1 blocked; 3 rows blocked.
- Surviving stand-in spellings on my targets after this hand-back (each set only shrinks; nothing is created): 0x00A63150
  (0x00BF3380), 0x0129BA20 (0x00BC5420), and the collateral 0x00A0AA70 / 0x0099A2F0 sets. On the strict reading of "every REFS row ends on
  the new spelling or the rename waits", the self rename to hold is rows/00a63150.cpp + rows/00a63150_test.cpp (q4552).
- Fitted wording: 0x00BDA360 / 0x00BDA3D0 allocate with `new (CMemoryAllocator::Allocate(sizeof(T), 0)) T`, standing for retail's
  expanded AllocVirtual 0x00BD9AF0 (which the header declares out of line); the bytes match, the exact original spelling is unproven.

## Rule slips
- Three Bash calls exited non-zero: the first capstone helper named dis.py (it shadowed stdlib `dis`), the objdump / grep loop over two
  rows (its last command failed), and the st.py call whose double-quoted note held backticks (the 0x00BF3380 note; rewritten via heredoc).
- Work went row family by row family (one row serves up to seven entries) rather than one entry to a terminal state before the next.
- 0x009E2E30's first form (full header port) reached vland and failed RELOC_IDENTITY before the in-place form was taken.
