# Hand chunk 67 (CPhysicsForceModelGotoOutsideSimple .. CPlayer): 37 of 40 byte-exact

All 37 matched sources re-graded with `gall.sh` (MATCH / RELOCATION_MATCH, default flags, no pragma
needed anywhere); all 37 tests print `<addr>_TEST PASS` under g++ (`tall.sh`) and under the lane's
VC7.1 cl.exe (`tall.py`). Identities were cross-checked against Ego_r.pdb names
(`ghidra_out/egor_pdb_names.tsv`: neighbour order and mangled signatures) and `rtti-vtables.tsv`;
line 1 of each source carries the real identity where the catalog label is wrong (see Label
corrections).

## What matched (37)

- **NCameraModes::CPhysicsForceModelGotoOutsideSimple** (008fcbd0 DoAttractToPoint, 008fcc10
  DoVelocityDamping; vtable 0x01275950 slots 6/7): `return CPhysicsForceModelBase::X() * k;` (3.0f,
  6.0f). The inline `C3DVector * float` goes through the value constructor straight into the hidden
  return slot (arguments right to left: z, y, x multiplied first).
- **CQuestManager** (004af740 GetIndexFromRegisteredQuest, catalog "CPhysicsMesh::GetNoDummyObjects"):
  index search over RegisteredQuestNames (retail +0x2C) with the inlined CCharString `==`. Retail
  reads the key's data block ONCE after the entry test: rotated loop (`if (it != last) { CCharString
  key = name; do { ... } while (it != last); }`). A plain `for` re-reads `name` per iteration; a
  local before the loop loads ahead of the entry test.
- **CPhysicsMesh** (retail keeps the PDB member order with 12-byte containers: HelperPoints +0x10,
  DummyObjects +0x20, TotalFaceIndices +0x30, TotalPoints +0x3C, NavigationLines +0x48,
  NavigationContainmentVolumes +0x54, RootSegment +0xB4, CentreOfGravity +0xA8):
  - 00a6c850 CalculateSegmentation: `if (TotalFaceIndices.size() > 60) RootSegment.Reset(Create(...))`;
    6-byte C3DTriangle2 = multiply by 0x2AAAAAAB with no shift; `jbe` = unsigned size().
  - 00a6d050 GetAllHelperPointNames / 00a6d090 GetAllDummyObjectNames: `names.reserve(n);
    for_each(first, last, CAddName(names));` with the SGI `__fastcall` for_each returning the functor
    by hidden pointer. The functor needs a user constructor (hidden return) and NO copy constructor
    (pushed as a dword); VC homes the discarded result in the dead `names` slot.
  - 00a73d10 GetNavigationLevelAt: `count != 0` guard, counted loop, `const SNavigationLevel& level
    = first[i]` so the level is read through the same pointer after the call; `this` spilled to the
    `push ecx` slot.
  - 00a73d70 GetNavigationLines: `level < size()` (signed) then the retail vector iterator OBJECT
    (`*(begin() + n)`, `*begin()`) so both arms re-read `first` (raw `first[n]` keeps it in esi).
- **CPhysicsMeshInfo** (vtable 0x0129CD50 over CMeshDataBankEntry 0x0129CD28; MeshData
  CIVCountedPointer at +0xC):
  - 00a24f30 `~CPhysicsMeshInfo`: own vtable store FIRST, then the counted pointer as a member object
    with a `__forceinline` destructor (pointee in a local, member cleared in the null test).
  - 00a2b740 `??_G`: the same with `__forceinline virtual` destructors plus a `new CPhysicsMeshInfo`
    factory.
  - 00a24f90 Reset (private virtual, slot 3): `MeshData = 0;` through `operator=(T*)` = release +
    the second unconditional zero store.
  - 00a26370 Sizeof (slot 2): the landed C3DAnimationInfo::Sizeof form
    `(p ? p->Sizeof() + sizeof(p) : sizeof(p)) + 0xC0` with CMeshData::Sizeof inlined
    (0x2C + mesh footprint (4 + Sizeof) + file-data footprint (8 + Sizeof)); matched first try.
- **CPixelShader / CVertexShader / CBaseObject** (three more catalog "~CPhysicsMeshInfo" rows, real
  classes from the vtables they store): 00987e20 `~CPixelShader` and 009be1e0 `~CVertexShader`
  (data pointer read once into a local, `--count == 0` then the virtual `delete`, no clearing store,
  inline CShaderBase destructor); 0099a430 `~CBaseObject` (IntelligentPointer block: `object = 0`,
  then the member re-read because the store may alias it, free when the refcount is 0, null it).
- **CPixelShader** 00a5ecb0 constructor: `Data(new CPixelShaderData(shader, consts, name))` in the
  initialiser list (vtable store before the allocation, null arm stores the zeroed eax).
- **CPixelShaderData** 00a5ed00 `??_G`: `__forceinline virtual ~T() { if (PixelShader)
  PixelShader->Release(); delete[] ConstIndices; }` over a base with an out-of-line virtual
  destructor (0x0099A300), COM Release as a `__stdcall` virtual, plus a `new` factory.
- **CPhysicsMeshObject** (vptr, Mesh +4, PositionMatrix +8, Scale +0x38; base CBaseClass ctor
  0x0099A2D0 / dtor 0x0099A2E0):
  - 00a11ec0 GetCentreOfGravity: `return PositionMatrix.Rotate(Mesh->CentreOfGravity);` -- the
    rotation MUST be an inline CMatrix3x4 member taking `const C3DVector&` with a named local result.
    Written directly in the caller VC7.1 reorders every sum to start with the X term whatever the
    source order (3 attempts); through the inline helper it starts with Z like retail.
  - 00a127d0 `~CPhysicsMeshObject`: member counted pointer with a `__forceinline` destructor, base
    destructor as a tail jump.
  - 00a12cc0 constructor: default-constructed Mesh (zero store) then `Mesh = mesh;` (operator=
    re-reads the member) and SetPositionMatrix.
  - 00a12840 GetRigidBodyBoundingPoints / 00a128a0 HasContainmentVolume / 00a12c90
    HasNavigationLineListInfo: a LOCAL copy of the counted pointer (`CIVCountedPointer<T>
    mesh(Mesh); mesh->Call(...)`; add-ref, release without clearing store, bool in bl); the scale
    functor of GetRigidBodyBoundingPoints is built in the dead argument slot that also receives the
    discarded for_each result.
- **CPixelFormat** 009e3790 Initialise(r, g, b, a, linear): do/while over the CD3DFormatInfo table at
  0x0129BA40 (PDB layout confirmed by the table bytes; BitDepth -1 terminates), `this` from
  engine/CPixelFormat.h; `linear` unused.
- **CPlane** 00a42140 Initialise: the C3DVector copy must be the IMPLICIT POD assignment (gives the
  retail `mov edx,ecx` this-copy and the esi temporary); a user memberwise operator= is 32 v 36.
- **CPlayer** (engine/CPlayer.h agrees with retail below +0x210; the retail PlayerModes list at
  +0x210 is the 4-byte head, so later members sit 8 bytes lower than the PDB):
  - 00487bd0 UninitCharacter: the inlined IsMainPlayer reads Number into a LOCAL before the call
    (`long number = Number; return number == PlayerManager->GetMainPlayerNumber();`); written as
    `Number == ...` the lane compares `[esi+28h]` after the call.
  - 00487eb0 SetAggressiveMode (AggressiveMode retail +0x217): `if (!A && on) { A = true; Setup(); }
    else if (A && !on) { A = on; Setup(); }`, both `call ; ret 4`.
  - 00633700 InitModeChargeQuickAccess, 00633b40 UninitModeQuestCompletionUI, 00634470
    UninitModeChargeQuickAccess, 00636e50 InitModes: the landed CPlayer_Methods + header pattern
    (virtual AddToInterface slot 1, non-virtual RemoveFromInterface); InitModes re-tests Local
    after the virtual call.
  - 00634160 HasMode: linear find over the 4-byte list at +0x210, `return node != head;` (full
    register bool).
- **CTCCreatureModeManager** 0073a8a0 / 00811340 IsModeActive (catalog "CPlayer::HasMode",
  byte-identical pair): find over PModes (+0xC) with the cached end for the loop and
  `return node != PModes;` re-reading the head.
- **CAIStateGroup_OpinionReactionRoot** 00982eb0 / 00982f10 / 00982f70 WifeFeeling/General/
  ToHerselfUpdate(EOpinionAttitudeType) (catalog "CPlayer::SayWifeOpinion*"): explicit
  `CCharString text("TEXT_AI_WIFE_OPINION_ACT_..._", -1)`, a dropped getter call (0x008FCFC0),
  `text += g_OpinionAttitudeNames[attitude]`, `return Say(...) == 2;` (bool in bl across the
  destructor).

## Parked for Fable 5.1 (3)

- 00a18760 CPhysicsMesh::GetPointIntersectionFromTo DIFFER(93v95): retail `mov edx,ecx` before the pre-test call and `[edx+0B4h]` / `mov ecx,edx` after it, plus `xor bl,bl` false result; lane keeps `this` in esi and `xor al,al` at the exit | tried: `bool result = false; if (pre-test) { if (g && seg) return A; return B; } return result;` (pointer and PDB reference signatures) | next: lane gap: whole-program (LTCG) register knowledge that 0x00A17690 preserves edx
- 00487aa0 CPlayer::InitJoystickDeviceNumber DIFFER(67v67): else arm `dec ecx ; and ecx,eax ; mov [esi+228h],ecx` vs lane `dec ecx ; and eax,ecx ; mov [esi+228h],eax` | tried: direct `Number == GetMainPlayerNumber()`, inline IsMainPlayer, negated condition, inline IsMainPlayer with a `number` local (the last fixes ebx/edi and the else-arm Number re-read) | next: write the else arm as `number != mainDevice ? number : 0` or `JoystickDeviceNumber = Number; if (Number == mainDevice) JoystickDeviceNumber = 0;` on top of the local-Number IsMainPlayer
- 00c21330 CPlayer::IsFirstPersonTargeting DIFFER(48v42): retail keeps `this` in ecx across `call 0x00C20EE0` (`add ecx,38h` after it, no `push esi ; mov esi,ecx`) | tried: `!= 0` byte result, bool member read from a named 16-byte record local (only the this-register difference remains) | next: lane gap: whole-program register knowledge of the 0x00C2xxxx callee (the 00c20cb0 family); identity also unproven (`ret 4` vs the PDB's no-argument IsFirstPersonTargeting)

## Skipped

None (no row spans two functions).

## Label corrections

- 008fcc10: "CPhysicsForceModelGotoOutsideSimple::DoAttractToPoint" -> NCameraModes::CPhysicsForceModelGotoOutsideSimple::DoVelocityDamping (vtable 0x01275950 slot 7; Ego_r order DoAttractToPoint then DoVelocityDamping 0x40 apart). 008fcbd0 is DoAttractToPoint (slot 6); the class is in namespace NCameraModes.
- 004af740: "CPhysicsMesh::GetNoDummyObjects" -> CQuestManager::GetIndexFromRegisteredQuest (body indexes RegisteredQuestNames at retail +0x2C like GetRegisteredQuestFromIndex 0x004AF3C0; Ego_r has it right after that function).
- 00a6d050: "CPhysicsMesh::GetAllDummyObjectNames" -> CPhysicsMesh::GetAllHelperPointNames (HelperPoints +0x10, 20-byte records; Ego_r order HelperPoint then DummyObject names).
- 00987e20: "CPhysicsMeshInfo::~CPhysicsMeshInfo" -> CPixelShader::~CPixelShader (vtables CPixelShader 0x0129A054 / CShaderBase 0x0129A030).
- 0099a430: "CPhysicsMeshInfo::~CPhysicsMeshInfo" -> CBaseObject::~CBaseObject (vtables CBaseObject 0x0129A7D8 / CBaseClass 0x01231710).
- 009be1e0: "CPhysicsMeshInfo::~CPhysicsMeshInfo" -> CVertexShader::~CVertexShader (vtable 0x0129B3D0).
- 0073a8a0 and 00811340: "CPlayer::HasMode" -> CTCCreatureModeManager::IsModeActive (Ego_r `QBE_NK`, right before AllocateMode as at 0x0073A8E0; 00811340 is a byte-identical second copy, class not separately proven).
- 00982eb0 / 00982f10 / 00982f70: "CPlayer::SayWifeOpinionFeeling/General/ToHerself" -> CAIStateGroup_OpinionReactionRoot::WifeFeelingUpdate / WifeGeneralUpdate / WifeToHerselfUpdate (Ego_r `AAE_NW4EOpinionAttitudeType@@@Z`, 0x60 apart as here).
- Callee labels: 0x00A6CBC0 "C3DMeshVolume_HasVolume" = CPhysicsMesh::HasContainmentVolume; 0x00A73CE0 "CTCInventoryBase::GetCanItemBeQuickAssigned" = CPhysicsMesh::HasNavigationLineListInfo; 0x0049BB40 / 0x00449700 "CWorld::DrawGetEnvironment" = CGameJoystickManager::GetMainPlayerJoystickDeviceNumber / CPlayerManager::GetMainPlayerNumber; 0x0099A2D0 / 0x0099A2E0 / 0x0099A300 "CBase::CBase" = CBaseClass constructor / destructor copies (store vtable 0x01231710); 0x008FA7E0 / 0x008FA800 = NCameraModes::CPhysicsForceModelBase::DoVelocityDamping / DoAttractToPoint; 0x00A2E710 `_Cons_val<...>` = the navigation volume's containment test; 0x004BA7A0 "CCombatWheel_Resize" = vector<CCharString>::reserve.
- 00c21330 "CPlayer::IsFirstPersonTargeting" is doubtful (body `ret 4`, PDB function takes no argument; callees in the 0x00C2xxxx module); kept, not corrected.
