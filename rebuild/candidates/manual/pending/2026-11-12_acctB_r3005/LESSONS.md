# Chunk 3005 -- lessons from the authoring agent (fold into GOTCHAS at landing)

- (0x00A9EB90, c2 mechanism) c2's symbol records (temps, locals, params, globals) come from a pool of 32-record chunks numbered at chunk
  creation (c2.dll 0x107603DD, counter 0x108A79D0) and reused through a free list. The base / index order of a two-register address
  `[a+b]` follows the operands' record ids MOD 1024 (the smaller one becomes the SIB index): ours to-IV 1021 / offset 1064 (-> 40) gives
  `[edx+eax]`, retail's `[eax+edx]` needs the IVs past 1024. A "per-function key" that moves with dead code is often this: count of
  records alive at the deciding pass, quantised by 32-record chunks, read mod 1024 / 256.
- (0x00A9EB90) Which records count depends on WHEN they are allocated: c1 temps / outer names (read before inlining) shift everything,
  records made during an inline expansion (SetSlerp, GFFastSin bodies) shift only what is created later (loop strength-reduction temps).
  Pure dead arithmetic on a fresh local is removed before it counts; CSE'd duplicates and float-to-long conversions do count.
- (0x00A9EB90) Measure such ties for free: tools/c2h/cands.py prints every candidate with its record id, sib.py the address-node
  operand order, recs.py every record id with its name; read the deciding instruction from the trace object (tools/my/k.py), never diff
  against retail in a sweep.
- (0x00550DC0) The r3004 outer-name lever does not touch a frame-packer tie: k = 1..16 dead outer names moved only a member-product
  operand order (period 8). Check which kind of tie a residue is (operand order vs packer list order) before sweeping names.
- (0x00550DC0) FableWin.pdb has CRGBFloatColour's product as a const member (??DCRGBFloatColour@@QBE?AV0@ABV0@@Z); the repo header models
  a free operator*. Byte-neutral here; worth a check when the header is next merged.

Review at landing:
- Both rows parked; prior differ sources unchanged except the `// Grade:` line (differ/), their tests pass under g++ and VC7.1.
- 0x00A9EB90: the row may match once the original nesting of the slerp helpers is known (one chunk of extra records during the
  scalar-loop inlining); the shared GFFastSin / GFFastArcCos / CalculateSlerpWeights bodies are row-local copies in three landed rows and
  this one -- a single shared header for them is still owed (chunk 3001).
