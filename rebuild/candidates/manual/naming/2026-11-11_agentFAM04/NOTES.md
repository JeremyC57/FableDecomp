# agentFAM04 -- respell of FAM03's proven virtual names (2026-11-11)

Family: 445 landed rows whose retail virtual names agentFAM03 proved (TLC vtable slot + Xbox 360 Test PDB virtual order + Ego_r /
FableWin name set + `ret N`). This pass respelled **279** of them to the proven names; every one grades MATCH (217) or
RELOCATION_MATCH (62) with the leaf symbol present in the object (graded with `FABLE_HDR_OVERLAY=$S/w22/agentFAM04/hdr`), and the
behaviour test of each was run (VC7.1 /Od + g++ -std=c++11 on x86-64): 275 pass both, 4 keep a pre-existing g++ failure (below).
166 rows are in LEFT.tsv. Max grades on any row: 3 (budget 10).

## Verification of the claims (work/verify.py -> work/verify.tsv)
Every one of the 445 claims was re-checked (slot holds the body, name in the Ego_r / FableWin set, `ret N` fits the signature):
443 hold. 0x00AF1200 `??1CParticleDataBank` fails (`ret 18h` cannot be a destructor) -> LEFT, name not applied. 0x00442760 holds
(slot 1 ClearPrimitivesIfNecessary); its symbol is also bound by landed callers of 0x00644627 (a deleting destructor) -- FAM03's
NAME_FIXES already carries that; 0x00442760 itself is left unrespelled (time).

## Renamed, by shape (table.tsv shape of the retail body)
OTHER 119, JMP 37, GETADDR 28, CONST 26, GETM 18, GETF 18, EMPTY 13, TWO 9, FCONSTG 6, FWDV 5.
By class family: NUISystem 74, CTCPhysics* 52, CThing family 50, NCameraModes 34, NCreatureMode + CTCCreatureModeManager 33, other CTC
components 31, NGraphicAppearance 3, NHitLocations 1, NPlayerGui 1.

## How the rows were written
- Shared headers first; where the header lacked the virtual, an agentFAM04 `#ifdef` opt-in declares it (hdr/README.md lists every
  macro and the rows that define it). FAM03's `FABLE_RETAIL_<HEADER>_VSLOTS` blocks were reused unchanged; new declarations went
  in separate `..._VSLOTS2` blocks so FAM03's text stays intact.
- Classes with no shared header got a minimal local definition on the RTTI base (ghidra_out/rtti_bases.tsv), with Ego_r member
  names at the retail offsets (opaque `unsigned char Name[N]` with the real type in a comment where no header exists for the
  member type). No address-suffixed classes, no invented namespaces.
- Retail offset shifts used (all re-derived from the retail bodies): NCameraModes::CBase is 0xa4 in retail (Ego_r 0xc8, derived
  members 0x24 lower); def classes 4 lower (packed 0x39-byte CParentDefClassBase); NUISystem components 0x30 lower; dragon modes
  4 lower (CCardinalSpline holds a 12-byte STLport vector); CModeHoverBase 0x40 (Ego_r 0x4c).
- Tail calls and forwards name the real callee with its PDB access (e.g. `?FinalCleanup@CTCMultiArrow@@IAEXXZ` protected,
  `?InitClosed@CTCChest@@AAEXXZ` private); every callee spelling was checked against the PDB name sets (work/pdbq.py).
- Virtual calls inside a body (FWDV shapes, `DrawIsInDrawList`, `CTCCreatureModeManager::FrameUpdate`) were only written where the
  header's vtable model is complete (CThing ORIGINAL_VTABLE, CTCBase, CModeBase VIRTUALS), so the slot offset is the retail one.

## Tests
- 0x0041D1C8, 0x004C7460: rows/<addr>_test.cpp = the unchanged catalog test plus a portable prelude (`#ifndef _MSC_VER` defines
  `__fastcall` / `__declspec(x)` etc. away) so g++ can build it; PassPattern unchanged.
- 0x0052E890: the catalog test reached the body through inline assembly (forbidden in tests); rows/0052e890_test.cpp is a self-contained
  replacement with the same PassPattern `OK_0x0052e890`.
- 0x00815070, 0x008DD6E0, 0x008DF9C0, 0x008E8830: VC7.1 PASS; g++ fails as before -- the catalog tests are 32-bit source snapshots
  (rebuild_abi.h's ptr32 assert) and this host has no 32-bit g++ runtime. The row change is comment-only (as-is respell).

## Name decisions beyond the family (NAME_FIXES.tsv)
- 0x0137F9B4 = `CTCPhysicsBase::DummyRHSet` (FableWin "dynamic initializer for 'CTCPhysicsBase::DummyRHSet'" + the GetRHSet body);
  landed `g_DefaultRHSet` is made up. Declared `protected: static` in the opt-in: the access letter is an assumption.
- 0x006AFE00 = `CTCPhysicsBase::GetMass() const` (Ego_r); the landed binders `GetAirResistance_006afe00` / `CTCPhysicsBase_Calls` are wrong.
- 0x007832B0 = `CTCBoastingCrowdControl::Shutdown` (Ego_r + catalog); landed `KillAllCrowdThings` is in no PDB.
- 0x0072E290 / 0x007502D0: the PDB spellings are private (`AAE`); landed binders use public `QAE`.
- 0x00686870, 0x00861D00, 0x00663BA0: several made-up landed spellings of CTCBase::OnInitialActivate, CModeStandBase::OnDeactivate,
  CThingCreatureBase::GetMovementSpeedFromCreatureMode.
- 0x01382438: the file-static CAnimFilterNoGroup returned by CModeBase::GetAnimFilter (0x00739E50). Its name is in no PDB; the row
  uses the landed spelling `s_NoGroupAnimFilter_01382438` (declared extern; retail defines it file-static in the same TU, whose
  atexit pair is landed at 0x0121ED80 / 0x01229A10). Descriptive name, kept consistent with the landed definition.

## Descriptive names
None introduced: every function, member, class and callee spelling is from Ego_r.pdb / FableWin.pdb / the landed headers.
