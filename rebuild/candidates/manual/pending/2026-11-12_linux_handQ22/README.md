# Chunk Q22 (six parked rows, one at a time, Opus 5.5)

3 of 6 matched; 2 landed in landing dn:
- 0x0096CD70 CAIStateGroup_EatAtTable::Update: ported onto the shared headers; `bool failed = AIAction_PerformAction(...) !=
  AI_ACTION_FINISHED;` fixed the last `setne`.
- 0x00A78B20 CNavQuadTree::SaveToFile: ported, matched on the first grade (statement order from FableWin 0x03289B50).
HELD BACK (held_back/): 0x006C1250 CTCBuyableHouse::FrameUpdate matches, but on the draft's local models of eight classes that have
shared headers (owner shared-header rule; strict mode rejects it). Chunk Q23 is porting it.
DIFFER: 0x00649E8B CDrawFishingGame::Draw (469v469; retail gives the two implicit CCharString temporaries separate stack slots, ours
share one). Not attempted: 0x0063F7C0 (port plan on its line 2; Q23 takes it), 0x00489D40 CPlayer::CreateCharacter.
Made-up names (NOTES): NAV_QUAD_TREE_FILE_VERSION 0x0129DCC8, NAV_QUAD_TREE_FILE_END_MARKER 0x0129DCCC.
