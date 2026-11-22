# NF282 notes (2026-11-22, round 35b): tracker batch T1 standin_alias, first hand-back (CCountedPointer DeleteData, 60 addresses)

## What was done
- **Class worked:** CCountedPointer (the brief's "CCountedPointer's 103 first"; ENTRIES.tsv is actually sorted by class name with the
  empty-class lines first, CCountedPointer in the middle; I took the CCountedPointer block first as the brief asked).
- **60 addresses done** (work/sel60.tsv, STATUS.tsv `done`): each is a `CCountedPointer<T>::DeleteData` deleter whose ONLY stand-in
  binder was a generated "repeated body shape" Reset / (T*) ctor row with a local `SCountBlock` / `CCountedPointer_<x>` model and a free
  `void __fastcall Destroy_<addr>(void*)` stand-in (`?Destroy_<addr>@@YIXPAX@Z`). 60 binder rows respelled (rows/), one per address.
- **Form (decision to confirm, see Decisions 1):** in place in each binder's existing local model. The free stand-in declaration becomes
  `#define FABLE_RETAIL_CCOUNTEDPOINTER_DELETEDATA_DECL_ONLY` + `#include "engine/CCountedPointer.h"` + a forward declaration of T
  (`class T;` or `namespace N { class T; }`), and the count block's initialiser `Destroy_<addr>` becomes `&CCountedPointer<T>::DeleteData`.
  DECL_ONLY is the landed NF222 opt-in (rows 0x00419428 / 0x0041947A / 0x004194D1 / 0x0042FC13 / 0x0042FC86 / 0x00433D3A use it): with T
  only declared the header's inline body would otherwise emit a plain-delete pick-any copy. No header edit, no new local class, no self
  leaf change (the binders' own stand-in leaves `?Reset@CCountedPointer_<x>@@QAEXPAX@Z` / `??0CCountedPointer_<x>@@QAE@PAX@Z` stay: their
  port onto the header is a T3 self rename). Each row gains a `// Leaf:` line on line 2, pinned from the HEAD object's defined function
  symbol (the generated rows had none; nf271 precedent). Transformer: tools/xf_ccp.py (refuses a file with any `#include`, a template,
  or other than exactly two `Destroy_<addr>` tokens).
- **Scopes:** every `@Scope` in the 60 T spellings is a namespace (NParticleEngine, NPlayerGui, NGameText, NAnimationEvents,
  NTCScriptedControl, NHitLocations, QuadricOptimiserInternals: `namespace X` in rebuild/include, no `class X {`; the class hits are
  `friend class NPlayerGui::...` lines). Nested-class T (CTCCreatureModeManager::CModeBase, CTCAppearanceMorphBase::CMeshMorphSet,
  NParticleEngine::CPSCSplineBase::CSplinePosParamData) were NOT forward-declared as fake namespaces: deferred (open).

## Proof (evidence/)
- `ccp_identity.tsv`: per target, the PDB spelling is in the FableWin.pdb name set (60/60; Ego_r also for some), the retail binder body
  stores the target address as an imm32 (`mov dword ptr [eax+4], <addr>`, 60/60; scan of Fable.exe over the binder's oracle extent),
  and the self row's Leaf (equal to the PDB spelling for most; see "Self rows still on stand-ins" below).
- `ccp_exe_refs.tsv`: Fable.exe scan of every landed row that references each target: 51 targets have the respelled binder as their only
  landed referencer; 9 have one more landed referencer, and each of those already binds the PDB spelling (ENTRIES spellings column:
  0x00403D20, 0x0041C030, 0x00507650, 0x0067F9B0, 0x00A12EE0, 0x00A8B990, 0x00A99420, 0x00A9B640, 0x00ACA830) -- a real-typed second
  source tying the address to T. So every target ends on ONE spelling (2 -> 1); no address grows; no new split. Five more referencing
  sites lie in no landed row (unlanded@0x00A6A9C4 / 0x0067FBE6 / 0x00A133A6 / 0x00A9A3B6 / 0x00A9CE66, for 0x0041BD80 / 0x0067F650 /
  0x00A110D0 / 0x00A99FE0 / 0x00A9CB20): no landed spelling there to count.
- `ccp_binder_callers.tsv` (second source tying T to the address, no compile): the reloc_bindings lines whose TARGET is each binder.
  56 of 60 binders have a real-typed caller binding `CCountedPointer<T'>::Reset` / the (T*) ctor with T' = the entry's T (0x00403E40:
  caller 0x00416832 binds `?Reset@?$CCountedPointer@VCDataBank@NGameText@@@@...`, so CDataBank is right and the binder's file label
  "CIPhysicsFacesSegmentBase const" is wrong). 4 callers bind stand-in spellings that still name the same T: 0x00413520 <- 0x00687F80
  `CCountedPointer_CConsole::Reset(CConsole*)`, 0x005669C0 <- 0x0055FC90 `CTexturePointer::Reset(CTexture*)`, 0x00A31640 <- 0x00A2E860
  `CCountedPointer::Reset(CContainmentVolume*)`, 0x0067FC10 <- 0x0067F810 `CCountedPointer<struct CPhysicalPrimitiveBase>` (no namespace,
  `U` key; the PDB-typed 0x0067F9B0 binds the NHitLocations spelling of the deleter). 3 binders have no caller line (0x004A98C0,
  0x00A8D400, 0x00AD65A0); their targets have the real-typed referencers 0x00403D20 / 0x00A8B990 / 0x00ACA830 (above). No disagreement
  on T at any of the 60; the 4 stand-in caller spellings are listed in NAME_FIXES.tsv.
- `proof_*.txt`: per row, objsec HEAD v new object with the one rename `?Destroy_<addr>@@YIXPAX@Z=<PDB>`: every non-debug section
  (bytes + relocation list) equal (ALL_EQ, .drectve included); rename-aware cmpobj: every function COMDAT SAME (the row's leaf and the
  pick-any `??0SCountBlock@@QAE@PAX@Z`); no `??_7` / `??_G` in either object (no table emitted, vtcheck not applicable).
- `link_b1.txt`: link of the HEAD set (60 HEAD binders + 59 owner self rows compiled at HEAD) and the new set (60 new binders + the same
  owners), LINK cleared, /FORCE: HEAD leaves all 60 `?Destroy_<addr>@@YIXPAX@Z` unresolved (no object defines them); the new set resolves
  53 of the 60 PDB references against their owners and leaves 7 unresolved, exactly the self rows still on stand-ins below (6) plus
  0x004ADC40's owner, which was not compiled. Duplicates: the same 4 pre-existing LNK4006 in both sets (DefineStatBarDestructor
  0x00444D90 / 0x00444DD0; `?DeleteData@@YIXPAUAutoTinyVirtualObject@@@Z` in 0x0067F650 / 0x00A110D0 / 0x00A99FE0 / 0x00A9CB20), none new.
- recheck_final.log: vland --recheck-landed over the 60 changed rows (two runs of 30, rf_f1 + rf_f2, concatenated; split as a precaution so each run fits its `timeout 900`, not measured) (their landed tests, unchanged: standalone models that do not link
  the row and do not name the stand-in): 60 RELOCATION_MATCH PASS, 0 DIFFER / RELOC_CONFLICT. Non-blocking warnings: STANDIN_LEAF on
  the binders' own stand-in leaves (now visible through the pinned Leaf lines) and RELOC_NAME on 6 rows whose callee's CATALOG label is
  a stand-in (0x0041BD80 / 0x0067F650 / 0x00A110D0 / 0x00A8CB70 / 0x00A99FE0 / 0x00A9CB20; NAME_FIXES.tsv carries the PDB names).
- Locks: the 60 binder rows stay NF282 (rows.txt); the 59 owner rows compiled only for the link check were set back to `released`.
- Census of the stand-in tokens (`Destroy_<addr>`) over rebuild/src, rebuild/tests, rebuild/include: after the change no CODE token is
  left for the 60 (comments in two already-ported rows 0x00848DB0 / 0x008ED950 mention `Destroy_007180b0`, not one of the 60).
- Name census (`ccp_name_census.tsv`): for each of the 60, every reloc_bindings line of the PDB spelling (column 1) and of the stand-in
  `?Destroy_<addr>@@YIXPAX@Z` targets the entry's own address (60/60 OK), and a `// Leaf:` line naming the PDB spelling exists only in
  the owner row at that address (54 rows) or nowhere (6: the self rows still on stand-ins below). No object newly DEFINES any symbol
  (objsec / DEF lists identical); the only new external reference per object is the PDB DeleteData spelling. New symbol names: none.
- Staleness: orig/<b>.cpp equals the repo source resolved by address in build_candidates.ps1 now (60/60, cmp); the 60 binder locks
  still read NF282.

## Self rows still on stand-ins (T3 follow-ups, not changed here)
The PDB spelling the binders now reference is not defined by these self rows' objects (it was not defined at HEAD either: HEAD's
`Destroy_<addr>` reference was unresolved everywhere, so the link is never worse):
- 0x0041BD80 `extern "C" OnDie(CTexture*)` (catalog `_global::DeleteObject_0041bd80`): DeleteData<CTexture> (`Uninitialise(); delete`).
- 0x00A8CB70 `CClass_00a8cb70::FreeSelf_00a8cb70` (no Leaf line): DeleteData<C3DBone2> (`push ecx; call delete; pop ecx; ret`).
- Plain `DeleteData(AutoTinyVirtualObject*)` stand-ins: 0x0067F650, 0x00A110D0, 0x00A99FE0, 0x00A9CB20 (Leaf `DeleteData`,
  the tracker counts it as agreeing; the object defines `?DeleteData@@YIXPAUAutoTinyVirtualObject@@@Z`).
- 0x004ADC40's owner row fails the clang precheck (`field has incomplete type 'CMessageEvent'`; VC7.1 accepts it), so it was not compiled
  and its lock released; its Leaf is the PDB spelling.
- The 60 binders' own leaves are stand-ins (`CCountedPointer_<x>`), and several binders' file / catalog labels disagree with T (e.g.
  0x00403E40 "CIPhysicsFacesSegmentBase const" stores the NGameText::CDataBank deleter 0x00403650; the self row 0x00403650's FAM10
  identity note and FableWin give CDataBank). Their port onto `template void CCountedPointer<T>::Reset(T*)` (the NF221 / NF233 form) is
  the T3 unit that also fixes those labels.

## Pre-existing debt seen (not changed)
- Every generated binder object defines the pick-any `??0SCountBlock@@QAE@PAX@Z` with a per-row deleter relocation (one name, ~100
  bodies). Unchanged by this pass (the body only changes its relocation's symbol name); it goes away with the T3 port above.

## Open (not started, APPLIED `open`)
- The other 31 simple / namespaced single-binder CCountedPointer DeleteData entries (work/sel_all.tsv lines 61-91): same transformer,
  next hand-back.
- Deferred CCountedPointer entries (work/deferred.tsv), each needing more than the transformer:
  - nested T: 0x007180B0 (CTCCreatureModeManager::CModeBase; binder 0x00718A00 + test 0x0073A8E0), 0x007222B0
    (CTCAppearanceMorphBase::CMeshMorphSet), 0x00B230E0 (NParticleEngine::CPSCSplineBase::CSplinePosParamData): the real outer header
    has to come in (bytes to re-prove); a fake `namespace Outer` would mangle the same but is a made-up model.
  - template T: 0x006174F0 (std::vector<unsigned __int64>: stlport header), 0x00AC3A10 (CArray<C3DTriangle2>: engine/CArray.h).
  - multi-binder: 0x0056C0F0 (6 binders: 0x0056C880, 0x0069C2F0 and four whose sources contain `template` -- CPlayer 0x00636E90,
    CCombatSequenceWillLightning 0x0069E3D0, CGameCameraManager 0x0069E460 / 0x0069E5D0; their models were not inspected), 0x0085A5A0 (2 binders).
  - not DeleteData: 0x004850F9 / 0x0048AFB0 (ReleaseData stand-ins), 0x005ED2C0 / 0x0065D5A0 / 0x006E46F0 (destructor stand-ins).
- All non-CCountedPointer classes of ENTRIES (316 entries).

## Decisions needed
1. Accept the hybrid form for these binder-only respells: a header-less local stand-in class (kept, T3 later) whose member calls/stores
   the shared header's `CCountedPointer<T>::DeleteData` (DECL_ONLY). It is the in-place form the brief describes; the alternative (full
   port of each binder onto the header's Reset) renames 60 self leaves and is T3.
2. NAME_FIXES.tsv lines for the 60 BINDER self names (`?Reset@?$CCountedPointer@<T>@@@@QAEXPAV<T>@Z` / the (T*) ctor; one FableWin
   name each, T tied by the deleter store and the callers above) and for the 6 owner self rows are T3 ports (self renames onto
   engine/CCountedPointer.h), NOT respells to apply at landing; their own callers were not censused. Queue them as such.
3. The `// Leaf:` lines added to the 60 binders pin their HEAD stand-in leaves; the tracker will then measure those addresses (some may
   show a self-vs-callers split that already exists in the objects). OK to keep, or drop them at landing?

## Rule notes
- No header changed (hdr/ and orig/ hold no header; orig/ holds the 60 HEAD sources). No `__asm` / `naked` / `_emit` / `volatile`.
- Every grade ran with `timeout 900` on its own line (hk.sh / nc.sh -> sc.sh), launched in the background and checked with kill -0;
  every compiled source (60 HEAD binders, 60 new binders, 59 owners) had a passing clang precheck in an earlier call (work/pre.log).
  Slip to disclose: the 60 landed behaviour tests that the final vland recheck compiled (unchanged, standalone) had NO clang precheck.
- Inherited tool fixes: proof.sh now uses tools/cmpobjm.py (cmpobj with the rename map applied); nc.sh added (new compile only).
