# Chunk 615: notes

Result: 4 of 10 rows match (RELOCATION_MATCH, tests PASS under g++ and VC7.1): 0x0088AE40, 0x008CAD50, 0x00849BE0
and 0x00867010. The 6 parked rows are in differ/ with their residue.

## Winning lever: respell with the PDB names
All four wins came from the same move. Replace the stand-in model with the Ego_r spellings: inline accessors,
GetTC<T>, reference returns, protected virtual bases, nested classes and namespaces. Then the register colouring
falls into place. The wrong stand-in shapes were not only cosmetic: they changed the IR.
- 0x00867010: SetSpin(m_spin) was really CThingPhysical::ModifyHealth(-GetMaxHealth(), true); MaxHealth is at retail +0xB0.
  The knocked-down mode is NCreatureMode::CModeKnockedDown(CThing&, const CCharString&) over
  CTCCreatureModeManager::CModeBase(CThing&), with a CAnimFilterOfGroup member. The group string is the
  CAnimComponentFollowingOnAnimFilter's NextFilter (CDefString::GetString). The lookup is the 0x00867F60 GetComponent<...>.
  The esi/ebx swap then went away. The frame dropped from 0xc to 8 once the component pointer sat in a block that copies
  the CDefString out.
- 0x00A24D70 (still parked, 39 -> 7 lines): the parked file's PeekData(handle, device) was the wrong call. The two pushes
  are the loader's (length, pool) arguments around a zero-argument GetData(). RecoverAnimationMemory, ReleaseData and
  CResourceBank::AddResource are named from the Ego_r name set and the donor order.

## Parked rows
- 00a24d70 414v414, 7 lines: AnimInfo->Bank / ->Index load order before GetEntryDataType. This is the same residue as
  the parked file had. Tried GetDataType(), GetBank/GetIndex, the CMeshDataBank::GetAnimationType inline (worse: a jump
  table and the index first), a const info local, a long type and an inline switch.
- 00a11f20 434v428: with the proven callee it is 6 bytes long. The residue is the x87 affine-inverse schedule (lane gap,
  GOTCHAS chunk 293). The parked 428v428 file only reached equal length with a guessed callee shape.
- 008cdd40 430v430, frame 0xd0 v 0xd4: a frame-packing difference (camera pointer, haste pair, insert result).
- 00680050 439v439, 5 lines (parked best). It also carries a stand-in GetSubDef body.
- 006a9690 450v450, 5 lines: respelled with the PDB names. The residue is the lea/push order at the RotateXY call.
- 00b17410 459v459: the position of the descriptor destructor.

## Header debt
- engine/CThingPhysical.h, CCreatureActionBase.h and CMeshDataBank.h are padding-only, with the vptr modelled as a
  data member. Every row models the virtual chain locally with UNPROVEN_SLOT fillers.
- There is no engine/CThing.h, CCombatActionBase.h, CTCCreatureModeManager.h, C3DVector.h or CRightHandedSet.h.
- CCountedPointer: 00a24d70 needs Reset(T*) out of line (a retail ICF body at 0x00A29C50), but
  engine/CCountedPointer.h defines it inline, so that file models CCountedPointer locally. 00867010 uses the engine
  header.
- CVectorMap<ETCInterfaceType, CTCBase*>::LowerBound uses the landed STLport pointer form. The Ego_r spelling returns a
  Dinkumware _Vector_iterator.

## For the coordinator at landing
- NAME_FIXES.tsv has 31 rows. The ones that touch landed code:
  - 0x00740820 is ??0CModeKnockedDown@NCreatureMode@@QAE@AAVCThing@@@Z (namespace NCreatureMode, CThing&). Its base
    constructor 0x0073EAC0 is ??0CModeBase@CTCCreatureModeManager@@QAE@AAVCThing@@@Z.
  - 0x007239C0 is SetRHSet, not SetFacingAnglesFromRHSet.
  - 0x00A229E0 takes const C3DAngles&.
  - GetPItem returns CBaseObject*.
- 008cad50 uses a one-argument inline CRightHandedSet(const C3DVector& forward) that is in neither PDB name set. The
  PDB two-argument (up, forward) constructor grows the frame (465v441). Never emitted.
- Globals g_pThingManager 0x013B8A1C and g_DefStringTable 0x013CA828 are descriptive, following the landed convention
  (there is no PDB global name).
- Enumerator names (ETCInterfaceType values, EMovementType, ENavigatorReturn, EAnimationType values) are descriptive.
- A directory literally named `D:\Documents\FableTLC` exists at the repo root (dated 2026-10-21, not from this chunk).
  It was created by verify_and_land being run without wine/env.sh. A copy that tall.py created inside this agent's
  test/ was removed.
