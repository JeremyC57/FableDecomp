# agent721c -- chunk 721 (w22, round 4)

## Row
- **0x00A1CEB0 CNavigatorAStar::Initialise (992)** -- parked DIFFER(992v992), 2 lines, 14 graded attempts (LOG.tsv).
  Best: `differ/00a1ceb0.cpp` (self-contained TU; grade with `FABLE_HDR_OVERLAY=<agent721c>/hdr`).
  Residue: one x87 store one instruction late in the CAStarState temporary set-up (retail `mov ecx,[NoOfStates]; fstp
  [esp+2Ch]; mov eax,[..]; inc ecx`, ours `mov ecx,..; mov eax,..; inc ecx; fstp [esp+2Ch]`).
  Levers that moved it (12 -> 2 lines): a named `std::pair<std::map<long,CAStarState>::iterator,bool> inserted` for the
  States insert (the open-list Push takes `&inserted.first->second` after the insert temporaries are gone), and the
  CAStarStateTotalGreaterThan comparator deriving `std::binary_function<const CAStarState*, const CAStarState*, bool>`
  (an empty-base comparator temporary is no longer zero-stored).
  Probes that moved nothing (all logged): SetCostToThisState placement (c, d), `distance + distance` heuristic (e, 6 lines),
  ctor without the cost initialisers (f), SetOnOpen closing the closed-list flag first (g), explicit
  `std::pair<long,CAStarState>` (h, 136 lines), assignment-style copy ctor (q), GetPosition() accessors (r), heuristic in
  a named local (t), the key in a named local (u). Last three moved nothing -> parked.

## Headers (hdr/ is the complete merged overlay; hdrcheck2: every file default=same)
- engine/CANavQuadTreeNode.h `FABLE_RETAIL_CANAVQUADTREENODE`: polymorphic node (slots 0..8, GetNodeType slot 6 returning
  ENavQuadTreeNodeType {NAV_NODE_QUADTREE, NAVIGABLE, BLOCKED, SWITCHABLE}), operator== 0x00A763F0, members Position +4,
  QuadTreeLevel, NavLayer +0xD, LeafNode, Blocked.
- engine/CNavBlockedQuadTreeNode.h `FABLE_RETAIL_CNAVBLOCKEDQUADTREENODE`: `: CANavQuadTreeNode`, ctor(uchar,uchar)
  0x00A768C0, operator= 0x00A76240.
- engine/CNavigatorManager.h `FABLE_RETAIL_CNAVIGATORMANAGER`: CIsNavigator, CNavigatorQueueManager (AddNavigatorToQueue
  0x00A154D0; inline RemoveNavigatorFromQueue = `Navigators.remove_if(CIsNavigator(&nav))`), QueueManager +0x2C,
  GetNodeAt 0x00A14F70.
- stlport/fable_containers.h: rebased on the current repo file (the owner's 2026-11-03 D2 tag-ctor default replaced the
  round-3 `FABLE_RETAIL_STLP_TAG_CTORS` opt-in, which is dropped); keeps chunk 719's opt-ins and adds
  `FABLE_RETAIL_STLP_LIST_MEMBERS` (list size() loop, front(), back(), push_front, template remove_if declaration).
  b.cpp regraded on the rebased header: unchanged (attempt 14).
- engine/C2DVector.h, C3DBoundingBox.h, CVectorMap.h, CThing.h, CTCVillage.h, CTCInventoryItem.h: round-3 / chunk 719-720 branches carried.

## Names
- 0x00A20970 CSteeringAgent::SetGoal(const C2DVector&, const C2DVector&) -- already in agent721's NAME_FIXES (not repeated).
- TU-local models (no engine header exists): CNavWayPoint, CPriorityQueue<T> (Push<C> 0x00A22460), CNavigatorAStar with the
  nested CAStarState, CAStarStateTotalGreaterThan; member names from Ego_r.pdb.
