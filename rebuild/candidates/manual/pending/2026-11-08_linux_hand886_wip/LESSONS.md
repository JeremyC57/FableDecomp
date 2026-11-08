# Chunk 886 -- lessons from the authoring agent (fold into GOTCHAS at landing)
- Wine lane: a FATAL cl.exe error (C1189 from a header `#error "... define FABLE_RETAIL_X"`, or C1075/C1004 from an unclosed
  brace/namespace) makes cl.exe spin with no output until q.py's 600 s timeout; plain C2xxx errors return at once. Run
  `g++ -E -P -x c++ -I<overlay> -Irebuild/include src.cpp` first: it reports the #error in 0.02 s.
- Overlaying stlport/fable_containers.h needs copies of rebuild/include/fable_string.h AND stlport/fable_string.h in the overlay
  too: stlport/fable_string.h includes "fable_containers.h" by its own directory first, so the repo copy otherwise wins.
- `mov [ebp-6xx],hdr` (a dead home store of a list header) then `[hdr+4]`, reload `[hdr+4]` after a call, `[x+4]` again =
  `std::list<T>::reverse_iterator it = l.rbegin(); *it ... *++it` (0x0041F9AA, 0x00420C03, 0x00422023); `l.end()` with `*--it`
  leaves no home store (0x0041EFAB, 0x004226D8).
- VC7.1 compiles a by-reference `CCountedPointer<CComponent>` argument from a named local exactly like the temporary
  (ctor 0x429C15 / call / dtor 0x4291DE): write the named local (portable; no non-const ref to a temporary).
- In a 35 KB size-optimised function, same-TU callees defined before it (setters 0x41CA31.., list begin 0x429A85) are inlined in
  early cases and called in late ones (0x0042247E inline vs 0x00422547 call of the same setter): the caller inline budget, not
  the callee; out-of-TU header-inline accessors (CTCQuestCompletionUI::IsActive) stay inline throughout.
Review at landing:
- Row needed > 4 grades and is not matched (parked; WORKLOG.md has the resume plan).
- Local models next to existing engine headers: CTCInventory, CTCInventoryStats, CTCInventoryMap, CTCInventoryTrade,
  CTCInventoryClothing, CTCInventoryExperience, CTCInventoryAbilities, CTCQuickAccessMenu, CTCInGameMenu, CPlayerGui, CWorld,
  CQuestManager, CTCBoastUI (see WORKLOG.md open problem 4).
- CUserProfileManager overlay accessors SetNewProfileToUseWASD / WereVideoChangesAccepted read opaque bytes (+0x78 / +0x13C);
  type the members when the layout opt-ins are merged.
Session 2 (2026-11-08):
- A big size-optimised function's biased frame (`push ebp; lea ebp,[esp-74h]; sub esp,N`) comes from `#pragma optimize("s", on)`
  alone; adding `optimize("y", off)` (as the landed neighbours do) gives a plain `mov ebp,esp` frame here.
- VC7.1 c2 packs disjoint-scope object locals (e.g. every block-scoped `CDeletion deletion`) into ONE shared frame slot -- but only
  while the function's count of frame-resident symbols stays under a limit: a probe switch packs at 360 cases and packs nothing
  at 380 (same per-case body); spilled scalars count, enregistered ones barely. Past the limit every object local gets its own
  slot and the frame balloons (0x0041E6D3: 0x8DC vs retail 0x800). `/d2inlS` does not move it. A frame that is uniformly too
  deep with retail sharing [ebp-xxx] across cases = too many symbols in our source, not a missing local. Diagnose with `/FAs`
  (the `_name$N = -off ; size` table before `PROC`): packed locals share one offset.
- A class with a user-declared ctor (CEngineGraphic: PDB ??0CEngineGraphic@@QAE@XZ) returned by value from a __fastcall function
  takes its hidden pointer in ecx (args shift to edx) and the caller reads the temporary in place; as a POD the hidden pointer is
  pushed and the caller copies it out (movsd x3 + movsw for 14 bytes).
- An inline bool getter (`CAction::IsValid()`) compiles to `mov al,[eax+0Ch]; test al,al`; the same member tested directly is
  `cmp byte ptr [eax+0Ch],0`.
Session 3 (2026-11-08) -- corrects the session-2 packing lesson above:
- VC7.1 c2 (13.10.3077) turns its global optimizer OFF for a function whose IL exceeds 10000 tuples (count at 0x1077bdd8,
  `cmp [esp+0Ch],2710h` at 0x1077be49; sets 0x10882b98). That is the "360 vs 380 cases" threshold. Above it ("regime 1") c2
  still packs frame slots, but only for TRACKED symbols: scalars, and objects whose address never reaches an external call.
  An object passed to an out-of-line, user-declared (external) ctor / copy ctor / by-ref param is address-exposed and gets its
  own slot. Retail's 35 KB Action is regime 1 (cutting it under 10000 tuples changes the codegen: CSE, merged tails, 70% vs 94%).
- The fix was the CLASS, not the source size: CDeletion's default and copy ctors are COMPILER-GENERATED in retail (0x42BED4 /
  0x42CD84 are the implicit members, emitted as same-TU COMDATs and called out of line). Declaring them as user-declared
  externals (the FABLE_RETAIL_CDELETION opt-in) gives the same call bytes but makes every `CDeletion deletion` address-exposed
  -> 27 slots and frame 0x8DC. Leaving them implicit -> c2 tracks the objects, packs them all at retail's [ebp-248h] and
  cross-jumps the shared `SetDeletion(copy)/~list` tail like retail (frame 0x804 v 0x800). Probe: probe/s4/gv.py HDR=impl
  (450-case regime-1 switch: implicit = 1 slot; user-declared, same-TU-defined, force-inlined helper, by-ref param = 450 slots).
  RULE: when retail calls a ctor/copy-ctor/dtor that the PDB lists but that is the compiler's implicit member, keep it implicit;
  never model it with a user declaration "because it is out of line".
- Regime-1 frame order: unpacked locals are laid out by reference density (refs / size) ascending from the bottom of the frame
  (rarely used big temporaries deepest, the dense shared scalar slots and the dead-parameter home [ebp+7Ch] nearest ebp);
  equal-density symbols come out in a scrambled (sort-unstable) order, so the exact slot order needs the exact symbol set.
- Measure c2 internals with probe/c2/nl.sh (tuple count, packing, frame) before spending a grade; probe/c2/work/regime.py
  scores any obj against retail (loose/strict) to tell which optimizer regime retail was built in.
- Exposure is about the callee's KIND, not out-of-line-ness: retail keeps ADD_CHILD's list iterator in the dead parameter
  home [ebp+7Ch] while passing `&it` to the out-of-line (not expanded) postfix `--` 0x42899B -- an inline (header-defined)
  callee does not expose; a user-declared external or a non-inline same-TU function does.
- /d2inlS is not monotonic here: S10/S13/S15 inline slightly more than the default S14 and S16 much more; none reproduces
  retail's late out-of-line begin() calls in a 35 KB caller.

## Session 4 (2026-11-08)
- Count the cases against the jump tables before polishing: a whole case (RESET_ASSIGNED_SPELLS, 211) was missing for three
  sessions; 206 distinct jump-table targets / 207 labels is the check (t4/src_cases.txt v t2/order.txt).
- Local class models drift: CTCBase is 0xC bytes, so a derived TC's first member is at +0x0C (CTCHero::WeaponsUsable,
  CTCQuestCompletionUI::Active), not +0x10. Use the shared header (engine/CTCHero.h opt-in) instead of a local model.
- VC7.1 c2 inline decision above the 10000-tuple limit (regime 1, path 0x107537A4; probe/inl/inl.py hooks it under gdb):
  cost = sz - sav0 - sav1 + pen - 10 (leaf callee) - 7 (inline-declared) + 4 (site flag 2), inline iff cost < S (9);
  pen = (sz/2) * N / 9000 once sz > S and N > 9000, N = the caller's IL tuples so far + inline growth (11998 at Action's entry,
  ~28500 at its end). A large callee (sz 18) crosses the threshold at a LOWER N than a small one (sz 13), so retail's split
  (begin rejected late, C2DVector ctor expanded late) has no uniform-N explanation: padding the entry by +6300..7000 tuples
  reproduces the setters / vector[] / list != but over-calls begin and makes the expanded-in-retail sites worse.
- `#pragma inline_depth(0)` is captured per FULL-EXPRESSION (at the statement's end): a region ending in the middle of
  `it != l.end();` still outlines both calls, one starting there outlines neither (probe/mini/m2.cpp). So a loop condition
  `it != c.end()` cannot be split; use a per-TU `__declspec(noinline)` (VC7.1 honours it; the body stays a COMDAT) when every
  site of that callee in the function is called.
- A `__forceinline` callee inside an inline_depth(0) region is still called (probe/mini/m1.cpp): forceinline twins cannot be
  combined with a region in the same statement.
- Once the inline split matched, the frame matched too (0x800): the "one extra unshared 4-byte slot" was an inlined callee's
  temporary, not a declaration-order problem.
