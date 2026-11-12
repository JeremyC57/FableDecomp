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

Review at landing:
- 0x0097BDA0 is parked (DIFFER 2025v2009) after a continuation pass (1 more grade, g09); the residue is one allocator decision plus
  three scheduling / x87 operand-order items (NOTES.md continuation section, ranked ideas). Needs the overlay headers in hdr/ (CScorpionKingBattleDef.h,
  NEntityEvents_CEventCreatureCharging.h and the opt-ins in hdr/README.md).
- Landed 0x0097C580 is named SpawnBabies but is UpdatePhase2 (NAME_FIXES.tsv); landed 0x008C4E20 ChargeInto ctor binds a pointer/long
  spelling where Ego_r has references/float.
