# Hand chunk 111 (CThingHolySite .. CThingPhysical): 36 of 40 byte-exact

## What matched (36)
- **CThingHolySite** (0x0083B9F0 DrawIsDrawable, 0x0083BA20 OnCreate, 0x0083BA70 slot 36, 0x0083BB40 slot 3, 0x0083BBA0 slot 4):
  `return engine->slot9() && CThing::DrawIsDrawable();` gives the full-register 0/1; OnCreate assigns the
  def's 14-byte CEngineGraphic (pack 2, PDB size 14) whole (edx/ax moves in a speed TU) and sets two 100.0f
  extents; slot 36 is DrawPreparePrimitivesForRendering (base, then PTCGraphicAppearance +0x64 without a null test);
  slots 3/4 are SerialiseGameState / ConstructFromLevelScript re-fetching the def with
  `GetDefinitionManager()->GetDef(DefGlobalIndex, Def)` (arguments pushed before the manager call) and
  `PlayerStart = Def->PlayerStart != 0` (`setne`).
- **CThingSwitch** (0x0083BE50, catalog says CThingHolySite): same DrawIsDrawable body (vtable 0x012453FC).
- **Deleting destructors** (catalog "CThingHolySite::vector_deleting_destructor", all propagated):
  0x0084B620 CCreatureAction_PerformExpressionExtended, 0x00982550 CCreatureAction_SendMessage, 0x00A009A0
  CGraphicInfoFrame -- no vtable store = non-polymorphic record whose FIRST member is the polymorphic base
  (24g/24m model) + a `delete` helper; the shared block's release must be a member of the block
  (`m_pBlock->Release()`), the `--m_pBlock->m_references` form re-reads the member. 0x00ABFAA0 C3DMeshMaterial
  stores its own vtable = user-declared `__forceinline virtual ~T() {}` + `__forceinline` member destructor + inline
  constructor + `new` factory.
- **CThingHolySiteDef / CThingMarkerDef::Copy** (0x00464498, 0x004644C0): CThingVillageDef sibling model
  (`static_cast`, non-virtual base CopyBase, memberwise assignment); the 14-byte CEngineGraphic becomes
  `movsd x3 ; movsw` only under `#pragma optimize("s", on)`.
- **CThingManager** (0x0051DEE0/0x0051DF20 GetMapAt, 0x0051DF80 AddToRenderList, 0x0051E530 GetNonLocalThingChecksum,
  0x0051EFC0 SetNonScriptedEntitesPaused, 0x005202F0 OnCreateClass, 0x005215F0 OnDeleteClass): the C2DCoordI is filled
  member by member (x first); MSVC lays overloaded virtuals out in REVERSE declaration order (declare the const
  CWorldMap::GetMapAt first to put it in slot 15) and the const twin reaches the world map as a const object;
  HasTC returns the masked word (`test dword ptr [ecx+2Ch],800000h`; a bool narrows to `shr ; test al,1`); the
  UID map key is an `unsigned __int64` at CThing +0x18 behind a vptr = `#pragma pack(4)`; the insert result
  must be a non-POD (iterator with a user constructor) to come back by hidden pointer; erase copies the key
  because it comes from a by-value accessor.
- **NThingMap::CThingMap** (0x0085CFE0 GetMemoryFootprint, 0x0085D370 ctor): retail vectors are 12-byte STLport
  {start, finish, end}; `Cells[level].size() * sizeof(CThingMapCell)` with `operator[]` as `*(begin() + n)`;
  the ctor's vector members have inline constructors (vtable store first), C2DBoxI needs a memberwise user copy
  constructor for the direct `[esi+20h..]` stores (the POD copy goes through `lea edx` + edi).
- **CBaseFilter** (0x00CAC830, catalog "CThingMap::CThingMap"): the DirectShow base-classes constructor;
  `AM_NOVTABLE` on the class and the interfaces removes every vtable store, SDK layout and initialiser list
  as published, the CLSID a POD struct copy.
- **NThingMap::CThingMapManager** (0x00637980 RemoveMapwhoNodeFromCell, 0x00637A80 GetThingMapAt): plain unlink with
  the three clears tail-duplicated; GetThingMapAt is `const`, the entry pointer must be a named local (esi)
  for the re-read on the hit, the area test an inline `C2DBoxI::Contains`.
- **CThingMarker** (0x0083D100 DrawIsDrawable): `if (base) return a || b; return false;` mixes `xor al,al` and
  the full-register 0/1.
- **CRenderTarget::~CRenderTarget** (0x004979A0, catalog "~CThingNoiseDef"): novtable class over CBase with an
  empty user destructor; the two CSurface members' inline virtual destructors store their vtable and call
  Uninitialise; base destructor tail-jumped.
- **CThingVillage / CThingObject constructors** (0x005299C0 -- catalog "CThingObject" --, 0x007031C0): the landed
  CThingSwitch layering; all zero stores after the derived vtable = member objects with inline constructors
  (CEngineGraphic() zeroing all five fields; CDefPointer PDef()).
- **CThingObject** (0x00702CE0 ResetDieOffTimer, 0x00703010 slot 57, 0x007030C0 slot 3, 0x00703110 slot 4,
  0x00703470 slot 22, 0x00703780 slot 64): slot 57 is the def's `UseHighDetailQuadTree` `unsigned char` bitfield
  (bit 3 of +0x98); slots 22/64 use the landed out-parameter GetInterface template (presence bitset +0x20,
  interface map +0x44, key in the `push ecx` slot) -- `!CreatedByOwnerThingCreation` gives
  `xor eax,eax ; mov al ; shr eax,1 ; not eax ; and eax,1`.
- **CThingPhysical** (0x006A4D20 OnCreate, 0x006A4D60 ValidateInitialisation): a named CCharString local in its own
  scope for `AddTC("CTCSoundPlayer")`, `? true : false` on the base byte; `if (!map->IsPosInMap(pos)) return false;`
  returns the callee's zero `al`.

## Parked for Fable 5.1 (4)
- 0051e4f0 CThingManager::GetPhysicsDebugInfo DIFFER(50v50): `mov edx,[ecx] ; call [edx+164h]` vs retail `mov eax,[ecx] ; call [eax+164h]` | tried: named physics local, bool-returning virtual, inline GetPhysics() accessor twice, Dinkumware iterator object | next: lane gap: register choice (vtable register after a dead eax temp)
- 00521a90 CThingManager::SetNameForThing DIFFER(78v78): first operator[] key `lea ecx` vs retail `lea eax`; second call `mov edx,[name] ; push edx ; lea eax ; push eax ; mov ecx,esi` vs retail `mov ecx,[name] ; push ecx ; lea edx ; mov ecx,esi ; push edx` | tried: inline `find()==end()`, named find local (fixed the hoisted head load), Contains() helper, named entry references | next: lane gap: register allocation of argument temporaries
- 0083d280 CThingMarker::GetProperties (slot 48) DIFFER(103v94): the middle add-ref/release -- retail `mov ecx,[esi+4] ; inc ecx ; mov eax,ecx ; dec eax ; mov [esi+4],ecx ; mov [esi+4],eax` on the cached def, the lane `inc [eax+4]` then re-reads the address-taken local and releases through the ReleaseTemporary shape | tried: explicit temporary copy, inline by-value def getter, `__forceinline` getter (final untested release + Properties read now match), direct count arithmetic in the copy ctor/dtor | next: copy the def pointer into a LOCAL inside the copy constructor/destructor (24f rule) so the release does not re-read the frame slot
- 00702c60 CThingObject::DrawIsDrawable DIFFER(76v75): blink parity `mov eax,[esi+0D4h] ; and eax,1` vs retail `mov al,[esi+0D4h] ; and al,1` | tried: `(x & 1) != 0`, `(bool)(x & 1)` with the nested-if (fixed the shared `xor al,al` tail), `(bool)((unsigned char)x & 1)`, an `unsigned char` return type | next: a byte-typed view of DieOffTimer (union/`*(unsigned char*)&`), else lane gap: load narrowing

## Skipped
- none (no row spans two functions).

## Label corrections
- 0x0083BE50 is CThingSwitch::DrawIsDrawable (vtable 0x012453FC slot 27), not CThingHolySite.
- 0x0083BA70 / 0x0083BB40 / 0x0083BBA0 (and CThingObject 0x007030C0 / 0x00703110): "vfunc_36/3/4" are
  DrawPreparePrimitivesForRendering / SerialiseGameState / ConstructFromLevelScript (the base calls carry those names).
- 0x0084B620, 0x00982550, 0x00A009A0, 0x00ABFAA0 "CThingHolySite::vector_deleting_destructor": scalar deleting
  destructors of CCreatureAction_PerformExpressionExtended, CCreatureAction_SendMessage, CGraphicInfoFrame, C3DMeshMaterial
  (their vtables' slot 0).
- 0x00CAC830 "CThingMap::CThingMap" is DirectShow CBaseFilter::CBaseFilter(name, pUnk, pLock, clsid); 0x00CACC80 is CUnknown::CUnknown.
- 0x004979A0 "CThingNoiseDef::~CThingNoiseDef" is CRenderTarget::~CRenderTarget.
- 0x005299C0 "CThingObject::CThingObject" is CThingVillage::CThingVillage (vtable 0x012451BC).
- 0x00703010 "vfunc_57" reads CThingObjectDef::UseHighDetailQuadTree; 0x0087C420 (manifest "CTCSinglePersonOccupiable::IsOfType")
  tests a property mask against its def's +0x28 word.
- 0x004C7990 / 0x004C79D0 (manifest "CViewBase::GetDisplayEngine") are CThing's GetDefinitionManager / GetWorldMap
  accessors (thing manager +0x20, then +0xd0); 0x00415FD3 "DivideByGameSpeed" is a `__stdcall` static.
- 0x0052A200 / 0x0052AFB0 / 0x00527E00 / 0x0052B8B0 are the UIDToThingMap insert / erase and the NPCNameMap find /
  operator[] (manifest "BinaryTree_*", "CParticleMorphs_Load").
- CThing bit layout check: +0x18 UniqueID, +0x20 component presence bitset, +0x44 interface map, +0x91 bit 0 Dead.
