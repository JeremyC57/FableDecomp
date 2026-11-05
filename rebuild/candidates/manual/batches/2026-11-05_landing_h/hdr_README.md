# Chunk 781 header overlay

## engine/CQ_HangingTreeEvilScript.h (default path, members added to one nested class)
- What changed: the nested `CQ_HangingTreeEvilScript::CHTE_FreedomFighter` was a stub declaring only `Alloc`. It now carries
  the retail layout and its virtuals:
  - `GetParentScript() const` (public virtual, slot 3 0x00D6A470);
  - `ParentClass` +0x14, `PMasterData` +0x18 and `HeroHitMe` +0x1c;
  - the private virtuals `Main` (slot 1 0x00D6A560), `Init` (slot 2 0x00D6A480) and `OnPredicateFail` (slot 5 0x00D6A4F0).

  A `// Retail layout (chunk 781)` evidence comment and `FABLE_STATIC_ASSERT(sizeof(...CHTE_FreedomFighter)==0x20)` were
  added too. Nothing else in the file changed (`diff` against the repo copy shows only that block plus the assert line).
- Evidence:
  - Ego_r.pdb struct_layouts_egor.tsv CHTE_FreedomFighter is 32 bytes: ParentClass at 20 (`CQ_HangingTreeEvilScript *`), PMasterData at 24 and `long HeroHitMe` at 28.
  - Factory 0x00D6A3D0 does `push 20h`.
  - Vtable 0x012D2170 is [0x00D6A540, 0x00D6A560, 0x00D6A480, 0x00D6A470, 0x00CDEBC0, 0x00D6A4F0, 0x00CDEBE0].
  - FableWin/Ego_r ?Main/?Init/?OnPredicateFail@CHTE_FreedomFighter@CQ_HangingTreeEvilScript@NScript@@EAEXXZ and
    ?GetParentScript@...@@UBEPAVCScriptBase@@XZ.
  - The landed Init/OnPredicateFail rows read HeroHitMe at +0x1c.
- Why the default path and not an opt-in branch: the header has no landed includer, and the change only completes a class that
  was declared with nothing but `Alloc`. Its one pending includer (chunk 744's parked 0x00D69480) gives the same built bytes with
  the overlay (SIBLINGS.tsv). `hdr/INCLUDERS_CQ_HangingTreeEvilScript.txt` lists the includers.
- Overlay confirmed in use: the first compile of 0x00D6A560 reported errors at
  `...\agent781\hdr\engine\CQ_HangingTreeEvilScript.h(90)` (an include-order slip that was then fixed in the row). The row needs
  the members, so it cannot compile against the repo copy.
- Used by: differ/00d6a560.cpp, the parked row. No matched row needs this header, so landing it is optional until 0x00D6A560
  lands.
