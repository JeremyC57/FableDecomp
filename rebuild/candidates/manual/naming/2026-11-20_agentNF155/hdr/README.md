# NF155 header overlay (add-only opt-ins; default path token-identical apart from pin_asserts' FABLE_STATIC_ASSERT_AT pins)

Base copies: `../orig/engine/...` (byte-equal to the repo working tree when this session started). NF153 / NF154 overlays read first:
NF153 has none, NF154 has only engine/CTCPhysicsStandard.h, so no member here is declared twice.

## engine/_quarantine/CThing.h
- `FABLE_RETAIL_QUARANTINE_CTHING_GETPOS` (new, member-specific): a forward `class C3DVector;` above the class and, after the last
  member, the declaration `const C3DVector& GetPos() const; // 0x004C73D0` (Ego_r.pdb / FableWin.pdb ?GetPos@CThing@@QBEABVC3DVector@@XZ,
  the spelling engine/CThing.h already declares). Declaration only, no layout change. Used by row 0x00927C80 (q4046).
- pin_asserts.py: `engine/_quarantine/CThing.h pinned 26` (the 26 FABLE_STATIC_ASSERTs after the inserted lines became
  FABLE_STATIC_ASSERT_AT(<their original line>, ...): same typedef names, so no includer's line-number-fed names move).
- Landed includers: 117 rows in rebuild/src/compiled include it. None defines the new macro (grep of rebuild/ finds it nowhere), so the
  default path they compile is unchanged apart from the pinned asserts: the opt-ins change none of them.

## engine/CCreatureActionBase.h
- `FABLE_RETAIL_CCREATUREACTIONBASE_GETPLAYER` (new): `protected: CPlayer& GetPlayer(); // 0x00692930` (Ego_r.pdb 0x00519290 +
  FableWin.pdb 0x01DB6430 ?GetPlayer@CCreatureActionBase@@IAEAAVCPlayer@@XZ). Declaration only. Row 0x00692930 (q4745).
- `FABLE_RETAIL_CCREATUREACTIONBASE_PEEKTHINGSEARCHTOOLS` (new): `protected: const CThingSearchTools& PeekThingSearchTools() const;
  // 0x00692990` (Ego_r.pdb 0x00519300 + FableWin.pdb 0x01DB6530 ?PeekThingSearchTools@CCreatureActionBase@@IBEABVCThingSearchTools@@XZ).
  Declaration only. Row 0x00692990 (q5083).
- Both blocks sit right after the existing FABLE_RETAIL_CCREATUREACTIONBASE_PEEKWORLD block, inside the retail branch.
- pin_asserts.py: `engine/CCreatureActionBase.h pinned 0` (no assert after the insertion point).
- Landed includers: 324 rows in rebuild/src/compiled include it; none defines either new macro, so the opt-ins change none of them.
