# Chunk 2005 notes (agent2005)

## Result
| row | grade | bytes | where |
|---|---|---|---|
| 0x00B51570 CEngineWeatherRenderer::BuildBuffers | RELOCATION_MATCH, test PASS (g++ + VC7.1) | 1254v1254 | src/ + test/ |
| 0x00A4D920 C3DAnimation2::CalcLocalBoneTransformationRelative | DIFFER, 2 lines | 1341v1341 | differ/ |
| 0x00AB2520 C3DAnimationBuilder::BuildMeshAnimation | DIFFER, 66 lines (x87 order only) | 1265v1265 | differ/ |
| 0x00A12EE0 CPhysicsMeshObject::GetNavigationLevelVolumes | DIFFER, 134 lines (26 masked) | 1118v1144 | differ/ |

No SPLITS: all four extents are right (each ends in `ret`, then int3 padding before the next function).

## Names
Proven, from Ego_r.pdb and FableWin.pdb (`ghidra_out/egor_pdb_names.tsv` and `fablewin_pdb_names.tsv`): the four row
symbols, `C3DAnimationBuilder::BuildBoneInfoTree(const C3DMesh2&, CArray<CCountedPointer<CBoneInfo> >&)` (0x00AB1CC0),
`GFFloatRandom` (0x004978A0), `C3DAnimation2::ConvertTimeToFrames` (0x00A52650), `CPlane::Initialise(const C3DVector&, float)`
(0x00A42140), `CContainmentVolume::Initialise` (0x00A755C0), and
`CEngineResourceManager::GetVertexBufferMemPool` (PDB member; see the 0x00B3B090 note below). Member names in the new
headers come from Ego_r `struct_layouts_egor.tsv`: CEngineWeatherSettings, CTVertexWeather(Base), C3DBone2,
C3DAnimationBuilder::CBoneInfo, N3DAnimationBuilder::CSequenceInfo / COutputAnimation, C3DMeshVolume and CContainmentVolume.

Descriptive (invented; no PDB/string source):
- `SetIndex(unsigned short*, long, unsigned short)`, an inline file-local helper in 0x00B51570. It keeps the 24 mist
  index stores in retail's order by splitting c2's scheduling region.
- `INDEX_BUFFER_FORMAT_16 = 0x65`, the D3DFMT_INDEX16 value. The PDB has the enum type, but no enumerator name was checked.
- Parameter and local names (`identity_generated_bones`, `mesh_bones`, `anim_root`, `source_planes`, ...).

Unresolved:
- **0x01436E98.** Landed code spells it three ways: `NEngineInternal::GPResourceManager` (00badb40, 00b2d9f0, and
  this row), `g_pEngineData` (CEngineData*, 00bba480 and others) and the label `g_pResourceManager`. None is in a PDB
  dump. I followed the 00badb40 spelling, so the coordinator should pick one.
- **0x00B3B090.** This is a COMDAT-folded `return 1` body, landed as `_global::ReturnConst_1_00b3b090`. This row calls it
  as `GetVertexBufferMemPool` (PDB member, both PDBs). The header declares that member at that address.
- **0x00A110D0.** This is the `DeleteData` function stored in the new counted block at 0x00A131B4. Retail would spell it
  `?DeleteData@?$CCountedPointer@VCContainmentVolume@@@@SIXPAX@Z` (FableWin.pdb), but the body is COMDAT-folded across
  instances. It is landed as `_global::DeleteViaVtable0_00a110d0`, so it is not a NAME_FIX.
- **0x008274B0 / 0x008270E0 / 0x00825B40.** These are `std::__introsort_loop`, `__insertion_sort` and
  `__unguarded_insertion_sort_aux` for `pair<long, CCountedPointer<CContainmentVolume>>`
  (labels_leaf_sync_2026-10-31). They are called from the inlined `std::sort` in 0x00A12EE0.

NAME_FIXES.tsv has one row. 0x00A52650 is landed under the leaf `C3DAnimation2_Methods::ConvertTimeToFrames`; both
PDBs say `?ConvertTimeToFrames@C3DAnimation2@@QBEXMAAJAAM@Z`.

## Bonus bodies (for landing)
- **0x004978A0 GFFloatRandom.** The `FABLE_RETAIL_GFFLOATRANDOM_INLINE` body in hdr/engine/GFFloatRandom.h
  RELOCATION_MATCHes the retail COMDAT. This was checked with `tools/g2.py --va 4978a0`.
- **0x00AB1CC0 C3DAnimationBuilder::BuildBoneInfoTree(const C3DMesh2&, CArray&).** The same-TU `__declspec(noinline)`
  body in differ/00ab2520.cpp RELOCATION_MATCHes all 612 bytes. 0x00AB2520 needs it in the same TU, because a visible
  body changes c2's address-exposure facts. So the pair should land together, with this body first.

## Parked rows: exact remaining diff and the next idea
**0x00A4D920 (2 lines).**
- Residue: 0x00A4DAC6, retail `fld dword ptr [esp+18h] ; fmul dword ptr [esp+14h]`, ours swapped. This is
  `(1-t) * omega` inside the inlined SetSlerp → GFFastSin. Our `1-t` is a CSE temp shared with
  `oneMinusFraction` (high candidate id), so c2 loads it first. Retail's operand order says its `1-t` had the lower id.
- Pass 1 used about 10 candidate grades; pass 2 used about 400 lever and header-variant probes.
- Next idea: run a c2prio trace of the SetSlerp expansion. Probe it against the out-of-line twin 0x00A4C1F0, which
  reaches 7 lines with the same body, to find a spelling that gives `1-t` a fresh low-id tuple without changing the
  rest of the code.

**0x00AB2520 (66 lines, x87 only).**
- Residue: SetInverse E22 at 0x00AB2698, the E41 sum at 0x00AB26F9..0x00AB2705, and the inlined PostMultiply term order
  at 0x00AB2779..0x00AB28BE. All integer code matches.
- tmp/CMatrix3x4_all.h, with reordered E11, E21 and E41 terms in PostMultiply, brings this down to 34 lines. So
  retail's inline PostMultiply and SetInverse spellings in this TU differ from the repo header. I did not change the
  shared header, because other landed rows depend on it.
- Next idea: match the out-of-line CMatrix3x4::PostMultiply and SetInverse copies, if retail has them, and fix the
  header from those bodies.

**0x00A12EE0 (134 lines; 26 with registers and frame masked).**
1. Retail keeps a second outer induction pointer, `it+8` in `[esp+14h]` (0x00A12F08, 0x00A12F16, 0x00A132A6). This
   gives a 0x64 frame against our 0x60, which shifts every frame offset by 4.
2. The x87 order of the plane point at 0x00A12F73..0x00A12F9B.
3. The AccurateNormalise result copy and the TransformPoint interleave at 0x00A12FFF..0x00A13037.

An index loop gets the 0x64 frame but loses retail's `begin == end` entry test (127 lines). Next idea: find the source
shape that makes c2 keep `it+8` as an explicit ADD before strength reduction. One candidate is the CVectorMap
const_iterator class; another is an accessor returning `const CCountedPointer&`.

## Header debt
Everything is in hdr/ (see hdr/README.md); all changes are opt-in macros except the new headers.
- `stlport/fable_containers.h`: the sort-inline and `__destroy_aux` opt-ins are the largest change.
- CQuaternion `FABLE_RETAIL_CQUATERNION_MULASSIGN_INLINE` and C3DKeyframe `FABLE_RETAIL_C3DKEYFRAME_POSTAPPLY` are
  experimental opt-ins that no deliverable uses. They can be dropped at landing.
