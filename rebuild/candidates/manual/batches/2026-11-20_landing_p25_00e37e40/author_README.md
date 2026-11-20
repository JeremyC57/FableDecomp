# 00e37e40 (p25B, Account B, 2026-11-20): NScript::CV_AssassinAttacksScript::CM_AssassinTrigger::Main -- READY

| Row | Function | Bytes | Grade | Test | Mutants |
|---|---|---|---|---|---|
| 0x00E37E40 | CM_AssassinTrigger::Main() | 1103 | RELOCATION_MATCH (no flags) | PASS (38 targeted + 20,000 random runs) | 41 / 43 killed (1 equivalent, 1 same code) |

Strict vland (LANDVERIFY_STRICT_HEADERS=1, FABLE_HDR_OVERLAY=hdr/): `RELOCATION_MATCH PASS`, no warnings. Purity: plain C++, no asm,
no pragma, no flag. No local class model: the row is the typed CM_AssassinTrigger of engine/CV_AssassinAttacksScript.h.

## What the parked diagnosis missed
The parked draft (DIFFER 1104v1103, local models; "lane gap: dtor inline-site choice", next step a `#pragma inline_depth`) had two residues:
1. The Knothole flag (`jne ; mov 1 ; jmp ; mov 0` in retail; VC7.1 hoisted the `mov 0` above the `jne`): the Ego_d function-scope local
   `ScriptTriggered` (line 213, 4 bytes = `bool ScriptTriggered=false;`) heads the trigger condition: `if(!ScriptTriggered&&(!..||!..))`.
2. Which exits expand ~CTimer: c2's static block frequency (docs/pipeline/INLINE_DEPTH_REVISIT.md). Measured with the c2 inline trace
   (tools/trace.sh, from followup's inl_host.py): ~CTimer cost 14, threshold f > 0.0323. `while(!IsActiveThreadTerminating()){..}` gives
   the loop exit f 0.008 (call) and the second wait loop's exit f 0.048 (expanded) -- the swap the draft had. The loop-head statement
   macro (lever 4 of that doc), `for(;;){do{if(IsActiveThreadTerminating())return _Fail_Val.Return();}while(0); ..}`, gives exactly
   retail's split: loop-head exit 0.156 and first wait exit 0.078 expanded, every other exit <= 0.023 called, the ctor (0.031) called.

## Source shape
Ego_d.pdb line table (egod_lines/, lines 209..306) and locals: `_Fail_Val`, `ScriptTriggered` at function scope; one block {data,
timer, spawn_mkr, spawn_pos, {assassin, {object}}}. Generated quest-script text of the landed Mains (0x00E377D0, 0x00DDAE40): `TRetVoid
_Fail_Val;`, `return _Fail_Val.Return();`, each wait loop with its trailing terminate test on one line, SetPredicate + first frame on one
line (Ego_d 211).
- `// Inferred:` (1) the `for(;;)` loop-head macro (above); (2) `!ScriptTriggered&&` (above; the dead `mov [esp+1Ch],0` at entry comes
  with either text); (3) `timer.GetTime()!=0` (`test eax,eax ; je`); (4) `ScriptTriggered=true;` before the final return (dead; Ego_d
  line 301 is a 4-byte store near the end; the row matches without it).
- Header contract bodies (as the landed CTimer / CScriptThing rows): inline CTimer ctor / dtor / operator= / GetTime through pDSTGame,
  inline CScriptThing::GetPos, inline CIsThingAlive ctor; ~CIsThingAlive stays out of line (retail calls 0x00E241B0).

## Header overlay (hdr/, hdr.diff; additive: the one `-` line is the existing size assert respelled FABLE_STATIC_ASSERT_AT(92, ..) by
pin_asserts.py)
- engine/CV_AssassinAttacksScript.h: NEW opt-in `FABLE_RETAIL_CV_ASSASSINATTACKSSCRIPT_GETREGIONIDFORREGION` --
  `AA_LocationIdentifier GetRegionIdForRegion(CCharString);` (Ego_r.pdb ?GetRegionIdForRegion@CV_AssassinAttacksScript@NScript@@QAE?AW4AA_LocationIdentifier@12@VCCharString@@@Z,
  retail 0x00E38290, landed).
- Regrade with the overlay (tools/gxa.py): the six landed includers stay matched -- 0x00E38F40 / 0x00E377D0 RELOCATION_MATCH,
  0x00E38990 / 0x00E38360 / 0x00E38670 / 0x00E37DF0 MATCH.
- No new header, so no newly flagged row. Landed rows of this class family still on local models (backlog, not new): CAssassinChest::Main
  0x00E389E0, CM_AssassinTrigger::OnPersist 0x00E38D80 and others in rebuild/src/compiled/00/e3.

## Test (test/00e37e40.cpp)
Links the real row object (its CTimer COMDATs and the expanded CIsThingAlive ctor / GetPos run as compiled). Every external bound by
/alternatename to __fastcall stubs; Game and pDSTGame are DIFFERENT fake objects (the timer calls are proved to use the global); Me's
slot 3 and the marker implementation's slot 6 through fake vtables; native-layout fixtures for the entity, the parent script (counter,
last-kill region, RegionInfo flags) and the script def (proximity, off-screen time, AssassinObjects vector). Scripted terminate /
distance / on-screen / timer sequences, Knothole / quest flags, null marker (NullCoord), region id, counter / vector size (wrap),
active-region and same-region cases. Checks the event order against an independent model, the stored regionName / regionId, the item
given, counter wrap, region flag, SetTimer value, the GetThingWithScriptName argument identity (Me's data string), and that every
string, thing and timer is released.

## Mutants (mutants/)
43 one-decision mutants: 41 killed; `!ScriptTriggered&&` -> `ScriptTriggered||` survives (equivalent: the flag is a constant false);
the creature script-name literal swap is SAME_CODE (only a relocation target changes; the test's strcmp would catch it).
