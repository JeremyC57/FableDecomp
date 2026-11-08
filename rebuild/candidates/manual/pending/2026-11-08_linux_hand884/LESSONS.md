# Chunk 884 -- lessons from the authoring agent (fold into GOTCHAS at landing)

- A 40 KB+ registration function matches from a generated source: extract the retail statement list once (class, allocator VA,
  driver immediate, inline-or-called form, frame offsets: tools/regs.py), generate the C++ from it (tools/gen.py) and grade the whole
  function; partial (--n) probes are only for shape questions. 0x004EE23F matched at the second full grade.
- `mov esi,offset G ; mov ecx,esi` before each `call` on the same global object, and `or edi,-1 ; push edi` for the CCharString
  (literal, -1) length, mark a size-optimised TU: `#pragma optimize("s", on)` (0x004EE23F; the entry is unaligned, packed against the
  previous function's `ret`). Under "s" the small pair / CRegisteredTCInfo constructors and vector::push_back stay out of line at every
  site, as retail.
- `call <pair ctor> ; push eax ; mov ecx,esi ; call push_back` = `push_back(value_type(key, value))` built directly; an inlined
  `std::make_pair(key, value)` re-loads `lea eax,[tmp]` before the push (CVectorMap::Insert at 0x004EE2C6; FableWin.pdb has no
  make_pair instance for any CVectorMap pair). Opt-in FABLE_RETAIL_CVECTORMAP_INSERT_VALUE_TYPE.
- VC7.1's caller-size inline budget gives "expanded at the first N call sites, called after" splits by itself (0x004EE23F:
  StatGetClassTypeName 41, ~pair 41, CDefClassInit ctor 8): write every site the same way and the split falls out; no flag or pragma.
- VC7.1 shares no temporary slot in a very large function: one frame slot per temporary, in statement order (0x004EE23F frame
  0x2FB4 = 340 x 32 + 111 x 12). A correct statement list in retail order gives every slot offset; no local-variable tricks.
- One named `CCharString name = T::StatGetClassTypeName();` used both as the map key and inside the value gives retail's single name
  slot passed to both constructors; calling StatGetClassTypeName twice would make two temporaries and two string constructions.
- Retail-order evidence for unnamed static calls: when the PDB name set holds exactly N `static void __fastcall()` setup functions on
  candidate classes, the Ego_r address order of those N equals the retail address order of the N calls, and identical-body pairs in
  Ego_r stay identical in retail (0x007899C0 / 0x007FFFA0), the mapping is fixed by two independent sources.

Review at landing:
- The row needs the hdr/ overlay (FABLE_HDR_OVERLAY or folded into rebuild/include): engine/CTCBase.h (_CTCBASE_REGISTRY),
  engine/CVectorMap.h (_INSERT_VALUE_TYPE, _SORT), engine/CDefinitionManager.h (_ADDDEFCLASS), engine/CDefClassInit.h (new),
  engine/NHitLocations_CHitLocation.h (_STATREGISTERCLASS). All additive; row 0x005DD670 still RELOCATION_MATCH through it.
- CVectorMap::Insert has two bodies side by side: make_pair (0x005DD670, landed, needed only for its 17/26 push_back inline split) and
  value_type (this row; the PDB evidence's form). 0x005DD670 should be re-derived on the value_type body when someone has the grades.
- Local declarations next to existing engine headers: 136 of the 451 registered classes have rebuild/include/engine/<Class>.h, but
  those headers declare no Alloc / StatGetClassTypeName / StatRegisterClass and do not compile together in one TU. The row declares
  each class with its registration interface only (no layout). SHARED_HEADER_AVAILABLE warnings for the def classes are expected.
- Errors in landed sources (NAME_FIXES.tsv): 0x004EBACE (CVectorMap<CCharString, CTCBase::CRegisteredTCInfo, ..>::Sort) is landed as
  CGuiVarTransferBool::TransferToGui; the CRegisteredTCInfo ctor 0x004D2EF0 and CDefClassInit ctor 0x0042DAE0 stand-ins use __cdecl
  allocator types (PDB: __fastcall, nested CTCBase::CRegisteredTCInfo, CThing&); 0x004D2B17 / 0x004D5853 (StatGetClassTypeName of
  CTCRotate / CTCTargetingSpirit) are bound to ?Init_004d2b17@T@@QAEXXZ / ??0CTargetingResult_Retail@@QAE@XZ; the map 0x013BA89C is
  ?g_RegisteredTCs@@...; the 8 setup calls and 0x0073B130 carry descriptive catalog names.
- Catalog: 0x0073B130 is also catalogued `Init_RegisterGameComponents`; it is CTCCreatureModeManager::RegisterModes, a separate row.
- Unproven: CTCBase::RegisteredTCs access (private chosen); the 13 driver-type enumerator names (written as numbers 12, 19..30).
