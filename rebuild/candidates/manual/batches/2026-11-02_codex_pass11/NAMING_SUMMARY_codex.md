# Pass11 naming and original-source recovery

The current merged `NAME_FIXES.tsv` contains **419 correction records**, with one distinct corrected symbol per address and no corrected symbol assigned to two different addresses. This is a correction list for proved wrong landed/bound spellings; it is not a claim that 419 new target functions were reconstructed. Per-row `NAMING_EVIDENCE.md`, original type/procedure records and relocation reviews retain the actual proof and its limits.

The 100 target identities are described in `ROW_DETAILS.md`. The final count remains 54 fully reviewed matches. Two byte-exact rows have an unresolved callee name, four rows have no authored source because required identity/layout evidence is missing, and several parked rows retain explicit helper-owner limitations. No manifest label was treated as sufficient proof.

## What counts as recovered

- **Original game/library PDB names:** raw PUBLIC records and full CodeView class/procedure records establish class key, namespace, access, constness, virtual/static status, calling convention and argument/return types. Native callers, vtable slots, field accesses, donor order or literals independently bind those names to retail addresses. Selected object symbol tables confirm the emitted spelling.
- **Original member and local names:** where the full-debug module preserves them, the declarations use those names and lexical scopes. Optimized Ego_r records often omit locals that survive in FableWin; the full block tree was checked where it changed the reconstruction. A recovered name does not by itself prove every original expression or annotation.
- **Original third-party source identities:** real STLport, zlib and DirectShow source provide genuine algorithms, member/API spellings and interfaces. Donor/native corroboration remains documented. GameCODA object/debug records provide stronger direct identity evidence for several audio functions.
- **Descriptive reconstruction names:** these are not presented as recovered originals. For example, the local algorithm variable spellings in the sphere-triangle distance row00a4b230 are reconstruction choices; its exact function signature, C3DVector type and native algorithm are proved. Names and access in stripped libpk support code sometimes have a weaker vendor/canonical-binding basis, called out below.
- **Compiler-generated names:** `_$E1`/`_$E2` denote compiler-generated initialization/cleanup symbols, not a handwritten game API. The actual exact spellings are in each Leaf and object review. The bow template's anonymous namespace hash differs by TU path; only that documented hash component is normalized.

## Representative target corrections

| Retail row | Correct identity | What independently establishes it |
|---|---|---|
|00a4fe60|`C3DAnimation2::InitialiseSharedAnimation(const C3DAnimFile&, EAnimationType)`|Original method/type and lib_3d_animation_2.cpp procedure records, plus native callers passing the file and enum. The CAnimatedMesh_LoadAndInitialize hint is wrong.|
|0078b7c0|`NMaths::GetRootsNumerically<anonymous CAnalyseRootsOfBowPullBackFunction>`|Original PUBLIC/procedure/type records and the native hidden vector result, functor and scan arguments. The BuildSpline_CubicRootFinding hint is wrong.|
|00774d20|`CTCShop::BuyItem(long)`|Both original procedure families/full BuyItem locals and native stock/profit/donation behavior. Earlier SellItem/SellHighlightedItem spellings were explicitly withdrawn; real SellItem is a separate function.|
|00c6c530 /00c6c0f0|The two `CSalIIRFilter::Process` overloads|Named GameCODA release object sections match the respective 1123/1075-byte spans, independent debug CodeView proves the class and overload types, and the native caller uses the corresponding arguments.|
|00ca5850|`CBaseVideoRenderer::ShouldDrawSampleNow`|DirectShow SDK declaration/source plus CTextureRenderer RTTI primary-vtable slot23 and the native timing/interface algorithm.|
|00c0d1b0|libpk `CString::FormatV(const char*, va_list)`|Original MFC implementation and WTL descendant, native CRT/literal/argument behavior, one-pointer CString layout and callers. No missing libpk PDB entry is claimed to exist. The CCharString manifest label is wrong.|
|008eec70|public static fastcall `Mass::Collide`|Both original PDB procedure records and native two-pointer, three-vector, four-float calling sequence.|
|00a8f1b0|private `C3DPrimitive2::ConvertToVBFormatStaticBump`|Both PDBs, neighboring primitive-conversion donor order, native Bump branch caller and complete original vertex/primitive layouts.|

These identities are separate from byte status: BuyItem, Mass::Collide and Bump remain parked despite their proved names.

## Recovered fields, locals and scopes

- **Audio filter:** original private integer fields `m_iA`, `m_iB`, `m_iZ` occupy offsets0/4/8 in the 12-byte `CSalIIRFilter`. Original `clip16` and unclipped state `iZ` come from debug/vendor records. The output clips to short while state remains integer.
- **Animation loader00a4fe60:** original parameters `file_data`, `anim_type`; locals `lock`, `capacity`, `anim_data`, `alloc_size_chunk`, `bone_mask_data`, `c_bone`, `vector_chunk`, `compressed_track`, `datastream`. The original lock member `Flag` is a bool reference. Native layout proof distinguishes the 0x7c retail `C3DAnimation2` from larger donor layouts caused by container differences; original fields such as `MovementTrack`, `MovementTrackData`, `Sequences`, `BoneMask`, `RigidBoneMask` and `InstanceList` retain their names.
- **Bump conversion00a8f1b0:** original `dest`, `no_vertices`, `c_vert`, `src_vertex`, `dest_vertex`, `u_tangent`, `v_tangent`, `crossprod` are recovered from module streams. Complete C3DVertex2, CTVertexNormTex1Bump and primitive owner types are retained. Native getters return vectors by value even where a descriptive reconstruction might have assumed references.
- **Mass collision008eec70:** original 36-byte polymorphic `Mass` has `Pos`, `Vel`, `IMass`, `APos`, `AVel`, `AIMass`; native construction confirms active offsets. The original free helper is `Right(C2DVector)`, not the invented neighboring `GetPerpendicular` member. Original `CopySign` is separately proved.
- **Spline00a58130:** full-debug scopes place `num_pts` with `smooth_iterations`/`smooth_counter`, while `new_points` and `p_first` belong to the smoothing loop. The genuine Rotate chain uses `CMatrix3x4 rotation`, `SetFromVectorAndAngle` and `TransformVector`. Those recoveries did not eliminate the final five-instruction scheduling difference.
- **Fracture00763060:** full-debug records recover the iterator/local set including `it_begin`, `it_anchor`, `it_start`, `it_result`, `result_point`, deletion iterators and `size_1`/`size_2`. These supersede incomplete optimized-local evidence; remaining frame/scheduling differences are still documented.
- **Bow0078b7c0:** original private functor member `DesiredPullBackAmount`, `GetValue(float x_axis)` and `IsValid()`; anonymous helper `GetBowPullBackAmountFromPullBackTimer(long pullback_timer)` with local `tanh_weight`; numerical-template locals include `r_start`, `r_end`, `c1`, `val_a`, `val_b`, `roots_pair`, iterator names and accuracy. Genuine `GFTanh` plus the actual VC7.1 float `fabs` overload context resolved the last code-generation gap.

Full names, types and scope evidence are kept in the relevant row rather than inferred from these short examples. Unaccessed opaque members, where documented in complete owner models, do not constitute recovery of their original implementation.

## Important callee spelling corrections

The exact mangled strings in `NAME_FIXES.tsv` are authoritative for these proved corrections; this table is a readable index.

| Native address | Correct declaration distinction |
|---|---|
|00487dc0|const `CPlayer::GetPControlledCreature()` returning `CThingPlayerCreature*`, not a generic hero/thing view getter.|
|004c73d0|const `CThing::GetPos()` returning `const C3DVector&`, not an invented GetPosition wrapper.|
|0099a2e0|public virtual `CBaseClass::~CBaseClass()`.|
|0099a300|protected virtual `CBaseClassNonCopyable::~CBaseClassNonCopyable()`; the protected access is part of the exact mangling.|
|009ad9e0|const `CDefinitionManager::GetPDefFromGlobalIndex(long)` returning the genuine `CDefPointer<const CDefClassBase>` class by value.|
|009ad6e0|protected const `CDefinitionManager::LoadDef(unsigned long)`, exact `?LoadDef@CDefinitionManager@@IBEXK@Z`. The final faction helper-context investigation proves this; selected faction source does not emit that extra supporting helper.|
|009ba080|`CStringParser::ReadNextItem(CParsedItem&)` returning bool, not a void call, pointer parameter or invented nested item type.|

CRT operators and data are also reviewed as actual addresses: free versus scalar/array deletion, exact literal payloads, class vtables and distinct byte-identical helper owners must not be conflated. Header and source spellings are checked against the emitted COFF symbols, not merely against the demangled name.

## Unresolved naming/layout limits to preserve

- **00431143:** the actual one-reference string-transfer callee used by 00520670 and 00521620 remains unnamed with adequate proof. Exhaustive donor PUBLIC/type/module searches do not establish the hypothesized one-argument `Transfer<CCharString>`. Both rows are byte-exact and tested, but remain `BYTE_MATCH_NAME_REVIEW` and not landing-eligible.
- **00c0f440:** receiver+4 forwarding and vtable slot11 do not identify the original libpk class/method/access/full signature. All supplied PDB name sets and available CODA libraries were searched. No fabricated target was authored.
- **005eb9e0 /00631c60:** genuine owner identities are known, but the native unaligned member layouts cannot yet be expressed through a proved allowed original mechanism. In particular CUIDef's real C2DVector PositionOffset is at +0x142. No new packing directive or substitute receiver is supplied as a solution.
- **006286f0:** required CASoundManager interface/layout and virtual-slot identity remain incomplete. No generic virtual placeholders were introduced to manufacture a row.
- **Parked subtitle/quadric work:** unresolved helper or destructor ownership remains explicit. In particular native 00ad4a00 must not be arbitrarily assigned to whichever CIndexList/set/tree layer happens to outline. Conversely, the original `RemovePairContraction(unsigned long other_vertex)` name, by-value key and local `found` were independently corroborated from full PDB records and native inline sites.
- **Stripped vendor CString:** `CString::FormatV` has strong original-source/native identity evidence, but supplied PDBs do not preserve its libpk support-member access. The row documents the canonical-binding/vendor basis instead of claiming nonexistent PDB proof.

Historical experiments retain superseded names as research evidence. Land only the current selected source and current settled corrections, and keep the explicit holds separate from byte comparisons. No remote commit or push was performed.
