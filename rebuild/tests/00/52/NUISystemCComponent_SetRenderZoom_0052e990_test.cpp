#include <cstdio>
struct C2DVector { float X; float Y; };
struct CComponent {
    C2DVector RenderPosition, RenderZoom;
    void SetRenderZoom(C2DVector& zoom) { RenderZoom = zoom; }
};
int main()
{
    CComponent c;
    C2DVector a = { 1.0f, 1.0f }; c.RenderPosition = a; c.RenderZoom = a;
    C2DVector z = { 0.25f, 4.0f };
    c.SetRenderZoom(z);
    if (c.RenderZoom.X != 0.25f || c.RenderZoom.Y != 4.0f || c.RenderPosition.X != 1.0f) { std::printf("0052e990_TEST FAIL\n"); return 1; }
    std::printf("0052e990_TEST PASS\n");
    return 0;
}