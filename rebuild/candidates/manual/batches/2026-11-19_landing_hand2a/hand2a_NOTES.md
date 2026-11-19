# hand2a NOTES (2026-11-19)

## 0x00B0D360 CPSCUpdateNormal::DatabaseSerialise -- RELOCATION_MATCH + PASS (strict vland, today's tree)
hand1 candidate unchanged (src/00b0d360.cpp, test/00b0d360.cpp). Closed by the visible CPersistContext::Transfer<T> body
(existing opt-in FABLE_RETAIL_PASS22_CPSCLIGHT_DATABASE_SERIALISE reused for what it gates in CPersistContext.h / CPersistTraits.h).
No new opt-ins, no flags.

## 0x00680050 CThingObjectDef::Validate -- RELOCATION_MATCH + PASS (strict vland) -- OWNER DECISION NEEDED (include order)
src = hand1/fable8 c1, unchanged. The ONLY difference from the escalated DIFFER draft is the include order:
`engine/CMeshDataBank.h` before `engine/CWorld.h`. Mechanism (fable8): c2 numbers candidate records from freed constant
records hashed into 64 buckets, address constants by c1 SYMBOL ORDINAL & 0x3F; the include move puts CCharString::NullChar
in bucket 0x3B so the cleanup-flags temporary takes record 18 and pappearance_def keeps 12 -> reloads at 0x00680196 in retail order.
Source-level shapes already measured without effect (hand1 + fable8, ~155 grades): if / nested if / && chain / !cond with dead asserts,
pappearance_def declared earlier (worse), `||` spellings, explicit GetSubDef instantiation, extern placement at every include boundary.
Supporting fact: Ego_d's thing_object_def.obj contains exactly ONE function (CThingObjectDef::Validate) (ghidra_out/egod_carved_functions.tsv),
so in the original TU the symbol ordinals came only from that file's include list: include order is a real property of the original source,
not a device; but the original include list itself is not recoverable. Margin is thin (fable8: two new declarations in a header ahead of
CAnimFilterBase un-match it) -> a fragility for later header growth.
Test: codex 3072-case standalone fixture (traces string copies/dtors, sub-def release, mesh stats refcounts, branch per Type/Property).
vland prints a non-blocking WARN STANDIN_LEAF because the catalog name is `CThingObjectDef::?Validate@...` (see NAME_FIXES.tsv).
No new opt-ins, no flags.

## 0x008EEC70 Mass::Collide -- RELOCATION_MATCH + PASS (strict vland)
src = hand1/fable5 final_dot_group (draft + `#define FABLE_RETAIL_C2DVECTOR_DOT_GROUP`). NEW OPT-IN in hdr/engine/C2DVector.h
(only the DOT_GROUP arm of hand1's patch; the probe arms DOT_X_GROUP / DOT_Y_GROUP / DOT_NAMED dropped): grouped `(X*vector.X+Y*vector.Y)`.
Why: Dot is the only C2DVector helper expanded exactly once in the post-`bounce_impulse<0` scheduler region; its PAREN tuple is the missing
81-tuple-region node that moves `mov eax,esp` (0x008EEFA8) after `sub esp,8`. Other tc_tavern_game_coin_base.obj rows expanding Dot
(e.g. CTCTavernGameCoinBase::UpdateCoins) may want the same define (ungraded).
Test: codex fixture + a new rotating-bodies block (bounce leaves normal contact speed at -e x before; unclamped friction zeroes tangential
contact speed; uses the angular terms); mutants killed: GetVel angular term, verlet choice, friction clamp, ForceToVel angular term.
No flags.

## 0x00918A40 CAIStateGroup_GazeAtWork::IsRunnable -- RELOCATION_MATCH + PASS (strict vland), 11 grades
Closed by SOURCE STRUCTURE (duplicated return arms), not spelling:
    if (GetOwner()) { CTCShopKeeper* ptc_shopkeeper;
        if (!PBrain->Creature->GetTC(&ptc_shopkeeper)) return FindFreeGazePos() != 0;
        CTCShop* ptc_shop = ptc_shopkeeper->GetShopTC();
        if (ptc_shop && ptc_shop->IsOpeningHours()) return FindFreeGazePos() != 0; }
    return false;
VC7.1 cross-jumps the two identical FindFreeGazePos returns; the edge structure that merge leaves keeps the inlined GetTC's store block
(0x00918A85 `mov eax,[eax+4] ; pop esi`) separate from the GetShopTC block (0x00918A89 `mov ecx,eax`), retail's "removed second
predecessor". Measured on the way (ATTEMPTS.log): every presence-test-twice shape shares CSE temps (`mov edx,[eax+30h] ; mov ecx,10000h`),
so retail never had a second presence test; a named bool for the GetTC result is inert. Ego_d: `this` + one block {ptc_shopkeeper, ptc_shop}
= the if (GetOwner()) block. Same header opt-ins as the codex draft (FABLE_RETAIL_CTHING_GETTC_EXPANDED etc.); no new opt-ins; no flags.
Test: codex fixture with the matched body (owner absent / no keeper / null shop / closed / no gaze pos / runnable, call counts).
Lesson: a block boundary with a vanished predecessor right before a call can be a cross-jumped duplicate `return` arm.

## 0x006E89B0 CCombatWheel::GetAttackerPlacementVector -- IN PROGRESS (DIFFER 344v344, 1 hunk pair)
Residue: arm-1 join region (0x006E8A6D..): retail `fld [esp+10h] ; pop edi ; fmul st(1) ; pop esi`, ours `pop edi ; pop esi ; fmul`.
Measured with the c2 DAG harness (tools/sched, copied from 2026-11-09_linux_hand892; tmp/dag6.sh): join region 18 nodes; cyc0 fadd+mov,
cyc1 fld+_epop13, cyc2 _epop14, cyc3 fmul. Retail needs ONE extra u0 no-code node ready at cyc<=2 and independent of the fadd
(c4 probe's 0x269 between fld and fmul gives exactly retail's order). Required fix independent of the hunk: DELTA_NOTIONAL_ZERO
(0x0129BA3C) instead of the 0.0001f literal (draft relocation was wrong; head6.txt has it).
Tried since (ATTEMPTS.log): helper bodies x8, arm-1 forms x7, C3DVector bodies x4, radius-local placement x3, GetRing after the row,
IsOuterRing condition (proven name; same bytes), dead asserts x3, /Zi /GX /Gr. Every one leaves the 18-node join DAG identical.
ESCALATED 2026-11-19 (tmp/ESCALATE_006e89b0.md, SendMessage to main) at ~50 measurements -- early versus the 100 rule, said so in the
message, because all other rows are matched and every natural level is exhausted. Waiting for the advisor's candidates.
Name facts for landing: 0x006E7FD0 = CProng::GetProngVector, 0x006E8810 = CCombatRing::GetAttackerProngVector (NAME_FIXES.tsv);
DELTA_NOTIONAL_ZERO relocation at 0x006E8A52.

## Tests: mutants run 2026-11-19
00918a40: 3/3 killed (GetTC test inverted, IsOpeningHours dropped, GetOwner dropped). 00680050: 4/4 killed. 008eec70: 4/4 killed.
