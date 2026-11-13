# OUTLINE 0x00AA0FA0 C3DAnimationManager::CalcBoneTransformation (retail order)

## Frame facts
- `push ebp ; mov ebp,esp ; and esp,-16 ; sub esp,214h ; push ebx ; push esi ; push edi`; args via ebp ([ebp+8] blend_state,
  +0Ch mesh, +10h bone, +14h transform, +18h scale); `ret 14h`. No EH states (no /EHsc), no /GS.
- Saved order ebx, esi, edi; epilogue 0x00AA2103 (also the early-return target of the 64-bit range check).

## Frame map (esp-relative after the three pushes, i.e. zero outstanding pushes)
| slot | local | notes |
|---|---|---|
| 0x10 / 0x14 | float scratch of GFFastArcCos/GFFastSin (y, y-0.5) and SetSlerp cosom | |
| 0x18 | float alpha = state.GetAlphaWeight() | |
| 0x1C | unsigned long temporary (resize's `T()` zero, then push_back's converted parent); later float sign (BlendWith); later ToMatrix d | shared temp slot |
| 0x20 | float sinom/scale0 scratch; later matrix-loop down-counter (count-1) | |
| 0x24 | long frame (ConvertTimeToFrames out) ; later ToMatrix `zs` scratch | |
| 0x28 / 0x2C | CCountedPointer<const C3DAnimation2> animation (Data, Info); reused by CCountedPointer<const C3DSkeletalMorphTarget> morph | dtor = inline release |
| 0x30 | unsigned long count = BoneIndicesBuffer.size() | re-read as every loop bound |
| 0x34 | C3DKeyframe* keyframes | |
| 0x38 | byte offset i*0x14 into CombinationBlends (strength-reduced) ; later ToMatrix xs | |
| 0x3C | fistp temp; later SSE homed `pa` | |
| 0x40 | this | |
| 0x44 | omega | |
| 0x48 / 0x4C / 0x80 / 0x88 / 0x8C | GFFloatToLongNear fistp ints | |
| 0x50..0x5C | CQuaternion rotation (SetSlerpFromIdentity result, PostApplyRelativeAnimationAlpha); later translation copies x,y,z for the loop matrix | |
| 0x60..0x6C | slerp result X..W (BlendWith) ; 0x58/0x64/0x68 position temps in the scaling loop | |
| 0x70 | cosom (abs) ; 0x84 sign | |
| 0x74 | float fraction | |
| 0x7B | __true_type tag temporary (push_back overflow) | |
| 0x7C | unsigned long i (combination blend index) | |
| 0x90..0xCF | `__declspec(align(16)) CMatrix4x4 parent` (keyframes[i].ToMatrix) | |
| 0xD0..0xDC | C4DVector Interpolate result (BlendWith) | |
| 0xE0..0x11F | `__declspec(align(16)) CMatrix4x4` matrix A (initial `current`) | |
| 0x120..0x14F | C3DKeyframe partial (ApplyPartialAnimationAlpha) | |
| 0x150..0x17F | C3DKeyframe relative (ApplyRelativeAnimationAlpha) | |
| 0x188/0x18C, 0x198..0x1AC | C4DVector temporaries (other_position*alpha; Interpolate's difference*t) | |
| 0x1B0..0x1EF | `__declspec(align(16)) CMatrix4x4` matrix B (initial `other`) | |
| 0x1F0..0x21F | C3DKeyframe relative (ApplyRelativeAnimation, alpha==1) | |

## Body (pseudo-C++, retail order)
```
// 0x00AA0FA0 prologue
BoneIndicesBuffer.resize(1);                       // 0x00AA0FAF..C7: push &tmp(0x1C)=0, push 1, ecx=&this->BoneIndicesBuffer, call 0x00464931
BoneIndicesBuffer[0] = bone;                       // 0x00AA0FCC (mov ecx,[edi] ; mov [ecx],eax)
long parent = mesh.GetBone(bone)->GetParent();     // 0x00AA0FD6 (Bones +0x9C, *0x3C, +4); ebx=4 hoisted for ++_M_finish
while (parent >= 0) {                              // 0x00AA0FE3 jl 0x00AA1034; loop head 0x00AA0FF0
    BoneIndicesBuffer.push_back(parent);           // inline STLport push_back; value stored to tmp 0x1C first (unsigned long temp);
                                                   //   finish!=eos: construct+`add [edi+4],ebx`; else 0x00AA1007 call 0x00464ACE(finish,&tmp,&tag,1,1)
    parent = mesh.GetBone(parent)->GetParent();    // 0x00AA1020
}
unsigned long count = BoneIndicesBuffer.size();    // 0x00AA1034 -> [0x30]
__int64 keyframe_count = count * 5;                // 0x00AA1044 lea ecx,[ebx+ebx*4]; eax=0 high
if (keyframe_count < 0 || keyframe_count > 0x7fffffff) return;   // 0x00AA1047..0x00AA1067 -> 0x00AA2103
C3DKeyframe* keyframes = MemoryPool.AllocateType<C3DKeyframe>((unsigned long)keyframe_count);  // 0x00AA106D..0x00AA1098 (mul 0x30 check; call 0x00A5C910; else 0)
CombineBaseAnimationsBoneList(blend_state.PeekBaseAnimation(), keyframes, count, 0, mesh, BoneIndicesBuffer);  // 0x00AA10B5 call 0x00AA0C50
for (unsigned long i = 0; i < blend_state.GetCombinationBlendCount(); ++i) {      // 0x00AA10BA init; head 0x00AA10C0 (Data null -> exit 0x00AA18DD; /20 via 0x66666667)
    const C3DAnimationStateAlpha& state = blend_state.PeekCombinationBlend(i);  // esi = begin + [0x38]
    const C3DAnimationInfo* info = MeshBank->PeekAnimation(state.GetAnimationIndex());  // 0x00AA1106 call 0x00A242C0
    CCountedPointer<const C3DAnimation2> animation = info->GetAnimation(mesh);       // 0x00AA1116 call 0x00A26C60 (hidden ret = &0x28)
    long frame; float fraction;
    animation->ConvertTimeToFrames(state.GetAnimationTime(), frame, fraction);        // 0x00AA112D call 0x00A52650
    float alpha = state.GetAlphaWeight();          // 0x00AA1132 -> [0x18]
    if (animation->IsRelative()) {                 // 0x00AA1140 shr ecx,2 ; test cl,1 ; je 0x00AA14CD
        if (alpha == 1.0f) {                       // 0x00AA114E fucompp ; jp 0x00AA1207
            for (j = 0; j < count; ++j)            // 0x00AA1167 (count==0 -> 0x00AA1896); ebx=fraction hoisted; esi=&keyframes[0]+0x18
                ApplyRelativeAnimation(keyframes[j], *animation->PeekSequence(BoneIndicesBuffer[j]), frame, fraction);
                // body 0x00AA1180: IsIdentity `js` skip; GetKeyframeInterpolate(rel@0x1F0) call 0x00A4C1F0;
                //   keyframe.Rotation *= rel.Rotation (call 0x00A88C10, ecx=&keyframe); Position += rel.Position (4 fadd)
            // 0x00AA1200 mov ebx,eax (count); jmp 0x00AA1896
        } else {
            for (j ...)                            // 0x00AA1207 / loop 0x00AA1216 (fraction reloaded from [0x74])
                ApplyRelativeAnimationAlpha(keyframes[j], seq, frame, fraction, alpha);
                // 0x00AA1247 GetKeyframeInterpolate(rel@0x150); SetSlerpFromIdentity 0x00AA124C..0x00AA1451 (cosom=rel.W, sign 0x84,
                //   0.05 test 0x00AA1298, GFFastArcCos 0x00AA12A9, sqrt 0x00AA1316, GFFastSin x2, else 0x00AA13F8);
                //   rotation@0x50; Rotation *= rotation (0x00AA1453 call 0x00A88C10); Position += other_position*alpha (0x00AA1458..B9)
            // 0x00AA14C8 jmp 0x00AA1896
        }
    } else if (animation->IsPartial()) {           // 0x00AA14CD shr eax,1 ; test al,1 ; je 0x00AA1896
        if (alpha == 1.0f) {                       // 0x00AA14D7 ; jp 0x00AA1542
            for (j ...)                            // 0x00AA14EA, loop 0x00AA1500 (edi=&keyframes[j], ebx=fraction)
                ApplyPartialAnimation(keyframes[j], seq, animation->IsBoneEnabled(bone), frame, fraction);
                // BoneMask +0x68 null or mask[bone]!=0 -> GetKeyframeInterpolate(keyframes[j]) 0x00AA1526
            // 0x00AA153B mov ebx,eax ; jmp 0x00AA1896
        } else {
            for (j ...)                            // 0x00AA1542, loop 0x00AA1553
                ApplyPartialAnimationAlpha(keyframes[j], seq, enabled, frame, fraction, alpha);
                // GetKeyframeInterpolate(partial@0x120) 0x00AA1588; BlendWith expanded: Dot Y,Z,W,X 0x00AA158D..C1 (needs the
                //   named `C3DKeyframe& keyframe = keyframes[j];` of 0x00A9F540), sign@0x1C, SetSlerp 0x00AA15EA..0x00AA17C3
                //   (stores via 0x60..0x6C then int moves to keyframe.Rotation), Position = Position.Interpolate(other, t)
                //   0x00AA17C5..0x00AA1887 (temp 0xD0..0xDC)
        }
    }
    // 0x00AA1896: ~CCountedPointer animation (dec RefCount, DeleteFunc(Data) call [eax+4], operator delete 0x00BFE9BC), then
    //   Data=Info=0 (0x00AA18C6/CA), ++i, offset += 0x14, jmp 0x00AA10C0
}
// 0x00AA18DD
if (blend_state.HasSkeletalMorph()) {              // Data != 0 && Data->SkeletalMorph != 0 (else 0x00AA1B61)
    C3DVector* scales = MemoryPool.AllocateType<C3DVector>(count);   // 0x00AA18F3..0x00AA1924 (mul 0xC; ebx=scales or 0)
    CCountedPointer<const C3DSkeletalMorphTarget> morph = blend_state.PeekSkeletalMorph().PeekCombinedMorph();  // 0x00AA1934 call 0x00A45260 (&0x28)
    for (unsigned long j = 0; j < count; ++j)      // 0x00AA193D, loop 0x00AA1945
        morph->GetBoneScaling(scales[j], mesh.GetBone(BoneIndicesBuffer[j])->GetNameCRC());   // 0x00AA1966 call 0x00A44CD0
    for (unsigned long j = 1; j < count; ++j)      // 0x00AA1977; compiler x4 unroll 0x00AA198C..0x00AA1ABA, remainder 0x00AA1AC4..0x00AA1B24
        keyframes[j - 1].SetPosition(keyframes[j - 1].GetPosition().GetScaled(scales[j]));
        // per element: y,z copied to 0x64/0x68 (GetPosition by value), products x*sx,y*sy,z*sz; W=0 stored first, z via 0x58,
        // then x,y -> SetPosition = Position = C4DVector(v, 0.0f)   (HYPOTHESIS: names from FableWin SetPosition/GetScaled; verify)
    scale = scales[0];                             // 0x00AA1B26..0x00AA1B36 (3 dword copies to [ebp+18h])
    // 0x00AA1B39: ~morph (release, NO zeroing), jmp 0x00AA1B75
} else {
    scale = C3DVector(1.0f, 1.0f, 1.0f);           // 0x00AA1B61 (ecx=0x3F800000 x3); also edi=keyframes
}
// 0x00AA1B75
__declspec(align(16)) CMatrix4x4 matrix_a;          // 0xE0  (esi)
__declspec(align(16)) CMatrix4x4 matrix_b;          // 0x1B0 (ebx)
CMatrix4x4* current = &matrix_a; CMatrix4x4* other = &matrix_b;
keyframes[0].ToMatrix(*current, false);            // 0x00AA1B75..0x00AA1D12 forced CQuaternion::ToMatrix 4x4 (d@0x1C, s = d!=0 ? 2/d : 1)
for (unsigned long j = 1; j < count; ++j) {        // 0x00AA1D19 jbe (count<=1 -> 0x00AA20AE); counter@0x20=count-1, ecx=&keyframes[1]+0xC
    __declspec(align(16)) CMatrix4x4 parent;       // 0x90
    keyframes[j].ToMatrix(parent, false);          // 0x00AA1D30..0x00AA1ECA (translation via 0x50/0x54/0x58 -> 0xC0..0xC8)
    SWAP(current, other);                          // 0x00AA1D56..5C `mov edx,esi ; mov esi,ebx ; mov ebx,edx` (scheduled into the ToMatrix)
    current->AlignedSetMultiply(*other, parent);   // 0x00AA1E1D g_SSEAvailable test ; je 0x00AA1FE2
        // SSE arm 0x00AA1ED9: pa=other@0x3C, pb=&parent@0x14, pdst=current@0x1C ; GF_SSE_MATRIX_MULTIPLY ; jmp 0x00AA209C
        // x87 arm 0x00AA1FE2: row loop edi=4 (accessor body, ALIGNEDSETMULTIPLY_ACCESSORS); B's 4th column folded (0,0,0,1)
    // 0x00AA209C dec counter ; jne 0x00AA1D30
}
current->CopyTo(transform);                        // 0x00AA20AE..0x00AA20FB (12 dword copies, rows 0..3 cols 0..2)
MemoryPool.FreeAll();                              // 0x00AA20FE call 0x00A5C720
// 0x00AA2103 epilogue, ret 14h
```

## Open points for the author
- Swap placement (before/after ToMatrix) and SWAP vs explicit temp: retail interleaves the swap inside ToMatrix; FableWin has a
  `SWAP<CMatrix4x4*>` COMDAT (fastcall) in this TU -> use SWAP.
- Matrix locals: two separate aligned CMatrix4x4 (0xE0 and 0x1B0 are not adjacent; an array[2] would be contiguous).
- Bone-loop index form: retail re-reads `this->BoneIndicesBuffer._M_start` every iteration as `mov eax,this ; add eax,8 ;
  mov eax,[eax]`; try both CArray operator[] forms (default `_M_start[i]` vs FABLE_RETAIL_CARRAY `*(begin()+i)`).
- resize: STLport's inline resize(n,x) must stay a call (FABLE_RETAIL_STLP_VECTOR_RESIZE_OUTOFLINE, or declaration only).
- The twin row 0x00AA2110 CalcBlendedBoneTransformation (same wave) shares every helper; keep the helper texts identical.
