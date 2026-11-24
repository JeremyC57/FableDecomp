# 2026-11-24 acctB B24 hand-backs (staged for Account A to land)

Owner (2026-11-24): Account B verifies that each row matches and passes its test, and saves it here; the main account (A, `FullDecomp`)
lands it later. Every row below was re-verified by the B coordinator; nothing here is landed by B.

Agent B24C's four matched rows, re-verified by the B coordinator with read-only `vland.py` (strict headers, each row's own overlay in
`hdr/<addr>/`; logs in `coordverify/`). No oracle bytes here. Each row still has open checks before landing:

| address | function | coordinator re-verify | open before landing |
|---|---|---|---|
| 0x012133C0 | EH unwind funclet of CPhysicalPlayer::Play 0x00C271F0 | RELOCATION_MATCH PASS, no warnings | ready to land. Its 18-byte extent covers the parent's `__ehhandler` thunk at 0x012133C8; no landed Unwind row contains a thunk, but no row or manifest start covers 0x012133C8 either, so the manifest extent (next start 0x012133D2) is the parent's whole block, all compiler-emitted; the agent notes an 8-byte cut matches too |
| 0x00407A70 | CUserProfileManager::SetAssignedInputKeyboard | RELOCATION_MATCH PASS, stock strict vland, no warnings (after the manifest starts below) | ready to land; header: copy `hdr/00407a70/engine/CUserProfileManager.h` (current header + one additive opt-in, pin_asserts: 0 pinned) |
| 0x00407BA0 | CUserProfileManager::SetAssignedInputMouse | same | ready to land (same overlay) |
| 0x00402510 | GFMain | RELOCATION_MATCH PASS, 5 warnings (4 RELOC_NAME on manifest labels the agent proves wrong, 1 expected RELOC_ALIAS `_exit`) | data globals spelled differently from landed rows (switch to the landed spellings, leave the proofs in NAME_FIXES.tsv for Account A); `Body_38[8]` placeholder in a new CSystemManagerInit.h; twelve `_GFMAIN` opt-ins and six new headers to check against the merged headers; `/Gr /GS` flags; test must be able to fail |

Agent notes, escalations and name fixes are in `notes/<addr>/`.

## Coordinator, 2026-11-24
- Manifest starts 0x00407A70 / 0x00407BA0 added by the boundary-starts route (`rebuild/manifest/boundary-starts.tsv` + `crawl/manifest_add_boundaries.py --write`, precedent 33add08c6f), with the evidence in the boundary line. Stock strict vland: both RELOCATION_MATCH PASS, 0 warnings. The 0x004075C0 landed extent (1194 B) ends at 0x00407A6A, before the new start.
- Name check: both mangled names are in Ego_r and FableWin; the keyboard setter stores type 2 (0x00407AE6) and an EInputKey, the mouse setter type 3 (0x00407C16) and an EMouseButtonControl.
- Landing these three was attempted and refused by this session's permission check, so nothing is landed; the landing payload for 0x012133C0 / 0x00407A70 / 0x00407BA0 is `payload.json` here (oracle to be cut on the landing machine).

## 0x00BE4FE0 CWaterPatchTesselatorBase<CTVertexSeaBackground>::TesselateEdge (B24C2, 2026-11-24)
Coordinator re-verify: RELOCATION_MATCH PASS, stock flags, no warnings; pick-any checker (agent): both COMDATs EQ. Overlay additive only
(CPatchTesselationEdgeStrip.h +18, CWaterPatchTesselatorBase.h +63, CLandscapeTesselatorBase.h +11, `_quarantine/CEngineLandscapeMap.h` +10).
What made it match: FableWin-exact FindWaterVert text, one shared `previous = -1` skip with `continue` per bridged span, and the reload of
`strip->GetWaterVertex(i)` for `start` (notes/00be4fe0/NOTES.md). Review item: `FABLE_FORCEINLINE` on the two sea helpers, justified in the
source by retail having no out-of-line copy (FableWin has debug copies, Ego_r's tesselator module has no proc record; plain `inline` calls them).

**HOLD 0x00BE4FE0 until its water twin 0x00BE4660 matches** (B24C2 ESCALATE.md, notes/00be4fe0/): both are instantiations of ONE shared
template text. The sea row matches with four different loop-tail spellings, so its oracle does not fix the text; the water twin (DIFFER
2349v2349, 4 hunks, progress/00be4660/) so far gets close only WITHOUT the `continue` the staged sea text uses. When the shared text is
settled, regenerate the sea row's source, test, payload, vland log and pick-any check from it.
