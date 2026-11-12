# Lessons, chunk 2032

- 0x009C47A0: `if (filters && !filters->empty())` leaves a zeroed esi. Retail takes the iterator inside the guard:
  `if (filters != NULL && (it = filters->begin()) != filters->end()) for (; it != end; ++it)`.
- 0x009C47A0: an `&&` guard puts the guarded loop entry at c2 frequency 0.25. That decides which callees inside the loop are expanded
  (CFileInfo ctor, CCountedPointer ctor, push_back) and which stay called at single sites.
- 0x009C47A0: `filename == L""` at f 1.0 scores 78 for the inner CWideStringData==. Both == layers need FABLE_FORCEINLINE
  (opt-ins _FORCEINLINE_EQ + _FORCEINLINE_DATA_TEXT_EQ). No /d2inlT fits, because push_back (77) and the ctor (76) must stay called.
- 0x009C47A0: the std::set insert needs the STLP_MAP / _TREE insert_unique body visible. Its pair temporary shares the frame slot,
  which gives retail's 0x74 frame.
- 0x009C47A0: the STLport `operator==(basic_string, const C*)` must be the inline template body, not the out-of-line 0x00404280
  specialisation. A `(finish-start)&~1` size test plus compare(...,0) in place means the inline body.
- 0x00BA6FF0: the squared camera distance accumulates X, Z, Y (0x00BA7101..0x00BA7111) only with a running sum
  (`s = x*x; s += z*z; s += y*y`). The one-line sum is reordered Z, Y, X.
- 0x00BA6FF0: the FindLocalLights channel is by value; a default temporary is copied into the same frame word
  (0x00BA71C7..0x00BA71DB). It needs FABLE_FORCEINLINE (score 98 at f 0.125).
- 0x00BA6FF0 (open): c2 forwards a single-use named float local (distance) past a later independent statement (radius). A second use
  pins the order but spills the value. The spelling that keeps retail's order unspilled is not yet found.

## Review at landing
- The whole-file fable_string.h overlay: re-base it on HEAD and keep the new opt-in only.
- Agent2028's CalcDistanceFromCamera body differs from this row's running-sum body; only one can be the header body.
- Row 0x00BA6FF0 was parked after pass 1 only (5 grades). Passes 2 and 3 are still open.
- Row 0x009C47A0 needed 8 full grades (4 explore, 1 match, 1 confirm, 2 regrades after overlay edits).
