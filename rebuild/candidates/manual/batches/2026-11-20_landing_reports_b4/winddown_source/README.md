# b4 (reports re-port, Account B, 2026-11-20): CEngineSubPrimitiveSprite -- 2 rows onto engine/CEngineSubPrimitiveSprite.h -- READY

Rows: 0x00BACE10 CEngineSubPrimitiveSprite::CEngineSubPrimitiveSprite, 0x00BB1740 CEngineSubPrimitiveSprite::`scalar deleting destructor'.

## Grades (vland1.log, STRICT, --recheck-landed, overlay hdr/)
0x00BACE10 RELOCATION_MATCH PASS and 0x00BB1740 RELOCATION_MATCH PASS, both as landed. No HEADER_POLICY or SHARED_HEADER warning is
left. The CBaseClassNonCopyable / CEngineSubPrimitiveBase / CEngineInternalPrimitive* local models went with the port. The header's
existing includer 0x00BADE40 regraded with the overlay: RELOCATION_MATCH PASS.

## 0x00BACE10 (src.diff)
The local CBaseClassNonCopyable / CEngineInternalPrimitiveBase / CEngineInternalPrimitiveSprite / CEngineSubPrimitiveBase /
CEngineSubPrimitiveSprite models become the shared header, with FABLE_RETAIL_PASS22_PRIMITIVE_IMPLICIT_DESTRUCTORS (the destructors
were implicit in the row's model and are implicit in retail: the class ??_G 0x00BB1740 calls ~CBaseClassNonCopyable directly) and the
new opt-in below. The out-of-line constructor definition is unchanged. No asserts in the row, so the line count is not held. Test unchanged.

## 0x00BB1740 (rewritten on the header: the landed text was a stand-in)
The landed row was a free stand-in: `struct CEngineSubPrimitiveSprite { void Destroy(); void* VDD(unsigned); }` with self leaf
?VDD@CEngineSubPrimitiveSprite@@QAEPAXI@Z, and 0x0099A300 / 0x00BEB550 bound as ?Destroy@ / free ?FreeMesh@ (RELOC_NAME warnings in the
baseline). It is now written as the landed sibling 0x00BCA060 CEngineSubPrimitiveSpriteTrail::`scalar deleting destructor' (NF86):
- Leaf ??_GCEngineSubPrimitiveSprite@@UAEPAXI@Z.
- The shared header with FABLE_RETAIL_PASS22_PRIMITIVE_IMPLICIT_DESTRUCTORS.
- An anonymous-namespace scaffolding use of the inline constructor, so the TU emits the vtable and ??_G (pick-any COMDATs). This is
  the q11120 / 0x00BCA060 pattern, marked `Fidelity: scaffolding, not retail`.
The relocations now bind the real names: ~CBaseClassNonCopyable 0x0099A300 and ??3CEngineSubPrimitiveBase@@SIXPAX@Z 0x00BEB550.
vland still prints STANDIN_LEAF, because the catalog name is spelled "`scalar_deleting_destructor'" (as for 0x00BCA060).
NEW TEST (test/00bb1740.cpp), pass pattern `00bb1740_TEST PASS` (pass/00bb1740.txt). The catalog's PassPattern 'VDD_OK' must change at
landing. The test builds the object in a static buffer, then:
- an explicit virtual destructor call, which goes through slot 0 with flags 0 (chain only, no delete);
- `delete`, flags 1 (chain, then the class operator delete with the object's address).
The base destructor and the operator delete are recording stubs. The test is self-contained, so it links the test TU's own copy of
the compiler-generated ??_G COMDAT (from the same header text), not the row object's.

## Header (hdr/engine/CEngineSubPrimitiveSprite.h; hdr.diff, add-only, 7 lines)
FABLE_RETAIL_CENGINESUBPRIMITIVESPRITE_CTOR_OUTOFLINE: the constructor as a declaration (FableWin.pdb
??0CEngineSubPrimitiveSprite@@QAE@PAVCEngineInternalPrimitiveSprite@@@Z, retail 0x00BACE10 out of line), the
CEngineSubPrimitiveBase CTOR_OUTOFLINE precedent. The default path is unchanged. The size assert is already pinned
(FABLE_STATIC_ASSERT_AT(13, ...)), so the added lines do not move its name.

## Mutants: not run. 0x00BACE10 has no statement change; 0x00BB1740 is compiler-generated (no hand-written statement).
## Names: the stand-ins above are retired by the port, so there is no NAME_FIXES entry.
