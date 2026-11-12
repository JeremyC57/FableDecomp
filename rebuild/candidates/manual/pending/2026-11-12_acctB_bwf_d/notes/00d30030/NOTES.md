# 0x00D30030 NScript::CQ_EndGameBossBattleScript::JackOfBladesBossBattle (8589 bytes) -- author session 2026-11-12

Status: DIFFER(8653v8589, 199 hunks), best source differ/00d30030.cpp (= c/v33.cpp). ~45 grades + 4 c2 traces + 1 probe.
Leaf ?JackOfBladesBossBattle@CQ_EndGameBossBattleScript@NScript@@QAEXXZ (name proven in DOSSIER.md; manifest label wrong).

## What moved it (10757 -> 8653; call list now = retail, counted from the object's relocations, tool c/cc.py)
- v1 full draft (OUTLINE order): 10757 -- ~vector expanded at 90 sites (retail 47), CRGBColour / MovieBase ctor+dtor /
  CScriptThing op= called.
- c2 inline trace (python3 /opt/fable_lane/w22/agent4004/tools/inl/inl_host.py src out.json, C2PRIO_RUNS / FABLE_HDR_OVERLAY set;
  c/dd.py prints the row's decisions): sites are decided by the score rule (expand iff sc < 74), sc falls with static frequency.
  Unguarded `for` loops multiply the frequency by 2.5 and give the in-loop and after-loop exits the same frequency; retail
  needs inner < after (loop 2: ~vector called inside, expanded after). `if (n > 0)` in front of every FIRST-PASS loop
  (x1.25 per loop, inner .47 / after .56 for loop 2) reproduces retail's split exactly: 13 ~vector calls, ~CScriptThing
  expanded L2-L5, called at L1 / L6 (pen kicks in at g>9000). `while` / do-while / guards only on some loops: all wrong
  (6024 / 9349-10461 bytes). Second-pass loops: NO guard (retail has single tests there; all exits cold anyway).
- `if (n > 0)` leaves a double compare where the count is in memory (loops 3-6): `if (this)` (folded null test, agent2058
  precedent) keeps the same frequency and gives retail's `mov eax,[cnt]; xor edi,edi; cmp; jle` shape. FITTED.
- Per-loop `for (long i = 0; ...)` (v19) better than one shared `long i` (217 -> 198 hunks); frame becomes 0xC8 = retail.
- Tail is cold (each `if (ITT) return` halves the frequency: movie sites f ~ 0.0001), retail still expands there ->
  forced helpers: FABLE_RETAIL_CSCRIPTGAMERESOURCEOBJECTMOVIEBASE_FORCEINLINE + new GROBase OUTOFLINE_CTOR opt-in (retail calls
  0x006E7A60 inside the expansion), FABLE_FORCEINLINE CScriptThing::operator= with CCountedPointer<CScriptThing>::operator=
  declared out of line (0x00704580; precedent Bowerstone 0x00D23DC0), new CRGBColour FORCEINLINE_CTOR4 opt-in.
  MovieBase ctor/dtor spec: `template <> CCountedPointer<MovieBase>::~CCountedPointer();` must precede the MovieBase include.

## Residue (what blocks it)
- Register allocation: c2 colours `const 0` (prio 117, 269 refs) late, splits it and gives the main piece ebp for the whole
  body including the prologue (`xor ebp,ebp; mov [esp+0Ch],ebp; push ebp`). Retail: prologue immediates (`mov [esp+0Ch],0;
  push 0`), ebx = 0 from 0x00D300F0, re-zeroed after loops 1/2, loop counter coalesced with it (`inc ebx`), nChandeliers in ebp
  (ours ebx, i spilled to [esp+14h]). Probe (probe1.cpp: first wait loop deleted): const 0 then takes ebx from the prologue and the
  TransitionToTheme temp moves to retail's 0x70 -- the first wait's bl bool (#22/#62 `local` line 44) blocks ebx for the const-0
  piece that also holds the prologue refs. Retail's split must cut const 0 at the first wait (prologue piece -> immediates).
  c2prio: python3 tools/decomp_pipeline/linux_lane/lane_tools/c2probe/c2prio.py c/v33.cpp --func JackOfBladesBossBattle --trace --blocks
  (c/p33b.txt): block 4 (wait retest, line 44) references const 0; piece #7 `allowed ......p`, costs ebx 403 / ebp 2000.
- Frame slots of CCharString temps / counts follow from it (first wait temp 0x24 v 0x14; counts chLong/book/round/trio slots).
- Smaller: &seh_Jack CSE (retail one `lea ebx,[esi+50h]` for IsValid/Release + the Start loop; ours edi then ebx), CS_AEONS
  vtable cached in ebp (retail 0x00D312A6), `mov bl,0FFh` kept for the FadeScreenOut colour alpha (retail 0x00D31437).

## Tried without effect on the residue
- explicit instantiation of ~vector / class vector (cost rule) -- no change; `== false` / explicit CCharString in the waits;
  counts declared together (before GetAlls / at top); guard spellings `>= 1`, `!= 0`, `0 < n`, `if (Game)` (emits a test).

## Best next ideas
1. Find the source fact that makes c2 split const 0 at the first wait loop (retail): c2prio --trace on variants of the
   prologue/first statements (e.g. the spawned-function flag word: AddSpawnedFunction in its own `{}` scope so its
   construction flag is not zeroed in block 1; SetTeleportingAsActive argument spelling), metric = const 0's register.
2. Check the guards' plausibility: the same frequency effect may come from a natural construct (an `if (n > 0)`-like test the
   compiler folds); only loop-level structure moves the inline split, so test each candidate with the inl trace (77 s).
3. Then the frame slots (c2 frame packer hook /opt/fable_lane/w22/agent2009/tmp/c2/pk.py) once registers agree.

## Later findings (same session)
- c2prio colouring: const 0 (#1) has `allowed .....b.` (only ebx) when the list is built; at its turn ebx is held by the wait
  bools (#22/#62 line 44, #4/#21 FrescoDome, #2/#20, #1/#19) so it is deferred to the split pass and piece #7 gets ebp.
  Retail's const 0 pieces evidently avoid the wait-bool ranges (ebx zero from 0x00D300F0 until the FrescoDome wait).
- const 0xFF: retail keeps it in bl (`mov bl,0FFh` 0x00D31437; 4 byte stores + the FadeScreenOut alpha 0x00D31922); ours
  `const 0xff` prio -39 -> immediates. Same 5 refs, so retail's priority (live-through penalties) differs too -> both point to a
  different block/K structure between 0x00D31437 and 0x00D31922 or around the waits, not to a spelling of the colour.
- Tried, no change: AddSpawnedFunction in its own `{}` scope, a named `watcher` pointer, SetTeleportingAsActive(0).

## Attempt 2 (2026-11-12) -- running notes
- REGISTERS FIXED: declare the loop index BEFORE the guard: `{ long i = 0; if (n > 0) for (; i < n; ++i) {...} }` on the six
  first-pass loops (no `if (this)`); retail's `xor edi,edi; cmp ebp,ebx; jle` = i zeroed before the entry test. Prologue now
  uses immediates, const 0 in ebx, loop counters right. 196 hunks, residue = frame slots only up to the late epilogue (a2/f_gi.cpp).
- FRAME/INLINE ROOT CAUSE (probe, NOT landable): c2's inline growth counter g starts at the caller's own size; the penalty
  kicks in at g ~9000. Retail crosses it between the 4th and 5th ~vector of the loop-5 after-exit (retail 0x00D30A60: 4
  expanded, chand/sconces called and cross-jumped into 0x00D32197), which also gives retail's vector slot order. Adding
  370..430 empty inline calls (`ProbeNop()`, ~2 IL each => caller size +740..860) at the top reproduces that exactly
  (a2/pt400.cpp: 41 hunks with `TRetVoid _Fail_Val` returns and `v.erase(v.begin(), v.end())` for clear(), since clear()
  is no longer inlined at the higher g). The landable fix = find what made retail's function ~800 IL bigger.
  `return _Fail_Val.Return();` adds +101 (measured, base 3053 -> 3154). Not TU-global (other functions' growth: no effect).
- (attempt 2, cont.) LEGIT gains: `while (!(EntityGetBossPhase(Jack) == 4 || GetHealth(Jack) <= 10.0f))` gives retail's `test ah,41h; jnp`;
  `v.erase(v.begin(), v.end())` keeps the erase call once caller size rises; MovieBase FORCEINLINE_CTOR (implicit dtor, no vtable
  reset) + GROBase OUTOFLINE_CTOR; new hdr opt-in FABLE_RETAIL_STLP_MALLOC_ALLOC_DEALLOCATE (STLport allocator::deallocate ->
  __malloc_alloc<0>::deallocate layer) supplies ~250 of the ~800 missing growth. differ/ = a2/base2.cpp (170 hunks).
- With the probe base (a2/mv2.cpp, 13 hunks) the only residue left is the cold-tail implicit ~MovieBase: retail expands it
  (0x00D317F3 / 0x00D318BB) but c2 scores it 162814..651172 (block f ~1.4e-5 after ~18 ITT halvings); pass-2 ~vector sites that
  retail CALLS score 2227..83964, so no /d2inlT fits; /d2inlS, ITT spellings (!= false, == true), wait loops as for(;;)/break,
  while(true), for(;c;) leave the frequencies unchanged. A visible noinline ~CCountedPointer<MovieBase> body with the forced dtor
  keeps the vtable store. RAII CWideScreenMagicPauseEntities (plain or forced ctor) is worse than the cached `game` pointer.

## MATCHED (attempt 2): RELOCATION_MATCH with `// Flags: /d2inlS25` (source 00d30030.cpp, test test/00d30030_test.cpp, payload.json)
What made it match, in order:
1. Loop index declared before the guard: `{ long i = 0; if (n > 0) for (; i < n; ++i) {...} }` on the six first-pass loops
   (registers: zero constant in ebx, first index coalesced with it, prologue immediates). Plain for / while forms change the
   inline split (10189 / 6040 bytes); the second pass stays plain `for (long i = 0; ...)`.
2. Caller-size / growth: c2's growth counter must cross the ~9000 penalty between the 4th and 5th ~vector of the loop-5 after-exit.
   Supplied by two STLport 4.6 allocator layers (hdr opt-ins FABLE_RETAIL_STLP_ALLOC_PROXY_REBIND -- chunk 883 text -- and the new
   FABLE_RETAIL_STLP_MALLOC_ALLOC_DEALLOCATE) plus `TRetVoid _Fail_Val; return _Fail_Val.Return();` (+101). Adding
   FABLE_RETAIL_CCOUNTEDPOINTER_RELEASEDATA on top overshoots (136 hunks); a __value_type layer was a probe only (removed).
3. `v.erase(v.begin(), v.end())` for the six clear() calls (clear itself stops being inlined at the higher growth).
4. `while (!(Game->EntityGetBossPhase(Jack) == 4 || Game->GetHealth(Jack) <= 10.0f))` -> `test ah,41h; jnp`.
5. Movie: FABLE_RETAIL_CSCRIPTGAMERESOURCEOBJECTMOVIEBASE_FORCEINLINE_CTOR (implicit dtor, no MovieBase vtable reset) +
   FABLE_RETAIL_CSCRIPTGAMERESOURCEOBJECTBASE_OUTOFLINE_CTOR (call 0x006E7A60); CScriptThing op= via the header's
   FABLE_RETAIL_CSCRIPTTHING_IMPLICIT_ASSIGNMENT (no row-local forced op=, no CRGBColour forced ctor needed).
6. `/d2inlS25` (smallest; S24 fails): expands the cold-tail implicit ~MovieBase (score ~3e5, f ~1e-5; no /d2inlT fits because
   pass-2 ~vector sites retail calls score 2227..83964). Precedent: CBH_GangDeputy2::Main 0x00D18DF0 /d2inlS15.
7. Game->ActivateQuest(...) inside each morality arm (after PlayAVIMovie): retail's arms are cross-jumped copies; the join form
   schedules `push -1; push lit; lea ecx` instead of retail's `push -1; lea ecx; push lit`.
Relocations checked with agent2004/rel.py: every call target equals retail's (the DIFF lines are pooled ""/0.5f literals).
Name: the manifest label `_global::CScriptGameResourceObject_LoadGuildComplex` is wrong; real name per DOSSIER.md (Ego_r + FableWin).
Not the answer (do not retry): CWideScreenMagicPauseEntities RAII (plain or forced ctor), CLevelScriptBase ORIGINAL_MEMBERS (no
change), wait loops as for(;;)/while(true)/for(;c;) (frequencies unchanged), ITT spellings, visible noinline ~CCountedPointer<Movie>.
