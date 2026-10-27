// libvorbis::_dist(int el, float* ref, float* b, int step) -- libvorbis sharedbook.c: squared Euclidean distance between an el-entry reference vector and a strided vector (sum of (ref[i] - b[i*step])^2)
// Grade: DIFFER(259v253) -- LTCG private register convention: retail takes step in ESI (never loaded), el/ref/b on the stack with ret 0Ch; ours loads step from a parameter (lane gap: LTCG)
// Leaf: @_dist@16
// Retail: Fable.exe (TLC, VC7.1) @ 0x00C95910, 253 bytes, `ret 0Ch`, the float result in st(0). libpk's statically linked Xiph
// libvorbis 20030909 (lib/sharedbook.c). The LTCG link gave this static a private register convention: el, ref and b on the stack
// and step in ESI (never loaded in the body) -- a convention VC7.1 cannot be asked for. VC7.1's own unroll by four of the loop
// shows in the body (four strided pointers kept in frame slots).
// Identity: sharedbook.c _dist (the acc of squared differences over b[i*step]); its caller is sharedbook.c _best. Catalogued as
// "Audio_ComputeL2Norm" (descriptive).
extern "C" float __fastcall _dist(int el, float* ref, float* b, int step)
{
    int i;
    float acc = 0.f;
    for (i = 0; i < el; i++) {
        float val = (ref[i] - b[i * step]);
        acc += val * val;
    }
    return (acc);
}
