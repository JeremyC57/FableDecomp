# Hand chunk 2039 (CThingPlayerCreature .. CThingPlayerCreature): 1 of 2 byte-exact (1 attempted)

Every grade, probe and test used `export FABLE_HDR_OVERLAY=/opt/fable_lane/w22/agent2039/hdr` (hdr/README.md: 10 opt-in / new-header
changes). Without it the row is SRC_FAIL.

## What matched (1)
- 006aca30 CThingPlayerCreature::OnCollisionWithObject (RELOCATION_MATCH 2026v2026, test PASS g++ + VC7.1; 10 grades in pass 1,
  matched at grade 9, grade 10 = the same body after comment / cast cleanup; 1 verification grade (gall.sh) in pass 2).
  The player touched an object: switch on CThingPhysical::GetProperties (slot 48): 0x2000000 object -> stock-for-sale veto, then
  HasTC<CTCActionUseActivateQuest> (quest container: per item GetDef<CThingObjectDef>; gold property -> AddMoney(gold def Gold) +
  gold sound; else DoNotAutoPickUp or not-yet-picked-up (SetItemAsPickedUp) -> CThingObject::Create + GiveItem, otherwise
  CreateAndAddObjectToInventory + CPlayerGui::AddAutoPickup + item sound; then kill the container with Kill(false)) or the single
  object (AutoPickUpAfterFirstPickUp + already picked up + AddToInventory into the item's inventory TC -> auto pickup, else
  GiveItem); 0x400 gold -> AddMoney(GetGold), gold sound, and when the gold's owner (CTCOwnedEntity) is a rented CTCBuyableHouse:
  AddRentMoney + UpdateScoreForActivity(16, rent). Finally the pickup sound (PlayAttachedCriteria) and the kill.
  Shape lessons (each measured): see the source header and LESSONS.md -- no early returns (flag + breaks: frame-packer weight),
  HasTC<T> proven against retail's unreferenced COMDAT, visible GetDef/GetSubDef bodies (no escape), if/else arm order for the
  out-of-loop auto-pickup block, `added` flag for one shared release, rent score read at the call (no named local).
- Length: the oracle length 2026 is right (ret 4 at 0x006AD217, int3 to 0x006AD220). No SPLITS.tsv entry.

## Parked for Fable 5.1 (0)

## Not attempted (1)
- 006ab820 CThingPlayerCreature::ResolveFacingDirection (2328 bytes): optional second row; not started because after row 1 well
  under half of my context remained (brief rule). No source written. Useful for whoever takes it: same TU as 0x006ACA30, so the
  hdr/ opt-ins here (GETTC_EXPANDED etc.) and the free COMDAT checks against 0x006AE5C0..0x006AE830 (two more unreferenced
  PeekTC/GetTC copies at 0x006AE790 key 0xE1 and 0x006AE7E0 key 0x7F probably belong to it or to WorldUpdate) apply.

## Skipped
- none

## Names and evidence
- Row and callees: LABELS.tsv (9 entries, all two-source). Descriptive (made-up) names: none in symbols. Local variable names in the
  row (collected, kill_thing, call_on_kill, sound, give_item, already_picked_up, added) are descriptive (not in any symbol).
- Enumerator values without PDB names: object properties 0x400 / 0x2000000 (EObjectProperty) and scoreboard activity 16
  (CTCHeroOnlineScoreboard::EOnlineScoreboardActivity) are written as numbers with comments.
- Label corrections (NAME_FIXES.tsv): 0x004C79A0 is CThing::PeekGlobalDefinition (bound as GetMeshBank / CThingCreatureBase::
  GetWorldState in landed rows 0x00837050 / 0x00837060 / 0x006A9590); 0x006AC200 bound as ?Give@Hero..; 0x0055F3B0's enum is
  nested in CTCHeroOnlineScoreboard (landed self row declares a global enum); 0x00437980 catalog label Forward_AddObject_via0x2fc.

## Header debt
- CTCStockItem / CGoldDef generated headers are flat (no CTCBase / CSubDefClassBase base): retail branches added as opt-ins.
- The shared GETDEF_GLOBAL_BODY / GETSUBDEF_BODY bodies do not byte-match retail's own COMDATs 0x0042B040 (116v98) / 0x006AE320
  (171v182) / 0x005BADE1 (171v162) (free checks); only their visibility matters to this row.
- CTCHeroStats.h now has yet another exclusive branch (RENTMONEY); a merged full-layout branch would be better.

## Probe compiles (free, not grades)
- About 25 compile-only probes: fragment probes (tools/frag.sh: container loop shapes, object-pickup tail, rent spellings),
  full-row compile-only layout probes (tools/pv.sh pa/pb/pc/pd/pe/r6: own layout only, never diffed against retail), out-of-line
  COMDAT checks against retail (tools/qx.py: HasTC / PeekTC x3 / GetDef / GetSubDef x2), c2 traces (inline trace tools/inl.sh,
  packer tools/pk.sh, c2prio + --blocks + --rotation, forced-priority what-if tools/pf.sh 53=12).

## Grade log 0x006ACA30 (pass 1)
| # | change | result |
|---|--------|--------|
| 1 | first full draft (switch on GetProperties, GETTC_EXPANDED, duplicated create+give, implicit "lit" compare) | DIFFER 2250v2026, 254/622 equal (md.py) |
| 2 | flag `give_item` (one create+give block), explicit CCharString("CTCActionUseActivateQuest") | DIFFER 2093v2026, 198/622 equal |
| 3 | HasTC<CTCActionUseActivateQuest>() (body proven vs retail COMDAT 0x006AE680) | DIFFER 1973v2026, 200/622 equal |
| 4 | HasTC<T> FABLE_FORCEINLINE in the overlay (plain c1=175, sc 283) | DIFFER 2142v2026, 268/622 equal, 453 regs+frame masked |
| 5 | drop CDEFPOINTER_RELEASE_SELFDELETING + RELEASE_DELETES (no change: release form came from escape) | DIFFER 2142v2026 |
| 6 | GETDEF_GLOBAL_BODY + GETSUBDEF_BODY (+ CInventoryItemDef::StatGetTypeName opt-in): defs no longer escape | DIFFER 2019v2026, 290/622 exact, structural 562/622 |
| 7 | container arm `if (!give_item) {auto} else {give}` (auto block laid out after the loop as retail); object path `added` flag (one shared def release) | DIFFER 2028v2026, 572/622 exact, 596 regs+frame masked, structural 614/622 |
| 8 | no early returns: `collected` flag + switch breaks, play/kill under `if (collected)` (sound weight 12 -> 7: container slot before sound) | DIFFER 2028v2026, 577/622 exact, 596 masked |
| 9 | rent score read at the call `(float)stats->GetRentMoney()` after GetTC(&scoreboard) (no named local: c2 CSEs the sum, no copy; also fixed stats/gold colouring and the tail rotation) | **RELOCATION_MATCH 2026v2026** |
| 10 | comment header, static_casts, kill-arg local renamed (no code change) | RELOCATION_MATCH |

Pass 2: grade 11 = gall.sh verification after the final comment edit: RELOCATION_MATCH. Pass 3 unused.

## Measurements behind the key steps
- Inline trace (grade 3 source): HasTC<CTCActionUseActivateQuest> plain inline c1 175, f 0.1875, score 283 -> called; forced -> expanded.
- Frame packer (pk.sh, grade 7): node weights this 20, sound 12, container 7: sound's slot before the container's. Sound = ctor +
  4 operator= + PlayAttachedCriteria + dtor at the end + one dtor per `return` (5). With no returns (grade 8) sound 7 = container 7,
  container reaches 7 first -> retail order.
- c2prio (grade 8): gold-path stats prio 9 > gold prio 5 (colouring order -> stats edi, gold ebx; retail the reverse). Forced
  what-if uid 53 (gold) = 12 fixed it alone (585/622 exact); the rest was the rent copy + one-step scratch rotation shift in the
  tail. Reading GetRentMoney() at the call (no named local) removed the extra allocator candidate and fixed all three (grade 9).
