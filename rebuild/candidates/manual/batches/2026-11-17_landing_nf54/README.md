# 2026-11-17 landing nf54: name-fix round 7, NF54 (60 rows: game-side tail, first half)

Owner: "Continue the naming fix queue with two agents until complete".

NF54 took the first half of the game-side tail of the queue (targets below 0x00800000): 36 small entries with 73 binder rows,
from NF41's probe and NF44's binder lists. NF55 has the second half. Brief, worklist and records: `naming/2026-11-17_agentNF54/`.

## Rows (73 worklist rows, all terminal; STATUS.tsv)

| Status | Rows |
|---|---|
| ported | 52 worklist rows + 8 self rows of NF54's targets = 60 changed rows |
| not-a-binder | 4 (0x0042F5A9, 0x0049A0B0, 0x005383DD, 0x0095F9A0 already bind the right spelling) |
| blocked/port | 17 (unchanged; each has a probe grade, a reason and the class to port first) |

The 8 self rows: 0x00458F89, 0x0049B6B0, 0x004FB470, 0x005178A0, 0x0051E580, 0x005B33F4, 0x005BC0D3 and 0x005ED300.

**Blocked/port rows, by what must move first:**
- **the CTCInventoryBase family (about a dozen local classes):** the seven CloseInventory rows 0x0057770A, 0x005778CA, 0x00587847,
  0x00589D30, 0x0059E2C7, 0x0059F304 and 0x005FCE60. Their stand-in 0x005BD597 lives in local NInventory models.
- **local CWorld / CThing / CThingManager / CAFile models:** 0x00416953 (local CWorld, C2039 x8), 0x004CA010 (local CThingManager;
  its callee 0x00A37300 has no PDB name), 0x00500230 (local CAFile `ReadVar<int>`).
- **CThing::PeekGlobalDefinition (protected) from a row's own class:** 0x006A9590, 0x00837050 and 0x00837060 (e141). The last two
  need their class on engine/CThingTrackNode.h.
- **intelligent-pointer vectors on a local CThing that is not a CBaseObject:** 0x008DDAD0 (C2440) and 0x008E0060 (27 local classes).
- **CTCAppearanceMorphBase::CMeshMorphSet:** 0x00722270 and 0x007222B0. The shared header lacks the nested set classes.

## Entries (APPLIED.tsv)
- **23 done:** nf44 3, 43, 45, 48, 49, 52, 94 and 99; nf41 e8, e71, e114, e134, e166, e170, e171, e173, e182, e193, e196, e207,
  e38, e39 and e50.
- **2 done for this half:** nf44 1 and e105. Their other binders are NF55's rows.
- **2 claims rejected and applied under the proven spelling (REJECTED.tsv):**
  - e26: the queued `CArray<unsigned long>::push_back` is wrong. The two callers pass `std::vector<unsigned long>` references, so it
    is `vector<unsigned long>::push_back` (0x005178A0, re-authored under that spelling).
  - nf44 4: the queued spelling was malformed; applied as `vector<CTexture>::push_back`.
- **9 blocked:** e206, e172, e141, e174, e54, nf44 50, nf44 5, nf44 54 and nf44 7.
  - e206: the wrong spellings are inside 0x004336BC itself, which was not in the worklist (queued).
  - The other eight wait on the blocked/port rows above. In e54 and nf44 5 the claim is proven and the self row is re-authored.

## Rows and checks
- **Self renames (section 1, 20 rows; 26 file moves for sources and tests).** Among them:
  - `CPersistContext::Transfer<CWideString>` / `<double>` / `<CWorld::ESaveType>` (the two-argument instance; 0x004AB640 is the
    three-argument overload of the same name);
  - `CPersistTraits<CFloatRange>::TransferBinaryIn`;
  - `vector<CTexture>::push_back`, `vector<unsigned long>::push_back`, `~vector<CEngineTextureMorph>`;
  - `std::uninitialized_fill_n` / `std::__copy` instances;
  - CWorldMap::GetRegionCount / GetLevelDimensions / GetLevelStaticMapChecksum;
  - CThingManager::ThingTypeToTypeFlags (was CEditWorldMap::GetEditMap);
  - the out-of-line CAnimInit argument constructor 0x005B33F4 (was `CRumbleEffect_005b33f4::Initialise`);
  - `CIntelligentPointer<const CThing>::operator=`, CVectorMap Insert, LSocket::Receive (LHNetworkLib), CTCDExplosion and NUISystem
    CList leaves.
- **Coordinator correction:** 0x005D84D1 is a leaf-only change on a descriptive stand-in list (`CRetailList_node0x10::Buynode`, now
  taking `std::pair<CWideString, long>`). NF54's section 1 would have given it that stand-in class as its catalog name; the
  coordinator kept the old catalog name `_global::StdList_CreateNode_sz0x10_005d84d1`.
- **Funclets:** no EH funclet includes a changed row. funclet_includes_check reports 1,355 checked, 0 broken.
- **Recheck (`logs/recheck_part0..2.log`):** all 60 changed rows graded from the repo after landing: 58 RELOCATION_MATCH and 2 MATCH,
  every test PASS. NF54's own final run (`naming/2026-11-17_agentNF54/recheck_final.log`) agrees.
- **Checks:**
  - `naming/rebind.py` removed 157 binding lines and added 117. It warned on 0x0047E07F and 0x005BC216 (destructor leaves it cannot
    match by name) and took the largest function, the destructor that vland graded;
  - bindings_multi_check is at its baseline (`NullCoord`).

## Headers (8 opt-ins in 7 files; default path unchanged)
NGameText_CDataBank.h, CPersistTraits.h (2), CWorld.h, boost/scoped_ptr.h, CWorldMap.h, CAnimInit.h and stlport/fable_containers.h.
Details: `naming/2026-11-17_agentNF54/hdr/README.md`.
- fable_containers.h is the one overlay that edits existing lines: two `#ifdef FABLE_RETAIL_AUGMENTATION_PARTICLES` become
  `#if defined(...) || defined(FABLE_RETAIL_STLP_DESTROY_DISPATCH_FORCEINLINE)`. With the new macro undefined the condition is
  unchanged; hdrcheck2 confirms the default path preprocesses the same.

## Name fixes queued (`land1117fu`, 20)
From NF54's NAME_FIXES.tsv. They include:
- callers of NF54's renamed rows outside its worklist that still bind the old spellings (0x00506E30, 0x005D8626, 0x00602E30,
  0x0053AD07, 0x0053BF20);
- the crude self rows 0x005D96F0 / 0x005D99D0 (CTCInventoryItem statics) and 0x004091C0 (needs a whole-row port);
- the catalog name of 0x004A6AB0, which is `~CWorld`, not `~CBase`;
- CAIStateGroup_CreatureUseCoverBase RefreshCoverList / IsRunnableLeaf, vector reserve / push_back instances and
  CGameScriptThing::MsgGetThingsKilled.

## Rule slips (NF54 `work/slips.txt`)
- One Bash call exited 1, before work started.
- Five misparsed grade fields in grades.log were corrected afterwards.
- Ten landed rows outside the worklist were compiled in the private directory to read their bindings; nothing was written.
- One heredoc ran backticks as a command and damaged a row comment, which was fixed before grading.
- The agent released the locks of its 24 unchanged rows, which the brief does not ask for.
- The final recheck was repeated after a late self-row fix; the first log is kept in the agent's work directory.
