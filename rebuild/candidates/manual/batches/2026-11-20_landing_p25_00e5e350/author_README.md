# 0x00E5E350 NScript::CV_BodyGuardScript::HenchmanCommentary (p25D, finishing p25C's batch)

**Grade:** RELOCATION_MATCH (585/585).
- The strict vland run passes (LANDVERIFY_STRICT_HEADERS=1, empty overlay `hdr/`, fresh LANDVERIFY_WORK; `vland.log`).
- Its one warning is the known labels-DB name of DELTA_NOTIONAL_ZERO; see NAME_FIXES.
- The real-object behaviour test passes.
- **Flags:** none.
- **Header changes:** none. `hdr/` and `hdr.diff` are empty, and newly_flagged_rows.tsv has the header line only.
- **Changes against p25C's batch:**
  - The bounded plain-C++ round the ruling asked for is done (below).
  - Line 2 is rewritten. It lists what the region suppresses, the measured flip frequencies and the graded families, and says the region is fitted and retail's construct is unproven.
  - The dead operand is now the fewest-token form, `||CTimer().GetTime()`, and its label carries the ruling's terms.
  - The declaration order is now an `// Inferred:` label citing the destruction order.
  - The loop-form label discloses Ego_d's loop head.
  - Dead-line mutants are added and reported separately.

## 1. The bounded plain-C++ round (2026-11-20 ruling)

**(a) Flip frequency, measured in the c2 trace** (`evidence/c2_freq_traces*.txt`; f = c2's static block frequency at the call site):

| callee at the site | c | expanded at | called at | formula `(c/f+6c)/7 >= 74` |
|---|---|---|---|---|
| ~CTimer (the two dtors of the target exit) | 14 | 0.0346 (`dw_all_head`) | 0.0313 (mask 245) | called when f <= 0.0323 |
| `IdleConversationTimer=5` operator= (next statement) | 20 | 0.0692 (`dw_all`) | 0.0346 (`dw_all_head`) | called when f <= 0.0503 |

Retail calls the dtors and expands the operator=. So the exit needs f <= 0.032 while the statement after it stays above 0.050.

**(b) Natural nestings, graded and traced:**
- **Plain check** (`none`): every in-loop exit sits at the main loop's exit share, 0.125 = 1/8 (sc 28).
  - `{}` block round the check: same, 0.125.
  - if/else form (`if(!T) IdleConversationTimer=5; else return`): same, 0.125.
  - All three DIFFER(559v585).
- **`do{ if(T) return; }while(0)` at the check:** the dtors take the frequency of the code that follows, which is the operator='s.
  - Measured pairs, dtors / operator=: 0.5 / 0.5 (`dw_tgt`), 0.0692 / 0.0692 (`dw_all`), 0.0346 / 0.0346 (`dw_all_head`), down to 0.0 / 0.0.
  - So the two flip together.
- **Plain at the check, `do{}while(0)` elsewhere:** the exit share rises to 0.33 or 0.5, and the operator= falls to 0.0, so it is called.
- **Sweep:** all 256 plain / `do{}while(0)` placements over the eight terminate checks were graded (`evidence/mask_sweep.tsv`, bit 0 = loop head ... bit 7 = target). None matches.

| result | count | what the trace shows |
|---|---|---|
| DIFFER(559v585) | 186 | target dtors expanded |
| DIFFER(553v585) | 53 | target dtors expanded, other shifts |
| DIFFER(579v585) | 11 | target dtors and `=5` operator= called, at one shared f (every one traced) |
| DIFFER(526v585) | 4 | as 579, and C3DVector::operator!= called |
| DIFFER(520v585) | 1 | as 526, and the `=1` operator= called |
| DIFFER(500v585) | 1 | target expanded, `=5` operator= and operator!= called |

**(c) /Od line sizes against Ego_d** (`egod_lines/od_calibration_*.txt`, VC7.1 `/Od /EHsc`):
- A scratch copy in the landed-Mains shape (each check on its opener's line, frame call in the `for` header) reproduces Ego_d's tail exactly:

  | Ego_d line | Ego_d size | statement | our size |
  |---|---|---|---|
  | 284 | 67 B | `if(MakeHenchmanPerformIdleComment()){if(IsActiveThreadTerminating())return _Fail_Val.Return();` | 67 B (15 + 52) |
  | 286 | 10 B | `IdleConversationTimer=5;` | 10 B |
  | 290 | 5 B | the loop's `}` | 5 B |
  | 292 | 52 B | the post-loop check | 52 B |

  The prolog matches too: 244 is 30 B.
- So Ego_d shows a bare check in that block, with no surplus and no extra construct.
- Ego_d is a later revision with 20 lines against retail's 27, so it cannot show what retail had there. Retail's construct remains unproven.

**Conclusion:** within the graded families (plain, `{}`, if/else, `do{}while(0)` at any subset of checks), no plain-C++ form gives the split. The region stays, as the smallest form: one statement, closed with `#pragma inline_depth()`, N = 0.

## 2. Inline-budget ablation (line 2, every device)

| device removed / changed (from the matching source) | grade |
|---|---|
| no `inline_depth(0)` region | DIFFER(559v585) |
| region at `inline_depth(1)` | DIFFER(559v585) |
| no region, /d2inlT20 | DIFFER(526v585) |
| no region, /d2inlT27 | DIFFER(532v585) |
| no region, /d2inlT30 | DIFFER(506v585) |
| no region, /d2inlT100 / S20 / S30 | DIFFER(559v585) |
| region kept, `return;` inside it (Return never emits) | RELOCATION_MATCH (neutral) |
| CTimer ctor declaration only (retail expands it at 0x00E5E368 / 0x00E5E37C) | DIFFER(588v585) |
| ~CTimer declaration only | DIFFER(561v585) |
| operator= declaration only | DIFFER(584v585) |
| GetTime declaration only | DIFFER(574v585) |
| no FABLE_RETAIL_C3DVECTOR_NOT_EQUAL (no declaration-only form) | SRC_FAIL |

**What the region suppresses:** only the two implicit ~CTimer calls of that `return`.
- IsActiveThreadTerminating 0x00CB7940 is never inline.
- The empty inline TRetVoid::Return emits nothing either way; a plain `return;` gives the same bytes.

Every row above was re-run on this source (`evidence/ablations.txt`).

## 3. The dead conditional temporary (Inferred, 2026-11-20 ruling, fifth row)

**Label:** starts `// Inferred: dead conditional temporary --`, in the header block and on the dead line. It says:
- the bytes prove a conditional temporary existed in dead code;
- they constrain neither its content nor its place.

**Place:** Ego_d line 292, the post-loop check.
- It has /Od bytes: 52, exactly the plain check at /Od /EHsc.
- It has no retail bytes, because the loop never exits normally.

**Tokens:** the forms graded on this source are below (`evidence/dead_forms.txt`; operand tokens counted without the `||`).

| form | grade | operand tokens |
|---|---|---|
| `if(T()\|\|CTimer().GetTime()==0)return _Fail_Val.Return();` (p25C) | RELOCATION_MATCH | 9 |
| **`if(T()\|\|CTimer().GetTime())return _Fail_Val.Return();` (chosen)** | RELOCATION_MATCH | 7 |
| `if(T()\|\|(CTimer(),0))return _Fail_Val.Return();` | RELOCATION_MATCH | 7 (tie) |
| `if(T()&&CTimer().GetTime())return _Fail_Val.Return();` | RELOCATION_MATCH | 7 |
| `if(T()?CTimer().GetTime():0)return _Fail_Val.Return();` | RELOCATION_MATCH | 9 |
| `if(T()\|\|CTimer().GetTime())return;` / `...==0)return;` | RELOCATION_MATCH | 7 / 9 |
| `if(T()\|\|CTimer().GetTime()==0);` (empty body) | RELOCATION_MATCH | 9 |
| `T()\|\|CTimer().GetTime();` (expression statement) | RELOCATION_MATCH | 7 |
| `if(T())return _Fail_Val.Return();` (no operand) | DIFFER(584v585) | none |

**Why this form:**
- The chosen form keeps Ego_d 292's statement as it is: the 52-byte plain check, with `return _Fail_Val.Return();` like the landed 0x00E72630 / 0x00E54E90. It adds the fewest operand tokens.
- The shorter whole-line forms (expression statement, empty if) drop the check that Ego_d 292 attests, so they were not taken.
- `(CTimer(),0)` ties on tokens. `CTimer().GetTime()` was taken because it uses only member calls the function already makes.
- **This choice is overridable by the coordinator.**

**Dead-line mutants, reported separately from the killed count** (`mutants/00e5e350.dead.*`):

| mutant | behaviour test | bytes |
|---|---|---|
| drop the operand | SURVIVED | DIFFER(584v585) |
| `\|\|` to `&&` | SURVIVED | RELOCATION_MATCH |
| delete the line | SURVIVED | DIFFER(584v585) |

The behaviour test cannot see dead code, so all three survive it. The bytes decide.

## 4. Other labels

**Declaration order** (Inferred): IdleConversationTimer, then CheckExpensiveThingsTimer. Ego_d lists CheckExpensiveThingsTimer first. Retail destroys CheckExpensiveThingsTimer first at both exits, the reverse of declaration:
- out-of-line exit: [esp+0Ch] at 0x00E5E580, then [esp+10h] at 0x00E5E58D;
- shared exit: DeregisterTimer at 0x00E5E564, then 0x00E5E573;
- [esp+10h] / edi is the timer set to 5 at 0x00E5E53D and read at 0x00E5E503.

Swapping the two declarations is a killed mutant.

**Loop form** (Inferred): `while(true)` with NewScriptFrame last.
- Ego_d's revision has the frame call on its loop-head line: there is no frame-call line before 290 `}`.
- Its head line 254 is 85 B. The generated for header with its check is 89 B, and `for(;;Game->NewScriptFrame())` is 76 B.
- Retail's bytes reject the for-increment form: `for(;;Game->NewScriptFrame())` gives DIFFER(579v585) and `for(;!false;...)` DIFFER(526v585).
- So here retail's loop differs from Ego_d's.

## Mutants and test

- **Mutants:** 18 of 18 killed (`mutants/00e5e350.out`).
  - 15 are in the source.
  - 3 are in C3DVector::operator!=. They run on a copy of the repo header in a scratch overlay; the batch overlay is empty.
- **Dead-line mutants:** reported in section 3, not counted here.
- **Test:** `test/00e5e350.cpp`, unchanged from p25C. It links the real row object, including the out-of-line ~CTimer COMDAT, and passes.
