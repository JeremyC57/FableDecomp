# 2026-11-12 landing dy -- chunk Q28 second pass: Create2DEmitter matched

Landed (RELOCATION_MATCH, test PASS under VC7.1, LANDVERIFY_STRICT_HEADERS=1): 0x00AEB790
NParticleEngine::CParticleEmitterManager::Create2DEmitter. It is the first row closed under the owner's no-switching rule: the Q28
agent was sent back to its five differ rows, matched this one, and ran out of budget on the next.

Headers (only the five the row uses; additions only, hdrcheck2 default=same, macros new): CParticleEmitter.h CREATE2D / GETPRIORITY,
CParticleEmitterManager.h CREATE2D, CParticleEmitterList.h INSERT, CParticleEmitterDatabase.h GETEMITTERTEMPLATE,
C3DOrientationInterpolator.h IMPLICIT_DTOR.
The row defines CParticleEmitterList::Insert (landed 0x00AEB060) out of line in its TU with the real body. Keep the two identical.
Checks: reserved_check 0; bindings_multi_check at baseline; funclet_includes_check 1317 checked, 0 broken.
