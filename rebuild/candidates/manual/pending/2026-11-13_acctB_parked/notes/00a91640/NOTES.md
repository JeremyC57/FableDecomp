# 0x00A91640 C3DPrimitive2::InitialiseFromMeshFileData (bwf1, first authoring)
- best: differ/00a91640.cpp (src/n1.cpp) DIFFER(4324v4344) with `/d2inlT108`; started 5739v4344.
- moved it: (1) VC7.1 loop unroller was the 1400-byte excess (both vertex + tangent loops unrolled x4) -- it stopped once the
  inline decisions matched retail; (2) inline decisions via c2 inline trace (tools copied to $W/inl): resize(n,x)<C3DVertex2>
  sc=80, _M_insert_overflow sc=101/104, then resize(n) sc=107 -> /d2inlT108 (T107 fails, T105 leaves resize(n) called);
  (3) explicit specialisation DECLARATION of std::__copy<CIVCountedPointer<CTriangleList>*> (0x00A964C0) keeps that copy out of
  line while erase<C3DVertex2> stays a call (0x00A96A90) -- the global FABLE_RETAIL_STLP_COPY_DECL inlined erase<C3DVertex2>;
  (4) explicit specialisation declaration of vector<C3DVector>::resize(n,x) (0x0075C040);
  (5) size guard as `if (IsValidSize(..)) { if (!Alloc) return false; ...rest...; return true; } return false;` gives retail's cold
  size-failure block 0xA926C4 + inline alloc-failure copy + cross-jumped fail tails (two flat ifs / `||` / helper do not);
  (6) IsNonZero `Z*Z; +=X*X+Y*Y`, Normalise `Z*Z+X*X+Y*Y` (hdr C3DVector NV hook) gives retail's x87 sequence exactly;
  (7) named `bufferVertexSize = CalculateVertexSize(...)` before SetupVertexBuffer (args pushed after the call).
- did not move: /Os (1932 bytes), while-loop form, ISNONZERO_ZYX/SQUAREDMAGNITUDE variants, unnamed tangent temps (e2),
  explicit resize(count, C3DVector()) (e3), _M_insert_overflow `inline` keyword (no effect without the T flag).
- remaining residue = c2 frame packer only (all instructions/calls in order): frame 0x9c v 0xa0. Retail address order of slots:
  [verts+FindNext temps 8 @0x10] [tag 0x1B] [FindFirst/C3DVector temps 12 @0x1C] [conv/mteh @0x28] [tris @0x30] [len @0x38]
  [estimate/newGroup + xmif @0x40] [group @0x48] [cloth @0x50] [this @0x58] [hasVertexBuffer @0x5C] [xapr/float @0x64]
  [u/xbds @0x6C] [v 32 @0x78] [vbd @0x98] [handle @0xA0] [C3DVector() resize temp @0xA4] [xuvt @0xA8].
  Ours (pk trace $W/pk/n1.txt, sim $W/pk/sim.py): verts shares xapr's slot; FindNext temps own slot; resize temps share u's slot;
  cloth/xmif/group order differs. Best next idea: replay the packer (pk/sim.py --w / --conf) to find which weights/conflicts give
  retail's membership, then find the spelling (xapr's weight must drop below verts' 6 or conflict with it; the C3DVector() temps
  must not join u's slot).
- names: descriptive (found nowhere): GMeshDataMemoryPool (0x013D2D6C), GLoadMirroredTextureHighlights (0x013D2D74),
  hasVertexBuffer/bufferVertexSize locals, IsValidSize helper (sibling CContainmentVolume::Initialise spelling).
- NAME_FIXES proposed (dossier evidence, Ego_r order): 0x00AA7F40 is the CONST FindFirstSubChunkOfType (QBE); 0x00AA7FF0 the const
  FindNextSubChunkOfType; 0x00AA7EA0 returns unsigned long; 0x00A93A60 = CCountedPointer<C3DClothPrimitive>::DeleteData;
  0x00AC4F20 = CMirroredTextureHighlightsChunk::GetCount.
- packer experiments (offline, pk/sim.py on pk/n1.cpp.pk.json): `--merge 33=34..39` (verts + the FindNext result temps as ONE node)
  reproduces retail's membership for verts / tris / group / xapr (each its own slot, verts slot heaviest = lowest address);
  frame stays 156 -- retail's +4 is the C3DVector() resize temp (n55/n56) sharing xuvt's slot at 0xA4 (12-byte slot) instead of
  u's slot. Source spellings tried for this: f1 (for-loop over tris: 4417, worse), f2 (verts direct-init: no change).
- best next idea: find the spelling that makes the FindNext result temporaries one c2 node with verts (e.g. a single named
  CCountedPointer<const CChunkedFileChunk>-typed result variable reused, or FindNext* returning into an existing object), then
  the resize temp; prove each with pk.py before grading.
