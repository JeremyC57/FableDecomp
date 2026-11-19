# hand4d hand-off (2026-11-19, wrap-up by owner order)

This workspace is a FableDecomp parked-row hand-authoring lane. Matching means VC7.1 RTM 13.10.3077 under Wine. The repo is read-only from here; the coordinator ("main") lands the batches.

## Done (handed back as READY batches; snapshots in batches/bN/)
| batch | address | function | grade | notes |
|---|---|---|---|---|
| b1 | 0x00540320 | NUISystem::CEditBox::KeyPressed | RELOCATION_MATCH + test | landed |
| b2 | 0x0063F7C0 | CThingSearchTools::ForEachInRoughArea<...> | RELOCATION_MATCH + test | landed |
| b3 | 0x00727220 | CApplyAdditionalImpulseIfInCombatAndTooClose::operator() | RELOCATION_MATCH + test | landed |
| b4 | 0x0085B2A0 | CThingSearchTools::GetAllThingsInRoughArea<CThingGameObject, list, Bind4<..Combat..>> | RELOCATION_MATCH + test | overlay CFilter_Bind4.h |
| b5 | 0x008DD7E0 | NCreatureMode::CModeThunderLightningStorm::UpdateDamage | RELOCATION_MATCH + test | NEW CThunderBattleDef.h |
| b6 | 0x00AEDE60 | NParticleEngine::CParticleEmitter::SetOrientation | RELOCATION_MATCH + test | overlay CMatrix3x4.h |
| b7 | 0x00A58130 | NBBBMaths::CLinearSpline::ProcessPoints | RELOCATION_MATCH + test | IDENTIFY (NAME_FIXES) |
| b8 | 0x00A8F1B0 | C3DPrimitive2::ConvertToVBFormatStaticBump | RELOCATION_MATCH + test | overlay C3DPrimitive2.h + 2 NEW headers; IDENTIFY |
| b9 | 0x00774B40 | CTCShop::GetFreeSaleItemPlaces (RE-PORT of a landed local model) | RELOCATION_MATCH + test | coordinator confirmed in |

I know b1 to b3 and b9 are in. For b4 to b8, check landing with the coordinator. Each batch's README lists its overlay, Inferred lines and name fixes.

## Remaining rows

### 0x008185B0 NMaths::GetRootsNumerically<CVeryRandomFunctionToFindThePointOnAnElipseThatIsClosestToAPoint> (OPEN, 78 grades)
- Best is src/008185b0.cpp, DIFFER(1360v1360) with 10 lines differing, flags /Gr. It is the landed sibling template text plus FableWin's GetValue with `(Ca*cos_x)` grouped.
- The residue is one inlined GetValue site, the refinement `val_b` at 0x00818827..0x00818855: retail factors by cos there, while this build factors by sin.
- cls.py classifies every site. Retail reads S S S C S; every probe gives all-S or all-C.
- What has been tried, and my next ideas, are in NOTES.md under "0x008185B0 state at hand-off".
- At 100 grades, ESCALATE (SendMessage `ESCALATE 0x008185B0: ...` + tmp/ESCALATE_008185b0.md).

### 0x00774D20 CTCShop::BuyItem (OPEN, 49 grades)
- Best is src/00774d20.cpp, DIFFER(1149v1149) with 34 lines differing, no flags.
- The residue is a register-role permutation from 0x00774D7B:

  | value | retail | ours |
  |---|---|---|
  | zero | esi, kept until the `false` push at 0x00774E6C | edi |
  | def | edi | esi |
  | pshop_def | ebp | edi |

- Coordinator levers 1-5 are done. Lever 5 became the b9 re-port.
- The full list of what was tried, and my next ideas, is in NOTES.md under "0x00774D20 coordinator levers" and "state at hand-off".
- Per the owner order, this row goes to the other account.

### GameCODA SDK rows (NOT started; assigned to the other account)
- The two rows:
  - 0x00C77800 CFloatReverbParameterTranslator::UpdateDecayTimes (CODAApi.lib FloatReverbParameterTranslator.obj).
  - 0x00C88E60 CReverbFnsSSE::ApplyFilterOrder2_quad (CODAApi.lib ReverbFns_SSE.obj).
- hand4c showed each is byte-identical to the SDK member once relocations are masked. The plan is owner-approved (2026-11-19; EXTRA_RULES, "rows byte-identical to the GameCODA SDK library").
- Step 1: author ordinary C++ on the shared audio headers (CReverbFns.h, CFloatFilter, CodaProcessingTypes).
  - Take names, locals, scopes, statement order and operand order from the CODAApiDbg.lib CodeView of that member. hand4c's tools/arx.py extracts members.
  - The SDK tree is read-only: scratchpad/sdk/gamecoda/tree/CODA. Never copy SDK files into a hand-back.
  - The recipe that closed 0x00C54C10 is in w24/hand4c NOTES / LESSONS. The drafts in w24/hand4c/differ/ are evidence only, not for landing.
- Step 2: if a row still differs after about 30 grades, stop. It then takes the library grade plus a FABLE_PORTABLE twin, and the best C++ becomes the twin. Do not chase 100 grades on these.

## Overlay headers (hdr/; README.md there has the details)
- engine/CFilter_Bind4.h: opt-in FABLE_RETAIL_CFILTER_BIND4_FORCEINLINE (b4).
- engine/CThunderBattleDef.h: NEW (b5).
- engine/CMatrix3x4.h: opt-in FABLE_RETAIL_CMATRIX3X4_INVERSE_NO_TRANSLATE_FORCEINLINE (b6).
- engine/C3DPrimitive2.h: opt-in FABLE_RETAIL_C3DPRIMITIVE2_CONVERTSTATICBUMP (b8).
- engine/CTVertexNormTex1BumpBase.h, engine/CTVertexNormTex1Bump.h: NEW (b8).
- Each was copied from the repo on 2026-11-19. Re-copy from the current repo before reuse.
- The open rows need no overlay.

## Tools (all run from this directory)
- g.sh `<addr> <src> [leaf] [--flags ...]`: grade. It appends to ATTEMPTS.log; set IDEA="note" to label the attempt. Use HO=<dir> for a probe overlay.
- mt.sh `<addr> <test>`: run the behaviour test. The pass marker is the lowercase `<addr>_TEST PASS`. tce.sh `<addr>` shows test compile errors.
- vl.sh: strict vland of payload.json / oracle.tsv. vlr.sh is the same with --recheck-landed, for re-ports.
- mkpayload.py: builds payload.json and oracle.tsv from matched.tsv (`addr module name [oracle.tsv]`).
- ddis.py `fw|ed|er <hexva> <len>`: disassemble a donor build. fwn.sh `<thunk>` resolves a FableWin thunk to its name.
- cls.py `<g_file>`: the 0x008185B0 per-site factoring classifier.
- sd.py / od.py / qd.sh / qq.sh: diff helpers.
- Ego_d locals: `python3 -I rebuild/candidates/manual/pending/2026-11-17_egod_parked/lane/egod_locals.py /tmp/claude-0/egod_pdb/Ego_d.pdb '<name>'`.
- egod_type.py takes `/tmp/claude-0/egod_pdb/types.txt <typeidx>`.

## Records
- NOTES.md (per row), STATUS.tsv, ATTEMPTS.log (every grade), NAME_FIXES.tsv, LABELS.tsv.
- Probe sources are in probe/; the reference dumps for each row are in ref/<addr>/.

## Coordinator note at hand-off (2026-11-19)
All of b1-b9 are landed in the repo (batches/2026-11-19_landing_hand4d_b1..b8, report_hand4d_b9). Every overlay header in hdr/ is identical to the repo copy, so nothing is pending merge.
