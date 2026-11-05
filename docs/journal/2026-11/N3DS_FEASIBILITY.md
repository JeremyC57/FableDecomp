# New 3DS port of the Xbox recomp: feasibility gate (2026-11-05)

Target: New 3DS in speedup mode (ARM11 MPCore 804 MHz, L2 on), extended memory (up to ~178 MB,
gate at ~150 MB), PICA200, devkitARM + libctru + citro3d.

## Measurements

Lifted `default.xbe` (`lift.sh`, 45,250 functions in 536 files, every discovered function emitted).

| What | Result |
|---|---|
| Lifted code, x86-64 clang `-O1` | 79.7 MB `.text` |
| Lifted code, ARMv6K/VFPv2 ARM mode, gcc `-O1` (`-ffunction-sections`) | **92.5 MB** |
| Same, `-Os` | 93.0 MB (no gain: the code is straight-line guest memory access) |
| Guest physical memory in use, Oakvale gameplay (peak) | 40.6 MB |
| Guest virtual memory touched (peak) | 23.7 MB |
| Guest memory total | **~64 MB** (the retail console budget, as expected) |
| Code + guest memory, before any host runtime, renderer, audio or textures | **~157 MB** |
| Game thread CPU per frame, Oakvale childhood gameplay, flips 3300–6000 | **18.1 ms avg, 12.2 min, 25.0 max** |
| NV2A pusher CPU per frame (renderer translation, not counting Vulkan) | 20.4 ms avg |

Game-thread time is the CPU time of the guest threads (they run one at a time under the GIL),
measured with `FABLE_PERF_STATS=60` on one pinned core of a Xeon (Emerald Rapids, 2.1 GHz
nominal), rendering through lavapipe on the other three. The game clock advances 33 ms per flip
(`gameClockNs`), so the simulation work per flip does not depend on the host frame rate.

A 150 MB memory-cgroup run (headless) survives only by evicting the binary's code pages: usage
sits at the cap (157,286,400 bytes) for the whole run.

No wall-clock CPU throttle run: a CFS quota of q only stretches the same CPU time by 1/q, so the
estimate below applies the slowdown factor to the measured CPU time directly. CoreMark could not be
fetched here; the factor comes from published ARM11-class figures (about 1.2–1.7 CoreMark/MHz at
804 MHz, against a modern x86 core several times higher per MHz at three to four times the
clock), plus the ARM11's lack of NEON and hardware divide, and its small caches.

## Gate

- Memory: 92.5 MB code + ~64 MB guest = ~157 MB against ~150 MB, with nothing yet for the host
  runtime, the PICA200 texture copies, audio buffers or stacks. **Fails**; it could only pass if
  reachable-only lifting removes a large share of the code.
- CPU: 18.1 ms per frame on the Xeon. At an assumed ARM11 slowdown of 15–30×, game logic needs
  about 270–540 ms per frame (2–4 fps); even an optimistic 8× gives 145 ms. The limit is 66 ms.
  **Fails by roughly 4–8×**, before the renderer, which needs about as much CPU again.

Verdict: no-go as specified. See the session report for risks and the conditions under which it
would be worth revisiting.
