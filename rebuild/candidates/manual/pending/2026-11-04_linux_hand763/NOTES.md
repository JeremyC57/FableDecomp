# Chunk 763 notes (25 rows parked by chunk 760, 54-297 bytes)

The read-only vland (`vland_result.txt`) has 8 wins (2 MATCH, 6 RELOCATION_MATCH, all behaviour PASS). The other 17 rows are parked in `differ/` with passing tests. Every row except the third-party 0x00C3A7D0 has 10 or more grades (see `LOG.tsv` and `STATUS.tsv`).

## Results
| addr | row | result | what fixed it / residue |
|---|---|---|---|
| 00be24d0 | C2DAlphaMaskInfo::Reset | MATCH | The real class is the PDB C2DAlphaMaskInfo (the label CVO_OracleMarker::Init was propagated). `AlphaMaskPositionFactors = C4DVector(0,0,0,0)` as a temporary gives retail's 0x10 frame, with the four zero stores after the member copy |
| 00be2540 | C2DAlphaMaskInfo::C2DAlphaMaskInfo | MATCH | Reset's body written in the ctor (`{ Reset(); }` also matches) |
| 00441ee0 | CPrimitiveHandleSet_00441ee0 dtor | RELOCATION_MATCH | Shared STLport vector, plus CEnginePrimitiveHandle with the new FABLE_RETAIL_CENGINEPRIMITIVEHANDLE_DTOR opt-in (__forceinline virtual dtor) |
| 00863280 | CThingBodyReorienter::GradualReorientBody | RELOCATION_MATCH | The short-way ternary rewritten as `delta = difference; if (difference > 0.5f) delta = difference - 1.0f;`, which homes the difference the way retail does |
| 008956b0 | CGameScriptInterface::EntityTeleportToHeroPosition | RELOCATION_MATCH | Shared retail headers. The hero and thing XY are two C2DVector locals (retail x87 order). The wander target C3DVector is declared in the inner scope, so its frame is shared. The navigation comes through `thing->PeekTC(TCI_NAVIGATION, &navigation)`, which goes through the const GetTC and so calls the const LowerBound 0x004365B0; the non-const GetTC gave a RELOC_CONFLICT |
| 008cf7d0 | CCreatureAction_TentacleStrikeLoop::FinishAction | RELOCATION_MATCH | The TentacleStrikeInto / FrameUpdate sibling models. The limit is written `(long)(def->TentacleSmackDownTime * CMainGameComponent::GetConstantFPS())`; with the float first, the frame packs the way retail's does. FinishAction is virtual (the UAE leaf) |
| 00cbc130 | vector<pair<u64,CPersistInfo>>::_M_insert_overflow | RELOCATION_MATCH | The real element type: the Ego_r pair<unsigned __int64, NScript::CRuntimeScriptedEntityPersist::CPersistInfo> from the shared engine header (the earlier SMusicTrack guess was wrong). The shared STLport body is explicitly instantiated |
| 0092bf50 | CAIStateGroup_MinionWander::FaceAwayFromBuildings | RELOCATION_MATCH | Same-TU template bodies for CThingCompare_Nearest::operator() 0x0092BE70 and GetPBestLeafThing 0x0092C260. With them VC7.1 keeps the building in edi and the compare copy is scheduled as in retail; both bodies byte-match too. The look-at speed constant is 0.0027777f (0x3B360A13), not 1/360 |

### Parked
The residue for each parked row is on line 2 of its `differ/<addr>.cpp` and in `STATUS.tsv`. Below are the best tries and the next idea for each.
- **0093d270 (dl=3):** the by-value GetProximityForNavigation() temporary is a dead store into the escaped key slot. The const float& getter matches but contradicts the PDB, so it is not used. Named, const, pointer and scope forms all compile the same; this needs the owner's call on the getter spelling.
- **0095d0e0 (dl=9):** the in-loop filter value is in one register. 15 loop forms tried (see differ line 2).
- **007e5570 (dl=11):** the def zero store is hoisted. Next idea: a retail-header rewrite with the GetSubDef<CBettingDef> member template, PeekPlayerManager and GetMainPlayer.
- **004b01b0 (dl=12):** the order of the begin load and the size guard. Next idea: the inline GetRegions body with __forceinline under an overlay sub-macro.
- **008fcc60 (dl=15):** the C3DVector copies alternate load and store. Operator=, the PDB accessors, /G5 and /G7, and a chained copy were tried.
- **00752e00 (dl=15):** frame word sharing only. The next try is a scope layout that lets VC7.1 reuse the dead turnAngle and targetAngle words.
- **0067e290 (dl=16):** a register rotation in save arms 2 and 3.
- **00b242d0 (dl=22):** the frame order of the position copy versus the transform result.
- **00877720 (dl=24):** the box-corner order and the Bind2 temporary packing.
- **00476e3e and 00477cd2 (dl=29):** the this spill and the 64-bit position register pair.
- **00bc2480 (dl=38):** retail keeps a 0x24 frame (CRightHandedSet copy scaled in place, Z scale in the dead flags slot). VC7.1's cross-product operand canonicalisation depends on where each operand comes from (mesh or copy) and not on source order; `Forward(mesh) ^ Up(copy)` gives retail's Up-first order. Next idea: the in-place scale on a copy with both cross operands from the mesh.
- **005fbfa0 (dl=44):** the frame matches with the same-TU GetPFirstThing body; left is the this/worldMap/region register rotation.
- **009c9170 (dl=52):** the landed GetPGroupEntry 0x009C8890 / GetPTextEntry 0x009C9070 bodies, merged into the TU, give retail's frame and keep the text pointer in a register. Left: this/index ebp/ebx are swapped, and the group release reloads the temporary. This try still uses local CResource / CIVCountedPointer models as the landed getters do; a shared-header version needs a CResource::TouchResource overlay.
- **00ad9770 (dl=77):** the same-TU IsProperSubsetOf and map::find bodies give the loop shape; register allocation and the copy order differ.
- **009e3b60 (dl=83):** retail does not CSE the GD3DFormats row loads.
- **00c3a7d0:** third-party: the libpk audio library (no PDB/RTTI entry, Dinkumware STL). It is not Fable code, so no attempts were spent.

## Fixes for GOTCHAS (one line each)
- A C4DVector(0,0,0,0) temporary assigned to a member gives retail's frame temporary and its zero stores after the copy; a member-wise clear does not (0x00BE24D0).
- A short-way wrap written `delta = d; if (d > 0.5f) delta = d - 1.0f;` instead of a ternary homes the difference as retail does (0x00863280).
- `(long)(float * longGlobal)` and `(long)(longGlobal * float)` give different frame packing; match the retail operand order (0x008CF7D0 is float first).
- Two C2DVector locals for an XY difference give retail's x87 load order; a C3DVector target declared in the inner scope shares the frame (0x008956B0).
- A TC lookup through `PeekTC(id, &p)` (const GetTC) calls the const CVectorMap LowerBound 0x004365B0, and the non-const GetTC calls 0x0040F020. A RELOC_CONFLICT on LowerBound means the const-ness is wrong (0x008956B0).
- Same-TU bodies for callees and templates that retail compiled in the same TU let VC7.1 see that the out slot does not escape, which fixes the frame and register reuse (0x0092BF50 GetPBestLeafThing / compare; 0x009C9170 getters; 0x005FBFA0 GetPFirstThing).
- Filter temporaries `CThingFilter_X(...) && CThingFilter_Y(...)` pack into dead argument slots (0x00877720).
- VC7.1 orders a cross product's multiply operands by where they come from (mesh member or local copy), not by source order (0x00BC2480).
- 1/360 in retail look-at calls is the float 0.0027777f (0x3B360A13), not 0.0027777778f (0x0092BF50).
- GetHeadTurnRangeXY written `0.5f*(range*1/360)` with `*0.5f` in the caller gives retail's fmul/fimul/fmul (0x00752E00).

## Header overlays (hdr/, all new sub-macros; hdrcheck2 default=same for every file)
- engine/CEnginePrimitiveHandle.h `FABLE_RETAIL_CENGINEPRIMITIVEHANDLE_DTOR`: __forceinline virtual dtor. Used by the matched 00441ee0.
- engine/CQuestManager.h `FABLE_RETAIL_CQUESTMANAGER_REGIONLOCK`: used by the parked 004b01b0 only.
- engine/CIEngine.h and engine/CEngine.h `FABLE_RETAIL_CIENGINE_CONVERTWORLDPOS`: the slot 13 declaration. Used by parked 00b242d0 tries only.
- Siblings: none. The default path of every overlay is unchanged, so no SIBLINGS.tsv.

## vland warnings (soft, left as is)
- **008cf7d0:** SHARED_HEADER_AVAILABLE for CThing, CKrakenTentacleDef, CDefPointer, CMainGameComponent, CWorld and CCombatActionBase. Every landed TentacleStrike* sibling (0x008CF460, 0x008CF700, ...) uses the same local models. The shared CCombatActionBase.h has no FinishAction or ctor, and CThing's retail model would need GetInterface overlays.
- **0092bf50:** SHARED_HEADER_AVAILABLE for CThing, CAIBrain, CThingSearchTools and CAIStateGroupBase. The match depends on the same-TU GetPBestLeafThing / compare bodies. The conversion needs overlays (PeekThingSearchTools, AIAction_LookAtPos, the IsIncludedInThingScans state bits) and was not attempted.
- **008cf7d0:** RELOC_NAME `??1CCombatActionBase@@UAE@XZ` -> 0x0062E900, labelled CCreatureAction_CastWillFireballSpell::~... The proposal is in LABELS.tsv.
- **00863280:** RELOC_NAME `DELTA_NOTIONAL_ZERO` -> 0x0129BA3C, labelled EPSILON. Both names are in Ego_r; not changed. RELOC_ALIAS: _CIfmod is the MSVCR71 import thunk.

## Helpers
None (no __asm / naked / _emit / volatile anywhere).
