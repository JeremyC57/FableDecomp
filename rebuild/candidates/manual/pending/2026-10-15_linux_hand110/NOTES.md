# Hand chunk 110 (CThingFilter_HasFreeActionPoint .. CThingHolySite): 36 of 40 byte-exact

All 36 sources grade MATCH / RELOCATION_MATCH (grades.txt) and their behaviour tests print PASS under g++ (tall.sh)
and under the lane's VC7.1 cl.exe (tall.py). One row is parked, three rows are skipped (multi-function rows).

## What matched (36)

- **CThing interface-component filters** (006a8b40 HasFreeActionPoint, 0080e480 IsAffectedByObstructions, 007892ea
  IsAffectedByTraps, 006a8450 IsPerceptible, 006a81f0 / 00809210 NoiseCameFromThing, 0085afd0 IsValidCombatSpline,
  006a8c00 IsInSameVolumeContextAsThing ctor): the landed `__forceinline` out-parameter `CThing::GetInterface(type, &out)`
  template (bitset +0x20, CVectorMap +0x44, LowerBound @ 0x004365B0) reproduces every presence test (`test ch,20h`,
  `test dword ptr [eax+24h],20000h`, `test ecx,ecx ; jns` for bit 31) with the key homed in the dead `thing` slot.
  `return Get && x;` materialises an int (`mov eax,1`/`xor eax,eax`), constant returns stay byte-sized; the "present"
  case must be the `if` body when retail puts the constant tail last (006a8450). 007892ea is the size TU twin:
  `optimize("y",off)` + `("s",on)` and `std::bitset::test` (0x00410DE0) called out of line. 006a8c00: the bool is an
  init-list member, the pointer is cleared in the body (retail stores +4 before +0).
- **Physics-position filters** (006a6f50 HasPhysicsMesh, 006a76b0 In2DRange, 006a7780 In3DRange, 006a7930 InMap,
  006a7960 InRegion, 006a7020 IsWithinZRange, 006a7a80 IsInBuilding, 006a7640 IsNavLineOfSightToThing, 006a7390 IsInFOV,
  006a7ad0 MakesLineWithPosInDirectionOfLine, 0063efb0 CThingFindNavigationLayerAt): retail CThing holds PhysicsTC at
  +0x60 (PDB +0x70 less the retail 0x10 narrower head) and CurrentMapNumber at +0x8E; the component's Position is at +0xC,
  Radius +0x34. Shape lessons: `Range` loaded before the deltas = a `float range = Range;` local; three deltas loaded
  before the products = a C3DVector local assigned member by member; `fabs(a - b) < r` (math.h intrinsic) gives
  `fabs ; fcomp ; test ah,5 ; jp`; `== member` after a call is `sub ; neg ; sbb ; inc`; the IsInFOV overload's unused
  radius out-value is homed in the dead argument slot; FindNavigationLayerAt keeps a 0x40-byte CPhysicsMeshObject local
  (hidden-pointer result) with no EH frame.
- **CThingFilter_HasScriptData** (006a7f30): the landed inline CCharString `operator==` (0x00A14EE0 model) on +0xE8.
- **CThingFilter_SolidToThing ctor** (00c61c70): four init-list stores of a 16-byte record (see Label corrections).
- **Re-attributed rows** (labels propagated): 00ca49c0 CTextureRenderer vtable slot 30 (`if (m_pPosition)
  m_pPosition->ResetMediaTime(); vfunc28(); return 0;`), 00c1e9f0 a 0xC1-module sequence client (`__fastcall
  AlignDuration(seq, GetTime(), m_extra)` -- right-to-left: the stack word is pushed before the virtual call),
  0075c4f0 CTCCameraPointScriptedSpline::IsCombatSpline (parent thing read into a named local so `mov ecx,[esi+4]`
  precedes the literal push and the hidden-pointer `lea edx`), 0083b3b0 CThingVillage::DrawIsDrawable (`return
  GetDisplayEngine()->vfunc9() && CThing::DrawIsDrawable();`).
- **CThingGameObject** (008388d0 ConstructFromParams, 00838a40 OnCreate, 00838b20 IsAddToComboMultiplierOnHit, 00838b60
  IsGivingHeroStatChangesOnBeingHit): ConstructFromParams forwards 7 of its 8 arguments (the script name, 5th, is used
  only for `if (name != "") ScriptName = name;` at +0x74); OnCreate is `PThingStandingOn = CIntelligentPointer<CThing>(0);`
  (inline derived ctor stores its vtable after the out-of-line base ctor; generated dtor) plus four one-bit clears that
  merge into `and byte ptr [esi+0C8h],0F0h`, success path as the `if` body; the two getters copy the def pointer (+0x70)
  into a local CDefPointer (copy ctor `if (p) ++count` = `inc dword ptr [ecx+4]`, destructor load/dec/store with no
  null test) and return a bitfield (`shr bl,3 ; and bl,1`) held in bl across the release.
- **Deleting destructors filed under CThingHolySite** (labels propagated): 004c3cd0 C3DAnimationBlendState and
  005e0de0 (no vtable store, member release then base destructor on `this`) = non-polymorphic class with the polymorphic
  base as FIRST MEMBER and `__forceinline` destructors; 00696060 / 004ae450 = plain non-polymorphic classes deleted
  through a helper; 00724de0 CTCPhysicsStandard (user `__forceinline` virtual destructor with an explicit `if (p) delete
  p;`, abstract `__declspec(novtable)` CTCPhysicsBase with a `__forceinline` destructor releasing PhysicsDef +0x38),
  0079c570 CTCSummonableCreature (`#pragma pack(1)` derived class: Summoner at +0x11) and 007a62d0 CTCForcePushPower
  (CIntelligentPointer member with a generated destructor, CDefPointer PDef release, out-of-line CTCBase dtor 0x00686830).

## Parked for Fable 5.1 (1)

- 0063ef50 CThingFilter_OverlappingCircle::operator() DIFFER(95v95): retail squares dx first (`fld st(2) ; fmul st(3)` twice) where ours squares dy first (`fld st(1) ; fmul st(2)` then `fld st(3) ; fmul st(4)`) | tried: named C3DVector local (reads the frame slot), const-ref local, inline operator- + GetLengthSquared2D (spills), inline `Overlaps(const C3DVector&, physics)` helper (only the square order left) | next: `dy * dy + dx * dx` inside the helper, or the deltas in a C3DVector local inside the helper

## Skipped

- 005271b0 (`jmp [eax+0ACh]` thunk + int3 + a 0x18-argument constructor at 0x005271C0), 00ac20d0 (a `jmp [eax+10h]`
  thunk + 0x00AC20E0 word getter + 0x00AC20F0 constant getter), 00caef70 (a `jmp [eax+30h]` forwarder + three
  `mov eax,ecx ; ret` bodies at 0x00CAEF80/90/A0): each catalog row covers several functions.

## Label corrections

- 00ca49c0 "CThingFilter_HasNavigationInformation::operator()" -> CTextureRenderer vtable 0x0129D08C slot 30 (no argument, returns 0; DirectShow EndFlush-style body).
- 006a7780 "CThingFilter_In2DRange::operator()" -> CThingFilter_In3DRange::operator() (sums all three squared deltas).
- 00c1e9f0 "CThingFilter_In3DRange::operator()" -> unattributed 0xC1-module sequence client (called by landed 0x00C1E280 as its "First" query).
- 0075c4f0 "CThingFilter_IsSuspect::operator()" -> CTCCameraPointScriptedSpline::IsCombatSpline (only caller 0x0075D8B0).
- 0083b3b0 "CThingHolySite::DrawIsDrawable" -> CThingVillage::DrawIsDrawable (vtable 0x012451BC slot 27).
- 004c3cd0 / 00724de0 / 0079c570 / 007a62d0 "CThingHolySite::`vector_deleting_destructor'" -> scalar deleting destructors of C3DAnimationBlendState / CTCPhysicsStandard / CTCSummonableCreature / CTCForcePushPower (vtable slot 0); 004ae450, 005e0de0, 00696060 are unreferenced copies, named by address.
- Callee labels: 0x0099A2E0 is CBaseClass::~CBaseClass (stores vtable 0x01231710), 0x00686830 is CTCBase::~CTCBase (stores 0x0125BE0C), 0x00CADDC0 is CRendererPosPassThru::ResetMediaTime, 0x004C73D0 is CThing::GetPosition (not "DoSizeof"), 0x007FEBF0 is IsActionPointFree (trusted), 0x004AE040 is a vector destructor (not CCombatWheel::ResetRings), 0x0073A6E0 / 0x007498D0 are component accessors (not CPixelShader::CountReferences / _Cons_val).
- 00c61c70 CThingFilter_SolidToThing ctor writes 16 bytes while the PDB class is 8 bytes; kept under the catalog name, possibly propagated.
