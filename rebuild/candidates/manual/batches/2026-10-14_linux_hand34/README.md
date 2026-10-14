# Batch `2026-10-14_linux_hand34` — coordinator hand-authoring chunk 34 (CEngineLightingManager .. CEnginePrimitiveManagerDecal): 32 of 40 landed

Chunk 34 of the class-labelled 17–96-byte pool, authored by the coordinator with the Wine lane: the lighting manager's
shadow-scene dispatch, point-light reset loop and light-primitive classifier, the engine manager's component-list
walkers (OnUpdate / OnPausedUpdate / AddMap / Cleanup / OnRender over a sentinel list with virtual IsActive), a map
listener broadcast, CEngineMap's unload notification and CloseStaticMap (an inline counted-pointer release through a
pointer-to-member destroy slot), two persist transfers (a colour, a byte pair through a -1/-1 reader local), a
CPersistContext Transfer pair on a hero-stats block, a local-detail layer-def copy constructor and map creation,
AddChildPrimitiveWithSettings with the 0x00C3xxxx last-error idiom, two record operator= bodies (nested triples,
CWideString / CIntelligentPointer members), the particle-simulation IsBusy loop, the changer / group / decal
CreateInternalPrimitive allocators (non-throwing new, the inlined constructors), the C3DAnimationInterpolator
constructor (two blend states with counted animations), two NCreatureAction event constructors (the words in a
`novtable` base), CEnginePrimitiveManager and the static-meshes manager constructors (a scoped CCharString name),
a flagged two-pass render helper, the animated-meshes RenderSubPrimitive override, and a GetSupportedTypes push_back.

Label corrections: 004595a3 / 004e8fcd "CEngineLightingManager::UpdateShadowScene" are CPersistContext transfers
(a colour; a byte pair); 00578797 "CEngineLocalDetailLayerDef::Transfer" persists "OpenedExperienceSpending" /
"TutorialsPlayed" on a hero-stats block (named by address); 00a3ebf0 "CEngineManager::RemoveMap" is a different
manager (list at +0x4c, owner at +0x44; named by address); 00543530 / 005c08e6 / 005c0c24 / 00c48770
"CEnginePrimitive2DText::operator=" are record copies of game-side structs (named by address; the two 0x005Cxxxx
bodies are copy constructors); 00550a20 "CEnginePrimitiveAnimation ctor" is C3DAnimationInterpolator (vtables
0x01239840 / 0x01239838); 00695400 / 00695690 "CEnginePrimitiveManager ctor" are NCreatureAction::
CActionEventTriggerParticle / CActionEventAddObject (vtables 0x0125C0D8 / 0x0125C178); 006f9b20 / 006f9bb0
"CEnginePrimitiveManager2DAnimatedMeshes::RenderSubPrimitive" are a game-side two-pass helper (named by address);
00ba1a70 is CEnginePrimitiveManagerStaticMeshes' constructor (vtable 0x012A5114); 00b59800's map constructor @
0x00BDEFC0 is labelled "CProjectileWeapon" (propagated); 00bd71b0 is a 28-byte prefix of a larger function (bad
family-scan row, skipped).

Lessons (also in GOTCHAS, wave 24e): a counted block whose destroy is `mov ecx,[eax+8] ; call [eax+4]` holds a
pointer-to-member at +4 (not a vtable); a constructor's trailing store scheduled after a local's destructor needs the
local in an inner scope; a struct member whose words are copied through eax in a constructor is a plain member
(an operator= schedules the `lea` early); an argument-carrying `novtable` intermediate base owns the words stored
before the vtable; a changer's four cleared words are one 16-byte vector object; a `// Leaf:` line in a chunk
source selects the object symbol (the implicit operator= of a modelled struct can be the TU's largest function).

Parked (evidence in the journal / scratch NOTES): 005c08e6 / 005c0c24 (lea scheduling), 00b3a0d0 (`mov ecx,eax`
materialisation), 00bb6520 (a `??_G` without a vtable store: `novtable` suppresses the body's emission), 00be2190
(a dead 16-byte zeroed temporary kept in the frame), 00bc6210 / 00bc6270 (the dead `lea eax,[eax+edi*8]` of a
reference-bound handle; a result byte kept in the frame).
