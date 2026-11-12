# 2026-11-12 landing ep -- name-fix pass NF29: 33 queue entries, 69 landed rows changed, 17 renamed

Source: `naming/2026-11-12_agentNF29/` (STATUS, APPLIED, REJECTED, RENAMES, NOTES, LESSONS, QUEUE, hdr README, parked/). Run as the
workflow `lane_tools/workflows/name_fix_queue_pass.js`: one Opus agent applied the queue (tags land1011ek, el, em), an independent
Opus verifier regraded all 69 rows and attacked the hand-back: ok on the first round (no blocker, no fix; notes only).

**Queue:** 24 done, 9 partial, none rejected.
**Rows:** 69 landed rows changed, 17 of them renames of the row's own name (RENAMES_applied.tsv section 1), the rest caller / member
respellings (section 2). All 69 rechecked in this landing: MATCH or RELOCATION_MATCH, every test PASS (`logs/recheck.log`).
Highlights: 21 rows moved from the placeholder global to g_pLandscapeRenderer (41 done in two passes, about 12 left), 11 more rows on
GD3DFormats (20 done, 4 left), the GetState virtual slots 134/135 respelled in nine rows, SetSoundAsMuted(bool) and
GetManualSaveFileName (CWideString) on the shared CUserProfileManager header, 0x009BEBB0 private so its symbol matches its Leaf.
**Headers** (2, declaration-only opt-ins, defaults unchanged): NPlayerGui_CDrawBase.h (SCALEALPHA), CUserProfileManager.h
(SETSOUNDASMUTED).
**Bindings:** rebind.py removed 81 and added 77 rows; bindings_multi_check at baseline; funclet_includes_check 1317 checked, 0 broken.
**Carried over (queue tag land1011ep):** the nine partial entries and the verifier's notes -- 0x00658EC3 still carries two STAND-IN
bodies for IS_THING_ALIVE instances (pre-existing; the real bodies are in engine/ThingAlive.h, as row 0x0064C869 uses them);
0x0052C730 / 0x0052CAB0 bind NUISystem::CComponent::Initialise / AddChild as non-virtual QAE where both PDBs have UAE;
CEngineLandscapeMemoryManager::RebindBlockOwner 0x00BD9AB0 is a made-up name in three manglings (Ego_r suggests
SetVirtualBlockMovableResource); 0x00BFAAE0 binds 0x01436EA8 as g_LocalDetailEngine; the ClearDisplay spellings left in 0x00438230.
The verifier reported that one of its own shell loops briefly wrote scratch files under rebuild/src/compiled and removed them; the
working tree was clean before this landing started.
