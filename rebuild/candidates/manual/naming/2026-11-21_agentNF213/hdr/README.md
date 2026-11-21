# NF213 header overlay (add-only opt-ins; `orig/` = repo working tree at start, byte-equal to HEAD when checked)

## stlport/fable_containers.h (base: orig/stlport/fable_containers.h)
Two blocks appended at the end of the header (inside namespace std; each closes std, forward-declares `namespace NUISystem { class
CComponent; }`, reopens std). Without the macros the text is token-identical to the base (the blocks are wholly inside `#ifdef`).
- `FABLE_RETAIL_STLP_VECTOR_OVERFLOW_NUISYSTEM_CCOMPONENT_SIZE_BODY`: explicit specialization of
  vector<NUISystem::CComponent*>::_M_insert_overflow (__true_type) with the STLport _vector.c body, `inline` (a LINK_ONCE_DISCARD COMDAT like the owner's) and noinline, under
  `optimize("y", off)` + `optimize("s", on)` (closed by `optimize("", on)`), and a declaration-only explicit specialization of the
  __fastcall fill_n<CComponent**, unsigned int, CComponent*> (0x0048514C). Fidelity: scaffolding, not retail. Owner 0x00485DBC sets the
  same pragmas. Needs FABLE_RETAIL_STLP_FILL_N_FASTCALL + FABLE_RETAIL_STLP_COPY_TRIVIAL_OUTOFLINE, and a per-row `/d2inlS12` so the
  copy expands allocator::allocate -> malloc (S11 and below: 135 v 137). Used by 0x005414A0 (cmpbody 137 = 137, bytes + relocs).
  Measured: with the macro removed the row body grades the same (RELOCATION_MATCH) and only that COMDAT changes (167 v 137).
- `FABLE_RETAIL_STLP_VECTOR_PUSHBACK_NUISYSTEM_CCOMPONENT_SIZE_BODY`: explicit specialization of vector<NUISystem::CComponent*>::push_back
  (the default in-class body text) under the same pragmas. Owner 0x00486024 sets them. Used by 0x00541390, 0x005414A0, 0x00563130
  (cmpbody 49 = 49 each; without it 50 v 49).
- A row using either must not carry a row-head optimize pragma before its include of the header (nf189). Under Wine/MSVC a quoted
  include from a repo header resolves to the repo copy first (includer-directory search), so a row must include
  "stlport/fable_containers.h" itself before any repo header that includes it, or the overlay is bypassed while grading (0x00563130).
- pin_asserts.py: pinned 0. Landed includers: 2192 files under rebuild/src/compiled; none defines either macro (grep), so the opt-ins
  change none of them.

## engine/CTCHeroOnlineScoreboard.h (base: orig/engine/CTCHeroOnlineScoreboard.h)
- `FABLE_RETAIL_CTCHEROONLINESCOREBOARD_SHOWSCOREBOARD` (right after StatGetInterfaceType, public): `void ShowScoreboard();`
  (Ego_r.pdb / FableWin.pdb ?ShowScoreboard@CTCHeroOnlineScoreboard@@QAEXXZ) and the inline `CCharString GetSignedInPassword()` beside
  the header's GetSignedInUserName (CUserProfileManager::GetScoreboardPassword().ToCharString()). Needs _NETWORK. Declarations / inline
  accessor only, no layout change; default path token-identical. Used by 0x00563130.
- pin_asserts.py: pinned 0. Landed includers: 27; none defines the macro.
