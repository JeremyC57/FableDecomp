# Chunk 1019 (agent1019) -- 14 near-miss rows: 3 RELOCATION_MATCH, 11 parked DIFFER

Every grade, probe and c2prio run used `FABLE_HDR_OVERLAY=$A/hdr`. The overlay has the chunk-1017 opt-ins re-merged onto
HEAD (2026-11-10). Chunk 1019 added no opt-in of its own; `hdr/README.md` gives the table. There are 18 grades in
ATTEMPTS.log: one per row, plus a second grade each for 0x0063D0A0 and 0x0063D4A0. Free compile probes
(`tools/probe.py`, which does not grade) did the exploration.

## Matched (src/ + test/)
| Address | Identity | Lever |
|---|---|---|
| 0x008365F0 | `std::for_each<const pair<unsigned long,CAnimationEntry>*, CDeadCreatureAnimPreloader>` (/Gr, STLport, explicit instantiation of the committed inline FABLE_RETAIL_STLP_FOR_EACH_BODY) | Read the def name through a `CDefString` copy (`CDefString name = entry.second.AnimName;`). This shifts the rotating scratch register by one: the strcmp zero lands in eax and is reused for `BeginLoading(false)`, as retail does. |
| 0x00AE3920 | `CParticleProgram::RemapBoneIndices(map&, CArray<C3DGroup2>&)`, the private static `CI` __fastcall overload | Use the committed `FABLE_RETAIL_STLP_MAP`. With the map::operator[] body visible, c2 sinks the key store past the tree-root load, even though the call stays out of line at 0x00553E60. |
| 0x006A3E00 | `std::__introsort_loop<pair<long,CIntelligentPointer<CThing> >*, ..., CKeyPairCompareLess<...> >` (/Gr) | Use the committed `FABLE_RETAIL_CINTELLIGENTPOINTER_COPY_ORIGINAL_CHAIN` (with CTHING etc.) for the pivot copy. This evens the first and last priorities (256 each), so the tie key colours last first: first ebx, last edi. |

The identity evidence is in each src header and in LABELS.tsv. Catalog labels that were wrong:
- CTCInventoryAbilities_PreloadDeadAnimation for 0x008365F0;
- Heap_SiftDown for 0x006A3E00;
- Combat_CheckRingRangeToTarget for 0x006E94D0, which is really CCombatManager::IsCreatureInAttackRingSector (Ego_r donor order, see LABELS.tsv).

## Callee names (LABELS.tsv, NAME_FIXES.tsv)
- 0x00A242C0 is `CMeshDataBank::PeekAnimation(unsigned long) const`. Landed callers spell it GetAnimation / GetAnimationInfo.
- 0x00A26730 is `C3DAnimationInfo::BeginLoading(bool) const`. Landed code spells it `QAEXXZ`.
- 0x00553E60 is `map<unsigned long,unsigned long>::operator[](const unsigned long&)`. Three landed local-model spellings exist.
- 0x009D49F0 is `CDefStringTable::GetStringAsCharArray(long) const`, carried over from chunk 1017.
- Further labels: 0x00AC4070, 0x00AE83D0, 0x00AE80F0 and 0x00AE7FE0.

## Header debt
- **0x00681E60 CTCCreatureOpinionOfHero::SetPersonalityOverride.** Still on local models. It needs shared headers for COpinionPersonalityDef and COpinionOfHeroDef, and the SERIALISE members of CTCCreatureOpinionOfHero.
- **0x006E94D0 CCombatManager::IsCreatureInAttackRingSector.** Uses invented CRetailVector / CRetailRbMap stand-ins. engine/CCombatManager.h already has `Combatants` as `std::map<unsigned __int64,CCombatantInfo>`, but the combat-wheel methods and _M_find 0x006F0250 need declarations. This must migrate before any landing.
- **0x00A229E0 CRightHandedSet ctor.** Local CRightHandedSet / C3DAngles / CMatrix3x4 models. It needs a shared SetFromVectorAndAngle body.
- **The matched rows.** They need the overlay opt-ins in hdr/ to land: the chunk-1017 set plus the INTROSORT block in fable_containers.h.

## What each parked row needs next (measured residues: see each differ/ line 2)
- **0x009F18F0 GetData (166v166)**
  - Residue: a this / start / size colour permutation. c2prio --blocks gives start 61 vs this 75, and retail needs start ahead.
  - Tried: 20 probes, all inert.
  - Next: find a spelling that gives start more weighted uses in the loop, or makes the `(size - first)` temporary die into the memcpy count.
- **0x00934E50 FindMostRecentMessage (208v208)**
  - Residue: the fail-path join reloads this before the iterator. This is a split-piece numbering effect: `this` gets recycled piece #1.
  - Tried: 12 probes, inert.
  - Next: change which split frees piece #1. The const 0x1f mask split is the candidate.
- **0x00A55F90 SetFromVectorAndAngle (259v259)**
  - Residue: X is addressed off the axis candidate while Y and Z are CSE'd temps, which gives X-first loads; also the zero-store schedule.
  - Tried: 20 probes, inert.
  - Next: try a spelling that makes axis+0 a CSE temp too.
- **0x00681E60 (626v626)**
  - Residue: one SIB byte order (`[eax+ecx+10h]` vs `[ecx+eax+10h]`).
  - Tried: 7 probes, inert.
  - Next: do the header migration first, then retry the old-traits address spelling.
- **0x00A229E0 (691v689)**
  - Residue: frame 0x50 vs 0x4c, plus the x87 spill order.
  - Not reworked in this chunk. Needs header migration first.
- **Peek family 0x00B43720 / 0x00B43870 (320v324) and 0x00B439C0 (352v356)**
  - Residue: one compensation-block placement.
  - Measured: splitting GetMap into `if (x<0||y<0) return 0; if (x>=W*32||y>=H*32) return 0;` puts the block where retail has it (324v324 / 356v356).
  - Catch: it only stays inline with /d2inlT301 or an explicit loop, and then the y reload `mov ebp,[esp+1Ch]` is hoisted above the map-null `je` (2 lines).
  - Next: find a GetMap spelling that keeps the split, stays inline under the default inline budget, and keeps the y reload late. The shared engine/CEngineMap.h GetMap body would then change under an opt-in.
- **0x0063D0A0 / 0x0063D4A0 (1017v1017)**
  - Measured: parenthesising the first plane test, `(normal.Dot(dir)) > 0.0f`, takes the hunks from 13 to 10. One PAREN tuple moves the start/end/hit reloads.
  - Left: the Normalise prologue (`fld dir.X` before `push esi`) and the end/hit reload pair order (ebp/esi).
  - Tried and inert: parens at the other three tests, and all 11 SQUAREDMAGNITUDE_XYZ combinations.
- **0x006E94D0 (426v426)**
  - Residue: frame-slot sharing of the two find keys. It needs the find body visible, but the earlier wave found that this sinks the call.
  - Next: migrate the headers first.
