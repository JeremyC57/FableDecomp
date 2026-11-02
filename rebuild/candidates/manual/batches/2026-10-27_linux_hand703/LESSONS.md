# Chunk 703 -- lessons from the authoring agent (fold into GOTCHAS at landing)

- A derived class's first member starts at the base's ROUNDED sizeof: a model that pads a derived class from just past a base's
  trailing bool (`_pad19d[...]` in the derived class) lands every later member 4 high; put the pad in the base (0x005596D0).
- `Member = C2DVector(0.0f, 0.0f)` builds an 8-byte stack temporary under VC7.1 (sub esp,8; two stores; two reloads); retail's
  `xor eax,eax` shared by a flag test and two zero stores is `Member.X = 0.0f; Member.Y = 0.0f;` (0x005596D0).
- A CIntelligentPointer member at an odd offset (+0x1B / +0x1D) with the PDB class size still a multiple of 4 is a 1-aligned
  pointer TYPE in a normally aligned class: `#pragma pack(push, 1)` around CBaseIntelligentPointer / CIntelligentPointer<T> only
  (0x00900B41, 0x0090143E; as 0x008367B0). A size-packed TU's odd function starts are real when the previous body ends in `ret`.
- Anim components are 1-packed (PDB CAnimComponentCombatMisc 0xA, FlourishTargetOffset vector at +9): `Alloc() const { return new
  T; }` with the IV reference count set by the inlined CIVCountedPointeeBase ctor before the vtable store (0x005DF750).
- The inventory-screen OnKill family is split across size and speed TUs: CTCInventoryMap / CTCInventoryQuests (0x005FAEE0 /
  0x0061A1F0) compile branchy without `#pragma optimize("s", on)`, the Stats / Clothing / Weapons / Experience twins need it.

Review at landing:
- NAME_FIXES.tsv: 17 rows (the 9 row labels plus 0x004C8BA0, 0x008FD255, 0x00A01B10, 0x004BF9A0, 0x004C5690, 0x005BC9C8,
  0x00437CD0, 0x013B8790; 0x005DF750's name was already right). 0x008FD255 / 0x00A01B10 respells touch the whole generated
  CAIStateGroup ctor family (CBase_008fd255 / CMemberBase_00a01b10 stand-ins) and the landed 0x00900B81 caller.
- 0x004C7590 has six landed spellings and no PDB name: coordinator decision (this chunk used the row's own SetGraphicAppearanceChanged).
- 0x004C5690's landed model keys std::allocator as `struct`: respelling it to the class key changes that row's leaf symbol.
- Local base models next to existing (flattened / mismatched) engine headers: CTCBase, CTCActivationReceptorBase, CThing,
  CAnimComponentCombatMisc (see NOTES.md Header debt).
- vland (read-only): WINS 10, no RELOC_IDENTITY / DIFFER; warnings SHARED_HEADER_AVAILABLE (CManager / CPlayerGui) and RELOC_NAME 0x004C7590.
