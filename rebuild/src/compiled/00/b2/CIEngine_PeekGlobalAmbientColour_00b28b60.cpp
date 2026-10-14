// CIEngine::PeekGlobalAmbientColour() -- the global ambient colour: opaque white (1, 1, 1, 1) built in the hidden return record
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00B28B60, length 23 bytes.
// Shape: one `mov ecx,3F800000h` feeding four stores through the hidden pointer, `ret 4`.
struct CRGBAColour_00b28b60 {
    float R, G, B, A;
    CRGBAColour_00b28b60(float r, float g, float b, float a) : R(r), G(g), B(b), A(a) { }
};
struct CIEngine {
    CRGBAColour_00b28b60 PeekGlobalAmbientColour();
};

CRGBAColour_00b28b60 CIEngine::PeekGlobalAmbientColour()
{
    return CRGBAColour_00b28b60(1.0f, 1.0f, 1.0f, 1.0f);
}