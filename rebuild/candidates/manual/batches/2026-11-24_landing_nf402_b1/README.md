# Landing nf402 b1: five StatGetClassTypeName rows respelt, with retail's string literal

Each row is its own unit:

| Address | Class |
|---|---|
| 0x004D2A74 | CTCTargetingAI |
| 0x004D39CF | CTCDCameraPoint |
| 0x004D3CDE | CTCObjectAugmentations |
| 0x004D4232 | CTCDHeroSuit |
| 0x004D5CE6 | CTCDExplosion |

Every row takes NF388's literal fix and is respelt from a stand-in to `?StatGetClassTypeName@<Class>@@SI?AVCCharString@@XZ`. Sources for the names:
- FableWin.pdb (tc_register.obj).
- The retail string each body pushes, which is the class name. For example, 0x01239FB0 = "CTCTargetingAI".
- For four of the rows, the landed binder CTCBase::RegisterTCs 0x004EE23F already binds exactly this spelling.
- For 0x004D2A74, the only retail caller is CTCTargetingAI::GetClassTypeName 0x004DDA94.

Tests are rewritten, linked against the row object, and assert the retail string. Each test fails when its row is rebuilt with the HEAD string, so no test debt remains.

Table: deletes the made-up `?Ctor@CCharString@@QAEXPBDH@Z -> 0099ebf0`, whose only user was the old 0x004D4232 text.

Coordinator checks:
- 5/5 landed, all RELOCATION_MATCH PASS, 0 LITERAL_DIFF.
- self_symbols respelt, column 4 aligned.
- No other user of the deleted spelling (reloc_audit.tsv is a generated snapshot).
- Baselines OK.

Held for a ruling: 0x00C48860 and its 3 funclets, a libpk mailbox whose payload type and method name are undecided (T3 nf402 0x00C48860).

Queued:
- The CTCVillage sibling 0x004D31C0.
- The four GetClassTypeName callers.
- A verifier false positive: SHARED_HEADER_AVAILABLE on layout-rename rows.
