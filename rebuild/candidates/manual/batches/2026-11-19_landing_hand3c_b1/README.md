# Landing hand3c b1 (2026-11-19): 4 not-plain-C++ particle rows (4,119 bytes)

vland --land, strict headers: RELOCATION_MATCH + PASS (`logs/vland_land.log`); tests also PASS on g++ with FABLE_PORTABLE. Overlay
merged: additive opt-ins in CPSCBase.h, CPSCEmitterGeneric.h, CPSCDecalRenderer.h, CParticleEmitterManager.h, CPSCSplineBase.h, and a
nested opt-in in CParticleSystem.h (`hdr_README.md` has the evidence). Three class headers had one FABLE_STATIC_ASSERT_AT line move down;
the pinned names are unchanged (accepted by the coordinator).

| row | notes |
|---|---|
| 0x00B012E0 NParticleEngine::CPSCRenderMesh ctor (1296) | the "bare fistp" sites are GFPackFloat / GFUnpackFloat calls (FableWin /Od call targets ?GFPackFloat@@YIKMKMMK@Z / ?GFUnpackFloat@@YIMKKMMK@Z): shared engine/GFPackFloat.h with FABLE_RETAIL_GFROUNDFLOATNEAR_GROUP_PRODUCT, as the landed CPSC rows; no asm in the row |
| 0x00B103F0 NParticleEngine::CPSCEmitterGeneric ctor (1463) | same |
| 0x00B1DFD0 NParticleEngine::CPSCDecalRenderer ctor (931) | same; first authored body matched on the first grade |
| 0x00B05C60 NParticleEngine::CPSCSingleSprite::GetComponentCost (429) | FableWin: `else if (FaceMe2D) 2 else 2` (the test folds, its `mov al,[esi+93h]` load stays); ScaleParticleDimensions(float&) as a header inline under FABLE_RETAIL_CPARTICLESYSTEM_SCALEPARTICLEDIMENSIONS_FLOAT_INLINE (a COMDAT VC7.1 declines to inline, as retail) |
