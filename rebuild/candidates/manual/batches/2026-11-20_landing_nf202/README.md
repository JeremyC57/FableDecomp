# Landing nf202 (Account A, 2026-11-20): q12195 .. q12271, fresh queue entries (38 rows)

Agent NF202 (round 10) finished its whole slice of 50 fresh entries: 21 done, 8 done-already, 19 blocked and 2 deferred, none open.
Its files are in `naming/2026-11-20_agentNF202/`, with the held 0x004115A0 draft (`held/`). Seven entries go back to the queue unclaimed
(partial binders or one row left to port: q12255 q12256 q12257 q12259 q12261 q12269 q12271). The blocked ones are noted for named family
passes or decisions: the TU-emitter `??_G` family (q12201 / q12202 / q12245, with q4411 / q6405 / q6513), the CCountedPointer ReleaseData
family (q12226 .. q12229), q12219 with q4394 / q12191, a header claim (q12207), an identity item (q12244), and the 0x004115A0 /
TEXT_EQ_INLINE header decision (q12262 / q12265).

- 38 rows land: 1 MATCH + 37 RELOCATION_MATCH, every test PASS.
- Self renames (RENAMES.tsv section 1, 20 rows), among them:
  - CScriptConversationManager::AddLineToConversation 0x006E5950, CConsoleCommandBase::GetFormattedName 0x009EA3A0,
    NCameraModes::CStringAndCageBase::GetZCoord 0x00871D30.
  - The C3DAnimFileLoader Register<X>SubChunkType family 0x00A98BE0 / 0x00A98C80 / 0x00A98D20 / 0x00A98DC0 (was Add).
  - The VillageTask CCountedPointer families: Torch, SecuritySweep and CarryCrate ReleaseData (0x0065D574 / 0x0065D5CC / 0x0065D5F8)
    and their scalar deleting destructors (0x0065E919 / 0x0065E989 / 0x0065E9A5). The 44-byte bodies are ReleaseData, as nf184 found
    for Horn.
  - STLport instances: `~list<NUISystem::CList*>` 0x0053C9BC, `_List_base<LSocket*>` 0x008F9F50, the `_Rb_tree::_M_erase` of the
    SExpSpendUndo map 0x00579D97, `vector<NUISystem::CComponent*>::erase` 0x00485AA7. Also CCountedPointer Reset / DeleteData 0x00A9CF40
    / 0x00AC1460 and `CPersistContext::Transfer<CDefIndex>` 0x005CC1E8.
- Binders respelled: 0x00890750, 0x009EA3A0, the three CTCAIScratchpad::BettingActive binders (0x0088FB40 / 0x0088FB50 / 0x007E4E30),
  two dead externs onto GDefStringTable (0x008ED440 / 0x008ED2B0), and two full Leaf lines (0x005CC1E8, 0x0065F01D).
- Self-rename check (rule: no landed row may still bind the old self leaf): every renamed row's old leaf is gone from the binding table
  and from every other row, except 0x004115A0.
- Held at landing: 0x004115A0 (`??8?$CBasicString@D@@QBE_NPBD@Z`, the 88-byte body, MATCH). About 20 landed rows, 0x00859040 among
  them, still bind its old self leaf `??8CBasicStringChar@@QBE_NPBD@Z`. Its object also defined a Compare copy that LNK2005d against the
  owner 0x00411570's strong definition. The tree was reset and the 38 other rows were re-applied and rechecked without it; q12262 /
  q12265 stay blocked.
- Headers: add-only `#ifdef` opt-ins; `orig` equals HEAD for all four; no landed includer defines them (`naming/.../hdr/README.md`).
  - engine/CScriptConversationManager.h: ADDLINETOCONVERSATION.
  - engine/CConsoleCommandBase.h: GETFORMATTEDNAME, a partial retail mode under the macro. It models the PDB base CConsoleInputBase
    (Name +0x04), which has no header of its own, and the protected virtual const GetFormattedName. The default class is unchanged, and
    its 4 asserts are pinned.
  - engine/NCameraModes_CStringAndCageBase.h: GETZCOORD; engine/NCameraModes_CBase.h: GETOBSERVEDTHING.
- Pick-any (`logs/pickany_check.txt`): the table-owned copies equal their owners. The agent compared the rest under the
  relocations-must-match rule: 0x00579D97's four copies against 0x0042A168 / 0x0042AB73 / 0x005797FA / 0x005799DE, 0x008F9F50 against
  0x0122A200, 0x00ABFE50 against 0x00AC0C00, and the VillageTask copies against each other.
- Link (`logs/link_landing.log`): 6 LNK2005 against 8 at HEAD (`logs/link_control_head.log`). The two
  `CLandscapeBackgroundPatch_ScalarDeletingDestructor` clashes and `?DeleteData@@YIXPAUAutoTinyVirtualObject@@@Z` are gone. One is new,
  `??1SExpSpendUndo@@QAE@XZ`: 0x00579D97's copy is byte- and relocation-identical to the owner 0x005797FA (35 = 35), which defines it
  strongly (the nf181 / nf184 case). Porting the owner is queued as debt.
- Table hygiene: two stale lines at 0x01375748 are dropped (`?g_bScriptAnimationFlag@@3_NA` from 0x00D7F460 and
  `?g_PlayAnimationDefaultFlag@@3_NA` from 0x00D08790); both rows now bind
  `?GDefaultToUsingPhysicsInCutsceneAnimations@NGlobalConsole@@3_NA`. The missing witness line
  `?pGlobalScriptDef@@3PAVCScriptDef@@A` 0x0143E90C (0x00EE2A80, which declares `extern CScriptDef*`) is added, so that split shows (q12259).
- Recheck (`logs/recheck.log`): rebind multi-check 1 (baseline); split gate 0 created. Gate and referencing rows 92 good, 0 bad.
- tablecheck (`logs/tablecheck.txt`): ten addresses shrink (0x0065D574 / 0x0065D5CC / 0x0065D5F8 3 -> 1, 0x013BAE1D 6 -> 4, 0x004C73D0
  5 -> 4, ...). One grows: 0x00AAE090 0 -> 1 (`??0C3DAnimFileHelpersChunk@@`, from 0x00A999B0, SAME: a binding the table lacked). No
  out-of-image oracle line.
