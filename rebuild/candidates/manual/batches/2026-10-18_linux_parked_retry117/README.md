# Region following and Screamer attack submission

008A0B10 (166 bytes) preserves both script-liveness checks, then obtains both native things before checking null/dead state and the leader's region-follower component. It dispatches AddFollower or RemoveFollower with the follower by reference. Main audited all3 relocations, actual CTCRegionFollower callees and donor signatures; virtual script slots75/11 match the previously audited79-slot interface. Exact-source fixtures cover every guard, both operations, sentinel fallback, irrelevant life bits, callback-time death and a changed map end pointer.

008A08A0 (168 bytes) obtains the mode-manager component, allocates the132-byte NCreatureMode::CModeScreamerSuperAttack, evaluates the target getter only on successful allocation, and passes a counted mode by value to AddMode. Main audited all5 relocations, constructor RTTI1278338 and PDB reference arguments, counted-info constructor718A00, deleting callback7180B0 and AddMode73ACC0's reference consumption. The target preserves its unchecked downstream behavior when allocation fails: the fixture verifies that it forwards an empty counted handle, without claiming the real AddMode accepts null. The mode constructor and manager are external dependencies; this landing claims byte equivalence of the script function only.

Actual CGameScriptInterface, CScriptThing, quarantined CThing and string headers are included and expanded into exact-source tests. The native thing view documents nine bitset words/map44/life91 versus the donor layout. Host C++98 O2 with nullable-new semantics and with copy elision disabled passes; canonical VC7.1 fixtures pass. Screamer tests cover all short circuits, failed allocation and skipped target evaluation, manager capture across a target callback, retained/nonretained mode ownership, and sentinel map cases.

Both rows used2 new grades including actual-header integration, with4 UNCERTAIN historical slots reserved separately. Root changed only fixture coverage, not target source; earlier variants and counts remain intact.
All independently byte-verified (MATCH or RELOCATION_MATCH) + PASS, with verbatim-source host and canonical VC7.1 fixtures.

Length clarification: the preserved authored source comment says169; the authoritative168-byte span ends at008A0948, immediately after ret8. Byte comparison, oracle and coverage use168.
