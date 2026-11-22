# ESCALATE 0x008E18D0 NCreatureMode::CModeThunderLightningStorm::UpdateLightningBeams (2073 B) -- AUTH10, 2026-11-22
W = $S/w22/auth10/008e18d0 (overlay W/hdr = AUTH9's add-only IMPLICIT_DTOR opt-in, base W/orig == HEAD header; hdr.diff applies to HEAD).

## New this pass (the residue is now proven to be ONLY four c2 inline decisions)
1. New tool `W/../tools/inl_force.py` (+ `force_host.py`, `fv.sh`, `gobj.py`, `mdiff.py`): the p25N inl.py hook plus a forced
   result per (callee, occurrence) at c2 0x1078CFDA (and an optional per-phase frequency scale). Forcing retail's split on AUTH9's b0
   reproduces retail's call sequence exactly (calls.py), but b0 still DIFFERs (2057v2073, 169 masked diff ins, frame slots differ).
2. The other difference was the sky's end_pos statement. Ego_d has ONE line (3358, 49 B) there; b0 had two statements. With
   `C3DVector end_pos = start_pos + C3DVector(0.0f, 0.0f, 15.0f);` (e2) the forced build is **RELOCATION_MATCH** (gobj.py, V.mask).
   Retail materialises end_pos (X/Y dword copies to 0x1C/0x20, Z+15 to 0x24); `end_pos = start_pos; end_pos.Z += 15` is copy-propagated.
3. Best source `W/e3.cpp` = e2 + Ego_d's dead locals lifetime_in_seconds / lifetime_of_main_beams (lines 3283/3285, the landed 0x008DD7E0
   spelling, needs CGAMECOMPONENT / CMAINGAMECOMPONENT + 2 includes). Forced (`W/force1.json`, /Gr /d2inlT65): RELOCATION_MATCH.
   Unforced: default 2061v2073; /Gr T64..66 2165v2073 (285 masked diff ins); /Gr T63 1992; /Gr T>=68 2293.
4. PROVEN SOURCE `W/proof/e4.cpp` = AUTH9's b1 (NParticleEngine::MAKE_VECTOR_PARAM_FROM_BASE + VECTORKEY_PASS26, the two-source spelling)
   + e3's deltas. `fv.sh proof/e4.cpp force1.json /Gr /d2inlT65` -> `proof/gobj.txt` RELOCATION_MATCH; `proof/forced_sites.txt` = the
   four hits (decision #314 ~CParticleUpdateParameters k1 f2.5; #340 LowerBound k3 f6.25 = loop 2 (k2 loop 1, k4 sky); #419 / #456
   __uninitialized_copy k3 / k5 f3.125 in the sky); `proof/leafrel.txt`: 1 problem = this obj's ~CParticleUpdateParameters COMDAT
   (35 B, inner ~vector called at T65) NEQ retail 0x006E2500 (inner expanded; known, the COMDAT came from another obj).
   e4 unforced /Gr T65: DIFFER(2044v2073) (`proof/unforced_grade.txt`; differs from e3's 2165 there, not analysed).

## The four decisions (e3, /Gr /d2inlT65; f / score / ours -> retail)
| site (occurrence) | f | score | ours | retail |
| ~CParticleUpdateParameters, loop 1 (k1) | 2.5 | 23 (dtor COMDAT small: its inner ~vector<pair<ulong,ParamDesc>> scores 65 at T65 -> call) | expand | CALL 0x006E2500 |
| CVectorMap<ETCInterfaceType,CTCBase*>::LowerBound, loop 2 (k3) | 6.25 | 63 | expand | CALL 0x0040F020 |
| __uninitialized_copy<pair<ulong,C3DVector>>, sky Insert #1 remainder copy (k3) | 3.125 | 68 | call | EXPAND |
| same, sky Insert #2 (k5) | 3.125 | 68 | call | EXPAND |
Everything else (LB loop 1 / sky, overflow loop 1 call / sky expand, fill_n, Calc, Insert, Update, sky dtor + inner ~vector) already agrees.

## Why no single T works (scores from inl.py; retail must satisfy all at once)
- overflow loop 1 call at 66 => T <= 66; sky uninit copy expand at 68 => T > 68. Contradiction.
- LB loop 2 call and LB sky expand at the identical record (sz 60, c1 52, f 6.25, score 63). Contradiction at any T.
- loop-1 dtor call needs the BIG dtor (inner ~vector expanded, needs T >= 66), then scores 65 at f 2.5 => call needs T <= 65.
So retail behaves as if the sky's sites are about 4x hotter than the loop bodies' (or the loops 4x colder), not 2x.
Frequency-scale diagnostic (inl_force fscale, crude: nested sites compound): loops x0.5 at T65 fixes LB loop 2 only; loops x0.25 at
T69/70 fixes LB loop 2 + the sky copies but turns loop-1 Insert (0.625/78) and bitset::test into calls (retail expands both); loops x0.5
+ sky x2 at T69: 184 masked diff ins. No uniform (factor, T) found; a per-site fit would be fitting, so I stopped there.

## Measured negative this pass (do not repeat)
- Sibling 0x007A1380 opt-ins one at a time on b0 (CVECTORMAP_INSERT_FORCEINLINE, CTHING_ORIGINAL_VTABLE, CINTELLIGENTPOINTER_ASSIGN /
  _ASSIGN_DIRECT / _DEREFERENCE, C3DVECTOR_PACKED / _ORIGINAL_COPY_FORMAL, CTCDPARTICLEEMITTER_UPDATEPARAMETERS): byte-neutral;
  STLP_VECTOR_OVERFLOW_OUTOFLINE 1925, FABLE_STLP_OUTOFLINE_UNINIT_COPY 2184, _FILL_N 2189 (worse).
- Loop spellings on minis (`!=`, `k > n`, post-increment, unsigned, no braces, if/else, `continue`, constant bound, member bound, no-init
  for): identical f (for body x5, after x5). Guarded do-while: in x5 / after x1; while: x9 / x1. No loop form makes after > body.
- c2 switches -inlfcsw / -inlflcsw / -inld / -inlnobr / -inlnild / -inlniln change no f; -inlld lowers scores uniformly. Flags /Os /Ot
  /Ox /G6 /G7 /GB /Oa /Ow /Op /Ob1 /GX /GS /O1 on e2: no match. The sky in do{}while(0) on e3 (device, not landable): best 2133 at T60..62.
- The const LowerBound is a separate retail body (0x004365B0), so "loops call the const overload" is out: loops call 0x0040F020 (non-const).
- Ego_d: n / num_beams / num_beams_ends are long; all three ptc locals one type; loop headers 39 B each; GetTC lines 517/544/534 B with an
  out-of-line Ego_d GetTC<CTCDParticleEmitter> (74 B) also present, so Ego_d expanded GetTC at these sites. No hidden if in the loops.

## Hypotheses for what retail did
(a) The TU was built with inline parameters we do not reproduce (a per-TU /d2inl* knob, or the frequency weights of another c2 build),
    with the same text: the loop-vs-sky ratio is systematic (also visible at the TU-mates, p25N calibration: 2.5/65 dtor and 2.5/70 LB
    called elsewhere in creature_modes.obj while the sky expands at 6.25/63).
(b) A zero-byte construct in the loops' path to GetTC / Insert that adds two x0.5 factors (a folded constant branch in a helper, like
    bitset::test's range check that folds with the constant 0x4B) and that VC10 /Od also leaves invisible. None found.
(c) Two TUs / a different compile order (the dtor COMDAT 0x006E2500 lies outside this obj's range), which explains the dtor only.

## Question for the advisor
The text is pinned up to inline decisions: e4 + four forced decisions = RELOCATION_MATCH. Ego_d: the sky line 3358 is consistent with one
statement, but the VC10 size (49 B; also 3283 = 44 B, 3285 = 26 B, the unexplained 27 B at 3297) is NOT measured with odall10.py yet
(first next step for whoever resumes).
Is the row landable as `proof/e4.cpp` with its best plain grade recorded as a bracket (forced-decision proof attached), or does it wait on the
Windows/c2 frequency question? If a hunt should continue, which is preferred: (1) reverse c2's site-frequency builder (where f is set
before 0x1078CFD5) to find what input gives a 4x loop/sky ratio, or (2) a bounded search of header-level helper forms (Insert /
operator[] / IS_THING_ALIVE) checked against Ego_d? Reproduce: `$S/w22/auth10/tools/fv.sh W/e3.cpp W/force1.json /Gr /d2inlT65`.
