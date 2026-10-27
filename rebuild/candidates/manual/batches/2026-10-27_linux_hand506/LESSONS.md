# Chunk 506 lessons

## GOTCHAS one-liners
- strmbase rows: paste the landed amfilter/renbase model verbatim (+/EHsc) and write the SDK body -- all nine matched on the first grade.
- VC7.1 reassociates `x*C/y` into `(x/y)*C`; put the multiply in its own statement to keep retail's operand order.
- `lea` base/index order in an align-up (`(v + a - 1) & ~(a - 1)`) follows an inline AlignUp(value, alignment) helper, not the open-coded expression.
- A `for(;;)` with the `got <= 0` test inside reproduces retail's break threading in ov_read pump loops (COggVorbisStream::Read).
- VC7.1 out-of-lines the second CAutoLock ctor in a function that takes two locks, exactly as retail (CRendererInputPin::Receive); keep CAutoLock inline in the model.
- Xiph statics compiled with LTCG take private register arguments (esi/ebx/eax) that no VC7.1 declaration can express: park as `lane gap: LTCG` after one grade.
- A gap-scan row whose span holds several int3-separated thunks is not one function: SKIP it, do not author the first thunk under the span's length.
- `pow(float, int)` is ambiguous (C2666) under VC7.1 headers; cast the exponent to `(double)`.

## Review at landing
- 0x00C6B5D0: rename the landed caller 0x00C546E0's `ResetDecoder(void*)` declaration to `ConvertInt128BlockToFloatInPlace_00c6b5d0(float*)` (one address = one name).
- 0x00C62920: landed CVtable_012bf014 ctor takes `long`; retail passes float 800.0f -- fix the landed declaration.
- Landed 0x00CAB370 / 0x00CAC300 sources contain `volatile`; 0x00CAB370's catalog name is a propagated particle label.
- 00c74ab0 next idea: convert the sample rate before the pow line (scheduling only).
