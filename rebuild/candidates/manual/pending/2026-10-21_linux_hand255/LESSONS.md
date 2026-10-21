# Chunk 255 -- lessons from the authoring agent (fold into GOTCHAS at landing)

- **Inline stream Read, chunk test (0x008707A0).**
  - Write the fast-path test `if (size <= m_remaining)` to get `cmp size,[+14h] ; jg overflow`.
  - The landed 0x007AD7F0 form `m_remaining >= size` gives `cmp [+14h],size ; jl`.
  - With a constant size, both forms compile to the same `cmp edx,4 ; jl`. So only a variable-size Read tells them apart.
- **Quest-script picks with several stages (0x00E23F30).**
  - Each stage picks, does its bookkeeping and assigns `speech = table[index]`, then `break`s to one shared tail after the loop: `if (IsDone()) return none; return speech;`.
  - VC cross-jumps the `push &line ; jmp` into one operator= call.
  - Returning inside each stage duplicates the tail (+57 bytes).
  - The loop is `for (;;) { if (IsDone()) return none; ... yield; }`. That gives the rotated `jne` entry test and the bottom `IsDone ; je loop`.
- **Nested quest-script classes.**
  - Ego_r.pdb can hold two classes with the same nested-script name (CTortureDoorCourtyard). Pick the right one by the ParentClass field offset that is read (+0x67 here).
  - A landed sibling in the same TU (0x00DD8990) is then a near-copy: 0x00DD8BA0 matched on the first grade.
- **Return a fallback copy as float locals (0x00760C40).**
  - Retail kept the first point as `float x = it->X, y = it->Y, z = it->Z;` and returned `C3DVector(x, y, z)`. The shape is Y on the FPU stack across the loop, with X and Z int-copied.
  - A `C3DVector first = *it;` copy is all int moves.
- **Loop-invariant reciprocal (0x00760C40).**
  - VC7.1 hoists `reached / TrackLength` inside a loop into a preheader `1.0f / TrackLength` reciprocal, placed after the empty-loop guard, and multiplies.
  - Do not hand-write the reciprocal.
- **Post-increment iterator (0x00760C40).** `const T& point = *it++;` inside `for (++it; it != end; )` gives retail's `mov esi,edx ; add edx,0Ch` (current/next pair).
- **Float compare direction.** The found arm out of line behind `test ah,5 ; jp` means the source tests `<` for the continue arm and has `else return ...;`.
- **Lane gap candidate: FPU enregistration of struct fields (0x00876CA0, 0x00760C40).**
  - Retail sometimes keeps two members of a copied C3DVector on the x87 stack (`fld x ; fld y`, Z int-copied) and packs frame slots tighter.
  - No source form (named copy, temporary, by-value operator argument) reproduced it here.
- **Lane gap candidate: out-pointer homed in a dead argument slot (0x006AD580).**
  - Retail homes the `GetTCDriver(&orb)` out-slot in the dead `type` argument slot.
  - The lane gives it a fresh frame slot whether the call is inline or in a helper.

Review at landing:
- Several parked rows (006ad580, 006ac200, 0083d2e0, 00986ed0, 00987040, 00760c40, 00876ca0) got more than 4 grades in total, spread over distinct strategies.
- Local models next to existing engine headers:
  - CHeroMarriageDef and CAIScratchpadDef: the headers are flattened pack(1) PDB views with no base class.
  - CThingMapManager: header at PDB offsets, which are wrong for retail. Its rows are parked.
  - CThingCreatureBase, CThingPhysical and CThingPlayerCreature: CThing sits 0x10 below the PDB in retail. The derived pad must start at 0x94, because a model ending in bitfields at 0x91/0x92 rounds to 0x94.
- 0x00DD8BA0's parent class is CQ_PrisonEscapeRescueMotherScript. Its flag at +0x67 is named MotherRescued after the landed 0x00DD8990. By the PDB drift (retail CQ base 0x14 smaller) +0x67 is PDB +0x7b = MotherFollowing, so the landed name may be wrong.
- Landed 0x007AD7F0 comments that its Read has no `size > 0` test. 0x008707A0 shows the variable-size caller does `if (bytes > 0)` itself.
- g++ test of 0x008707A0: `long` is 8 bytes on the 64-bit host, so the test maps `long` to `int` and `__int64` to int64_t, and routes memcpy to a memmove shim, all under `#if !defined(_MSC_VER)`.
- Lander checks from the brief, applied to the matched rows: free() for 0x00BFEA14 (0x00522720); the const LowerBound copy route for 0x004365B0; GetPosition 0x004C73D0 / 0x006A4D00 through static_cast<const CThing*>.
