# NF435 hand-back (2026-11-25): CGameScriptInterface round 3, TC family pilot

`NOTES.md` has the recipe and every gate; `RESIDUE.tsv` lists the 60 rows left.

## Landed: 11 rows (`batches/2026-11-24_landing_nf435_tc_b1`)
These are script-interface methods that look up a thing's component (TC) and call it.
- **Addresses:** 0x00895090, 0x008950E0, 0x00895430, 0x00895A90, 0x00896210, 0x0089A270, 0x0089A2D0, 0x0089C8A0, 0x0089D750, 0x0089ECC0, 0x008A12A0.
- **What each row is now:** the real `CGameScriptInterface::<Method>`, using retail's GetTC / PeekTC expansion (`FABLE_RETAIL_CTHING_GETTC_EXPANDED`).
- **Header opt-ins:** six, all add-only. Each declares a TC member that is in Ego_r publics and that FableWin's /Od body calls at retail's target: CTCVillage, CTCInventoryStats, CTCCreatureGenerator, CTCSpecialAbilities, CTCFishing, CTCHeroOpinionDeedLog.
- **Grades** are unchanged (RELOCATION_MATCH). Every test passes and every mutant fails.
- **Other gates:**
  - objcmp: SELF_EQ;
  - pk_all: 0 differing copies;
  - /FORCE duplicates stay at 0.
- **Table:** 5 adds, 8 deletes. Three of the deletes are stand-ins at the LowerBound pair.
- **Coordinator checks:**
  - equiv_check copy against copy on the tree at landing time: 0 of 57,160 rows differ;
  - tracker per address: 16 closed, none worse.
- **Released:** NF324's queue lines for these rows.

## Ruling R-NF435-1 (coordinator, 2026-11-25): granted
TC rows may land when their only name_gate failure is the component-map `LowerBound` at 0x0040F020 (and its const twin 0x004365B0).
- Both addresses carry the established `?LowerBound@?$CVectorMap@W4ETCInterfaceType@@...` spelling, already bound.
- They also carry about 30 landed stand-in spellings from older rows.
- naming_status counts the established spelling as std, so the gate never offers DEBT there. No GetTC row could pass.

This follows R-NF430-2 / R-NF432-1. The rows use the bound spelling, add none, and remove three stand-ins. Respelling the remaining stand-in binders is queued (nf435 LowerBound split line).

## Held
- **0x0089EC60** (`hold_0089ec60/`): fully gated. It lands after 0x0074AC80 is renamed from the catalog's `CTCShop::AddShopper` to `CTCCreatureGenerator::AddCreatureTriggerer` (Ego_r + FableWin; the address sits in the CTCCreatureGenerator code). Landing it before would put a second name on that address.
- **Drafts** (`hold_drafts/`):
  - 0x006E72A0 `Validate`: waits on 0x00A36FE0 becoming `CClassFactory::Validate`.
  - 0x0089DFF0: the CTCCombat.h macro sets did not compile, and one hung cl.

## Residue (nf435 T3 family lines)
| Group | Rows | Note |
|---|---:|---|
| TC rows on the same recipe | about 20 | |
| Body ports | 12 | |
| tc_query IsThingA* | 5 | retail keeps only the bit test |
| T1 caller respells | 8 | |

## Name fixes (queued)
- 0x0074AC80 and 0x0074A1E0: the CTCCreatureGenerator triggerer pair.
- 0x00A36FE0: `CClassFactory::Validate`.
- 0x00788388 / 0x00787C0C: CTCTrapBase stand-in selves.
- 0x006D6570: three spellings from different functions.
