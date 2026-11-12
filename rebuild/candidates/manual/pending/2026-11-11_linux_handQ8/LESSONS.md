# Chunk Q8 lessons (GOTCHAS-ready one-liners)

- (chunk Q8) Redundant parentheses fixed a push_back schedule: in 0x009F5690 CInputEventPackage::CInputEventPackage(CAFile&),
  `event.StartTime = (float)((GFGetTime() - age));` keeps retail's memory-form `cmp ecx,[esi+8]` (0x009F5746) ahead of the
  StartTime fstp; unparenthesised, VC7.1 splits it into `mov eax,[esi+8] ; cmp ecx,eax`. First grade with the PAREN lever.
- (chunk Q8) The PAREN lever also works on inline helper bodies when the residue is a region cut: 0x006A4FB0
  CThingPhysical::GetFirstCollision matched only with GFFastOneOverSqrt's Newton step fully grouped,
  `((3.0f-((r*r)*value))*r)*0.5f` (new opt-in FABLE_RETAIL_GFFASTONEOVERSQRT_NEWTON_GROUPED). One extra pair moved the epilogue
  pops two slots, two pairs one slot short, three pairs exact (0x006A51F1 `pop edi` ahead of `fmul [eax]`). Count the slots and
  add one pair per slot.
- (chunk Q8) The same row needed FABLE_RETAIL_C3DVECTOR_NORMALISE_SQUARE_HELPER: the GFSqr(X)+GFSqr(Y)+GFSqr(Z) Normalise loads X, Y,
  Z in order (0x006A5181..86); the default GetSquaredMagnitude body loads Z, Y, X.
- (chunk Q8) Rewriting an old local-model draft onto the shared headers can by itself remove the residue the draft could not:
  0x0095D0E0 (283v285 for 15+ local spellings) matched on the third shared-header grade. The lever there: build the loop's
  filter from the member, `while (events->GetEvent(&shot, CIsEventCreatedAfter(TimeOutCounter))) TimeOutCounter = shot->WorldFrame;`
  -- VC7.1 forwards the just-stored frame (`mov edx,[ecx+4] ; mov eax,edx`, 0x0095D1A4) and stores the filter after the pushes;
  a separate local copy coalesces into one register, and reading shot->WorldFrame twice reloads it.
- (chunk Q8) `long frames = GetWorld().GetFrame() - TimeOutCounter; if (frames > ...)` (named local) keeps retail's register-form
  `mov esi,[edi+24h] ; ... ; sub eax,esi` (0x0095D1D1); the one-expression test gives `sub eax,[edi+24h]`.
- (chunk Q8) prior/<addr>_lastdraft.cpp was a behaviour test, not the source, for 8 rows (0x00989830, 0x006D8980, 0x007F68B0,
  0x00918A40, 0x009F5690, 0x0095D0E0, 0x0093D270, 0x00A76770): take the newest rebuild/candidates/manual/pending/*/differ/<addr>.cpp.
- (chunk Q8) `// Flags: none` in old drafts is passed to cl as a file name by gr-style scripts that read the line verbatim (0x0093D270
  graded 305v295 that way); strip it.
- (chunk Q8) 0x00989830 CShaderRenderManager::InitialiseLight on the shared header: the attenuation slot written block-first
  (`float* attenuation = &LightAttenuations[((index-1)/4)*2].X; attenuation[(index-1)%4] = ...`) recomputes `lea [ebx-1]` like
  retail but hoists the Attenuation2 load; the component local first fixes the order but CSEs the lea (98v99). Unresolved.
