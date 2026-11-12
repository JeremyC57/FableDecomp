# agentQ10 header overlay (grade with FABLE_HDR_OVERLAY=<this dir>)

Copies of repo HEAD headers with new opt-ins only; the default (no-macro) path is byte-identical to the repo.

| header | opt-in | for row | what it adds |
|---|---|---|---|
| engine/CTCDivineWrath.h | FABLE_RETAIL_CTCDIVINEWRATH_STOPSOUND_INLINE | 0x0078F8F0 CTCDivineWrath::CancelForCutscene | the body of private StopSound(unsigned long) (0x0078EC80) in class, spelled exactly as CTCUnholyPower::StopSound in engine/CTCUnholyPower.h, so the cutscene cancel expands the interface-16 lookup inline as retail (0x0078FA64). Needs engine/CTCSoundPlayer.h with FABLE_RETAIL_CTCSOUNDPLAYER + FABLE_RETAIL_CTCUNHOLYPOWER_CANCELFORCUTSCENE included first. Default path: the declaration as before. |
| engine/CQ_GuildTrainingScript.h | FABLE_RETAIL_CQ_GUILDTRAININGSCRIPT_KILLBIRD (with FABLE_RETAIL_CQ_GUILDTRAININGSCRIPT_MAIN) | 0x00D43190 CQ_GuildTrainingScript::CKillBird::Main | the nested entity script CKillBird : CEntityScriptBase (Ego_r.pdb CKillBird, 28 bytes: ParentClass +0x14, PMasterData +0x18; private Main / Init / OnPredicateFail = vtable 0x012CE32C slots 1/2/5: 0x00D43190 / 0x00D430F0 / 0x00D43120), appended as a new #if block like the existing CAppleGirl / CMeleeApprentice ones. |

Re-diffed against the repo at hand-back: only these two blocks differ.
