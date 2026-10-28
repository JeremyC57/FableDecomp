# Retry chunk 536 (agent536, 2026-10-28): 5 of 18 parked rows land (all RELOCATION_MATCH), 13 re-parked

| Address | Function | What fixed it |
|---|---|---|
| 0x00B9DFC0 | CEngineInternalPrimitive2DStaticMesh::Update | polymorphic `this` model; inline PeekMeshBank and by-value GetBoundingSphere (FableWin PDB) |
| 0x00B812F0 | CEngineClothSimulationStaticMesh::Render | same-TU CalculateFadeOut defined first; real callee names (PrepareForRendering 0x00AE1600) |
| 0x0054F6E0 | NUISystem::CText::~CText | real callee and vtable spellings (LoadAndReturnDef, ReleaseFont, ~CBaseClass, three CText vtables) |
| 0x008C9A40 | `anonymous`::AttachEffectToStaff | the real 12-byte STLport CVectorMap with LowerBound 0x0040F020 |
| 0x00924520 | `anonymous`::CCreatureAction_KrakenLowAttackLoop::FinishAction | GetTC key as a temporary, `bool Dead:1`, CTCKraken::EndAttackBeam 0x007FC800 |

Re-parked (pending/2026-10-27_linux_hand536/differ, residue and next step on line 2 of each file): 0x0070D580
CAnimateThing::SetMainAnim is now 504v504 (register roles only); 12 more unchanged (x87 operand order, frame-slot order,
register roles, one vptr-store lane gap at 0x00C11D00).

Label corrections from this chunk: 0x00947110 FindPrettyThingInShop; 0x00B43720 / 0x00B43870 CEngineMap::PeekThemeId /
PeekThemeBlend; 0x00C11D00 the CSoundEventManager ctor (vtable 0x012B2748); 0x007FC800 CTCKraken::EndAttackBeam.
NAME_FIXES.tsv: 26 rows, all deferred to the naming pass (each touches many landed callers).

Review at landing: the anonymous-namespace `?A0x` hashes on 0x00924520 and 0x008C9A40; 0x0129BA3C keeps the bound
descriptive spelling `g_epsilon_0129ba3c`; SHARED_HEADER_AVAILABLE warnings on all five rows (header debt).
