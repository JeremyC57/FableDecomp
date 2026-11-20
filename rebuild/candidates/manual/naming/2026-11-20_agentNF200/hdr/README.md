# NF200 overlay headers (2026-11-20)

Base copies of every overlaid header are in ../orig/ (copied from the repo working tree when the overlay was made). Every opt-in is
add-only, inside a new member-specific #ifdef; with the macros undefined each header is token-identical to its orig/ copy except
for the asserts pin_asserts.py pinned (same line numbers as the base, so the same typedef names). NF198's overlay carries only
engine/CThing.h and NF199 has none, so no member here is declared twice.

## engine/CTCFireballSpell.h (copy of rebuild/include/engine/CTCFireballSpell.h)
- FABLE_RETAIL_CTCFIREBALLSPELL_MEMBERS (inside FABLE_RETAIL_CTCFIREBALLSPELL): the data members in the Ego_r.pdb order / spelling with the
  retail 0x34-byte timer (Timer +0x0C .. SpellFinished +0x7C, sizeof 0x80; new FABLE_STATIC_ASSERT after the class, under the macro), plus the
  includes they need (fable_string.h, CWillButtonHelperTimer.h, CIntelligentPointer.h, CDefPointer.h). Used by rows/007c9180.cpp.
- FABLE_RETAIL_CTCFIREBALLSPELL_FINISHSPELL: declaration `void FinishSpell();` (0x007C9180; Ego_r.pdb 0x007C7F80 + FableWin.pdb 0x02721680
  ?FinishSpell@CTCFireballSpell@@QAEXXZ). Used by rows/007c9180.cpp.
- pin_asserts.py: pinned 0 (the new sizeof assert is under the new macro only).
- Landed includers: 5 rows (grep -rlE 'engine/CTCFireballSpell\.h' rebuild/src/compiled); none defines either macro, so the opt-ins change none of them.

## engine/CCreatureAction_FinishFireballSpell.h (copy of rebuild/include/engine/CCreatureAction_FinishFireballSpell.h; generated header)
- FABLE_RETAIL_CCREATUREACTION_FINISHFIREBALLSPELL_CLASS: the polymorphic retail class on CCombatActionBase (RTTI CCombatActionBase@0, sizeof
  0x114) with the two PDB constructors (0x008BCE10 / 0x008BCE50), in the shape of CCreatureAction_CastWillLightningSpell.h _MAZE; the generated
  opaque class moved under `#else` unchanged. Used by rows/007c9180.cpp.
- pin_asserts.py: pinned 2 (the generated sizeof / __vftable asserts now FABLE_STATIC_ASSERT_AT(30, ..) / (31, ..), their base lines).
- Landed includers: 1 row (0x008BABE0); it does not define the macro, so the opt-in changes it in nothing but the pinned (same-name) asserts.

## engine/NGraphicAppearance_CTCGraphicAppearanceReplaceableMesh.h (copy of the repo header)
- FABLE_RETAIL_CTCREPLACEABLEMESH_REPLACEMESHBYINDEX (inside FABLE_RETAIL_CTCREPLACEABLEMESH_PASS22): declaration
  `void ReplaceMeshByIndex(unsigned long index);` (0x007A69F0; Ego_r.pdb 0x007AB780 + FableWin.pdb 0x026C4B30
  ?ReplaceMeshByIndex@CTCGraphicAppearanceReplaceableMesh@NGraphicAppearance@@QAEXK@Z). Declaration only, no layout change. Used by rows/006412b0.cpp.
- pin_asserts.py: pinned 1 (the sizeof assert, now FABLE_STATIC_ASSERT_AT at its base line).
- Landed includers: 1 rows; 0 define the new macro, so the opt-in changes none of them (the pinned assert keeps its typedef name).
