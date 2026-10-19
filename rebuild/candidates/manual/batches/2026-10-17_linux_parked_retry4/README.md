# Parked row retry batch 4

The canonical VC7.1/Wine verifier accepted all three candidates and ran their behavior fixtures successfully: two `MATCH` rows and one `RELOCATION_MATCH`.

- `00aed090 CParticleEmitter::Set2DPosition` — 167-byte exact match from a word view of the shared layer bits and the retail XOR delta.
- `00988490 CShaderRenderManager::SetProjectionTransform` — 173-byte exact match; an inline RAII cleanup emits the final tail OR after the matrix stores.
- `00ad7400 CWound::Kill` — 197-byte relocation match; the embedded tree headers are modeled at `+0x84`, `+0x9c`, and `+0xa8`.

The per-candidate source, test, pass pattern, and retail bytes are preserved in `payload.json` and `oracle.tsv`. The verifier transcript is in `verify.log`. The `00aed090` source uses a local function-level layout because the generated `CParticleEmitter` header places the fields differently from this function's observed `this` offsets; the verifier emitted a shared-header warning, not a rejection.
