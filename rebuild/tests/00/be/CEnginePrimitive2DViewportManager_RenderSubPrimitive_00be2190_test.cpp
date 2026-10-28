// Behaviour test for CEnginePrimitive2DViewportManager::RenderSubPrimitive (0x00BE2190): the viewport's alpha mask is current while it renders, then reset (clip box kept).
#include <cstdio>
struct V4 { float X, Y, Z, W; };
struct Box { float TLX, TLY, BRX, BRY; };
struct Mask { void* Bank; unsigned int Index; V4 Factors; Box Clip; bool Outside; bool Filter;
    void Reset() { Bank = 0; Index = 0; V4 z = { 0, 0, 0, 0 }; Factors = z; Outside = false; Filter = false; } };
struct Manager;
struct Viewport { Mask AlphaMaskInfo; Manager* seen; unsigned int indexDuringRender; void Render(); };
struct Manager { Mask AlphaMaskInfo;
    void RenderSubPrimitive(Viewport& v) { AlphaMaskInfo = v.AlphaMaskInfo; v.seen = this; v.Render(); AlphaMaskInfo.Reset(); } };
void Viewport::Render() { indexDuringRender = seen->AlphaMaskInfo.Index; }
int main()
{
    int bank; Viewport v; v.AlphaMaskInfo.Bank = &bank; v.AlphaMaskInfo.Index = 5; V4 f = { 1, 2, 3, 4 }; v.AlphaMaskInfo.Factors = f;
    Box c = { 9, 8, 7, 6 }; v.AlphaMaskInfo.Clip = c; v.AlphaMaskInfo.Outside = true; v.AlphaMaskInfo.Filter = true;
    Manager m; m.RenderSubPrimitive(v);
    if (v.indexDuringRender != 5 || m.AlphaMaskInfo.Bank != 0 || m.AlphaMaskInfo.Index != 0 || m.AlphaMaskInfo.Factors.W != 0.0f ||
        m.AlphaMaskInfo.Outside || m.AlphaMaskInfo.Filter || m.AlphaMaskInfo.Clip.TLX != 9.0f) { std::printf("00be2190_TEST FAIL\n"); return 1; }
    std::printf("00be2190_TEST PASS\n");
    return 0;
}