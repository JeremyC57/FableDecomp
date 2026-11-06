# Landing 2026-11-05o: chunk 788 (6 of 6 quest / village script Mains)

One expert agent authored the rows. All six are plain C++ and grade `genuine`, and four matched on the first grade.

| Address | Name | Grade |
|---|---|---|
| 0x00D8DD10 | NScript::CQ_HobbeCaveScript::Main | RELOCATION_MATCH PASS |
| 0x00D84D60 | NScript::CQ_HeroSoulsGuildmasterScript::Main | RELOCATION_MATCH PASS (2 grades) |
| 0x00D7C890 | NScript::CQ_HeroSoulsArenaScript::Main | RELOCATION_MATCH PASS |
| 0x00F024C0 | NScript::CQ_AmbushTradersScript::Main | RELOCATION_MATCH PASS |
| 0x00E693F0 | NScript::CV_ChapelOfEvilScript::Main | RELOCATION_MATCH PASS (2 grades) |
| 0x00EDB190 | NScript::CV_TempleOfLightScript::Main | RELOCATION_MATCH PASS |

- **Chapel class proof.** 0x00E693F0 is slot 2 of vtable 0x012E8080. Slot 1 is the landed RegisterMain 0x00E69030 and slot 3 the
  landed Init 0x00E68FA0, and the Alloc 0x00E6E230 stores that vtable.
- **Names.**
  - Labels: `ghidra_out/labels_landing_2026-11-05o.tsv`, the six rows plus six unlanded thread functions.
  - 10 queue lines (tag `land0511o`), among them:
    - 0x00EDC450, landed as CV_ChapelOfEvilScript::DoMission (the same symbol as 0x00E6A7F0), which is really
      CV_TempleOfLightScript::DoMission;
    - two truncated WatchForQuestInfoDisplayConditions names;
    - six Chapel / Hobbe thread functions landed without NScript::.
- **Headers** (`hdr_README.md`).
  - New headers with no landed includers: CQ_HobbeCaveScript.h, CQ_HeroSoulsGuildmasterScript.h, CV_ChapelOfEvilScript.h,
    CV_TempleOfLightScript.h.
  - Opt-ins: CQ_AmbushTradersScript.h (FABLE_RETAIL_CQ_AMBUSHTRADERSSCRIPT_MAIN) and fable_string.h (FABLE_RETAIL_CCHARSTRING_CONTAINS,
    a __forceinline CCharString::Contains over strstr).
  - hdrcheck2 reads `default=same`, and the agent rechecked 8 landed includers. The agent's unchanged copy of
    fable_definition_table.h, needed only for overlay builds, was not copied in.
- **Landing:** `vland --land` (`logs/land.log`).
