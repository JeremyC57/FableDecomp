# chunk 801 notes

All six row names are proven from two sources (PDB name sets + vtable slot / callers); none is descriptive. Callee lists with
addresses are in each source's `// Callees:` block. Descriptive names used in models (not retail symbols): the stand-in filter
structs' member names where the PDB gives none are marked in the sources.

## 0x007B0CD0 CTCFishing::FrameUpdate -- RELOCATION_MATCH (src/)
Five-stage switch (table 0x007B1A14). Key spellings: FABLE_RETAIL_CTHING_GETTC_EXPANDED with out-pointer TC locals; an `fps`
local copy of CMainGameComponent::GetConstantFPS() in the caught-timer and cases 2/3 (retail reloads [0x01375550] once per
block); the experience test folded into one condition; reference locals for the text bank and the caught-fish def. Final
respell: the first draft's `g_framesPerSecond` stand-in is the protected static CMainGameComponent::ConstantFPS read through
the inline GetConstantFPS() (vland RELOC_NAME; name-only, bytes unchanged). FinishFishing 0x007B0950 is
`?FinishFishing@CTCFishing@@IAEX_N@Z`.

## 0x007A7D20 CTCBattleCharge::CreateHitEffect -- DIFFER(2504v2520), 64 lines (differ/)
Shape: MakeNoise; first rough-area search (Bind3<In3DRange, IsNot, CanBeHitByEpicSpell>, 0x007A9300) in its own block; the
DragonCliff2 region branch (second search 0x007A9580 with HasInstantiationName, push_back into the things vector); hit loop on
`things.size()` (damage scaled with distance, enemy check, CHitParameters 0x00789238, OnHit slot 65, entity event
CEventHitBySpecialAbility); radial blur at the impact point; sound via an inline CVectorMap lookup; SpellLevel switch (4
levels) for on-hit / displacement effects; debris effect via the inline GetEffectFromString (CRC table 0x0129A168 + map find);
rumble for the main player. Retail def layout: CSpecialAbilitiesBattleChargeDef BattleChargeDamage +0x58, Radial* +0x68..+0x84,
DragonExtraGumph +0x9C / +0xA0; CHitParameters CreateDustParticleEffectOnHit +0x24. Residue in the Grade line.
Labels to correct: 0x006B82E0, 0x007A9300, 0x007A9580 (NAME_FIXES.tsv).

## 0x007DB6A0 CTCTavernGameCardBlackjack::CardsMainGame -- DIFFER(2544v2556), 63 lines (differ/)
Switch over TavernGameState 0x17..0x25 (table 0x007DC060), case source order 0x1c, 0x1e, 0x1d, 0x1f, ..., 0x25 before 0x20;
return-ending cases except 0x25. 0x007DB5C0 is Split (not DealCards). Uses the existing shared GFFloatToLongFloor retail-asm
helper (Bets[0] * 1.5f, 0x007DB91A..0x007DB94B), so a match would grade retail_asm. Residue: case 0x1c's SetState tail.

## 0x00833C00 CThingAICreature::DrawPreparePrimitivesForRendering -- DIFFER(2296v2300), 648 lines (differ/)
The slot-36 name proof is in LABELS.tsv. Shape: clear DebugRenderHandles (out of line 0x00834F90), base pass 0x00663C40, then
debug blocks (def name text, brain state-group text + name, stealth perception, lines to home / work building, navigation
level + containing volume's def name), each adding a handle with `push_back()` = insert(end()) 0x007464A0. Only the line block
unscoped gives the retail frame 0x88. Header opt-ins: hdr/README.md. Residue: FPU Y copy and info.Engine caching.

## 0x00850A80 CContextSensitiveItemDef::IsValidOverride -- DIFFER(2684v2576), 378 lines (differ/)
Two switch tables (Type 2..9, ExtraData1 1..6). Same-TU noinline stand-in bodies (GetSubDef / GetDef / GetAllThingsInRoughArea
0x00851490) brought it from 585 to 383 lines. Uses the shared GFFloatToLongCeil helper (retail asm 0x008511A6..0x008511D5),
so a match would grade retail_asm. Residue: frame 0x3c v 0x30 / parameter-slot homing, edi = 0, epilogue copies, case-8 Y-first.

## 0x00960120 CAIStateGroup_DeedReactionRoot::Update -- DIFFER(1576v1600), 618 lines (differ/)
State switch (5 entries, table 0x0096074C). GetTC (non-const, 0x0040F020) and PeekTC (const, 0x004365B0)
split per retail; the CIntelligentPointer<const CThing> copy is out of line (0x00654D58, hdr opt-in). Residue: the bit-16 HasTC
narrowing (VC7.1 byte load v retail dword shr).

## Header debt
None beyond hdr/README.md. Candidate for the generator: CSpecialAbilitiesBattleChargeDef retail offsets (above) are modelled
locally in differ/007a7d20.cpp, not in a shared header.
