# NF75 header overlay (round 7, land1117fu q10910..q10944)

Each file is a copy of the current repo header with `#ifdef` opt-ins added. The base copies are in `../orig/engine/`, and at
hand-back each one was cmp-identical to `rebuild/include/engine/`. With no macro defined, every header is token-identical to the
repo text. The macro names are member-specific so they can be merged with the NF70..NF74 overlays.

| Overlay file | Opt-in | Used by |
|---|---|---|
| engine/CCombatActionBase.h | `FABLE_RETAIL_CCOMBATACTIONBASE_FRAMEUPDATE`: `public: virtual void FrameUpdate(); // 0x00859D80`, added in both retail branches (the MELEE branch and the default branch). Ego_r.pdb 0x0083A360 and FableWin.pdb 0x0282A4B0 both give ?FrameUpdate@CCombatActionBase@@UAEXXZ (public virtual, slot 56). Declaration only; no layout change. | 0x0085BAD0, 0x008E6390 |
| engine/CCreatureAction_CastStrengthMultiStrike.h | `FABLE_RETAIL_CCREATUREACTION_CASTSTRENGTHMULTISTRIKE_CLASS`: the polymorphic retail class `: public CCombatActionBase`, with FinishAction (slot 4, 0x008E6AE0), FrameUpdate (slot 56, 0x008E6390) and `protected: long NumberOfStrikes` (+0x114, sizeof 0x118). The repo's flat layout is the `#else` arm, unchanged. | 0x008E6390 |
| engine/CMap.h | One line inside the existing `#ifdef FABLE_RETAIL_CMAP_THEMEPALETTE` block: `public:` before AddThemeDefIndexToPalette became `protected:`. Ego_r.pdb and FableWin.pdb both have ?AddThemeDefIndexToPalette@CMap@@IAEEK@Z. The default path is unchanged. All 9 landed THEMEPALETTE users were regraded on the overlay: 0x0081F910 gives MATCH and the others give RELOCATION_MATCH (recheck_final.log). | 0x008F70F0 and the 9 CMap users (not-a-binder) |
| engine/CTCFadeOutAndIn.h | `FABLE_RETAIL_CTCFADEOUTANDIN_SDD_FRIEND`: a friend declaration of the landed free-function SDD stand-in `CTCFadeOutAndIn_CompositeScalarDeletingDestructor`, so it can reach the protected FadingThings by name instead of through an offset cast. Declaration only. | 0x006C4430 |

Static asserts: pin_asserts.py ran on all four overlays.
- **CCreatureAction_CastStrengthMultiStrike.h:** pinned 3 asserts, now `FABLE_STATIC_ASSERT_AT(80/81/82, ...)`, pinned to their repo line numbers. They sit in the `#else` (repo) arm. The opt-in arm has no `__vftable` member, so the asserts do not apply there; sizeof 0x118 holds by construction (0x114 + 4).
- **Other three overlays:** pinned 0 asserts. No assert was moved.
