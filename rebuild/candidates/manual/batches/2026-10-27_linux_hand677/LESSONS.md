# Chunk 677 -- lessons from the authoring agent (fold into GOTCHAS at landing)

- SDK `fSuccess = FALSE; break;` inside the block loop, then `return fSuccess;`, gives retail's shrink-wrapped
  `mov eax,1 ; je <shared TRUE tail>` before the callee-saved pushes. `return FALSE` or an early `return TRUE` does not
  (CImaAdpcmCodec::DecodeS16 0x00C5F7F0).
- STLport 4.6 `vector::operator[](n) { return *(begin() + n); }` gives the re-read `mov eax,[base] ; lea ecx,[idx+eax]`.
  `_M_start[n]` / `*(_M_start + n)` give `mov ecx,[base] ; add ecx,idx` (~CTCCombat 0x00735340).
- A switch case that falls into `default: result = -3; break;` gives the re-assigned `mov esi,-3` in a duplicated
  epilogue. `return -3` folds to `mov eax,-3` (CEaxContext dispatcher 0x00C683C0).
- STLport helpers in the 0x00B7Fxxx speed TU are `__forceinline` (as the landed 0x00B7FFC0 sibling). With plain `inline`,
  VC7.1 leaves `__uninitialized_copy` / `fill` and the others out of line (0x00B7FDE0).
- With only scalar `operator new` declared, `new VARIANT[n]` calls scalar new 0x00BFEA1A (CDispParams 0x00CAE9B0).
- strmbase ctlutil.cpp constructors with class members that have destructors need `/EHsc` (CDeferredCommand 0x00CAF230:
  461v501 without it). Use the oaidl `tagVARIANT` / `tagDISPPARAMS` struct tags so the symbols mangle as `PAUtagVARIANT@@`.
- Quest-script ctor with a map: STLport 4.6 `_Rb_tree(const _Compare&, const allocator_type&)` +
  `_Rb_tree_base` (`_M_header(alloc, 0)` then `_M_header._M_data = allocate(1)`) + `_M_empty_initialize` gives the
  retail `mov [hdr],0 ; call malloc ; mov [hdr],eax ; mov [cnt],0 ; color/parent/left/right` with every `_M_data` re-read.
  `CCharString[5]` members without /EHsc give inline `mov ebp,5 ; call ctor ; add edi,4 ; dec ebp ; jne` loops
  (CQ_AmbushTradersScript 0x00CF9350).
- Inline-budget residue: in a function with a local set, an inline dtor at three returns and two HasIndex calls, retail
  inlines the first set find and calls the second out of line (0x00AD2790). VC7.1 inlines both, and no layering, flag or
  definition order tried changed it (0x00AC8360, parked).

Review at landing:
- 0x00AC8360 needed 14 probes plus /Ob1 and is parked (inline budget). The best source is differ/00ac8360.cpp.
- 0x00C5F7F0 (13 probes) and 0x00735340 (7 probes) needed more than 4 grades. The levers are recorded above.
- 0x00735340 keeps local CTCBase / CIntelligentPointer / CThing models next to engine/CTCCombat.h (flat header: Header
  debt). CCountedPointer, CDefPointer and CCombatAbility now come from engine headers.
- Landed ~CTCChest 0x00750850 has a needless `#pragma pack(push,1)` around CBaseIntelligentPointer (8 bytes either way).
- Landed CImaAdpcmCodec::Initialize 0x00C5F9E0 / DecodeSample models declare DecodeS16 (and DecodeM16 / EncodeM16) with
  6 parameters (`...SIHPAE0IIPAH1@Z`). The SDK signature has 7; check them against the SDK.
- Landed Alloc 0x00CFA2F0 is the stand-in Create_CClass_00cf9350_00cfa2f0. It should be
  ?Alloc@CQ_AmbushTradersScript@NScript@@... (NAME_FIXES.tsv). 0x00CFAD30 "CHeroMorphDef_Create" is likely wrong too.
- Landed CQ_HangingTreeGoodScript ctor 0x00D6EF20 uses `(void*, CGameScriptInterface*)`. The PDB signature is
  `(CScriptDataBase*, CGameScriptInterfaceBase&)`.
- vland SHARED_HEADER_AVAILABLE `CBaseObject` on 0x00CAF230 is a false positive (strmbase class of the same name).
