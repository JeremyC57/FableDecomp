# 2026-11-11 landing cv -- naming agent NF21: Codex pass 20's name-fix queue, 85 landed rows respelled, 21 renamed

Source: `naming/2026-11-11_agentNF21/` (STATUS, APPLIED, REJECTED, RENAMES, NOTES, LESSONS, QUEUE, hdr README).
- The queue had 176 entries (land1011bu): 29 done, 147 partial, 0 rejected.
- 21 GameCODA/SAL entries were not applied: no SDK CodeView here for a second source.
- The large stand-in families (PMainGameComponent, GPCamera, PClass ...) stay partial.

**Verification**
- vland --recheck-landed ran in two parts: all 85 grade MATCH or RELOCATION_MATCH.
- Running every test exe found its catalog PassPattern in 85 of 85.
- Bindings were re-derived. bindings_multi_check is at its NullCoord baseline.
- NF21 copied its rows from 6e32e4208, and none of the 85 changed since. Its three headers were 3-way merged on that base: all clean.

**Not landed (2 of NF21's 87)**

0x0072C790 and 0x007C9180. NF21's respell added a small local CCreatureActionBase model, and that class has a shared header (rule:
no new local models). Both keep their landed text.

**Changes**
- AddMessage: 10 rows, now on the shared CMessageEventManager.h with named EMessageEventType values.
- AddTC 5, sprite constructors 5, SetCurrentAction / ~CCombatActionBase 5, IsPerformingAnyAction 5.
- Also the const-reference filter family, CTCWife / opinion, GetPerceivedThing, and the CDataOutputStream Write* members.
- 21 self-row renames (RENAMES_applied.tsv). Among them: CMessageEventFilter_IsOfType::operator() (was HasExtraData),
  AdjustOpinionTransient, CTCBuyableHouse::SetWifeLivingHere, C3DKeyframe::ToMatrixTransposed, and N3DPrimitive::CBaseBlock::SaveBinary.

**Headers (opt-ins only)**
- CTCBuyableHouse.h: _WIFE
- CDataOutputStream.h: _WRITEUBYTE
- N3DPrimitive_CStaticBlock.h: CBASEBLOCK_SAVEBINARY

**Follow-ups (queued for NF22 as land1011cv)**
- ST_HEAD_TURN_H / ST_HEAD_TURN_V for g_CCharString_013bace4 / _013bace8.
- 0x007C9180's SetPItemRemoved call.
- Header notes in NOTES.md: CMessageEventManager.h lacks its rebuild_abi.h include, and CThingFilter_IsNot.h does not
  forward-declare CThing.

Labels: ghidra_out/labels_landing_2026-11-11cv.tsv.
