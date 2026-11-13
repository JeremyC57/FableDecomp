# OUTLINE 0x00AA2110 C3DAnimationManager::CalcBlendedBoneTransformation (4556 B, `ret 1Ch`)

## Frame facts
- `push ebp ; mov ebp,esp ; and esp,-16 ; sub esp,214h ; push ebx ; push esi ; push edi` -> saved order ebx, esi, edi; frame
  0x214 (+3 saves = 0x220, so post-push esp is 16-aligned). Args via ebp (+8 start_blend, +0Ch end_blend, +10h blend, +14h mesh,
  +18h bone, +1Ch transform, +20h scale). Epilogue 0x00AA32D3 `pop edi/esi/ebx ; mov esp,ebp ; pop ebp ; ret 1Ch`.
- No EH states (no /EHsc), no /GS cookie. Register homes: ebx = this in most of part 1 (also kept in [esp+3Ch]); edi =
  &BoneIndicesBuffer at the start; ebx = fraction bits in the two "full weight" loops.

## Frame map (post-push esp offsets)
| slot | holds |
|---|---|
| +10h, +14h | float temps (GFFastArcCos/GFFastSin `fst/fistp` staging; scale1/sinom); +14h = `prev` matrix pointer homed for the SSE kernel (part 3) |
| +18h | `alpha` (interpolated alpha weight); later float temp (quat->matrix) |
| +1Ch | `count` = BoneIndicesBuffer.size() |
| +20h | resize temp `unsigned long()` = 0 (0x00AA212F) -> push_back value `parent` (0x00AA2166) -> blend down-counter (0x00AA2268) -> float temp -> SSE `dest` ptr |
| +24h | `frame` (long, ConvertTimeToFrames out) -> later float temp |
| +28h/+2Ch | CCountedPointer<const C3DAnimation2> `animation` (Data, Info) per blend iteration; reused for CCountedPointer<const C3DSkeletalMorphTarget> `target` |
| +30h | sinom / scale0 temps |
| +34h | `keyframes` (C3DKeyframe*) |
| +38h | blend byte offset i*20 (strength-reduced PeekCombinationBlend(i)) -> later float temp |
| +3Ch | this |
| +40h | sign (BlendWith) / float temp / SSE `&local` pointer |
| +44h | omega; +48h/+4Ch fistp ints / float temps; +70h fistp int, later matrix-loop counter (count-1) |
| +50h..+5Ch | CQuaternion `rotation` of PostApplyRelativeAnimationAlpha (SetSlerpFromIdentity result); part 3: copies of kf[i].Position X/Y/Z (+50/+54/+58) |
| +60h..+6Ch | CQuaternion `rotation` of BlendWith; +64h/+68h/+58h part-2 int->float staging of Position.Y/Z |
| +74h cosom, +78h `fraction` (float, ConvertTimeToFrames out), +7Fh __true_type tag (push_back overflow), +80h/+84h/+88h fistp ints, +8Ch sign |
| +90h..+CFh | `__declspec(align(16)) CMatrix4x4 local` (row 3 = position,1) |
| +D0h..+DCh | C4DVector temp (Position.Interpolate result, BlendWith) |
| +E0h..+11Fh | `__declspec(align(16)) CMatrix4x4` A (first result: keyframes[0].ToMatrix) |
| +120h..+14Fh | C3DKeyframe `partial` (ApplyPartialAnimationAlpha) |
| +150h..+17Fh | C3DKeyframe `relative` (ApplyRelativeAnimationAlpha) |
| +180h..+18Ch | C4DVector temp (other_position * alpha) |
| +190h..+1ACh | C4DVector temps (Interpolate: other - this, * t) |
| +1B0h..+1EFh | `__declspec(align(16)) CMatrix4x4` B |
| +1F0h..+21Fh | C3DKeyframe `relative` (ApplyRelativeAnimation) |

## Block-by-block (retail order)

```cpp
void C3DAnimationManager::CalcBlendedBoneTransformation(const C3DAnimationBlendState& start_blend, const C3DAnimationBlendState& end_blend,
    float blend, const C3DMesh2& mesh, unsigned long bone, CMatrix3x4& transform, C3DVector& scale)
{
  // 00AA211F  BoneIndicesBuffer.resize(1);                 call 0x00464931 (vector<unsigned long>::resize(1, unsigned long()=0), out of line)
  // 00AA213C  BoneIndicesBuffer[0] = bone;
  //           long parent = mesh.GetBone(bone)->GetParent();      (Bones +9Ch, stride 3Ch, Parent +4)
  // 00AA2160  while (parent >= 0) {                                (loop top 2160, test at 21A0 `jge 2160`)
  //               BoneIndicesBuffer.push_back(parent);             inline fast path 2160..2175; slow 2177 call 0x00464ACE _M_insert_overflow(finish,x,__true_type,1,true)
  // 00AA2190      parent = mesh.GetBone(parent)->GetParent(); }
  // 00AA21A4  unsigned long count = BoneIndicesBuffer.size();      ([esp+1Ch])
  //           __int64 keyframe_count = count * 5;
  //           if (keyframe_count < 0 || keyframe_count > 0x7fffffff) return;   -> 00AA32D3 (skips FreeAll) -- 0x00AA0090's text
  // 00AA21DD  C3DKeyframe* keyframes = MemoryPool.AllocateType<C3DKeyframe>((unsigned long)keyframe_count);  call 0x00A5C910 at 2201; null at 2208
  // 00AA220E  CombineBlendedBaseAnimationsBoneList(start_blend.PeekBaseAnimation(), end_blend.PeekBaseAnimation(), blend,
  //               keyframes, count, 0, mesh, BoneIndicesBuffer);   call 0x00AA0DC0 at 2235
  // 00AA223A  unsigned long blend_count = start_blend.GetCombinationBlendCount();   (Data null -> 0; /20)
  //           if 0 -> 00AA2AA0
  // 00AA2270  for (unsigned long i = 0; i < blend_count; ++i) {    (down-counter [esp+20h], offset [esp+38h] += 14h at 2A8E; jne 2270 at 2A9A)
  //   const C3DAnimationStateAlpha& start_state = start_blend.PeekCombinationBlend(i);  (esi)
  //   const C3DAnimationStateAlpha& end_state   = end_blend.PeekCombinationBlend(i);    (edi)
  //   const C3DAnimationInfo* info = MeshBank->PeekAnimation(start_state.GetAnimationIndex());   call 0x00A242C0 at 2291
  //   CCountedPointer<const C3DAnimation2> animation = info->GetAnimation(mesh);             call 0x00A26C60 at 22A1 (-> [esp+28h])
  //   long frame; float fraction;
  //   float time = GFGetInterpolatedValue(start_state.GetAnimationTime(), end_state.GetAnimationTime(), blend);  22A6..22C2
  //   animation->ConvertTimeToFrames(time, frame, fraction);                                 call 0x00A52650 at 22C7
  //   float alpha = GFGetInterpolatedValue(start_state.GetAlphaWeight(), end_state.GetAlphaWeight(), blend);    22CC..22E8 -> [esp+18h]
  //   (the whole head = 0x00AA0090's text, named start_time/end_time/start_alpha/end_alpha locals)
  //   if (animation->IsRelative()) {                              22DD `shr ecx,2 ; test cl,1 ; je 2685`
  //     if (fabsf(alpha - 1.0f) < 0.0001f) {                       22F4..2311 `jp 23B9`
  // 00AA2330  for (bone_i = 0; bone_i < count; ++bone_i)       (edi=bone_i, esi=&keyframes[bone_i].Position.Z; ebx=fraction)
  //           ApplyRelativeAnimation(keyframes[bone_i], *animation->PeekSequence(BoneIndicesBuffer[bone_i]), frame, fraction);
  //             -> if (!seq.IsIdentity()) { C3DKeyframe relative(+1F0h); seq.GetKeyframeInterpolate(relative,frame,fraction) call 0x00A4C1F0 at 2359;
  //                keyframe.PostApplyRelativeAnimation(relative): Rotation *= (call 0x00A88C10 at 2369); Position += (4 fadds 236E..23A1) }
  //           BoneIndicesBuffer is re-read through [esp+3Ch]+8 each iteration (this not in a register here).
  //       -> 2A56
  //     } else {
  // 00AA23D0  for (...)  ApplyRelativeAnimationAlpha(keyframes[bone_i], *animation->PeekSequence(BoneIndicesBuffer[bone_i]), frame, fraction, alpha);
  //             IsIdentity js -> 2670; GetKeyframeInterpolate(relative +150h) call at 23FB; SetSlerpFromIdentity inline 2400..25BE
  //             (cosom<0 flip 2418..243E; 1-cosom>0.05 test 2442..2457 else 25AC; GFFastArcCos 245D..24B8; sqrt 24CA; two GFFastSin 24D0..25A6);
  //             q -> +50h; Rotation *= q call 0x00A88C10 at 2607; Position += relative.Position * alpha (260C..266D)
  //       -> 2A56 (jmp at 2680)
  //     }
  //   } else if (animation->IsPartial()) {                         2685 `shr eax,1 ; test al,1 ; je 2A56`
  //     if (fabsf(alpha - 1.0f) < 0.0001f) {                       268F..26AA `jp 2704`
  // 00AA26C0  for (...)  ApplyPartialAnimation(keyframes[bone_i], *PeekSequence(idx), animation->IsBoneEnabled(idx), frame, fraction);
  //             idx = BoneIndicesBuffer[bone_i]; BoneMask(+68h)==0 || BoneMask[idx] -> GetKeyframeInterpolate(keyframes[bone_i],...) call at 26E6
  //       -> 2A56 (jmp 26FF)
  //     } else {
  // 00AA2715  for (...)  ApplyPartialAnimationAlpha(keyframes[bone_i], *PeekSequence(idx), IsBoneEnabled(idx), frame, fraction, alpha);
  //             disabled -> 2A46; GetKeyframeInterpolate(partial +120h) call at 2744; BlendWith inline 2749..2A43:
  //             SetSlerp (dot order Y,Z,W,X 2749..277D; flip 2783..27A2; >0.05 test 27A6..27BB else 28F5; ArcCos 27C1..2813; sqrt 2825;
  //             Sins 282B..28F3), rotation -> +60h..+6Ch copied to keyframe (2969..297C);
  //             Position = Position.Interpolate(partial.Position, alpha) 2981..2A43 (temps +198h.., result +D0h..+DCh)
  //     }
  //   }
  // 00AA2A56  ~animation (release; then Data = Info = 0 at 2A80..2A86)
  // }
  // 00AA2AA0  if (start_blend.HasSkeletalMorph()) {              (Data null -> 2D2E; Data->SkeletalMorph.Data(+18h) null -> 2D2E)
  //   C3DVector* scales = MemoryPool.AllocateType<C3DVector>(count);           call 0x00A5C910 at 2AE3 (ebx)
  //   CCountedPointer<const C3DSkeletalMorphTarget> target = start_blend.PeekSkeletalMorph().PeekCombinedMorph();   call 0x00A45260 at 2AFC (-> [esp+28h])
  // 00AA2B10  for (i = 0; i < count; ++i)
  //     target->GetBoneScaling(scales[i], mesh.GetBone(BoneIndicesBuffer[i])->GetNameCRC());   call 0x00A44CD0 at 2B31 (this/mesh re-read each iteration)
  // 00AA2B42  for (i = 1; i < count; ++i)                          unrolled x4 2B70..2C86, remainder 2CB0..2CF1
  //     keyframes[i - 1].SetPosition(keyframes[i - 1].GetPosition().GetScaled(scales[i]));   (W stored 0 first; X*sx, Y*sy, Z*sz with
  //       Y/Z staged through +64h/+68h by integer moves = the C4DVector->C3DVector GetPosition copy)
  // 00AA2CF3  scale = scales[0];
  // 00AA2D06  ~target (release, no zeroing) ; jmp 2D42
  // } else {
  // 00AA2D2E  scale = C3DVector(1.0f, 1.0f, 1.0f);   (ecx = 3F800000h x3; edi = keyframes loaded here too)
  // }
  // 00AA2D42  __declspec(align(16)) CMatrix4x4 A (+E0h), B (+1B0h); CMatrix4x4* result = &A (esi); CMatrix4x4* temp = &B (ebx);
  //           keyframes[0].ToMatrix(*result, false);           inline 2D42..2EDF (norm 2D68..2D7E; s = norm==0 ? 1.0f : 2.0f/norm 2D8A..2DA9;
  //                                                              rows 0..2 W=0, row3 = (Pos, 1.0f) at +110h..+11Ch)
  //           if (count > 1)  (2EE6 `jbe 327E`)
  // 00AA2F00  for (i = 1; i < count; ++i) {                    (counter [esp+70h] = count-1, ecx = &keyframes[i] + 0Ch)
  //   __declspec(align(16)) CMatrix4x4 local (+90h);  keyframes[i].ToMatrix(local, false);   2F00..30A1
  //   SWAP(result, temp);                                       2F26..2F2C `mov edx,esi ; mov esi,ebx ; mov ebx,edx` (scheduled inside ToMatrix)
  //   result->AlignedSetMultiply(*temp, local);                 g_SSEAvailable test 2FED -> SSE 30A9..31AD (dst=result, a=temp, b=&local),
  //                                                             else C arm 31B2..3266 (4 rows; col 3 folds to (p0+p2+p1)*0.0f + p3)
  // }                                                           3278 `jne 2F00`
  // 00AA327E  result->CopyTo(transform);                         12 dword copies (row r cols 0..2 -> transform[3r..3r+2]), interleaved
  // 00AA32BE  MemoryPool.FreeAll();                              call 0x00A5C720 at 32CE
  // 00AA32D3  epilogue
}
```

## Notes for the author
- Cold blocks: none after the epilogue; no jump tables. The only out-of-order branch target is the early `return` 0x00AA32D3.
- The per-bone loop bodies (0x00AA2319..2A43) are instruction-identical to the twin 0x00AA0FA0's (1169..1887) and the
  SetSlerpFromIdentity / BlendWith expansions to 0x00AA0090's (030E..0493, 0601..07C8): copy the agent3022 helper block unchanged.
- Partial loops: the twin and this row read BoneMask then index; IsBoneEnabled(idx) with idx = BoneIndicesBuffer[bone_i].
- Unknown/to probe: whether SWAP is written before or after ToMatrix(local) (the 3 movs sit inside the ToMatrix schedule); whether
  the scaling loop is spelled `for (i = 1; ...) keyframes[i-1] ... scales[i]` (retail esi starts at 1, `cmp esi,count`) -- keep i from 1.
