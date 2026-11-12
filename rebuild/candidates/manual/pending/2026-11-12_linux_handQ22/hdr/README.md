# Q22 header overlay (grade with `export FABLE_HDR_OVERLAY=$S/w22/chunkQ22/hdr`)

Every file is a full copy of the CURRENT repo header (re-diffed against rebuild/include at hand-back, 2026-11-12: additions only,
0 removed lines) plus `#ifdef FABLE_RETAIL_*` opt-ins; the default path of every header is unchanged. No new headers. Nothing was
copied from the Q21 overlay (no overlap with it).

| header | opt-in | change | rows |
|---|---|---|---|
| engine/CAIStateGroupBase.h | `FABLE_RETAIL_CAISTATEGROUPBASE_PERFORMACTIONONCE` | declares `EAIActionReturn AIAction_PerformAction(const CCreatureActionBase&)` 0x0097D8B0 (Ego_r 0x00848680; the one-shot overload, inserted before the existing PERFORMACTION block) | 0x0096CD70 (match) |
| engine/CTCTavernTable.h | `FABLE_RETAIL_CTCTAVERNTABLE_SEATS` | Ego_r members `bool BeingServed` +0x0C and `std::vector<CThingAICreature*> Occupants` +0x10 (public); `SetSeatState(long, bool)` 0x007B8530 and `CThingObject* GetFood(long)` 0x007B85C0; includes stlport/fable_containers.h, forward-declares CThingAICreature | 0x0096CD70 (match) |
| engine/NCreatureMode_CModeSitFloor.h | `FABLE_RETAIL_NCREATUREMODE_CMODESITTAVERNSTOOL` | `NCreatureMode::CModeSitTavernStool : CModeSitBase` (ctor `(CThing&)` 0x0094B530, virtual dtor, GetType 0x0094B5B0, private Filter[0xC] +0x2C; sizeof 0x38 asserted; vtable 0x01292FF8) | 0x0096CD70 (match) |
| engine/CNavQuadTree.h | `FABLE_RETAIL_CNAVQUADTREE_SAVETOFILE` (inside FABLE_RETAIL_CNAVQUADTREE) | declares `void SaveToFile(CAFile&) const` 0x00A78B20 (Ego_r / FableWin QBE) | 0x00A78B20 (match) |
| engine/CANavQuadTreeNode.h | `FABLE_RETAIL_CANAVQUADTREENODE_SAVETOFILE` (inside FABLE_RETAIL_CANAVQUADTREENODE) | protected slots 16..22 in PDB order: SetUpNeighboursWithLeaf, SetUpNeighbours (`(ENavQuadTreeNeighbours, CANavQuadTreeNode*)`), CalculateChildrenNeighbours, RemoveNeighbours, AddToIndexTable(long&, map<const CANavQuadTreeNode*,long>&) const (+0x50), SaveToFile(CAFile&, const map&, bool, const C2DVector&) const (+0x54), ConvertIndexesToPointers(const map<long,CANavQuadTreeNode*>&); includes stlport/fable_map.h and declares `enum ENavQuadTreeNeighbours { }` (enumerators not recovered) | 0x00A78B20 (match) |
| engine/CAFile.h | `FABLE_RETAIL_CAFILE_WRITEVAR` | FableWin.pdb inline `template<class T> void WriteVar(const T&)` = `Write(&var, sizeof(T), 0)` (no alignment branch in retail; as the landed 0x00A76950 model) | 0x00A78B20 (match) |
| engine/NPlayerGui_CDrawQuestInfo.h | `FABLE_RETAIL_CDRAWQUESTINFO_UPDATEMINIGAME` | declares `UpdateMiniGame(float percentage, long value, long maxValue)` 0x00644EFD and `UpdateMiniGame(const CRGBColour&)` 0x00644F85 (public) | 0x00649E8B (DIFFER) |
| engine/NPlayerGui_CDrawBase.h | `FABLE_RETAIL_NPLAYERGUI_CDRAWBASE_SETASDRAWING` | FableWin.pdb inline `void SetAsDrawing(bool drawing) { Drawing = drawing; }` (retail `mov byte ptr [edi+18h],1` at 0x00649EFB) | 0x00649E8B (DIFFER) |

The matched sources also use existing repo opt-ins (each row's #define block lists them), notably FABLE_RETAIL_CTHING_GETTC_EXPANDED +
FABLE_RETAIL_CTCWEAPONTRAIL_UPDATE (the `const bool&` HasTC) and FABLE_RETAIL_CCOUNTEDPOINTER_VISIBLE_CALLED_CTOR/_DTOR in 0x0096CD70, and
FABLE_RETAIL_STLP_MAP + FABLE_RETAIL_STLP_LIST(_SIZE_DISTANCE) + FABLE_RETAIL_CNAVIGATIONPOSITION_CTORS in 0x00A78B20.
