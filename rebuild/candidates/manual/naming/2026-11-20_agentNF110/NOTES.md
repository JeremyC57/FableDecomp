# NF110 notes (round 10, 2026-11-20): the GetPFirst / GetPBest rows of GetAllThingMapsInArea (q11280 / q11283)

## Counts (from APPLIED.tsv / STATUS.tsv / recheck_final.log)
- Entries 2: blocked: port 2 (q11280, q11283 = the same claim; 4 rows left, all outside NF110's locks); done 0, done-already 0,
  rejected 0, deferred 0, open 0 (HELD.txt empty).
- Rows in STATUS.tsv 17: ported 16 (all of rows.txt) + 1 caller respell (0x007FEAF0, claimed); not-a-binder 0; blocked 0.
  rows/: 17 sources, no test changed (every test is a self-contained model and PASSes unchanged).
- recheck_final.log: one vland --recheck-landed run over the 17 changed rows, every line RELOCATION_MATCH + PASS.
  Control run work/ctl.log (repo sources, NF110 overlay): 0x009871C0, 0x0063F4B0 (Account B's), 0x00837E40, 0x008383D0 RELOCATION_MATCH + PASS.
- Header opt-ins 2 (hdr/README.md): CThingSearchTools.h FIRSTROUGH_EGOD, CThingTrackNode.h STATTYPEFLAGS.
- selfcheck.log: `selfcheck: 0 problem(s)`.
- recheck_final.log WARNs (all non-blocking): STANDIN_LEAF 0x006CE300 (names the old propagated catalog label; RENAMES s1 replaces it);
  RELOC_NAME on 0x0079A050 / 0x007F5D20 / 0x009147E0 / 0x00937690 x3 / 0x0093CDB0 / 0x00942390 (the callee catalog labels of 0x00782560,
  0x007F5730, 0x0093C9A0 say `CThingCompare_Nearest::operator()<T>` without the mangled form, and 0x006A73E0 / 0x006A7A40 carry wrong
  propagated labels, as for NF107's landed 0x00937420); SHARED_HEADER_AVAILABLE x8 on 0x007FEAF0 (its pre-existing local models, untouched).

## What changed
1. GetPFirstThingInRoughArea (8): the landed 0x009871C0 row defines the Ego_d-shaped body (EGOD: `flags` local, inline GetMapwhoArea,
   GetWidth / GetHeight test, inline PeekCell) in its own source. Graded in-row on 0x007FCEF0 it matched first time (work/q_007fcef0_egod.log),
   so it moved verbatim into engine/CThingSearchTools.h as the opt-in FIRSTROUGH_EGOD, and every instance is now an explicit instantiation on
   the 0x009871C0 define set + T's type-flags define + its filter header: 0x007FCEF0, 0x007512E0 (+ the inline OverlappingCircle body of
   0x0063FD40), 0x007FD110, 0x007313B0, 0x0079A280, 0x007FE730, 0x0096F260, 0x00987430: all first form (0x007512E0's first compile failed
   on a missing access define, CTCPHYSICSBASE_IMPULSE). The older fitted FIRSTROUGH_BODY was not needed.
2. GetPBestThingInRoughArea (6): NF107's BESTROUGH_BODY + BESTROUGH_THISEND + byte-offset cell set, plus T's type flags and
   FABLE_RETAIL_CTHINGCOMPARE_NEAREST_OUTOFLINE (retail calls the compare COMDAT): 0x0079A050, 0x007F5D20, 0x00942390 first form;
   0x00937690 on the NF107 0x00937420 set (same TU); 0x0093CDB0 + the new CThingTrackNode type-flags opt-in; 0x009147E0 on its TU-mate
   caller 0x00914650's set with that caller's inline ScaresBirds::operator() text (retail expands the filter) + the CThingCreatureBase
   type-flags define (5 forms; see STATUS and Rule slips). No pointer-taking filter binder was needed: every filter is by const reference in
   both PDBs, and the shared CFilter_Bind2 / Bind3 compile them.
3. 0x006CE300: <CThingCreatureBase, vector<const CThingCreatureBase*>, CThingFilter_IsNot> (FableWin.pdb 0x0247F080) on the NF107 0x008786E0
   recipe (K016 + TYPEDLOCAL) + STLP_VECTOR_PUSHBACK_NOINLINE (retail calls push_back 0x006CE530). The catalog label (<const CThingGameObject,
   vector<CIntelligentPointer<const CThing> >, CThingFilter_None>) was propagated; vland prints a non-blocking WARN STANDIN_LEAF against it
   until RENAMES s1 replaces it.
4. 0x0080ECC0: <CThingCreatureBase, list<CThingCreatureBase*>, CFilter_Bind5<IsNot, IsNotList, In2DRange, unary_negate<In2DRange>, anon
   IsAffectedByExplosions> > (Ego_r 0x00809E30 + FableWin 0x0276D800): the std::list ROUGHAREA_EGOD body of its TU-mates on NF107's 0x00809A70
   define / include set (the only CFilter_Bind5 is in engine/TargetingPlayerSupport.h, which needs CTCTARGETINGPLAYER_UPDATE_LIST and its chain).
5. 0x007FEAF0 (caller, claimed): its local model's GetPFirstThingInRoughArea template respelled in place to `const`, so it binds the QBE spelling
   0x007FE730 now has (closes that split).
- Self leafs: 15 change (RENAMES s1; 0x007FCEF0 was already right). 12 move QAE -> the Ego_r.pdb QBE spelling (two of them also to a const T
  or another T: 0x00987430 const CThingAICreature, 0x0093CDB0 CThingTrackNode for the local CThingMask10007; 0x0096F260 to
  <CThingFilter_IsRelatedTo, const CThingDeadCreature> off the propagated IsBlocking label); 0x006CE300 / 0x0080ECC0 move off
  CThingSearchTools_Calls stand-ins.
- Callee respellings (RENAMES s2, from work/b0_pairs.json -> work/fin_pairs.json, + 0x007FEAF0 from work/b5_pairs.json): 0x0049C6F0 / 0x0049C700
  (16 each), CThingCompare_Nearest instances 0x00782560 (x4), 0x007F5730, 0x0093C9A0 (off non-template stand-ins to their own rows'
  template spellings), push_back 0x006CE530 (off ThingVector to its row's STLport spelling), 0x00942270 and 0x006A8880 (to their own rows'
  spellings), 0x006A7F30 and 0x0080E480 (to the PDB spellings; their own rows are mislabelled: NAME_FIXES), 0x007FE730 (0x007FEAF0).

## Split check (no new split)
- Exe scan (refscan_cli.py) of every renamed self address and every respelled callee: every landed caller is listed below; the eight landed
  callers of 0x007FD110 (0x007FBC90, 0x008C2240, 0x00954F40, 0x0095AB50, 0x0095B020, 0x0095B3E0, 0x00972830, 0x00972F30) all declare the
  template const (local models) or use the shared header's FIRSTROUGH declaration, so they bind the QBE spelling; 0x00782560's other callers
  0x00782670 / 0x00937420 bind the template spelling. Uncatalogued call sites: 0x006CDF80, 0x0080C830, 0x0080EBBF, 0x00942368, 0x00986749.
- Self renames: every landed caller that binds these instances already binds the new spelling (0x00730F90, 0x00750E30, 0x00799900 x2,
  0x007F5AE0, 0x0095AB50, 0x007FF0C0, 0x00914650, 0x00941FC0, 0x00972990 / 0x007FBC90) or is respelled here (0x007FEAF0). Four callers keep a
  stand-in they already had (existing splits, unchanged in count: NAME_FIXES): 0x0096F180 (0x0096F260), 0x009863E0 (0x00987430), 0x00937C20
  (0x00937690), 0x0093CC60 (0x0093CDB0); each defines a same-TU stand-in body, so an in-place respell is not cheap. 0x006CE300 / 0x0080ECC0 have
  no catalogued caller (0x006CDF80 / 0x0080C830).
- Callees: 0x00782560, 0x007F5730, 0x0093C9A0, 0x006CE530, 0x00942270, 0x006A8880: the binder now equals the callee row's own leaf (split
  removed). 0x006A7F30 / 0x0080E480: the callee rows spell another (wrong) name, so the address keeps two spellings before and after (old binder
  stand-in -> PDB spelling); NAME_FIXES gives the correct names. 0x006A73E0 / 0x006A7A40 (IsUnconscious / IsAnAlly in 0x00937690): the same
  spellings NF107's landed 0x00937420 binds; their catalog labels (ReturnFalse / _Cons_val) are wrong but were already so (vland WARN RELOC_NAME).

- 0x009147E0 carries `#define FABLE_RETAIL_CTHING_TYPEFLAGS`, copied verbatim with the TU-mate 0x00914650's define set; no header tests that
  macro (inert in both rows). Every other FABLE_RETAIL_* in rows/ resolves in rebuild/include or hdr/.

## 0x006CE300 identity chain
- Besides FableWin.pdb 0x0247F080: the out-of-line push_back 0x006CE530 calls _M_insert_overflow 0x006CE570, which the landed, PDB-named
  0x0084F850 (GetAllThingsInRoughArea<const CThingCreatureBase, vector<const CThingCreatureBase*>, CFilter_Bind2<...> >, Ego_r) also calls: the
  container is vector<const CThingCreatureBase*>; the type test is CThingCreatureBase's 0x0F; the filter callee is CThingFilter_IsNot 0x006A76A0.
  T non-const is the FableWin spelling (no Ego_r instance of this filter / container pair).

## 0x009147E0's inline ScaresBirds::operator()
- The row defines `inline bool CThingFilter_ScaresBirds::operator()` with the same tokens as its landed TU-mate 0x00914650 (both `inline`, so a
  pick-any COMDAT in each object: no LNK2005, unlike the nf80 strong `_DEFINE` case). A header opt-in in engine/CThingFilter_ScaresBirds.h
  (`FABLE_RETAIL_CTHINGFILTER_SCARESBIRDS_BODY`, the OVERLAPPINGCIRCLE_BODY pattern) would hold the text once for both rows: follow-up, not done
  (the header is outside this round's assignment and 0x00914650 would need a respell).

## Header ownership / needs header claim
- engine/CThingTrackNode.h is outside the four headers this round assigns (CThingSearchTools.h, NThingMap_CThingMap.h, CThingDeadCreature.h,
  CThingMarker.h); it got one additive opt-in (FABLE_RETAIL_CTHINGTRACKNODE_STATTYPEFLAGS), as NF107 did for CThingMarker.h etc.
  COORDINATION_CLAIMS.tsv has no claim on it (nor on 0x007FEAF0) from either account.
- Needs header claim (not applied): a standalone engine/CFilter_Bind5.h (today the only CFilter_Bind5 is in engine/TargetingPlayerSupport.h
  behind FABLE_RETAIL_CTCTARGETINGPLAYER_UPDATE_LIST) would let 0x0080ECC0 drop the borrowed 0x00809A70 targeting define / include set.

## Descriptive names
- None added. All self leafs are PDB instance names; the anonymous-namespace leaf of 0x0080ECC0 is cut at `?A0x` (catalog convention).

## EH funclets / file moves
- No file in rebuild/src or rebuild/tests #includes any of the 17 changed sources (grep of their file names).
- 0x006CE300 and 0x0080ECC0 change catalog name (RENAMES s1), so their source / test slugs (GetAllThingsInRoughAreaCThingGameObject... /
  CThingSearchToolsCalls_...) move at landing.

## Locks
- The 16 rows of rows.txt (NF110); 0x007FEAF0 claimed (nflocks/007feaf0 = NF110). No UNMATCHED_ROWS / Account B row touched; 0x0063F4B0
  only rechecked (unchanged source).

## Rule slips
- 0x009147E0 took 5 graded forms (NF107 set; EGOD-shaped GetPBest body in-row; caller set; + THISEND / byte-offset cells; + T's type-flags
  define = RELOCATION_MATCH). The first two failed for the same missing prerequisite (the expanded ScaresBirds filter body), the fourth for a
  missing type-flags define, so I counted 3 body forms; recorded for the coordinator to judge.
- 0x00937690's first compile failed on my include-set slip (TargetingInteractionFilters_pass24.h before CThingAICreature.h), 1 grade.
- 0x00987430's and 0x0080ECC0's header comments were corrected (a FableWin address; a caller count) after their recheck pass; the final
  recheck_final.log run covers the corrected files.
- Several read-only orientation calls ran without the env line.
