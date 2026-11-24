# Landing nf314 (Account A, 2026-11-23): NF309's continuation (25 rows, 1 header opt-in, 26 table lines)

Hand-back: `pending/2026-11-23_nf314_handback/`; helpers under `naming_batch/agent_tools/nf314/`. All 88 entries reached: 7 done, 14
done-partial, 28 skipped-port (18 by census), 39 skipped-held (14 the CPersistTraits family unit), 4 rejected / reverted after measuring.

- Header (NF309 Q2): `engine/C3DPrimitive2.h` gains the `#ifdef FABLE_RETAIL_CDEVICEINDEPENDENT_ADDTRIANGLELIST_PRIVATE` branch (a global
  forward declaration of C3DPrimitive2, `friend class ::C3DPrimitive2;`, private AddTriangleList; `#else` keeps the old line); binder
  0x00A91640 defines it. equiv_check over 1,443 includers: 0 differ; the planted control flags the 9 LOADER opt-in users; a no-friend control
  fails as a private-member error. Header equal to HEAD at landing.
- Respells (both PDB name sets + the donor caller): PushCurrentManualMode (0x00699090), private RemoveMessageEventsAdded (0x0093A080),
  Terminate (0x009574D0 / 0x00958000), ResizeWindowToBackBufferSize (0x009C1050), private const GetClosestDepthBufferFormat (0x009BFB30),
  OnNoBoastsRemain (0x0056A980), `FindSymbol(name, long&) const` (0x006F9640 / 0x00AE85B0), GetAnimation (0x008DBB30). Pointer -> reference
  / const: CGameJoystickManager::Get (0x00449E60), CTBaseSingleton<CEngineManager>::Get (0x004FF440 / 0x004FF8B0), GetAnim (0x005B37F7 /
  0x005CD767), GetAnimation (0x00750470), CParentDefClassBase::operator= (0x00477964), GetAnimFromName (0x00733690), GetCombatTypeDef
  (0x006EC4F0). `virtual` on local models (no new vtable / RTTI symbol): 0x007A7BF0, 0x0098ABF0 (member model, pad shrunk), 0x008EC260 (its
  own override onto the PDB virtual spelling, full Leaf). 0x00692B30 / 0x006655A0: CThing::GetWorld -> protected `PeekWorld() const`
  (0x00692B30 adds `friend class CCreatureActionBase;`, NF305 Q5).
- Proof: RELOCATION_MATCH; objsec ALL_EQ under exact rename pairs; link checks; every rewitness target's HEAD object compiled; every delete
  audited against all exe callers. Staleness: all orig/ copies and the header equal HEAD at landing (after nf312 / nf313 / nf316).
- Table: 15 delete, 3 replace, 8 rewitness. The vland WARN RELOC_CONFLICT on 0x004FF8B0 (`?RemoveMap@CEngineManager@@QAEXJ@Z` ->
  0x006403D0 there, 0x00640430 in landed 0x004FEDE0) predates this batch: HEAD 0x004FF8B0 already bound RemoveMap to 0x006403D0;
  0x00640430 is SetMapAsUnloaded (donor UnloadLevel, Ego_r order, catalog), queued in NAME_FIXES.
- Rulings: Q1 the DepopulateLevel respell (0x00DC0300, `evidence/held/`) lands in the next batch now that NF312 has landed. Q2 the stale
  FinishAction public-virtual witness waits for the self-symbol measurement pass. Q3 the CModeStandBase pointer-form ctor (self 0x00861E00
  + 23 binders) is one in-place NF304 Q6 unit (queued). Q4 the two vtable cases stay ports. Q5 the friend stands. Q6 the rewitnessed lines'
  follow-up binders go to the next pass. Q7 above.
- Queued: 28 T3 ports + 3 rejected-as-port + the 0x00861E00 unit; NAME_FIXES_QUEUE: 6 lines.

## Recheck
`logs/recheck.log` (landed tree, 25 rows + 0x004FEDE0): WINS 26, 0 DIFFER; one pre-existing WARN RELOC_CONFLICT (above).
