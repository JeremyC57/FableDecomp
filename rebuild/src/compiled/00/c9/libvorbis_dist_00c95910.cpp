// libvorbis::_dist(int el, float* ref, float* b, int step) -- libvorbis sharedbook.c: squared Euclidean distance between an el-entry reference vector and a strided vector (sum of (ref[i] - b[i*step])^2)
// Shape (retry1): parked as an "LTCG custom register convention" gap; inside `extern "C" { static __declspec(noinline) }` with a same-TU caller VC7.1 /O2 gives the libvorbis static retail's private register convention itself (chunk 585/586/593 lever).
// Leaf: __dist
// Retail: Fable.exe (TLC, VC7.1) @ 0x00C95910, 253 bytes, `ret 0Ch`, the float result in st(0). libpk's statically linked Xiph
// libvorbis 20030909 (lib/sharedbook.c). Retail convention: el, ref and b on the stack
// and step in ESI (never loaded in the body); VC7.1 /O2 picks it itself for the same-TU static. VC7.1's own unroll by four of the loop
// shows in the body (four strided pointers kept in frame slots).
// Identity: sharedbook.c _dist (the acc of squared differences over b[i*step]); its caller is sharedbook.c _best. Catalogued as
// "Audio_ComputeL2Norm" (descriptive).
extern "C" {
static __declspec(noinline) float __stdcall _dist(int el, float* ref, float* b, int step)
{
    int i;
    float acc = 0.f;
    for (i = 0; i < el; i++) {
        float val = (ref[i] - b[i * step]);
        acc += val * val;
    }
    return (acc);
}

// Same-TU caller (the _dist calls of sharedbook.c _best), present so the static gets retail's private
// convention. Not graded.
float __cdecl _dist_caller(int el, float* ref, float* b, int step) { return _dist(el, ref, b, step); }
}