# NF287 overlay headers

## engine/CAIStateGroupBase.h (base: orig/hdr/engine/CAIStateGroupBase.h = repo HEAD copy, 2026-11-23)
- New add-only opt-in `FABLE_RETAIL_CAISTATEGROUPBASE_FLAT_AIACTION_LOOKATPOS`, in the flat (`#else`, pack(1)) branch only:
  - before the class: forward declarations `class C3DVector; enum EAIActionReturn; enum EAIActionRepeat;` (MS forward enums);
  - first thing in the class: `protected:` + the declaration (no body, no virtual, no data) of
    `EAIActionReturn AIAction_LookAtPos(EAIActionRepeat, C3DVector, long, float)` 0x0097D580, then the original `public:` line.
  - Spelling: Ego_r.pdb 00848330 + FableWin.pdb 02891410 `?AIAction_LookAtPos@CAIStateGroupBase@@IAE?AW4EAIActionReturn@@W4EAIActionRepeat@@VC3DVector@@JM@Z`
    (a member's own mangling carries no class key, so the flat view gives the PDB `IAE` spelling; the retail branch already declares it
    under FABLE_RETAIL_CAISTATEGROUPBASE_MEALACTIONS, but that branch needs FABLE_RETAIL_CAISTATEGROUPBASE and the full retail model).
  - User: row 0x0094A6D0 only (its CAIStateGroupGoToPubMethods derives the flat class, so the protected member is reachable).
- No existing line edited; the default path is token-identical. No layout change (a non-virtual member function in a pack(1) class).
- pin_asserts.py: 0 pinned (every assert in the header is already `FABLE_STATIC_ASSERT_AT`).
- equiv_check (evidence/equiv_e2.txt): the header's transitive landed includers plus every row that includes a relative source path;
  0 differ. The planted-token control (a typedef at the top of the flat branch, outside the opt-in) flags the flat-view rows.
- Other agents' overlays: NF286's hdr/ held no header when checked (2026-11-23, after the restart).
