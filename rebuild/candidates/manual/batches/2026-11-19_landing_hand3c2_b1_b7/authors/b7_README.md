# b7 (hand3c2, Account B, 2026-11-19): 0x00766660 CTCObjectAugmentations::AttemptToStartParticle -- READY

- Grade: RELOCATION_MATCH at the first grade (g.py + strict vland `RELOCATION_MATCH PASS`, LANDVERIFY_STRICT_HEADERS=1). Purity: genuine.
  ATTEMPTS.log: 1 grade. Flags: none.
- Leaf ?AttemptToStartParticle@CTCObjectAugmentations@@AAEXAAV?$CIntelligentPointer@VCThing@@@@JJABVCCharString@@1@Z (private), 696 bytes, `ret 14h`.

## Identity
Ego_r.pdb / FableWin.pdb; declared by the shared header's FABLE_RETAIL_AUGMENTATION_PARTICLES class (landed siblings
AttachAugmentationParticles 0x007677E0 etc.). Locals and scopes exactly as Ego_d.pdb (debug length 1943).

## What closed it
The October draft (DIFFER(700v696): esi/edi swap, the `or byte ptr [ebp+41h],2` RMW "lane gap") was a local model. The landed
CTCMultiStrike::ApplyEffectOnWeapon 0x007CCF50 (MATCH) holds the same attachment block with the same Ego_d local names; ported onto
its opt-in set (CParticleEmitter Sleep / SetAlwaysUpdate inline -- the RMW comes from the bitfield Sleep(), GetPEmitter inline,
GetTC original chain, IS_THING_ALIVE) + FABLE_RETAIL_CINTELLIGENTPOINTER_COMPARE_NULL_INT (IS_THING_ALIVE on the intelligent pointer
gives retail's two GetPItem calls) + the AUGMENTATION_PARTICLES class. The spline difference is floored with
std::max(distance_between_particle_points, DELTA_NOTIONAL_ZERO) (0x007668B6). No Inferred / FITTED construct.

## Headers
No header change (all existing opt-ins). DELTA_NOTIONAL_ZERO is declared in the row as engine/C2DVector.h declares it (that header's
retail branch would re-pack C2DVector for the TU).

## Test (test/00766660.cpp)
Links the REAL row object (expanded GetTC with the interface-map LowerBound, the CIntelligentPointer<CThing> temporaries and
assignment, IS_THING_ALIVE, std::max, the emitter bit setters). Raw native-layout things (interface bits, a three-entry interface map
with decoy neighbours, the dead bit with a neighbouring bit set), emitters and particle emitters (neighbouring flag bits preserved).
An independent model predicts every engine call (receiver and arguments: helper / dummy lookups with name, index, flags, scale;
Create ids, position and flag; spline lengths in order; InterpolateTo target and factor; AttachToThing arguments; Kill), the pfx item
and that its temporaries are destroyed. 17 cases (each early return, effect 0 / -3 / 1, null and dead effect / blend, the 1e-4 floor
from equal, negative and just-above differences). Prints `00766660_TEST PASS`.

## Mutants (mutants.tsv / mutants.log): 42 / 42 KILLED

## Names (NAME_FIXES.tsv)
0x0129BA3C is ?DELTA_NOTIONAL_ZERO@@3MB (Ego_d carved names + assert strings; a labels table still says g_NormaliseEpsilon).

## Files
src/00766660.cpp, test/00766660.cpp, payload.json, oracle.tsv, mutants.tsv, mutants.log, vland.log, NAME_FIXES.tsv.
