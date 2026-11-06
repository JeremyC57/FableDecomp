# chunk 785 notes

## Matched
- **0x00DD3B80 NScript::CQ_PrisonEscapeRescueMotherScript::PrisonLifeIntro** -- RELOCATION_MATCH, src/00dd3b80.cpp. Catalog label
  (CQ_GuildTrainingScript::RunCeremonyCutscene) wrong; identity from the CScriptThread spawn at 0x00DD34EF in Main 0x00DD2CE0 and
  the TEXT_QST_055_PRISON_INTRO_* literals. Model: landed WatchHeroEscapeStatus 0x00DD35D0 + scripted-thing handles
  (FABLE_RETAIL_CSCRIPTGAMERESOURCEOBJECTSCRIPTEDTHINGBASE_OUTOFLINE_DTOR); "_08" line is `"..._08" + GetHeroTitleAsLHTSTag()`
  (operator+(const char*, const CCharString&) 0x0099F690 __fastcall, GetHeroTitleAsLHTSTag 0x00CBEBA7 __fastcall).
- **0x00EAC5B0 NScript::CV_MurderTwistScript::CMurderTwistBrotherFriend::Main** -- RELOCATION_MATCH, src/00eac5b0.cpp, on the
  landed CMurderTwistBrotherOlder::Main 0x00EABB30 model (repo headers only).

## Parked
- **0x00BE4660 CWaterPatchTesselatorBase<CTVertexWaterBackground>::TesselateEdge** -- DIFFER(2380v2349), 171 attempts. Real name
  from vtable 0x012A3334 slot 0 (catalog `vfunc_0`). Same template body as the sea row. Shape: four private helpers
  (FindFirstWaterAdjacentVertex, IterateThroughAdjacentVerts, AddDuplicateEdgeVertex, AddFanBaseVertex) expanded inline
  (`__forceinline`; plain `inline` out-of-class leaves them out of line), CArray<T>::push_back(const T&) out of line
  (0x00BE4F90; explicit specialisation declaration keeps it so), CArray<unsigned short>::push_back expanded straight to
  vector<unsigned short>::_M_insert_overflow 0x005135D0. Residue: register colouring of the helpers' by-value parameter copies.
- **0x00BE4FE0 CWaterPatchTesselatorBase<CTVertexSeaBackground>::TesselateEdge** -- DIFFER(2404v2412), 173 attempts; same residue
  (retail keeps -1 in ebp at the loop head).
- **0x0070D940 CAnimateThing::CalcInterpolatedMeshAnim** -- DIFFER(2572v2395), 12 attempts. Callee set proven (LABELS.tsv);
  residue is VC7.1's inline budget: retail calls GetProgressFromAlpha 0x0070BE40 and push_back 0x00711C50 before the loop but
  expands both inside it, calls _M_insert_overflow 0x00711080 in the first combination pass and inlines it in the second.
  The transitions push_back uses an address cast as a stand-in (retail passes a never-addref'd slot holding the new object).
  Not /Gr (tested). Row-local models (no CAnimateThing header; engine C3DAnimation* headers are Ego_r-flattened).
- **0x00ACDB80 QuadricOptimiserInternals::COptimisedPrimitive::COptimisedPrimitive** -- DIFFER(2658v2674), 26 attempts,
  struct diff 79 lines. KEY: the TU is **/Gr** (`// Flags: /Gr`): without it the two SubChunksToKeep push_back overflows stay
  calls whatever the inlining hints (2299 bytes); with it they expand and __destroy_aux 0x00AA9140 / __copy 0x00AD6FF0 are the
  fastcall calls retail makes. No explicit specialisations needed (resize<COptimisedVertex> 0x00AD87A0 and
  erase<COptimisedFace> 0x00AD3D60 stay out of line by themselves). Spellings that mattered: per-loop counters (strided loops),
  the unique-vertex insert counter `unsigned int` (binds a temp to `const unsigned long&` like retail), the max taken from the
  list value, the triangle count read before the face temporary, the blend weight read before Groups[] (float temp), the
  tree's declared operator= (makes CUniqueVertex's erase call __copy), FindNextGroup taking CCountedPointer<const ...>
  (group declared const, no temp) while FindNextSubChunkOfType takes CCountedPointer<const CChunkedFileChunk> (cloth converts
  through a temp, as retail). Residue: esi/edi swap of `this` and &Faces, vertex-loop scheduling, a hoisted bone-key copy.
  Row-local models: engine/COptimisedPrimitive.h is the flattened Ego_r layout (opaque arrays, no namespace) -- header debt below.

## Names
Proven (2 sources): every row name above; callee labels in LABELS.tsv; the C3DMeshFile*Chunk getters (GetCount x3, GetBoneIndex)
replace propagated landed names (NAME_FIXES.tsv). Descriptive: the 0x0070D940 bool globals g_AnimationLOD 0x01380BDA and
g_DrawCombinationAnims 0x013756F7; CTVertexSeaBackground member names X/Y/Z; the COptimisedPrimitive model's tree helper class
names (CRbTree, CIndexList body, CGroupMap, CContractionMultimap stand for STLport set/map/multimap).

## Header debt
- engine/COptimisedPrimitive.h: replace the flattened global-namespace struct with `namespace QuadricOptimiserInternals` and real
  12-byte STLport containers (CArray<CUniqueVertex> +4, CArray<COptimisedVertex> +0x10, CArray<COptimisedFace> +0x1C,
  CArray<COptimisedEdge> +0x28, multimap<float, CCountedPointer<CVertexPairContraction> > +0x34, SegmentCount +0x40,
  CArray<float> +0x44, CArray<CCountedPointer<CChunkedFileChunk> > +0x50, COptimisationParameters +0x5C) as a retail opt-in;
  COptimisedVertex (0xB8: UniqueIndex +0x94, Groups +0x9C), CUniqueVertex (0x1C: VertexIndices +4, FaceList +0x10) and
  COptimisedFace (0x48, landed 0x00AD0100) need headers too. Too large for this chunk.
- engine/C3DMeshFilePrimitiveChunk.h is flattened (no CChunkedFileSuperChunk base, no Peek*/Find* members).
