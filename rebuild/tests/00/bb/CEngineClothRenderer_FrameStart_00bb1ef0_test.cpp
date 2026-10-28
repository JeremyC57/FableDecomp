// Behaviour test for CEngineClothRenderer::FrameStart (0x00BB1EF0): with the colour-buffer shadow technique (2) pick the colour-buffer shader arrays, else the depth-buffer ones; an empty array yields the zero-size array.
#include <cstdio>
static int g_zero[1];
struct Arr { int* start; int* finish; int* GetAsCArray() { return (finish - start) > 0 ? start : g_zero; } };
struct Renderer { int* Shadow; int* SpotShadow; Arr Colour, SpotColour, Depth, SpotDepth;
    void FrameStart(int technique) {
        if (technique == 2) { Shadow = Colour.GetAsCArray(); SpotShadow = SpotColour.GetAsCArray(); }
        else { Shadow = Depth.GetAsCArray(); SpotShadow = SpotDepth.GetAsCArray(); } } };
int main()
{
    int c[2], sc[2], d[2]; Renderer r;
    r.Colour.start = c; r.Colour.finish = c + 2; r.SpotColour.start = sc; r.SpotColour.finish = sc + 1;
    r.Depth.start = d; r.Depth.finish = d + 2; r.SpotDepth.start = 0; r.SpotDepth.finish = 0;
    r.FrameStart(2); if (r.Shadow != c || r.SpotShadow != sc) { std::printf("00bb1ef0_TEST FAIL\n"); return 1; }
    r.FrameStart(1); if (r.Shadow != d || r.SpotShadow != g_zero) { std::printf("00bb1ef0_TEST FAIL\n"); return 1; }
    std::printf("00bb1ef0_TEST PASS\n");
    return 0;
}