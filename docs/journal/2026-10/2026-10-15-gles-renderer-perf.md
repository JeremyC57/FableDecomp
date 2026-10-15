# 2026-10-15 — GLES renderer: per-draw GL traffic, tiler-friendly clears, 16-bit DXT

Target: the RG34XX SP (Mali-G31 MP2, 4x A53, 2 GB shared, 720x480). Last device log (v15):
~41 ms gameplay frames, the game thread the whole frame, the render thread (Mali driver
CPU) 27 ms of it hidden, swap 0 (the GPU kept up). So the renderer's per-draw call count
matters on both threads; GPU-side wins are headroom (and heat) under the 30 fps cap.

Evidence used: the 8000-frame D3D census (`FABLE_D3D_CENSUS`, menus + gameplay):
960k DrawIndexedPrimitive, 4.16M SetVertexShaderConstantF (4.3 per draw, registers up to
c96), 176k SetVertexShader, ~100-200k toggles each of z-write, alpha blend, colour write,
src/dest blend, 37k alpha-test toggles, 314k SetSamplerState; 1320 of 1571 textures DXT1
(858) / DXT3 (455) / DXT5 (7); no D3DUSAGE_DYNAMIC buffers at all (managed WRITEONLY);
Clear 0x7 with the full viewport once a frame, never with rectangles.

## Changes (all in `rebuild/recomp/posix/gles/`, compile-checked for aarch64, not yet run)
- **Uniforms per program, only on change.** GL keeps uniforms per program object; the
  renderer re-sent ~9 state-mirroring uniforms whenever any render state changed (nearly
  every draw) and all 128 constant registers on every program switch. Registers now carry
  a version bumped only when the value changes (identical re-sets are free); each program
  records what it was sent and gets only the differing runs; fog / alpha / bump / viewport /
  texture-factor / colour-swizzle uniforms are compared with the program's copy. A
  randomized model test (12 programs, truncated active sizes, 200k draws) found no stale
  register and ~32 registers sent per draw.
- **Sampler parameters** compared with what each sampler object holds (one D3D sampler
  state change re-sent all five; the compare mode is the object default and is not sent).
- **Program builds in one round trip.** Each `glGetUniformLocation` etc. was a queue round
  trip: ~158 per new program (a mid-frame hitch on first use). The FFP compile, the link and
  all queries now run on the render thread as one call; shader creation 2 -> 1.
- **Whole-target Clear without the scissor** (same-size colour/depth only), so the tiler
  sees a full-attachment clear (no tile load).
- **Alpha test as a shader variant.** Every fragment shader ended in a uniform-guarded
  `discard`; on Mali a shader that can discard loses early depth update and forward pixel
  kill for every draw. `FABLE_ALPHA_TEST` is now compiled in only for draws with the test on
  (program key bit); all 635 census shaders and both variants pass glslangValidator.
- **No periodic readback**: the 120-frame log line read the centre pixel (a GPU drain);
  now only with `FABLE_GLES_PROBE=1`. The line also counts buffer uploads (in place / bytes).
- **Fix: stale texture binding.** Creation/upload/mip generation bound textures on the
  active unit without telling the binding tracker: the next draw keeping its texture on that
  unit sampled the uploaded one. Now recorded (`bindForUpdate`).
- **Word-wise hash** for the VAO key (every layout change) and the small-level re-upload
  check (5.4x faster on x86, ideal bucket spread).
- **16-bit DXT** (texture detail Low, or `FABLE_GLES_TEX16=1|2|0`): decoded levels stored as
  RGB565 (opaque; endpoints exact, interpolants within 5/255) or RGB5_A1 (0/255 alpha),
  smooth alpha stays RGBA8 (mode 2: RGBA4). Storage chosen at the first upload; a level
  needing more re-creates the texture and re-uploads from the CPU copies.

## Next device log: what to read
- `frame ... gl ... render thread ...`: the render thread's share should drop (fewer calls
  per draw); `buffer uploads N (M in place, K KB)` decides whether managed buffers re-locked
  per frame need a ring / orphaning path.
- Visual check before turning `FABLE_GLES_TEX16=1` on in `Fable.sh` by default.

## Not done (recommendations)
- ETC2 transcode of DXT (same memory as DXT, 4-8x less bandwidth than RGBA8): the big GPU
  lever, but a real-time encoder on the A53 and a quality check on the device.
- Trim the 11 vec4 varyings every vertex shader writes to what the paired fragment shader
  reads, if the Mali linker does not already drop them (check with Mali Offline Compiler).
- MSAA through `EXT_multisampled_render_to_texture` (on-tile resolve) instead of
  multisampled renderbuffers + blit (AA is off on the handheld).
- Render the last pass straight into the window (no present blit) — touches the y-flip
  conventions; a full-screen copy a frame.
- The queue's per-call mutex (`g_producer`, ~10k lock pairs a frame) could become a
  spinlock or a per-thread ring.

## v24 on the device (user log 2026-10-15) and the next round

- Render thread per draw (world median) 35.1 us against 40.7-43.4 us in v18-v23 (-15..18%);
  the same first view after load (663 draws) 20.5 ms against 25.2 (v23) / 29.2 (v22b).
  `recomp_lookup` is gone from the profile (3.3-7.1% before: the trap-range dispatch); the
  runtime module fell from 7-12.6% to 3-5%. The title screen is now vsync-bound (16.7 ms).
- The world is still game-thread bound in every version (`waited 0.0`, render thread 11-24
  ms against 25-47 ms frames); v24's log has too little world time to measure world fps.
- The profiler's `runtime!com::QueryInterfac` (4.1%) is the stripped runtime's nearest symbol
  for the whole guest-to-host call path (D3D/DInput/DSound wrappers, the trap dispatcher, the
  guest heap, the CRT imports), not QueryInterface. libc +0x81d6c/+0x82040/+0x820b8 are futex
  waits / contended mutex lock and unlock; the condition-variable broadcasts come from the
  Win32 layer's single global condition variable (every SetEvent wakes every waiter) and the
  audio callback mixing under `audioLock`.
- **The inventory screens ran 35-40 ms per frame**: menu_nav.cpp's `safeRead` made two system
  calls per guest read (process_vm_readv + getpid: 16% of the menu window) and the clothing
  labels, the list shift and the row markers walk parents and children every frame. safeRead
  now copies directly when the POSIX layer's page table says the pages are committed
  (`w32::guestReadable`), the checked read only otherwise; the label diagnostic string is built
  only when it is logged.
- The render thread read the clock twice per replayed GL call and on every idle-spin poll
  (clock_gettime 1.3-2.1%): now once per burst and every 64 polls.
- Next (ranked): the game thread's fixed cost per D3D call (inline caches holding trap
  handlers, a lock-free GL queue producer, the per-draw `GlTime`), static x87 TOP tracking in
  the lifter (the world-fps lever), per-object waits in the Win32 layer and mixing audio
  outside its lock (loading), a plain mutex for the guest heap; profiler symbol names for the
  host path.
- 24-bit colour: nothing to gain. The window surface is 32 bpp either way (Mali has no packed
  24-bit format, stride 2880 at 720 px), the present pass writes alpha 1, the main target is
  RGBA8 + D24S8; RGB565 would save ~40 MB/s at the cost of banding while the GPU is idle
  (world swap 0.1-0.2 ms).
