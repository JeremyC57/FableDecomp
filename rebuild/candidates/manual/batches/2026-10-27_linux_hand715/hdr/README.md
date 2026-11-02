# chunk 715 header overlays

Both overlays are copies of the current repo file (rebased after chunks 713/714 landed; `diff` against
`rebuild/include/engine/<Class>.h` shows only the line-3 RETAIL_MARK and the added opt-in branch). The default
(`#else`) text is byte-for-byte the repo text, so no landed includer changes; no includer recheck was needed
(44 landed includers of CGameScriptThing.h, all on the default path).

Overlay use was proven with a deliberate byte change in each branch (never `#error`): CTexture with a doubled
`Uninitialise()` graded 0x00A3B970 DIFFER 96v89; CGameScriptThing with an extra member graded 0x008D4740 DIFFER
90v90 (bytes moved). Both restored: RELOCATION_MATCH.

## engine/CGameScriptThing.h -- opt-in `FABLE_RETAIL_CGAMESCRIPTTHING` (row 0x008D4740)
- What: the retail polymorphic class `CGameScriptThing : public CScriptThing` (engine/CScriptThing.h,
  FABLE_RETAIL_NSCRIPT) with the Ego_r.pdb members PThing +0x0C (CIntelligentPointer<CThing>), ScriptCounter
  +0x14, ThingUID +0x18, Name +0x20, DataString +0x24, Pos +0x28, HomePos +0x34, AngleXY +0x40, World +0x44,
  sizeof 0x48, and the two private PDB ctors.
- Evidence: RTTI .?AVCGameScriptThing@@ : CScriptThing : CBaseClass (vtable 0x012852CC, ??_G 0x008D4720);
  the implicit dtor 0x008D4740 walks exactly those offsets; FableWin.pdb ??1CGameScriptThing@@UAE@XZ,
  ??0CGameScriptThing@@AAE@AAVCWorld@@@Z, ??0CGameScriptThing@@AAE@AAVCWorld@@ABVCThing@@@Z.
- Default: unchanged flattened Ego_r layout (CScriptThing and the intelligent pointer flattened).
- Includer contract (opt-in only): define CCharString, C3DVector and CIntelligentPointer<T> before the include
  (no shared headers exist for them), and FABLE_RETAIL_NSCRIPT with CCountedPointer for CScriptThing.h.

## engine/CTexture.h -- opt-in `FABLE_RETAIL_CTEXTURE` (row 0x00A3B970)
- What: the retail 8-byte non-polymorphic class with its member functions: copy ctor 0x009F9D00,
  operator= 0x009FA1C0, Uninitialise 0x009F9F70, and the inline `~CTexture() { Uninitialise(); }`;
  PD3DTexture +0x00, ByteLength:28 + AllocationSource:4 sharing the +0x04 word.
- Evidence: ~CTextureRenderer 0x00A3B970 and CRenderManager2D::AddTri 0x009DB810 expand `delete Texture` as
  `call 0x009F9F70` then operator delete (?Uninitialise@CTexture@@QAEXXZ: Ego_r 0x00B69E70, FableWin
  0x030DE280; nine landed bindings use that spelling); Ego_r.pdb (release) has no ??1CTexture; the copy ctor
  masks the +4 word with 0FFFFFFFh (one bitfield word). Retail keeps two unreferenced `jmp 0x009F9F70`
  bodies (0x0041A840, 0x0049B1C0), not attributed here.
- Default: unchanged flattened layout.

## engine/CLevelScriptBase.h -- not overlaid
Row 0x00E37680 uses the repo's chunk 713 branch FABLE_RETAIL_CLEVELSCRIPTBASE (compiler-generated
~CLevelScriptBase). My earlier overlay with the same macro was deleted after the coordinator note.

## Repo branches used unchanged
FABLE_RETAIL_CTCBASE (with the 713/714 PeekWorld/PeekWorldMap), FABLE_RETAIL_CCLASS, FABLE_RETAIL_BASE_OBJECT,
FABLE_RETAIL_DEF_CLASS_BASE, FABLE_RETAIL_NSCRIPT, FABLE_RETAIL_CMODESTANDBASE.
