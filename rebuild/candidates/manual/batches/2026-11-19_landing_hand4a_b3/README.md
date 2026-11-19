# Landing hand4a b3 (2026-11-19): NCameraModes::CScriptModuleOpenChest / UseLamp::OnInitLeaf (0x00853670 / 0x008539C0) -- INFERRED, owner review

vland --land, strict headers: RELOCATION_MATCH + PASS. Ported from local models onto shared headers with FableWin's debug bodies
(0x0283B7D0 / 0x0283BEC0) and Ego_d's block locals; three NEW engine headers NCameraModes_CScriptModuleBase.h / _OpenChest.h / _UseLamp.h
(`hdr_README.md`; size asserts 0x4C / 0x54 / 0xA0 checked against retail ctors and the UseLamp `push 0A0h ; call operator new` at 0x00847F2F).

**Advisor decision (owner delegated):** the residue was c2's 81-tuple scheduling-region cut, two tuples off. The rows are written plainly
(`float mag = AxisY.GetAccurateMagnitude();`); the two no-code brackets live in shared helper text as new nested opt-ins in engine/C3DVector.h:
FABLE_RETAIL_C3DVECTOR_ACCURATE_MAGNITUDE_PAREN_RETURN (`return ((float)sqrt(magnitude));`) and FABLE_RETAIL_C3DVECTOR_DIVASSIGN_RECIPROCAL_PAREN
(`float reciprocal = (1.0f / v);`). No donor shows either bracket (fitted; 0x00F361D0 precedent). Default path unchanged: all 16 landed
ACCURATE_MAGNITUDE users still match (`logs/regrade_accurate_magnitude_users.tsv`); forced on everywhere, 0x00875690 and 0x00761280 would break,
so the bracket is a per-TU opt-in, not a general reading. Rejected alternative: GetAccurateMagnitude via GetSquaredMagnitude (contradicts
FableWin 0x01D99FB0, breaks 0x00875690). OWNER REVIEW: revert this one commit if not accepted.

Queued: 82 landed rows still model the NCameraModes script-module classes locally (`QUEUE_NCAMERAMODES_REPORTS.tsv`); strict mode rejects them
on their next re-land.
