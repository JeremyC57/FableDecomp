// Behaviour test for 0x00819320 fill<CTextureManagerPoolInfo> (model copy).
#include <cstdio>
class CPixelFormat { public: int FormatIndex; };
class CTextureManagerPoolInfo { public:
    unsigned int Width, Height; CPixelFormat PixelFormat; bool UseMipmaps; unsigned int Count;
};
static void fill_model(CTextureManagerPoolInfo* first, CTextureManagerPoolInfo* last, const CTextureManagerPoolInfo& val)
{
    for (; first != last; ++first) *first = val;
}
int main()
{
    CTextureManagerPoolInfo v[5];
    int i;
    for (i = 0; i < 5; ++i) { v[i].Width = 1; v[i].Height = 2; v[i].PixelFormat.FormatIndex = 3; v[i].UseMipmaps = false; v[i].Count = 4; }
    CTextureManagerPoolInfo x; x.Width = 256; x.Height = 128; x.PixelFormat.FormatIndex = 21; x.UseMipmaps = true; x.Count = 9;
    fill_model(v + 1, v + 4, x);
    int ok = 1;
    for (i = 1; i < 4; ++i) if (v[i].Width != 256 || v[i].Height != 128 || v[i].PixelFormat.FormatIndex != 21 || !v[i].UseMipmaps || v[i].Count != 9) ok = 0;
    if (v[0].Width != 1 || v[4].Count != 4 || v[4].UseMipmaps) ok = 0;
    fill_model(v, v, x);
    if (v[0].Width != 1) ok = 0;
    if (!ok) { std::printf("00819320_TEST FAIL\n"); return 1; }
    std::printf("00819320_TEST PASS\n");
    return 0;
}