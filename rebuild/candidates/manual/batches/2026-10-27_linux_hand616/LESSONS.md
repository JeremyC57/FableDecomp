# Chunk 616 -- lessons from the authoring agent (fold into GOTCHAS at landing)

- Constructor that writes a 3x3 frame of floats (0/1 immediates, zero register reused): model it as three small vector members
  with a `(x, y, z)` constructor built in the init list. STransform-style aggregates with ctor-body stores give the wrong
  store order (0x00C11D00 CSoundEventManager ctor, RELOCATION_MATCH).
- When retail CSEs the squares of a 2D magnitude into a later inline Normalise, the register/spill order follows the
  compound-assignment order of BOTH inlines: `squared = Y*Y; squared += X*X;` and `lengthSquared = z*z; += x*x; += y*y;`
  (0x009718F0). Try the compound order of each inline separately; each fixes a different half of the diff.
- A dead accessor call in front of a static call (`call GetWorld ; ... call GetFrame`) is a separate statement in the source.
  `GetWorld().GetFrame()` inside the expression drops the GetWorld call entirely (0x007F5AE0: 529 v 544).
- Scratch-register rotation shifted by one over a whole tail: count the temporaries. A `bool found = a() && b();` adds exactly
  one and put 0x00871A80's rotation right, but it also let VC7.1 tail-merge the two identical release epilogues. Look for a
  temporary that does not change the control flow.
- STLport vector with a begin()-based operator[] and member-difference size() changes which register an indexed element
  load uses (0x0070D580 IsValid Anims[1]: 23 -> 19 lines). Model the real _Vector_base before calling it a lane gap.
- `xor eax,imm ; test al,imm` for a `(~f & m) == 0` mask test shows up only at inline sites inside large Update bodies.
  Out-of-line copies and every per-TU source form give `not eax`; probably an LTCG/link-time inline choice
  (0x00910DF0, 7 sites: 0x0090D030 x2, 0x0090E5D0, 0x00940BF0 x3, 0x00910DF0).
- A leaf that matches no symbol (public where the PDB says protected: QAE v IAE) silently grades another function. Check the
  access specifier against the PDB spelling before trusting an old grade (0x0077C960).

Review at landing:
- Rows over 4 grades: 00910df0 (~38), 00871a80 (13), 007f5ae0 (9), 0070d580 (~10), 007d5070 (~8). All parked.
- 00c11d00 lands under `??0CSoundEventManager@@...`. The caller 0x00C094A0 binds 0x00C11D00 as `??0CSteeringSolver@@QAE@PAUCSteeringGroup@@...`; respell it (NAME_FIXES.tsv) or vland reports RELOC_IDENTITY.
- 00c11d00's name is probable (libpk, no PDB): the landed dtor 0x00C13470 and vtable 0x012B2748 are the evidence.
- 0x00686D90 is one ICF-folded body for CTCBase::GetThingSearchTools and PeekThingSearchTools (both in the PDB). Binding one symbol to it needs a coordinator decision.
- 0x0049D870 is an ICF-folded getter bound to several names (CWorld::GetFrame SIJ / SAJ, GetQuakeFrame, g_preMainMemoryUsed). It needs one canonical name.
- Local base models next to an existing header: 0077c960 (CTCHighlightItem.h), 00589a30 (CTCInventory.h), 004a34f0 (CWorld.h); 007f5ae0's local polymorphic CTCBase (the generated CTCBase.h cannot be a polymorphic base).
