# Landing 2026-11-06u: chunk 820 (2 of 6 rows landed, 1 held, 3 parked)

One expert agent authored the rows, sharing the lane with the chunk 821 agent. All six rows were retries of earlier parks, started
from their parked sources. Their names were already proven. Both landed rows are plain C++ and grade `genuine`.

| Address | Name | Grade |
|---|---|---|
| 0x00D7B510 | NScript::CQ_HeroSoulsThunderScript::CHeroThunder::Main | RELOCATION_MATCH PASS (3 attempts) |
| 0x00E64210 | NScript::CV_ChickenKickingScript::CKickedChicken::Main | RELOCATION_MATCH PASS (25 attempts) |

- **CHeroThunder** uses the chunk 817 else-wait idle loop: `if (T) return; for (;;) { NSF; if (T) return; }`.
- **CKickedChicken** builds its direction vector member-wise: `float dy = ...; float dx = ...; C3DVector direction;`, then the X, Z
  and Y stores. An inlined Dot follows how its operand was built (GOTCHAS chunk 820).
- **Held, not landed: 0x00DA91A0 `NScript::CQ_MinionClifftopChaseScript::CMapKeeper::Main`.** It reaches RELOCATION_MATCH, but only
  through `if (!reinterpret_cast<const CScriptThing&>(escort).IsAlive())`.
  - VC7.1 devirtualises a virtual call on a by-value class local through every ordinary spelling. The same-type
    reinterpret_cast keeps the call virtual and computes `this` first, as retail does.
  - This is the cast-only form of the I-44 CScriptThingView device. No new row may land with that device without owner review.
  - The matching source and test are kept in `pending/2026-11-06_linux_hand820/differ/00da91a0.cpp` / `.test.cpp`.
  - The question for the owner is in HANDOFF. If it is approved, the cast could also replace the view struct in the six landed
    CScriptThingView rows. If it is not, the row stays at its chunk 797 park (2 lines).
- **Headers:**
  - Nothing is copied into rebuild/include; neither landed row uses the overlay.
  - `pending/2026-11-06_linux_hand820/hdr/` carries chunk 818's ChickenMaster and FORCEINLINE_ASSIGN opt-ins unchanged, for the
    parked 0x00E64FB0.
- **vland:**
  - The new binding `?GetDistanceBetweenThingAndPosition@@YIMABVCScriptThing@@ABVC3DVector@@@Z` -> 0x00CBE551 has no second address
    and no second symbol.
  - Warnings:
    - RELOC_NAME at 0x0099E4C0.
    - DELTA_NOTIONAL_ZERO at 0x0129BA3C, a catalog global labelled EPSILON (naming question in HANDOFF).
    - RELOC_ALIAS for rand and _CIacos.
- **Names:**
  - The agent's only label line (0x00CDBFB0, the std::map dtor placeholder) is already queued, so there is no labels file and no
    queue line.
- **Landing checks:**
  - `vland --land` ran (`logs/land.log`); payload.json / oracle.tsv here exclude the held 0x00DA91A0.
  - The manifest sync dry run prints 0.
  - bindings_multi_check prints only the known NullCoord case.

## Parked (`pending/2026-11-06_linux_hand820/`)
| Row | Grade | Residue |
|---|---|---|
| 0x00DD5A60 NScript::CQ_PrisonEscapeRescueMotherScript::CPrisoner::Main | DIFFER(2246v2250), 21 lines, 21 attempts | retail calls ~CTimer out of line at the GUARDS hit-arm exit; ours expands it, which cross-jumps into the final block (inline-frequency blocker) |
| 0x00E64FB0 NScript::CV_ChickenKickingScript::CChickenMaster::Main | DIFFER(15210v15210), 16 lines, 10 attempts (20 over chunks 818 and 820) | global register colouring: eax and edx swapped at two vptr loads |
| 0x005E78F0 CTCQuestCompletionUI::GlobalUpdate | DIFFER(2340v2340), 4 lines, 10 attempts | x87 operand order |
