# NF315 lessons (2026-11-23)

- **Intersect declarers with the exe's call sites before compiling.** For the SetPItem(CThing*) line, 43 global-model declarers
  shrank to 3 real retail callers of 0x00A01B90 (tools/setpscan.py + xsites.py); all three bind the spelling, while three declarers
  without a retail call bind nothing. A declaration grep alone would have meant ~40 compiles.
- **The exe scan keeps finding binders the table hides:** CombatPerformActionAndWait's CCombatActionBase* form had 3 binders (one
  witness), ClearOpinionSource's owner stand-in 6 (one witness, two of them off-limits, so the owner unit is held), ActivateQuest's
  reference / reference stand-in 2. Run delchk.py on every target before planning a unit.
- **A member-template declaration's /alternatename follows its parameter types.** Respelling `GetComponent(T**)` to `(const T**)`
  left .text equal but changed the compiler-emitted .drectve line (0x00738F90): check .drectve on every template respell before
  counting it done (NF303 Q2).
- **Local enums can carry made-up enumerator names under the right enum name** (0x006EBEE0: INNER_RING / OUTER_RING for MELEE / RANGED
  4 / 5); the mangled name looks right, so a census by spelling misses it. Replace them with the shared enum header.
- **Owner self pointer -> reference is a body respell, still byte-equal:** `creature->` -> `creature.`, `*creature` -> `creature`,
  pointer arguments to local stand-ins -> `&creature` (0x006EBDD0, 0x00843F50: ALL_EQ, COMDATs SAME).
- A clang `-Waddress-of-temporary` failure on an unchanged landed source disappears in the respelled source when the binder moves to a
  const-reference parameter (`&Temp(...)` -> `Temp(...)`); the HEAD source still needs its `cl /Zs` (NF300 D1 extension).
- **Screen T3_PORT_QUEUE by every column, not just the target column.** Six of my proven rows were named in column 3 / 4 of other ports' lines (binders those ports must change); offl.py v2 read column 2 only, so they were found at hand-back and moved to held_t3q/. offl.py v3 adds every 0x address on a queue line.
