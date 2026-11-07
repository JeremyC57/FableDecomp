# NOTES -- chunk 844 (expert retry of the eleven rows chunks 839/840 parked)

Result: 4 landed (0x0070C4E0 MATCH; 0x0073ACC0, 0x007534B0, 0x00E482F0 RELOCATION_MATCH; all four tests PASS under g++ and
VC7.1, read-only vland: WINS 4, no RELOC_IDENTITY), 7 parked with their best source and test in differ/. Every attempt is in
STATUS.tsv (notes column); probe sources stay in probe/ for the record. Names proven (PDB, two sources) for all eleven rows;
descriptive callee names are marked below.

## 0x0070C4E0 CAnimateThing::GetFirstCombinationAnimFromType(long) -- src/0070c4e0.cpp, MATCH (1 attempt)
Shape: std::find_if over the combination-anim list with a predicate functor on the type, return the found set or 0. Key
spelling: the STLport 4.6 `_List_iterator(const iterator&)` user copy ctor on the local list-iterator model. That by-value
iterator with a user copy ctor is what leaves retail's first-return-block `xor eax,eax` ahead of `pop edi` (micro probes x1-x5;
the landed TurnToFace 0x0095EDF0 shows the same). Callees unchanged from chunk 840 (none out of line). Label right already.

## 0x0073ACC0 CTCCreatureModeManager::AddMode(CCountedPointer<CModeBase>) -- src/0073acc0.cpp, RELOCATION_MATCH (3 attempts)
Shape: `std::find_if(PModes.begin(), end, CIsModeOfType(type))` over the STLport-style list iterator (user copy ctor), `if (it !=
PModes.end()) return false;`, then the old front leaves (OnLeave / replaced-type check), push_front, the new front resumes,
UpdateTurningAnims through the look component, TimeForNextIdleMode = CWorld::GetFrame() + ConstantFPS * 30, RemoveMode(replaced).
Respelled this chunk to the bound symbols (one symbol, one address -- NAME_FIXES.tsv): list insert 0x007421C0 is push_front
(retail links the new node before begin(); the chunk 840 model and test said push_back -- the test now checks that the NEW mode
is the one resumed), CWorld::GetFrame 0x0049D870 (`?GetFrame@CWorld@@SIJXZ`, static __fastcall; labels GetWorldFrame /
GFGetCurrentFrame are the same getter), 0x01375550 read as the protected static CMainGameComponent::ConstantFPS through the
inline GetConstantFPS() (the landed 0x00836260 spelling; five global stand-ins are bound there). Leaf: the local CModePointer
model mangles as `UCModePointer@@` (stand-in); the PDB symbol `?AddMode@CTCCreatureModeManager@@QAE_NV?$CCountedPointer@VCModeBase@
CTCCreatureModeManager@@@@@Z` is available through engine/CTCCreatureModeManager.h (FABLE_RETAIL_CTCCREATUREMODEMANAGER_ADDMODEPOINTER)
-- not switched here (the local list/iterator model is what matches; header debt below). Descriptive: CModeBase slots 1/5/6/11
(engine header: 5 OnActivate, 6 OnDeactivate, 8/11 IsIdleMode/IsRemoveOnDeactivate), CTCLook::UpdateTurningAnims 0x00754050
(bound). Labels: 0x0049D870 and 0x01375550 (LABELS.tsv).

## 0x007534B0 CTCLook::IsPosInViewRange(const C3DVector&, float, bool, bool) const -- src/007534b0.cpp, RELOCATION_MATCH (13)
Semantics corrected: retail returns false when PParentCreature is null or dead (+0x91 bit 0) -- the chunk 757/840 source
skipped the animation check instead (md.py hid the jump targets; the test now encodes retail). Shape: GetInterface(0x5A) with
the inlined CVectorMap::LowerBound 0x004365B0, three separate `return false;` statements (null, dead, blocking turn state 7
with queued entries and flag bit 1 clear) -- that gives retail's one shared `xor al,al` block; body facing through the +0x60
object's slot 63; `C3DVector from = fromHead ? GetHeadPos() : PParentCreature->GetPos();` then `GFGetAngleXYTo(C3DVector(from),
position)` with a POD C3DVector (the explicit copy at the call elides the second copy and hoists the three loads as retail);
the angle wrap written in the body on a named copy `float angle = lookAngle;` (an inline WrapAngle(float) helper stays out of
line once the function has the three early returns, under /d2inlS20 and /d2inlT80 as well -- diagnostics only; `__forceinline`
also matched but is not a landing spelling). Respelled to the bound symbols: class C3DVector (leaf now the retail
`?IsPosInViewRange@CTCLook@@QBE_NABVC3DVector@@M_N1@Z`), `?GetPos@CThing@@QBEABVC3DVector@@XZ`, private `?GetHeadPos@CTCLook@@ABE?
AVC3DVector@@XZ`, `?GFGetAngleXYTo@@YIMABVC3DVector@@0@Z` 0x00661540, `?AngleDifference@@YGMMM@Z` 0x00A441E0 (two symbols are bound
there, see NAME_FIXES.tsv), `_CIfmod` 0x00BFEB28 (import thunk). Descriptive: CTCTurnSource::GetState 0x0070B470 (the bound
spelling of the landed 0x00753B40 / 0x006D5AF0; the catalog label CWorld::DrawGetEnvironment is wrong -- LABELS.tsv), CTurnState
members, CThingPlacement slot 63 GetAngleXY, Flags91.

## 0x00E482F0 NScript::CV_BanditTollScript::CheckForLeaderDead() -- src/00e482f0.cpp, RELOCATION_MATCH (1 attempt)
Chunk 839's a4 source with `CScriptThing::operator=` as a plain `inline` body and the single-site lever `do { TauntingBandit =
Game->GetNearestWithScriptName(...); } while (0);` around the first loop re-assignment (chunk 839's own next idea). VC7.1's
inliner takes loop-nested call sites first: the one-trip loop makes the budget-limited operator= expand at that site only and
stay the 0x008AB980 call at the TollBandit / BlockingBandit re-assignments, exactly retail's split (plain inline expanded none,
`__forceinline` all three). Behaviour-identical; noted on line 2 for review at landing. The same-type
`reinterpret_cast<const CScriptThing&>` on the three inner wait loops (landed CHTE_FreedomFighter::Main 0x00D6A560 form,
owner-accepted 2026-11-07) is kept from chunk 839. Names all PDB (engine headers).

## Parked rows (differ/<addr>.cpp + <addr>.test.cpp; line 2 grade/residue, line 3 next idea)
- 0x00C52BB0 CCpuInfo::GetAMDModelName -- DIFFER(140v140), 4 lines, 51 attempts. Residue: the family-5 K5 test register. Finding:
  the Model load takes eax only when the arm's first compare constant is the switch base 4 (`< 4` -> `cmp eax,4; jae`), ecx with any
  other constant (`<= 3` gives retail's `cmp ..,3; ja` in ecx); nothing in reach (case order, locals, no-op nodes, casts, range
  checks, nested switch, ternary chain, default labels, the QFE 4035 cl) moves it.
- 0x0058301A CTCInventory::CreateEditBoxActions -- DIFFER(321v321), 2 lines, 50 attempts. The address-taken `box` home store
  stays ahead of the first setter's vtable load (call-result pointer); retail hoists the load. Inline non-virtual wrappers, rvalue
  temps at the push_back (built at the push_back site instead), register/const/block forms change nothing.
- 0x0047ED76 CTCInGameMenu::SetToSave -- DIFFER(366v366), 2 lines, 54 attempts. Scheduler tie `fsubp` / `push ecx` at the
  ChangePosition depth slot; robust to every ctor/temp/named-local/constant-order/default-arg spelling and to /G5-/G7, /GS, /O1.
  Model respelled to the PDB slot names ChangePosition (31) / Initialise (43).
- 0x00E76E90 NScript::CV_ExposeMayorScript::HeroFlashesLamp -- DIFFER(1613v1612), 10 lines, 27 attempts. Block order of the merged
  in-loop return block inside the second inlined recursion copy; `#pragma inline_recursion(on)` now inside `#ifdef _MSC_VER`.
- 0x00DE4710 CQ_RansomVictimScript::DisappearFollowers -- DIFFER(1913v1428), 473 lines (best shape); documented per-exit vector-dtor
  blocker. New: declared-only vector dtor specialisations give 1024v1428 / md 172 (every exit a call -- wrong shape).
- 0x00775350 CTCShop::UpdateStock -- DIFFER(1971v1988), 791 lines, 18 attempts; documented inline-budget blocker. New: the helper as a
  template, unchanged. The inliner's loop-first site order explains retail's two expansions (per-day loop, then the first depth-1 site).
- 0x00D0EE70 CQ_BanditCampBossBattleScript::CheckForFirstAreaMassacre -- DIFFER(1991v1989), 466 lines (new best from 640); documented
  per-exit dtor blocker. New: declared-only `template<> std::vector<CScriptThing>::~vector(); template<> std::vector<unsigned long>::
  ~vector();` make every vector-dtor exit a call; the two expanded exits of the second for-loop remain.

## Header debt
- engine/CTCInGameMenu.h FABLE_RETAIL_CTCINGAMEMENU branch: add Components +0x58 (std::vector<NUISystem::CComponent*>), SaveGameIndex
  +0xC8, FramesToWaitToSave +0xCC, PSaveInProgress +0xD0, SaveInProgressTime +0xD8 (retail STLport layout, PDB offsets - 0x20) so
  0x0047ED76 can drop its local model.
- engine/CTCCreatureModeManager.h: the PDB AddMode symbol needs the real std::list<CCountedPointer<CModeBase> > iterator with
  STLport's user copy ctor to keep the matched shape; the row's local CModeList/CModeIterator model is the stand-in.
- stlport/fable_containers.h: a vector dtor split by site (FABLE_RETAIL_STLP_VECTOR_DTOR_INLINE-style) and the allocator-argument
  vector constructor declared only (0x00450C72) would be needed for 0x00DE4710 / 0x00D0EE70.
