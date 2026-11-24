# 0x00839420 NThingHelpers::CreateEffectOnHit -- working notes (AUTH23, 2026-11-22)
Best so far: `p2_00839420.cpp`, stock flags, no overlay: DIFFER(1316v1308), 29 masked lines (draft r0: 1304v1308, 75 lines). 3 grades used.
## Ego_d decode (egod_lines.txt / egod_locals.txt; VC10 /Od sizes of p2 in od/, tool odall10 --lib fablelib)
Params thing_hit / hit_params / world_map / delta_health; locals pthing_hitting, point_of_hit, attack_direction, pmaterial_def, pcreature_def,
line_out, new_point_of_hit, new_direction_of_hit, perp, phitter, lod_centre, def_man, region, pregion_def, region_def_index, pthing_emitter,
from_effect_id, to_effect_id, interpolate, tcd_particle_emitter, parameter (std::pair<unsigned long, C3DVector>), params
(NParticleEngine::CParticleUpdateParameters<unsigned long, unsigned long>). Lines that p2 matches by size: every line except the ones below.
Decoded by size: 110 = IS_THING_ALIVE(pthing_hitting) && .. (61 B); 117 = `if (line_out.IsNonZero())` (19 B); 127 = `C3DVector perp =
line_out.CrossWithUp();` (12 B); 132 = `new_direction_of_hit = perp * -1.0f;` (44 B); 136 = `new_direction_of_hit = perp * 1.0f;` (38 B);
140 / 141 = the two plain assignments (18 B each); 171 = one statement with the implicit CDefString -> CCharString conversion (77 B).
Still off by size: 87 (Ego_d has ONE 14-B statement, no separate `= 0`; retail has a dead `mov [pthing_hitting],0` at 0x00839430, so p2
keeps the draft's two statements: fitted); 129 (phitter: Ego_d 13 B = a two-argument __fastcall cast such as GFDebugPDynamicCast, p2's
static_cast is 6); 151 and 166 (Ego_d 77 / 52 B, p2's `.IsNull()` / `.Get()` on the by-value CDefPointer temporaries are 97 / 72: both 20 B
more; `.Get()` in an if is 81); 207 (481 B: an assert macro around GetTCDriver); 208 (40 v 42).
## Residue of p2 (29 lines)
1. pmaterial_def / pcreature_def frame slots swapped ([esp+14h] / [esp+10h] in retail), and with them from_effect_id etc.
2. pcreature_def's release is load / dec / store / test in ours, `dec [ecx+4] ; jne` in retail (0x0083968F). On 0x0066B370 the same shape went
   away with the visible GetSubDef body (FABLE_RETAIL_CDEFCLASSBASE_GETSUBDEF_BODY): next step, which needs the port below. The pick-any
   copy GetSubDef<CCreatureDef> (owner 0x00485815, not 16-aligned: probably a size TU) will then need the 0x0066B370 device
   (FABLE_RETAIL_CDEFCLASSBASE_GETSUBDEF_CENTITYSOUNDDEF_SIZE_BODY pattern) and a pk.py pass.
3. (point_of_hit - lod_centre).GetSquaredMagnitude(): retail sums X*X, Z*Z, Y*Y (0x00839736); ours Z, Y, X with the default
   `(X*X+Z*Z+Y*Y)` text. C3DVector.h has _SQUAREDMAGNITUDE_XYZ / _ZYX / _ORIGINAL_EXPR_PASS23 opt-ins: not tried.
## Port still owed (local models in the draft that have shared headers today)
CDefinitionManager / CGameDefinitionManager / CMainGameComponent / CWorld (GetGameLODCentre) / CWorldMap (IsPosInMap, GetLoadedRegion,
GetRegion) / CRegion (FABLE_RETAIL_CREGION, PDef is protected: needs GetPDef) / CCRC.h / ThingAlive.h (FABLE_RETAIL_IS_THING_ALIVE) /
CTCDParticleEmitter.h (FABLE_RETAIL_CTCDPARTICLEEMITTER_FULL: Create overloads, SetEmitterUpdateParameters) /
NParticleEngine_CParticleUpdateParameters.h / CMaterialDef.h (needs an opt-in for GetBlendedSwordHitEffect 0x008EE790,
GetRandomBerserkedSwordHitParticleEffect 0x008EE720, GetRandomSwordHitParticleEffect 0x008EE600). Row 0's source
(`../0066b370/0066b370.cpp`) shows the define set for the def-manager / world headers.
NAME_FIX (in ../NAME_FIXES.tsv; reloc_bindings already binds the symbol at 006e11f0): the draft's free function `CreateScaledEffect` 0x006E11F0 is CTCDParticleEmitter::Create(long, long, float, const C3DVector&,
bool) (engine/CTCDParticleEmitter.h line 128; PDB overload list).
