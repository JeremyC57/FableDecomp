# Wave 2, section 3: parked-row retry sweep (0x007820E0..0x0088B0C0, 353 rows)

Every WORKLIST row has a STATUS.tsv line (addr, status, grades, levers tried, residue).

| status | rows | where |
|---|---|---|
| MATCH | 5 | src/ + test/ (payload.json) |
| RELOCATION_MATCH | 27 | src/ + test/ (payload.json) |
| MATCH_NAME_REVIEW | 3 | name_review/ (007d97e0, 007d9a00, 008068b0: byte-match, name not proven) |
| PENDING_HEADER | 2 | hdr/ (007cf7e0, 00802d10: need an ENullDef/default ctor in engine/CDefPointer.h) |
| IMPROVED | 99 | differ/ (new `// Grade:` line, levers tried, next step) |
| HELD | 217 | parked source unchanged; levers tried named in STATUS |

The 5 skeleton parks (empty or stub bodies) now each have a first full reconstruction from the disassembly, graded 3
times, in differ/: 0079f970 1136v1052/188 (was 348 lines), 00848020 1151v1139/106 (was 352), 0087cfe0 1468v1447/344
(was 462), 0082afa0 1409v1393/366 (was 446), 0087d590 1238v1215/334 (the stub graded 327 lines; ours has the full
control flow, but its frame is 8 bytes smaller so nearly every esp offset differs). Each file's header names the
residue and the next step. The 0079f970 g3 variant is kept as differ/0079f970.g3.cpp for reference.

Dry vland (`LANDVERIFY_WORK=$A/lv`, no --land): **WINS 32/32**, 2 MATCH + 30 RELOCATION_MATCH. Every test prints
PASS. No RELOC_IDENTITY. (vland counts the 5 STATUS-MATCH rows by their relocation-normalised grade.) No retail_asm
row matched, so there is no retail_asm_payload.json. The retail_asm helpers in 0081ea50, 0079d360 and 0078d250 are
improvements kept in differ/.

## Per-lever tally: matches (35 byte-matches = 32 payload + 3 name-review)
| lever | wins |
|---|---|
| BOUNDS_FIX (grade over the true extent, oracle_extra.tsv) | 7 (0083b3e0, 00806220, 007d97e0*, 007d9a00*, 0085ae20, 007a8be0; 00871a80 improved) |
| as-is regrade of a never-graded / pass8 park over the true extent | 5 (007b7b60, 00855880, 008558d0, 0083ecf0, 0083f420) |
| out-parameter `bool GetInterface(T**, long)` replaces HasTC+GetTC | 4 (0078ec80, 00794180, 0079cee0, 0079ddb0) |
| PDB inline accessors (GetParentThing, GetTCGraphicAppearance, GetPhysicsTC) | 4 (007a9b10, 00814c70, 007f01f0, 007ee8f0) |
| shared engine/CDefPointer.h / CCountedPointer.h | 6 (007f3d60, 008621a0, 008508b0, 0085ae20, 00830cb0, 007a9b10) |
| landed CrossProduct shape ((float)(double)) | 3 (007dccb0, 007de030, 007bebe0) |
| PDB value type: Dinkumware iter_swap over pair<C3DVector,long> | 4 (00806220, 008068b0*, 007d97e0*, 007d9a00*) |
| TU-sibling model (landed neighbour's STLport list/map model) | 3 (007b7c30, 0082e860, 0085ae20) |
| inline `operator CCharString()` CDefString conversion | 2 (0083d710, 0083b3e0) |
| ternary GetInterface store / same-TU LowerBound / HasTC bool | 3 (0087ebd0, 007b2d50, 008621a0) |
| FastInvSqrt + operator*= Normalise model | 1 (007be0f9) |
| vector ctor operators + inline reciprocal argument | 1 (008520f0) |
| goto shared tail (shrink-wrap) | 1 (00847c90) |
| byte-wise CRGBColour member stores + PDB identity | 1 (00830cb0) |
(* = name_review. A row can count under more than one lever.)

## Per-lever tally: improvements this pass (difflines or length)
- **First full draft from the disassembly (skeleton parks).** 5 rows: 0079f970, 00848020, 0087cfe0, 0082afa0, 0087d590.
  Callee names come from reloc_bindings and the catalog, layouts from Ego_r.pdb and the landed siblings.
- **Literal straight to a `const CCharString&` parameter.** 11 rows: 0078cd80 (switch arms exact), 0078ecd0,
  00846710, 0083c2d0, 00865f40 (2 bytes off), 0087ee70, 008548b0, 00859ef0, 007ce7a0 (exact length), 0079d360. In
  007c99f0 the explicit temp is the better form.
- **`__forceinline` memberwise C3DVector copy ctor.** 007f9960, 008329b0, 007bf510, 00867200, 008664f0,
  00872af0, 00875b10, 008651b0.
- **By-value C3DVector helper parameter.** 00875b10 (Normalise: 281 -> 208 lines).
- **Literal unit-axis products.** 0080d300 ring offset (334 -> 242); 008651b0 `up = ZAxis*h`.
- **Inline sin/cos inside the vector expression.** 00872af0.
- **User-declared empty inline destructor (changes the inline split).** 0088b0c0 (970/47 -> 971v971/31).
- **Inline def-pointer copy.** 0087ee70 (`return CDefPointer(PDef)` plus a raw-pointer AddRef ctor); 0083db60
  (verify-copy header).
- **Un-hoisted reference local.** 008873c0 (401 -> 228).
- **Named locals for evaluation order.** 0078ecd0 (durations/world in the theme loop; PDef-> timers); 00846710
  (sprite id; const CCharString& bind; inner text scope).
- **Split if/else-if arms with duplicated tails.** 0084d2f0.
- **Layout fix against the PDB.** 007a8700 (Range at +0x3C).
- **retail_asm helpers.** FloatToLong 0081ea50, CeilToLong 0079d360, ROUND 0078d250.
- **PDB inline SetToDefault order.** 008818a0.
- **Per-arm temporaries passed directly.** 007bde31.
- **Forceinline single-arg resize.** 0085d180.
- **Named block string.** 00871430.

## Names
NAME_FIXES.tsv has 18 rows (each with correct symbol, evidence and the wrong spellings seen). New this pass:
- **DrawGetThingDisplayInformation.** 00830cb0 and its landed twins 0082d1d0 / 006a9d10.
- **Other respellings.** CInitBaseClass::IsValid 0099a330, CThingPhysical::GetHealthPercentage 006a4e40, the const
  CPlayerManager::GetPlayer 00449910, CThing::PeekPlayerManager 004c79f0.

No descriptive (no-PDB) names were introduced in this section.

## Bounds
BOUNDS_FIX.tsv has 8 rows. Their oracle lines are in oracle_extra.tsv. finish.py now prefers oracle_extra over
oracle.tsv; before that change vland graded 007a8be0 against the 617-byte catalog row.
