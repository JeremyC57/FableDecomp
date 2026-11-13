# 2026-11-13 landing ft -- 0x008C12D0 CCombatAction_StrikeResponseJackOfBladesIntoRush::Initialise (wf20, after repair)

Source: `pending/2026-11-12_linux_handWF20/`. The row matched in workflow wf20 with two verifier findings. An Opus repair
(`lane_tools/workflows/repair_and_verify_row.js`) fixed both without moving a byte: `GetTC(TCI_BOSS, &boss)` replaces a raw cast,
and two new test cases (a dead hitter behind the creature with the point of hit in front, and the same geometry with a living
hitter) make the IS_THING_ALIVE branch testable; the mutation that used to pass now fails.
Both verifiers confirmed bytes, test, purity and the landing-verifier dry run (no jump-table or identity line). Their one finding:
the CTCBoss.h overlay had gone stale against a landing made while the repair ran. Rebased at landing by 3-way merge: it keeps the
SETPHASESTARTPOS block of 0x00972990 and adds only GETGENERICCOUNTER; 0x00972990 rechecked (RELOCATION_MATCH, PASS).
RELOCATION_MATCH, test PASS, LANDVERIFY_STRICT_HEADERS=1. Headers (additions under opt-ins with no other user; defaults
unchanged): CTCBoss.h, CCombatAction_StrikeResponseJackOfBladesIntoRush.h, CCombatAction_GenericStrikeResponseBase.h.
Checks: bindings_multi_check at baseline; funclet_includes_check 1317 checked, 0 broken.
