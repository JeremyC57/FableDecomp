# Landing 2026-11-04s: chunk 756 (retry of 40 parked rows 1-16 bytes off retail, 81-183 bytes, C++ only), 18 landed

## Result
- **Landed 18 of 40** (`logs/land.log`). Each was graded read-only on the merged tree first (`logs/new2.log`): 6 MATCH and
  12 RELOCATION_MATCH, all PASS.
  - The rows include:
    - the std::swap / iter_swap twins over pair<unsigned long,C3DVector> and pair<C3DVector,long>;
    - CRegionMinimap::InitialiseRegionBox;
    - map<void*,CRGBColour>::operator[];
    - CTCHero::SetWeaponsAsUsable, CThing::AddAndCopyTC;
    - CombatDialogueManager::CanSaySpeech;
    - CAIStateGroup_MinionSitOnSeatBase::GetSeat;
    - C4DVector::C4DVector(const CRGBColour&);
    - CPhysicsMeshObject::GetBoundingBoxXEdge;
    - CParsedItem::InitAsInteger;
    - CQuestManager::FailAllActiveQuests.
  - What fixed each one is in `agent_NOTES.md`. The main fixes:
    - real element types;
    - retail headers;
    - shapes copied from the narrow/wide twin;
    - same-TU `__declspec(noinline)` callee bodies;
    - FableWin inline transforms.
- **Asm rules (owner, 2026-11-04):** no asm, helper, pragma or `volatile` in any row or test. 0x009B9770 InitAsFloat's earlier
  `volatile` divisor was removed; that row stays parked.
- **Parked 22** (`pending/2026-11-04_linux_hand756/differ/`). The residues are register entry choice, frame slots, argument-load order
  and cross-jump tails.
  - Not every parked row got the full 10+ grades (`STATUS.tsv`): 0x00A69B70 and 0x006E2C00 got pass 1 only, and 0x009E4CB0 got 3
    (it shares the residue of its twin 0x00997880, which got 13). They are the first picks for a later retry.
- **Header debt** (vland warns, does not reject):
  - 0x004B2AD0 uses local CQuestManager / list models. The CQuestManager.h retail branch has no ActiveQuests, and the shared list
    inlines both callees.
  - 0x006451DD uses local C2DVector / C3DVector / CRightHandedSet.

## Headers (21 changed; every addition behind a new or existing FABLE_RETAIL_* opt-in)
- **New sub-macros:** FABLE_RETAIL_C4DVECTOR_RGBCOLOUR, _CMATRIX3X4_TRANSFORMVECTOR, _CPHYSICSMESHOBJECT_MATRIX / _EDGES,
  _CAISTATEGROUPBASE_PEEK, _C2DBOXI_COPY, _CTHING_ADDANDCOPYTC, _CTHINGSEARCHTOOLS_CLIPAREA / _LEAFBODY and _CTHINGOBJECT_STATGETTYPE,
  among others (`logs/hdrcheck2_pre_merge.txt`).
- **Merge with chunks 754/755** (all changed after the 756 base):
  - CTCTalk.h, CombatDialogueManager.h and CThing.h merged 3-way cleanly.
  - Resolved by hand:
    - CTCShop.h: the union, adding IsOpeningHours.
    - CAIStateGroupBase.h: the union; 756's `_PEEK` block follows 755's `_AIACTIONS`.
    - CIntelligentPointer.h: kept the landed 754 `operator=`.
    - CMainGameComponent.h: kept 755's `GetCurrentWorldFrame` under `_WORLDFRAME`, and row 0x008AF6E0 now defines that sub-macro
      instead of redeclaring the member.
- hdrcheck2: `default=same` for all 21 headers.
- **Siblings:** `ppcmp.py` over 899 includers shows 12 different. All 12 were rechecked on the merged tree (`logs/recheck.log`):
  RELOCATION_MATCH + PASS.

## Names
- **Labels:** `ghidra_out/labels_landing_2026-11-04s.tsv` (34). `apply_labels.py` made 20 catalog and 18 manifest renames.
- **Queued PRIORITY** (`land0411s`), among others:
  - 0x004B2AD0 FailAllActiveQuests (was DeactivateEventQuests; the forwarder 0x00891520 jumps to it);
  - 0x004B2280 CQuestManager::SetQuestAsFailed (landed as doFailQuest);
  - 0x00BB17D0 the C4DVector(const CRGBColour&) ctor (was SetVSVectorAsColour);
  - 0x00A3A840 DecompressStream takes `bool&`;
  - 0x00997880 is the `__fastcall` NFile::GetItemFromPath;
  - 0x00A11C20 GetBoundingBoxXEdge (was GetHalfAxisX).
- `bindings_multi_check.py`: 0. `sync_manifest_from_catalog.py --dry-run`: 0 / 0 / 0.

## Coverage
46,669 genuine of 49,568; 5,811,273 function bytes (56.85%).
