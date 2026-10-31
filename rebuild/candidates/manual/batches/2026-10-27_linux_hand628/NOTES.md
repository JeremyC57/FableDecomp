# Hand chunk 628 (CEnginePrimitive2DSprite .. CTCInventoryQuests): 3 of 10 byte-exact

Chunk 628 is the 10-row parked-retry chunk (every row had a same-length best attempt). Three now match (all
RELOCATION_MATCH, vland WINS 3, tests PASS under g++ and VC7.1); the seven parked rows each moved closer or got a sharper residue.

## What matched (3)
- **CTCWeapon::SetProjectileWeaponLoaded 0x005DAC50**: the ammo's CTCObjectAugmentations (interface 0xBF) copies the
  weapon thing's augmentation data, the loaded flag is stored, then a weapon whose def NeedsLoading (retail +0xD0, Ego_r +0xE0)
  and is not loaded stops aiming. Shape: a NAMED bool condition, `bool canAim = !needsLoading || ProjectileWeaponLoaded;
  if (canAim) {...} else {...}`, is the only spelling that lays the restart arm first. All direct `if` forms (|| or &&,
  nested, early return, if/else either way round) put the -1 arm first. `this` comes from engine/CTCWeapon.h through
  the `#define CTCWeapon CTCWeaponLayout` include.
- **CTCInventory::OnItemSelectedInHand 0x0058400A**: carried-action-use component (interface 0x0E, proven by the retail
  CTCCarriedActionUseBase vtable slot 21 `push 0Eh ; pop eax`) used in hand, else a creature owner sheathes into the inventory.
  Shape: the PDB REFERENCE spellings close the old owner-register residue (esi v ebx): virtual `OnItemSelectedInHand(CThing&, bool&)`,
  `CCreatureAction_SheatheItemToInventory(CThingCreatureBase&, CCreatureActionBase*, long)` called with `*creature`,
  `SetCurrentAction(const CCreatureActionBase&)`, `IsPerformingAnyAction() const`.
- **CLibpkObject1D8::NormaliseCopy 0x00C418F0** (descriptive, libpk): zero input -> (0,0,1) and 0, else copy, length,
  normalise the copy (landed CPKVector3_00c402e0::Normalise 0x00C402E0). Shape: the GOTCHAS double-local rule:
  `double length = v.GetLength(); float result = (float)length;` gives retail's `fsqrt ; fstp st(3) ; fstp st(0) ; fstp st(0)`
  and only then the spill to the dead pOut slot. This was parked as an "x87 spill lane gap".

Names: 0x005DAC50 / 0x0058400A are PDB-exact (both name sets). 0x00C418F0 is descriptive. libpk has no PDB/RTTI names. `this` is the
object whose dirty word sits at +0x1D8 (caller 0x00C422A4 ORs 0xD0000 into it), which is the landed CLibpkObject1D8 setters'
class. The vector is the landed CPKVector3_00c402e0 class (one class with its Normalise). The manifest's "Math_ComputeVectorMagnitude"
is propagated.

## Parked for Fable 5.1 (7)
- 00493ac2 CEnginePrimitive2DSprite::CEnginePrimitive2DSprite DIFFER(153v153): retail `and dword ptr [esi+40h],0 ; mov eax,[ebp+40h]`, ours loads [ebp+40h] before the `and` | tried: real-name CEnginePrimitive base + CEnginePrimitiveThemeChannelDesc member with inline ctor (closed the old `lea ecx,[esi+0Ch]` residue), trailing members as body assignments, Texture(0)/ClipBoxIsPos in the init list with Flags in the body | next: something that orders the Texture store before the Flags parameter load (an aliasing barrier, a record-typed neighbour), or lane gap: immediate-store scheduling
- 009ba650 CStringParser::ReadAsFloat DIFFER(154v154): integer arm copies through ecx (retail edx, store after `lea ecx`), float arm through edx (retail ecx) | tried: PDB inline CParsedItem::GetInteger()/GetFloat() (closed the old fild-before-dtor and error-arm residue), switch, float-first if, direct item.Float in the float arm, /Op | next: a named `long` copy of GetInteger() in the integer arm, or the shared CStringParser.h class model; else lane gap: register swap
- 009350c0 CAIStateGroup_ScriptingBase<CAIStateGroupBase>::Fired DIFFER(159v159): ours `fstp [esp+0Ch]` (1+variation) before the variation's integer copy and `fld variation` after `push ecx`; retail copies the variation between fadd and fstp and loads it before `push ecx` | tried: declaration order frames/scale/variation (frame slots now right), variation-first (slots wrong), direct GetInterface out-local, unnamed 1+variation | next: lane gap: x87/int store scheduling. The parked source now carries the PDB spelling (protected, class key V).
- 006af3f0 CMessageEventFilter_IsMyDeadLeader::operator() DIFFER(172v160): CDefPointer zero store before the pushes (retail after), true exit calls the def dtor out of line, false exit re-reads Object after `dec [eax+4]` | tried: IsThingOfType<T>(const CThing* const&) + mask local in CIntelligentPointer<CThing>::Get() (reproduces the compiled-out `mov [esp+0Ch],eax ; xor al,al ; jne`), __forceinline ctor/dtor with and without a local pointee, shared engine/CDefPointer.h (all 192 bytes with a `mov esi,[eax+4] ; add eax,4 ; dec esi ; mov [eax],esi` release) | next: plain-inline dtor holding the pointee in a local (`CDefPointeeBase* o = Object; if (o) o->Release();`), the def declared before the `dead` test
- 00c11520 global::Inventory_ProcessNavigationItem (descriptive CPanel::DispatchNavigation) DIFFER(163v163): retail stores the first direction word `mov [esp+10h],ecx` before `push eax`, ours after | tried: struct assignment, memberwise x/y/z, z-first locals (earlier), inline user operator=, a named reference to the event's direction | next: lane gap: store/push scheduling
- 009f18f0 CGameEventDataPackage::GetData DIFFER(166v166): callee-saved permutation (retail this=ebx size=ebp StartPos=edx; ours size=ebx this=edx StartPos=esi) | tried (on top of the earlier levers): a named `dest` byte pointer, StartPos re-read with a `rest` local (179) | next: question the IsValidPosition guard model (the `xor ecx,ecx ; jl/jg/jb` 64-bit range test), else lane gap: register allocation
- 0061a9d0 CTCInventoryQuests::MoveToNextTestQuest DIFFER(168v168): callee-saved permutation (retail this=esi, vector start loaded straight into ebp, index=edi; ours this=edi, start in ecx copied to esi) | tried (on top of five earlier levers): named `selected` local, early-return guard, named first/last in the dtor plus a named first in size() | next: the real STLport 4.6 vector<CTestQuest> (_Vector_base + _STLP_alloc_proxy, same-TU noinline members), else lane gap: register coalescing

## Skipped
- None.

## Label corrections
- 0x00C418F0 "Math_ComputeVectorMagnitude" (propagated) is a libpk vector normalise-copy member (descriptive CLibpkObject1D8::NormaliseCopy).
- 0x00768210 catalog "CTCObjectAugmentations_InitAbilitySlot" is CTCObjectAugmentations::CopyAugmentationDataFromThing(const CThing&) (Ego_r.pdb, landed).
- 0x0048C950 catalog "_unattributed::~CPhysicsMeshInfo_Derived_0048c950" is ??1CCreatureAction_SheatheItemToInventory@@UAE@XZ (vland RELOC_NAME warning).
- CTCCarriedActionUseBase (vtable 0x0123F894): slot 21 GetInterfaceType = 0x0E. Slot 30 is a bool getter, false in the base (IsUsableWhileThingPerformingAction, probable from the PDB name set). Slot 32 is OnActionUseInHand(CThing&) (0x007ECE60, landed as "vfunc_32").
- CWeaponDef retail +0xD0 is NeedsLoading (Ego_r +0xE0). The older parked source's "IsAnalogueLoading" name was wrong: UseAnalogueLoading is retail +0x90.

## Header debt
- 0x005DAC50 keeps a local counted-pointee model (`CDefPointeeBase` with `if (--IVRefCount == 0) Delete();`). The shared
  engine/CDefPointer.h (Release() returning the count) grades 156v156 here: it puts `dec eax` before `push ebx`. vland warns
  SHARED_HEADER_AVAILABLE CDefPointeeBase.
- 0x0058400A keeps a local CCreatureActionBase (virtual dtor), as the landed 0x0062B7C0 does. engine/CCreatureActionBase.h models
  the vptr as data. vland warns SHARED_HEADER_AVAILABLE CCreatureActionBase.
