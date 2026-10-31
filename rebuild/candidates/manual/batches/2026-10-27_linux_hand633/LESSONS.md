# Chunk 633 -- lessons from the authoring agent (fold into GOTCHAS at landing)

- Respell the callee to its PDB signature even when the old one compiles: 0x00A1AF00 matched only with
  `const CANavQuadTreeNode* CNavigatorManager::GetNodeAt(const C2DVector&, unsigned char) const`, reached through the PDB's private
  inline `CNavigatorAStar::GetNodeAt(const C3DVector&) const`. The landed non-const `(const C2DVector&, bool)` direct call scheduled
  the second `lea ecx,[esp+1Ch]` ahead of `push eax`.
- `it->second = T(rhs);` with a named STLport iterator evaluates rhs before reading the iterator back from its frame word.
  That gives retail's `call _ftol ; mov edx,[esp+14h] ; mov [edx+14h],eax` (0x008AF500). `it->second.m = rhs;` loads and offsets
  the node right after find().
- Real STLport `_Rb_tree_iterator` (`operator->` = `&operator*()` over `_M_node->_M_value_field`) and no `#pragma pack` on
  CBaseIntelligentPointer flip the esi/edi map of 0x008AF500 to retail's, but `this` then lands in ebp (240v235).
- `std::make_pair` for a pair<CCharString, CCharString> key costs 11 bytes against a direct pair constructor (0x00A3F250, 225v236).
- Several parked rows show a one-step eax/ecx/edx rotation across the whole body against retail: 0x0091EA70 (+1), 0x00A3F250 (-1)
  and 0x009FE460. In this session nothing changed it: const-ness, PDB respelling, named locals, inline accessors, make_pair. The
  QFE 4035 compiler (FABLE_VC71=$S/wine/vc71) emits the same bytes as RTM 3077 for 0x0091EA70, 0x00A3F250, 0x00752F30 and
  0x008DA7A0. Treat it as a separate residue class ("scratch rotation") when parking.

Review at landing:
- 0x00A1AF00 is the only new match. Land it with the NAME_FIXES.tsv respell of 0x00A14F70. The landed callers that spell it
  non-const with bool/long are 0x00A199F0, 0x00A19A30, 0x00A1AB40, 0x00A1BEE0 and 0x00A15140. Respell 0x006CDA00 in 0x00A1AB40 too.
- The landed CModeStrafe::OnActivate 0x008E9A30 declares `AnimateThingMethods::GetFirstCombinationAnimFromType` (wrong class) for 0x0070C4E0.
- The reloc binding for 0x0091EA70 spells IsAbleToFire QAE (public non-const); the PDB says ABE (private const).
- The prior 0x008AF500 source used `#pragma pack(push, 1)` (forbidden); the parked differ/ version has it removed.
- 0x00A00680 ReleaseData is a single-source (probable) name: name_review before any respell.
- Two rows used exactly 4 grades (0x004C10E0, 0x008AF500); none exceeded 4.
