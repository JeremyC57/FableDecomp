# Landing 2026-11-06o: chunk 814 (2 of 6 rows landed, 4 parked)

One expert agent authored the rows, sharing the lane with the chunk 813 and 815 agents. The six rows were unnamed gap-scan entity
Mains of 7.0-8.6 KB. The agent proved every name from vtable slot 1 (stored by a landed Alloc) and both PDB name sets. The landed rows
are plain C++ and grade `genuine`.

| Address | Name | Grade |
|---|---|---|
| 0x00EA8F40 | NScript::CV_MurderTwistScript::CMurderTwistLeader::Main | RELOCATION_MATCH PASS (first attempt; defines, includes and declarations taken verbatim from the landed CMurderTwistBrotherYounger::Main 0x00EADBB0) |
| 0x00E44EA0 | NScript::CV_BordelloScript::CBordelloClient::Main | RELOCATION_MATCH PASS (2 attempts; modelled on the landed CBordelloGuard::Main; `__forceinline CTimer::operator=` for retail's single expanded `timer = 120` site) |

- **The MovieBase dtor (chunks 804, 810, 812, 813).** Both landed rows keep the implicit dtor and add a declared-only
  `template<> CCountedPointer<CScriptGameResourceObjectMovieBase>::~CCountedPointer();`. That reproduces retail's mix of expanded
  and called sites from the inline budget alone: one shared exit pad in MurderTwistLeader, two sites in BordelloClient.
  - Rows where retail expands every site are still open. Parked 0x00CF68C0 is the example. There, retail shares one frame slot
    between the two talk-block movies and merges the exit pads, giving 6 dtor sites. Our build gives 14 sites, and VC7.1's inline
    budget turns 11 of them into calls.
- **Headers:** none copied into the repo, because neither landed row needs an overlay. The overlay stays in
  `pending/2026-11-06_linux_hand814/hdr/`. It holds:
  - three repo headers, each with only an additive opt-in: CV_ChapelOfEvilScript `_ACOLYTEEVIL`, CV_AmbushScamScript `_BUDDY`,
    CQ_ArenaHoldingScriptScript `_ENTRANCEGUARD`;
  - a new engine/CQ_GuildTrainingWoodsDepartureScript.h, built on chunk 810's pending draft. Whichever of 810 and 814 lands its
    draft second must merge with the other, not overwrite it.
- **vland:** one warning, RELOC_NAME on 0x0099E4C0, whose catalog label `CCharString::operator_char_const*` is a placeholder (queued
  below). No new bindings.
- **Names:**
  - Labels: `ghidra_out/labels_landing_2026-11-06o.tsv` holds the six Mains.
  - Three catalog renames are queued under tag `land0611o`:
    - 0x00CF0C00 `sub_00cf0c00` is `CQ_ArenaHoldingScriptScript::CEntranceGuard::GetParentScript` (FableWin.pdb; vtable 0x012C6158 slot 3).
    - 0x0099E4C0 is `CCharString::operator const char*` (`??BCCharString@@QBEPBDXZ`, already bound under that spelling).
    - 0x00704430 `_global::CCountedPointer_CopyCtor_00704430` is `CCountedPointer<CScriptThing>`'s copy ctor (FableWin.pdb). This one
      is queued as a CHECK: confirm no other instantiation shares the 25-byte body before rebinding.
- **Landing checks:**
  - `vland --land` ran (`logs/land.log`) and the labels are applied.
  - The manifest sync dry run prints 0.
  - bindings_multi_check prints only the known NullCoord case.

## Parked (`pending/2026-11-06_linux_hand814/`)
| Row | Grade | Residue |
|---|---|---|
| 0x00D647F0 NScript::CQ_GuildTrainingWoodsDepartureScript::CFinalMaze::Main | DIFFER(7405v7389), 10 attempts, 316 lines | one register choice in the phase-2 lightning branch: retail keeps the Game vtable in ebx and reloads the counter, ours spills the conversation. The register-masked diff is 10 lines, and every call, string and relocation matches |
| 0x00E6BC50 NScript::CV_ChapelOfEvilScript::CCOE_AcolyteEvil::Main | DIFFER(7692v7655), 10 attempts, 102 lines | swapped registers in the follower loop; retail keeps a thread-exit pad that ours merges (GOTCHAS chunk 801); the natural-exit ~seh_me stores come in a different order |
| 0x00E2FF00 NScript::CV_AmbushScamScript::CAS_Buddy::Main | DIFFER(8551v8567), 10 attempts, 387 lines | the CScriptThing copy split: retail calls the copy and the parameter dtor inside each of three forced `CSpawnedFunc1` ctors and expands them elsewhere; ours does the reverse. The callee destroys its by-value parameter, so copy or dtor spelling alone cannot reach it |
| 0x00CF68C0 NScript::CQ_ArenaHoldingScriptScript::CEntranceGuard::Main | DIFFER(8210v8070), 4 attempts, 1299 lines | the all-sites MovieBase dtor case (above); parked early after two new tries: a declared-only CCountedPointer<CScriptThing> copy ctor, and the forced-inline movie ctor |
