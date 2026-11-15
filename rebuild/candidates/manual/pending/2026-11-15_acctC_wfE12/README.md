# acctC workflow hand-back wfE12 (unworked rows round 3, 2026-11-15)

| address | name | bytes | grade | test |
|---|---|---|---|---|
| 0x0121B9B0 | _gapscan::sub_0121b9b0 | 9,567 | RELOCATION_MATCH | PASS |
| 0x01221D20 | _gapscan::sub_01221d20 | 7,990 | RELOCATION_MATCH | PASS |

Verified twice (workflow verifier + coordinator, `coordverify.log`). The verifier's audit of each row follows; review items are for the owner / landing account.

`open/` holds the best drafts of the batch's unmatched rows (full residues in NOTES.md):
- 0x00C4F600: parked, DIFFER(9204v9204). The catalog name PropertySerializer_GetPropertyTypeCode is wrong. The leaf is ?SetEAXProperty@CEAXManager@@QAEJPBU_GUID@@KPAUCEAXVoice@@PAXK@Z: the landed QueueCommit 0x00C51A20 declares and calls it at this address. The extent is 9204 bytes: code ends at 0x00
- 0x0121F86F: byte-matches but broke a lane rule; in the fix-up workflow, not staged yet.

## 0x0121B9B0
Verifier audit: none. No __asm/naked/_emit/volatile, no flags or pragmas, no inline control, no dead code, no ^class/^struct lines (strict-header check clean). Header changes are additive opt-ins only: FABLE_RETAIL_CWORLD_EVENTDISPATCH (declarations only), FABLE_RETAIL_CMAINGAMECOMPONENT_EVENTDISPATCH, plus a new engine/CGameEventDispatch.h (Ego_r layout, 0x40 asserted). The FABLE_STATIC_ASSERT -> FABLE_STATIC_ASSERT_AT changes in hdrdiff are the overlay's mechanical line-pinning (macro exists in rebuild_abi.h); CWorld.h and fable_containers.h also carry other agents' pending opt-ins (C29/C36). Warnings to report: RELOC_NAME ?EAPlayerRespawn@CWorld -> 0x006294A0, which is labelled 'CRenderManager2D::AddLineToDrawBuffer' (the author's NAME_FIXES.tsv corrects it, citing event-enum slot 8 + Ego_r/FableWin), and a leaf warning (no symbol named for the leaf; the largest function was graded, the same as the landed 0x0121AF20 precedent). The test is real: it calls every slot through the table on a CWorld model against an independent expected-code list and checks the {0,0,0,-1} null words. It uses a standalone model, and its table is textually identical to the source.

Fitted items: none (NOTES: 'All EVIDENCED (no FITTED spelling)'). The data-member spelling ?EventDispatch@CMainGameComponent@@1QBVCGameEventDispatch@@B (access/const) comes from the Anniversary PDB only, which the coordinator should note.

## 0x01221D20
Verifier audit: RELOC_CONFLICT in vland.log: '_$E2 -> 0122a460 here, 01228930 in landed 01219f00'. By the verification rule this makes the row unverified. It is very likely a false positive: _$E2 is a per-TU compiler-local atexit-thunk name, so two TUs legitimately bind it to different addresses, and the author says the same ('per-TU static'). The coordinator should adjudicate. The source has no violations: no __asm/volatile/flags/pragmas/inline control/dead code, and no ^class/^struct lines. Header changes are additive: FABLE_RETAIL_CAISTATEGROUP_OPINIONREACTIONROOT_BEHAVIOURS (83 private handler declarations + the static table, no layout change) and a new engine/COpinionReactionBehaviour.h (inline 3-argument constructor per the FableWin signature, inline in retail, 0x28 asserted). Warnings to report: RELOC_NAME CommentToSelfIsRunnable -> 0x00984B40 labelled 'CCombatAction_GenericStrikeResponseBase::IsOverrideableByActionOfSamePriority'; FollowCloseUpdate -> 0x00985420 and FollowFarUpdate -> 0x00985430 labelled '_unattributed::CallWith2_*'; _$E2 -> 0x0122A460 labelled 'CFontBankStateBlock::CreateCRC' (NAME_FIXES covers 0x00985430 and 0x0122A460); plus the leaf warning. The test is real (all 79 names, each pointer calls the expected handler code, null words {0,0,0,-1}, sizeof 40). Its table matches the source apart from NULL vs 0.

Fitted items: none (NOTES: 'All EVIDENCED'). The shared wife is-runnable addresses (WifeClothingIsRunnable 0x00985E90, WifeNotWhileJustMarriedIsRunnable 0x00985EF0) are resolved by Ego_r donor order and size, an inference rather than a direct symbol. The member spelling ?Behaviours@...@@0QBVCOpinionReactionBehaviour@@B comes from the Anniversary PDB only.

## Coordinator note
0x01221D20: RELOC_CONFLICT on `_$E2` is a per-TU compiler-local name (two TUs bind it to different addresses): benign.
These rows are compiler-generated dynamic initialisers; names are descriptive (`dynamic initializer for '...'`).
