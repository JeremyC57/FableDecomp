# Chunk 2006 header overlay (hdr/engine)

Every change is an addition behind an opt-in macro; with the macros undefined each header preprocesses exactly as the repo copy
(CV_BodyGuardScript.h and CQ_HobbeCaveScript.h are the pending hand-back 2026-11-11_linux_handH40g's versions, whose blocks are
also opt-in). Grade with `FABLE_HDR_OVERLAY=/opt/fable_lane/w22/agent2006/hdr`.

| header | opt-in | what | evidence | used by |
|---|---|---|---|---|
| engine/CEngineInternalAttachedPrimitive.h | `FABLE_RETAIL_CENGINEINTERNALATTACHEDPRIMITIVE_TRANSFORMS` | declarations of GetParentTransform 0x00B398E0, GetParentTransformAnimatedMesh 0x00B39080, GetChildInverseTransform 0x00B39900, GetChildInverseTransformStaticMesh 0x00B393D0 (reference parameter); GetOutputPrimitive / UpdateOutputPrimitive when the UPDATE opt-in is off. No layout change | Ego_r.pdb + FableWin.pdb ?<Name>@CEngineInternalAttachedPrimitive@@ spellings; retail TU order 0x00B38DB0..0x00B399A0 | 0x00B399A0 (match) |
| engine/CMatrix3x4.h | `FABLE_RETAIL_CMATRIX3X4_SETMULTIPLY` | `SetMultiply(const CMatrix3x4&, const CMatrix3x4&)` (this = m1 * m2) and `PreMultiply(const CMatrix3x4&)` written as `CMatrix3x4 copy(*this); SetMultiply(m, copy);`, both FABLE_FORCEINLINE | Ego_r.pdb / FableWin.pdb ?SetMultiply@CMatrix3x4@@QAEXABV1@0@Z, ?PreMultiply@CMatrix3x4@@QAEXABV1@@Z; retail 0x00B399F9..0x00B39EDB (three expansions, `rep movsd` copy); the only nesting that gives retail's 36 x87 sums | 0x00B399A0 (match). Mutually exclusive with the chunk 764 `FABLE_RETAIL_CMATRIX3X4_PREMULTIPLY` |
| engine/CRGBFloatColour.h | `FABLE_RETAIL_CRGBFLOATCOLOUR_INITIALISE_CTOR` | `CRGBFloatColour(const CRGBColour&)` written through `Initialise(r, g, b, a)` (channels converted A, B, G, R) | Ego_r.pdb ??0CRGBFloatColour@@QAE@ABVCRGBColour@@@Z and ?Initialise@CRGBFloatColour@@QAEXMMMM@Z; retail 0x00550EA8..0x00550F53 conversion order | 0x00550DC0 (parked) |
| engine/NUISystem_CTable.h | `FABLE_RETAIL_CTABLE_DRAW` | `virtual void Draw(CIEngine&, CEnginePrimitiveHandle*, long, long*, CComponent*)` override | Ego_r.pdb / FableWin.pdb ?Draw@CTable@NUISystem@@UAEXAAVCIEngine@@PAVCEnginePrimitiveHandle@@JPAJPAVCComponent@2@@Z; CTable vtable 0x0124A224 | 0x00550DC0 (parked) |
| engine/CV_BodyGuardScript.h | `FABLE_RETAIL_CV_BODYGUARDSCRIPT_CBODYGUARD_MAIN` (H40g, unchanged) | the nested CBodyGuard entity class (Ego_r 0x3c bytes) incl. `SetHenchmanAsHired(bool)` and `GetGoldCost()` | hand815 / H40g README; both PDBs | 0x00E621F0 (match) |
| engine/CQ_HobbeCaveScript.h | `FABLE_RETAIL_CQ_HOBBECAVESCRIPT_CHOBBESPAWN_MAIN` (new; the file also carries H40g's `..._CBANDITSACRIFICE_MAIN` block unchanged) | the nested CHobbeSpawn entity class: ParentClass +0x14, PMasterData +0x18, NumberToGenerate +0x1c, size assert 0x20 | struct_layouts_egor.tsv CHobbeSpawn (32 bytes, member names); both PDB name sets (Alloc SI, Main / Init EAE, GetParentScript UBE); landed Alloc 0x00D8F310 | 0x00D91E00 (match) |

Notes:
- The shared CRGBFloatColour.h declares a free `operator*`; both PDBs have the member ??DCRGBFloatColour@@QBE?AV0@ABV0@@Z. Both
  give the same bytes in 0x00550DC0 (not changed here: header debt).
- Re-base onto HEAD before merging (other agents may have touched these headers since the copies were taken); every block is a
  pure insertion.
