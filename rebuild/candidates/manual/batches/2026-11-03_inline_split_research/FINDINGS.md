# Per-site inline expansion in VC7.1 (cl 13.10.3077): what the compiler actually does

Research only (nothing landed, nothing written under the repo). Lane: Wine, RTM cl, `cl /nologo /c /O2 /Oy /W3`.
Probe sources and per-probe results: `probes/` (`probes/RESULTS.md`). Row work: `rows/`, `revisit/`, `family/`.

## 1. The mechanism (answer)

VC7.1 makes the plain-`inline` decision **per call site**, from a cost/benefit score that uses c2's **static
block-frequency estimate** of the site. There is no per-function all-or-nothing rule and no depth rule for plain inline.
Reconstructed from c2.dll (`0x1078d1aa` decision, `0x1078d03c` per-site argument savings, `0x1078cf54` pre-check;
the hidden c2 knobs and their defaults are set at `0x107085e4`) and confirmed by probes:

```
score = (c * 1e8 / f + (k - 1) * c) / k        f = site frequency (1e8 = once per call), integer math
k = 7 (-inld) when f <= 1.0,  k = 3 (-inlld) when f > 1.0
inline  iff  score < 74 (-inlT)
c = optimised size of the callee AFTER its own inlining (bottom-up; dead code, temps, redundant stores do not count)
    - per-site argument savings - 7 for a user-`inline` callee (-inluserinl) [+ (size/2)*callerSize/9000 past 9000]
__forceinline: accepted before the score (callee flag 0x2000), so it expands everywhere.
```

Probe evidence (callee `Release`, c = 49): T* (lowest `/d2inlT` that inlines) = 50 at entry, 57 behind one if,
71 behind two, 2·49 = 98 behind three (> 74: call). A callee of ~2x the size flips one nesting level earlier (p03).
`/d2inlT1000` inlines nearly everything, `/d2inlT20` almost nothing (p03), so the threshold is that knob.
`/d2inlT` and friends (`-inlS -inld -inlld -inlocsa1..4 -inlcrmax -inlfcsa -inluserinl -inlnobr`) are c2 diagnostics
only; retail never had them and they must never be a landing flag.

### c2's static frequency estimate (measured, `probes/p07_freqmeter`)
- Every two-way branch is 50/50, whatever the test (null, ==, <, !=, bool call): no branch heuristics.
  `a && b` reaches the body with 0.25, `a || b` with 0.75. A join restores the sum.
- Loop bodies (for/while/do/goto/`for(;;)`) are ~5x (counted and pointer loops) to ~8x (while(call)) the entry.
- **All exits of one loop share the loop's entry frequency equally** (1 exit 1.0, 2 exits 0.5 each, 3 → 0.33,
  5 → 0.2), independent of nesting depth inside the body (p13, p14, p15). Straight-line exits halve (0.5, 0.25, ...).
  This is the "function-wide all-or-nothing" behaviour the GOTCHAS recorded for ~CScriptThing at in-loop returns:
  every return of the loop has the same score, so they flip together when a return is added.
- **`do { ... } while (0)` is a loop** to the estimator. A site inside it is hot (T* 35, like a loop body), and a
  `return` inside one nested in a loop is an *inner-loop* exit, not one of the outer loop's equal shares: such exits get
  high, geometrically declining frequencies in program order (T* 36, 40, 46, 54 for four wrapped returns, against 57
  for an unwrapped return of the same loop; p18).
- **Post-loop leak**: after a guarded counted/pointer `for` loop the next block keeps the loop body's frequency (~5x);
  a second loop compounds (~25x; p08). `while (call())` loops and `for(;;){ if (x) break; }` do not leak.

### Caller-size cap (Hypothesis 2, real but not the cause of the parked rows)
Once the caller's IL size (plus growth so far) passes 9000, a penalty `(calleeSize/2)*size/9000` is added: practically
no more plain inlining. Sites are visited in source order, so in a huge function the first sites expand and the later
ones stay calls (p05c/p05d: 4400 vs 4600 padding calls). A counted loop before the sites suspends it (p05e/f,
unexplained). The parked rows are far below 9000.

## 2. Hypotheses

| # | hypothesis | verdict | evidence |
|---|---|---|---|
| 1 | extra inline wrapper layers exceed a default depth | **rejected** | W1..W10 wrappers around Release all expand at f=1 and f=0.25 (p06). c2 compiles bottom-up, so a wrapper costs what it expands to (p09: X1 = Y = 34). `-inld` (7) is a weighting constant, not a depth. A dtor at a scope exit is an ordinary site. |
| 2 | per-function budget consumed in order | **real, but only past ~9000 IL instrs** | p05 (sharp cutoff between 4400 and 4600 padding calls, source order). Not the parked rows. |
| 3 | EH context | not needed | the rows are /EHsc-free; no EH probe run |
| 4 | block frequency / cold paths | **confirmed: the mechanism** | formula above; p02, p03, p07, p13-p15, p18 |
| 5 | expanded and called copies are different entities | **rejected for the checked rows** | retail ~CScriptThing 0x004AA840 and CEnginePrimitiveLine 0x004C3090 are byte-identical to our out-of-line copies; Reset 0x00429C6F calls ReleaseData 0x004291DE out of line (that COMDAT came from another TU) while every expanded Reset in 0x005FF190 expands the release, so the copies differ only by TU |
| 6 | other | **do-while(0) blocks and loop guards** move frequencies; definition order (p17) and the number of other references in the TU (p16) do not matter | |

## 3. Recommended pragma-free patterns (in order of preference)

1. **Delete the pragma and make the forced dtor plain `inline`** first. 27 of the 129 marked rows grade
   RELOCATION_MATCH that way (`family/summary.tsv`, column `plain`): their retail split is just the frequency rule.
2. **Raise the frequency of the exits retail expands** with an inner loop: write the script terminate check as
   `do { if (IsActiveThreadTerminating()) return; } while (0);`. Written that way at EVERY check (one macro) it matches
   24 marked rows (8 that `plain` misses) and the parked CV_AmbushScamScript::Main 0x00E2B090; written only at the exits retail expands it
   matches 3 more (0x00E2E520, 0x00E9CB20, 0x00ED9790). 0x00EF5CB0, one of the two chunk 694 pilot failures, matches.
   Plausibility: `do { ... } while (0)` is the standard statement-macro wrapper (MFC's ASSERT/VERIFY in afx.h), so
   `#define CHECK_TERMINATE() do { if (IsActiveThreadTerminating()) return; } while (0)` is ordinary period C++; that
   retail used such a macro is NOT proven (no macro text survives in the PDBs), only that its exits behave like it.
3. **MFC-style ASSERT**: CString::ConcatInPlace 0x00C0C270 is MFC's function line for line; with the library's
   `LIB_ASSERT` written as MFC writes ASSERT (`do { if (!(cond)) { ... } } while (0)`) and GetData plain inline, the
   ASSERT pair expands and the three plain uses call 0x00C0B8C0, RELOCATION_MATCH with no pragma
   (`revisit/00c0c270/dowhile0.cpp`). Strongly plausible: the row's assertion strings are MFC's.
4. **Lower the frequency of the sites retail calls** with a source-level guard the optimiser then folds:
   CTCCreatureGenerator::DrawDebugInfo 0x0074BC20 -- one `if (!OpenPositions.empty())` around the first loop (and
   FABLE_RETAIL_CRGBCOLOUR, which makes our CEnginePrimitiveLine ctor byte-identical to 0x004C3090) gives retail's call
   sequence exactly (ctor called in the open-positions loop, expanded in the hidden-positions loop). The second loop is
   hot because of the post-loop leak. Not a full match: the row's frame/temporary residue is unrelated.
5. Make the callee's body retail's (it is the cost): model the inline callee so its out-of-line compile is
   byte-identical to retail's COMDAT before tuning any site.

## 4. The 9 parked rows

| row | mechanism | status |
|---|---|---|
| **0x00E2B090 CV_AmbushScamScript::Main** | in-loop returns share frequency equally; the four exits retail expands must be inner-loop exits | **RELOCATION_MATCH pragma-free**: every one of the 23 `if (IsActiveThreadTerminating()) return;` written `do { if (IsActiveThreadTerminating()) return; } while (0);` (one macro everywhere) with the chunk's plain `inline CScriptThing::~CScriptThing() { }`; wrapping only the 4 retail-inline exits also matches (`rows/00e2b090/dwall.cpp`, `dw4.cpp`; `matched/00e2b090_Main_dwall.cpp`). 2183v2357 before. No behaviour test exists for this row (chunk 730 wrote none), so it is graded, not vland-checked. |
| 0x00E2F170 SpringAmbush | same; the do-while(0) form moves 2163 -> 2272 v 2781 and fixes the dtor split | the remaining 500 bytes are other inlining (CScriptThing::GetPos expanded in retail, called in ours) and creature temporaries -- not this mechanism's residue |
| 0x00E2EC30 GetTrickster | frequency (GetDataString Good site is one 50/50 hotter than Evil) | wrapping the returns changes nothing; the row is 283 bytes short for model reasons (vector, copies) |
| 0x0074BC20 DrawDebugInfo | frequency: loop 2 inherits loop 1's frequency (post-loop leak) -- loop-1 sites score 66-67, loop-2 57 (both < 74, so ours expands all four); one 50/50 guard pushes loop 1 past 74 | call sequence now identical to retail (30 = 30) with `if (!OpenPositions.empty())` around loop 1 (+FABLE_RETAIL_CRGBCOLOUR); DIFFER 1351v1290 (`rows/0074bc20/v1.cpp`; `v4.cpp` = `size() > 0` 1323v1290); the rest is frame layout, not inlining |
| 0x005FF190 CreateMapMarkers, 0x00603000 | frequency: our sequential loops leak and compound, so loop-3 sites are 4x+ too hot and the loop-1 core/optional sites are hot enough to expand | not matched; meter on the row is confounded (T changes other inlinees). Next step: find which of retail's loops do not leak (while-form) or carry an extra guard |
| 0x00F3D980 BuildRings, 0x00F3EA50 GenerateTriangleListAndStrip | frequency (push_back / _M_insert_overflow / CArray copy at different loop depths); not graded | |

## 5. Generalisation to the 129 `// Revisit: inline-depth` rows (copies only, never the tree)

Mechanical rewrites of every marked row (`family/transform.py`), graded with the vland parity function at base flags
(row `// Flags:` honoured) -- `family/summary.tsv`, sources in `family/rows/<addr>/`:

| rewrite | matches |
|---|---|
| `plain`: drop the `#pragma inline_depth` lines, make `__forceinline` destructors plain inline | 27 |
| `dwall`: plain + every `if (COND) return;` of the row function as `do { if (COND) return; } while (0);` | 24 (8 not in `plain`) |
| `dw`: plain + wrap only the returns that were outside depth-0 regions | 25 (3 not in `plain`/`dwall`: 0x00E2E520, 0x00E9CB20, 0x00ED9790) |
| greedy per-site toggling (`family/greedy.py`) on the 10 nearest misses | 3 (0x00F061E0, 0x00E1FCF0, 0x00E1FEF0; all three also match `dwall`) |
| MFC do-while(0) ASSERT + plain GetData (by hand) | 1 (0x00C0C270) |
| **union** | **39 of 129** (`matched/revisit/`, one source per row) |

Read-only vland (`vland.py <payload> <oracle> --recheck-landed`, no `--land`) on six of them, one per kind:
`vland/vland.log`: 0x00E2E520, 0x00C0C270, 0x00EF5CB0, 0x00E1FCF0, 0x00D16920, 0x00F061E0 all RELOCATION_MATCH + PASS
(only SHARED_HEADER_AVAILABLE warnings). 0x00EF5CB0 is one of the two chunk 694 pilot rows that failed with 13 probes.

The 90 rows left: 66 have a return inside a depth-0 region that neither uniform rewrite reproduces (they need the
per-row shape: which exits are inner-loop exits, plus the loop-test form); 18 put depth-0 around non-return constructs
(temporaries, Reset, member assignment, holder classes) whose sites differ in frequency or cost in a way not yet
modelled; 6 use other depths. Expected yield of per-row work with the frequency rules: most of the return rows (the
greedy search converts about 1 in 3 near misses with exit wrapping alone).

## 6. What is and is not plausible as retail source
- Plain `inline` everywhere (no `__forceinline` dtors, no pragmas) is what the PDBs show for CScriptThing /
  CCountedPointer (FableWin.pdb names, no forceinline evidence); the 27 `plain` rows need nothing else.
- `do { ... } while (0)` statement macros: standard practice; MFC's ASSERT is exactly this (0x00C0C270's strings are
  MFC's). For the quest scripts it is unproven which construct retail used (the Lua ports are not C++); any inner loop
  around the exit has the same effect on the estimator. Prefer `dwall` (one macro for every check) over hand-picked
  wrapping: 24 rows plus Main match that way.
- A redundant `if (!v.empty())` guard is ordinary code; it costs nothing after optimisation.
- `/d2inlT`, `/d2inlS` and the other `-inl*` knobs are hidden c2 switches: diagnostics only.

## 7. Files
- `probes/` probe TUs + `probes/RESULTS.md`; `c2re/c2.asm` objdump of c2.dll (decision at 0x1078d1aa).
- `cc.sh`, `calls.py`, `meter.py`, `rowmeter.sh`, `qq.py`/`qq.sh` (private q.py with call alignment), `mkoracle.py`,
  `rdis.py`, `mkpayload.py`.
- `family/` transform, batch runners, `summary.tsv`, `greedy.log`; `matched/` sources; `vland/` payload, oracle, log.
- `GOTCHAS_PROPOSED.txt` one-liners.
