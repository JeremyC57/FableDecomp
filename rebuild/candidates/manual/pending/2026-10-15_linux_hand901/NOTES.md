# Chunk 901 -- the skipped-row ledger pieces (34 bodies from 15 multi-function catalogue rows)

Hand-authored by the coordinating session (Fable 5.1). Every row of the ledger kind `multi` that `split_rows.py` cut into
pieces of <= 600 bytes: thunks, bit accessors, forwarders and three STLport `<cmath>` float overloads. 34 of 34 matched
(7 needed a second form: three size-optimised TUs, the counted-pointer accessor for operator==/!=, a named float local
for std::pow, and the stale 82-byte chunk-59 oracle row for 0073a840 removed so the 49-byte piece grades on its own).

## Identities (vtables from rebuild/manifest/rtti-vtables.tsv, names from landed siblings)
- 00455fa0 / 00455fa8 / 00455fb4: CPersistContext_TransferableSubComponent vtable 0x012338FC slots 0, 1, 2 (forward to the
  owner's slot 18; read / write bit 2 of the owner's +0x24 flag byte).
- 00581ab3 / 00581ae1: CTCInventory slots 59 / 60 = ProcessButtonBReleased / ProcessButtonXReleased (chunk 91 named the
  slot 58 / 61 siblings ProcessButtonAReleased / ProcessButtonYReleased). The +0x1cc / +0x1cd bools are modelled locally:
  engine/CTCInventory.h carries the PDB's 16-byte vectors, retail's are 12 bytes.
- 007312f0 / 00731300: CTCDoor slots 10 / 23 (slot 10 tail-calls slot 9 = the landed CTCDoor::FrameUpdate; slot 23 is
  constant true). 00750960 / 00750970: the identical CTCChest pair (slot 9 there is 0x00750780, unlanded).
- 0073a840 (17 callers) + 0073a880 (CTCCreatureModeManager slot 9): both walk the STLport list<CCountedPointer<CModeBase>>
  at +0x0c (PDB PModes). The manifest label CNavQuadTreeNode::RemoveNeighbours on the row was propagated.
- 0088b7e0 GetCurrentScriptPriority + 0088b800 (+0x3c getter), 008aae20 CScriptThing::operator== + 008aae40 operator!= +
  008aae60 / 008aae70 (+0x7c / +0x38 getters): operator==/!= really are CScriptThing (engine header used, the counted
  pointer's implementation compared through its slot 78); the four getters belong to a larger object in the same TU
  (the 0xc-byte CScriptThing has no +0x38), so they are modelled locally under the propagated class name.
- 00991620 (bool setter notifying slot 2) + 00991640 (vector * float at +0x10): the manifest's CTCDParticleEmitter is
  propagated (its PDB bools are bitfields at +0x44); modelled locally.
- 009cbd30 / 009cbd50 / 009cbd70: three forwarders to the +0xec object's slots 1 / 0 / 2 (the AskHeroQuestion-style
  row); engine/CScriptGameResourceObjectScriptedThingBase.h is 0x10 bytes, so the owner is modelled locally.
- 00aacf90 / 00aacfa0: CSoundReceiverLH slots 2 / 4 (forward the position to slot 1; return the +0x04 position).
- 00c3eb60 / 00c3eb80: audio-library forwarders to the +0x1d4 device's slots 8 / 9 returning -9 when unset (00c3eb60 is
  reached by a tail jump from 0x00C52999 with ecx = the global at 0x0143BB5C). 00c40e20: forward to the +0x7c object's
  slot 6. The CScriptThing labels are propagated.
- 00c40e30 / 00c40e40 / 00c40e50: std::atan2(float,float), std::log10(float), std::pow(float,float) -- the STLport
  <cmath> float overloads emitted out of line (address taken), compiled through the VC7.1 maths intrinsics.
- 00ca5510: a __cdecl block-header thunk (owner two words before the block, its operation table's third entry) stored
  in the non-RTTI function table 0x012BF3C8; 00ca5520 / 00ca5530 / 00ca5540: a library object's byte getter (+0xa1),
  sub-object address (+0xa8) and `Sleep(interval / 10000)` (KERNEL32!Sleep, IAT 0x0143FE1C). Labels propagated.

## Label corrections
- 0073a840 is a CTCCreatureModeManager member, not CNavQuadTreeNode::RemoveNeighbours.
- 00991620 is not EditFrameUpdate (a bool setter with a slot-2 notification).
- 00581ab3 / 00581ae1 are ProcessButtonBReleased / ProcessButtonXReleased, not vfunc_59 / vfunc_60.

## Review at landing
- 20 of the 34 bodies keep the propagated class in their name (func_<addr>) because the real owner is not evidenced.
- 0073a840's stale 82-byte row was removed from $S/batch/chunk59_oracle.tsv (the row was skipped there).
