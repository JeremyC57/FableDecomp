# Hand chunk 2042 (CTCLightningNode .. CTCLightningNode): 0 of 1 byte-exact

## What matched (0)
None.

## Parked for Fable 5.1 (1)
- 007f2dc0 CTCLightningNode::FrameUpdate DIFFER(1929v1942): retail keeps a frame copy of Timer (0x007F2DE6 `mov eax,[ebx+28h]`, 0x007F2DEF `mov [esp+18h],eax`, 0x007F2DFA `fild [esp+18h]`), ours `fild [ebx+28h]`; that missing node swaps frame words [esp+18h]/[esp+1Ch] (24 operand differences); 0x007F2E9A `mov [esp+5Ch],eax` scheduled after `fld [ecx+3Ch]` (ours before); alignment pads 0x007F301D / 0x007F318E follow from the size | tried: see "Timer" below (16 spellings, all forwarded) | next: find the construct that keeps a `long` copy of Timer in the frame without volatile (an address-taken copy: e.g. a helper taking `const int&` bound to the long member, which forces a converted temporary -- no such helper found in either PDB; or a second, dead def), then regrade; the frame swap and the residue sizes then follow (proved by the volatile-member diagnostic g5).

Source: differ/007f2dc0.cpp (line 2 = the exact residue); test differ/007f2dc0.test.cpp (PASS under g++ and VC7.1).

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
