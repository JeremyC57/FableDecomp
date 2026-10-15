// C4DVector::C4DVector(const CRGBColour& c) -- build a 4-float vector from a BGRA byte colour, each channel times 1/255 (x=R, y=G, z=B, w=A)
// Grade: DIFFER(129v115) -- retail keeps R,G on the FPU stack and spills only B,A through a 16-byte temporary ([esp+8]/[esp+0xc]); every form tried here spills x,y (or all four) to the frame first
// Retail: Fable.exe (TLC, VC7.1) function @ 0x00BB17D0, 115 bytes (`mov eax,ecx` = returns this, `ret 4`). Float pool 0x01231724 = 1/255.
// Label: the catalog's CShaderRenderManager::SetVSVectorAsColour (3 params) does not fit `ret 4`; modelled as a converting constructor.
// Leaf: ??0C4DVector@@QAE@ABUCRGBColour@@@Z
struct CRGBColour { unsigned char B, G, R, A; };
struct C4DVector {
    float x, y, z, w;
    C4DVector() {}
    C4DVector(float x_, float y_, float z_, float w_) : x(x_), y(y_), z(z_), w(w_) {}
    C4DVector(const CRGBColour& c);
};
C4DVector::C4DVector(const CRGBColour& c)
{
    float r = (float)c.R * (1.0f / 255.0f);
    float g = (float)c.G * (1.0f / 255.0f);
    float b = (float)c.B * (1.0f / 255.0f);
    float a = (float)c.A * (1.0f / 255.0f);
    *this = C4DVector(r, g, b, a);
}
