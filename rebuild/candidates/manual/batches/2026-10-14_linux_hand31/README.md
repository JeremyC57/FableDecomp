# Batch `2026-10-14_linux_hand31` — coordinator hand-authoring chunk 31 (CEditBox .. CEngine): 32 of 40 landed

Chunk 31 of the class-labelled 17–96-byte pool, authored by the coordinator with the Wine lane: the CEngine statics and
members of the 0x00B2xxxx module (texture-morph cache, cutscene preload through engine/CEngine.h, SetCamera, the
draw-distance factor, the screen colour filter's `rep movsd` copy, SetSeaBodyToRender's by-value CCharString through an
inlined by-value setter, the 2D/3D scene flag toggles, ClipBox through a 48-byte matrix, the two memory-pool getters that
copy a CCountedPointer temporary, the map-state loaded/unloaded/visibility bodies, the rain/snow overrides), the
editor/UI bodies (CEditBox::GetTextComponent's recomputed count loop, the profile-locked atmosphere setters, a theme
display activation, a bitstream flush, two editor constructors, the lip-sync bank loader, a C2DBoxF to C2DBoxI
conversion, CEmitterData::UnloadResource, two marker-vector resizes, a render-setting float store, a critical-section
decrement, the timer-base initialiser with two QueryPerformanceCounter calls).

Label corrections: 009e1cb0 "FlipSelectedTrack" is the free `InitialiseTimerBase()`; 00c14820 / 00c14930 / 00c1e570
"SetAtmosEditingMode" are profile-locked setters/getter on 0x00C1xxxx objects (named by address); 00c911b0 "RemoveMap"
is a bitstream FlushByte; 008239d0 "AddToInterface" is a constructor; 0049ba70 "AddToInterface" is an Initialise
(parked); 0082cac0 is CEditPrimitiveStats' copy constructor; 00c2d260 "Redo" is stack-imbalanced retail code (parked);
00a42ac0 "CEditWorldMap::IsMapChangeable" is CLipSyncDataBank::BeginLoadingEntry; 00a55a30 "GetPenArea" is
C2DBoxF::ToIntBox; 0088aa30 / 00a85f30 / 00c5d700 "CEngine::ClipBox" are a marker-vector resize pair and a
critical-section decrement; 00b401e0 / 00b40230 / 00b42390 are CEngineMapManager (pointer table at +0x20); 00b50ee0 /
00b50f40 are the weather state (floats at +0x12e8..), not CEngine (sizeof 0x188); 00c445c0 "Enable2DSceneRendering" is
a float setter with a dirty bit (the callee of the landed null-object helpers).

Lessons (also in GOTCHAS, wave 24d): a `ret 4` on a CEngine "static" means a non-static member whose `this` is unused;
`fld arg ; fld member ; fucompp` is `member != arg` (operand order from the source); a by-value CCharString argument
destroyed by the callee (`lea ecx,[esp+8] ; call ~`) and copied once first is an inlined by-value setter; the flag store
duplicated into both arms of a float clamp comes AFTER the clamp in the source; `fld [x] ; fdivr [1.0]` (x loaded
first) is not reproduced by `1.0f / x` in any form on the lane; a counted-pointer temporary's add-ref and release
fold into one null test with both count stores kept; a static_cast from a secondary base adds a null test that retail
lacks (use pointer arithmetic); a `__fastcall` helper with the key's address in edx takes by-value quads copied through
ecx only (lane: ecx/edx).

Not landed (best sources in `$S/w22/agent31/differ/`, notes in NOTES.md): 004e1db5 (VC6 tree insert idiom, not attempted),
00540120 (`push eax` vs `mov ecx,esi` order), 007b52b0 (copy register allocation), 006d63a0 (`fdivr` + `jp` layout),
0049ba70 (zero-register coalescing across the call), 00b25d20 / 00b25d50 (copy through esi with the hidden pointer in
edx), 00c2d260 (stack-imbalanced retail body).
