# Landing nf250 (Account A, 2026-11-22): twelve of the open CODEXP27 lines (12 rows); L86 held, five lines blocked

Agent NF250 (hand-back early at an entry boundary). Hand-back: `pending/2026-11-22_nf250_handback/` (blocked/ = the drafts of L72 / L74 /
L89 / L92 / L100; NOTES = the plan for the E3 0x01296F14 unit).
- **L71 0x008AA9B0:** made-up `CCountedPointer_CopyCtor` -> converting ctor `CCountedPointer<CModeSitBase>(const CCountedPointer<
  CModeSitFloor>&)` (FableWin PDB + the one call site), MATCH.
- **L83 0x008F69B0:** -> STLport `vector<CHeightMapCell>(size_type)`; new `// Flags: /Gr` line (its TU, CHeightMap, is /Gr).
- **L104 0x0096E4A0:** made-up `Sort_SElement4` -> STLport `std::sort<CThing**, CAdaptThingCompareToSTLComparitor<CThingCompare_Nearest>>`;
  its callees 0x0096E410 / 0x0096D980 / 0x0096D780 2 -> 1 each (one table line deleted, two replaced).
- **L115 0x00AB15F0:** -> the nested const `C3DAnimationBuilder::CBoneInfo::FindChildIndex` (both PDBs).
- **L118 0x00B209C0:** catalog `CPSCSingleSprite::vfunc_31` -> `NParticleEngine::CPSCSplineBase::GetApproximateSplineLength`, QAE -> UAE (both
  PDBs, retail vtable slot 31), in place on the row's local CPSCSplineBase model.
- **L119:** binder 0x00B3E580 onto engine/CCountedPointer.h: stores `?DeleteData@?$CCountedPointer@VCMeshBankStateBlock@@@@SIXPAX@Z`, its copy
  EQ retail 0x00B3E1A0 (catalog-only, queued); table line replaced.
- **L29 / L32 / L50 / L95 / L96** (0x004A9710, 0x004D6915, 0x006F8010, 0x009539A0, 0x00953A00): each the real `??_G` of its class on the
  shared header, emitted by an anonymous-namespace harness; L50 / L95 / L96 use the #define / #include block of the landed vtable emitters
  0x00955620 / 0x00951FB0 (slot for slot equal).
- **E2 0x0049D800:** catalog `CVertexBufferWin32::DoSizeof` -> `?GetLocalSeed@CWorld@@QBEPAKXZ` (MATCH) with two add-only opt-ins,
  `FABLE_RETAIL_CWORLD_GETLOCALSEED` / `FABLE_RETAIL_CMAINGAMECOMPONENT_GETLOCALSEED` (equiv_check over 57,125 rows: 0 differ); binder
  0x008EE720 still needs a full port (queued).
- Files / Modules respelled for the seven catalog renames. Link of the 13 objects: 0 LNK4006 / LNK2005 (except decision 3).
- **Recheck** (`logs/recheck.log`, landed tree): the 12 changed rows + the sort callees, the binders 0x00AB1740 / 0x008EE720 and the vtable
  emitters 0x00955620 / 0x00951FB0 (19 rows): 2 MATCH + 17 RELOCATION_MATCH, all PASS; 0 RELOC_CONFLICT. 0x008F69B0 first graded DIFFER: its
  `// Flags: /Gr` comment does not reach the payload, so the coordinator added `CompilerFlags = '/O2 /Oy /W3 /Gr'` to its build_candidates
  entry (the form 949 rows use); regraded RELOCATION_MATCH.

## Coordinator rulings on NF250's questions
1. Catalog / file / module renames: applied for 0x008AA9B0, 0x008F69B0, 0x00AB15F0, 0x0049D800, 0x00B3E580, 0x00B209C0, 0x0096E4A0 (the five
   `??_G` rows already carried their catalog names; 0x0091C080 is held).
2. **L86 0x0091C080 held** (the agent's default): its object would define the real `??_G?$CIntelligentPointer@VCThingCreatureBase@@@@`
   at 30 bytes against the owner 0x004D6915 (28 bytes, size-optimised). Under a stand-in name at HEAD that NEQ copy met nothing; under
   the real name it is a pick-any NEQ copy of an owned symbol, which the link can pick. Lands when the copy is EQ (queued).
3. L115's one LNK2005 with the binder 0x00AB1740 (a same-TU strong copy measured EQ to 0x00AB15F0): accepted (the equal-copy ruling).
4. L118's in-place virtual on the row's local CPSCSplineBase model: accepted (in-place ruling, as nf248 ruling 1); the port onto
   engine/CPSCSplineBase.h is queued as debt.
5. L83's `// Flags: /Gr`: approved (the TU's own flag; the row matches only with it).
6. The anonymous-namespace emission harness for the five `??_G` rows: accepted (compiler-emitted C++, no asm; the STANDIN_LEAF warning is
   vland comparing the `??_G` leaf with its catalog spelling, non-blocking).
Blocked (locks released): L72 / L74 / L92 / L100 (each object would emit its class vtable for the first time with base-class slot names
where retail has the class's overrides: declare the overrides in the class headers first), L89 (a 30-byte `??_GCCreatureAction_
PickUpTankard` v the landed owner's 43 bytes). Not started: L4 / L5, L28, L37, L57, L111, L114, L120, L129, E3 + L101-L103.
