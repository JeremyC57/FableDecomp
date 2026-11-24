// Leaf: ?IsStillRunnable@CAIStateGroup_JackDragon@@AAE_NXZ
// CAIStateGroup_JackDragon::IsStillRunnable() -- the dragon fight continues while the enemy lives and nothing stops the group.
// Naming (agentNF31, 2026-11-12): access only, public -> private: Ego_r.pdb egor_signatures 0x009747B0 + FableWin.pdb
//   _fablewin_api 0x017C1E60 / 0x02BD9900 ?IsStillRunnable@CAIStateGroup_JackDragon@@AAE_NXZ; engine/CAIStateGroup_JackDragon.h
//   declares it private and the landed caller IntoHover 0x00951D60 binds this spelling.
// Retail: Fable.exe (TLC, VC7.1) function @ 0x0094FDF0, length 55 bytes.
// Option S unit (owner ruling, advisor decision 2026-11-24; unit NF379): this function's one body lives in its retail TU-mate's row
//   0x0094FBB0 (Ego_r.pdb / FableWin.pdb ai_state_group_dragon.obj), included below with no code of its own (the EH-funclet / 0x00B2AB10
//   shape); that object's ?IsStillRunnable copy grades against this address. Unit flags /d2inlT59 (UpdateHover's). Before the unit this
//   row's body was a second strong definition beside 0x00951FB0's same-TU copy (and 0x00951070 / 0x00951430 / 0x00952890 still define
//   it strongly: pre-existing debt, the same TU).
#include "../src/compiled/00/94/CAIStateGroupJackDragon_IsTargetOnATower_0094fbb0.cpp"
