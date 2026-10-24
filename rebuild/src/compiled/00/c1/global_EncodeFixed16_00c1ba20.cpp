// _global::EncodeFixed16(float value) -- the sound/sign parameter record's 16-bit fixed-point encoder: `(long)(value * 65535 * 0.0001)`
// Leaf: EncodeFixed16
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00C1BA20, length 24 bytes (+ int3 padding), __stdcall `ret 4`.
// Callee: __ftol2 @ 0x00BFEA70. Constants: 65535.0f @ 0x012B322C, 0.0001f @ 0x0129F0BC.
// Callers: the record's range setters 0x00C1BB80 / 0x00C1BBA0 (`push value ; call ; mov [esi+10h/12h],ax`; ecx is not
// set up for it, so it is not a member). The landed CChickenSign::Init (0x00C1BC90) models the same body as a file-static
// `EncodeFixed16` for its TU; this is the out-of-line retail copy. Shape: `fld value ; fmul 65535 ; fmul 0.0001` -- the
// scaled value is a named local: the one-expression `value * 65535.0f * 0.0001f` folds the two literals into one
// `fmul` (also under /Op).
// Label correction: the catalog's `_Cons_val<allocator<pair<EHeroMorphType,CParticleMorphs::CEntry>>>` is propagated.
long __stdcall EncodeFixed16(float value)
{
    float scaled = value * 65535.0f;
    return (long)(scaled * 0.0001f);
}