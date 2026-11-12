# Hand chunk 2042 (CTCLightningNode .. CTCLightningNode): 1 of 1 byte-exact

## What matched (1)
- CTCLightningNode::FrameUpdate 0x007F2DC0 RELOCATION_MATCH (1942 bytes, src/007f2dc0.cpp, test/007f2dc0.cpp PASS g++ + VC7.1; 10 grades).
  Expire / spread / feed-branches update of a lightning node. Shape lessons that closed it (continuation, g08-g10):
  (1) `if (PDef.IsNull()) return; long timer = Timer; const bool has_life_time = PDef->LifeTime > 0.0f; if ((has_life_time && timer > ...`
  -- the materialised bool puts a control-flow join between the copy and its use, so VC7.1 keeps the copy in a frame word (retail
  0x007F2DE6..0x007F2DFA) instead of forwarding `fild [ebx+28h]`; that one frame node gives retail's whole frame. `const` (not a plain
  bool) keeps the X, Z, Y squaring order of the range test. (2) `float range = PDef->Range;` before the C2DBoxI: a load through a named /
  forwarded value has no may-alias edge to the address-taken C2DCoordI temporary's Y store, so `fld [ecx+3Ch]` issues before it (0x007F2E9A).
  Descriptive local names: timer, has_life_time, range (plus the authoring agent's locals).

## Parked for Fable 5.1 (0)
None (the authoring agent's parked residue is closed; history below).

## Continuation (near-miss agent, grades 8..)
- g08 probe/n02.cpp DIFFER(1942v1942): `if (PDef.IsNull()) return; long timer = Timer; bool has_life_time = PDef->LifeTime > 0.0f;
  if ((has_life_time && timer > ...)` -> RESIDUE 1 SOLVED (retail prologue + whole frame). Lever (measured in fragments probe/frag2/b.cpp,
  c.cpp, and found in the landed CTCHeroExperience::IsToAutoBlock 0x006D6CB0): c2 forwards a single-use copy of a member only when no
  control-flow JOIN lies between the def and the use; a materialised bool (`bool b = x > 0.0f;` = branch + join) between `long timer =
  Timer;` and the use keeps the copy in the frame (def scheduled into the bool's block: `fld; mov eax,[ebx+28h]; fcomp; mov [esp+18h],eax`).
  New residue in g08: x87 squaring order of the range test (ours Z,Y,X; retail X,Z,Y) -- not the outer-name count (dead-local sweep k=1..7:
  no change); `const bool` (va) or `(x > 0.0f ? true : false)` (ve) restore X,Z,Y; plain bool / bool declared earlier / float life_time
  local do not (tools/var.py peeks).
- g09 probe/va.cpp DIFFER(1942v1942), equal-instr 580/581: ONLY residue 2 left: 0x007F2E9A `mov [esp+5Ch],eax` retail after
  `fld [ecx+3Ch]` (ours before it, between `mov ecx,[ebx+24h]` and the fld).
- Residue 2 lever (scheduler DAG, tools/sched.sh region 9 of FrameUpdate): the Y store into the C2DCoordI temporary (address-taken:
  passed to the out-of-line C2DBoxI ctor) had a may-alias edge to `fld [ecx+3Ch]` (a load through the expression `PDef->`), so it could
  not move below it. A load through a NAMED value has no such edge: fragments probe/frag2/f.cpp R1 (`const CLightningDef* def`), R3
  (`const CLightningDef& def = *PDef`), R4 (`float range = PDef->Range;`, forwarded to its use) all give retail's order.
- g10 probe/r4.cpp (va + `float range = PDef->Range;` before the area) RELOCATION_MATCH 1942v1942, 581/581.

## Row 0x007F2DC0 details
- Extent: `push ebp` .. `ret` at 0x007F3555 = 1942 bytes (the oracle length is right; the reservation estimate 2323 includes this
  TU's unreferenced inline COMDATs 0x007F3560..0x007F36D0: getters, IsNull, vector::empty (0x007F35D0), vector<8-byte>::size,
  C3DVector Set / ctor, pair ctor, allocate, CCountedPointer copy ctor). No SPLITS entry.
- Name: Ego_r.pdb 0x007EC6B0 + FableWin.pdb 0x027AD000 ?FrameUpdate@CTCLightningNode@@UAEXXZ; rtti-vtables 0x0124337C slot 9.
- Descriptive names (no PDB layout record exists): CTCLightningNode::CLightningBranch members PTarget (+0), PEffect (+8), Timer (+0x10);
  locals are free names.
- Header debt: none of the generated headers were used for CLightningDef (quarantined flat copy); see hdr/README.md for all overlays.

## Grades (7 used: pass 1 = 4 + 1 diagnostic, pass 2 = 2, pass 3 = 0)
| # | source | result | what it measured |
|---|---|---|---|
| 1 | probe/p01.cpp | DIFFER(1833v1942) | first draft; inline splits wrong (ToC2DCoordI called, IS_THING_ALIVE / CIntelligentPointer ctor+op= expanded in the branch loop, Insert called) |
| 2 | probe/p03a.cpp | DIFFER(1937v1942) | `target == 0 || target->IsDead()` fixes the loop's inline split but cross-jumps the two erase calls |
| 3 | probe/t1.cpp | DIFFER(1948v1942) | parent test spelled as four `||` terms + forced Insert + direct C2DCoordI + while things loop: all 75 calls pair with retail |
| 4 | probe/p04.cpp | DIFFER(1929v1942) | + do-while things loop, named branch_effect reference, named difference vector, `long timer = Timer` first: registers equal retail; residue = Timer copy + frame swap + 1 store + pads |
| 5 | (diag) probe/diag_mvol.cpp + tmp/diaghdr (volatile Timer member) | DIFFER(1950v1942) | NEVER delivered: with the Timer copy kept, the frame equals retail; left only the volatile `++Timer` artefact (x2) and the 0x007F2E9A store |
| 6 | probe/m3.cpp | DIFFER(1929v1942) | `if (!things.empty())` guard (FableWin COMDAT list) with the iterator before GetDef: same bytes as g4 |
| 7 | differ/007f2dc0.cpp (= m4: + GFDebugPDynamicCast for the AddTC result) | DIFFER(1929v1942) | final confirmation (code-identical to m3) |
Pass 2 / pass 3 were not spent further: every remaining spelling idea was checked first with free compile-only peeks
(tools/peek.sh) and none changed the measured decision (the Timer copy is forwarded), so a grade could not move the row.

## Free measurements (commands in tools/)
- Inline decisions: `HDR=hdr bash tools/inl.sh <src> <tag>` (c2 0x1078D1AA; score = c1*(1/f+6)/7, expanded iff < 74). Retail split:
  IS_THING_ALIVE(PParent) expanded, IS_THING_ALIVE(PSourceOfFaction / branch effect / PSourceOfHit) called, CIntelligentPointer(T*) and
  operator= called, Insert expanded (forced: plain inline scores 191), C2DBoxI ctor called.
- Tuple count: tools/pk/tc.py (hook c2 0x1077BE49): 814 -> regime 0.
- Frame packer: tools/pk/run.py <src> <tag> FrameUpdate@CTCLightningNode --nocmp, replay tools/sim2.py tools/pk/p04.json --w n=w:
  +2 weight on the [esp+18h] group (the timer node) restores retail's order.
- Call pairing: tools/calls.py; relocation names vs reloc_bindings checked (NAME_FIXES.tsv).
- Timer spellings (all folded into `fild [ebx+28h]`): named long after / before the null test, `long timer; timer = Timer;`, int,
  const, `(void)&timer`, `(float)timer` with parenthesised product, reversed compare, reversed && order, `long fps` local, `float
  life_time` local, `const CLightningDef& def`, bool `expired` (two forms), reuse of the local for the branch timer; fragments
  (probe/frag/f2.cpp, f3.cpp): inline helpers with by-value / const-reference params, do/while(0) block, `const long&` alias to the
  member. Kept only by: volatile (member or local -- wrong increment / extra reload, and not allowed), a named reference to the copy
  (unnatural), or reusing the address-taken `effect` local (wrong frame).
- 0x007F2E9A store: unchanged by a named centre (moves the frame), forced ToC2DCoordI (diag), implicit long conversions, parentheses,
  const / direct-init position copy.

## Ranked untried ideas
1. Trace c2's copy propagation (the pass that drops `timer` before colouring: it is absent from c2prio's candidate list) to find which
   condition keeps a copy without address taking.
2. Look in EgoCore / FSE / sibling sources for a Lionhead expiry idiom (a macro or helper around Timer / LifeTime) whose expansion
   address-takes or re-defines the copy. (A `const int&` helper bound to the long member was tried in probe/frag/f4.cpp: forwarded too.)
3. The 0x007F2E9A store: replay the scheduler region (agent2031 tools/sched.sh) once the Timer copy exists -- the region numbering changes.

## Skipped
None.

## Label corrections
See NAME_FIXES.tsv (GetFocalPos 0x004C1060, CParticleUpdateParameters ctor 0x006E24D0, the three GetAllThingsInRoughArea instantiations
0x007F4020 / 0x007F42A0 / 0x007F4500, erase 0x007F3E90, CCountedPointer ctor 0x007F3F60, CLightningBranch ctor 0x007F3AD0, the three setters).
