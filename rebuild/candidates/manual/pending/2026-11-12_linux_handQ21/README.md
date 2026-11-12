# Chunk Q21 (the 14 parked near misses chunk Q20 did not reach, Opus 5.5)

1 matched and landed in landing dl: 0x00B39080 CEngineInternalAttachedPrimitive::GetParentTransformAnimatedMesh. It was ported onto
the shared headers; the port alone removed its old one-`lea` residue, then the occlusion-base slot-28 virtual and SetMultiply's
operand order finished it.
2 are ported but still DIFFER: 0x00540320 CEditBox::KeyPressed (three vptr loads with eax/edx swapped; `sub edi,5` vs `add edi,-5`) and
0x007DAEF0 CTCTavernGameCardBlackjack::DealCard (scratch-register rotation in the flip branch).
11 were not ported; NOTES lists what each port needs. Chunk Q22 takes the six smallest.
