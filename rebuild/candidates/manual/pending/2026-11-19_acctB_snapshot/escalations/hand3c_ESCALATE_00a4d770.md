# ESCALATE 0x00A4D770 C3DAnimation2::CalcLocalTransformationsRelative -- waiting for advisor candidates (not parked)

## Best source and grade count
- src/00a4d770.cpp: DIFFER(420v420), ONE instruction. ATTEMPTS.log: 42 grades; plus ~15 no-ATTEMPTS probes this session (probe/a4/).
- Overlays (rebased, = today's repo + opt-in): C3DAnimation2.h FABLE_RETAIL_C3DANIMATION2_CALCLOCALRELATIVE, C3DMesh2.h
  FABLE_RETAIL_C3DMESH2_GETBONEKEYFRAME.
- Shape: FableWin 0x03227CA0 (/Od): ConvertTimeToFrames(t, frame, fraction); for c_seq < SequenceCount { C3DKeyframe kf_rel;
  GetKeyframeInterpolateFixPos(c_seq, kf_rel, frame, fraction); dest[c_seq] = *mesh.GetBoneKeyframe(c_seq); }. FableWin's obj
  order confirms the callee set: the COMDATs first referenced by this function are #58 C3DMesh2::GetBoneKeyframe and #59
  C3DKeyframe::C3DKeyframe (no operator=; PostApplyRelativeAnimation is first referenced by #60, not by this row).

## Exact residue
| addr | retail | ours |
|---|---|---|
| 0x00A4D8E4 | mov edx,[esp+30h] (dest) | same |
| 0x00A4D8E8 | add esi,ebx (BoneKeyframes + iv) | same |
| 0x00A4D8F1 | **lea edi,[edx+ebx]** (dest + iv) | **lea edi,[ebx+edx]** (iv + dest) |
Everything else (registers, frame, schedule) is identical; ebx = the strength-reduced c_seq*0x30 IV, ebp = c_seq.

## Measured (c2prio_op, tools/c2/, this session)
- Candidates: dest = param id 8 (split, all memory: reloaded each iteration), IV = compiler temp id 40 (the last temp; ebx).
  hand892's rule "commutative pair: higher candidate id first" predicts ours (40 before 8). A named parameter cannot outrank a
  compiler temp, so retail's dest-first needs either a dest piece with a new (high) id or an IV that is not the last temp.
- Diagnostic dg3 (a dummy dest use after the loop) prints dest first, but only because the IV moved to ebp ([edx+ebp]
  encoding); dest stays id 8 / memory. Not a lever.
- Diagnostic d1 (ConvertTimeToFrames call replaced by stores): same ids, same lea (the older note that it flips does not reproduce).

## Tried (all keep the lea; probes in probe/a4)
Earlier: 5 copy spellings (index, pointer add, reversed index, ref local, memcpy), 4 loop spellings, inline depth, SetPosition +
C3DKeyframe CTOR opt-ins, CalcLocalTransformations defined ahead, body after the row, /Oa /GS /EHsc, QFE-4035, include order.
This session: copy before the interpolation (93 diffs), kf_rel outside the loop, ++c_seq, c_seq declared before the loop, while
loop, GetBoneKeyframe(c_seq)[0], long c_seq, an extra `c_seq < dest_size` bound (440 bytes).

## Top 3 untried hypotheses
1. The IV inherits an earlier temp (lower id) instead of being the last temp: spellings that create the c_seq*0x30 index in IL
   before the inlined FixPos temps (ids 30-39) without moving code, e.g. an inline accessor returning a reference.
2. dest reached through an expression whose temp outranks the IV (an inline helper with a C3DKeyframe* parameter), measured with
   c2prio_op on the lea operands.
3. A c2 what-if (hand885 pk/wi.py) forcing dest's / the IV's id to prove which one flips the lea, then search the spelling.

## Advisor round 1 (2026-11-19), results
- Step 3 (no compile): FableWin ?GetBoneKeyframe@C3DMesh2@@QBEPBVC3DKeyframe@@K@Z = const member, unsigned long index, returns
  const C3DKeyframe* = the overlay. Ego_d: c_seq unsigned long (0x22) in its own scope, frame long (0x12), fraction float, kf_rel in
  the inner scope; dest_size unsigned long; Ego_d / FableWin body 461 bytes. No type mismatch to try.
- Frame arithmetic: retail `sub esp,1Ch` + 4 pushes -> return address at [esp+2Ch], so [esp+30h] in the loop is dest's own argument
  slot (and ConvertTimeToFrames writes `frame` into t's dead slot, [esp+34h] at the call). The slot can equally hold a temp packed
  into the dead parameter slot: the bytes do not tell dest from a copy of it.
- Step 2 cross-check (retail scan: `lea edi,[r+r]` before `rep movsd`, intersected with landed rows): landed 0x004E9BDA
  CopyPtrs<CShopItemDef> has retail `mov eax,[ebp-4] ; lea edi,[eax+ebx]` (memory-reloaded value first, IV ebx second). c2prio_op on
  the landed text: ebx = IV temp id 7, the memory value = temp id 9 (the strength-reduction difference temp `result - first`, created
  AFTER the IV). So the rule holds there (higher id first) and the memory base is a later compiler temp, not a parameter.
  => For this row, retail's [esp+30h] value is most likely a compiler temp created after the IV (packed into dest's dead slot).
- Pointer-advance spellings (`*dest++ = ...`, `dest++` in the step / body; probes pi1..pi4) keep dest as its own IV (426 bytes): not
  the lock-step shape. The what-if (forcing ids) is not run yet: ids index c2's bit sets, so overwriting +0x1C needs care.
