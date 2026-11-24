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

## Landed by Account A (2026-11-24, batches/2026-11-24_landing_acctB_b24)
0x012133C0 (at 8 bytes, as its sibling funclets; the `__ehhandler` thunk stays a manifest-gaps line), 0x00407A70, 0x00407BA0. Still staged: GFMain (under repair) and 0x00BE4FE0 (held for the shared template).

## 0x00402510 GFMain: READY (repaired by B24C4, re-verified by the coordinator 2026-11-24)
RELOCATION_MATCH PASS, /Gr /GS (as GFConfigDetection 0x004017B0, the landed row from the same object). Test regenerated from the row body;
two one-line mutations FAIL. Pick-any: 12 / 12 copies EQ (pickany.txt in notes). Overlay: 12 existing headers with additive `#ifdef` opt-ins
only (diff -0 against today's headers; each named after what it adds, no `_GFMAIN` gates) plus 7 NEW headers with Ego_r layouts
(CSystemManagerInit, CInputManagerInit, CDisplayManagerInit, CSystemRegistry, CBankFileManager, NUISystem_CIME, CSoundManagerInit); evidence
per opt-in in hdr/00402510/README.md. Data globals use the landed spellings (52 data relocations checked; one exception, 0x013B864F, in NOTES).
vland warnings, all explained in notes/00402510/NAME_FIXES.tsv (24 lines, supersedes the 20 queued by A): RELOC_NAME on 0x009D81E0 (placeholder
label -> descriptive DisableVTuneCollection, no PDB name), 0x00415530 GetLanguageFolder, 0x009BC8A0 SetNoHangulWordWrap, 0x00402130
TestDirForValidFilePermissions (Ego_r), and `??1CAFile` -> 0x0099A300, a linker-folded body shared with ~CBaseClassNonCopyable (GOTCHAS nf104;
the new CAFILE_DTOR_DECL_ONLY removes a differing ~CAFile copy); RELOC_ALIAS `_exit` (import thunk, expected).

## 0x00D11930 NScript::CQ_BanditCampBossBattleScript::CAssassinMarker::Main: READY with review items (B24A4, re-verified 2026-11-24)
RELOCATION_MATCH PASS, strict vland, no warnings. Flags `/d2inlS15` (the landed BanditCamp family form, CBCGameMaster 0x00D067A0) plus three
scoped `#pragma inline_depth(0)` regions, each justified in the source's `// Inline-budget:` line (EXTRA_RULES owner decision 2026-11-08):
around the `~CIsThingAlive` definition, the final loop's check, and the cut-scene block's terminate check, where retail calls ~CTimer
(0x00D128BA). Overlay: CQ_BanditCampBossBattleScript.h +22, additive (`FABLE_RETAIL_CQ_BANDITCAMPBOSSBATTLESCRIPT_ASSASSINMARKER`: class body
and a `sizeof == 0x1c` assert). Test fixed for VC7.1 (no zero-init of a non-POD `World()`), vt PASS. Review items for the lander:
- `map<CCharString,STB>::operator[]` is declared-only ROW-LOCALLY (it was a leaf-referenced NEQ copy of 0x00CD3D2E); EXTRA_RULES 2026-11-20
  prefers a header decl-only opt-in.
- Pick-any: six NEQ copies, none leaf-referenced, all already in Account A's /d2inlS15 audit (`??1MovieBase`, `??4STB`, `??1pair`,
  `??1CIsThingAlive`, `_M_erase`, `clear`): family debt.

## 0x00DB3FA0 NScript::CQ_NewOakValeIntroScript::CNOVI_BookTrader::Main: matched, HELD (B24B3, re-verified 2026-11-24)
RELOCATION_MATCH PASS, strict vland, no warnings; vt PASS. What made it match: the draft's renamed header opt-in (`..._ENTITIES3`) and
`#pragma inline_depth(0)` around the forced `~CIsThingConscious` definition (retail: outer vtable store expanded, nested ~CScriptThing
0x004AA840 called); the leaf-referenced ~MovieBase copy was fixed by using the visible header template (HEAD row 0x00D44CB0's spelling).
HELD: the depth-0 pragma makes the object emit a `??1CIsThingConscious` copy that differs from its own landed row 0x00EF7AB0 (without the
pragma the copy matches). Under EXTRA_RULES 2026-11-20 that holds the row until the lander rules on it or a form without that copy is found.

## Lane B findings (progress/b24b_SHARED_FORM.md)
Today's CTimer header text is the original: all four bodies hash-equal FableWin's Ghidra instruction signatures once VC10's split virtual call
is allowed for (tool: notes in progress/00f1eed0/). ~CTimer costs c1 14 everywhere. The ??BCDefString 0x00415DD0 pick-any NEQ is HEAD debt
(landed 0x00EB3D10 emits it too). Oracle extents of 0x00D0D910 (4752) and 0x00DC93F0 (4507) were wrong in B's cut and are re-cut.

## 0x00975BB0 CAIStateGroup_JackOfBladesStage1::Update: READY, with an Inferred item for the lander (B24A7, re-verified 2026-11-24)
RELOCATION_MATCH PASS, stock flags, strict vland; warnings: RELOC_NAME 0x0129BA3C (`?DELTA_NOTIONAL_ZERO@@3MB`, labelled g_NormaliseEpsilon)
and 0x0091F990 (full mangled converting-constructor name) -- both label spellings, in notes/00975bb0/NAME_FIXES.tsv; RELOC_ALIAS `__CIfmod`
(import thunk, expected). vt PASS, mutation FAILs. Pick-any: 13 / 13 copies EQ (pickany.log). Overlay additive (C3DVector.h +5
`ISNONZERO_YZX_BRACKET`; CThing.h +23 `ISDEAD_DIRECT` and the size-owner scaffold `GETTCDRIVER_PHYSOBSTRUCTION_SIZE_OWNER`, the
2026-11-20 pattern of the landed `CDEFCLASSBASE_CREATURE_OWNER_SIZE_PASS26`, copying landed owner 0x007890EA's text).
What made it match: c2 orders the three shared x87 product temporaries by temp number mod 1024, so the number of temps in the whole function
decides site 2; evidenced helper texts (GetTC / PeekTC original chain, IsDead direct, IsStillRunnable nested ifs; FableWin size SAME) plus
**`// Inferred:` nine dead release ASSERTs** (fable_assert.h, the form 72 landed rows use), one after each unchecked pointer result. Evidence:
FableWin's copy is +734 instructions at /Od (one debug ASSERT ~42 i, so roughly 10-17 asserts); graded window 9-14 matches; conditions are
unrecoverable (notes/00975bb0/ESCALATE.md). Under EXTRA_RULES "2026-11-19 advisor decision" this is a fitted tie-breaker that stays
`// Inferred:` and is reported here: the lander decides. GET_ROUNDED_ANGLE keeps FABLE_FORCEINLINE with evidence (no out-of-line copy in
Fable.exe / Ego_r; plain inline scores 75 v limit 74; landed 0x0068CB90 / 0x00974AA0 use the same).

## 0x0070D940 CAnimateThing::CalcInterpolatedMeshAnim: WITHDRAWN (B24C9) -- the /d2inlT67 payload below was removed from rows/ and payload.json
The agent withdrew it: the flag applies to the whole TU and turns a site inside an emitted `_M_insert_overflow` copy into a call, so that copy
no longer equals retail 0x00711A50. Stock best: DIFFER(2507v2395), 21 hunks, one inline site left (progress/0070d940/).

Earlier note:
Coordinator re-verify of the agent's intermediate payload: RELOCATION_MATCH PASS, but only with `/d2inlT67` (a device still under work: the
first combination pass's `_M_insert_overflow` site scores 68 v the default 74) and with `RELOC_CONFLICT _$E2 -> 0x01229630 here, 0x01228930 in
landed 0x01219F00` (a compiler-generated static initializer symbol) plus RELOC_NAME 0x0129BA3C (label spelling). Staged only so the work is
durable; the agent is still on it. Do not land until this section says READY.

## 0x00839940 CThingGameObject::OnHit: NOT READY -- being re-texted (Account A, 2026-11-24: FableWin.exe 0x027EC280 has ELEVEN recoverable asserts; B is replacing the five fitted ones with them; A's NF341 fixes the GetDef<CMaterialDef> copy that holds the row). Earlier status: (B24B5..B24B7, re-verified 2026-11-24)
RELOCATION_MATCH PASS, stock flags, strict vland on overlay `hdr/00839940/` (= the agent's `hdrf`, re-based by the coordinator onto today's
CThing.h: it now uses the LANDED `FABLE_RETAIL_CTHING_ISDEAD_DIRECT` instead of the agent's identical `ISDEAD_BITFIELD`, and the one edited line
is the existing `#ifdef FABLE_RETAIL_CTHINGGAMEOBJECT_ONHIT` turned into `#if <ONHIT && ONHIT_HASTC_CLASSTYPE> ... #elif <ONHIT>`: default
and existing ONHIT branches unchanged; CCreatureDef.h / CHeroExperienceDef.h additive StatGetTypeName opt-ins). vt PASS, mutation FAILs.
What made it match: the GetSubDef<T> body visible (engine/CDefClassBase_GetSubDef.h), one flat weapon guard instead of a nested block (frame
packer trace), FableWin-text helpers, and **`// Inferred:` five dead release ASSERTs** on the pointer results OnHit dereferences unchecked
(two GetArmourDef reads, ptcd_explosion, two GetPBaseDef calls): they push c2's running IL at the hero-experience site from 8927 to 9237
(cap 8992), so IS_THING_ALIVE / bitset::test are called there as in retail. Graded window +74..+396 matches; FableWin's /Od OnHit is +1217 i
(upper bound ~29 asserts after VC10's per-virtual-call extra). The two GetPBaseDef asserts build a class temporary inside the dead branch; the
3-assert alternative without temporaries (agent's asrt/f4.cpp) also matches with a 9-unit margin. Same rule as 0x00975BB0 (landed by A).
Warnings: 6 RELOC_NAME (folded bodies / placeholder labels, two-source lookups in notes/00839940/NOTES.md) and 1 RELOC_CONFLICT:
`DeregisterCombatant` -> 0x006EA430 here v 0x006ED9C0 in landed 0x007355F0 -- the landed spelling of 0x006EA430 is wrong: it is
`CCombatManager::SetAttackerAsBeenStruck` (Ego_r object order and size, FableWin agree; notes/00839940/NAME_FIXES.tsv). Pick-any: every
leaf-called copy compared (agent's leafcopies.py): 12 EQ, 1 NEQ = the known GetDef<CMaterialDef> 0x0066C340 HEAD debt.
