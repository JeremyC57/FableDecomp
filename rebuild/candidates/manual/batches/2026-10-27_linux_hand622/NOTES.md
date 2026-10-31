# Hand chunk 622 (CAIStateGroup_ServeFood .. global): 2 of 10 byte-exact

Chunk 622 is a 10-row retry of rows that were already parked. Each row started from its best earlier attempt (paths in CHUNK_BRIEF.md).

## What matched (2)
- **CAIStateGroup_ServeFood::IsRunnable 0x00923070** (RELOCATION_MATCH 1184v1184): a female, creature-type-2 worker whose building
  owns tavern tables. When IsTime() is true it serves a free table (mode 1), gives up if she is already one of a table's Occupants,
  frees her eating seat if she has one, then tries a mode 1 serve again. Otherwise it serves in mode 2. On success it stores the table.
  Shape lesson: the GetServingPoint out-pointer (`CThingObject* food;`) is declared inside the block of each call. VC7.1 then puts it
  in the frame word above the 0xB8 lookup key (retail esp+0x14 key, esp+0x20 out). Declared at function scope, the two words swap.
  This 22-line residue had survived every earlier attempt. The same-TU `CInterfaceMap::LowerBound` stand-in was replaced by the
  landed retail body: the real `CVectorMap<ETCInterfaceType,CTCBase*>::LowerBound` template, as a `__declspec(noinline)` member.
  It still matches, so the row now binds the canonical 0x0040F020 spelling. Every callee is respelled to its PDB signature:
  GetSex, GetCreatureType, IS_THING_ALIVE<CIntelligentPointer<CThing>>, CBaseIntelligentPointer::GetPItem const, and
  CTCTavernTable::GetServingPoint / GetEatingPoint / FreeSeat (objdump confirms the mangled names). Every member uses its PDB name:
  Table, Seat, ServePos, ServeRHSet, ServeActionPoint, Mode, BeingServed, Occupants, ObjectList, Dead, InLimbo.
- **CThing::IsEntitySerialisable 0x004C7870** (MATCH 15v15; catalog "CThing::vfunc_22"): returns false when the thing has no
  driver, else the driver's IsComponentSerialisable(). Name proof: slot 22 of the CThing vtable. The landed
  CThingBuilding::IsEntitySerialisable 0x0082D0C0 shows that the eleven classes overriding slot 22 are exactly the PDB's
  IsEntitySerialisable set. The callee at driver slot 23 is IsComponentSerialisable, in the PDB for CTCDCameraPoint,
  CTCDNavigationSeed, CTCDRegionEntrance, CTCDRegionExit and CTCDParticleEmitter, whose bodies match. Shape lesson:
  `bool result = false; if (driver) result = driver->IsComponentSerialisable(); return result;` puts `xor al,al` before the test
  and keeps the tail jump. The ternary and if/return forms put it after the jump.

## Parked for Fable 5.1 (8)
- 008de7a0 NCreatureMode::CModeIceTrollGroundSmashIcicleAssault::FrameUpdate DIFFER(1239v1239): trap spawn: retail does `movsx eax,[ParentThing+90h]` before the PDef load, and the trap-lookup key push and the SetThingAsImmune argument use ecx/edx where ours use eax/ecx; fmul base eax v edx | tried: same-TU noinline LowerBound 0x0040F020 body (no change); earlier 8 levers (header) | next: lane gap: register rotation in the trap-spawn block (eax/ecx/edx shifted by one)
- 006c2170 CObject::Load DIFFER(1345v1329): loop 1 keeps the element offset in memory and enters through a jmp over a 3-byte alignment pad; retail has the offset in ebp, the index in [esp+10h] and a bottom-tested loop | tried: `CLevelFileEntry& entry = LevelFiles[i]` with `level` read before the test (worse, 377 v 283 lines); alt 006c2170.alt.cpp upstream (1329v1329/166) | next: in loops 2/3, re-index with a cached begin pointer local; check whether VC7.1 aligns the loop head only when it is entered by a jmp
- 00927f30 CAIStateGroup_SetupWares::Update DIFFER(1331v1331): the per-place retry count lives in eax (retail ecx) and `lea esi,[edi+edi*8]` comes after `push 1.4f` (retail before) | tried: block-scoped `bool reached` (fixed the reached slot, 44->38 lines), reached declared before tries (no change), `i != 0 ? 3 : 5` argument (1321, worse) | next: respell to the real CVectorMap LowerBound (00923070 form), then a named element pointer limited to the MoveToPos block
- 00590235 CTCInventory::GetItemsListUIComponent DIFFER(1393v1393): pure callee-saved permutation (retail this edi / zero esi / index ebx; ours esi / ebx / edi) | tried: /O1 /Oy- flags instead of the optimize("y",off)+("s",on) pragmas (identical) | next: lane gap: callee-saved permutation
- 007b3120 CTCGuard::FrameUpdate DIFFER(1592v1592): frame now retail's 0x1c; left: retail keeps village in [esp+14h] and reloads it at each use in the apology loop and keeps the guard in ebx after the loop, ours caches village in esi/ebx and the guard in edi | tried: same-TU noinline LowerBound body (frame 0x24->0x20), scoreboard as an out-parameter GetInterface (frame 0x1c), village lookup as an out-parameter (no change) | next: reshape the apology loop (village read through an inline accessor per use) to lower village's register weight
- 0046313e CSimVoice::operator= DIFFER(43v43): the Voices.Dirty copy after the vector operator= call: retail `mov al ; mov [ebx+0Dh],al` before `pop edi`, ours dl sunk below `pop esi` | tried: same-TU noinline vector operator= body, fully implicit assignments emitted through a CSimVoicesDef::Copy user | next: lane gap: post-call byte-copy scheduling
- 0045cfad CSoundMap::operator= DIFFER(49v49): the same post-call Dirty-byte scheduling as 0x0046313E (cl after the pops; retail al before) | tried: (shape shared with 0x0046313E; earlier /G5 /G7, implicit/explicit/void inner operator=) | next: lane gap: post-call byte-copy scheduling
- 007af940 std::__uninitialized_fill_n<CIntelligentPointer<CThingAICreature>*,unsigned int,CIntelligentPointer<CThingAICreature> > DIFFER(62v62): x in ebx and n in edi (retail edi/ebx) | tried: loop on __first, a separate count local, a `const _Tp& __val` alias | next: lane gap: callee-saved register tie

## Skipped
- none

## Label corrections
- 0x004C7870 catalog "CThing::vfunc_22" -> `?IsEntitySerialisable@CThing@@UBE_NXZ` (vtable slot 22 + PDB class set, see above).
- 0x00923070 cutter "CAIStateGroup_ServeFood::CAIStateGroup_ServeFood::IsRunnable" -> `?IsRunnable@CAIStateGroup_ServeFood@@MAE_NXZ` (protected virtual).
- 0x0045CFAD catalog "CSoundMap::CSoundMap" is CSoundMap::operator= (earlier finding, kept).
- 0x007AF940 catalog "_Fill_n<unsigned long*,unsigned int,unsigned long>" is the bound
  `??$__uninitialized_fill_n@PAV?$CIntelligentPointer@VCThingAICreature@@@@IV1@@std@@YIPAV?$CIntelligentPointer@VCThingAICreature@@@@PAV1@IABV1@ABU__false_type@0@@Z`
  (reloc_bindings.tsv). Ego_r is Dinkumware, so the PDB has no STLport spelling.
- 0x00661FF0 manifest "CountReferences" is CThingCreatureBase::GetSex() const; 0x00662000 "CountReferences" is
  CThingCreatureBase::GetCreatureType() const (NAME_FIXES.tsv).
- The ServeFood virtuals at slots 14/15 are IsTime() and GetBuilding() (PDB, AtTavern/AtHome overrides). The earlier attempt named
  them IsOnDuty and GetPWorkplace. Interface 0x21 is CTCThingOwner, not a "CTCTavern" table list.

## Names
- Proven: every name in src/00923070.cpp and src/004c7870.cpp, except the ECreatureType enumerator `CREATURE_TYPE_2`
  (descriptive; the value 2 is retail's).
- Parked rows keep the spellings of their earlier attempts. Several still use the `CInterfaceMap::LowerBound` stand-in name, which
  the landing must respell to the canonical 0x0040F020 symbol (NAME_FIXES.tsv).

## Header debt
- engine/CAIStateGroupBase.h and engine/CTCBase.h flatten the vptr (`void* __vftable`), so the virtual calls need local models.
  engine/CAIBrain.h's Creature +0x18 agrees with retail. No header exists for CThing, CTCThingOwner or CTCTavernTable
  (retail CTCTavernTable: BeingServed +0x0c, Occupants +0x10, the same as Ego_r).

## vland (read-only run on payload.json)
- 004c7870 MATCH PASS; 00923070 RELOCATION_MATCH PASS. Warnings: SHARED_HEADER_AVAILABLE CAIStateGroupBase (that header
  flattens the vptr, see Header debt); RELOC_NAME 0x00662000 is labelled 'CThingCreatureBase::GetMovementMode' in the catalog,
  while the source uses the PDB GetCreatureType (NAME_FIXES.tsv). No RELOC_IDENTITY.
