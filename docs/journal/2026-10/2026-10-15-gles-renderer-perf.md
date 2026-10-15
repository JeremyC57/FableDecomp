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

## v26: the round from the towns/fights/cutscenes log (527c3e3b, v25)

The log: towns, Greatwood, fights, cutscenes; no crash. World frames p50 35-59 ms with
600-1100 draws, `waited 0.00` (the game thread sets the frame), render thread 22-35 ms.
Process CPU over 100k samples: guest 45%, libmali 25%, libc 13%, host 10%, runtime 4%.
Eight investigations ran in parallel, each patch adversarially reviewed (several twice);
everything below was reviewed, nothing was measured on the device yet.

- **Device::dropVaos was 2.4% of all CPU, 5-8% in the 45-55 ms town windows** (the v25
  FableRecomp+0x387d8e8.. samples): every buffer, declaration or shader release walked the whole
  VAO map. Now each buffer lists its VAOs (noted once per index buffer; amortised, no compaction
  cliff); VAOs are shared across programs (explicit attribute locations) and keyed by
  declaration/input content; VAO names come from the render thread in batches of 1024; LRU
  eviction at 4096. Queue: producer/consumer counters on separate cache lines, cached positions,
  a futex producer lock (spin then sleep); constant uploads merged across gaps of 16; sampler
  objects cached per parameter set; the program looked up only when the textured-stage mask
  changes; whole-buffer relocks orphaned only when a draw used the buffer. Switches:
  FABLE_GLES_{VAO_INDEX,VAO_SHARE,VAO_CONTENT,VAO_EVICT,ARRAYBUF,TEXMASK,UNIFORM_GAP,
  SAMPLER_CACHE,ORPHAN_WHOLE,QUEUE_FUTEX,QUEUE_CACHE}=0. Frame line: GL calls per type, VAOs.
- **Shader programs compiled mid-game cost 40-55 ms each** (4 -> 115 programs in a session,
  frames up to 858 ms): a persistent program-binary cache, `save/gles_programs.bin` (SHA-256
  of the GLSL, driver-keyed header, per-record checksums, LRU to 1024, read on a cache thread,
  published before any rewrite, programs of the last 3 sessions pre-linked in <= 2 ms slices per
  frame, the rest loaded from binary at first draw); shaders compile lazily at first use with
  the old fallbacks. `gles: long frame` line for every frame >= 100 ms, classifying it.
- **FlushFileBuffers was an fsync on the SD card on every guest seek and close** (CDiskFileWin32
  Close/SetPosition/MovePosition all flush): no-op for handles that never wrote, otherwise a
  coalesced background fsync per inode, drained (tickets: only what was requested before) at
  ExitProcess, the Back+Start quit and the overlay exit. FABLE_SYNC_FLUSH=1 restores it.
- **CResourceList::GetUnloadableMemory walked every resident texture each frame** (two
  GetLevelDesc COM calls per frame, 1.1% + the trap path) for a sum nothing reads (AvailableMemory
  +0x278 is write-only; budgets come from the engine pools): `--hook 0x9FC070=host_unloadable_memory`.
- **The top "graphics" guest functions were audio** (labels wrong): 0xC78A10/0xC79440 are the
  software DirectSound mixer's MMX HRTF FIR (25 taps per ear per 128-sample block, every 3D
  voice: no hardware 3D buffers), 0xC9C2F0.. are libvorbis 1.0.1's MDCT. Native replacements
  (NEON FIR, bit-exact; the butterfly in the same double order), verified against the lifted
  code on the first 64 calls each on the device (FABLE_DSP_VERIFY, FABLE_DSP_HOST=0).
- **Host overhead**: the guest's calls to host traps (every D3D9/DSound COM call) never hit the
  per-site inline cache (65536 reserved slots, `recomp_host_handler`); the Win32 layer's global
  condition variable became per-waiter wake-ups, critical sections and SRW locks futex locks;
  QPC/GetTickCount/timeGetTime/rdtsc read CNTVCT_EL0, anchored to CLOCK_MONOTONIC every 64 ms
  (seqlock, never backwards: a re-base bound added after review for chrony-size slews), the fps
  cap keeps clock_gettime. Profiler: host symbols from `FableRecomp.syms` (packaged), the guest
  range bounded by the next host symbol, per-thread CPU lines, library time by caller.
- **Lifter: the x87 stack in C locals** (static depth per function, written back where the GPRs
  are spilled) and `_ftol2` inlined at 1,993 sites: A53 instructions -26..-55% on the hot
  animation/particle/frustum functions (CalcBlendedBoneTransformation 5224 -> 2972); old vs new
  differential over all 5,940 x87 functions identical but NaN signs. `--no-x87-locals`,
  `--no-ftol2-inline` give the old output byte for byte.
- **Build**: lld with `-ffunction-sections` and a hot-symbol ordering file from the device
  profile (rg34xx/hot_symbols.txt, gen_hot_order.py); hot text on transparent huge pages when
  the kernel has them (posix/codelayout.cpp, FABLE_HUGE_TEXT); **the Cortex-A53 errata 843419
  linker fix was missing** (clang does not pass it; the H700 is r0p4): on now with
  -mfix-cortex-a53-835769; `-mno-outline-atomics` (no LSE on the A53). PGO plumbing
  (FABLE_PGO=gen / <profdata>, rg34xx/pgo_train.sh) not yet run. FABLE_PMU=60 (opt-in) logs IPC
  and I-side stalls of the busiest thread.
- **Settings**: the device was already at the lowest preset; new installs get it plus 80% draw
  distance; the host menu gains 2D distortion and character LOD (retail defaults).
- **Crash reports were never true on the device**: SDL2's evdev keyboard handler took every
  SIGSEGV, restored ours and re-raised it, so each report showed libc's raise() and "reading
  address <our pid>" (0xA45..0xB42 were pids). The crash handler now goes back on top of SDL's
  (and passes the fault on), reads the kernel's recorded fault address for sent signals, logs
  the host registers and names the lifted function (wrap `_orig` bodies and host code included,
  approximate where it can't tell). The v17/v25 animation crash is in
  C3DAnimation2::CalcLocalTransformations' bone loop (stack-pointer arithmetic): both it and
  CalcLocalTransformationsRelative are now guarded (`--wrap 0xA4CEC0/0xA4D770`): an unreadable
  instance/track/key/output skips the mesh for that frame (identity or bind pose) and logs which
  pointer, once.
- **Overlay input**: buttons, triggers and sticks held when the host menu closes stay hidden from
  the game until released; Back+Start is held back 120 ms alone so the menu combo never reaches
  the game (FABLE_OVERLAY_SOLO_MS).

## v27: the third animation crash, CalcLocalBoneTransformation (device log fa917f32)

The device crashed when the hero was attacked and took damage. v26's honest crash report pinned it
down. Lifted `F_00A4D330` +0x26c is `C3DAnimation2::CalcLocalBoneTransformation`, the single-bone
sibling of the two functions v26 guards. It was reached from `CombineBlendedBaseAnimationsBoneList`
(0xAA0DC0), which is called by `CalcBlendedBoneTransformation` (0xAA2110). The read was of address
0x38000100:

- `Sequences[bone].Data` (0x07099D40) was readable;
- the bone's rotation mode was 1 (one constant key);
- the key pointer `[data+0x14]` held 0x38000100, the bits of a float rather than a pointer.

So the sequence data was stale, the same family as the v17 and v25 crashes. The fix is
`--wrap 0xA4D330=host_calc_local_bone_transformation` (`win/menu_nav.cpp`). It applies the same
mapped-page checks to the one bone (instance, frame via the lifted ConvertTimeToFrames, sequence
entry, data, rotation and position keys or palettes, rigid mask and position). On a bad pointer it
writes an identity keyframe and logs once with `anim guard:`. That log line says whether the data
lies inside the instance's sequence data buffer, which is the next clue to the root cause (a
relocated pool, or an instance outliving its animation).

The same log's perf numbers:

- The program cache works: session 2 read 90 programs in 50 ms and pre-linked 89 in 123 ms over
  83 frames, with 0 compiled.
- Towns run at 30 fps with p99 at about 34 ms.
- Combat dips to about 28 fps.
- The texture encode/decode cost (14% in the first profile window) is load-time only (1% later).
