# NF471 hand-back (2026-11-26): the CAIStateGroupBase destructor chain lands; 11 full-size vftables

`README_agent.md` is the agent's own report. `T3_RESOLVED.tsv` covers T3 line 651's six binders.

## Landed (one commit, after equiv_check)
- **b0_hdr: 6 add-only headers.**
  - NF469's StandStill / MinionKidCalledToMother / MinionWander.
  - `_MEMBERS_ONLY` views for JackDragon and RangedCombatUsePreferredFirePoint, used instead of the r470d guard. They keep `fable_containers.h`, and with it libcp, out of the owner objects (r471c).
  - The WanderAroundShops retail-class opt-in on `CAIStateGroup_WorkBase<CAIStateGroupBase>` (r470e).
- **bR1_008fd288_unit: 38 rows, landed with `REL=nf303`.** T3 line 651 is closed: all six table binders are real implicit `~CAIStateGroup_<X>` thin rows.
  - 0x008FD288 and the 6 binders are pick-any thin rows, EQ to retail.
  - 6 callee-respelt `??_G` owners.
  - 4 caller respells.
  - 21 local models declare the destructor only (r469a), which removes 21 non-retail copies.
- **b4_regate: 22 rows.** 11 CAIStateGroup ctors now emit full vftables, 4 → 56 bytes, equal to retail. Each lands with its `??_G` home as a thin row.
  - 18 rows (9 units) are held on r467b: their slot targets are landed under misspelt names. NF472 is on them.
- **Catalog:** 12 relabels (JackDragon's dtor, and the 11 `??_G` homes); 29 SELF_SYMBOLS lines.
- **Gates (agent):**
  - vland 38 / 38 and 22 / 22 RELOCATION_MATCH PASS;
  - full-chain link: duplicates 114 → 12, the 12 being pre-existing local-model `??0` clashes; no duplicate of `??1CAIStateGroupBase` or of any binder name;
  - census 0 BAD.
- **Coordinator, after landing:**
  - bindings_multi 1; funclets 1,722, 0 broken; descriptive_check 2,030, 0 FAIL.
  - s_unit_check: 177 units, 0 failed, after the r470b tool fix (a qualified destructor call `this->X::~X()` was read as a definition).
- **Tracker:** +17 to agree_pdb (11 rtti_identified, 5 split_unknown, 1 split_fixable), nothing worse.

## Coordinator table adds (accuracy)
- NF471's census found 4 more landed rows that reach 0x008FD288 under stand-in names with no table line: 0x008FDA24, 0x008FECEC, 0x00900533, 0x00913490 (`untabled_008fd288_binders.tsv`).
- They were added to the table (`TABLE_ADDS_coordinator.tsv`). So 0x008FD288 reads split_fixable T1, not proven, until NF472 respells them.
- They are outside line 651's subject, which named the six table binders, so `REL=nf303` was right.

## Rulings (coordinator, on precedent)
- **r471a: granted.** 0x009000BC keeps the declaration-only form until its own 14-slot port.
- **r471b: granted.** Ownerless copies that were already NEQ change bytes but stay NEQ (the bR0 0x00902FB9 precedent).
- **r471c: granted.** `_MEMBERS_ONLY` views are add-only and better than a guard. The JackDragon full view's wrong `virtual ~` declaration is queued.
- **r471d: granted.** The pick-any `~CAIStateGroup_WorkBase<CAIStateGroupBase>` copy is EQ to retail 0x008FDA24. That row's own respell is NF472 step 1.
- **r470b: fixed in the tool.** `s_unit_check.spelled()` no longer reads a name after `.` / `->` as a definition.
