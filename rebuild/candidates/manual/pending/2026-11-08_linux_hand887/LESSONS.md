- (chunk 887) A local VC7.1 packs into a dead incoming-argument home ([esp+0x80] after the pushes) costs disp32 on every access; in
  0x00BB0970 that is the whole +67-byte length gap. Retail keeps the same local at local 0 with an 8-byte-larger frame. `const`,
  declaration order, an if/assign spelling, a pointer instead of the reference, and same-TU hidden-result callee bodies all left it in
  the argument home.
- (chunk 887) `bank->PeekGraphic(i) ? &bank->PeekGraphic(i)->Frames[f] : 0` written straight in the caller compiles the second call as
  branches. Put it in the PDB's inline `CGraphicDataBank::PeekGraphicInfoFrame(index, frame)` and it gives retail's `cmp ; sbb ; and`
  select on the first call's pointer.
- (chunk 887) The UV-offset lerp `(End - Start) * factor + Start` is C2DVector operator chains: the Y difference is spilled before X is
  multiplied. Explicit X/Y float statements do not reproduce that order. Four-element `+=` loops stay rolled, so write the eight adds out.
- (chunk 887) Inline GetFrame 0x0041C260 with mipmap 0: the `out = Data->Frame` assignment needs no null re-test in the CGraphicFrame
  destructor. VC7.1 drops the destructor's `if (FrameData)` after the deref, and the failure return carries no destructor.
- (chunk 887) RenderClockPrim opens an ebp frame with `lea ebp,[esp-0x6c]` because of the inline-asm GFFloatToLongCeil. Our
  `mov ebp,esp ; sub esp,0x7c` shows that the frame size, not the frame kind, is the open residue.
