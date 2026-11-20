# Landing nf185 (Account A, 2026-11-20): STL / counted pointer / CCharString template instances (14 rows)

Agent NF185 (round 10) took the template-instance entries. It handed back early (context) with 49 entries: 2 done, 9 done-already,
5 blocked or partial, and 33 open (not reached; each has its HEAD census). Its files are in `naming/2026-11-20_agentNF185/`. `work/`
holds the HEAD compile census of every referencing row (`probe.tsv`, `cmp.tsv`) and q4451's list of 85 binders. The open and partial
entries go back to the queue unclaimed, with their census pointers.

- 14 rows, all RELOCATION_MATCH, every test PASS.
  - q4389: ten CCombatManager rows (0x006E9450 0x006E97A0 0x006E9EC0 0x006EA450 0x006EA540 0x006EB9F0 0x006E9680 0x006E9D10 0x006E9D90
    0x006EA2C0) move from the invented `CRetailRbMap_UID_CombatantInfo` onto the real STLport `std::map<unsigned __int64,
    CCombatantInfo>`.
  - q4385: 0x006E76E0 is a self rename from a stand-in free function to `?SetTimer@CGameScriptInterface@@UAEXJJ@Z` (both PDBs; only
    the vtable references it), on engine/CGameScriptInterface.h.
  - q4388: 0x006E5800 / 0x006E5950 replace the invented CConversationMap / CScriptConversationCalls with std::map<long,
    CScriptConversation> and the conversation calls of engine/CScriptConversation.h.
  - q4457: 0x008D9530's != now calls CCharString::operator!= from fable_string.h (FABLE_RETAIL_CCHARSTRING) instead of a stand-in
    member.
- Headers (add-only, `orig` equals HEAD, landed includers define neither macro):
  - engine/CGameScriptInterface.h: FABLE_RETAIL_CGAMESCRIPTINTERFACE_SCRIPTTIMERS_MAP types ScriptTimers (+0x3C) as the PDB's
    std::map<long,long> under the macro: the same 0xC bytes, and the default path is token-identical. It retypes no member on the
    default path, so it needs no header claim (coordinator call).
  - engine/CScriptConversation.h: FABLE_RETAIL_CSCRIPTCONVERSATION_ADDTOCONVERSATION, declarations only. pin_asserts pinned its 10
    asserts at their own lines.
- Pick-any (`logs/pickany_check.txt`):
  - less<unsigned __int64> equals the owner 0x00526120 (37 = 37 bytes).
  - The MISSING lines are the folded `_Rb_tree_base_iterator::operator!=` 0x0041C830, which has no Leaf line (MATCH in nf177 / nf180).
  - The other new COMDATs are the shared header's map / tree / iterator inlines (find, end, `operator*` / `->`). By hand: no landed
    row's Leaf names any of them, and no changed row binds any of them at a retail address (gate pairs). The agent's census also
    compared `_M_find<unsigned __int64>` with its owner 0x006F0250 (84 bytes, equal).
- Coordinator calls on NF185's questions:
  - 1: the pick-any rule stands for copies that differ only by the TU's optimisation regime. 0x006D0630 and 0x005D66BA stay out, and
    q4385 / q4386 return with a pointer to nf184's size-pragma route, usable where the owner row sets the pragma itself.
  - 2: 0x005D8548's identity (map::find or _Rb_tree::find) is queued.
  - 4: q4478 is accepted as done-already. 0x00918640's `vector<CThingGameObject*>::push_back` is the nf104 folded identity (PDB return
    type, both name sets).
- Link (`logs/link_landing.log`): 2 LNK2005 against 4 at HEAD (`logs/link_control_head.log`). The two
  `?find@CRetailRbMap_UID_CombatantInfo@@` clashes are gone, and no new one appears.
- Recheck (`logs/recheck.log`): rebind multi-check 1 (baseline); split gate 0 created. Includers and referencing rows 1741 good, 0 bad
  (engine/CGameScriptInterface.h has 1,721 includers); owner recheck good.
- tablecheck (`logs/tablecheck.txt`): four addresses shrink (0x006E6990 2 -> 1, 0x005937E1 3 -> 2, 0x0099E900 2 -> 1, 0x006E5790
  2 -> 1), none grows. No out-of-image oracle line added.
- Queued: the CScriptConversationManager self signatures (CThing& per both PDBs) with callers 0x00890710 / 0x00890750, and the stale
  0x0099EFB0 witness lines.
