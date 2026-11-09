# Chunk 895 notes (agent895)

Result: 6 of 31 rows match (5 RELOCATION_MATCH + 1 MATCH), each with a C++03 behaviour test that passes under g++ -std=c++03 and
VC7.1 /Od /W3 (tall.py). 25 rows stay parked; 13 have an updated differ/ source with the measured residue.
Grade 0x00BEBAA0 with `FABLE_HDR_OVERLAY=$A/hdr` (one new opt-in, see hdr/README.md).

## Matched
| Address | Identity | Lever |
|---|---|---|
| 0x006F3570 | CThingSearchTools::GetAllThingsInRoughArea<CThingObject, std::vector<const CThing*>, CThingFilter_None> | identity (mask 0x807; caller 0x006EB101 shares the vector with 0x006F37B0; FableWin order) + the landed 0x00436D60 shape |
| 0x006F37B0 | ...<CThingBuilding, std::vector<const CThing*>, CThingFilter_None> | same (mask 0x47; Ego_r + FableWin) |
| 0x00807620 | std::__push_heap<pair<C3DVector,long>*, int, pair<C3DVector,long>, CCompareTargetedPositionsForFirstPersonTargeting> | shared CVectorCompare_BestForTargeting header + _BODIES |
| 0x007FA160 | std::__push_heap<CThing**, int, CThing*, CThingCompare_BestForTargeting2> | same header + _BODIES |
| 0x00619560 | GFSerialiseVectorBinaryIn<CTCInventoryMap::CUIRegionInfo> | resize(count, traits.DefaultVal) on the shared STLport vector / CDataInputStream / CPersistTraits / CTCInventoryMap headers |
| 0x00BEBAA0 | CEngineInternalPrimitiveMeshBase::CalcObjectMatrix | shared C3DVector / CRightHandedSet / CMatrix3x4 headers + three PAREN tuples + SetTranslateOnly overlay |

Callee bindings checked against rebuild/oracles/reloc_bindings.tsv: ClipAndConvertWorldMapAreaToLocalArea 0x0049C700,
GetAllThingMapsInArea 0x0049C6F0, vector<const CThing*>::_M_insert_overflow 0x0049C370 (real STLport spelling, already bound),
the g_MapwhoLevels / g_NumMapwhoLevels / g_SearchStamps globals (as the landed siblings), GetCompareValueForPoint 0x007F9660,
GetPos 0x004C73D0, GetThingCompareMultiplier 0x007F9E70, CDataInputStream::ReadWithSrcChunkOverflow 0x00993CA0,
vector<CUIRegionInfo>::resize 0x006190C0. New (unbound) callee: CPersistTraits<CTCInventoryMap::CUIRegionInfo>::TransferBinaryIn
0x00614DA0 (FableWin spelling, LABELS.tsv).

Leaf note: 0x00807620's Leaf stops at `?A0x` because the anonymous-namespace hash is per TU (retail ?A0x58ed6966), as the landed
0x00807170; the full spelling is in the source comment and LABELS.tsv.

Header debt kept from the landed siblings: 0x006F3570 / 0x006F37B0 model CThingSearchTools / CThing / NThingMap / the STLport
vector locally exactly as the landed 0x00436D60 family (engine/CThingSearchTools.h carries the PDB's 16-byte vector); CThingFilter_None
is local because engine/CThingFilter_None.h pulls stlport/fable_containers.h, which clashes with that local vector.
0x007FA160 keeps the landed siblings' local CThingCompare_BestForTargeting2 (no shared header exists).

## Parked, with what to try next
- 0x00B801F0 vector<CTVertexSeaBackground>::_M_fill_insert (1 line): the hoisted x_copy loads order Z,X,Y v retail X,Y,Z. 11 probes;
  reference/direct loops give Z,Y,X. The shared FABLE_RETAIL_STLP_VECTOR body is 304 bytes (inline helpers stay out of line).
- 0x006D6CB0 / 0x006D68D0 (2 lines each): clamp store v `fmul [0.3]` slot. 14 probes (parens, constants first, splits, orders).
  Next: hand890 sim.py on the region DAG, or the shared CTCHeroExperience header with inline stat bodies.
- 0x0074E140 (2 lines): return copy Y load after `fstp [eax]`; a visible noinline AdjustTrajectoryVector body did not help (not
  capture). AdjustTrajectoryVector 0x0074E080 = normalise (GFFastOneOverSqrt), *0.3f, Z += (row + 1) * 0.2f (constants read).
- 0x00A56670 (4 lines): retail has one PAREN tuple fewer before the final copy (every added paren makes it worse).
- 0x006D8830 (4 lines), 0x0099BB80 (3 lines, header debt), 0x00A358F0 / 0x00A22CA0 (cross z operand; 0x00A22CA0 needs the full
  shared-header rebase that fixed 0x00BEBAA0), 0x00C3D030 (5 lines), 0x007BC6FA (header debt), 0x00868E50 (stand-in bodies).
- 0x00E5E350: the dead zero store is to the FIRST timer's slot (frame+4) before its RegisterTimer, and both timers escape through the
  out-of-line ~CTimer at the second return; a zero-initialising CTimer ctor would store into both, so look for a third object.
- 0x00C6B9D0 uses `__asm` in its draft (libpk fistp helper): a retail_asm row under docs/pipeline/ASM_RULES.md, not graded further.
- Not worked: 0x008EEBE0, 0x00BA48F0, 0x00A65720, 0x00D9EB30, 0x00826C90, the RemoveThingFromTask family (one re-probe of the nested
  erase(find_if(...)) form, already in hand720's list).

No NAME_FIXES (no landed spelling proven wrong); catalog corrections are in LABELS.tsv.
