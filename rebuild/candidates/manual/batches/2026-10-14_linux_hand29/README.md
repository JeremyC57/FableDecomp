# Batch `2026-10-14_linux_hand29` — coordinator hand-authoring chunk 29 (CDisplayManager .. CDrawMiniMap): 34 of 40 landed

Chunk 29 of the class-labelled 17–96-byte pool, authored by the coordinator with the Wine lane: CDisplayManager
(the virtual-screen box from two double globals, PostDeviceReset over the VC6 callback list), the "SetView" trio
that is really three free `__stdcall` null-object-call helpers of the 0x00C3xxxx module (`accepts ? object : 0`
as the call target, one negated), the NUISystem three-vtable constructors (CChangingStateComponent with its
self-linked sentinel list, CParticleEffect, CDraggableInto), the NPlayerGui size-optimised drawers (CDrawBase's
constructor with push_back's temporary homed in the dead argument slot, the two ability-button mappings, the
quick-access / ability-button PC constructors, CDrawDamage::Update's tail-merged `erase`/`next` loop,
CDrawGuildSeal SetEnabled / GetAlpha, the highlighting cursor's constructor and Clear, the melee-effects
constructor and ClearPrimitivesIfNecessary, a second CDrawBase-derived constructor by vtable), the memory
allocator destructor, three "clear all" loops, a skill-property setter, a tree lower_bound test, a profile-locked
flush, a drag descriptor dispatch and a singleton activation with a sibling-call through a callback pointer.

Label corrections: 00640050 is the only real CDisplayViewManager::SetView; 00c34050 / 00c34180 / 00c35f20 are free
`__stdcall` helpers (Enable2DSceneRenderingOnAcceptingObject / SetHeadTurnStatesOnAcceptingObject / ...OnRejectingObject);
0052cc50 / 00544010 / 00558ec0 "CDraggableInto" are NUISystem::CChangingStateComponent / CParticleEffect / CDraggableInto
(rtti-vtables.tsv); 00a65690 "CDrawGuildSeal::ClearMessages" is ~CMemoryAllocatorVariableSize (vtable 0x0129DB1C);
00c0ec60 / 00c4b010 / 00c60410 are three unrelated clear loops named by address; 00657d60 "CDrawBettingHUD" is
NPlayerGui::CDrawQuickAccessIconsPC (vtable 0x0125A804); 0064ed06 "CDrawMeleeActionButtonEffects" is a sibling drawer
(vtable 0x0125A670, landed as CVtable0125a670); 00ac7fc0 "CopyBackBufferToTexture", 00c437c0 "DestroyDevice",
00c20340 / 00c279a0 "ClearPrimitivesIfNecessary", 00c1d880 "CDrawAutoPickups", 00c3d090 and 00c67470 are named by
address. The 00c3d090 chunk row over-captured the sibling setter at 0x00C3D0C0; its oracle was trimmed to 35 bytes.

Lessons (also in GOTCHAS, wave 24c): a non-polymorphic base's call site `lea ecx,[esi+4]` means the model put the
vptr in the wrong class (give the base the virtuals); a size-TU bool return through the eax zero register is a long
result (`xor eax,eax ; inc eax`); a size TU does not inline a counted/def-pointer temporary's functions -- write the
release out; a by-value key whose slot address is passed on (`lea eax,[esp+0Ch]`) vs a reference (`mov eax,[esp+4]`);
`cmp value,member` is `value != member` (operand order follows the source); the member pointer used as `this` of
the next call goes through an inline accessor to land in ecx; a CRGBColour stored B, G, R, A has a constructor
with parameters in that order; the free-function form of the null-object call (`T* t = o->Accepts() ? o : 0;
t->Set(x);`) is the whole 0x00C3xxxx family.

Not landed (best sources in `$S/w22/agent29/differ/`, notes in NOTES.md): 00c20cb0 (`this` kept in ecx across a
call: whole-program codegen), 006472b2 (bare `fistp` rounding = inline-asm helper in the original), 00643e09
(eax vs al zero register), 00c437c0 (8 dead frame bytes), 00644334 / 006443ac (`mov ecx,ebx` scheduling).
