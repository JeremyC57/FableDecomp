# Chunk Q9 notes (40 parked near misses)

All grades were run with `FABLE_HDR_OVERLAY=$A/hdr` (tools/gg.sh exports it); the overlay is documented in `hdr/README.md`.
Landing these rows needs the overlay headers promoted (or copied into the landing overlay) with the src/ files.

## Result
- 4 matched (src/ + test/): 0x009F3010 MATCH, 0x006AB430 / 0x00E1CAE0 / 0x0092B900 RELOCATION_MATCH (only float-literal and
  string-literal relocations are unbound; every callee relocation agrees with rebuild/oracles/reloc_bindings.tsv, checked with
  tools/rn.py).
- 36 parked in differ/, each with a line-2 `// Grade: DIFFER(ours v retail) -- ...` measured this pass. Rows not worked beyond the
  as-is regrade say so on that line and keep the prior draft's residue text (still accurate: the same byte counts).

## Identities proven (LABELS.tsv; both Ego_r.pdb and FableWin.pdb name sets)
- 0x009F3010 CSurface::ReadOnlyLock, 0x006AB430 CThingPlayerCreature::GetMovementBandTypeFromJoytickMagnitude ("Joytick" sic),
  0x00E1CAE0 NScript::CQ_WizardBattleScript::MonitorMazePhases, 0x0092B900 CAIStateGroup_OnHeroPush::Update,
  0x0092ACE0 CAIStateGroup_ApprenticeHero::GoToLibrary, 0x00F35C60 NFractal::CNoise::PerlinNoise, 0x009F2F90 CSurface::GetDimensions.

## Callee names
- NAME_FIXES.tsv: 0x00F35E00 InitPerlinNoise is NFractal::CNoise's private member (`AAEXXZ`); 0x009F2F90 / 0x009F33E0 belong to
  CSurface, not the landed `CSurface_Methods` stand-in.
- 0x00487DC0 is CPlayer::GetPControlledCreature (OpenChest 0x00896390 / OnHeroPush 0x008328D0); the prior 0x0092B900 draft called it
  GetPCharacter (draft only, never landed).
- 0x00929FC0 is CAIStateGroup_ApprenticeHero::GetObjectWithScriptName (header opt-in _SCRIPTNAME); the prior 0x0092ACE0 draft
  called it FindDummy (draft only).
- 0x0138CBA8 (.data float 0.01f) has two landed descriptive names in reloc_bindings: `g_LookAtTurnSpeed` (bound from
  0x0092AE70) and `g_LookAtTurnSpeed` (from 0x0092AFE0 LookAtMap). One address, two names: the coordinator should pick one. No
  original name found (not in either PDB name set); 0x0092ACE0's differ uses g_LookAtTurnSpeed.
- 0x00693EF0 is ~CCreatureActionBase; 0x0092B900 now calls it directly through the implicit PlayAnimation destructor.

## Header debt
- The landed CThingAICreature::OnHeroPush 0x008328D0 still models CTCOnHeroPush locally: hdr/engine/CTCOnHeroPush.h can replace it.
- The landed PhaseTwo 0x00E1C580 / PhaseOne 0x00E1CFD0 / WatchForLeaving 0x00E1C4D0 / CutsceneScreamers 0x00E1CD40 keep local
  models of CQ_WizardBattleScript: hdr/engine/CQ_WizardBattleScript.h can replace them.
- Many parked drafts are still local models with stand-in classes (CInterfaceMap, CItemVector, CThingPtrVector, CTCMap, ...):
  0x005BFD5E, 0x0083C2D0, 0x006F2C00, 0x0097BB30, 0x007C81F0, 0x0057B43F, 0x0093F3C0, 0x006AC200, 0x006AB0E0, 0x00AAC5E0,
  0x00701590, 0x009363D0, 0x00C2DA20 and others. Three of this chunk's four matches came from migrating such a draft onto the
  shared headers and then fixing one or two body spellings, so migration is the next step for each of them.

## What each closest unmatched row needs next
- 0x0092ACE0 GoToLibrary (389v389, 2 hunks, shared headers): only the LookAtPos argument setup -- the turn-speed load after the
  first fld and the by-value look target built through eax after `mov ecx,esi`. Try the look target as a named C3DVector argument
  temporary built by the header ctor, or c2prio on the eax/ecx tie.
- 0x0047042A TransferBinaryIn (716v716): one SIB operand order (`[eax+ecx+4]`).
- 0x005A6769 ConstructNormalShopListPC (1336v1336): the wanted-total sum operand order ([1F0],[1F4],[1EC]).
- 0x004FEB70 HeightAreaChanged (260v261): constant 0 in ebx with a loop-1 split; c2prio says the loop-2 iterator outranks it.
- 0x005BFD5E OnSerialise (424v425): esi zero / ebx this colouring on a local model; migrate first.
- 0x007C81F0 OnCollision (768v768): local model; the 0x0092B900 levers (grouped Dot, reference local) look applicable.
- 0x004B01B0 (283v277), 0x009B9D70 (127v132), 0x00F35C60 (312v316): probed this pass without convergence (see differ/ line 2).
- 0x006AC200 (404v398), 0x006AB0E0 (538v529): CThingPlayerCreature rows; migrate with the 0x006AB430 defines block first.
