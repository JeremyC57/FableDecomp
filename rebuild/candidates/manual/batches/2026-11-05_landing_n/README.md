# Landing 2026-11-05n: chunk 787 (6 of 6 quest-level script bodies)

One expert agent authored the rows. All six are plain C++ and grade `genuine`, and five matched on the first grade.

| Address | Name | Grade |
|---|---|---|
| 0x00CF9A50 | NScript::CQ_AmbushTradersScript::Init | RELOCATION_MATCH PASS |
| 0x00CFB080 | NScript::CQ_AwakeningTheOracleScript::Main | RELOCATION_MATCH PASS |
| 0x00D038E0 | NScript::CQ_BanditCampBossBattleScript::Main | RELOCATION_MATCH PASS |
| 0x00DA34B0 | NScript::CQ_MinionClifftopChaseScript::Main | RELOCATION_MATCH PASS |
| 0x00DE0850 | NScript::CQ_RansomVictimScript::Main | RELOCATION_MATCH PASS |
| 0x00DFA450 | NScript::CQ_TraderConflictGoodScript::Main | RELOCATION_MATCH PASS (2 grades) |

- **Method.** Each row starts from the landed quest-Main pattern (CQ_PrisonEscapeRescueMotherScript::Main, chunk 786) and adds a
  per-quest header. Member offsets in that header are the Ego_r.pdb offset minus 0x14, then a further minus 4 after each STLport
  vector or map. reloc_identity skips string relocations, so the agent checked the string-literal order by hand.
- **Names.** Every callee is proven by the PDB name set plus its single reference from Main next to its thread-name literal.
  - Labels: `ghidra_out/labels_landing_2026-11-05n.tsv`, the six rows plus seven unlanded thread functions.
  - 10 queue lines (tag `land0511n`), among them:
    - thread functions landed under wrong names: 0x00D04770, 0x00DE1770 (both landed as CV_ChapelOfEvilScript::EndMission),
      0x00DA3F40 and 0x00D043A0;
    - 0x008ADF10 std::map<CCharString,bool>::operator[], which has four invented spellings in landed code;
    - 0x00DFC320's old binder spelling UpdateTraders.
- **Headers** (`hdr_README.md`).
  - CQ_BanditCampBossBattleScript.h: additive declarations. No landed source includes it, and hdrcheck2 reads `default=same`.
  - CQ_RansomVictimScript.h: opt-in FABLE_RETAIL_CQ_RANSOMVICTIMSCRIPT_METHODS; the agent rechecked all 13 landed includers.
  - New headers: CQ_MinionClifftopChaseScript.h, CQ_TraderConflictGoodScript.h, CQ_AwakeningTheOracleScript.h.
- **Landing:** `vland --land` (`logs/land.log`).
