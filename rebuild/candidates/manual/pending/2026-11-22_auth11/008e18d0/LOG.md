# 0x008E18D0 -- AUTH11 log (Step B of S/w22/auth11/BRIEF.md: measurement only, no grading hunt)
Base: AUTH10 proof/e4.cpp (copied to ./e4.cpp). Overlay hdr/ = AUTH10's (NParticleEngine_CParticleUpdateParameters.h, its orig ==
HEAD) + engine/CThing.h from HEAD with one add-only opt-in FABLE_RETAIL_CTHING_GETTC_EGOD_WRAPPER (pin_asserts: pinned 0).
Ego_d compiland: FableLib\debug\creature_modes.obj -> odall10 preset fablelib.
1. Calibration: cal/gettc.cpp (standalone) reproduces Ego_d CThing::GetTC<CTCDParticleEmitter> exactly: 9/21/17/15/4/2/6 = 74 B.
   Its text: HasTC; `CTCBase& base = GetTC(type)` (FableWin: base is CTCBase&); `*ptc = GFDebugPDynamicCast(&base, (T*)0)`
   (FableWin GFDebugPDynamicCast<CTCBase *,CTCDParticleEmitter *>(original_val, new_type), fastcall under -Gr); return true / false.
   The repo wrapper (e4's header) is one 31-B line forwarding to GetTC(id, ptc): contradicted.
2. e4 measured (e4.od10.txt), e5 = e4 + the correction set measured (e5.od10.txt). Table in MEASURE.md.
3. For-scope idiom: Ego_d 3302 / 3332 = 39 B = e4's 30 + 9 (near exit jump). inl readouts (no forcing, /Gr /d2inlT65) of
   e4i (= e4 + idiom only) and e5 against AUTH10's proof/e4.f.json (same flags; f / sc recorded before forcing).

## PRE-REGISTERED (before the one grade)
- Graded text: e5.cpp = e4 + {C1 GetParentThing() for the six ParentThing accesses; C2 storm_rotation split (Ego_d 3288/3291/3292
  = 15/11/9) with beam_length after it (3294 = 15); C3 rhs declared before weapon_pos (3296 / 3297 = 8 / 27); C4 the VC6 for-scope
  idiom on both loops (3302 / 3332 = 39); C5 the Ego_d GetTC<T>(T**) wrapper (opt-in, calibrated 74 B)}. Nothing else.
- One grade: q.py 008e18d0 e5.cpp, stock VC7.1, the row's declared flags (`// Flags: none`), FABLE_HDR_OVERLAY=hdr, no c2 hook,
  no /d2inl switch. No re-grade, no variant, whatever the result.

## Results
4. Witness: pp/e5.i holds the GetTC wrapper opt-in body. Graded ONCE: DIFFER(1948v2073), mdiff 287 masked diff ins. STOP.
   Grades used 1; inl readouts 2 (e4i, e5); VC10 measurements 4 (e4, e5, cal/gettc, cal/e5_assert).
5. After advisor review: an unforced e4 readout added (same flags as e4i / e5) so the f table compares like with like; site
   labels re-derived from the row's own decisions (pipe events close a function; inl/row.py). MEASURE.md corrected. Readouts 3.
