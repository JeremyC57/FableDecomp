# NParticleEngine::CPSCRenderSprite::AddToBoundingSphere

The VC7.1 object exports `?AddToBoundingSphere@CPSCRenderSprite@NParticleEngine@@UAE_NAAVC3DBoundingSphere@@MAAVCParticleSystem@2@ABV?$CParticleUpdateParameters@KK@2@@Z`. The source-order record in Ego_d identifies `NParticleEngine::CPSCRenderSprite::AddToBoundingSphere`. The submitted extent is 2405 bytes.

- egor donor procedure at 00abc8e0
- fablewin donor procedure at 02f27160

- abs .rdata at 0x0129F6C8 (a data reference: in .rdata usually a vtable slot, see names/rtti-vtables.tsv; in .text an EH or jump table)

The accepted inline split removes the extra IsActive, GetRecordedPos and TransformPosIfLocal calls. The receiver allocation and frame differ from the prologue onward.

Checks empty and expired particles, sprite and trail combinations, both delay ranges, timed animation, phase clamps, transformed positions, computed radii, and handle clears.
