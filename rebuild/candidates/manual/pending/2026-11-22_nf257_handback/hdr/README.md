# NF257 header overlay (2026-11-22)

Base copies: `../orig/` (repo working tree at the start of NF257, byte-equal to rebuild/include; `cmp` re-checked at hand-back).
Every change is ONE add-only, member-specific `#ifdef` block under a NEW macro; no existing line is edited. Default path token-identical.

## engine/CCombatActionBase.h (q12722, L72)
- Adds `FABLE_RETAIL_CCOMBATACTIONBASE_MELEE_PRIORITYCTOR` inside the MELEE branch (after MELEE_COPYCTOR):
  `protected: CCombatActionBase(long priority);` (Ego_r.pdb 0x00838AC0 + FableWin.pdb 0x02823960 `??0CCombatActionBase@@IAE@J@Z`, retail
  0x00857E40; the third branch's FABLE_RETAIL_CCOMBATACTIONBASE_PRIORITYCTOR declares the same constructor). A new name, not PRIORITYCTOR,
  because landed 0x008B9010 defines both MELEE and PRIORITYCTOR. Declaration only; its own access label; the next line keeps `public:`.
- Asserts: the one after the block was already `FABLE_STATIC_ASSERT_AT(245, ...)`; the branch-1 assert (line 133) is above it (pinned 0).
- Includers: hundreds (every CCombatActionBase.h user); no landed row defines the new macro. Whole-tree equiv_check with this header and
  CCreatureAction_SummonCreatures.h: 0 of 57,125 rows differ (../evidence/equiv_l72.log); with all four headers: ../evidence/equiv_final.log.
- Users: rows/008b83c0.cpp (and the other four L72 rows, same prelude text).

## engine/CCreatureAction_SummonCreatures.h (q12722, L72)
- Adds `FABLE_RETAIL_CCREATUREACTION_SUMMONCREATURES_OVERRIDES` after the existing constructor: the default and `(CThingCreatureBase&)`
  constructors (Ego_r 0x008F1140 / 0x008F11C0 + FableWin 0x029FF160 / 0x029FF3A0; retail 0x008B83C0 / 0x008B8440), `virtual Clone() const`
  (Ego_r 0x008F1520 + FableWin 0x029FF1A0, slot 11 = 0x008B89A0) and `virtual Initialise()` (Ego_r 0x008F0930 + FableWin 0x029FF450, slot 12
  = 0x008B7A20). Declarations only; sizeof 0x118 still asserted (already `_AT(33)`; pinned 0).
- Includers: 0x0091C6A0 (+ its test) only; it does not define the macro.
- Users: rows/008b83c0.cpp, 008b83e0.cpp, 008b8440.cpp, 008b89a0.cpp, 008b87a0.cpp.

## engine/CCombatAction_WillHealInto.h (q12723, L100)
- Adds `FABLE_RETAIL_CCOMBATACTION_WILLHEALINTO_OVERRIDES` after the constructor: `virtual FinishAction()` / `virtual Initialise()` (public;
  Ego_r 0x008FAF00 / 0x008F8380 + FableWin 0x02A11440 / 0x02A113C0; slots 4 / 12 = landed 0x008C2AD0 / 0x008C0560) and, under its own
  `private:`, `virtual Clone() const` (EBE; Ego_r 0x0097B570 + FableWin 0x02BF9AD0; slot 11 = 0x00956A20). Declarations only; the existing
  `private: long SpellLevel;` keeps its label; assert already `_AT(13)` (pinned 0).
- Includers: 0x00955620 (+ test) and 0x006F8010; neither defines the macro.
- Users: rows/00956870.cpp, 00956a20.cpp, 00956940.cpp.

## engine/CAIStateGroup_WhisperRetreat.h (q12724)
- Adds `FABLE_RETAIL_CAISTATEGROUP_WHISPERRETREAT_UPDATE` at the head of the FABLE_RETAIL_CAISTATEGROUP_WHISPERRETREAT class: `public: virtual
  void Update();` and `protected: void JumpToSafety(); void UseWill();` (Ego_r + FableWin `?Update@...@@UAEXXZ`, `?JumpToSafety@` /
  `?UseWill@CAIStateGroup_WhisperRetreat@@IAEXXZ`; retail 0x0096AC20 / 0x0096AE50). Declarations only; the existing `protected:` label
  follows the block.
- pin_asserts pinned 1: the retail-branch `FABLE_STATIC_ASSERT(sizeof(...)==0x30)` -> `FABLE_STATIC_ASSERT_AT(22, ...)` (old typedef name
  kept); the else-branch asserts were already `_AT`.
- Includers: 0x008FE547, 0x0090188E, 0x0096A260, 0x0121F86F (rebuild/include: INDEX.tsv only); none defines the new macro.
- Users: rows/0096b600.cpp.
