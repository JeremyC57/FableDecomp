# 00e479b0 final handoff

Final status: **VERIFIED_MATCH**. Selected grade: 73. 

ROW.json and root STATUS.tsv are authoritative. Earlier status claims below are historical; they may have been superseded.

## rows/00e479b0/NAMING_EVIDENCE.md

# 00e479b0 identity and selected residue

Exact original symbol: `?Main@CV_BanditTollScript@NScript@@UAEXXZ`. Both PDB name sets identify this virtual Main; native owner vtable 012e4b7c slot 2 points to 00e479b0. The original owner type is Ego_r TPI 0x10d13; the native base is 0x48 and the complete owner is 0x5c. All state fields and every nested entity member are retained in the shared owner header, with original access. Ten literal/factory pairs are in the accompanying naming evidence JSON.

Both original procedure records name the two locals `ReluctantTraderSpawn` and `SafeTraderSpawn`. Those names are preserved. `CScriptThing::GetPos` is proven by its PDB symbol and native 004aa980 null arm/virtual slot 6; `NullCoord` is the protected static C3DVector at 0143e8e0. Shared original Game declarations provide IsRegionLoaded, NewScriptFrame, GetThingWithScriptName and CreateCreature; their native slots are 12, 7, 72 and 91. The callbacks are CheckForLeaderDead (00e482f0) and ResetToll (00e480d0), independently identified by PDB and thread-name literals. CSpawnedFunc fields and base are original types, not stand-ins.

The complete native span ends in ret at 00e480ca. Selected code compiles to 1803 bytes versus 1819 native. The main body agrees apart from affected exit displacements until two thread construction sites: VC7.1 retains calls to the ordinary inline CSpawnedFunc constructor where native code expands it. Twenty-six conservatively grouped source strategies are retained in PROBES.md; flag diagnostics, combined probes and confirmation builds do not inflate that count. A closer length alone was rejected because its control flow differed more. No stronger inline annotation was added: original PDB contains an out-of-line constructor but does not prove that annotation.

The behavior fixture pastes selected GetPos and Main and checks all termination guards, zero/delayed waits, both creation-suppression flags, null/non-null positions, binding/thread allocation failures, callback identities, order and temporary lifetimes. All 2688 scenarios pass GNU C++03 and VC7.1 /GR-. This validates behavior, not byte identity: the row remains PARKED.

## Current verified result

The previous parked selection is superseded by the proven original authored spawned-function constructor expansion. Original body, type and access were independently checked; stronger keyword is not claimed as recovered text. New Owner conditional preference is documented in evidence/00e479b0_INLINE_RULE_REVIEW.json. Root independently recompiled, verified every actual operand, checked helper/thread/dtor bodies, and reran all2688 scenarios on both compilers. Full1819-byte match. Old selections and failed histories are preserved.
