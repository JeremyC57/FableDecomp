# Identity and naming evidence

Compiled leaf: `?Update@CPSCUpdateNormal@NParticleEngine@@UAEXAAVCParticleSystem@2@ABV?$CParticleUpdateParameters@KK@2@AA_N@Z`. The final object symbol table is checked against this exact spelling.

- Ego_r name-set entry: `00ace770	?Update@CPSCUpdateNormal@NParticleEngine@@UAEXAAVCParticleSystem@2@ABV?$CParticleUpdateParameters@KK@2@AA_N@Z`.
- FableWin name-set entry: `02f3f4c0	?Update@CPSCUpdateNormal@NParticleEngine@@UAEXAAVCParticleSystem@2@ABV?$CParticleUpdateParameters@KK@2@AA_N@Z`.

The corresponding named FableWin donor at 02F3F4C0 and native vtable 0129FC1C slot 9 establish the receiver and operation. Both full PDB type records establish the original particle, system, quaternion, matrix, intelligent-pointer and random-acceleration models. See the included type-evidence files and shared-header comments for offsets and signatures.

The row uses the real NParticleEngine types and STLport list, not the inherited local replacement models. Original local_acceleration and the original system_accelecration spelling were restored. The iterator local alone is descriptively spelled emitterIterator to satisfy the explicit forbidden-substring rule; it carries no ABI symbol. Its original spelling is recorded in the PDB local evidence. GetSystemTransform's internal emitter-transform accessor is the original private PeekEmitterTransform; the wrong inherited binding is reported in NAME_FIXES.tsv.

The true selected extent is 5602 bytes; the assigned 5616 includes alignment after the function. Selected source emits 5604 bytes and remains PARKED. The 29-scenario fixture copies the complete Update body but substitutes deterministic math/environment dependencies. It does not independently certify all quaternion/math helper behavior. The native inactive-particle iterator path is deliberately not exercised because it does not advance; the active-list invariant is documented rather than silently fixed. No complete exact-byte relocation-match claim is made for this row.
