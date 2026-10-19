# Retry section 200A: five of five byte-exact

All five rows matched on the first new source grade using /O2 /Oy /W3, with no additional flags. Each retains four UNCERTAIN historical reservations and five unused new grades; no new grades were spent after the match. The previous address-named placeholder files were not used as implementations or counted as proven attempts.

The sources use actual CGameScriptThing, CTCEntityEvents, CQuestManager, CSpawnedFuncBase and quarantined CThing headers. Native projections document CThing Components+0x44 versus donor+0x48 and lifecycle+0x91; quest currently running+0x88 versus donor+0xe4; active script+0x2c. Frame fields use actual CSpawnedFuncBase LastScriptFrame+0x18 and CurrentScriptFrame+0x1c. No generated event headers exist; PDB layouts provide their semantic fields.

All five methods borrow list event pointers without incrementing or releasing their counted-pointer information. Nodes are next+0, previous+4, Data+8, Info+12. Source has no counted ownership callbacks or synthetic PMFs. EventBase's virtual destructor is genuine slot0, and GetType genuine slot1. RTTI tables and disassembly establish:

- CEventExpressionPerformedTo: VT01274240 slot1=0084B070 returns3; DurationInFrames at+0x14.
- CEventReceivedMoney: VT01274210 slot1=0084AE40 returns9; Amount at+0x10.
- CEventReceivedItemInInventory: VT01253C8C slot1=005BC3E3 returns40.
- CEventPerformedSpecialAbility: VT012723C0 slot1=00811EE0 returns26; Ability at+8, Aggressive at+0xc.

These identities correspond to ghidra_out/struct_layouts_egor.tsv CEventBase/CEventExpressionPerformedTo/CEventReceivedMoney/CEventPerformedSpecialAbility. Native disassembly is retained in logs/native_event_types.txt. The common direct calls are intelligent pointer GetPItem00A01B50 and component map LowerBound004365B0; QuestManager global013B89FC. These were independently audited in prior section199H and match the native accesses here. Sources introduce no string or float constants and no own vtable relocation in these leaf functions.

Each scan accepts WorldFrame > LastScriptFrame and <= CurrentScriptFrame. Inventory deliberately checks the frame range before virtual GetType; other methods query type first. MsgPerformedSpecialAbility returns the comparison for the first frame/type match, including false on a differing ability. The aggressive query continues past nonaggressive matches.

Tests execute expanded actual source and actual included headers. ABI assertions remain active under VC7.1 and disabled only for host pointer-width differences. Additional native fixture assertions verify event/node/native-projection offsets and sizes. Host pointers and virtual dispatch retain their full host width. Fixtures exercise absent/dead things, absent components, empty list, wrong types, inclusive current/exclusive last boundary, skipped early nodes, negative frames, null quest/script, output preservation and negative payloads, first-match ability behavior and aggressive continuation. All five pass host C++98 -O2 and VC7.1; full command/stdout/stderr logs are retained.

No parked or skipped rows. No canonical files, headers, documentation, shared tools, or Wine processes were modified.
