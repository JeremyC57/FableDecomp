# Hand chunk 199 (CCreatureAction_ScorpionKingDeath .. CEngineSkyRenderer): 7 of 40 byte-exact

## What matched (7)
- **CDataOutputStream::WriteDateAndTime** (00994180, MATCH): serialises a CDateAndTime (PDB: Year long, then six ulongs) as
  long + four bytes + two words. Shape: one inline typed writer per field (`WriteLong/WriteUByte/WriteUWord`), each by-value
  parameter homed in the dead argument slot; the word writer's `xor ecx,ecx ; mov cx` + dword store is just an
  `unsigned short` parameter.
- **CCreatureAction_UnsheatheWeapons::Initialise** (008b7ed0, RELOCATION_MATCH): ShouldPlayUnsheatheWeaponAction, then
  STANDARD_SHEATHE/STANDARD_UNSHEATHE, then the combat component's weapon type. Shape: the creature argument goes through an
  inline `operator CThing*()` on the intelligent pointer, which makes GetPItem run before the plain argument pushes.
- **CCreatureAction_TeleportIn / TeleportOut::Initialise** (008b9f20 / 008b9e50, RELOCATION_MATCH) and
  **CCreatureAction_UndeadReturnToEarth::Initialise** (008ec320, RELOCATION_MATCH): the "special-effect from the creature
  def" family -- GetSubDef<CSpecialEffectsDef> into a plain out-slot, the def copied into a named pointer that is released
  by hand at the end (that keeps it in edi/ebp across the calls), a named `CThing* creature` local (GetPItem before the
  pushes; the &PCreature register takes over `this`'s), and the lookup string as the implicit `const char*` conversion inside
  `found && def->GetEffectFromString("NAME", type)` (VC builds it before the pushes and keeps a construction flag).
  ReturnToEarth also needs a second named creature local for `SetSuspendAnimation(true, 0)`.
- **CCreatureAction_ShowItemToThing::PerformAction** (00845270, RELOCATION_MATCH): adds a CEventShownItem(creature, item)
  to the onlooker's entity-events component. Shape: the first constructor argument via the conversion operator (fetched
  before the item's plain GetPItem); the two `& 1` liveness tests share `mov bl,1`.
- **CEngineOcclusionManager::AllocateOccluderHandleWithRetrivalID** (00b8d6c0, RELOCATION_MATCH): next occluder entry (or
  -1) and a sorted retrieval-ID -> handle map. Shape: the entry is cleared by copying a zeroed local COccluderEntry at the
  size read once (`xor edx`/`xor esi` two zero registers, `inc [size]`), and the map lookup is an inline STLport
  lower_bound over a signed distance.

## Parked for Fable 5.1 (32)
- 008c4170 CCreatureAction_ScorpionKingDeath::Initialise DIFFER(218v218): frame 0xC v 8 -- retail reuses the dead GetSubDef out-slot word for the "DEATH" string | tried: function-scope slot, block-scoped slot, inline sub-def helper | next: a helper returning the CDefPointer by value
- 008b8bc0 CCreatureAction_ScreamerDrainAttack::Initialise DIFFER(231v231): ebx/ebp swap (retail `this` in ebp, target in ebx) | tried: flag + if/else, inline __forceinline helper, C3DVector delta | next: lane gap: register assignment
- 00845b10 CCreatureAction_TavernGameMasterInteraction::Initialise DIFFER(165v165): retail `this` in edi + `lea esi,[edi+0A8h]`, ours `this` in esi + `add esi,0A8h` | tried: inline IsAlive, named master local | next: lane gap: register assignment
- 008b7a20 CCreatureAction_SummonCreatures::Initialise DIFFER(236v236): esi/edi swap (`this` vs summon component) | tried: map-less GetInterface, named creature, unnamed effect | next: lane gap: register assignment
- 008c67a0 CCreatureAction_TrollThrowProjectile::FrameUpdate DIFFER(190v190): esi/edi swap after the named shot local fixed the push order | tried: named creature, alpha local, map-less GetInterface | next: lane gap: register assignment
- 008bf1c0 CCreatureAction_UnblockableMidAttack::Initialise DIFFER(178v178): the position interface's vtable load scheduled after the target's GetPosition (retail right after the creature GetPItem) | tried: plain args, conversion-operator AngleBetweenThings helper, named interface local | next: lane gap: scheduling
- 008ec260 CCreatureAction_UndeadRiseUp::OnAnimationLoaded DIFFER(188v188): esi/edi swap; a named creature for the effect fixes registers but moves `push 0` | tried: 4 local/reference forms | next: lane gap: register assignment
- 006a02b0 CCreatureAction_UnsheatheItemFromInventoryWithHasteAction::FinishAction DIFFER(240v240): frame 0x10 v 0x14 (the multiplier pair shares the key word) | tried: temporary, named local, explicit pair ctor | next: insert(make_pair(...)) through a real two-type pair conversion
- 00997880 CDriveManager::GetItemFromPath (fastcall) DIFFER(172v168): scan registers (retail c in cx, i in eax, count in edi) and the backward start search | tried: 4 loop forms | next: a do/while with the start test inside an inline helper
- 009e4cb0 CDriveManager::GetItemFromPath (thiscall twin) DIFFER(180v179): same as 00997880 | tried: the 00997880 best form | next: solve 00997880 first
- 0099ac90 CDiskFileWin32::MovePosition DIFFER(215v227): retail keeps a dead `cdq ; mov [esp+18h],edx` (the seek target's high half) and a 0xC frame | tried: LARGE_INTEGER local, inline SeekTo(LARGE_INTEGER) | next: an __int64 by-value helper or SetFilePointer through a LARGE_INTEGER* argument
- 009acd00 CDefinitionManager::ValidateDefs DIFFER(137v162): retail keeps the class iterator in the frame plus a second induction pointer (`lea edi,[eax+0Ch]`) | tried: pointer, reference, iterator class | next: a CVectorMap iterator with an out-of-line-looking operator++ / an index loop
- 009ad5f0 CDefinitionManager::DecompressDefinitionData DIFFER(231v231): ebx/ebp swap and the source sum in eax not ecx | tried: 2 declaration orders | next: lane gap: register assignment
- 006451dd NPlayerGui::CDrawAbilityButtons::DrawEffects DIFFER(172v173): ebx/edi swap (`this` vs index*8) | tried: one expression, polymorphic pointer, real CRightHandedSet, nested ifs + named emitter | next: a `CIntelligentPointer& slot = Effects[index]` reference
- 00c43fd0 CDrawAutoPickups::OnVisibilityChange DIFFER(177v181): SetState(1) compare byte-narrowed (retail dword `and ecx,7Fh ; cmp ecx,1`) and the Visible bitfield write xor-merged (retail if/else or/and) | tried: if chains, switch + SetState, register callback | next: explicit raw-mask Visible flip and a long-typed GetState
- 0064e0da NPlayerGui::CDrawGuildSeal::AddMessage DIFFER(228v228): ebx/edi swap once the manager load precedes the name push | tried: named string, implicit conversion, inline accessor, named manager pointer | next: lane gap: register assignment
- 0064b5c1 NPlayerGui::CDrawQuickAccessIcons::GetActivationTimeForButton DIFFER(169v167): the inventory pointer (and its absent-path garbage) lands in eax then ecx | tried: 4 IS_THING_ALIVE/def forms | next: an inline `T* GetInterface(type)` with no return on the absent path
- 00b24400 CEngine::ConvertScreenPosToWorldPosAtViewDistance DIFFER(223v238): retail preloads screen.Y, spills view X to [esp] and keeps a 0xC result temp | tried: inline and __forceinline matrix*vector | next: C3DVector operators (component temporaries) instead of a component formula
- 00b2a010 CEngine::AddEngineComponent<CEngineLightingManager> DIFFER(163v163): only the up-cast push_back temporary's slot (retail dead `component` slot, ours `globalPointer`) | tried: named local, reordered stores, inline register helper, in-class template | next: lane gap: frame slot choice
- 00b44f20 CEngineLightingManager::SetupShaderPointLights DIFFER(195v189): attenuation zero terms not hoisted into the preheader (retail `mov [esp+10h],edi`) and the loop head padded | tried: inner local, outer local, ctor temporary, outer local filled in the loop | next: a C2DVector/zero-pair member assigned once
- 00b61cd0 CEngineSkyRenderer::BuildBaseBandMesh DIFFER(201v209): retail hoists Z=-500 into ebp and walks eax/edx | tried: plain ++, post-increment pointers, store order | next: a C3DVector ring-position local copied whole
- 00ba6ef0 CEnginePrimitiveManagerStaticMeshesBase::GetSupportedTypes DIFFER(176v176): the 2nd/3rd value temporaries are not homed in the dead `types` slot | tried: temporaries, named locals, one reused local, enum element, by-value push_back, inline helper, s/t/y pragmas | next: lane gap: frame slot choice
- 00bd27f0 CEnginePrimitiveManagerAnimatedMeshesBase::GetSupportedTypes DIFFER(176v176): twin of 00ba6ef0 (9/11/13) | tried: the 00ba6ef0 form | next: solve 00ba6ef0
- 00bc5820 CEnginePrimitiveChangerManager::PreloadPrimitive DIFFER(171v182): retail keeps the result bool in cl (frame spill, `mov al,cl` returns), begin in ebx, lea+load indexing | tried: named count, named begin + handle reference | next: an `unsigned char` result and a for loop over a CArray iterator
- 00bc65d0 CEngineInternalPrimitiveGroup::SetChildPrimitive DIFFER(126v210): draft -- the handle replacement's two inlined CIVCountedPointer assignments are collapsed | tried: one draft | next: model CEnginePrimitiveHandle with the real CIVCountedPointer operator= (detach slot 3, release slot 1)
- 00bca6b0 CEnginePrimitiveManagerSpriteTrails::RenderPrimitive DIFFER(188v188): `push esi` and the distance/renderer loads scheduled differently | tried: user copy ctor (fixed the head copy), named distance, one expression | next: lane gap: scheduling
- 00bcc700 CEnginePrimitiveManagerWeaponTrails::RenderPrimitive DIFFER(190v192): twin of 00bca6b0 plus the strip-0 load order | tried: the 00bca6b0 form | next: solve 00bca6b0
- 00bd2a90 CEngineInternalPrimitiveAnimatedMesh::InitialiseBoneScalings DIFFER(197v218): draft -- loop counter in a frame word with two strided pointers in retail | tried: one draft | next: index loop with the bone pointer and the scaling pointer as named locals
- 00bdd2f0 CEngineLandscapeMap::CEngineLandscapeMap DIFFER(163v169): the lane folds the height's signed `/16` (cdq/and/add/sar) into `shr` (zero-extended ushort); argument registers differ | tried: plain, long getter, inline helper | next: lane gap: value-range folding (or a signed source type)
- 00bf44a0 CEngineLandscapePatch::RenderForegroundWater DIFFER(210v208): local copy stored before the type load, box copy via `add ecx,20h` | tried: 4 (if-chain, pair local, forceinline push_back + out-of-line Add, user operator=) | next: std::make_pair(BoundingBox, water) with a C3DBoundingBox copy ctor
- 005954ad CFrontEndManager::~CFrontEndManager DIFFER(213v213): `pop edi` scheduled before the base vtable store | tried: inline / __forceinline CObserver dtor | next: lane gap: scheduling
- 00cac4a0 CMediaRenderer_00cac4a0::OnRenderEffect DIFFER(194v194): `lea ecx` v `lea eax` for the CheckMediaType argument | tried: pointer, reference, ternary return | next: lane gap: register assignment

## Skipped
- 009be2d0 CDisplayManager::CalculateD3DPresentationInterval: the row holds the function, its jump table, int3 padding and two
  further bodies (a `ret 8` stub, a D3D query, an int-to-double divide).

## Label corrections
- 005954ad: "CEngineScreenEffectDisplacementRenderer::~CEngineScreenEffectDisplacementRenderer" is NUISystem's
  **CFrontEndManager::~CFrontEndManager** (stores vtable 0x012521A8 = CFrontEndManager by RTTI, only caller the deleting
  destructor 0x0059B641; members match the PDB CFrontEndManager less 4). 0x00594FA9 is CFrontEndManager::DestroyVirtualKeyboard(bool)
  (deletes +0x60), not "FinishWithDynamicTextures"; 0x0042FB40 is NUISystem::CManager's singleton delete (0x013B8710).
- 0x00B49320 is **CShaderPointLight::CShaderPointLight(const C3DVector&, const CRGBFloatColour&, float, const CShaderLightAttenuation&)**
  (vtable 0x012A2114), not "CShaderDirectionalLight".
- 0x0084AEE0 is **NEntityEvents::CEventShownItem::CEventShownItem(CThing*, CThing*)** (vtable 0x012723D8), not "CEventDied".
- 0x00CB1D60 / 0x00CB1CC0 / 0x00CB1D00 are DirectShow **CMediaType** (copy ctor from AM_MEDIA_TYPE, destructor, default ctor:
  0x48 bytes, bFixedSizeSamples +0x20 / lSampleSize +0x28 = 1, pUnk +0x3C, pbFormat +0x44). The landed sources call them
  "COM_CopyVariantSafe", "CComHolder_00cb1cc0::Release" (it is the destructor: EH state 0 covers it in 0x00CAC4A0) and
  "CCodecState_00cb1d00"; 0x00CAC4A0 itself is DirectShow base-class code, not CEngineScreenEffectRenderer.
- 0x00669D30 takes the def itself as `this` and reads the PDB CSpecialEffectsDef::SpecialEffectsMap (+0x28): it is a
  CSpecialEffectsDef method (landed as "CCRC::CalcAndLookup" and as "NativeEffects::GetEffectFromString"); this chunk declares
  it `CSpecialEffectsDef::GetEffectFromString(const CCharString&, long&) const`.
- 0x00666D10 CThingCreatureBase::GetCombatTypeDef is right; the field this chunk reads at +0x80 is PDB CCombatTypeDef::FrontNearLimit (+0x84).
- 0x00BF5FE0 is the CArray<pair<C3DBoundingBox, CWaterPatchMesh*> > push_back, not "CScriptModuleTransitionBetweenPoints::OnInitLeaf";
  0x00BF5EA0 is the out-of-line CArray<CWaterPatchMesh*> push_back.
- 0x00661540 AngleBetweenPositions is `__fastcall(const C3DVector& from, const C3DVector& to)` returning float.
