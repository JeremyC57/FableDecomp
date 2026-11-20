# Landing nf179 (Account A, 2026-11-20): returned script / TC / player GUI / UI entries (17 rows)

Agent NF179 (round 10) handed back early (context) with 33 entries: 15 landed, 2 done-already, 2 blocked, 14 not reached. Its files
are in `naming/2026-11-20_agentNF179/` (census witnesses under `work/`, the CTeamSpawn link proof under `work/link/`, trimmed of
unresolved-external lines). The 14 entries not reached and q12077 go back to the queue unclaimed.

- 17 rows land (1 MATCH + 16 RELOCATION_MATCH). Self renames (8): CPlayerGui::ChangeGraphicsBankScene 0x00437570 (was the gapscan
  `T::Run`), CTCTavernGameCoinBase::EndRound 0x008EF190 (the extern "C" forwarder stand-in is gone), CPersistTraits<BriarRoseOpinion>::
  TransferBinaryIn 0x00E9E350 (free stand-in -> the FableWin.pdb member specialisation), CMainGameComponent::ForceRender 0x004164E0 (QAE ->
  protected IAE, with its only referencer 0x00417001), CTCMapwho::OnDie 0x0071A7C0 (onto the 0x0071A790 shared-header preamble), and the
  NScript::CQ_OrchardFarmRaidScript::CTeamSpawn family: ctor 0x00DCD160, GetParentScript 0x00DCD1C0, Init 0x00DCD1D0 and the ??_G
  0x00DCF6E0 (NF163's held text, re-authored), with the ??1 0x00DCF700 on the same preamble. Files follow the self renames.
- Ports onto shared headers: 0x0059DB82 / 0x0059E6BD (NUISystem::CDeletion; the `N_0042bed4::CAction` stand-in is gone), 0x008006D0
  (SaveRoute onto the shared STLport vector of pair<C2DVector, unsigned char>). Callee respells in place: 0x00689050 / 0x00689C40
  (IsEventButtonReleased, protected const), 0x007E00D0 (GetWorldMap, protected, CWorldMap&).
- No header changes (hdr/ empty; rows use existing opt-ins). Vtable label 0x012DB2D4 moves to `??_7CTeamSpawn@CQ_OrchardFarmRaidScript@NScript@@6B@`.
- CTeamSpawn one definition: the ctor is now `inline` from engine/CQ_OrchardFarmRaidScript.h, so ??0 / ??_7 / ??_G / ??1 of CTeamSpawn are
  pick-any in all three objects; no row defines the ctor strongly. The agent's link proof: HEAD 0x00DCF700 with the new 0x00DCD160 gives
  LNK2005 + LNK1169, the five new objects with HEAD 0x00DCD350 link clean. This settles nf163's held 0x00DCF6E0.
- Pick-any check (`logs/pickany_check.txt` plus `$S/tools/cmpbody.py` by hand): the CTeamSpawn ??1 copies in 0x00DCD160 / 0x00DCF6E0 equal the
  self row 0x00DCF700's body (94 = 94 bytes, relocations masked); the CScriptThing ctor / dtor copies equal their owners (26 / 66 bytes);
  placement new is 5 bytes in every object (its owner 0x00405C10 carries no Leaf, hence MISSING). The NO-OWNER-OBJ lines are the
  self rows' own symbols and vtables. The agent's census found the 0x008006D0 vector / pair COMDATs SAME against all 14 landed users and
  the CDeletion helpers SAME against 7 sampled users.
- Coordinator calls: 0x00437570 reaches the member through `const_cast<CGraphicDataBank*>(Graphics)` (`// Fidelity: cast, header debt`): Ego_r
  types it non-const and engine/CPlayerGui.h declares it const. Accepted as the least-bad form; the member retype is queued as a header claim.
  q11950 is blocked on a header claim (engine/NUISystem_CList.h declares GPreviousSelected public 2V; the PDB has protected 1V). The seven
  rows that define a non-virtual ?OnDie@CTCMapwho@@QAEXXZ for other addresses, and the free ?CTCMapwho_OnDie stand-ins, are queued as name
  fixes (do not respell those to UAE).
- Recheck (`logs/recheck.log`): 1 MATCH + 16 RELOCATION_MATCH, every test PASS. Split gate 0 created (`logs/split_gate.txt`); includers 108
  good, 0 bad; owner recheck 15 of 15 good.
- tablecheck (`logs/tablecheck.txt`): four addresses shrink. Two grow, both explained:
  - 0x005E9AA0 1 -> 2: 0x008EF190 moved from the stand-in `_ForwardTarget_005e9aa0` (absent from the HEAD table) to the PDB
    `?EndRound@CTCTavernGame@@UAEXXZ`; landed 0x007D7BA0 / 0x008F1720 still bind the QAE stand-in (queued as debt). Real spellings went
    from QAE + stand-in to QAE + UAE.
  - 0x00DCF700 0 -> 1: the ??_G row 0x00DCF6E0 now binds `??1CTeamSpawn@CQ_OrchardFarmRaidScript@NScript@@UAE@XZ`, the self row's own Leaf
    (before: a CLandscapeBackgroundPatch stand-in).
- Link (`logs/link_landing.log`): 3 LNK2005, the same three CScriptThing ones as HEAD (`logs/link_control_head.log`).
