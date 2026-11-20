# Landing nf174 (Account A, 2026-11-20): script / UI / creature mode namespace names (6 rows)

Agent NF174 (round 10) handed back early (context) with 28 entries: 7 done, 6 done-already, 1 blocked, 14 open. Its files are in
`naming/2026-11-20_agentNF174/`. The 14 open entries go back to the queue unclaimed (q11945 / q11946 with their census finished).

- 6 rows. Self renames (2): NScript::CV_ArcheryCompetitionScript ctor 0x00E32350 (onto engine/CV_ArcheryCompetitionScript.h; the vtable
  0x012E2EE4 now binds `??_7CV_ArcheryCompetitionScript@NScript@@6B@`) and NGameText::CDataBankTextEntry::BuildText 0x009C8FB0
  (access-only, private const), with its binder 0x009C9070. Binders respelled: 0x00CB8440 (erase / sort onto their owner rows' exact
  symbols), 0x004175E5 (`m_14.~CCountedPointer(); m_14.Data = 0;` with an `// Inferred:` line; `Reset(0)` would emit a pick-any
  Reset<CDataBank> whose body differs from retail 0x00403E40).
- One definition (q4102): the landed dtor row 0x00E37680 also defined the PDB ctor strongly, which clashes with the renamed 0x00E32350.
  It now declares the ctor only and emits ??1 from an anonymous-namespace qualified-destructor helper (the GOTCHAS nf86 / nf88 form,
  marked `// Fidelity: scaffolding`). The agent's pair link is `logs/link_agent_archery_pair.log`. 0x00E37660 (??_G) is its own row
  and is unaffected. The same HEAD clash in the AmbushScam pair (0x00E2AB60 / 0x00E32310) is queued as DEBT.
- Header: engine/CV_ArcheryCompetitionScript.h (`orig` equals HEAD) gets FABLE_RETAIL_CV_ARCHERYCOMPETITIONSCRIPT_CTOR, declaration
  only; pin_asserts pinned 0.
- No casts of stand-ins to real types, no new local models. 0x00CB8440 keeps the local class tokens it had at HEAD (two
  SHARED_HEADER_AVAILABLE warnings as before); its full port needs a header claim (queued).
- Recheck (`logs/recheck.log`): 6 RELOCATION_MATCH, every test PASS. rebind multi-check 1 (baseline); split gate 0 created; includers
  84 good, 0 bad; owner recheck 11 of 11 good. tablecheck: 0x00403750, 0x00CBC130 and 0x00CBD370 shrink; nothing grows.
- Link (`logs/link_landing.log`): 1 LNK2005 (??0CTimer), the same as HEAD (`logs/link_control_head.log`).
- Blocked: q4639 (0x00EF7800, best DIFFER(628v627); needs the q11224 pattern first).
