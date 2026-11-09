# Matching playbook: how to finish a row that "only differs in registers / slots / scheduling"

Distilled from the rows the 2026-11-09 all-means push closed or moved:
- 0x0041E6D3 NUISystem::CManager::Action: 35,651 bytes, eight sessions, matched.
- 0x00498490 CProgressDisplay::DrawRetailDisplay: matched.
- 0x0056AD40 CTCBoastUI::Activate.
- 0x00CBFB7D RunCutsceneMacro: first wrong register decision fixed.

Sources: the hand-backs `rebuild/candidates/manual/pending/2026-11-08_linux_hand886_wip/` (LESSONS.md sessions 3-8, tools8/),
`.../2026-11-08_linux_hand885/` and `.../2026-11-08_linux_hand890/`. Allocator formulas: [C2_ALLOCATOR_PROBE.md](C2_ALLOCATOR_PROBE.md).

## 0. The premise (owner, 2026-11-09)

> "find any means possible to get them to match bytes, behavior, without compromising portability"

VC7.1 13.10.3077 is deterministic, and retail's bytes came out of it from C++ source. Every register, frame slot, cross-jump and
scheduling slot is therefore a function of the source and headers.

"Register colouring", "slot order" and "scheduler" are not park reasons. They are the next decision to measure.

Action sat at "only colouring left" for seven sessions, then matched in one once the decisions were measured instead of guessed.

## 1. Method (in this order)

1. **Check the source is retail's logic first.**
   - Count the cases against the jump table (`tools8/cases.py`). Action lacked a `GO_BACK` case, and an earlier session lacked
     RESET_ASSIGNED_SPELLS. A missing case shifts every case after it.
   - Use shared headers, not local models; local models drift. A derived TC's first member is at +0x0C, not +0x10.
2. **Find which regime the function is in.**
   - c2 turns its global optimizer off above 10000 IL tuples (`cmp [esp+0Ch],2710h` at c2.dll 0x1077BE49). That is "regime 1".
   - The tuple count can be measured from c2 itself (c2probe, hand886 `probe/c2/nl.sh`).
   - Regime 1 changes the priority, inline and frame rules below. Never cut a regime-1 row under the limit to "simplify" it.
3. **Locate the first wrong decision** with registers and frame offsets masked:
   - `tools/first.py --noframe` in hand885;
   - `m3.py`: exact / registers-masked / registers+frame-masked share.
   Byte totals mislead: Action's size moved away from retail while its instruction match rose.
4. **Turn the decision into an inequality.**
   - Read c2's numbers for that decision: c2prio `--blocks --trace --func <undecorated name>` gives priority, key, allowed set
     and costs. The default `--func` from a decorated Leaf can match nothing.
   - Write down what retail's outcome requires, e.g. "prio(game) > prio(action CSE temp)" or "prio(name) <= 25".
5. **Prove the lever with a diagnostic build** before looking for the real spelling. Even an ugly one is fine, e.g. 120 extra
   `sink = game;`. Action's diagnostic fixed the entry and took exact share to 99.2%, which proved the colouring order was the
   only big residue. Diagnostics are never kept.
6. **Search for the natural spelling automatically.**
   - Write a generator (one lever per variant) and a cheap fitness that needs no full grade: c2prio register per candidate, a
     simulated frame map, first-diverging-instruction index, cross-jump diff.
   - Compile serially and keep a leaderboard. Full-grade only improvements, then combine the winners.
   - Before keeping an odd spelling, look for the natural C++ with the same IL, and comment the retail address it fixes.

## 2. Rules measured in c2 (all VC7.1 13.10.3077)

### Register priority and choice
- Priority is computed per block and summed: a candidate gains w(b)·K(b)·cost in each block that references it and loses
  w(b)·K(b) in each block it is live through without a reference (C2_ALLOCATOR_PROBE.md section 4).
- **Regime 1: every block has w = 1**, so loops add nothing. Loop-based "raise its weight" ideas are dead ends there.
- **K counts every candidate referenced in the block, memory-only ones included.** In 0x00CBFB7D, seven entry flags that never
  get a register inflated K from 12 to 19. That lifted `name` to priority 38, ahead of the -1 constant, so `name` took esi.
  Taking them off the candidate list gave retail's ebx/esi.
- **A global pointer is itself a register candidate.** In Action, re-reading `CMainGameComponent::Get()` at each site gave the
  global priority 1141 and esi, as in retail. A named `game` local had priority 574 and lost to the action-pointer CSE temp (958).
- **A call kills the global's register copy.** Reads right after a call must use the local, which shares esi through its copy.
- **A named call-result intermediate costs priority.** It adds +1 K per referencing block (+3 priority) for identical code.
- **Ties go to the larger key**, i.e. the value defined earlier in its block.
- **Choice:** the cheapest allowed register wins, ties in table order eax, ecx, edx, esi, edi, ebx, ebp. Copy relations give -1
  to -3, and a neighbour left with one register costs 100·weight.

### Frame packer (regime 1; traced in hand886 tools8/pack.py + phook.py, 284/284 slots; hand885 tools/pk/ 905/905)
- **Node order:** nodes are listed by size ascending, then IL weight descending (c2.dll 0x10788642; +1 per IL reference,
  counted BEFORE cross-jumping).
- **Slot joining:** each node joins the latest slot with node.size <= 2·slot.size and no conflict, else opens a new slot
  (0x10791426).
- **Final sort:** qsort (0x107AE0C4, comparator 0x107AE9C2) by the /GS-buffer flag, then density w·1000/size descending. Ties are
  sort-unstable, so the exact symbol set decides the slot order.
- **Exposed objects get their own slot.** An object whose address reaches a user-declared out-of-line function conflicts with
  everything and never shares a slot.
  - Keep compiler-generated constructors implicit.
  - A same-TU constructor that retail calls at every site gets the real in-class body plus `__declspec(noinline)` under
    `#ifdef _MSC_VER`. A declaration only would expose the object. This was Action's frame fix, 0x808 -> 0x800.
- **Spellings that add or drop nodes:**
  - `iterator it = c.begin()` constructs in place; `iterator it; it = c.begin()` adds a weight-1 temporary.
  - A named `CDefPointer x = Get..()` per arm adds nodes; `Get..()->Field` read in the call does not.
  - Under /GS, pointer- and reference-typed nodes stay out of the dead-parameter front slot.
- **Grading is unreliable while the slot set is wrong.** Every added or removed symbol reshuffles the density sort, so exact
  share can fall while the code improves. Grade shape fixes with the frame-masked share and track slot pairing separately.

### Inline decisions
- **Regime 0 (normal functions):** a site is expanded iff c1·(1/f + 6)/7 < 74 (c2.dll 0x1078D1AA), where f is the site's static
  frequency.
  - A `for` body runs at 5x and the code after the loop stays at that frequency.
  - A `while (...) { ...; ++i; }` body runs at 9x and the code after it drops back.
  - One loop rewritten as `while` fixed 17 of 20 sites in 0x0056AD40, and gave 0x00551EA0 its exact split.
  - Try loop and guard shapes before devices.
  - Branch frequencies: `if (a || b)` sends 0.75 down the then-path (even when `b` folds away), `a && b` 0.25, `else if` gives
    the fall-through 0.75, and a `while` containing another loop behaves like a `for`.
  - Regime-0 caller-size penalty (0x0061E6D0 inline trace): pen ≈ sz·(a+g)/18256, applied only once a+g > 9000. g starts at
    the caller's own size and grows with each expansion. Past 9000, a CCountedPointer site (c1 70) is called at frequency
    ≈ 9.4; below it, only at f ≤ 0.714. A budget-only explanation must fit every site at once; if it can't, look for a
    frequency difference instead.
  - `new T` puts T's constructor at half the statement's frequency (the null test).
  - `for` → `while` over a std::list is code-identical in VC7.1 and cools all code after the loop by 5x.
  - Declaration position is code: moving a counted pointer's default declaration above a call moves its zero stores above it.
  - When forcing a candidate in a what-if, address it by creation ordinal, not by c2 id (ids are recycled). Check both
    sides of a pair inequality.
  - Callee cost is computed from the optimised callee, so header spellings cannot move it. Solve a call/expand split as a
    constraint system over the sites: hand890 s6/fmodel.py solved 0x0056AD40's 34 sites with no device.
- **Regime 1:** cost = sz - sav0 - sav1 + pen - 10 (leaf) - 7 (inline-declared) + 4 (site flag 2); expanded iff cost < S.
  - pen grows with the caller's tuple count, so late sites are called (path 0x107537A4).
  - /d2inlS is not monotonic.
  - Use the owner's per-site devices where no uniform flag fits.
- **`#pragma inline_depth(0)` scope:** it is captured per full-expression, so a loop condition cannot be split.
- **Forceinline inside a region:** a `__forceinline` callee inside an inline_depth(0) region is still called.

### Cross-jumping (shared tails in a switch)
- **Edge order:** the exit block's edges are ordered `break`s in source order, then `return`s in source order, then edges into
  `default:`.
- **First pass:** the LAST edge whose block shares a tail absorbs every other matching block, which jumps into it.
- **Second pass:** the blocks that now jump to the same place merge again, and there the earlier case survives.
- **Levers:** to make a given case keep a tail, end the competing case with `return;`, or move `default:` right after the case
  (Action: six `return;` endings and a mid-switch `default:`).
- **Never use a user label as a lever.** Any label in a big switch reorders cross-jumps across the whole function (175 changed
  in Action).
- **Mini probes need distinct non-virtual head calls per case**, or the heads merge too.

### x87 order, argument order, scheduling
- **x87 operand order:** of two frame operands, c2 loads the one stored most recently.
  - A temporary built by an inline constructor's parameters outranks a member assigned field by field.
  - A constructed vector stays first when it is copied and then modified (0x00498490).
- **Argument order:** an inline accessor argument is evaluated before another argument's call; a direct member read is pushed
  after it.
- **Values read at the call:** a parameter pointer with `->Value` read at the call pushes `[reg+8]`; a named `long` local loads
  it first.
- **Hoisting:** `if (c) f(a); else f(b);` lets c2 hoist identical argument code and cross-jump the calls; `f(c ? a : b)` does
  not.
- **Scheduler:** the list scheduler issues two instructions per cycle (c2.dll 0x1078734C).
  - A parenthesised floating sub-expression keeps a no-code tuple (0x269) that the scheduler counts, so `(a * 0.5f) - b` can
    schedule one instruction apart from `a * 0.5f - b` (0x00498490).
  - hand890 `tools/sim.py` replays the scheduler.
- **Bools in regime 1:** a named `bool b = x > 0; f(b);` lives in memory (`mov [ebp+7Ch],al; push [ebp+7Ch]`); `f(x > 0)` does
  not.

## 3. Measured dead ends (do not retry)
- Loop spellings to stop rotation of a wait loop: `while`, `for(;;)` + break and `goto` all compile identically in a regime-1
  function (0x00CBFB7D CrowdKill, four families).
- Reordering declarations or first uses to change colouring: c2 colours by priority, not first use.
- `(void)&x`, `bool x[1]` and by-reference arguments of inlined calls do not remove a candidate.
- Per-row `/d2inlT` to fix one site in a large caller: it moves others too (prove it with the inline trace, then use a scoped
  device).

## 4. Tools (in the hand-backs; promote to lane_tools when generalised)
- **hand886 `tools8/`:**
  - `pack.py` / `phook.py`: frame-packer replay;
  - `xj.py`: retail v ours cross-jump diff;
  - `cases.py`: jump-table case map v source labels;
  - `hh.sh`: artefact-filtered hunks;
  - `kgain.py`, `pb.py`: priority bookkeeping;
  - `fit.py` / `ev.sh`: hooked compile, scorer, frame map and fitness in about 65 s.
- **hand885 `tools/`:**
  - `pk/fit.sh`: about 60 s fitness;
  - `pk/wi.py` + `wig.py`: c2 what-if that forces a candidate's priority or key;
  - `pk/chain.py`: split-piece ancestry;
  - `pk/first.py`;
  - `pk/sim.py`: packer model.
- **hand890 `tools/`:**
  - `sim.py`: scheduler replay;
  - `t13.py`: per-cycle scheduler choices.
