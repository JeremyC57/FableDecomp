# NF241 header overlay (2026-11-22)

One header, **comment-only** (no token changes): `stlport/fable_containers.h` (base copy: `orig/stlport/fable_containers.h`, the repo
tree at the start of this run). Two comments rewritten in place, each on the same number of lines as before (every line below keeps
its number; `FABLE_STATIC_ASSERT` line numbers identical, `pin_asserts.py` pinned 0):

1. `FABLE_RETAIL_STLP_LIST_DTOR_IN_LIST` (nf239 ruling 3, authorised): its rationale cited FableWin.pdb's `??1?$list@PAVCComponent..`
   and the absence of `??1?$_List_base`; both PDBs carry a Dinkumware list (FableWin: 0 `_List_base`, 14,069 `_List_val` symbol
   lines; Ego_r: 0 / 1,258 in `$S/codex27/pdbx/*.symbols.txt`), so they cannot arbitrate STLport layering. The comment now says the
   opt-in is kept for its bytes only and cites retail 0x0042B6D2 (list's own `~list() {}`, `jmp 0x0042ABCA`). Row 0x00607330 (its
   first user) no longer sets it; four other landed rows still do (0x00CDDCB0, 0x00936C70, 0x0053C9BC, 0x004B8940).
2. `FABLE_RETAIL_STLP_LIST_DEFAULT_CTOR_NOINLINE` (NOT in the brief: a decision for the coordinator, drop it if unwanted): its comment
   named the Adjust*OptionsSliders rows and 0x0042ABAF as list's own constructor; those rows now use `_LIST_BASE_VISIBLE_NOINLINE` and the
   comment says 0x0042ABAF is _List_base's (list's own 0x0042B6A6 calls it). One landed row (0x00CDDCB0) still sets the opt-in.
   (A rewrite of the DTOR_OUTOFLINE comment was reverted to the base text: at HEAD its out-of-line copies are mostly the 5-byte jmp to
   ~_List_base, so "the copy's body is ~_List_base's" was not true in general. 19 other landed rows still set that opt-in.)

Equivalence: the overlay is token-identical to the base after stripping comments (tools: inline python token compare, "token-identical"),
so no includer's preprocessed token stream changes (2244 landed rows include it); every changed row was graded with the overlay on
(FABLE_HDR_OVERLAY) and its relocation list compared with the HEAD compile.
