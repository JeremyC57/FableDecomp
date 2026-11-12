# Chunk Q15 (agentQ15) -- notes

Every grade ran with `FABLE_HDR_OVERLAY=$A/hdr` (tools/gg.sh exports it; ATTEMPTS.log has one line per grade). Overlay contents and
opt-ins: `hdr/README.md`. Default paths of every copied header are unchanged (each change sits behind a new `#ifdef`).
Helper scripts added: tools/chk.sh (compile + check every undefined symbol against reloc_bindings.tsv), tools/vt.py (VC7.1 /Od /W3
behaviour build, as verify_and_land does). tools/cc.py was fixed to write into this folder (it pointed at agent1026's bin).

## Result
- 24 rows: 2 RELOCATION_MATCH (0x0092E130, 0x00713D50), 5 more ported onto shared headers and handed back in differ/ with a
  measured residue (0x00738A80, 0x0090C900, 0x00AC7CB0, 0x0086B9D0, 0x00AC7300), 17 regraded as they sit (all still local models;
  differ/ copies with the regrade on line 2). Every regrade reproduced the Nov 3 index (no header landed since fixed one as-is).

## Matched
- 0x0092E130 CAIStateGroup_MinionSleep::IsRunnable: moving onto the retail CAIStateGroup_GoToBed / CTCSleep headers and the
  expanded CThing::GetTC fixed the parked this/CTCSleep esi-edi swap at once; the last gap (return-false block placement) closed by
  splitting `if (!ready || !base) return false;` into two ifs (retail keeps the return blocks inline after the readiness test).
  New opt-ins: FABLE_RETAIL_CTCSLEEP_FASTTRACK (GetFastTrack decl + inline GetIsReadyToSleep, Ego_r names) and
  FABLE_RETAIL_CAISTATEGROUP_MINIONSLEEP (retail class over the generated header).
- 0x00713D50 NTCScriptedControl::CActionFollowCreature::SetAsSneaking: the parked scratch-register residue was the local model's
  `PTCScript->ParentThing` chain; the shared CActionBase::GetParentThing() + expanded GetTC<T> + CTCStealth::IsSneaking/SetSneaking
  matched on the first grade. New header engine/NTCScriptedControl_CActionFollowCreature.h (Ego_r layout 0x28, name set) and
  FABLE_RETAIL_CTCSTEALTH_SETSNEAKING (inline StatGetInterfaceType = TCI_STEALTH, inline SetSneaking; FableWin-only names, expanded
  in retail).

## Ported, still DIFFER (next steps)
- 0x00738A80 GetEvent<const CEventHitBy, Bind3<Melee, not Blocked, HitBy>> 177v177: Data/Info esi-edi. c2prio: Info prio 80 (esi),
  Data 76 (edi); Data wins a tie on key (16 v 15), so Data needs +4 or Info -4. Tried: one typed local / cast twice, ReleaseData dtor.
  New opt-ins: NEntityEvents_CEventHitBy TYPE (StatGetType = 1), EventFilters CISHITEVENTHITBY (the filter class, from Ego_r).
- 0x0090C900 CThingFilter_IsValidGossipee::operator() 345v345 (ratio 1.000): only one jump target. The factionless
  `else if (thing.HasTC(TCI_ENEMY)) return false;` goes to the SECOND return-false copy (0x0090C994) in retail, the first (0x0090C946)
  in ours. Probed (compile-only): separate ifs in the faction arm swap BOTH targets (je 94 / jne 46), braced else / separate village
  ifs / hoisted enemy local leave it, `if (!PFaction)` and a conditional-expression test change the layout. Next: the cross-jump
  edge order (PLAYBOOK cross-jumping section) on the four false returns.
- 0x00AC7CB0 GetFaceNormal 350v350: everything but the IsNonZero sum. Retail squares Z,Y,X and sums `fld st(0); fadd st(2);
  fadd st(3)`; every IsNonZero / Normalise body probed sums `fld st(1); fadd st(3); fadd st(1)` (running statements in all orders,
  GFSqr helper, grouped forms); one-line IsNonZero forms lose the shared squares and the `test ah,5` compare (338v350).
- 0x0086B9D0 GetRandomSkeletalMorph 389v389: same as the local model -- retail this ebp, map ebx, UpperBound result spilled; ours
  map ebp, end ebx. c2prio: map temp (prio 20) colours first; the ebx/ebp choice for it is the decision to move.
- 0x00AC7300 FindTextureTangent 1077v1091: the x87 tail (final scale spilled to the dead parameter slot, dead duplicate product,
  point copies through `mov eax,edx`) is unchanged by the port. 1 grade on the port.

## Not ported (regraded only) -- header debt
00408820 (CControlsDef has no header; CDefPointer handling), 0057aeb4, 00644677 (CDrawLetterBoxBlackBars no header), 0065c5bb,
00726980, 00774730, 007ce7a0 (CTCElectrocutedEffect no header), 007da640 (CTCTavernGameCardBlackjack no header; draft leaf not in its
object), 0083d2e0, 008bf500, 008cdd40, 009101c0 (draft declares Update QAE; PDB MAE), 00957db0 (CTCPerceiveThing has no retail
CTCBase branch), 0095b820 and 00acd960 (drafts use invented stand-in callees: CollectSink_95b820 / UpdateCostBody -- not landable
as they are), 009faf70 (CRenderTarget / CBase have no headers), 00b81820 (CEngineSimulationManager retail branch has no members
modelled). The two matches here say the GetTC / GetParentThing local models were the register-tie cause in their rows; 00957db0,
0065c5bb, 008bf500, 008cdd40, 00774730 and 0095b820 carry the same local GetInterface model and are the next candidates.

## Identities and names
- 0x00ACD960 is ?Optimise@COptimisedPrimitive@QuadricOptimiserInternals@@QAEXM@Z (the draft's SimplifyByVertexPairs is invented).
- 0x0086B900 is AddSkeletalMorphToMorphSet (the draft's CreateRandomMorphEntry is invented); 0x0086B9D0 is protected const (IBE).
- NAME_FIXES.tsv: 0x00658D5D IsCurfewTime is const (QBE); 0x00810FC0 is GetPHitBy (five wrong bound spellings listed).
