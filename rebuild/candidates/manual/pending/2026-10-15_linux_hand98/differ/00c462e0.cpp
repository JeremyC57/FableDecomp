// _unattributed::CFloatQuad::Interpolate(float u, float v) -- bilinear interpolation over four corner values: lerp(lerp(c2, c3, u), lerp(c0, c1, u), v)
// Grade: DIFFER(ours v retail) -- 40v51: retail loads all four corners, spills the first lerp to an 8-byte frame ('sub esp,8 ; fstp [esp] ; fstp st(0)') and re-reads it twice ('fsub [esp]', 'fadd [esp]'); the lane keeps it on the x87 stack (no frame, fxch)
// Retail: Fable.exe (TLC, VC7.1) @ 0x00C462E0, 51 bytes (`ret 8`). No callees, no callers by call or address (an engine-library
//   body; the manifest's "CTCPhysicsControlled::GetRenderZOffset" is propagated -- CTCPhysicsControlled's real render-Z lerp is
//   slot 79, 0x00725E60, and its +0x00..+0x0c are not four floats). `this` = four consecutive floats.
// Shape: all four corners are read into locals first (four fld), each lerp is `(b - a) * t + a`; the second-row result is
//   spilled to the 8-byte frame (`fstp [esp]`) and re-read twice by the final lerp.
// Leaf: Interpolate
inline float Interp(float to, float from, float t)
{
    return (to - from) * t + from;
}

struct CFloatQuad {
    float m_corner[4];                      // +0x00 .. +0x0c
    float Interpolate(float u, float v) const;
};

float CFloatQuad::Interpolate(float u, float v) const
{
    float c0 = m_corner[0];
    float c1 = m_corner[1];
    float c2 = m_corner[2];
    float c3 = m_corner[3];
    return Interp(Interp(c1, c0, u), Interp(c3, c2, u), v);
}
