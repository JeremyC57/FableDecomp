# acctC workflow hand-back wfE11 (unworked rows round 3, 2026-11-15)

| address | name | bytes | grade | test |
|---|---|---|---|---|
| 0x004736C4 | CPlayerGuiDef::?Transfer@CPlayerGuiDef | 6,498 | RELOCATION_MATCH | PASS |
| 0x004804DE | CTCInGameMenu::OpenInGameMenuPC | 11,203 | RELOCATION_MATCH | PASS |

Verified twice (workflow verifier + coordinator, `coordverify.log`). The verifier's audit of each row follows; review items are for the owner / landing account.

`open/` holds the best drafts of the batch's unmatched rows (full residues in NOTES.md):
- 0x004A3740: parked, SRC_FAIL(draft). Third session: read the whole function and wrote a full draft of all 9 cases from the disassembly (differ/004a3740.cpp, 653 lines), using the original local names from the Anniversary 4k PDB. It does not compile yet: about 45 callee/member declarations still n
- 0x0044C72B: byte-matches but broke a lane rule; in the fix-up workflow, not staged yet.
- 0x006F5C10: parked, DIFFER(9096v8717). Parked by the first agent after ~25 grades. Residue is a reload / alias difference in blocks 1-19 (frame 0x30 v 0x2C). Name proven: the catalog's _global label is wrong. No new measured lead, so it was not reopened this session. Best source: differ/006f5c10.cp

## 0x004736C4
Verifier audit: none. Grep of src/004736c4.cpp found no __asm, naked, _emit or volatile, no /d2inl or inline_depth, no if(0) dead calls, and no local class or struct (it uses engine/CPlayerGuiDef.h and engine/CPersistContext.h under existing opt-ins). TU pragmas optimize("y", off) and ("s", on) come with stated evidence (CSoundThemeDef::Transfer 0x0046329E twin; map defaults in the dead context slot). No header change for this row. Warnings to report: 5x RELOC_NAME, because the TransferVector<long/C2DVector/float/CRGBColour/CCharString> callees 0x00466A47/0x00475D99/0x00464CBF/0x00468EB4/0x004331F9 are landed as TransferBinary_* or as the wrong label 'CCreatureAction_FireMissileWeapon::FrameUpdate'; the author handed all 5 back in NAME_FIXES.tsv. 1x STANDIN_LEAF 'names another function'. This looks like a false positive: the catalog name is the raw decorated string 'CPlayerGuiDef::?Transfer@CPlayerGuiDef@@...', so the method-name compare fails, while the leaf ?Transfer@CPlayerGuiDef@@UAEXAAVCPersistContext@@@Z is the correct PDB name. The manifest name could be cleaned up. Test: a model of the body (member offset and helper kind for each statement) is compared against the retail call-sequence table, and a one-swap mutant FAILs. It is a real order/kind check but a model, not the compiled row.

Fitted items: none marked FITTED

## 0x004804DE
Verifier audit: none. Grep of src/004804de.cpp found no __asm, naked, _emit or volatile, no /d2inl or inline_depth (only `#pragma optimize("s", on)`, evidenced by the same-TU landed twin CTCInGameMenu::OpenInGameMenu 0x004830A1), and no if(0). One local class has a base clause, `class CTCFishing : public CTCBase`. No rebuild/include/engine/CTCFishing.h exists in the repo or the overlay, so it is allowed; it is copied from the landed twin with Ego_r layout. Its inline IsActive() / StatGetInterfaceType() are retail-inline (FableWin ?IsActive@CTCFishing, `cmp [eax+14h],0 ; setne`), not stand-ins for called functions. NOTES says no header change for this row; the overlay diffs in hdrdiff (C2DBoxF, C3DVector, CPlayer, CQuestManager, CVectorMap, NUISystem_CComponent, NUISystem_CUIScreenUnitConverter, fable_containers) are other lanes' additive FABLE_RETAIL_* opt-ins with pinned asserts. Warnings to report: 5x RELOC_NAME (IsDebugProfile -> 00407030 'ReturnFalse_ret0_00407030'; two std::operator== overloads; CCountedPointer<CText>/<CList>::ReleaseData labelled ~CCountedPointer) and 1x RELOC_ALIAS (_wcscmp -> import thunk MSVCR71.DLL::wcscmp). Test: a model of the save/load list control flow over a mock profile/quest/fishing state with about 20 concrete checks (offsets, entry kinds, sub-list action sets, frames, gating, dialog centring). It is real but a model.

Fitted items: FITTED (source header + NOTES): (1) `C2DVector position(...)` as a named function-scope local (retail's own frame slot [ebp-254h]); (2) the parenthesised `(GetBackBufferWidth() * 0.5f)` (PAREN tuple puts fsubp before lea ecx,[ebp-254h] at 0x00482FDE)
