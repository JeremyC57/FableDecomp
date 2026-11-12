# Chunk 2034 -- lessons from the authoring agent (fold into GOTCHAS at landing)
- A do-while wait whose condition is `(GetWorld().GetFrame() - start) / CMainGameComponent::GetConstantFPS() < 2` loads ConstantFPS into a
  callee-saved register before the GetFrame call (`idiv edi`) and loses the aligned loop head; `frames_waited = GetFrame() - start;` at the
  end of the body with `while (frames_waited / GetConstantFPS() < 2)` gives retail's `idiv dword ptr [ConstantFPS]` and the `lea esp,[esp]`
  padded head (0x0097C50C).
- `if (a && !(Health <= HL - D))` gives `test ah,1 ; je` (retail 0x0097C248); `HL - D < Health` and `Health > HL - D` both give
  `test ah,5 ; jp` in that position.
- A same-TU method with an EH temporary is never auto-expanded at a deep site (c2 score 2348 v T74), and forcing it inline evaluates its
  inner inline sites at frequency 1.0 (the temporary's dtor expands): a retail site that expands the method body but CALLS the inner dtor
  is open code, not a forced helper (0x0097C33E / 0x0097C351).
- The by-value `CIntelligentPointer<const CThing>` argument of AIAction_LookAtThing built from a CIntelligentPointer<CThingCreatureBase>
  member splits by itself with the CONVERT_MUTABLE inline body: called (0x0091F990) at the f 0.0117 site, expanded at f 0.1875.
- Frame packer: equal-weight 4-byte nodes are joined to the LATEST non-conflicting slot, so one extra IL reference on a long-lived pointer
  (CTCBoss* boss, weight 5) moves a later float (radius) from the pointer's slot into an earlier temporary's slot; dropping one boss use in a
  diagnostic reproduced retail's slots exactly (0x0097BDA0).
- FableWin.pdb's COMDAT run after a function lists the inline helpers it references in first-use order (here SetCountingStrikesSinceHitHero,
  Left turn ctor, Right turn ctor, PrepareTailPlunge ctor, CEventCreatureCharging ctor) -- also evidence for which inline ctors a row expands.

- (continuation) A forced-priority what-if (hand883 c2pf `C2_FORCEUID=<creation ordinal>=<prio>`, copy in tools/c2pf/, wrapper
  tools/fw.sh) proves whether ONE allocator decision is the whole residue: forcing the boss near piece of 0x0097BDA0 to prio >= 13
  gave 2009 bytes and retail's frame exactly. Frame-packer weights are counted after allocation (a piece in a register drops its
  references from the home's weight), so a frame-only residue can be the shadow of one register decision.
- (continuation) A battle_def piece whose only allowed register is ebx takes it even at a 200 neighbour cost; the chooser has no
  refusal threshold -- the colouring ORDER (priority, then key) decides (0x0097BDA0 near arm).
- (continuation) `const C3DVector& facing = ...GetFacingVector();` (lifetime-extended temporary) gives a different x87 term order
  of a following facing.Dot(...) than copy-initialisation (Z,Y,X v X,Z,Y; 0x0097BFF7); `a.Dot(b)` and `b.Dot(a)` compile the same.
- (continuation) Frame-masked instruction counts hide x87 operand-order residues (`fld [esp+30h] ; fmul [esp+20h]` v the swap):
  always look at the unmasked md diff before calling a row "allocator only".
- (tool) hand883 pblk.py labels flag-16 block entries "live"; they are referencing blocks (the piece's first/def block).

- (pass 3, 0x0097BDA0 matched) An allocator residue that ~60 traces and the forced-priority what-if had pinned to ONE colouring decision
  (battle_def near piece v boss for ebx) was closed by the if / else ORDER of a later arm: the far `if (charge ok) charge else tail plunge`
  written `if (ChargeCount >= N || Health <= (HL - D)) tail plunge else charge`. Grade the r3009 arm swaps on every if / else in the function,
  not only around the block that differs.
- (pass 3) x87 `fld a ; fmul b` with both factors in memory: VC7.1 loads the factor whose c2 field record id is LARGER (agent3010 recs2.py +
  fld.py show the ids, 14 s, free). Field records are created early when the outer function names the field (`facing.X`), late when reached
  only through inlined members (`facing.Dot(..)`, `CrossWithUp()`), and a lifetime-extended `const T&` temporary gets all its field records
  late. So a mixed per-product load pattern = some fields named in the outer source: 0x0097BDA0's side test is
  `to_enemy.Dot(C3DVector(facing.Y, -facing.X, 0.0f))` with a by-value facing (retail's `fmul 0.0f` on to_enemy.Z at 0x0097C0A0 shows the Z = 0
  vector). Dead-local rulers move only the TERM order (period 8), never these loads.
- (pass 3) A parenthesised `(HL - D)` on the right of `Health <= ...` moves the creature-pointer load ahead of `fld [esi+3Ch]` (0x0097C15A /
  0x0097C234); NORMALISE_UNPAREN splits Normalise's two `fstp st(0)` pops around the exponent shift (0x0097BE80 / 0x0097BE85).

Review at landing:
- 0x0097BDA0 RELOCATION_MATCH (pass 3). Needs the overlay headers in hdr/ (CScorpionKingBattleDef.h, NEntityEvents_CEventCreatureCharging.h and
  the opt-ins in hdr/README.md); uses the existing repo opt-in FABLE_RETAIL_C3DVECTOR_NORMALISE_UNPAREN. NAME_FIXES.tsv adds 0x0091F990 (landed
  local-model spellings of the converting CIntelligentPointer<const CThing> ctor).
- Landed 0x0097C580 is named SpawnBabies but is UpdatePhase2 (NAME_FIXES.tsv); landed 0x008C4E20 ChargeInto ctor binds a pointer/long
  spelling where Ego_r has references/float.
