# DOSSIER 0x00ACA830 -- QuadricOptimiserInternals::COptimisedPrimitive::Initialise (7256 bytes)

No earlier scout existed: $W/prior/ is empty, and /opt/fable_lane/w22/agent2097 has only the brief (CHUNK_BRIEF.md line 7). The scouting here is new.

## 1. Retail shape (xdis 00aca830 0x1c68 = $W/dis_all.txt)
- `push ebp; mov ebp,esp; and esp,-8; sub esp,0D4h; push ebx; push esi; push edi`. This is an aligned frame: there is a local double (the CQuadric). The `and esp,-8` comes from the 8-byte-aligned CQuadric local, not from /Oy-. Use VC7.1's normal /O2 aligned-frame form. Locals sit at esp+0Ch..esp+0DFh after the three pushes. The argument is read as [ebp+8].
- `ret 4`, __thiscall, ecx = this (kept in [esp+1Ch] and in edi for most of the body).
- No SEH prologue and no EH states, even though local sets, maps and CCountedPointers have destructors. The TU is built without /EHsc, like the siblings ComputeVertexCost and ComputeContraction ("no EH frame"). There is no /GS cookie.
- Speed-optimised code: loop alignment padding (`lea esp,[esp]`, `mov edi,edi`, `lea ecx,[ecx]`), size divisions by magic multiply (0xB21642C9 for /0xB8, 0x38E38E39 for /0x48, 0x92492493 for /0x1C). Flags: none (as the sibling ComputeContraction).
- Epilogue at 0x00ACC47F: `pop edi; pop esi; pop ebx; mov esp,ebp; pop ebp; ret 4`. There are no cold blocks after the epilogue.

## 2. Identity (two sources plus caller)
- Ego_r.pdb (ghidra_out/egor_pdb_names.tsv:37604, 0x00C64040) and FableWin.pdb (ghidra_out/fablewin_pdb_names.tsv:147779, 0x034C32F0; also naming_stragglers/FableWin.exe.signatures.tsv) both give `?Initialise@COptimisedPrimitive@QuadricOptimiserInternals@@QAEXABVCOptimisationParameters@2@@Z`.
- That decodes to `public: void QuadricOptimiserInternals::COptimisedPrimitive::Initialise(const QuadricOptimiserInternals::COptimisationParameters& parameters)`, which is non-const.
- BSim audit ghidra_out/bsim_port_audit_05final.tsv:5757 maps aca830 to the same leaf.
- Sole caller: COptimisedMesh::Initialise 0x00ACD500 (call at 0x00ACD54F; landed rebuild/src/compiled/00/ac/QuadricOptimiserInternalsCOptimisedMesh_Initialise_00acd500.cpp declares `void Initialise(const COptimisationParameters&)`).
- The manifest label `COptimisedPrimitive::Initialise` is correct. Add the namespace QuadricOptimiserInternals.
- The size differs between builds (Ego_r 6858, FableWin debug 5401) because of the build and container differences. The retail copy is 7256 bytes.

## 3. Callees (in retail call order). Unless stated otherwise, the landed name matches the PDB.
| retail | how called | PDB name / signature | landed row |
|---|---|---|---|
| 0x00514C10 x6 | `CIndexList::AddIndex` -> set<unsigned long>::insert -> `_Rb_tree<unsigned long,...>::insert_unique(const V&)` (hidden pair result, ret 8) | STLport insert_unique | 00/51/stdRbtree_insertunique_00514c10.cpp |
| 0x00AC9A60 | `FindAdjacentFaces()` (AAEXXZ, private) | Ego_r 0x00C631D0 | NOT LANDED (labels_chunk688: was misnamed QuadricOptimiser_PreprocessAdjacencyAndNormals) |
| 0x00AC8540 | `PaintSegments()` (AAEXXZ) | Ego_r 0x00C61140 | 00/ac/QuadricOptimiserInternalsCOptimisedPrimitive_PaintSegments_00ac8540.cpp |
| 0x00AC9D50 | `MeasureSegments()` (AAEXXZ) | Ego_r 0x00C61E40 | 00/ac/...MeasureSegments_00ac9d50.cpp |
| 0x00AC7CB0 x4 | `C3DVector GetFaceNormal(const COptimisedFace&) const` (ABE, hidden C3DVector*: push face, push &result) | Ego_r 0x00C60570 | NOT LANDED. ComputeVertexCost carries a noinline body (00/ac/..._ComputeVertexCost_00ac8c80.cpp lines ~95-107) |
| 0x00AC7050 x3 | `CQuadric::Initialise(const C3DVector& normal, const C3DVector& point, float weight)` (QAEXABVC3DVector@@0M@Z, ret 0Ch) | Ego_r 0x00C5F910 | 00/ac/QuadricOptimiserInternalsCQuadric_Initialise_00ac7050.cpp |
| 0x00AC7E10 | `FindFaceTextureTangents(COptimisedFace&)` | Ego_r 0x00C60740 | 00/ac/COptimisedPrimitive_FindFaceTextureTangents_00ac7e10.cpp (models its classes locally) |
| 0x00AC8C80 | `ComputeVertexCost(COptimisedVertex&)` | Ego_r 0x00C621E0 | 00/ac/..._ComputeVertexCost_00ac8c80.cpp |
| 0x00BFEA0E malloc x6 | STLport allocator (tree header nodes 0x20 / 0x14, vector overflow) | CRT | 00/bf/global_malloc_00bfea0e.cpp |
| 0x00AD6740 | `map<pair<long,long>,CCountedPointer<CVertexPairContraction> >::operator[](const pair<long,long>&)` | ??A?$map@U?$pair@JJ@std@@... | 00/ad/STLmap_operator_00ad6740.cpp |
| 0x00BFEA1A operator new x4 | `new CVertexPairContraction` (0x28), `new CCPPointerInfo` (0xC) | | 00/bf/global_operatornew_00bfea1a.cpp |
| [eax+4] / [esi+4] / [ebx+4] | CCPPointerInfo::DeleteFunc (CCountedPointer release); deleter stored = 0x00AD5ED0 (`CCountedPointer<CVertexPairContraction>::DeleteData`) | | (CCountedPointer.h) |
| 0x00BFE9BC operator delete x9 | CCPPointerInfo freed on release | | global_operatordelete_00bfe9bc.cpp |
| 0x00BFEA14 free x8 | STLport deallocate (vector storage, set nodes, tree headers) | | global_free_00bfea14.cpp |
| 0x00512DA0 x2 | set<long>::insert -> `_Rb_tree<long,long,_Identity<long>,less<long>,allocator<long> >::insert_unique` | | 00/51/stdRbtreelonglong..._insertunique_00512da0.cpp |
| 0x00AC92C0 | `ComputeContraction(CVertexPairContraction&)` | Ego_r 0x00C62890 | 00/ac/..._ComputeContraction_00ac92c0.cpp |
| 0x00AD4540 x2 | `_Rb_tree<float,pair<const float,CCountedPointer<CVertexPairContraction> >,...>::_M_insert(x,y,v,w)` (ret 14h) reached from an INLINE insert_equal | | 00/ad/STLRbtree_Minsert_00ad4540.cpp |
| 0x004A9CB0 x2 | `_Rb_tree<long,long,_Identity<long>,less<long>,allocator<long> >::_M_erase(_Rb_tree_node<long>*)` (the recursive call inside the inlined ~set<long>) | manifest: `_M_erase std::_Rb_tree<long,long,_Identity<long>,...>` | 00/4a/unattributed_FreeTreeCTreeOwner004a9cb0_004a9cb0.cpp. **NAME DISAGREES**: landed as `CTreeOwner_004a9cb0::FreeTree_CTreeOwner_004a9cb0(SNode*)`; the real spelling is STLport `_Rb_tree<long,...>::_M_erase`. Put it in NAME_FIXES.tsv. |
| 0x00AC7EB0 | `ComputeSelfContraction(CVertexPairContraction&)` | Ego_r 0x00C607F0 | NOT LANDED |
| 0x00AD64C0 x2 | `map<unsigned long,CContractionReference>::operator[](const unsigned long&)` (COptimisedVertex::PairContractions at vertex+0xA8) | ??A?$map@KVCContractionReference... | 00/ad/STLmap_operator_00ad64c0.cpp |
| 0x00AD3300 | `_Rb_tree<pair<long,long>,...CCountedPointer<CVertexPairContraction>...>::_M_erase` (local map dtor's clear) | | 00/ad/stdRbtree_Merase_00ad3300.cpp |

Constants: 0x0122ED70 = 0.0 (double), 0x0129BA3C = 0.0001f (DELTA_NOTIONAL_ZERO), 0x0122DED4 = 3.0f, 0x0122F59C = 0.5f, 0x0122DEDC = 0.0f, 0x013CE558 = GFastOneOverSqrtTable[128]. The 1.0f weight is the immediate `push 3F800000h`, and the 0.0f weight is `push 0`.

## 4. Members and layouts (retail STLport: 12-byte containers)
- **this, COptimisedPrimitive** (rebuild/include/engine/COptimisedPrimitive.h, sizeof 0x84). Members: UniqueVertices +0x04 (CArray<CUniqueVertex>, elem 0x1C), Vertices +0x10 (elem 0xB8), Faces +0x1C (elem 0x48), Edges +0x28 (elem 0x10), VertexPairContractions +0x34 (multimap<float,CCountedPointer<CVertexPairContraction>>), Parameters +0x5C (40 bytes, `rep movsd` ecx=0Ah).
  - The default path of the header has only flat byte members (Parameters_Scaling...). Under `FABLE_RETAIL_QUADRIC_COPTIMISEDPRIMITIVE` it has the real typed class (needs CArray.h, CCountedPointer.h, stlport/fable_map.h, C3DVector.h) with member `COptimisationParameters Parameters`, and it declares the class COptimisationParameters itself.
  - That branch declares ComputeSubContractionCost(const CSubContraction&...) under `_SUBCOST` without a forward declaration of CSubContraction; fix it in a hdr/ overlay if you use it. It also has `_JOINADJACENTFACES` and `_METHODS` (GetFaceNormal, FindTextureTangent, FindFaceTextureTangents, PerformContraction, DetectFoldOver, PopFrontContraction, UpdateCost, Optimise).
  - Missing declarations (add as overlay `#ifdef FABLE_RETAIL_*`): `Initialise(const COptimisationParameters&)` (public), FindAdjacentFaces, PaintSegments, MeasureSegments, ComputeVertexCost, ComputeContraction, ComputeSelfContraction, and the inline UniqueToActualIndex.
  - The siblings ComputeVertexCost and ComputeContraction use the flat default instead: `#define COptimisedPrimitive COptimisedPrimitiveFields` / include / `#undef`, then `class COptimisedPrimitive : public COptimisedPrimitiveFields` with reinterpret_casts. Initialise needs the typed `Parameters = parameters` copy (a 40-byte `rep movsd`) and typed containers, so the FABLE_RETAIL_QUADRIC_COPTIMISEDPRIMITIVE branch fits better.
- **COptimisationParameters** (Ego_r 40 bytes): Scaling +0, EdgeWeight +4, EdgeCost +8, GroupWeight +0xC, TextureWeight, TexturePenalty, UnsplitTextureWeight, UnsplitNormalWeight, HighPenalty, MaximumBonesPerVertex +0x24. The function reads the argument (`[ebp+8]`) for Scaling, EdgeWeight and EdgeCost, NOT this->Parameters.
- **COptimisedVertex** (engine/QuadricOptimiserTypes.h; retail 0xB8): Position +0, Normal +0xC, TextureCoord +0x18, PositionError (CQuadric) +0x28, Cost +0x80, FaceList (CIndexList) +0x84, Exists +0x90, UniqueIndex +0x94, OutputIndex +0x98, Groups +0x9C (map<ulong,float>), PairContractions +0xA8 (map<ulong,CContractionReference>). In the shared header FaceList and PairContractions are raw `unsigned char[12]`, so a typed overlay is needed.
- **CQuadric** (QuadricOptimiserTypes.h, 0x58): AA..DD and Weight are doubles, with Weight at +0x50. That header declares only operator+= and EvaluateError. operator*=(double) and Initialise must be added. Retail inlines operator+= (0x00AC6E80, landed body `AA += other.AA;` ...) and operator*= (0x00AC6EF0, landed `AA *= scale;` ...) here.
- **COptimisedFace** (QuadricOptimiserTypes.h under `FABLE_RETAIL_QUADRIC_COPTIMISEDFACE`, 0x48): VertexIndices[3] +0, UniqueVertexIndices[3] +0xC, Edges[3] +0x18 (long; the adjacent FACE index, or -1 on a border), Exists +0x3C, Segment +0x44.
- **COptimisedEdge** (QuadricOptimiserTypes.h under `FABLE_RETAIL_QUADRIC_COPTIMISEDEDGE`, 0x10): FaceA +0, FaceB +4, UniqueVertex1 +8, UniqueVertex2 +0xC.
- **CUniqueVertex** (retail 0x1C): Exists +0, VertexIndices (CIndexList) +4, FaceList (CIndexList) +0x10. NO shared header. It is modelled locally in ComputeContraction 0x00AC92C0.
- **CIndexList** (12 bytes: `std::set<unsigned long> Indices`). No shared header; it is local in ComputeContraction and ComputeVertexCost. FableWin inline methods: AddIndex(K), GetSize, Begin/End, Empty, HasIndex.
- **CVertexPairContraction** (retail 0x28; Ego_r 44): Exists +0, Unsplit +1, SubContractions (CArray<CSubContraction>) +4, Weight2 +0x10, Cost +0x14, ResultingPosition +0x18, ListIterator (multimap iterator) +0x24. No shared header; it is local in UpdateCost and ComputeContraction. The ctor `??0CVertexPairContraction@...@QAE@XZ` (FableWin 0x034C4DF0) is inline here: Exists=true, Unsplit=false, vector pointers 0, ListIterator node 0.
- **CSubContraction** (8): QuadricOptimiserTypes.h has `Index1, Index2` (the PDB names). Use these, not the `Vertex1/Vertex2` spelling of the ComputeContraction row. FableWin has ??8 / ??9 operators, so the "both sub-contractions equal" test may be `!(sub2 == sub1)` or `sub2 != sub1`.
- **CContractionReference** (12): Contraction (CCountedPointer) +0, SubIndex +8. The ctor `(const CCountedPointer<CVertexPairContraction>&, unsigned long)`, the copy ctor, operator= and the dtor all exist in FableWin, and all are inlined here. It is modelled locally in 00/ad/STLmap_operator_00ad64c0.cpp.
- **CCountedPointer<T>** (engine/CCountedPointer.h): Data +0, Info +4. CCPPointerInfo is {RefCount, DeleteFunc, Data}, 12 bytes.
- The classes that local rows model and that have a shared header: COptimisedFace and COptimisedEdge (QuadricOptimiserTypes.h), COptimisedVertex and CQuadric (QuadricOptimiserTypes.h), COptimisedPrimitive (COptimisedPrimitive.h), C3DVector, CCountedPointer. These have no shared header: CIndexList, CUniqueVertex, CVertexPairContraction, CContractionReference. Put them in a hdr/ overlay (QuadricOptimiserTypes.h `#ifdef FABLE_RETAIL_...`).

## 5. Visibility and inlining
- **Inlined here:** CQuadric::operator+= (x8 sites) and operator*= (x3) (both out of line elsewhere, earlier in the TU). CIndexList::AddIndex. UniqueToActualIndex (FableWin 0x034C7AC0, `ABEJKABVCOptimisedFace@2@@Z`, a loop over 3 returning VertexIndices[k] or -1; inlined 4x). CVertexPairContraction ctor. CCountedPointer ctor(T*) / copy / operator= / dtor. CContractionReference ctor / operator= / dtor. vector<CSubContraction>::push_back including the whole _M_insert_overflow (malloc, element-copy loop with placement-new null tests, free). **multimap::insert -> _Rb_tree::insert_equal**: inlined twice (0x00ACBD94, 0x00ACC109), only _M_insert called. ~set<long> (clear with _M_erase expanded one level: `while(x){_M_erase(x->right); y=x->left; free(x); x=y;}`, then header reset, then free(header)). ~map<pair<long,long>,...> (clear calls _M_erase 0xAD3300, then free(header)). Tree iterator ++ (_M_increment inline, 5 sites). C3DVector operator-, ^, +, Dot, GetMagnitude (fsqrt), IsNonZero/Normalise with the GFFastOneOverSqrt table.
- **Called out of line:** set insert_unique (0x514C10, 0x512DA0), both map::operator[], _M_insert, the map _M_erase, and every COptimisedPrimitive member listed in section 3.
- **The insert_equal trap:** the same-TU UpdateCost 0x00AC8A60 CALLS insert_equal 0x00AD54B0, but Initialise inlines it. In the row, give insert_equal an inline (FABLE_FORCEINLINE if needed) STLport body `y=header; x=root; while(x){y=x; x = key_compare(v.first, key(x)) ? left : right;} return _M_insert(x,y,v,0);`. fable_map.h line 94 only declares it (no body), so the body must be supplied by an overlay.
- **Register clobbers (lesson c857 / ComputeVertexCost):** give the same-TU callees real `__declspec(noinline)` bodies where cheap, at least GetFaceNormal (copy ComputeVertexCost's) and CQuadric::Initialise (landed body). Without a body, VC7.1 assumes a call clobbers everything.
- **Opt-ins:**
  - Use `FABLE_RETAIL_C3DVECTOR` + `FABLE_RETAIL_C3DVECTOR_NORMALISE_UNPAREN`, exactly as ComputeVertexCost. Retail 0x00ACABD2: IsNonZero loads X, Z, Y and sums `fld st0; fadd st2; fadd st3`. Normalise then does `fadd st(1); fadd st(2)`, the same pattern as 0x00AC8DCA. That opt-in also gives `GetSquaredMagnitude` X*X+Z*Z+Y*Y, which matches the fsqrt edge lengths at 0x00ACAAE1 (X, Z, Y).
  - The border-edge pass (0x00ACB4B2) loads Z, Y, X for the cross product's IsNonZero. The value comes straight from `normal ^ edge` with Z still on the stack (`fst [esp+28h]; fmul [esp+28h]`), so it may still be the same IsNonZero. Test it.
  - stlport: `FABLE_RETAIL_STLP_SET` (set used by CIndexList), `FABLE_RETAIL_STLP_MAP` (map::operator[] declared out of line: check the fable_map.h comment at line 437 and keep operator[] declaration-only), `FABLE_RETAIL_STLP_VECTOR` for push_back. Do NOT set VECTOR_PUSHBACK_OUTOFLINE: retail expands overflow inline. Do NOT set TREE_CTOR_DTOR_OUTOFLINE or TREE_CLEAR_OUTOFLINE: the ctor/dtor/clear are inline here.

## 6. Sibling landed rows (same TU, 0x00AC6CE0..0x00ACDB00)
- 00/ac/QuadricOptimiserInternalsCOptimisedPrimitive_ComputeVertexCost_00ac8c80.cpp. `// Flags:` none. Defines FABLE_RETAIL_STLP_SET, _STLP_VECTOR, _STLP_VECTOR_RESERVE, _STLP_TREE_POSTINC, FABLE_RETAIL_C3DVECTOR, FABLE_RETAIL_C3DVECTOR_NORMALISE_UNPAREN. Uses the flat-header alias trick and noinline GetFaceNormal / MakeOrthogonal bodies. Copy its idioms.
- 00/ac/QuadricOptimiserInternalsCOptimisedPrimitive_ComputeContraction_00ac92c0.cpp. `// Flags: none`. Uses FABLE_RETAIL_STLP_SET_COPY. Local CIndexList, CUniqueVertex, CVertexPairContraction, CQuadric (noinline Clear / += / EvaluateError), and the `!(a > b)` max idiom note.
- 00/ac/QuadricOptimiserInternalsCOptimisedPrimitive_UpdateCost_00ac8a60.cpp: multimap insert spelling `insert(std::pair<const float,CCountedPointer<CVertexPairContraction> >(contraction->Cost, contraction))`.
- 00/ac/QuadricOptimiserInternalsCOptimisedPrimitive_MeasureSegments_00ac9d50.cpp and _PaintSegments_00ac8540.cpp: local STLport models (`#pragma auto_inline(off)` around out-of-line helpers).
- 00/ad/STLmap_operator_00ad64c0.cpp: the local CContractionReference model. 00/ad/STLRbtree_insertequal_00ad54b0.cpp: the insert_equal body.
- Family hdr overlays (from HINTS): /opt/fable_lane/w22/agent2059 and agent2060 for helper sizing with fwsize. FableWin debug sizes are in fablewin_pdb_names.tsv / FableWin.exe.signatures.tsv.

## 7. Source-order hint
In FableWin.pdb the methods appear in this address order: GetVertex, then Initialise (0x034C32F0), RemapGroups, CopyTo, Optimise, UpdateCost, ..., FindAdjacentFaces, PaintSegments, MeasureSegments, ..., GetFaceNormal, ComputeVertexCost, FindFaceTextureTangents, ComputeContraction, ComputeSelfContraction. Retail places Initialise after all of those (0x00ACA830). Do not rely on definition order. Retail never inlines any of these members, so noinline bodies or declarations are both faithful.
