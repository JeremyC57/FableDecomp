# Hand chunk 174 (CThingFilter_In2DRangeLimit .. CThingPhysicalSwitch): 30 of 40 byte-exact

## What matched (30)
- CThingFilter_In2DRangeLimit 0x6A7700: min/max XY range test; each range test is a file-static inline helper over two C3DVector refs (a member helper lets VC7.1 CSE the deltas through the frame); `return !A && B;` gives the int tails.
- CThingFilter_InArea 0x6A7990: point-in-C2DBoxF via inline `Contains(const C2DVector&)` on a by-value C2DVector (x stays on the FPU, y through the frame).
- CThingFilter_IsAvailableForCreatureInteraction 0x6A7430: one `&&` return expression (both-children-or-both-adults, UID differs, IsAvailableForInteractionType) materialises every exit as an int.
- CThingFilter_IsAwareOfThing 0x6A8630 / IsTargetable 0x6A84F0: HasTC + GetTC with the component a named local; `!= 0.0f` gives `fucompp ; test ah,44h ; jnp`.
- CThingFilter_IsNotList 0x6A7EB0: two arms, count taken into an int local once, `*(begin()+i)` index loops.
- CThingFilter_IsValidGossipee ctor 0x90C870: two out-parameter `__forceinline` GetInterface lookups (HasTC+GetTC picks the wrong registers).
- CThingFilter_IsValidTargetForPlayerCreature ctor 0x805890: CCharString named local in its own block, result into a bool local tested after the block (bl across the dtor, `lea ecx ; push ecx`).
- CThingFilter_IsWithinArea 0x7FD830: non-POD C3DVector (fastcall hidden result in ecx); `X*X + Y*Y` source order emits the Y term first.
- CThingFilter_OverlappingSphere 0x6689E0, PosInContainmentVolume 0x6A7B20, CThingFindHeightZBelowAt 0x668AB0: physics bounding/mesh queries; mesh object a named local destroyed on each return.
- CThingGameObject::UpdateStandingOnThing 0x838AA0: early returns, GetPItem re-called per use.
- CThingHolySite::ConstructFromParams 0x83BAB0, CThingObject::ConstructFromParams 0x703050: base call with "" named CCharString in a block + bool local; `MaxHealth = Health = def->Health` gives `fst ; fstp`; `playerStart != 0` gives `setne`.
- CThingObject::IsFrameUpdated 0x7040E0: bool local from an out-parameter GetInterface (`mov eax,[eax+4] ; mov ecx,[eax+10h]`), then one `||` return.
- CThingMarker::DrawGetEditDrawInfo 0x83D070: one full expression `CCharString("") + "Marker - " + ScriptName` (temporaries in dead argument slots) + inline colour Set.
- CThingSwitch::SerialiseGameState 0x83C230: as the landed CThingPhysicalSwitch sibling, float defaults as `0.0f` temporaries.
- CThingManager (9): Remove/AddToUpdateList, RemoveFromRenderList (intrusive lists, list a named pointer); UpdateDeaths / DeleteAllThings / DeleteThingsWithSectionName (STLport list walk, inline `m_dead ? false : true` gives `not al ; test al,1`, inline CCharString ==, inline STLport clear); ActivateAfterLoading (progress CCharString a named block local); RenderUpdateThings (`for (long i = 1; i < 16; ++i)` strength-reduced).
- NThingMap::CThingMap::GetAllThings 0x85D010: three nested loops; the unused pair<iterator,bool> insert result.
- NThingMap::CThingMapManager DestroyMapwhoNode 0x637A10 / InitPos 0x637DA0 / Init 0x638540: TL,TR,BR,BL node order; signed `/128`; inline box GetWidth()/GetHeight() on each GetBounds() result.

## Parked for Fable 5.1 (9)
- 008091a0 CThingFilter_IsTargetableBehind::operator() DIFFER(104v101): flag test `mov ecx,[eax+4] ; mov dl,[ecx+0Ch] ; mov al,1 ; test al,dl` + separate true exit vs retail `mov eax,[eax+4] ; test byte ptr [eax+0Ch],1 ; je` | tried: dword & 1, nested if, unsigned long bit field, __forceinline GetTC | next: out-parameter GetInterface or a CTCTargeted inline bool accessor
- 007f6d70 CThingFilter_IsValidAttackTargetForCreature::ctor DIFFER(134v125): sub-def release re-reads the out slot after `dec [eax+4]` vs retail one ecx + `dec [ecx+4] ; jne` | tried: raw pointer local, holder dtor, CDefPointer-like out with dtor, Release() member | next: lane gap: CDefPointer release that does not re-read the escaped local
- 008077b0 CThingFilter_ViableProjectileAimingTarget::operator() DIFFER(132v114): same escaped out-slot release (both arms) | tried: null-testing CDefPointer dtor, register copy in dtor, Release(), copied holder | next: lane gap: CDefPointer release re-read (same as 0x7F6D70)
- 00839080 CThingGameObject::OnCollision DIFFER(125v123): looked-up component lands in eax and moves to ecx after the arg loads vs retail `mov ecx,[eax+4] ; mov eax,[ecx]` | tried: __forceinline GetTC, map Find member with by-ref LowerBound, Find with key local | next: lane gap: looked-up component loaded straight into ecx
- 00520c80 CThingManager::OnThingKill DIFFER(112v113): section text re-reads SectionName and goes to edx with the empty arm first vs retail `jne ; mov eax,offset empty ; jmp ; mov eax,[eax] ; ... mov edx,eax` | tried: ternary/if accessors, local order | next: a single inline accessor returning both, or GetString through a CCharStringData* local
- 006378e0 CThingMapManager::GetThingMapwhoLevel DIFFER(105v106): bare dword `fistp [ebp+8]` ceil helper | tried: (long) cast helper | next: lane gap: bare dword fistp
- 00703150 CThingObject::PeekPMaterial DIFFER(111v105): local zero store before the pushes and copy-out re-reads the local vs retail merged `inc edx ; mov eax,edx ; dec eax` in ecx | tried: AddRef/Release members, /Oa | next: lane gap: temporary CDefPointer zero-store placement / escaped-local re-read
- 006a4e90 CThingPhysical::DrawGetEditDrawInfo DIFFER(115v115): register/scheduling only (PhysicsTC into ecx before the pushes, copy through edx) | tried: inline operator=, user copy ctor | next: scheduling artefact
- 006a68f0 CThingPhysical::ModifyHealth DIFFER(156v159): bare dword `fistp`/`fild` round-to-nearest helper | tried: (long) cast helpers | next: lane gap: bare dword fistp

## Skipped
- 0043e430 CThingFilter_IsValid::operator(): the row is a `jmp [eax+4]` thunk followed by int3 padding and four more bodies (several functions).

## Label corrections
- 0x0083C230 is CThingSwitch::SerialiseGameState (vtable 0x012453FC slot 3), not CThingPhysicalSwitch (that one is the landed 0x0083D940).
- 0x00703150 "CThingObject::vfunc_58" is CThingObject::PeekPMaterial() const; 0x007040E0 "vfunc_44" is CThingObject::IsFrameUpdated() const.
- 0x007EA280 "CScriptBase::IsActiveThreadTerminating" is CTCTargeted::IsTargetable; 0x007EE540 "IS_THING_ALIVE<...>" is the CTCVillageMember village getter.
- 0x004C73D0 "CVertexBufferWin32::DoSizeof" is CThing::GetPosition; 0x0049D150 "CTCPhysicsStandard::GetFacingVector" is the physics position-at-time lerp.
- 0x00449970 "Catch@0040d959" is CPlayerManager::GetMainPlayer (const); 0x00487DC0 "_Cons_val<...>" is CPlayer::GetHeroThing; 0x004C9840 "CBaseObject::OnPropertyChanged" is CThing::RemoveTC(long).
- 0x004C7E30 "CThing::GetTypeName" is a bool type-info flag getter; 0x006380C0 "CShaderResource::CloseShaderBank" is the CThingMapManager game-map clear; 0x00639810 is the game-map cell vector resize; 0x0085E140 "FindInsertPosition" is set<CTCMapwho*>::insert.
- 0x00410620 "Transfer<signed char>" is Transfer<float>(name, float*, const float&); 0x0066C340 / 0x0042B040 / 0x0083BD10 GetDef<T> labels name propagated instantiations (CMaterialDef / CThingObjectDef / CThingHolySiteDef here).
