# 2026-11-17 landing nf88: name-fix round 7, NF88 (21 rows: the nf86 follow-ups and one definition for three shared bodies)

Owner: "Continue the naming fix queue with two agents until complete", "Keep going until queue is done", then "Decrease to 2 agents
at a time once next agent finishes".

NF88 took the 13 entries queued at the nf86 landing (q11150..q11162), among them the coordinator's one-definition ruling. Brief, list and
records: `naming/2026-11-17_agentNF88/`.

## Rows (STATUS.tsv, 36 = 35 locked + 0x004DD1DB, claimed through the lock): 21 ported, 13 not-a-binder, 2 blocked
- **One definition (q11162).** 0x00686F10 `CTCBase::AllocateTC`, 0x008C1600 `GetValidRushPosition` and 0x0081F910 `CMap::GetStrongestEngineThemeAt`
  no longer define their header bodies as strong copies through `_DEFINE` macros (the `_DEFINE` branches stay in the headers, unused). Each row
  takes the inline body's address in an anonymous-namespace helper marked `// Fidelity: scaffolding, not retail`, so VC7.1 emits it as a pick-any
  COMDAT like every includer (COMDAT selection 1 before, 2 now). For the two protected members the address is taken inside an anonymous derived
  class that is never instantiated (coordinator accepted this variant). Link proof with `LINK=` cleared (`naming/2026-11-17_agentNF88/link/`): the
  old owner object plus an includer (0x00686FA0, 0x008C2550, 0x0081FA20) gave LNK2005 + LNK1169; the new owner plus the same includer links.
- **NInventory chain (q11153..q11159).** The PASS22 CCategory / NInventory_CTCInventoryBase.h chain only needed `FABLE_RETAIL_DEF_CLASS_BASE` and
  `ALL_ITEMS_CHAIN_PASS22` in the default branch. Ported: CCategory GetBoughtPrice / GetPerItemDataForItem / GetAmountOfItemsContainable, the
  CTCInventoryBase wrappers 0x005BEAD5 / 0x005BF30D (were catalogued `GetItemDescript...`) and 0x005BE8D3 GetPerItemDataForItem (was catalogued
  `GetBoughtPrice`), `find_if` 0x005C08C0 (was catalogued as a `vector<EWeaponClass>` member; test rewritten, PassPattern kept), `__find_if`
  0x005C0653, CTCInventoryItem::GetBoughtPrice and the binders 0x005BDF08 / 0x005BF2ED.
- **Others:** the EObjectAugmentationType / EHeroExperienceStatCategory tree `_M_erase` / `clear` / `~_Rb_tree` (0x004DB853 was catalogued
  `CTCCarryable::OnKill`), 0x006DC450 / 0x006DD1C0 `GFSerialiseMapBinaryOut` / `In` (were catalogued as CHeroExperienceDef's WriteToStream /
  ReadFromStream), 0x0091D190 Update on the virtual `UAE`, and 0x00664670 `CThingCreatureBase::SetCurrentAction(const CCountedPointer<
  CCreatureActionBase>&)` (was catalogued `std::_Cons_val<...>`).
- **Blocked:** 0x008E74F0 (retail expands CModeBase::OnDeactivate inline, whose body is the separately landed strong row 0x00739E60: queued as
  another one-definition case) and 0x004C9D60 `CThing::AddTC` (a 504-byte all-local model with ten stand-in bindings).

## Entries (APPLIED.tsv, 13): 10 done, 1 done-already (q11150), 2 blocked (q11161 binder 0x008E74F0; q11162 binder 0x004C9D60)
- The Ghidra label of 0x01436E9C (q10973 / q11098 / q11150) is applied in this commit: `ghidra_out/labels_chunks718_725_2026-11-03.tsv` now names it
  `NEngineInternal::GPLightingManager` (one PDB source, Ego_d.pdb; the old label `g_pEngineLightingManager` had none).

## Rows and checks
- **Section 1: 16 self renames (26 file moves).** Modules match their siblings' catalog spellings.
- **Headers (4 opt-ins plus 1 nested body macro):** engine/CPersistTraits.h (`_HEROSTATINCREASEDATA_BINARYOUT`), engine/CCategory.h (`_CCATEGORY_ITEMQUERIES`,
  1 assert pinned), stlport/fable_algorithm.h (`FABLE_RETAIL_STLP_FIND_IF_RANDOM_ACCESS` + `_BODY`), engine/CTCInventoryItem.h (`_GETBOUGHTPRICE`).
  Token-identical without the opt-ins (`tokid.log`); `lane_merge/assert_names_check.py`: 0 mismatches against 49b943df65.
- **Funclets:** none affected (1,355 checked, 0 broken).
- **Recheck (`logs/recheck.log`):** all 21 rows graded from the repo after landing: 19 RELOCATION_MATCH and 2 MATCH, every test PASS.
- **Checks:** `naming/rebind.py` removed 33 binding lines and added 20; bindings_multi_check is at its baseline (`NullCoord`).

## Name fixes queued (`land1117fu`, 13)
NF88's NAME_FIXES.tsv (12), plus the CModeBase::OnDeactivate one-definition case (0x00739E60 / 0x008E74F0).

## Rule slips
- Two rows were blocked on a probe alone; one grade went on a mistyped Leaf line (0x006DC450); two compiles failed on wrong defines or missing
  headers; 0x008C1600's first scaffolding form put the helper inside the class, so it was never compiled (OBJDUMP_ERR).
