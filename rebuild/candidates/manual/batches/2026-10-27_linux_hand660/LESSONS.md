# Chunk 660 -- lessons from the authoring agent (fold into GOTCHAS at landing)

- Landed-twin rows are identity work (third chunk in a row: 10/10 here, every label wrong). A pool of byte-identical
  implicit destructors is one per class: settle each from its sole caller (a vtable slot-0 ??_G), or for a 0-caller row from
  the TU neighbours (the class's ctor before it, its ??_G after it inlining the same sequence) plus the FableWin ??1 that
  Ego_r lacks (0x006DF920 SheatheTrophy, 0x009771A0 JackScream, 0x007D7950 CCardDef).
- A derived class's ??_G that `call`s a far-away dtor shared by several sibling ??_Gs means the derived dtor is implicit and
  the shared callee is the BASE dtor (0x0048C950 = ~CCreatureAction_SheatheItemToInventory, 0x00430300 = ~CParentDefClassBase);
  the same derived dtor emitted out of line in another TU inlines that base body -- identical bytes, different symbol.
- VC7.1 emits an unreferenced implicit ??1 COMDAT next to the class's other implicit members when a TU uses the class by value
  (0x007D7950 ~CCardDef, 0x007D79C0 operator=, 0x007D7A00 ctor: FableWin names exactly that trio).
- An odd member offset in a naturally aligned class (bool padding elsewhere) means the member's own class packs to 1: put only
  that class under #pragma pack(1) (CIntelligentPointer<CThingAICreature> GuardFollowingPlayer at CTCVillage+0xBD, 0x0090C780;
  C3DVector XAxis at CProjectileWeapon+0xA9).
- `.CRT$XCU`-referenced bodies in 0x0121xxxx that construct engine objects are compiler dynamic initializers, not library
  code: author the object definition, leaf `_$E1`. A `const` namespace-scope object in `namespace { }` gets an internal
  `_NAME` symbol (no ?A0x hash); a non-const one gets `?NAME@?A0x<local hash>@@...` (0x0121AB3F). PDB `anonymous
  namespace'::dynamic initializer for 'X'` lists give the TU's object names in retail initializer order.
- A static object with a dynamic constructor is kept without any reference (the initializer references it); no harness.

Review at landing:
- NAME_FIXES.tsv (16 rows): own-row respells for the twins 0x0048C950, 0x00430300, 0x005DAE40, 0x007D9EF0, 0x0087B640 (and
  their callers); caller respells 0x0072CD00 / 0x0072CE20 (QAE -> UAE dtors, pointer -> reference ctor parameters), 0x006DD980,
  0x008916A0; stand-in families on 0x00693EF0, 0x0099A410, 0x0099B510; the atexit-thunk rows 0x01228930 / 0x01228FD9
  (AutoTinyThunk stand-ins over GGame / NEWLINE).
- 0x013B8BA8 needs an owner decision: the established spelling ?g_NewLineWideString@@3VCWideString@@B is descriptive; the
  proven name is the TU-local `anonymous namespace'::NEWLINE` (CTCInventoryWeapons TU, donor hash ?A0x0a34196c). As a const it
  has internal linkage (`_NEWLINE`, never bindable across TUs); its const-ness is inferred from use only.
- The harnesses (TUHarness_*) in 0x007D7950, 0x0072D3E0, 0x0072D430, 0x006DF920, 0x009771A0 are not retail functions (same
  convention as chunks 657/658).
- engine/CParentDefClassBase.h's out-of-line ~CParentDefClassBase blocks the inline expansion retail does in derived implicit
  dtors (0x007D7950); see NOTES.md Header debt.
